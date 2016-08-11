// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "MyPlayerController.h"

#include "HairNode.h"
#include "HairSegment.h"
#include "HairLayer.h"
#include "Hair.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AMyPlayerController::OnLMB()
{
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
	AHairSegment* Segment = Cast<AHairSegment>(HitResult.GetActor());
	AHairNode* Node = Cast<AHairNode>(HitResult.GetActor());

	if (Node)
	{
		//Hair->DeselectAllSegments();
		if (!IsShiftDown)
		{
			Hair->DeselectAllNodes();
		}
		Hair->SelectNode(Node);
	}
	else if (Segment)
	{
		//Hair->DeselectAllNodes();
		if (!IsShiftDown)
		{
			Hair->DeselectAll();
		}
		Hair->SelectSegment(Segment);
	}
	else
	{
		if (IsExtending)
		{
			Hair->ExtendSegment();
			Hair->UpdateHair();
		}
		else if (TargetSegments.Num() + TargetNodes.Num() > 0)
		{
			Hair->DeselectAll();
		}
		else
		{
			Segment = Hair->SpawnSegment();
			IsExtending = IsValid(Segment);
		}
	}
}

void AMyPlayerController::DragNode()
{
	if (TargetNodes.Num() == 0) return;

	if (IsCtrlDown)
	{
		// Hit object array
		TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
		// Custom collision 'Head' is 7
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);

		FHitResult Hit;
		bool bHitOccurred = GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit);
		if (bHitOccurred)
		{
			FVector Location = Hit.Location + Hit.Normal * 25.0;
			Hair->SetNodeLocation(TargetNodes[0], Location, IsShiftDown);
		}
	}
	else if (IsAltDown)
	{
		for (int i = 0; i < TargetNodes.Num(); i++)
		{
			FRotator Rot = FRotator(0.0f, MouseWheel*MouseSpeed*2.0f, 0.0f);
			Hair->SetNodeRotation(TargetNodes[i], Rot, false);
		}
	}
	else
	{
		for (int i = 0; i < TargetNodes.Num(); i++)
		{
			APawn* Camera = GetPawn();
			FVector ForwardOffset = MouseSpeed*-MouseX*Camera->GetActorForwardVector();
			FVector RightOffset = MouseSpeed*MouseWheel*Camera->GetActorRightVector();
			FVector UpOffset = MouseSpeed*MouseY*Camera->GetActorUpVector();
			FVector Location = TargetNodes[i]->GetActorLocation() + ForwardOffset + RightOffset + UpOffset;
			Hair->SetNodeLocation(TargetNodes[i], Location, IsShiftDown);
		}
	}
}