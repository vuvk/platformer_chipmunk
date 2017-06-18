#pragma once
#ifndef ADDITIONS_H
#define ADDITIONS_H

#include "defines.h"

#define SQR(X) (X)*(X)

bool FileExists (const char* fileName);

void Swapc (char** a, char** b);
void Swaps (short* a, short* b);
void Swapi (int* a, int* b);
void Swapf (float* a, float* b);

void LimitShort (short* a, short min, short max);
void LimitInt (int* a, int min, int max);
void LimitFloat (float* a, float min, float max);

float DegToRad (float degree);
float RadToDeg (float radian);

byte StrEqual (const char* str1, const char* str2);

#endif // ADDITIONS_H
