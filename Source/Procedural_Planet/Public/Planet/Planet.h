// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

class UColorGenerator;
class UShapeGenerator;
class ATerrainFace;
class UProceduralMeshComponent;
class UColorConfig;
class UShapeConfig;

UCLASS()
class PROCEDURAL_PLANET_API APlanet : public AActor
{
	GENERATED_BODY()

public:
	APlanet();

	virtual void BeginPlay() override;

	void GeneratePlanet();
	void Initialize();

	void UpdateColor();
	void GenerateColor();

	void UpdateShape();
	void GenerateMesh();

private:
	TArray<ATerrainFace*> Faces;
	// Edit
	UPROPERTY(VisibleAnywhere, Category=Material)
	UColorConfig* ColorConfig;
	UPROPERTY(VisibleAnywhere, Category=Material)
	UShapeConfig* ShapeConfig;
	UPROPERTY(VisibleAnywhere, Category=Planet)
	UShapeGenerator* ShapeGenerator;
	UPROPERTY(VisibleAnywhere, Category=Planet)
	UColorGenerator* ColorGenerator;	
	// Visible
	UPROPERTY(VisibleAnywhere, Category=Material)
	UMaterialInstanceDynamic* DynamicMaterial;
	
	UPROPERTY(EditAnywhere, Category=Property, meta = (ClampMin = "2", ClampMax = "256", UIMin = "2", UIMax = "256"))
	int Resolution;
	UPROPERTY(VisibleAnywhere, Category=Property, meta = (ClampMin = "2", ClampMax = "256", UIMin = "2", UIMax = "256"))
	int FaceAmount;
};