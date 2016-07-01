// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProceduralMeshData = NewObject<UProceduralMeshData>();
}


