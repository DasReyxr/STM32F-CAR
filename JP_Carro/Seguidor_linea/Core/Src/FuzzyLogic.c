#include "FuzzyLogic.h"
#include "stm32f4xx_hal.h"

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
  FuzzyArgs LowArgs = {0, 0, 50};
  return Trapz(x, LowArgs, TRAPZ_LOW);
}

float Mediumf(float x) {
  FuzzyArgs Mid = {25, 50, 75};
  return Triang(x, Mid);
}

float Highf(float x) {
  FuzzyArgs HighArgs = {50, 0, 100};
  return Trapz(x, HighArgs, TRAPZ_HIGH);
}



void areas(void){
  /*
  Slopes
Functions of pertinence
u_Low  = (L.R-x)/(L.R-L.L)
u_Mid1 = (x-M.L)/(M.C-M.L)
u_Mid2 = (MR-x)/(MR-Mmid)
u_High = (x-HL)/(HR-HL)  
*/
  FuzzyArgs Low = {0, 0, 50};
  FuzzyArgs Mid = {25, 50, 75};
  FuzzyArgs High = {50, 0, 100};
  float x_Low ; // Example input for Low
  float x_mid ; // Example input for Mid
  float x_High ; // Example input for High
  
  float y_Low;// Lowf(x_Low);
  float y_mid;//Mediumf(x_mid);
  float y_High;//Highf(x_High);
  // Low x_l y_l
  
      // Low and Mid
    float m_Low = -1/(Low.R-Low.L); 
    float x_low = (Low.R - y_low)/(m_low);

  float C_Low = y_Low*x_Low;
  float T_Low = y_Low*(Low.R-x_Low)/2;
  float A_Low = T_Low+C_Low;

  // Medio x_m y_m
  float m_Mid0 = 1/(Mid.C-Mid.L);
  float m_Mid1 =-1/(Mid.R-Mid.C);

  float x_m0 = (y_mid - Mid.L) /m_Mid0;
  float x_m1 = (y_mid - Mid.R) /m_Mid1;

  float TS = (x_m1-x_m0)*(1-y_mid)/2.0f;
  float TT = (Mid.R-Mid.L)/2.0f;
  float A_M = TT-TS;

  float x_Low0 = (y_mid-Mid.L)*(Mid.R-Mid.L);


  // Mid and High

  float Y1 = (Low.R * Mid.C - Low.L * Mid.L)/(Mid.C - Mid.L+ Low.R- Low.L);
  float y_lm = fmin(y_Low,y_Mid);
  
  float x_lm0 = (y_lm - Mid.L)/m_Mid0; // Derived from m_Mid1 
  float x_lm1 = (Low.R - y_lm)/m_low; // Derived from m_Mid1 
  
  if(y_lm<Y1) 
    float T2 = (x_Low-x_Low0)*(Y1-y_lm)/2.0f;
  else T2 = 0

  float TLeft = Y1*(Low.R-Mid.L)/2.0f;
  float A_LowMid = TLeft-T2;

  float y_mh0 = (y_High-High.L)*(High.R-High.L);
  float Y2 = (High.R * Mid.R - High.L * Mid.C)/ (Mid.R-Mid.C+High.R-High.L);// (MR-x)/(MR-Mmid)  = (x-HL)/(HR-HL)  
  float y_mh = fmin(y_Low,y_Mid);
  if(y_mh<Y1) 
    float T3 = (x_MidHigh - X_High)*(Y2-y_mh)/2.0f;
  else T3 = 0;

  float TRight = (Mid.R-High.L * Y2)/2.0f;
  float A_MidHigh = TRight-T3;
    // High x_h y_h
  float C_High = (High.R-x_High) * y_High;
  float T_High = (x_High-High.L) * y_High / 2.0f;
  float A_High = T_High + C_High;

  float A_Total = C_Low + T_Low+ TT - TS + C_High+ T2 +T3 - TRight - TLeft;
  }