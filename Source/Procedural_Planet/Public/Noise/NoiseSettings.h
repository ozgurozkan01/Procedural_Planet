#pragma once

#include "CoreMinimal.h"
#include "NoiseSettings.generated.h"

UENUM(BlueprintType)
enum class EFilterType : uint8
{
	Simple,
	Ridgid,
};

USTRUCT(BlueprintType)
struct FSimpleNoiseSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings",  meta = (ClampMin = "1", ClampMax = "8", UIMin = "1", UIMax = "8"))
	int32 LayerNum = 1; // It is used for more realistic surface
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float Strength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float Roughness = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float BaseRoughness = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float Persistence = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	FVector Centre;
};

USTRUCT(BlueprintType)
struct FRidgidNoiseSettings : public FSimpleNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	float WeightMultiplier = 0.8f;
};

USTRUCT(BlueprintType)
struct FNoiseSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings")
	EFilterType FilterType = EFilterType::Simple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (EditCondition = "FilterType == EFilterType::Simple", EditConditionHides))
	FSimpleNoiseSettings SimpleNoiseSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise Settings", meta = (EditCondition = "FilterType == EFilterType::Ridgid", EditConditionHides))
	FRidgidNoiseSettings RidgidNoiseSettings;
};