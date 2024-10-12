// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainFace.generated.h"

class UShapeGenerator;
class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_PLANET_API ATerrainFace : public AActor
{
	GENERATED_BODY()
	
public:	
	ATerrainFace();
	
	void Initialize(UShapeGenerator* Generator, int Res, FVector LocalUpVector);
	void ConstructMesh();
	void RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals);
	void CalculateVertexColor(TArray<FColor>& Colors, TArray<FVector>& Vertices);
	
	FORCEINLINE UProceduralMeshComponent* GetMesh() const { return Mesh; };
private:
	UPROPERTY(VisibleAnywhere, Category=Property)
	UShapeGenerator* ShapeGenerator;	
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
};
