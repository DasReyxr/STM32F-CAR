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
  if (x <= args.L) return 0;
  if ((x < args.C) && (x > args.L)) return (x-args.L)/(args.C-args.L);
  if ((x < args.R) && (x > args.C)) return (args.R-x)/(args.R-args.C);
  if (x >= args.R) return 0;
}
//-------- Sensors --------
float Black(float x) {
  FuzzyArgs BlackArgs = {350, 0, 500};
  return Trapz(x, BlackArgs, TRAPZ_LOW);
}

float Gray(float x) {
  FuzzyArgs GrayArgs = {400, 500, 600};
  return Triang(x, GrayArgs);
}

float White(float x) {
  FuzzyArgs WhiteArgs = {550, 0, 600};
  return Trapz(x, WhiteArgs, TRAPZ_HIGH);
}

//-------- Motors --------
float Lowf(float x) {
  return Trapz(x, Low, TRAPZ_LOW);
}

float Mediumf(float x) {
  return Triang(x, Mid);
}

float Highf(float x) {
    return Trapz(x, High, TRAPZ_HIGH);
}
