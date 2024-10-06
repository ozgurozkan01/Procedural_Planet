// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseFilterInterface.h"
#include "Components/ActorComponent.h"
#include "NoiseSettings.h"
#include "SimpleNoiseFilter.generated.h"

class UNoiseManager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROCEDURAL_PLANET_API USimpleNoiseFilter : public UActorComponent, public INoiseFilterInterface
{
	GENERATED_BODY()

public:
	USimpleNoiseFilter();

	virtual float Evaluate(const FVector Point) override;
	virtual void Initialize(const FNoiseSettings& Settings) override;
	
	UPROPERTY(EditAnywhere, Category=Noise)
	FSimpleNoiseSettings NoiseSettings;
private:
	UPROPERTY(VisibleAnywhere, Category=Noise)
	UNoiseManager* NoiseManager;
};
