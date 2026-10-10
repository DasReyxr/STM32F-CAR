#include "Mamdani.h"
#include "Centroide.h"

// Default rule base for two motors (MI and MD).
// Circuit with WHITE track on BLACK background.
// Each rule now defines the consequent for both motors.
static const MamdaniRule_t default_rules[] = {
    // Both sensors over the white line -> both motors moderate (follow line)
    // Centrado perfecto -> ambos motores al máximo
{S_WHITE, S_WHITE,  O_HIGH, O_HIGH, 1.0f},

// Borde izquierdo (L en gris, R en blanco)
// -> Ligeramente a la izquierda: reducir motor derecho
{S_GRAY,  S_WHITE,  O_HIGH, O_MID,  2.0f},

// Borde derecho (L en blanco, R en gris)
// -> Ligeramente a la derecha: reducir motor izquierdo
{S_WHITE, S_GRAY,   O_MID,  O_HIGH, 2.0f},

// Desvío fuerte izquierda (L en negro, R en blanco)
// -> Girar fuerte a la izquierda: motor L alto, motor R muy bajo
{S_BLACK, S_WHITE,  O_HIGH, O_LOW,  2.0f},

// Desvío fuerte derecha (L en blanco, R en negro)
// -> Girar fuerte a la derecha: motor L muy bajo, motor R alto
{S_WHITE, S_BLACK,  O_LOW,  O_HIGH, 2.0f},

// Ambos en borde (curva suave o línea gruesa) -> velocidad media
{S_GRAY,  S_GRAY,   O_MID,  O_MID,  1.0f},

// L en negro, R en gris -> desvío moderado-izquierda
{S_BLACK, S_GRAY,   O_HIGH, O_LOW,  1.0f},

// L en gris, R en negro -> desvío moderado-derecha
{S_GRAY,  S_BLACK,  O_LOW,  O_HIGH, 1.0f},

// Ambos en negro (perdió la línea completamente) -> detener
{S_BLACK, S_BLACK,  O_LOW,  O_HIGH,  1.0f},
};

const MamdaniRule_t* Mamdani_GetRules(size_t *count)
{
    if (count) *count = sizeof(default_rules)/sizeof(default_rules[0]);
    return default_rules;
}

// Map fuzzy set enum to membership function
static float sensor_membership(SensorFuzzySet_e set, float raw)
{
    switch(set){
        case S_BLACK: return Black(raw);
        case S_GRAY:  return Gray(raw);
        case S_WHITE: return White(raw);
        default: return 0.0f;
    }
}

void Mamdani_Infer(float left_raw, float right_raw,
                   float *mi_low, float *mi_mid, float *mi_high,
                   float *md_low, float *md_mid, float *md_high)
{
    float a_mi_low = 0.0f, a_mi_mid = 0.0f, a_mi_high = 0.0f;
    float a_md_low = 0.0f, a_md_mid = 0.0f, a_md_high = 0.0f;
    size_t n;
    const MamdaniRule_t *rules = Mamdani_GetRules(&n);

    for(size_t i=0;i<n;i++){
        const MamdaniRule_t *r = &rules[i];
        float ml = sensor_membership(r->left, left_raw);
        float mr = sensor_membership(r->right, right_raw);
        float strength = MamdaniRuleStrength(ml, mr) * r->weight;

        switch(r->mi){
            case O_LOW:  if (strength > a_mi_low) a_mi_low = strength; break;
            case O_MID:  if (strength > a_mi_mid) a_mi_mid = strength; break;
            case O_HIGH: if (strength > a_mi_high) a_mi_high = strength; break;
        }

        switch(r->md){
            case O_LOW:  if (strength > a_md_low) a_md_low = strength; break;
            case O_MID:  if (strength > a_md_mid) a_md_mid = strength; break;
            case O_HIGH: if (strength > a_md_high) a_md_high = strength; break;
        }
    }

    if(mi_low)  *mi_low  = a_mi_low;
    if(mi_mid)  *mi_mid  = a_mi_mid;
    if(mi_high) *mi_high = a_mi_high;

    if(md_low)  *md_low  = a_md_low;
    if(md_mid)  *md_mid  = a_md_mid;
    if(md_high) *md_high = a_md_high;
}

void Mamdani_Init(void)
{
    Centroide_Init();
}
/*
float Mamdani_Defuzz(float left_raw, float right_raw)
{
    float mi_low = 0.0f, mi_mid = 0.0f, mi_high = 0.0f;
    float md_low = 0.0f, md_mid = 0.0f, md_high = 0.0f;
    Mamdani_Infer(left_raw, right_raw,
                  &mi_low, &mi_mid, &mi_high,
                  &md_low, &md_mid, &md_high);

    const float left_cmd  = Centroide(mi_low, mi_mid, mi_high);
    const float right_cmd = Centroide(md_low, md_mid, md_high);
    (void)right_cmd;
    return left_cmd;
}
*/
void Mamdani_MotorCommand(float left_raw, float right_raw, MotorCommand_t *cmd)
{
    float mi_low = 0.0f, mi_mid = 0.0f, mi_high = 0.0f;
    float md_low = 0.0f, md_mid = 0.0f, md_high = 0.0f;

    Mamdani_Infer(left_raw, right_raw,
                  &mi_low, &mi_mid, &mi_high,
                  &md_low, &md_mid, &md_high);

    if (cmd != 0)
    {
        cmd->left  = Centroide(mi_low, mi_mid, mi_high);
        cmd->right = Centroide(md_low, md_mid, md_high);
    }
}
