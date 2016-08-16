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
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

AHair::AHair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create default layer
	AddNewLayer();

	SetupMesh();
}

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

	// X = Right, Y = Forward, Z = Up
	MiddleMeshData->Vertices.Add(FVector(-25.0f, 50.0f, 0.0f));
	MiddleMeshData->Vertices.Add(FVector(25.0f, 50.0f, 0.0f));
	MiddleMeshData->Vertices.Add(FVector(-25.0f, 0.0f, 0.0f));
	MiddleMeshData->Vertices.Add(FVector(25.0f, 0.0f, 0.0f));
	MiddleMeshData->Triangles.Add(2);
	MiddleMeshData->Triangles.Add(1);
	MiddleMeshData->Triangles.Add(0);
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


//// PROCEDURAL MESH GENERATION ////

AHairSegment* AHair::SpawnSegment()
{
	// Get world and player controllers
	UWorld* const World = GetWorld();
	if (!World) return NULL;
	AMyPlayerController* Controller = GetController();
	if (!Controller) return NULL;

	// Set controller cursor hit result
	Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Controller->HitResult);

	// Check hit result has actor
	if (!Controller->HitResult.Actor.IsValid()) return NULL;
	// Only proceed if hit head mesh
	if (!Controller->HitResult.Actor->IsA(AHead::StaticClass())) return NULL;

	// Spawn segment object
	FActorSpawnParameters SpawnParams;
	DeselectAll();
	SelectSegment(World->SpawnActor<AHairSegment>(AHairSegment::StaticClass(), FVector(0.0f, 0.0f, 0.0f) , FRotator(0.0f), SpawnParams));
	// Setup new segment
	FVector Loc = Controller->HitResult.Location + Controller->HitResult.Normal*10.0f;
	Controller->TargetSegments[0]->AddSplinePoint(Loc);
	Controller->TargetSegments[0]->Normals.Add(Controller->HitResult.Normal);
	Controller->TargetSegments[0]->HairLayer = Controller->TargetLayer;

	// Set IsExtending
	Controller->IsExtending = true;

	SpawnNode(Controller, World, Loc);
	UpdateSegment(Controller->TargetSegments[0]);

	// Add hair segment to current layer
	if (!Controller->TargetLayer) return Controller->TargetSegments[0];

	Controller->TargetLayer->Segments.Add(Controller->TargetSegments[0]);

	return Controller->TargetSegments[0];
}


void AHair::ExtendSegment()
{
	UWorld* const World = GetWorld();
	if (!World) return;
	AMyPlayerController* Controller = GetController();
	if (!Controller || Controller->TargetSegments.Num() == 0) return;

	// Set controller cursor hit result
	Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Controller->HitResult);
		
	// Check hit result has actor
	if (!Controller->HitResult.Actor.IsValid()) return;
	// Only proceed if hit head mesh
	if (!Controller->HitResult.Actor->IsA(AHead::StaticClass())) return;

	Controller->TargetSegments[0]->AddSplinePoint(Controller->HitResult.Location + Controller->HitResult.Normal*25.0f);
	Controller->TargetSegments[0]->Spline->SetUpVectorAtSplinePoint(Controller->TargetSegments[0]->Spline->GetNumberOfSplinePoints()-1,
																	Controller->HitResult.Normal, ESplineCoordinateSpace::World);
	Controller->TargetSegments[0]->Normals.Add(Controller->HitResult.Normal);

	SpawnNode(Controller, World, Controller->HitResult.Location + Controller->HitResult.Normal*25.0f);
	UpdateSegment(Controller->TargetSegments[0]);
}

AHairNode* AHair::SpawnNode(AMyPlayerController* Controller, UWorld* World, FVector Location)
{
	if (!Controller || !World) return NULL;

	// Spawn node object
	FActorSpawnParameters SpawnParams;
	AHairSegment* Segment = Controller->TargetSegments[0];
	int Index = Segment->Spline->GetNumberOfSplinePoints() - 1;
	// Use spline direction and normal to get rotator
	FRotator Rot = Segment->Spline->GetRotationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	AHairNode* Node = World->SpawnActor<AHairNode>(AHairNode::StaticClass(), Location, Rot, SpawnParams);
	if (Node)
	{
		// Resize node object
		Node->SetActorScale3D(FVector(0.04f, 0.04f, 0.04f));
		if (Segment)
		{
			// Assign segment reference and index for node
			Node->Segment = Segment;
			Node->Index = Index;
			// Add node to list for segment
			Segment->Nodes.Add(Node);
		}
	}
	return Node;
}

