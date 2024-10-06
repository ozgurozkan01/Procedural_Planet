// Fill out your copyright notice in the Description page of Project Settings.


#include "Noise/RidgidNoiseFilter.h"

#include "Noise/NoiseManager.h"

// Sets default values for this component's properties
URidgidNoiseFilter::URidgidNoiseFilter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	NoiseManager = CreateDefaultSubobject<UNoiseManager>(TEXT("NoiseManager"));
}

float URidgidNoiseFilter::Evaluate(const FVector Point)
{
	if (NoiseManager == nullptr || NoiseSettings.Persistence == 0) { UE_LOG(LogTemp, Warning, TEXT("Noise is null")); return 1.f; }

	float NoiseValue = 0;
	float Frequency = NoiseSettings.BaseRoughness;
	float Amplitude = 1.f;
	float Weight = 1;
	
	for (int Index = 0; Index < NoiseSettings.LayerNum; Index++)
	{
		float V = 1 - FMath::Abs(NoiseManager->Evaluate(Point * Frequency + NoiseSettings.Centre));
		V *= V;
		V *= Weight;
		Weight = FMath::Clamp(V * NoiseSettings.WeightMultiplier, 0, 1);
		
		NoiseValue += V * Amplitude; // Clamp the value in range(0, 1)
		Frequency *= NoiseSettings.Roughness;
		Amplitude *= NoiseSettings.Persistence;
	}

	NoiseValue = FMath::Max(0, NoiseValue - NoiseSettings.MinValue);
	return NoiseValue * NoiseSettings.Strength;
}

void URidgidNoiseFilter::Initialize(const FNoiseSettings& Settings)
{
	NoiseSettings = Settings.RidgidNoiseSettings;
}