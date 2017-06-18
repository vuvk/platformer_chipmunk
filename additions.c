#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <string.h>


#include "SDL2/SDL.h"

#include "additions.h"

bool FileExists (const char* fileName)
{
    FILE* f = fopen (fileName, "r");

    if (f == NULL)
    {
        return false;
    }
    else
    {
        fclose (f);
        f = NULL;

        return true;
    }
}

void Swapc (char** a, char** b)
{
    char* t = *a;
    *a = *b;
    *b = t;
}

void Swaps (short* a, short* b)
{
    short t = *a;
    *a = *b;
    *b = t;
}

void Swapi (int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

void Swapf (float* a, float* b)
{
    float t = *a;
    *a = *b;
    *b = t;
}


void LimitShort (short* a, short min, short max)
{
    if (*a < min) *a = min;
    if (*a > max) *a = max;
}

void LimitInt (int* a, int min, int max)
{
    if (*a < min) *a = min;
    if (*a > max) *a = max;
}

void LimitFloat (float* a, float min, float max)
{
    if (*a < min) *a = min;
    if (*a > max) *a = max;
}


float DegToRad (float degree)
{
    static float coeff = 180.0f / M_PI;

    return (degree / coeff);
}

float RadToDeg (float radian)
{
    static float coeff = 180.0f / M_PI;

    return (radian * coeff);
}


byte StrEqual (const char* str1, const char* str2)
{
    // not correct!
    if (str1 == NULL || str1 == NULL)
        return -1;

    // strings equal
    if (strcmp (str1, str2) == 0)
        return true;

    // strings not equal
    return false;
}
