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

	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AHair* Hair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int AAA = 0;

	UPROPERTY(BlueprintReadWrite)
	class AMyPlayerController* Controller;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Setup();
};
