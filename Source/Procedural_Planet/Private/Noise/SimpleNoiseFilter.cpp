// Fill out your copyright notice in the Description page of Project Settings.


#include "Noise/SimpleNoiseFilter.h"
#include "Noise/NoiseManager.h"

USimpleNoiseFilter::USimpleNoiseFilter()
{
	PrimaryComponentTick.bCanEverTick = false;
	NoiseManager = CreateDefaultSubobject<UNoiseManager>(TEXT("NoiseManager"));
}

float USimpleNoiseFilter::Evaluate(const FVector Point)
{
	if (NoiseManager == nullptr || NoiseSettings.Persistence == 0) { UE_LOG(LogTemp, Warning, TEXT("Noise is null")); return 1.f; }

	float NoiseValue = 0;
	float Frequency = NoiseSettings.BaseRoughness;
	float Amplitude = 1.f;

	for (int Index = 0; Index < NoiseSettings.LayerNum; Index++)
	{
		float V = NoiseManager->Evaluate(Point * Frequency + NoiseSettings.Centre);
		NoiseValue += (V + 1) *.5f * Amplitude; // Clamp the value in range(0, 1)
		Frequency *= NoiseSettings.Roughness;
		Amplitude *= NoiseSettings.Persistence;
	}

	NoiseValue = FMath::Max(0, NoiseValue - NoiseSettings.MinValue);
	return NoiseValue * NoiseSettings.Strength;
}

void USimpleNoiseFilter::Initialize(const FNoiseSettings& Settings)
{
	NoiseSettings = Settings.SimpleNoiseSettings;
}
