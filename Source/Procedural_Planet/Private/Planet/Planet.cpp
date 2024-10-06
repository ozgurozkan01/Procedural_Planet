#include "Planet/Planet.h"

#include "ProceduralMeshComponent.h"
#include "Config/ColorConfig.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Planet/ShapeGenerator.h"
#include "Planet/TerrainFace.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = false;

	ShapeGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("ShapeGenerator"));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> InterfaceMat(TEXT("/Script/Engine.Material'/Game/Material/MAT_Planet.MAT_Planet'"));
	static ConstructorHelpers::FObjectFinder<UColorConfig> ColorConf(TEXT("/Script/Procedural_Planet.ColorConfig'/Game/DataAsset/Color/DA_ColorConfig.DA_ColorConfig'"));
	static ConstructorHelpers::FObjectFinder<UShapeConfig> ShapeConf(TEXT("/Script/Procedural_Planet.ShapeConfig'/Game/DataAsset/Shape/DA_ShapeConfig.DA_ShapeConfig'"));

	if (InterfaceMat.Succeeded())
	{
		MaterialIntarface = InterfaceMat.Object;
	}
	if (ColorConf.Succeeded())
	{
		ColorConfig = ColorConf.Object;
	}
	if (ShapeConf.Succeeded())
	{
		ShapeConfig = ShapeConf.Object;
	}
}

void APlanet::BeginPlay()
{
	if (MaterialIntarface)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialIntarface, this);
	}
	if (ShapeConfig && ShapeGenerator)
	{
		ShapeGenerator->Initialize(ShapeConfig);
	}
	GeneratePlanet();
}

void APlanet::GeneratePlanet()
{
	Initialize();
	GenerateMesh();
	GenerateColor();
}

void APlanet::Initialize()
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
			TerrainFace->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			// Initialize variables values
			TerrainFace->Initialize(ShapeGenerator, Resolution, Directions[i]);
			// Add face into the buffer.
			Faces.Add(TerrainFace);
		}
	}
}

void APlanet::UpdateColor()
{
	Initialize();
	GenerateColor();
}

void APlanet::UpdateShape()
{
	Faces.Empty(); // Clear the buffer.
	Initialize();
	GenerateMesh();
}

void APlanet::GenerateMesh()
{
	for (int i = 0; i < FaceAmount; i++)
	{
		Faces[i]->ConstructMesh();
	}
}

void APlanet::GenerateColor()
{
	for (auto Face : Faces)
	{
		// Mesh Color Set
		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), ColorConfig->PlanetColor);
		Face->GetMesh()->SetMaterial(0, DynamicMaterial);
	}
}
