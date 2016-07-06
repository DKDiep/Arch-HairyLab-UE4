// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Input
#include "InputCoreTypes.h"

#include "ProceduralMeshData.h"

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AMyPlayerController(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshData* ProceduralMeshData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshData* StartMeshData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshData* MiddleMeshData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshData* EndMeshData;

	// Data for connecting to components
	UPROPERTY(BlueprintReadWrite)
	FVector AnchorA = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite)
	FVector AnchorB = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> EndPointsA;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> EndPointsB;
	UPROPERTY(BlueprintReadWrite)
	FVector P1 = FVector(0,0,0);
	UPROPERTY(BlueprintReadWrite)
	FVector P2 = FVector(0,0,0);

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Methods")
	int SetupBP();

	UFUNCTION(BlueprintCallable, Category = "Prodecural Mesh Methods")
	void CalculateEndPoints(TArray<FVector> Vertices);

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Methods")
	void AssignPositions(FVector InP1, FVector InP2);

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Methods")
	void MapVertex(FVector V, FVector Direction, FVector Normal);
};