void AHair::SetNodeLocation(AHairNode* Node, FVector Location, bool IsPropToChildren)
{
	AHairSegment* Segment = Node->Segment;
	if (IsPropToChildren)
	{
		FVector Offset = Location - Node->GetActorLocation();
		for (int i = Node->Index; i < Segment->Nodes.Num(); i++)
		{
			Segment->Nodes[i]->AddActorWorldOffset(Offset);
			Segment->SetSplinePoint(i, Segment->Nodes[i]->GetActorLocation());
		}
	}
	else
	{
		Node->SetActorLocation(Location);
		Segment->SetSplinePoint(Node->Index, Node->GetActorLocation());
	}
	UpdateSegment(Segment);
}

void AHair::SetNodeRotation(AHairNode* Node, FRotator Rotation, bool IsPropToChildren)
{
	AHairSegment* Segment = Node->Segment;
	USplineComponent* Spline = Segment->Spline;

	FVector Up = Spline->GetUpVectorAtSplinePoint(Node->Index, ESplineCoordinateSpace::World);
	FVector UpRotated = Rotation.RotateVector(Up);
	Spline->SetUpVectorAtSplinePoint(Node->Index, UpRotated, ESplineCoordinateSpace::World);

	Node->SetActorRotation(Spline->GetRotationAtSplinePoint(Node->Index, ESplineCoordinateSpace::World));
	UpdateSegment(Segment);
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
	if (!InSegment || InSegment->Spline->GetNumberOfSplinePoints() < 2) return;
	InSegment->ProceduralMesh->ClearAllMeshSections();
	ClearMeshData(InSegment);

	// Update node rotations to match spline
	for (int i = 0; i < InSegment->Spline->GetNumberOfSplinePoints(); i++)
	{
		FRotator Rot = InSegment->Spline->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World);
		InSegment->Nodes[i]->SetActorRotation(Rot);
	}

	// Populate new data
	if (!MiddleMeshData) return;
	for (int i = 0; i <= InSegment->NumSegments; i++)
	{
		float TotalDistance = InSegment->Spline->GetSplineLength();
		float Distance = (TotalDistance / InSegment->NumSegments)*i;
		float NextDistance = (TotalDistance / InSegment->NumSegments)*i+1;
		AssignPositions(InSegment->Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local),
			InSegment->Spline->GetLocationAtDistanceAlongSpline(NextDistance, ESplineCoordinateSpace::Local));

		//Weight = 1.0f - Distance / TotalDistance;
		
		float Displacement = InSegment->FallOff - 1.0f;
		Weight = 1.0f + (Distance / TotalDistance)*Displacement;

		// Interpolate distance to closest spline point index
		float Delta = ((InSegment->Spline->GetNumberOfSplinePoints() - 1)*1.0f) / (InSegment->NumSegments*1.0f);
		int Index = FGenericPlatformMath::RoundToInt(i*Delta);
		if (i == 0)
		{
			AddVertices(0, MiddleMeshData->Vertices, InSegment, Distance);
			AddTriangles(InSegment);
			AddUVs(InSegment, true);
		}
		else
		{
			AddVertices(2, MiddleMeshData->Vertices, InSegment, Distance);
			AddTriangles(InSegment);
			AddUVs(InSegment, false);
		}
	}
	// Create mesh 
	InSegment->ProceduralMesh->CreateMeshSection(0, InSegment->ProceduralMeshData->Vertices,
													InSegment->ProceduralMeshData->Triangles,
													TArray<FVector>(), 
													InSegment->ProceduralMeshData->UVs,
													TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	// Duplicate for outline as custom depth not available for translucent materials
	InSegment->OutlineMesh->CreateMeshSection(0, InSegment->ProceduralMeshData->Vertices,
													InSegment->ProceduralMeshData->Triangles,
													TArray<FVector>(),
													InSegment->ProceduralMeshData->UVs,
													TArray<FColor>(), TArray<FProcMeshTangent>(), true);
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
		if (InVertices[i].Y > maxLength)
		{
			maxLength = InVertices[i].Y;
			AnchorLengthStart = FVector(0.0f, 0.0f, InVertices[i].Y);
		}
		if (InVertices[i].Y < minLength)
		{
			minLength = InVertices[i].Y;
			AnchorLengthEnd = FVector(0.0f, 0.0f, InVertices[i].Y);
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
		if (InVertices[i].Z > maxThickness)
		{
			maxThickness = InVertices[i].Z;
			AnchorThicknessStart = FVector(0.0f, InVertices[i].Z, 0.0f);
		}
		if (InVertices[i].Z < minThickness)
		{
			minThickness = InVertices[i].Z;
			AnchorThicknessEnd = FVector(0.0f, InVertices[i].Z, 0.0f);
		}
	}
	// Get all points that lie on ends
	for (int i = 0; i < InVertices.Num(); i++)
	{
		if (InVertices[i].Y == maxLength)
		{
			EndPointsA.Add(InVertices[i]);
		}
		if (InVertices[i].Y == minLength)
		{
			EndPointsB.Add(InVertices[i]);
		}
	}
}

