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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Vertices;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Triangles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> UVs;

	UFUNCTION(BlueprintCallable, Category="Mesh Data Methods")
	void ClearMeshData();
};
