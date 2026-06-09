#ifndef CENTROIDE_H
#define CENTROIDE_H

#include "FuzzyLogic.h"

// Pendientes
const float M_LOW  = -1.0f / (Low.R - Low.L);
const float M_MID0 =  1.0f / (Mid.C - Mid.L);
const float M_MID1 = -1.0f / (Mid.R - Mid.C);
const float M_HIGH =  1.0f / (High.R - High.L);


// ################## Functions ##################

float Centroide(float y_low, float y_mid, float y_high);

float Centroid_Low(float y_low);
float Centroid_Mid(float y_mid);
float Centroid_High(float y_high);

float Centroid_InterLm(float y_low, float y_mid);
float Centroid_InterMh(float y_mid, float y_high);


float f_Low(float y){
    return (Low.R - y)/M_LOW;
}

float f_Mid1(float y){
    return (y - Mid.L)/M_MID0;
}
float f_Mid2(float y){
    return (Mid.R - y)/M_MID1;
}

float f_High(float y){
    return (y-High.L)/M_HIGH;
}

float Centroid_T(float x0,float x1,float x2){
    return (x0+x1+x2)/3.0f;
}

float Centroid_T_R(float x0, float x1)
{
    return x0 + ((x1 - x0) * (2.0f / 3.0f));
}

float Centroid_Sq(float x0, float x1)
{
    return (x0 + x1) / 2.0f;
}

// Puntos

const float Y1 = (Low.R * Mid.C - Low.L * Mid.L)/(Mid.C - Mid.L+ Low.R- Low.L);
const float X1 = f_Mid1(Y1);
const float Y2 = (High.R * Mid.R - High.L * Mid.C)/ (Mid.R-Mid.C+High.R-High.L);
const float X2 = f_High(Y2);

// Areas y Centroides 

const float TLeft = Y1*(Low.R-Mid.L)/2.0f;
const float TT = (Mid.R-Mid.L)/2.0f;
const float TRight = (Mid.R-High.L * Y2)/2.0f;

const float C_TLeft = Centroid_T(Low.R, Mid.L, X1);
const float C_TT = Centroid_T(Mid.L, Mid.R, Mid.C);
const float C_TRight = Centroid_T(Mid.R, High.L, X2);


#endif // FUZZYLOGIC_H