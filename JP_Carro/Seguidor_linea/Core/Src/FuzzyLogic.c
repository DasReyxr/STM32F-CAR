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


// Centroide 

float f_low(float y)
{
    return (Low.R - y)/m_low;
}

float f_Mid1(float y)
{
    return (y - Mid.L)/m_Mid0;
}

float f_Mid2(float y)
{
    return (Mid.R - y)/m_Mid1;
}

float f_High(float y)
{
    return (y-High.L)/m_High;
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
//  float x_Low ; // Example input for Low
//  float x_mid ; // Example input for Mid
//  float x_High ; // Example input for High
  
  float y_low;// Lowf(x_Low);
  float y_mid;//Mediumf(x_mid);
  float y_high;//Highf(x_High);
  // Low x_l y_l
  
  // Low and Mid
  // Area Low
  float m_LOW = -1/(Low.R-Low.L); 
  float x_low = f_low(y_low);

  float C_low = y_low*x_low;
  float T_low = y_low*(Low.R-x_Low)/2;
  float A_low = T_low+C_low;


  // Medio x_m y_m
  //Area Mid (Triangule)
  float m_Mid0 = 1/(Mid.C-Mid.L);
  float m_Mid1 =-1/(Mid.R-Mid.C);

  float x_mid0 = f_Mid1(y_mid);
  float x_mid1 = f_Mid2(y_mid);

  float TS = (x_mid1-x_mid0)*(1-y_mid)/2.0f;
  float TT = (Mid.R-Mid.L)/2.0f;
  float A_M = TT-TS;

  float x_Low0 = (y_mid-Mid.L)*(Mid.R-Mid.L);


  // Mid and low
  // Area Intersetnction 1
  float Y1 = (Low.R * Mid.C - Low.L * Mid.L)/(Mid.C - Mid.L+ Low.R- Low.L); // Constante
  float y_lm = fmin(y_Low,y_Mid);
  
  float x_lm0 = f_Mid1(y_lm);  
  float x_lm1 = f_low(y_lm);
  float X_1 = f_Mid1(Y1); // Constante 
  
  if(y_lm<Y1) 
    float T1 = (x_lm1-x_lm0)*(Y1-y_lm)/2.0f;
  else T1 = 0

  float TLeft = Y1*(Low.R-Mid.L)/2.0f;
  float A_LowMid = TLeft-T2;
  
  // mid and high
  // Area Intersection 2
  float y_mh0 = (y_High-High.L)*(High.R-High.L);
  float Y2 = (High.R * Mid.R - High.L * Mid.C)/ (Mid.R-Mid.C+High.R-High.L);// (MR-x)/(MR-Mmid)  = (x-HL)/(HR-HL)  
  float y_mh = fmin(y_Low,y_Mid);
  
  float x_mh0 =  f_High(y_mh);  
  float x_mh1 = f_Mid2(y_mh); 
  float X_2 = f_High(Y2);
  
  if(y_mh<Y1) 
    float T2 = (x_mh1 - x_mh0)*(Y2-y_mh)/2.0f;
  else T2 = 0;

  float TRight = (Mid.R-High.L * Y2)/2.0f;
  float A_MidHigh = TRight-T2;
  
  // Area High x_h y_h
  float m_High = 1/(H.R-H.L);
  float x_high= f_High(y_high);
  float C_high = (High.R-x_high) * y_high;
  float T_high = (x_high-High.L) * y_high / 2.0f;
  float A_high = T_high + C_high;

  float A_Total = C_low + T_low+ TT - TS + C_high + T_high + T1 +T2 - TRight - TLeft;
  }