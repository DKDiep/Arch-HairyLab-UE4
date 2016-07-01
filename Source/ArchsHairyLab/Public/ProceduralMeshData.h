// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ProceduralMeshData.generated.h"

/**
 * 
 */
UCLASS()
class ARCHSHAIRYLAB_API UProceduralMeshData : public UObject
{
	GENERATED_BODY()
public:
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UVs;

	void ClearMeshData();
};
