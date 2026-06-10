#ifndef MAMDANI_H
#define MAMDANI_H

#include "FuzzyLogic.h"

typedef enum {
    S_BLACK = 0,
    S_GRAY,
    S_WHITE,
} SensorFuzzySet_e;

typedef enum {
    O_LOW = 0,
    O_MID,
    O_HIGH,
} OutputFuzzySet_e;

typedef struct {
    SensorFuzzySet_e left;
    SensorFuzzySet_e right;
    OutputFuzzySet_e mi;
    OutputFuzzySet_e md;
    float weight; // rule weight (0..1)
} MamdaniRule_t;

typedef struct {
    float left;
    float right;
} MotorCommand_t;

// Evaluates two motor outputs (MI and MD) from the sensor readings.
void Mamdani_Infer(float left_raw, float right_raw,
                   float *mi_low, float *mi_mid, float *mi_high,
                   float *md_low, float *md_mid, float *md_high);

// Access to default rule base (modifiable by user)
const MamdaniRule_t* Mamdani_GetRules(size_t *count);

// Initialize Mamdani subsystem (calls Centroide init if needed)
void Mamdani_Init(void);

// Run full pipeline: infer + defuzzify -> returns control value
float Mamdani_Defuzz(float left_raw, float right_raw);

// Builds a two-motor command (MI, MD) from the fuzzy rule base.
void Mamdani_MotorCommand(float left_raw, float right_raw, MotorCommand_t *cmd);

#endif // MAMDANI_H
