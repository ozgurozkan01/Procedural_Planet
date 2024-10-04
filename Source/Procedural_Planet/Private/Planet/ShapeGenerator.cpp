// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet/ShapeGenerator.h"

UShapeGenerator::UShapeGenerator()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}

void UShapeGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void UShapeGenerator::Initialize(UShapeConfig* Config)
{
	ShapeConfig = Config;
}

void UShapeGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

