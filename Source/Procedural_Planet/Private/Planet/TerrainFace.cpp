// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet/TerrainFace.h"
#include "ProceduralMeshComponent.h"

ATerrainFace::ATerrainFace()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = Mesh;
}

void ATerrainFace::Initialize(int Res, FVector LocalUpVector, UMaterial* MeshMaterial)
{
	Material = MeshMaterial;
	Resolution = Res;
	LocalUp = LocalUpVector;
	AxisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	AxisB = FVector::CrossProduct(LocalUp, AxisA);
}

void ATerrainFace::ConstructMesh()
{
	TArray<FVector> Vertices;
	Vertices.SetNum(Resolution * Resolution);

	TArray<int32> Triangles;
	Triangles.SetNum((Resolution - 1) * (Resolution - 1) * 6);

	TArray<FVector> Normals;
	Normals.SetNum(Vertices.Num());

	TArray<FProcMeshTangent> Tangents;
	Tangents.SetNum(Vertices.Num());

	for (int Y = 0; Y < Resolution; Y++)
	{
		for (int X = 0; X < Resolution; X++)
		{
			int Index = X + Y * Resolution;
			FVector2D Percent = FVector2D(X, Y) / (Resolution - 1);
			FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisA + (Percent.Y - 0.5f) * 2 * AxisB;
			FVector PointOnUnitSphere = -PointOnUnitCube.GetSafeNormal() * 25.f;

			// Set the vertex position
			Vertices[Index] = PointOnUnitSphere;

			// Calculate and assign the normals for shading
			Normals[Index] = PointOnUnitSphere.GetSafeNormal();

			// Optionally set tangents (here we use an arbitrary direction)
			FVector TangentDir = FVector(1, 0, 0); // This should be changed depending on your mesh geometry
			Tangents[Index] = FProcMeshTangent(TangentDir, false); // Assuming the tangent is left-handed
			

			// Create triangles for the mesh
			if (X != Resolution - 1 && Y != Resolution - 1)
			{
				Triangles.Add(Index);
				Triangles.Add(Index + Resolution + 1);
				Triangles.Add(Index + Resolution);

				Triangles.Add(Index);
				Triangles.Add(Index + 1);
				Triangles.Add(Index + Resolution + 1);
			}
		}
	}

	// Create the procedural mesh section with vertices, triangles, normals, tangents, and empty arrays for UVs and colors
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, TArray<FVector2D>(), TArray<FColor>(), Tangents, true);

	// Set the material for the mesh
	Mesh->SetMaterial(0, Material);
}
