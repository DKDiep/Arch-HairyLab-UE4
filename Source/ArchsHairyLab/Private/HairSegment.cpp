// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairSegment.h"

#include "ProceduralMeshComponent.h"

// Sets default values
AHairSegment::AHairSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AHairSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHairSegment::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

