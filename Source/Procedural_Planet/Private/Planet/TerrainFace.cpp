// Fill out your copyright notice in the Description page of Project Settings.

#include "Planet/TerrainFace.h"
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
	Resolution = Res; // Face Amount in 1 line
	LocalUp = LocalUpVector; // Vector through the up from the face
    AxisA = FVector(LocalUp.Y, LocalUp.Z, LocalUp.X); // Perpendicular Vector component of LocalUp
	AxisB = FVector::CrossProduct(LocalUp, AxisA); // Perpendicular Vector component of LocalUpf

    // Containers to construct mesh
    Vertices.SetNum(Resolution * Resolution);
    Triangles.SetNum((Resolution - 1) * (Resolution - 1) * 6);
    Normals.SetNum(Vertices.Num());
    Tangents.SetNum(Vertices.Num());
    UVs.SetNum(Vertices.Num());  // UVs array
    VertexColors.SetNum(Vertices.Num()); // Vertex colors array
}

void ATerrainFace::ConstructMesh()
{
    CalculateMeshComponents();
    
    // Mesh Creation
    Mesh->ClearMeshSection(0);
    RecalculateNormals();
    // Create mesh section with UVs and vertex colors
    Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void ATerrainFace::RecalculateNormals()
{
    Normals.SetNum(Vertices.Num());
    for (int32 i = 0; i < Normals.Num(); i++)
    {
        // All vectors are zerod out, because we need the value itself.
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

void ATerrainFace::CalculateMeshComponents()
{
    if (ShapeGenerator == nullptr) { return; }


    for (int Y = 0; Y < Resolution; Y++)
    {
        for (int X = 0; X < Resolution; X++)
        {
            int Index = X + Y * Resolution;

            // Calculate UV coordinates
            FVector2D UV = FVector2D(static_cast<float>(X) / (Resolution - 1), static_cast<float>(Y) / (Resolution - 1));
            UVs[Index] = UV; // Assign UV coordinate

            // Calculate position on the cube and then on the planet
            FVector2D Percent = FVector2D(X, Y) / (Resolution - 1);
            FVector PointOnUnitCube = LocalUp + (Percent.X - 0.5f) * 2 * AxisA + (Percent.Y - 0.5f) * 2 * AxisB;
            FVector PointOnUnitSphere = -PointOnUnitCube.GetSafeNormal();
            Vertices[Index] = ShapeGenerator->CalculatePointOnPlanet(PointOnUnitSphere);

            if (X != Resolution - 1 && Y != Resolution - 1)
            {
                int32 Index1 = Index + Resolution + 1;
                int32 Index2 = Index + Resolution;
                if (Index != Index1 && Index1 != Index2 && Index2 != Index) {
                    Triangles.Add(Index);
                    Triangles.Add(Index1);
                    Triangles.Add(Index2);
                }

                Index1 = Index + 1;
                Index2 = Index + Resolution + 1;
                if (Index != Index1 && Index1 != Index2 && Index2 != Index) {
                    Triangles.Add(Index);
                    Triangles.Add(Index1);
                    Triangles.Add(Index2);
                }
            }
        }
    }
}

void ATerrainFace::CalculateVertexColor(const float& Min, const float& Max)
{
    for(int Index = 0; Index < Vertices.Num(); Index++)
    {
        float DistanceFromPlanetCenter = Vertices[Index].Length();
        // Inversed lerp (linear interpolation) for normalizing between 0 and 1
        float InversedLerp = (DistanceFromPlanetCenter - Min) / (Max - Min);

        // Smoothstep function for smoother transitions
        InversedLerp = FMath::Clamp(InversedLerp, 0.0f, 1.0f);
        InversedLerp = FMath::SmoothStep(0.0f, 1.0f, InversedLerp);

        // Interpolating the color from black to white based on the smoothed value
        VertexColors[Index] = FColor(InversedLerp * 255, InversedLerp * 255, InversedLerp * 255, 255);
    }
}

