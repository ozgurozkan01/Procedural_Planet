// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet/TerrainFace.h"
#include "ProceduralMeshComponent.h"
#include "Planet/ShapeGenerator.h"

ATerrainFace::ATerrainFace()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = Mesh;
}

void ATerrainFace::Initialize(UShapeGenerator* Generator, int Res, FVector LocalUpVector)
{
    ShapeGenerator = Generator;
	Resolution = Res; 
	LocalUp = LocalUpVector;
    AxisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X);
	AxisB = FVector::CrossProduct(LocalUp, AxisA);
    Size = 100;
}

void ATerrainFace::ConstructMesh()
{
    if (ShapeGenerator == nullptr) { return; }
    
    // Containers to construct mesh
    TArray<FVector>          Vertices;  Vertices.SetNum(Resolution * Resolution);
    TArray<int32>            Triangles; Triangles.SetNum((Resolution - 1) * (Resolution - 1) * 6);
    TArray<FVector>          Normals;   Normals.SetNum(Vertices.Num());
    TArray<FProcMeshTangent> Tangents;  Tangents.SetNum(Vertices.Num());
    
    for (int Y = 0; Y < Resolution; Y++)
    {
        for (int X = 0; X < Resolution; X++)
        {
            int Index = X + Y * Resolution;
            // Represents the normalized (between 0 and 1) coordinates of a point on the grid along the X and Y axes.
            FVector2D Percent = FVector2D(X, Y) / (Resolution - 1);
            // Represents the 3D point calculated on the surface of the cube based on the normalized grid coordinates.
            FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisA + (Percent.Y - 0.5f) * 2 * AxisB;
            // Arranging shading better, multiply by minus 1 normalized vector.
            FVector PointOnUnitSphere = -PointOnUnitCube.GetSafeNormal();

            Vertices[Index] = ShapeGenerator->CalculatePointOnPlanet(PointOnUnitSphere);

            if (X != Resolution - 1 && Y != Resolution - 1)
            {
                // Assign the triangle points accordingly clockwise. 
                Triangles.Add(Index);
                Triangles.Add(Index + Resolution + 1);
                Triangles.Add(Index + Resolution);

                Triangles.Add(Index);
                Triangles.Add(Index + 1);
                Triangles.Add(Index + Resolution + 1);
            }
        }
    }

    // Mesh Creation
    Mesh->ClearMeshSection(0);
    RecalculateNormals(Vertices, Triangles, Normals);
    Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, TArray<FVector2D>(), TArray<FColor>(), Tangents, true);
}

void ATerrainFace::RecalculateNormals(const TArray<FVector>& Vertices, const TArray<int32>& Triangles, TArray<FVector>& Normals)
{
    Normals.SetNum(Vertices.Num());
    for (int32 i = 0; i < Normals.Num(); i++)
    {
        Normals[i] = FVector::ZeroVector;
    }

    for (int32 i = 0; i < Triangles.Num(); i += 3)
    {
        // Get the index of edges on the triangle
        int32 Index0 = Triangles[i];
        int32 Index1 = Triangles[i + 1];
        int32 Index2 = Triangles[i + 2];
        // Get the coordinates of edges on the triangle
        FVector Vertex0 = Vertices[Index0];
        FVector Vertex1 = Vertices[Index1];
        FVector Vertex2 = Vertices[Index2];
        
        FVector Edge1 = Vertex1 - Vertex0;
        FVector Edge2 = Vertex2 - Vertex0;
        // Normal vector calculation from edges of triangle
        FVector FaceNormal = FVector::CrossProduct(Edge1, Edge2).GetSafeNormal();

        // Smooth shading.
        Normals[Index0] += FaceNormal;
        Normals[Index1] += FaceNormal;
        Normals[Index2] += FaceNormal;
    }

    for (int32 i = 0; i < Normals.Num(); i++)
    {
        Normals[i] = -Normals[i].GetSafeNormal();
    }
}