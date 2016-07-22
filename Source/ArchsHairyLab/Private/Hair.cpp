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
	ProceduralMeshData = NewObject<UProceduralMeshData>();
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
	MiddleMeshData->Triangles.Add(1);
	MiddleMeshData->Triangles.Add(2);
	MiddleMeshData->Triangles.Add(3);
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
	InSegment->ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void AHair::CalculateEndPoints(TArray<FVector> InVertices)
{
	float min = FLT_MAX;
	float max = FLT_MIN;
	// First calculate ends
	for (int i = 0; i < InVertices.Num(); i++)
	{
		if (InVertices[i].Z > max)
		{
			max = InVertices[i].Z;
			AnchorA = FVector(0.0f, 0.0f, InVertices[i].Z);
		}
		if (InVertices[i].Z < min)
		{
			min = InVertices[i].Z;
			AnchorB = FVector(0.0f, 0.0f, InVertices[i].Z);
		}
	}
	// Get all points that lie on ends
	for (int i = 0; i < InVertices.Num(); i++)
	{
		if (InVertices[i].Z == max)
		{
			EndPointsA.Add(InVertices[i]);
		}
		if (InVertices[i].Z == min)
		{
			EndPointsB.Add(InVertices[i]);
		}
	}
}

void AHair::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
	NumTriangles = 0;
	IsUVReversed = true;
}

void AHair::AssignPositions(FVector InP1, FVector InP2)
{
	P1 = InP1;
	P2 = InP2;
}

FVector AHair::MapVertex(FVector V, FVector Direction, FVector Normal, float XWidth, float YWidth, float InWeight)
{
	// Find percentage distance between A to B
	float VDistance = AnchorA.Z - V.Z;
	float Distance = FVector::Dist(AnchorA, AnchorB);
	float VRatio = VDistance / Distance;
	// Get displacement vector from P1 to P2
	FVector P3 = P2 - P1;
	// From P1, add weighted displacement vector for vertical offset
	FVector V2 = P1 + VRatio*P3;

	//Get X direction
	FVector DirX = FVector::CrossProduct(Direction, Normal);
	// Apply X direction
	V2 = V2 + (DirX*V.X*InWeight);

	// Get Y direction
	FVector DirY = DirX.RotateAngleAxis(90, FVector(0, 0, 1));
	// Apply Y direction
	V2 = V2 + (DirY*V.Y*InWeight);

	//return V;
	return V2;
}

void AHair::AddVertices(int FirstIndex, TArray<FVector> InVertices, FVector Direction, FVector Normal)
{
	for (int i = FirstIndex; i <= MiddleMeshData->Vertices.Num() - 1; i++)
	{
		Vertices.Add(MapVertex(InVertices[i], Direction, Normal, 0.0f, 0.0f, Weight));
	}
}

void AHair::AddTriangles()
{
	for (int i = 0; i <= MiddleMeshData->Triangles.Num() - 1; i++)
	{
		Triangles.Add(NumTriangles + MiddleMeshData->Triangles[i]);
	}
	NumTriangles = NumTriangles + MiddleMeshData->Vertices.Num() - 2;
}

void AHair::AddUVs(bool IsFirst)
{
	if (IsFirst)
	{
		UVs.Append(MiddleMeshData->UVs);
	}
	else
	{
		if (IsUVReversed)
		{
			UVs.Add(MiddleMeshData->UVs[0]);
			UVs.Add(MiddleMeshData->UVs[1]);
		}
		else
		{
			int N = MiddleMeshData->UVs.Num();
			UVs.Add(MiddleMeshData->UVs[N-2]);
			UVs.Add(MiddleMeshData->UVs[N-1]);
		}
		IsUVReversed = !IsUVReversed;
	}
}