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
	TArray<class AHairNode*> TargetNodes;

	UPROPERTY(BlueprintReadWrite)
	TArray<class AHairSegment*> TargetSegments;

	UPROPERTY(BlueprintReadWrite)
	class AHairLayer* TargetLayer;

	UPROPERTY(BlueprintReadWrite)
	bool IsExtending;

	UPROPERTY(BlueprintReadWrite)
	bool IsRotate;

	UPROPERTY(BlueprintReadWrite)
	bool IsMove;

	UPROPERTY(BlueprintReadWrite)
	bool IsShiftDown;

	UPROPERTY(BlueprintReadWrite)
	bool IsAltDown;


	// Methods
	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void HairOp();

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void NodeOp();
};