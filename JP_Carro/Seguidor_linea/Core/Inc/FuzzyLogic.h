#ifndef FUZZYLOGIC_H
#define FUZZYLOGIC_H

typedef struct {
    float Inf;
    float Mid;
    float Sup;
} FuzzyArgs;

float Trapz(float x, FuzzyArgs args);
float Triang(float x, FuzzyArgs args);

// Sensores
float Black(float x);
float Gray(float x);
float White(float x);

// Motores
float Low(float x);
float Medium(float x);
float High(float x);

#endif // FUZZYLOGIC_H