void AHair::ClearMeshData(AHairSegment* InSegment)
{
	InSegment->ProceduralMeshData->Vertices.Empty();
	InSegment->ProceduralMeshData->Triangles.Empty();
	InSegment->ProceduralMeshData->UVs.Empty();
	InSegment->NumTriangles = 0;
	InSegment->IsUVReversed = true;
}

void AHair::AssignPositions(FVector InP1, FVector InP2)
{
	P1 = InP1;
	P2 = InP2;
}

void AHair::AddVertices(int FirstIndex, TArray<FVector> InVertices, AHairSegment* InSegment, float Distance)
{
	// Get player controllers
	AMyPlayerController* Controller = GetController();
	if (!Controller || !InSegment) return;

	for (int j = FirstIndex; j <= MiddleMeshData->Vertices.Num() - 1; j++)
	{
		InSegment->ProceduralMeshData->Vertices.Add(MapVertex(InVertices[j], InSegment, Distance, Weight));
	}
}

FVector AHair::MapVertex(FVector V, AHairSegment* Segment, float Distance, float InWeight)
{
	if (!Segment)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Segment during vertex mapping"));
		return FVector(0, 0, 0);
	}

	FVector Forward = Segment->Spline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	FVector Up = Segment->Spline->GetUpVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	FVector Right = Segment->Spline->GetRightVectorAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);

	// Find local percentage distance between A to B
	float VDistance = AnchorLengthStart.Y - V.Y;
	float LocalDistance = FVector::Dist(AnchorLengthStart, AnchorLengthEnd);
	float VRatio = VDistance / LocalDistance;
	// Get displacement vector from P1 to P2
	FVector P3 = P2 - P1;
	// From P1, add weighted displacement vector for vertical offset
	FVector V2 = P1 + VRatio*P3;

	// Get right ratio
	float RDistance = AnchorWidthStart.X - V.X;
	float RDisplacement = FVector::Dist(AnchorWidthStart, AnchorWidthEnd);
	float RRatio = 0.0f;
	if (RDisplacement == 0.0f)
		RRatio = 0.0f;
	else
		RRatio = -0.5f + RDistance / RDisplacement;
	// Apply right direction
	float TotalWidth = GlobalXWidth + Segment->SegmentXWidth;
	V2 = V2 + (Right*TotalWidth*RRatio*InWeight);

	// Find percentage distance of up
	float UDistance = AnchorThicknessStart.Z - V.Z;
	float UDisplacement = FVector::Dist(AnchorThicknessStart, AnchorThicknessEnd);
	float URatio = UDistance / UDisplacement;
	// Apply Y direction
	V2 = V2 + (Up*V.Z*InWeight);

	return V2;
}

