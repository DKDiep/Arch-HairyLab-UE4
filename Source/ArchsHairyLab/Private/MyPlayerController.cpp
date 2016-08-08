// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "MyPlayerController.h"

#include "HairNode.h"
#include "HairSegment.h"
#include "HairLayer.h"
#include "Hair.h"

AMyPlayerController::AMyPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void AMyPlayerController::HairOp()
{
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
	AHairSegment* Segment = Cast<AHairSegment>(HitResult.GetActor());
	bool IsEnabled = TargetLayer->IsVisible && !TargetLayer->IsLocked;
	// Spawn or extend segment
	if (IsEnabled)
	{
		if (Segment)
		{
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
			else
			{
				Segment = Hair->SpawnSegment();
				IsExtending = IsValid(Segment);
			}
		}
	}
}

void AMyPlayerController::NodeOp()
{
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
	AHairNode* Node = Cast<AHairNode>(HitResult.GetActor());
	if (!Node) return;

	if (!TargetSegments.Contains(Node->Segment)) return;

	if (!false) //is shift pressed
	{
		//
		//Hair->DeselectNodes();
	}

	//Hair->SelectNode(Node);
}
