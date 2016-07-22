// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//Input
#include "InputCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

UCLASS()
class AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AMyPlayerController(const FObjectInitializer& ObjectInitializer);

public:
	// Data for usage
	UPROPERTY(BlueprintReadWrite)
	class AHair* Hair;
	
	UPROPERTY(BlueprintReadWrite)
	FHitResult HitResult = FHitResult();

	UPROPERTY(BlueprintReadWrite)
	class AHairNode* TargetNode;

	UPROPERTY(BlueprintReadWrite)
	class AHairSegment* TargetSegment;

	UPROPERTY(BlueprintReadWrite)
	class AHairLayer* TargetLayer;
};