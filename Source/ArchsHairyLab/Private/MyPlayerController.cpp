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
			TargetNodes[i]->AddActorWorldOffset(Offset);
			TargetNodes[i]->Segment->SetSplinePoint(TargetNodes[i]->Index, TargetNodes[i]->GetActorLocation());
			Hair->UpdateSegment(TargetNodes[i]->Segment);
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
			TargetNodes[0]->SetActorLocation(Location);
			TargetNodes[0]->Segment->SetSplinePoint(TargetNodes[0]->Index, TargetNodes[0]->GetActorLocation());
			Hair->UpdateSegment(TargetNodes[0]->Segment);
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
			FVector Offset = ForwardOffset + RightOffset + UpOffset;
			TargetNodes[i]->AddActorWorldOffset(Offset);
			TargetNodes[i]->Segment->SetSplinePoint(TargetNodes[i]->Index, TargetNodes[i]->GetActorLocation());
			Hair->UpdateSegment(TargetNodes[i]->Segment);
		}
	}
}