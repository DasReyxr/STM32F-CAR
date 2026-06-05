#ifndef FUZZYLOGIC_H
#define FUZZYLOGIC_H


#define LOW 0
#define HIGH 1

typedef struct {
    float Inf;
    float Mid;
    float Sup;
} FuzzyArgs;

float Trapz(float x, FuzzyArgs args, uint8_t type);
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