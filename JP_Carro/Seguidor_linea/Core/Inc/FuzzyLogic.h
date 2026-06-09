#ifndef FUZZYLOGIC_H
#define FUZZYLOGIC_H

#include "stm32f4xx_hal.h"

#define TRAPZ_LOW 0
#define TRAPZ_HIGH 1

typedef struct {
    float L; // Left
    float C; // Center
    float R; // Right
} FuzzyArgs;

float Trapz(float x, FuzzyArgs args, uint8_t type);
float Triang(float x, FuzzyArgs args);

//-------- Sensors --------
float Black(float x);
float Gray(float x);
float White(float x);

//-------- Motors --------
float Lowf(float x);
float Mediumf(float x);
float Highf(float x);
void areas(void);
 
static const FuzzyArgs Low = {0.0f, 0.0f, 50.0f};
static const FuzzyArgs Mid = {25.0f, 50.0f, 75.0f};
static const FuzzyArgs High = {50.0f, 0.0f, 100.0f};

static const uint16_t limit_High = 3000; 

#endif // FUZZYLOGIC_H