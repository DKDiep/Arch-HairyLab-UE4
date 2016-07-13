// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "ArchsHairyLab.h"
#include "Head.h"


// Sets default values
AHead::AHead()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHead::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

