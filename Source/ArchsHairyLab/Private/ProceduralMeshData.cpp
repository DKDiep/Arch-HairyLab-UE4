// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchsHairyLab.h"
#include "ProceduralMeshData.h"


void UProceduralMeshData::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	UVs.Empty();
}
