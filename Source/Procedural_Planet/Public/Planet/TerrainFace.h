// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainFace.generated.h"

class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_PLANET_API ATerrainFace : public AActor
{
	GENERATED_BODY()
	
public:	
	ATerrainFace();
	
	void Initialize(int Res, FVector LocalUpVector, FColor MeshColor, UMaterialInstanceDynamic* DynamicMaterial);
	void ConstructMesh();
	void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals);
	
private:
	UPROPERTY(VisibleAnywhere, Category=Property)
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY(VisibleAnywhere, Category=Property)
	FColor Color;
	UPROPERTY(VisibleAnywhere, Category=Property)
	UProceduralMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category=Property)
	FVector LocalUp;
	UPROPERTY(VisibleAnywhere, Category=Property)
	FVector AxisA;
	UPROPERTY(VisibleAnywhere, Category=Property)
	FVector AxisB;
	UPROPERTY(VisibleAnywhere, Category=Property)
	int Resolution;
	UPROPERTY(VisibleAnywhere, Category=Property)
	int Size;
};
