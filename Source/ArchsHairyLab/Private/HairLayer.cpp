// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairLayer.h"


// Sets default values
AHairLayer::AHairLayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHairLayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHairLayer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

