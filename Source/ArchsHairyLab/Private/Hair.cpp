// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "Hair.h"

#include "HairLayer.h"
#include "HairSegment.h"
#include "HairNode.h"
#include "MyPlayerController.h"
#include "Head.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshData.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Core/Public/Misc/CoreMisc.h"
#include "Runtime/Core/Public/HAL/PlatformFilemanager.h"

AHair::AHair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create default layer
	AddNewLayer();

	SetupMesh();
}

// Called when the game starts or when spawned
void AHair::BeginPlay()
{
	Super::BeginPlay();
}

void AHair::AddNewLayer()
{
	AHairLayer* NewLayer = NewObject<AHairLayer>();
	NewLayer->Name = FString("New Layer");
	NewLayer->Id = LayerId;
	LayerId += 1;
	HairLayers.Add(NewLayer);
}

void AHair::SetupMesh()
{
	StartMeshData = NewObject<UProceduralMeshData>();
	MiddleMeshData = NewObject<UProceduralMeshData>();
	EndMeshData = NewObject<UProceduralMeshData>();

	// Create default mesh data for start, middle, end components
	StartMeshData->Vertices.Add(FVector(-10.0f, 0.0f, 50.0f));
	StartMeshData->Vertices.Add(FVector(10.0f, 0.0f, 50.0f));
	StartMeshData->Vertices.Add(FVector(-25.0f, 0.0f, 0.0f));
	StartMeshData->Vertices.Add(FVector(25.0f, 0.0f, 0.0f));
	StartMeshData->Triangles.Add(0);
	StartMeshData->Triangles.Add(1);
	StartMeshData->Triangles.Add(2);
	StartMeshData->Triangles.Add(1);
	StartMeshData->Triangles.Add(2);
	StartMeshData->Triangles.Add(3);
	StartMeshData->UVs.Add(FVector2D(0.0f, 1.0f));
	StartMeshData->UVs.Add(FVector2D(1.0f, 1.0f));
	StartMeshData->UVs.Add(FVector2D(0.0f, 0.0f));
	StartMeshData->UVs.Add(FVector2D(1.0f, 0.0f));

	MiddleMeshData->Vertices.Add(FVector(-25.0f, 0.0f, 50.0f));
	MiddleMeshData->Vertices.Add(FVector(25.0f, 0.0f, 50.0f));
	MiddleMeshData->Vertices.Add(FVector(-25.0f, 0.0f, 0.0f));
	MiddleMeshData->Vertices.Add(FVector(25.0f, 0.0f, 0.0f));
	MiddleMeshData->Triangles.Add(0);
	MiddleMeshData->Triangles.Add(1);
	MiddleMeshData->Triangles.Add(2);
	MiddleMeshData->Triangles.Add(3);
	MiddleMeshData->Triangles.Add(2);
	MiddleMeshData->Triangles.Add(1);
	MiddleMeshData->UVs.Add(FVector2D(0.0f, 1.0f));
	MiddleMeshData->UVs.Add(FVector2D(1.0f, 1.0f));
	MiddleMeshData->UVs.Add(FVector2D(0.0f, 0.0f));
	MiddleMeshData->UVs.Add(FVector2D(1.0f, 0.0f));

	EndMeshData->Vertices.Add(FVector(-25.0f, 0.0f, 50.0f));
	EndMeshData->Vertices.Add(FVector(25.0f, 0.0f, 50.0f));
	EndMeshData->Vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	EndMeshData->Triangles.Add(0);
	EndMeshData->Triangles.Add(1);
	EndMeshData->Triangles.Add(2);
	EndMeshData->UVs.Add(FVector2D(0.0f, 1.0f));
	EndMeshData->UVs.Add(FVector2D(1.0f, 1.0f));
	EndMeshData->UVs.Add(FVector2D(0.5f, 0.0f));

	CalculateEndPoints(MiddleMeshData->Vertices);
}

