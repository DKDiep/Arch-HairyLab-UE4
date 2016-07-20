// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairLabGameMode.h"

#include "Hair.h"
#include "MyPlayerController.h"

AHairLabGameMode::AHairLabGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	// Find custom player controller base class
	PlayerControllerClass = AMyPlayerController::StaticClass();

	// Find Blueprint class object
	static ConstructorHelpers::FClassFinder<AMyPlayerController> PCOb(TEXT("Blueprint'/Game/Blueprints/BP_MyPlayerController.BP_MyPlayerController_C'"));
	if (PCOb.Class != NULL)
	{
		PlayerControllerClass = PCOb.Class;
	}

	/*PlayerStateClass = ;
	GameStateClass = ;
	SpectatorClass = ;*/
}

// Called when the game starts or when spawned
void AHairLabGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("GameMode_Begin"));

	// Controller exists by StartPlay
	UWorld* const World = GetWorld();
	if (World) {
		// Spawn hair object
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		this->Hair = World->SpawnActor<AHair>(AHair::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f), SpawnParams);

		// Get player controllers
		Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());
		if (Hair && Controller && Hair->HairLayers.Num() > 0 && Hair->HairLayers[0])
		{
			Controller->Hair = Hair;
			Controller->TargetLayer = Hair->HairLayers[0];
		}
	}
}
