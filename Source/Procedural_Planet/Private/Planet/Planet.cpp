#include "Planet/Planet.h"
#include "Config/ColorConfig.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Planet/TerrainFace.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APlanet::BeginPlay()
{
	GeneratePlanet();
}

void APlanet::GeneratePlanet()
{
	// UpVectors
	TArray Directions =
		{
		FVector::ForwardVector,
		FVector::BackwardVector,
		FVector::LeftVector,
		FVector::RightVector,
		FVector::UpVector,
		FVector::DownVector
		};

	// Planet Generation
	for (int i = 0; i < FaceAmount; i++)
	{
		// Face Creation
		ATerrainFace* TerrainFace = GetWorld()->SpawnActor<ATerrainFace>(ATerrainFace::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (TerrainFace && MaterialIntarface)
		{
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialIntarface, this);

			// Assign its root
			TerrainFace->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			// İnitialize variables values
			TerrainFace->Initialize(Resolution, Directions[i], ColorConfig->PlanetColor, DynamicMaterial);
			// Generate just one face
			TerrainFace->ConstructMesh();
		}
	}
}