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

void AMyPlayerController::OnLMB()
{
	/*
	if click hair
		left click drag to move hair
		right click drag to rotate hair
	else if click node
		left click drag to rotate node
		right click drag to rotate node
	else
		deselect all
	*/

	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
	AHairSegment* Segment = Cast<AHairSegment>(HitResult.GetActor());
	AHairNode* Node = Cast<AHairNode>(HitResult.GetActor());

	if (Node)
	{
		if (!IsShiftDown)
		{
			Hair->DeselectAllNodes();
		}
		Hair->SelectNode(Node);
	}
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

void AMyPlayerController::DragNode()
{
	FVector Offset = FVector(MouseY, MouseX, MouseWheel)*MouseSpeed;
	if (!IsAltDown)
	{
		for (int i = 0; i < TargetNodes.Num(); i++)
		{
			TargetNodes[i]->AddActorWorldOffset(Offset);
			TargetNodes[i]->Segment->SetSplinePoint(TargetNodes[i]->Index, TargetNodes[i]->GetActorLocation());
			Hair->UpdateSegment(TargetNodes[i]->Segment);
		}
	}
}