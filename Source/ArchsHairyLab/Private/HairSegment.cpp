// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "HairSegment.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralMeshData.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

// Sets default values
AHairSegment::AHairSegment()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshData = NewObject<UProceduralMeshData>();
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ProceduralMesh;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	Spline->ClearSplinePoints();

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> TmpMat(TEXT("MaterialInterface'/Game/Materials/M_Test.M_Test'"));
	if (TmpMat.Succeeded())
		Material = TmpMat.Object;

	if (Material)
		SetSegmentMaterial(0, Material);
}

void AHairSegment::AddSplinePoint(FVector Location)
{
	Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World);
}

void AHairSegment::SetSegmentMaterial(int Index, UMaterialInterface* InMaterial)
{
	if (InMaterial)
		ProceduralMesh->SetMaterial(0, InMaterial);
}

// Called when the game starts or when spawned
void AHairSegment::BeginPlay()
{
	Super::BeginPlay();
}