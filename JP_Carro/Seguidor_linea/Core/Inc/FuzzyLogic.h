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

static const float M_LOW  = -1.0f / (Low.R - Low.L);
static const float M_MID0 =  1.0f / (Mid.C - Mid.L);
static const float M_MID1 = -1.0f / (Mid.R - Mid.C);
static const float M_HIGH =  1.0f / (High.R - High.L);

static const uint16_t limit_High = 3000; 

#endif // FUZZYLOGIC_H