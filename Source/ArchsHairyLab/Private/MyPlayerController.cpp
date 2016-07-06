// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProceduralMeshData = NewObject<UProceduralMeshData>();
	StartMeshData = NewObject<UProceduralMeshData>();
	MiddleMeshData = NewObject<UProceduralMeshData>();
	EndMeshData = NewObject<UProceduralMeshData>();

	// Create default mesh data for start, middle, end components
	StartMeshData->Vertices.Add(FVector(-10.0f, 50.0f, 0.0f));
	StartMeshData->Vertices.Add(FVector(10.0f, 50.0f, 0.0f));
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

	MiddleMeshData->Vertices.Add(FVector(-25.0f, 50.0f, 0.0f));
	MiddleMeshData->Vertices.Add(FVector(25.0f, 50.0f, 0.0f));
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

	EndMeshData->Vertices.Add(FVector(-25.0f, 50.0f, 0.0f));
	EndMeshData->Vertices.Add(FVector(25.0f, 50.0f, 0.0f));
	EndMeshData->Vertices.Add(FVector(0.0f, 0.0f, 0.0f));
	EndMeshData->Triangles.Add(0);
	EndMeshData->Triangles.Add(1);
	EndMeshData->Triangles.Add(2);
	EndMeshData->UVs.Add(FVector2D(0.0f, 1.0f));
	EndMeshData->UVs.Add(FVector2D(1.0f, 1.0f));
	EndMeshData->UVs.Add(FVector2D(0.5f, 0.0f));

	CalculateEndPoints(MiddleMeshData->Vertices);
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
			AnchorA = Vertices[i];
		}
		if (Vertices[i].Z < min)
		{
			min = Vertices[i].Z;
			AnchorB = Vertices[i];
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