AHairSegment* AHair::SpawnSegment()
{
	UWorld* const World = GetWorld();
	if (!World) return NULL;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller) return NULL;

	// Set controller cursor hit result
	Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Controller->HitResult);

	// Check hit result has actor
	if (!Controller->HitResult.Actor.IsValid()) return NULL;
	// Only proceed if hit head mesh
	if (!Controller->HitResult.Actor->IsA(AHead::StaticClass())) return NULL;

	// Spawn segment object
	FActorSpawnParameters SpawnParams;
	Controller->TargetSegment = World->SpawnActor<AHairSegment>(AHairSegment::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f), SpawnParams);
	// Setup new segment
	Controller->TargetSegment->AddSplinePoint(Controller->HitResult.Location);
	Controller->TargetSegment->Normals.Add(Controller->HitResult.Normal);

	// Set IsExtending
	IsExtending = true;

	SpawnNode(Controller, World, Controller->HitResult.Location);
	UpdateSegment(Controller->TargetSegment);

	// Add hair segment to current layer
	if (!Controller->TargetLayer) return Controller->TargetSegment;

	Controller->TargetLayer->Segments.Add(Controller->TargetSegment);

	return Controller->TargetSegment;
}


void AHair::ExtendSegment()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller || !Controller->TargetSegment) return;

	// Set controller cursor hit result
	Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Controller->HitResult);

	Controller->TargetSegment->AddSplinePoint(Controller->HitResult.Location + Controller->HitResult.Normal*25.0f);
	Controller->TargetSegment->Normals.Add(Controller->HitResult.Normal);

	SpawnNode(Controller, World, Controller->HitResult.Location + Controller->HitResult.Normal*25.0f);
	UpdateSegment(Controller->TargetSegment);
}

AHairNode* AHair::SpawnNode(AMyPlayerController* Controller, UWorld* World, FVector Location)
{
	if (!Controller || !World) return NULL;

	// Spawn node object
	FActorSpawnParameters SpawnParams;
	Controller->TargetNode = World->SpawnActor<AHairNode>(AHairNode::StaticClass(), Location, FRotator(0.0f), SpawnParams);
	if (Controller->TargetNode)
	{
		// Resize node object
		Controller->TargetNode->SetActorScale3D(FVector(0.03f, 0.03f, 0.03f));
		FRotator Rot = (Controller->HitResult.Location - (Controller->HitResult.Location+Controller->HitResult.Normal)).Rotation();
		Controller->TargetNode->SetActorRotation(Rot);
		if (Controller->TargetSegment)
		{
			// Add node to list for segment
			Controller->TargetSegment->Nodes.Add(Controller->TargetNode);
			// Assign segment reference and index for node
			Controller->TargetNode->Segment = Controller->TargetSegment;
			Controller->TargetNode->Index = Controller->TargetSegment->Nodes.Num()-1;
		}
	}
	return Controller->TargetNode;
}

void AHair::UpdateHair()
{
	for (int i = 0; i < HairLayers.Num(); i++)
	{
		UpdateLayer(HairLayers[i]);
	}
}

void AHair::UpdateLayer(AHairLayer* InLayer)
{
	if (!InLayer) return;

	for (int i = 0; i < InLayer->Segments.Num(); i++)
	{
		UpdateSegment(InLayer->Segments[i]);
	}
}

