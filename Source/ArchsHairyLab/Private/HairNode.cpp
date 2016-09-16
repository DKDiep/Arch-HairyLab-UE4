// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairNode.h"


// Sets default values
AHairNode::AHairNode()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TmpMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Sphere.Sphere'"));
	if (TmpMesh.Succeeded())
		StaticMesh->SetStaticMesh(TmpMesh.Object);
	StaticMesh->SetCastShadow(false);
	StaticMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TmpMat(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (TmpMat.Succeeded())
		StaticMesh->SetMaterial(0, TmpMat.Object);
}

// For debug
void AHairNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bHidden) return;

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector()*50.0f,
		FColor::Green,
		false,
		-1.0f,
		0,
		1.0f
		);

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorRightVector()*50.0f,
		FColor::Red,
		false,
		-1.0f,
		0,
		1.0f
		);

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorUpVector()*50.0f,
		FColor::Blue,
		false,
		-1.0f,
		0,
		1.0f
		);
}