void AHair::AddTriangles(AHairSegment* InSegment)
{
	if (!InSegment) return;

	for (int i = 0; i <= MiddleMeshData->Triangles.Num() - 1; i++)
	{
		InSegment->ProceduralMeshData->Triangles.Add(InSegment->NumTriangles + MiddleMeshData->Triangles[i]);
	}
	InSegment->NumTriangles = InSegment->NumTriangles + MiddleMeshData->Vertices.Num() - 2;
}

void AHair::AddUVs(AHairSegment* InSegment, bool IsFirst)
{
	if (!InSegment) return;
	if (IsFirst)
	{
		InSegment->ProceduralMeshData->UVs.Append(MiddleMeshData->UVs);
	}
	else
	{
		if (InSegment->IsUVReversed)
		{
			InSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[0]);
			InSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[1]);
		}
		else
		{
			int N = MiddleMeshData->UVs.Num();
			InSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[N-2]);
			InSegment->ProceduralMeshData->UVs.Add(MiddleMeshData->UVs[N-1]);
		}
		InSegment->IsUVReversed = !InSegment->IsUVReversed;
	}
}

//// NODE ////

void AHair::SelectNode(AHairNode* Node)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	Controller->TargetNodes.Add(Node);
	Node->StaticMesh->SetRenderCustomDepth(true);
}

void AHair::DeselectNode(AHairNode* Node)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;
	Node->StaticMesh->SetRenderCustomDepth(false);
	Controller->TargetNodes.Remove(Node);
}

void AHair::DeselectAllNodes()
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	int i = 0, n = 100;
	// Use bounded while to prevent 
	while (Controller->TargetNodes.Num() > 0 && i < n)
	{
		DeselectNode(Controller->TargetNodes[0]);
		i++;
	}
	if (i == n) UE_LOG(LogTemp, Warning, TEXT("Max %d loop capacity reached"), n);
}

void AHair::RemoveNode(AHairNode* Node)
{
	// Remove node from segment and update indices
	AHairSegment* Segment = Node->Segment;
	Segment->Nodes.Remove(Node);
	for (int i = 0; i < Segment->Nodes.Num(); i++)
	{
		Segment->Nodes[i]->Index = i;
	}
	// Reform spline
	Segment->Spline->ClearSplinePoints();
	for (int i = 0; i < Segment->Nodes.Num(); i++)
	{
		Segment->Spline->AddSplinePoint(Segment->Nodes[i]->GetActorLocation(), ESplineCoordinateSpace::World);
		Segment->Spline->SetUpVectorAtSplinePoint(i, Segment->Nodes[i]->GetActorUpVector(), ESplineCoordinateSpace::World);
	}
	//Node->Segment->Spline->RemoveSplinePoint(Node->Index);
	Node->Destroy();
	UpdateSegment(Segment);
}

void AHair::SetNodeVisibility(AHairSegment* Segment, bool IsVisible)
{
	for (int i = 0; i < Segment->Nodes.Num(); i++)
	{
		Segment->Nodes[i]->SetActorHiddenInGame(!IsVisible);
		Segment->Nodes[i]->SetActorEnableCollision(IsVisible);
	}
}

//// SEGMENT ////

void AHair::SelectSegment(AHairSegment* Segment)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	Controller->TargetSegments.Add(Segment);
	Segment->OutlineMesh->SetRenderCustomDepth(true);
	SetNodeVisibility(Segment, true);
}

void AHair::DeselectSegment(AHairSegment* Segment)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;
	Segment->OutlineMesh->SetRenderCustomDepth(false);
	Controller->TargetSegments.Remove(Segment);
	SetNodeVisibility(Segment, false);
}

void AHair::DeselectAllSegments()
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;
	
	// Disable extend mode
	Controller->IsExtending = false;

	int i = 0, n = 100;
	// Use bounded while to prevent 
	while (Controller->TargetSegments.Num() > 0 && i < n)
	{
		DeselectSegment(Controller->TargetSegments[0]);
		i++;
	}
	if (i == n) UE_LOG(LogTemp, Warning, TEXT("Max %d loop capacity reached"), n);
}

