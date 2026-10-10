#ifndef CENTROIDE_H
#define CENTROIDE_H

#include "FuzzyLogic.h"

// Pendientes
static const float M_LOW  = 1.0f / (Low.R - Low.L);
static const float M_MID0 = 1.0f / (Mid.C - Mid.L);
static const float M_MID1 = 1.0f / (Mid.R - Mid.C);
static const float M_HIGH = 1.0f / (High.R - High.L);


// ################## Functions ##################
typedef struct {
    float num; // AiCi 
    float den; // Ai
} DefuzzArgs;

float Centroide(float y_low, float y_mid, float y_high);
void Centroide_Init(void);

DefuzzArgs Area_Low(float y_low);
DefuzzArgs Area_Mid(float y_mid);
DefuzzArgs Area_High(float y_high);

DefuzzArgs Area_InterLm(float y_low, float y_mid);
DefuzzArgs Area_InterMh(float y_mid, float y_high);


static inline float f_Low(float y){
    return (Low.R - y)/M_LOW;
}

static inline float f_Mid1(float y){
    return (y - Mid.L)/M_MID0;
}
static inline float f_Mid2(float y){
    return (Mid.R - y)/M_MID1;
}

static inline float f_High(float y){
    return (y-High.L)/M_HIGH;
}

static inline float Centroid_T(float x0,float x1,float x2){
    return (x0+x1+x2)/3.0f;
}

static inline float Centroid_T_R(float x0, float x1)
{
    return x0 + ((x1 - x0) * (2.0f / 3.0f));
}

static inline float Centroid_Sq(float x0, float x1)
{
    return (x0 + x1) / 2.0f;
}

#endif // FUZZYLOGIC_H
