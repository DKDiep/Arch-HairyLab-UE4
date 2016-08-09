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
	bool IsLMB = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsRMB = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsShiftDown = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsCtrlDown = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsAltDown = false;

	UPROPERTY(BlueprintReadWrite)
	float MouseX = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MouseY = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MouseWheel = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MouseSpeed = 5.0f;

	//// Operation Methods ////

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void OnLMB();

	UFUNCTION(BlueprintCallable, Category = "Hair Editing")
	void DragNode();
};