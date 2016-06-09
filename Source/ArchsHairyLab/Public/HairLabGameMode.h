// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyPlayerController.h"

#include "GameFramework/GameMode.h"
#include "HairLabGameMode.generated.h"
	
UCLASS()
class ARCHSHAIRYLAB_API AHairLabGameMode : public AGameMode
{
	GENERATED_BODY()
	
	AHairLabGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void StartPlay() override;
	
	
};
