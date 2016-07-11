// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int AMyPlayerController::SetupBP()
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
	return 0;
}

void AMyPlayerController::CalculateEndPoints(TArray<FVector> Vertices)
{
	float min = FLT_MAX;
	float max = FLT_MIN;
	// First calculate ends
	for (int i = 0; i < Vertices.Num(); i++)
	{
		if (Vertices[i].Z > max)
		{
			max = Vertices[i].Z;
			AnchorA = FVector(0.0f,0.0f,Vertices[i].Z);
		}
		if (Vertices[i].Z < min)
		{
			min = Vertices[i].Z;
			AnchorB = FVector(0.0f,0.0f,Vertices[i].Z);
		}
	}
	// Get all points that lie on ends
	for (int i = 0; i < Vertices.Num(); i++)
	{
		if (Vertices[i].Z == max)
		{
			EndPointsA.Add(Vertices[i]);
		}
		if (Vertices[i].Z == min)
		{
			EndPointsB.Add(Vertices[i]);
		}
	}
}

void AMyPlayerController::AssignPositions(FVector InP1, FVector InP2)
{
	P1 = InP1;
	P2 = InP2;
}

FVector AMyPlayerController::MapVertex(FVector V, FVector Direction, FVector Normal, float Weight)
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
	V2 = V2 + (DirX*V.X*Weight);

	// Get Y direction
	FVector DirY = DirX.RotateAngleAxis(90, FVector(0, 0, 1));
	// Apply Y direction
	V2 = V2 + (DirY*V.Y*Weight);
	//return V;
	return V2;
}
