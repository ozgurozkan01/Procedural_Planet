// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoiseManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCEDURAL_PLANET_API UNoiseManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNoiseManager();

	void InitializeRandomize(int32 Seed);
	float Evaluate(FVector Point);
private:
	void Randomize(int32 Seed);
	static int32 FastFloor(double x);
	static TArray<int8> UnpackLittleUint32(int32 Value, TArray<int8>& Buffer);
	double Dot(const TArray<int32> Grad, double X, double Y, double Z, double T);
	double Dot(const TArray<int32> Grad, double X, double Y, double Z);
	double Dot(const TArray<int32> Grad, double X, double Y);

	TArray<int32> Source;
	TArray<TArray<int32>> Grad3;

	TArray<int32> RandomBuffer;

	int32 RandomSize = 256;
	double Sqrt3 = 1.7320508075688772935;
	double Sqrt5 = 2.2360679774997896964;
	double F2 = 0.5*(Sqrt3 - 1.0);
	double G2 = (3.0 - Sqrt3)/6.0;
	double G22 = G2*2.0 - 1;
	double F3 = 1.0/3.0;
	double G3 = 1.0/6.0;
	double F4 = (Sqrt5 - 1.0)/4.0;
	double G4 = (5.0 - Sqrt5)/20.0;
	double G42 = G4*2.0;
	double G43 = G4*3.0;
	double G44 = G4*4.0 - 1.0;
};
