// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairLayer.h"


// Sets default values
AHairLayer::AHairLayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AHairLayer::AHairLayer(FName InName, int InId)
{
	PrimaryActorTick.bCanEverTick = false;

	Name = InName;
	Id = InId;
}

// Called when the game starts or when spawned
void AHairLayer::BeginPlay()
{
	Super::BeginPlay();
	
}