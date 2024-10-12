// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet/ColorGenerator.h"

#include "Config/ColorConfig.h"
#include "Config/MinMaxFinder.h"

UColorGenerator::UColorGenerator()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UColorGenerator::Initialize(UColorConfig* Config)
{
	if (Config)
	{
		ColorConfig = Config;
	}
}

void UColorGenerator::UpdateElevation(FMinMaxFinder MinMax)
{
	if (ColorConfig && ColorConfig->DynamicMaterial)
	{
		ColorConfig->DynamicMaterial->SetVectorParameterValue("ElevationMinMax", FLinearColor(MinMax.MinPointOnPlanet, MinMax.MaxPointOnPlanet, 0, 0));
	}
}
