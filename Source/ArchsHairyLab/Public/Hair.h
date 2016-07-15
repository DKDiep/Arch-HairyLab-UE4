// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Hair.generated.h"

UCLASS()
class ARCHSHAIRYLAB_API AHair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHair();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<class AHairLayer*> HairLayers;
	
	UPROPERTY(BlueprintReadWrite)
	float GlobalXWidth = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	float GlobalYWidth = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Generation")
	AHairSegment* SpawnSegment();

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Generation")
	void ExtendSegment();

	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Generation")
	AHairNode* SpawnNode(AMyPlayerController* Controller, UWorld* World);
};
