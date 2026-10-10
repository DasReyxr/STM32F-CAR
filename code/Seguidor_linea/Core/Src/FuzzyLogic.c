#include "FuzzyLogic.h"

float Trapz(float x, FuzzyArgs args, uint8_t type)
{
    if(type == TRAPZ_LOW)
    {
        // 1 ----\
        //        \
        //         \
        // 0 --------\

        if(x <= args.L) return 1.0f;
        if(x < args.R) return (args.R - x)/(args.R - args.L);
        return 0.0f;
    }
    if(type == TRAPZ_HIGH)
    {
        //         /-----
        //        /
        //       /
        // -----/ 1

        if(x <= args.L) return 0.0f;
        if(x < args.R) return (x - args.L)/(args.R - args.L);
        return 1.0f;
    }
}


float Triang(float x, FuzzyArgs args) {
  if (x <= args.L) return 0.0f;
  if ((x < args.C) && (x > args.L)) return (x - args.L) / (args.C - args.L);
  if ((x < args.R) && (x > args.C)) return (args.R - x) / (args.R - args.C);
  if (x >= args.R) return 0.0f;
  return 0.0f;
}

float FuzzyMembership(float x, FuzzyArgs args, uint8_t type)
{
    if (type == TRAPZ_LOW || type == TRAPZ_HIGH)
    {
        return Trapz(x, args, type);
    }
    return Triang(x, args);
}

float MamdaniAnd(float a, float b)
{
    return (a < b) ? a : b;
}

float MamdaniOr(float a, float b)
{
    return (a > b) ? a : b;
}

float MamdaniRuleStrength(float a, float b)
{
    return MamdaniAnd(a, b);
}

//-------- Sensors --------


float White(float x) {
  FuzzyArgs WhiteArgs = {3500, 0, 3600};
  return FuzzyMembership(x, WhiteArgs, TRAPZ_LOW);
}
float Gray(float x) {
  FuzzyArgs GrayArgs = {3600, 3760, 3900};
  return FuzzyMembership(x, GrayArgs, TRIANG);
}

float Black(float x) {
  FuzzyArgs BlackArgs = {3900, 0, 4020};
  return FuzzyMembership(x, BlackArgs, TRAPZ_HIGH);
}

//-------- Motors --------
float Lowf(float x) {
  return FuzzyMembership(x, Low, TRAPZ_LOW);
}

float Mediumf(float x) {
  return FuzzyMembership(x, Mid, TRIANG);
}

float Highf(float x) {
    return FuzzyMembership(x, High, TRAPZ_HIGH);
}
