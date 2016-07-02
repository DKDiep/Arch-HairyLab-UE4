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
};
