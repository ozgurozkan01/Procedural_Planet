// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Noise/SimpleNoiseFilter.h"
#include "ShapeConfig.generated.h"

struct FNoiseSettings;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FNoiseLayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bEnabled = true;
	UPROPERTY(EditAnywhere)
	bool bUseFirstLayerAsMask;
	UPROPERTY(EditAnywhere)
	FNoiseSettings NoiseSettings;
};

UCLASS()
class PROCEDURAL_PLANET_API UShapeConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	float PlanetRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Data)
	TArray<FNoiseLayer> NoiseLayers;	
};
