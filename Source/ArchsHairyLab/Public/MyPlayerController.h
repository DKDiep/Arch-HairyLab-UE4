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
	//// Variables ////
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
	bool IsExtending = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsRotate = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsDrag = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsShiftDown = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsAltDown = false;


	//// Operation Methods ////

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void HairOp();

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void NodeOp();

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void DragNode(float MouseX, float MouseY, float MouseWheel);
};