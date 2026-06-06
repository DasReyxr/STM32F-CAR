#include "FuzzyLogic.h"


float Trapz(float x, FuzzyArgs args, uint8_t type)
{
    if(type == TRAPZ_LOW)
    {
        // 1 ----\
        //        \
        //         \
        // 0 --------\

        if(x <= args.Inf) return 1.0f;
        if(x < args.Sup) return (args.Sup - x)/(args.Sup - args.Inf);
        return 0.0f;
    }
    if(type == TRAPZ_HIGH)
    {
        //         /-----
        //        /
        //       /
        // -----/ 1

        if(x <= args.Inf) return 0.0f;
        if(x < args.Sup) return (x - args.Inf)/(args.Sup - args.Inf);
        return 1.0f;
    }
}


float Triang(float x, FuzzyArgs args) {
  if (x <= args.Inf) return 0;
  if ((x < args.Mid) && (x > args.Inf)) return (x-args.Inf)/(args.Mid-args.Inf);
  if ((x < args.Sup) && (x > args.Mid)) return (args.Sup-x)/(args.Sup-args.Mid);
  if (x >= args.Sup) return 0;
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
float Low(float x) {
  FuzzyArgs LowArgs = {0, 0, 50};
  return Trapz(x, LowArgs, TRAPZ_LOW);
}

float Medium(float x) {
  FuzzyArgs MediumArgs = {25, 50, 75};
  return Triang(x, MediumArgs);
}

float High(float x) {
  FuzzyArgs HighArgs = {50, 0, 100};
  return Trapz(x, HighArgs, TRAPZ_HIGH);
}
