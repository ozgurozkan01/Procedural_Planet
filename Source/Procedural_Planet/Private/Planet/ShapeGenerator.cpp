// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet/ShapeGenerator.h"
#include "Noise/NoiseFilterFactory.h"

UShapeGenerator::UShapeGenerator()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShapeGenerator::Initialize(UShapeConfig* Config)
{
	if (Config)
	{
		ShapeConfig = Config;
		NoiseFilters.SetNum(Config->NoiseLayers.Num());

		for (int Index = 0; Index < NoiseFilters.Num(); Index++)
		{
			auto NoiseFilter = UNoiseFilterFactory::CreateNoiseFilter(ShapeConfig->NoiseLayers[Index].NoiseSettings);
		
			if (NoiseFilter)
			{
				NoiseFilter->Initialize(ShapeConfig->NoiseLayers[Index].NoiseSettings);
				NoiseFilters[Index] = NoiseFilter;
			}
		}	
	}
}

FVector UShapeGenerator::CalculatePointOnPlanet(FVector PointOnUnitSphere)
{
	float FirstLayerValue = 0;
	float Elevation = 0;

	if (NoiseFilters.Num() > 0)
	{
		FirstLayerValue = NoiseFilters[0]->Evaluate(PointOnUnitSphere);

		if (ShapeConfig->NoiseLayers[0].bEnabled)
		{
			Elevation  = FirstLayerValue;
		}
	}
	
	for (int Index = 1; Index < NoiseFilters.Num(); Index++)
	{
		if (ShapeConfig->NoiseLayers[Index].bEnabled)
		{
			float MaskValue = (ShapeConfig->NoiseLayers[Index].bUseFirstLayerAsMask) ? FirstLayerValue : 1;
			Elevation += NoiseFilters[Index]->Evaluate(PointOnUnitSphere) * MaskValue;
		}
	}

	Elevation = ShapeConfig->PlanetRadius * (1 + Elevation);
	MinMaxFinder.FindMinAndMax(Elevation);
	return PointOnUnitSphere * Elevation;
}
