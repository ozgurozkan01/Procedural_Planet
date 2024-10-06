// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Config/ShapeConfig.h"
#include "ShapeGenerator.generated.h"


class UNoiseFilter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURAL_PLANET_API UShapeGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShapeGenerator();

	void Initialize(UShapeConfig* Config);
	FVector CalculatePointOnPlanet(FVector PointOnUnitSphere);
	UPROPERTY(EditAnywhere, Category=Config)
	TArray<INoiseFilterInterface*> NoiseFilters;
private:
	UPROPERTY(EditAnywhere, Category=Config)
	UShapeConfig* ShapeConfig;

};
