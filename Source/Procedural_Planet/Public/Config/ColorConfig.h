// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ColorConfig.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURAL_PLANET_API UColorConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	FColor PlanetColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Data)
	UMaterialInterface* MaterialInterface;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Data)
	UMaterialInstanceDynamic* DynamicMaterial;
};
	