void AHair::RemoveSegment(AHairSegment* Segment)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;
	for (int i = 0; i < Segment->Nodes.Num(); i++)
	{
		Segment->Nodes[i]->Destroy();
	}
	Controller->TargetSegments.Remove(Segment);
	Segment->Destroy();
}

void AHair::RemoveSelected()
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	int i = 0, n = 100;
	if (Controller->TargetNodes.Num() > 0)
	{
		for (int i = 0; i < Controller->TargetNodes.Num(); i++)
			RemoveNode(Controller->TargetNodes[i]);
	}
	else
	{
		while (Controller->TargetSegments.Num() > 0 && i < 100)
		{
			RemoveSegment(Controller->TargetSegments[0]);
			i++;
		}
	}
	if (i == n) UE_LOG(LogTemp, Warning, TEXT("Max %d loop capacity reached"), n);
}

void AHair::SetSelectedSegmentXWidth(float InVal)
{
	AMyPlayerController* Controller = GetController();
	for (int i = 0; i < Controller->TargetSegments.Num(); i++)
	{
		Controller->TargetSegments[i]->SegmentXWidth = InVal;
	}
}

void AHair::SetSelectedSegmentFallOff(float InVal)
{
	AMyPlayerController* Controller = GetController();
	for (int i = 0; i < Controller->TargetSegments.Num(); i++)
	{
		Controller->TargetSegments[i]->FallOff = InVal;
	}
}

void AHair::SetSelectedQuality(float InVal)
{
	AMyPlayerController* Controller = GetController();
	for (int i = 0; i < Controller->TargetSegments.Num(); i++)
	{
		Controller->TargetSegments[i]->NumSegments = InVal;
	}
}

//// LAYER ////

void AHair::SelectLayer(AHairLayer* Layer, bool IsAppend)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	if (!IsAppend)
		DeselectAll();

	for (int i = 0; i < Layer->Segments.Num(); i++)
	{
		SelectSegment(Layer->Segments[i]);
	}
}

void AHair::DeselectAll()
{
	DeselectAllSegments();
	DeselectAllNodes();
}

void AHair::RemoveLayer(AHairLayer* Layer)
{
	AMyPlayerController* Controller = GetController();
	if (!Controller) return;

	// Remove all segments beforehand
	for (int i = 0; i < Layer->Segments.Num(); i++)
	{
		RemoveSegment(Layer->Segments[i]);
	}

	// Only delete layer if there is more than one
	if (HairLayers.Num() > 1)
	{
		HairLayers.Remove(Layer);

		// Pick top layer when removing selected
		if (Controller->TargetLayer == Layer)
		{
			Controller->TargetLayer = HairLayers[0];
		}

		Layer->Destroy();
	}	
}

void AHair::SetLayerLock(AHairLayer* Layer, bool IsLocked)
{
	Layer->IsLocked = IsLocked;
	for (int i = 0; i < Layer->Segments.Num(); i++)
	{
		AHairSegment* Segment = Layer->Segments[i];
		// Remove from selection, nodes will be disabled
		DeselectSegment(Layer->Segments[i]);
		Segment->SetActorEnableCollision(!IsLocked);
	}
}

void AHair::SetLayerVisibility(AHairLayer* Layer, bool IsVisible)
{
	Layer->IsVisible = IsVisible;
	for (int i = 0; i < Layer->Segments.Num(); i++)
	{
		AHairSegment* Segment = Layer->Segments[i];
		Segment->SetActorHiddenInGame(IsVisible);
		// Hide nodes
		for (int j = 0; j < Segment->Nodes.Num(); j++)
		{
			Segment->Nodes[j]->SetActorHiddenInGame(IsVisible);
		}
	}

	if (!IsVisible)
		SetLayerLock(Layer, true);
}


//// FILE MANAGEMENT ////

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


//// MISC ////

AMyPlayerController* AHair::GetController()
{
	UWorld* const World = GetWorld();
	if (!World) return NULL;

	// Get player controllers
	AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

	if (!Controller) return NULL;

	return Controller;
}