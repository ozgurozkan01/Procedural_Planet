#pragma once

#include "CoreMinimal.h"
#include "MinMaxFinder.generated.h"	

USTRUCT(BlueprintType)
struct FMinMaxFinder
{
	GENERATED_BODY()

	FMinMaxFinder() {}
	
	UPROPERTY(VisibleAnywhere, Category=MinMax)
	float MinPointOnPlanet = FLT_MAX;
	UPROPERTY(VisibleAnywhere, Category=MinMax)
	float MaxPointOnPlanet = FLT_MIN;	

	void FindMinAndMax(float Value)
	{
		if (Value < MinPointOnPlanet) { MinPointOnPlanet = Value; }
		if (Value > MaxPointOnPlanet) { MaxPointOnPlanet = Value; }
	}
};