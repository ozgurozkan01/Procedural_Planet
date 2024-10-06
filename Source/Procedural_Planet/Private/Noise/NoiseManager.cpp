// Fill out your copyright notice in the Description page of Project Settings.

/*
	This file is part of libnoise-dotnet.
	libnoise-dotnet is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libnoise-dotnet is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libnoise-dotnet.  If not, see <http://www.gnu.org/licenses/>.

	Simplex Noise in 2D, 3D and 4D. Based on the example code of this paper:
	http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf

	From Stefan Gustavson, Linkping University, Sweden (stegu at itn dot liu dot se)
	From Karsten Schmidt (slight optimizations & restructuring)

	Some changes by Sebastian Lague for use in a tutorial series.
	*/

	/*
	 * Noise module that outputs 3-dimensional Simplex Perlin noise.
	 * This algorithm has a computational cost of O(n+1) where n is the dimension.
	 * 
	 * This noise module outputs values that usually range from
	 * -1.0 to +1.0, but there are no guarantees that all output values will exist within that range.
*/

#include "Noise/NoiseManager.h"

// Sets default values for this component's properties
UNoiseManager::UNoiseManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	Source = {
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
		8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203,
		117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165,
		71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
		55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
		18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
		124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189,
		28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
		242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
		181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114,
		67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

	Grad3  =
	{
		{1, 1, 0}, {-1, 1, 0},  {1, -1, 0},
		{-1, -1, 0}, {1, 0, 1},  {-1, 0, 1},
		{1, 0, -1},  {-1, 0, -1}, {0, 1, 1},
		{0, -1, 1},  {0, 1, -1},  {0, -1, -1}
	};

	Randomize(0);
}

void UNoiseManager::InitializeRandomize(int32 Seed)
{
	Randomize(Seed);
}

void UNoiseManager::Randomize(int32 Seed)
{
	RandomBuffer.SetNum(RandomSize * 2);
	
	if (Seed != 0)
	{
		// Shuffle the array using the given seed
		// Unpack the seed into 4 bytes then perform a bitwise XOR operation
		// with each byte
		TArray<int8> Buffer;
		Buffer.SetNum(4);
		UnpackLittleUint32(Seed, Buffer);

		for (int i = 0; i < Source.Num(); i++)
		{
			RandomBuffer[i] = Source[i] ^ Buffer[0];
			RandomBuffer[i] ^= Buffer[1];
			RandomBuffer[i] ^= Buffer[2];
			RandomBuffer[i] ^= Buffer[3];

			RandomBuffer[i + RandomSize] = RandomBuffer[i];
		}

	}
	else
	{
		for (int i = 0; i < RandomSize; i++)
			RandomBuffer[i + RandomSize] = RandomBuffer[i] = Source[i];
	}
}

