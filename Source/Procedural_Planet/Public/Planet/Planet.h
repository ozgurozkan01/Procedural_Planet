// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

class ATerrainFace;
class UProceduralMeshComponent;

UCLASS()
class PROCEDURAL_PLANET_API APlanet : public AActor
{
	GENERATED_BODY()

public:
	APlanet();

	virtual void BeginPlay() override;

	void GeneratePlanet();
	
	TArray<ATerrainFace*> Faces;
	
	UPROPERTY(EditAnywhere, Category=Property)
	UMaterial* Material;
	UPROPERTY(EditAnywhere, Category=Property, meta = (ClampMin = "2", ClampMax = "256", UIMin = "2", UIMax = "256"))
	int Resolution;
	int FaceAmount;
};