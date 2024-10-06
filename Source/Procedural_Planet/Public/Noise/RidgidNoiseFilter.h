// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoiseFilterInterface.h"
#include "NoiseSettings.h"
#include "Components/ActorComponent.h"
#include "RidgidNoiseFilter.generated.h"

class UNoiseManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURAL_PLANET_API URidgidNoiseFilter : public UActorComponent, public INoiseFilterInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URidgidNoiseFilter();
	virtual float Evaluate(const FVector Point) override;
	virtual void Initialize(const FNoiseSettings& Settings) override;
	
	UPROPERTY(EditAnywhere, Category=Noise)
	FRidgidNoiseSettings NoiseSettings;
private:
	UPROPERTY(VisibleAnywhere, Category=Noise)
	UNoiseManager* NoiseManager;
};
