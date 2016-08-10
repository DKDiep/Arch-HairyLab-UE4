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
		Hair->DeselectAllSegments();
		if (!IsShiftDown)
		{
			Hair->DeselectAllNodes();
		}
		Hair->SelectNode(Node);
	}
	else if (Segment)
	{
		Hair->DeselectAllNodes();
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
	if (IsAltDown)
	{
		FVector Offset = FVector(MouseY, MouseX, MouseWheel)*MouseSpeed;
		for (int i = 0; i < TargetNodes.Num(); i++)
		{
			FVector Location = TargetNodes[i]->GetActorLocation() + Offset;
			Hair->SetNodeLocation(TargetNodes[i], Location, IsShiftDown);
		}
	}
	else if (IsCtrlDown)
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
	else
	{
		for (int i = 0; i < TargetNodes.Num(); i++)
		{
			APawn* Camera = GetControlledPawn();
			FVector ForwardOffset = MouseSpeed*MouseWheel*Camera->GetActorForwardVector();
			FVector RightOffset = MouseSpeed*MouseX*Camera->GetActorRightVector();
			FVector UpOffset = MouseSpeed*MouseY*Camera->GetActorUpVector();
			FVector Location = TargetNodes[i]->GetActorLocation() + ForwardOffset + RightOffset + UpOffset;
			Hair->SetNodeLocation(TargetNodes[i], Location, IsShiftDown);
		}
	}
}