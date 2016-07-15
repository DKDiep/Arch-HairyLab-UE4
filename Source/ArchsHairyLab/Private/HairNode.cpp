// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairNode.h"


// Sets default values
AHairNode::AHairNode()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TmpMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Sphere.Sphere'"));
	if (TmpMesh.Succeeded())
		StaticMesh->SetStaticMesh(TmpMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TmpMat(TEXT("MaterialInterface'/Game/Meshes/defaultMat.defaultMat'"));
	if (TmpMat.Succeeded())
		StaticMesh->SetMaterial(0, TmpMat.Object);
}

// Called when the game starts or when spawned
void AHairNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHairNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

