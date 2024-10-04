// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Config/ShapeConfig.h"
#include "ShapeGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURAL_PLANET_API UShapeGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShapeGenerator();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialize(UShapeConfig* Config);
	FORCEINLINE FVector CalculatePointOnPlanet(FVector PointOnUnitSphere) const { return PointOnUnitSphere * ShapeConfig->PlanetRadius; };

private:
	UPROPERTY(EditDefaultsOnly, Category=Config)
	UShapeConfig* ShapeConfig;
};
