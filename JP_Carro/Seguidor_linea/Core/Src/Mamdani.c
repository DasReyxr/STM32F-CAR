#include "Mamdani.h"
#include "Centroide.h"

// Default rule base for two motors (MI and MD).
// Circuit with WHITE track on BLACK background.
// Each rule now defines the consequent for both motors.
static const MamdaniRule_t default_rules[] = {
    // Both sensors over the white line -> both motors moderate (follow line)
    {S_WHITE, S_WHITE, O_MID, O_MID, 1.0f},

    // Slight left drift (left on white, right on gray) -> reduce right motor, boost left
    {S_WHITE, S_GRAY,  O_MID, O_LOW, 1.0f},
    
    // Slight right drift (right on white, left on gray) -> reduce left motor, boost right
    {S_GRAY,  S_WHITE, O_LOW, O_MID, 1.0f},

    // Strong drift toward the left (right sensor left track) -> left motor very weak, right stronger
    {S_GRAY, S_BLACK, O_LOW, O_HIGH, 1.0f},
    
    // Strong drift toward the right (left sensor left track) -> left motor stronger, right very weak
    {S_BLACK, S_GRAY, O_HIGH, O_LOW, 1.0f},

    // Both sensors off the track (black background) -> stop or slow down
    {S_BLACK, S_BLACK, O_LOW, O_LOW, 1.0f},

    // Gray + Gray (both between line and background) -> both moderate
    {S_GRAY,  S_GRAY,  O_MID, O_MID, 1.0f},
    
    // One on white, one completely off -> stop/reverse
    {S_WHITE, S_BLACK, O_LOW, O_HIGH, 1.0f},
    {S_BLACK, S_WHITE, O_HIGH, O_LOW, 1.0f},
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
        cmd->left  = Centroide(mi_low, mi_mid, mi_high) / 100.0f;
        cmd->right = Centroide(md_low, md_mid, md_high) / 100.0f;
    }
}