float UNoiseManager::Evaluate(FVector Point)
{
		double X = Point.X;
        double Y = Point.Y;
        double Z = Point.Z;
	
        double N0 = 0, N1 = 0, N2 = 0, N3 = 0;

        // Noise contributions from the four corners
        // Skew the input space to determine which simplex cell we're in
        double S = (X + Y + Z)*F3;

        // for 3D
        int I = FastFloor(X + S);
        int J = FastFloor(Y + S);
        int K = FastFloor(Z + S);

        double T = (I + J + K)*G3;

        // The x,y,z distances from the cell origin
        double X0 = X - (I - T);
        double Y0 = Y - (J - T);
        double Z0 = Z - (K - T);

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        // Offsets for second corner of simplex in (i,j,k)
        int I1, J1, K1;

        // coords
        int I2, J2, K2; // Offsets for third corner of simplex in (i,j,k) coords

        if (X0 >= Y0)
        {
            if (Y0 >= Z0)
            {
                // X Y Z order
                I1 = 1;
                J1 = 0;
                K1 = 0;
                I2 = 1;
                J2 = 1;
                K2 = 0;
            }
            else if (X0 >= Z0)
            {
                // X Z Y order
                I1 = 1;
                J1 = 0;
                K1 = 0;
                I2 = 1;
                J2 = 0;
                K2 = 1;
            }
            else
            {
                // Z X Y order
                I1 = 0;
                J1 = 0;
                K1 = 1;
                I2 = 1;
                J2 = 0;
                K2 = 1;
            }
        }
        else
        {
            // x0 < y0
            if (Y0 < Z0)
            {
                // Z Y X order
                I1 = 0;
                J1 = 0;
                K1 = 1;
                I2 = 0;
                J2 = 1;
                K2 = 1;
            }
            else if (X0 < Z0)
            {
                // Y Z X order
                I1 = 0;
                J1 = 1;
                K1 = 0;
                I2 = 0;
                J2 = 1;
                K2 = 1;
            }
            else
            {
                // Y X Z order
                I1 = 0;
                J1 = 1;
                K1 = 0;
                I2 = 1;
                J2 = 1;
                K2 = 0;
            }
        }

        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z),
        // and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z),
        // where c = 1/6.

        // Offsets for second corner in (x,y,z) coords
        double x1 = X0 - I1 + G3;
        double y1 = Y0 - J1 + G3;
        double z1 = Z0 - K1 + G3;

        // Offsets for third corner in (x,y,z)
        double x2 = X0 - I2 + F3;
        double y2 = Y0 - J2 + F3;
        double z2 = Z0 - K2 + F3;

        // Offsets for last corner in (x,y,z)
        double x3 = X0 - 0.5;
        double y3 = Y0 - 0.5;
        double z3 = Z0 - 0.5;

        // Work out the hashed gradient indices of the four simplex corners
        int ii = I & 0xff;
        int jj = J & 0xff;
        int kk = K & 0xff;

        // Calculate the contribution from the four corners
        double t0 = 0.6 - X0*X0 - Y0*Y0 - Z0*Z0;
        if (t0 > 0)
        {
            t0 *= t0;
            int gi0 = RandomBuffer[ii + RandomBuffer[jj + RandomBuffer[kk]]]%12;
            N0 = t0*t0*Dot(Grad3[gi0], X0, Y0, Z0);
        }

        double t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
        if (t1 > 0)
        {
            t1 *= t1;
            int gi1 = RandomBuffer[ii + I1 + RandomBuffer[jj + J1 + RandomBuffer[kk + K1]]]%12;
            N1 = t1*t1*Dot(Grad3[gi1], x1, y1, z1);
        }

        double t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
        if (t2 > 0)
        {
            t2 *= t2;
            int gi2 = RandomBuffer[ii + I2 + RandomBuffer[jj + J2 + RandomBuffer[kk + K2]]]%12;
            N2 = t2*t2*Dot(Grad3[gi2], x2, y2, z2);
        }

        double t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
        if (t3 > 0)
        {
            t3 *= t3;
            int gi3 = RandomBuffer[ii + 1 + RandomBuffer[jj + 1 + RandomBuffer[kk + 1]]]%12;
            N3 = t3*t3*Dot(Grad3[gi3], x3, y3, z3);
        }

	return (float)(N0 + N1 + N2 + N3)*32;
}

int32 UNoiseManager::FastFloor(double x) { return x >= 0 ? static_cast<int32>(x) : static_cast<int32>(x) - 1; }

TArray<int8> UNoiseManager::UnpackLittleUint32(int32 Value, TArray<int8>& Buffer)
{
	if (Buffer.Num() < 4)
	{
		Buffer.SetNum(4);
	}
	
	Buffer[0] = (Value & 0x00ff);
	Buffer[1] = (Value & 0xff00) >> 8;
	Buffer[2] = (Value & 0x00ff0000) >> 16;
	Buffer[3] = (Value & 0xff000000) >> 24;

	return Buffer;
}

double UNoiseManager::Dot(const TArray<int32> Grad, double X, double Y, double Z, double T) { return Grad[0] * X + Grad[1] * Y + Grad[2] * Z + Grad[3] * T; }
double UNoiseManager::Dot(const TArray<int32> Grad, double X, double Y, double Z) { return Grad[0] * X + Grad[1] * Y + Grad[2] * Z; }
double UNoiseManager::Dot(const TArray<int32> Grad, double X, double Y) { 	return Grad[0] * X + Grad[1] * Y; }
