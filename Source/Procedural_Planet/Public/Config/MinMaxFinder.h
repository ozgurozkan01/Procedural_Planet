#pragma once

#include "CoreMinimal.h"
#include "MinMaxFinder.generated.h"	

USTRUCT(BlueprintType)
struct FMinMaxFinder
{
	GENERATED_BODY()

	FMinMaxFinder()
	{
		MinPointOnPlanet = FLT_MAX;
		MaxPointOnPlanet = -FLT_MAX;
	}
	
	UPROPERTY(VisibleAnywhere, Category=MinMax)
	float MinPointOnPlanet;
	UPROPERTY(VisibleAnywhere, Category=MinMax)
	float MaxPointOnPlanet;	

	void FindMinAndMax(float Value)
	{
		if (Value < MinPointOnPlanet) { MinPointOnPlanet = Value; }
		if (Value > MaxPointOnPlanet) { MaxPointOnPlanet = Value; }
	}
};