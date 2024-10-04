// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShapeConfig.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_PLANET_API UShapeConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	float PlanetRadius;
};
