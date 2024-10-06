#pragma once

#include "CoreMinimal.h"
#include "SimpleNoiseFilter.h"
#include "RidgidNoiseFilter.h"
#include "NoiseFilterFactory.generated.h"

// CUSTOM CLASS
UCLASS()
class PROCEDURAL_PLANET_API UNoiseFilterFactory : public UObject
{
	GENERATED_BODY()

	UNoiseFilterFactory() {}
public:
	static INoiseFilterInterface* CreateNoiseFilter(const FNoiseSettings& Settings)
	{
		switch (Settings.FilterType)
		{
		case EFilterType::Simple:
			{
				USimpleNoiseFilter* SimpleNoiseFilter = NewObject<USimpleNoiseFilter>();
				SimpleNoiseFilter->Initialize(Settings);
				return SimpleNoiseFilter;
			}
		case EFilterType::Ridgid:
			{
				URidgidNoiseFilter* RidgidNoiseFilter = NewObject<URidgidNoiseFilter>();
				RidgidNoiseFilter->Initialize(Settings);
				return RidgidNoiseFilter;
			}
		default:
			return nullptr;
		}
	}
};
