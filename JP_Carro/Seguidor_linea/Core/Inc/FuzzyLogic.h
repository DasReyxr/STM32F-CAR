#ifndef FUZZYLOGIC_H
#define FUZZYLOGIC_H

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
#endif // FUZZYLOGIC_H