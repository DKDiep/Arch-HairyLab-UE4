// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "Hair.h"

#include "HairLayer.h"
#include "HairSegment.h"
#include "MyPlayerController.h"
#include "Head.h"

AHair::AHair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create default layer
	AHairLayer* DefaultLayer = NewObject<AHairLayer>();
	DefaultLayer->Name = FName("Layer 0");
	DefaultLayer->Id = 0;
	HairLayers.Add(DefaultLayer);
}

// Called when the game starts or when spawned
void AHair::BeginPlay()
{
	Super::BeginPlay();
	
}

AHairSegment* AHair::SpawnSegment()
{
	UWorld* const World = GetWorld();
	if (World) {
		// Get player controllers
		AMyPlayerController* Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());

		if (!Controller)
			return NULL;

		// Set controller cursor hit result
		Controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, Controller->HitResult);

		// Only proceed if hit head mesh
		if (!Controller->HitResult.Actor->IsA(AHead::StaticClass()))
			return NULL;

		// Spawn segment object
		FActorSpawnParameters SpawnParams;
		Controller->TargetSegment = World->SpawnActor<AHairSegment>(AHairSegment::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f), SpawnParams);
		// Setup new segment
		Controller->TargetSegment->AddSplinePoint(Controller->HitResult.Location);
		Controller->TargetSegment->Normals.Add(Controller->HitResult.Normal);

		// Add hair segment to current layer
		if (!Controller->TargetLayer)
			return Controller->TargetSegment;

		Controller->TargetLayer->Segments.Add(Controller->TargetSegment);
	}

	return NULL;
}