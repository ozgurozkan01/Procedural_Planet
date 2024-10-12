// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ColorGenerator.generated.h"


struct FMinMaxFinder;
class UColorConfig;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURAL_PLANET_API UColorGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UColorGenerator();

	void Initialize(UColorConfig* Config);
	void UpdateElevation(FMinMaxFinder MinMax);
	
private:

	UPROPERTY(VisibleAnywhere, Category=Config)
	UColorConfig* ColorConfig;
};