void AHair::UpdateSegment(AHairSegment* InSegment)
{
	if (!InSegment) return;
	InSegment->ProceduralMesh->ClearAllMeshSections();
	ClearMeshData();

	//Populate new data
	if (!MiddleMeshData) return;
	for (int i = 1; i <= InSegment->Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		AssignPositions(InSegment->Spline->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local), InSegment->Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local));
		float Distance = InSegment->Spline->GetDistanceAlongSplineAtSplinePoint(i);
		float TotalDistance = InSegment->Spline->GetDistanceAlongSplineAtSplinePoint(InSegment->Spline->GetNumberOfSplinePoints()-1);
		Weight = 1.0f - Distance / TotalDistance;

		if (i == 1)
		{
			Weight = 1.0f;
			AddVertices(0, MiddleMeshData->Vertices, InSegment->Spline->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::Local), InSegment->Normals[i]);
			AddTriangles();
			AddUVs(true);
		}
		else
		{
			AddVertices(2, MiddleMeshData->Vertices, InSegment->Spline->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::Local), InSegment->Normals[i]);
			AddTriangles();
			AddUVs(false);
		}
	}

	// Create mesh 
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller || !Controller->TargetSegment) return;
	//InSegment->ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	InSegment->ProceduralMesh->CreateMeshSection(0, Controller->TargetSegment->ProceduralMeshData->Vertices,
													Controller->TargetSegment->ProceduralMeshData->Triangles,
													TArray<FVector>(), 
													Controller->TargetSegment->ProceduralMeshData->UVs,
													TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void AHair::CalculateEndPoints(TArray<FVector> InVertices)
{
	float minLength = FLT_MAX;
	float maxLength = FLT_MIN;
	float minWidth = FLT_MAX;
	float maxWidth = FLT_MIN;
	float minThickness = FLT_MAX;
	float maxThickness = FLT_MIN;
	// First calculate ends
	for (int i = 0; i < InVertices.Num(); i++)
	{
		// Length
		if (InVertices[i].Z > maxLength)
		{
			maxLength = InVertices[i].Z;
			AnchorLengthStart = FVector(0.0f, 0.0f, InVertices[i].Z);
		}
		if (InVertices[i].Z < minLength)
		{
			minLength = InVertices[i].Z;
			AnchorLengthEnd = FVector(0.0f, 0.0f, InVertices[i].Z);
		}
		// Width
		if (InVertices[i].X > maxWidth)
		{
			maxWidth = InVertices[i].X;
			AnchorWidthStart = FVector(InVertices[i].X, 0.0f, 0.0f);
		}
		if (InVertices[i].X < minWidth)
		{
			minWidth = InVertices[i].X;
			AnchorWidthEnd = FVector(InVertices[i].X, 0.0f, 0.0f);
		}
		// Thickness
		if (InVertices[i].Y > maxThickness)
		{
			maxThickness = InVertices[i].Y;
			AnchorThicknessStart = FVector(0.0f, InVertices[i].Y, 0.0f);
		}
		if (InVertices[i].Y < minThickness)
		{
			minThickness = InVertices[i].Y;
			AnchorThicknessEnd = FVector(0.0f, InVertices[i].Y, 0.0f);
		}
	}
	// Get all points that lie on ends
	for (int i = 0; i < InVertices.Num(); i++)
	{
		if (InVertices[i].Z == maxLength)
		{
			EndPointsA.Add(InVertices[i]);
		}
		if (InVertices[i].Z == minLength)
		{
			EndPointsB.Add(InVertices[i]);
		}
	}
}

void AHair::ClearMeshData()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller || !Controller->TargetSegment) return;

	Controller->TargetSegment->ProceduralMeshData->Vertices.Empty();
	Controller->TargetSegment->ProceduralMeshData->Triangles.Empty();
	Controller->TargetSegment->ProceduralMeshData->UVs.Empty();
	Controller->TargetSegment->NumTriangles = 0;
	Controller->TargetSegment->IsUVReversed = true;
}

void AHair::AssignPositions(FVector InP1, FVector InP2)
{
	P1 = InP1;
	P2 = InP2;
}

FVector AHair::MapVertex(FVector V, FVector Direction, FVector Normal, float XWidth, float YWidth, float InWeight)
{
	// Find percentage distance between A to B
	float VDistance = AnchorLengthStart.Z - V.Z;
	float Distance = FVector::Dist(AnchorLengthStart, AnchorLengthEnd);
	float VRatio = VDistance / Distance;
	// Get displacement vector from P1 to P2
	FVector P3 = P2 - P1;
	// From P1, add weighted displacement vector for vertical offset
	FVector V2 = P1 + VRatio*P3;

	// Get X direction
	FVector DirX = FVector::CrossProduct(Direction, Normal);
	// Get X ratio
	float XDistance = AnchorWidthStart.X - V.X;
	float XDisplacement = FVector::Dist(AnchorWidthStart, AnchorWidthEnd);
	float XRatio = 0.0f;
	if (XDisplacement == 0.0f)
		XRatio = 0.0f;
	else
		XRatio = -0.5f + XDistance / XDisplacement;
	// Apply X direction
	V2 = V2 + (DirX*GlobalXWidth*XRatio*InWeight);
	//V2 = V2 + (DirX*V.X*InWeight);

	// Find percentage distance of Y
	float YDistance = AnchorThicknessStart.Y - V.Y;
	float YDisplacement = FVector::Dist(AnchorThicknessStart, AnchorThicknessEnd);
	float YRatio = YDistance / YDisplacement;
	// Get Y direction
	FVector DirY = DirX.RotateAngleAxis(90, FVector(0, 0, 1));
	// Apply Y direction
	V2 = V2 + (DirY*V.Y*InWeight);

	//return V;
	return V2;
}

