// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "Hair.h"


// Sets default values
AHair::AHair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHair::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHair::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

