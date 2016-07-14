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
#
	//Hair = (AHair*)GetWorld()->SpawnActor<AHair>(AHair::StaticClass(), FVector(0.0f,0.0f,0.0f), FRotator(0.0f));
	UWorld* const World = GetWorld();
	if (World) {
		// Spawn hair object
		FActorSpawnParameters SpawnParams;
		Hair = World->SpawnActor<AHair>(AHair::StaticClass(), FVector(0.0f,0.0f,0.0f), FRotator(0.0f), SpawnParams);

		// Get player controllers
		Controller = Cast<AMyPlayerController>(World->GetFirstPlayerController());
	}


}

// Called when the game starts or when spawned
void AHairLabGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("GameMode_Begin"));
}

// Called every frame
/*void AHairLabGameMode::Tick(float DeltaTime)
{
}*/