void AHair::AddVertices(int FirstIndex, TArray<FVector> InVertices, FVector Direction, FVector Normal)
{
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	for (int i = FirstIndex; i <= MiddleMeshData->Vertices.Num() - 1; i++)
	{
		Controller->TargetSegment->ProceduralMeshData->Vertices.Add(MapVertex(InVertices[i], Direction, Normal, 0.0f, 0.0f, Weight));
	}
}

void AHair::AddTriangles()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller || !Controller->TargetSegment) return;

	for (int i = 0; i <= MiddleMeshData->Triangles.Num() - 1; i++)
	{
		Controller->TargetSegment->ProceduralMeshData->Triangles.Add(Controller->TargetSegment->NumTriangles + MiddleMeshData->Triangles[i]);
	}
	Controller->TargetSegment->NumTriangles = Controller->TargetSegment->NumTriangles + MiddleMeshData->Vertices.Num() - 2;
}

void AHair::AddUVs(bool IsFirst)
{
	UWorld* const World = GetWorld();
	if (!World) return;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (IsFirst)
	{
		Controller->TargetSegment->ProceduralMeshData->UVs.Append(MiddleMeshData->UVs);
	}
	else
	{
		if (Controller->TargetSegment->IsUVReversed)
		{
			Controller->TargetSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[0]);
			Controller->TargetSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[1]);
		}
		else
		{
			int N = MiddleMeshData->UVs.Num();
			Controller->TargetSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[N-2]);
			Controller->TargetSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[N-1]);
		}
		Controller->TargetSegment->IsUVReversed = !Controller->TargetSegment->IsUVReversed;
	}
}

//////////////////// FILE MANAGEMENT ////////////////////

void AHair::ExportHair()
{
	FString SaveDirectory = FPaths::GameDir();
	FString FileName = FString("Hair.obj");
	FString SaveText = FString("");
	bool IsOverwriting = true;

	// Parse data to obj format
	int NumVertices = 1; // obj counts from 1
	for (int i = 0; i < HairLayers.Num(); i++)
	{
		for (int j = 0; j < HairLayers[i]->Segments.Num(); j++)
		{
			UProceduralMeshData* MeshData = HairLayers[i]->Segments[j]->ProceduralMeshData;
			for (int k = 0; k < MeshData->Vertices.Num(); k++)
			{
				// Get segment location add local vertex
				FVector Loc = HairLayers[i]->Segments[j]->GetActorLocation();
				Loc += MeshData->Vertices[k];
				SaveText = SaveText + "v " + FString::SanitizeFloat(Loc.X) + " " + FString::SanitizeFloat(Loc.Y) + " " + FString::SanitizeFloat(Loc.Z)+ "\n";
			}

			for (int k = 0; k+3 < MeshData->Triangles.Num(); k+=3)
			{
				// Triangles form faces
				SaveText = SaveText + "f " + FString::FromInt(NumVertices + MeshData->Triangles[k]) + " "
										+ FString::FromInt(NumVertices + MeshData->Triangles[k+1]) + " "
										+ FString::FromInt(NumVertices + MeshData->Triangles[k+2]) + "\n";
			}

			for (int k = 0; k < MeshData->UVs.Num(); k++)
			{
				//SaveText += "U\n";
			}

			// Add numbers at end for faces
			NumVertices += MeshData->Vertices.Num();
		}
	}

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// CreateDirectoryTree returns true if the destination
	// directory existed prior to call or has been created
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		// Get absolute file path
		FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

		// Allow overwriting or file doesn't already exist
		if (IsOverwriting || !PlatformFile.FileExists(*AbsoluteFilePath))
		{
			FFileHelper::SaveStringToFile(SaveText, *AbsoluteFilePath);
			UE_LOG(LogTemp, Warning, TEXT("Exported hair to %s"), *AbsoluteFilePath);
		}
	}
}