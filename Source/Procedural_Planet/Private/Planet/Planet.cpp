#include "Planet/Planet.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Planet/TerrainFace.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = false;

	FaceAmount = 6;
	Resolution = 3;
}

void APlanet::BeginPlay()
{
	GeneratePlanet();
}

void APlanet::GeneratePlanet()
{
	TArray UpVectors = {
		FVector::ForwardVector,
		FVector::BackwardVector,
		FVector::LeftVector,
		FVector::RightVector,
		FVector::UpVector,
		FVector::DownVector
	};

	for (int i = 0; i < FaceAmount; i++)
	{
		ATerrainFace* TerrainFace = GetWorld()->SpawnActor<ATerrainFace>(ATerrainFace::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (TerrainFace)
		{
			TerrainFace->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			TerrainFace->Initialize(Resolution, UpVectors[i], Material);
			TerrainFace->ConstructMesh();
		}
	}
}
