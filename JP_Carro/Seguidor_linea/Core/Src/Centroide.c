#include "Centroide.h"
#include <math.h>

// GLobal variables 
static float Y1;
static float X1;
static float Y2;
static float X2;

static float TLeft;
static float TT;
static float TRight;

static float C_TLeft;
static float C_TT;
static float C_TRight;


void Centroide_Init(void)
{
    Y1 = (Low.R * Mid.C - Low.L * Mid.L) /
         (Mid.C - Mid.L + Low.R - Low.L);

    X1 = f_Mid1(Y1);

    Y2 = (High.R * Mid.R - High.L * Mid.C) /
         (Mid.R - Mid.C + High.R - High.L);

    X2 = f_High(Y2);

    TLeft  = Y1 * (Low.R - Mid.L) / 2.0f;
    TT     = (Mid.R - Mid.L) / 2.0f;
    TRight = (Mid.R - High.L) * Y2 / 2.0f;

    C_TLeft  = Centroid_T(Low.R, Mid.L, X1);
    C_TT     = Centroid_T(Mid.L, Mid.R, Mid.C);
    C_TRight = Centroid_T(Mid.R, High.L, X2);
}

// Functions
float Centroide(float y_low, float y_mid, float y_high){
  DefuzzArgs Area_L = Area_Low(y_low);
  DefuzzArgs Area_M = Area_Mid(y_mid);
  DefuzzArgs Area_H = Area_High(y_high);
  DefuzzArgs Area_LM = Area_InterLm(y_low, y_mid);
  DefuzzArgs Area_MH = Area_InterMh(y_mid, y_high);
  float num = Area_L.num + Area_M.num + Area_H.num + Area_LM.num + Area_MH.num;
  float den = Area_L.den + Area_M.den + Area_H.den + Area_LM.den + Area_MH.den;
  if(den == 0) return 0.0f; // Evitar división por cero
  return num / den;
}

DefuzzArgs Area_Low(float y_low){
  DefuzzArgs Area_n_Centroid = {0.0f,0.0f};
  float x_low = f_Low(y_low);

  float A_Sq_low = y_low*x_low;
  float A_T_low = y_low*(Low.R-x_low)/2;
  
  // Centroides 
  
  float C_Sq_low = Centroid_Sq(0, x_low);
  float C_T_low = Centroid_T_R(x_low, Low.R);
  Area_n_Centroid.num = A_Sq_low*C_Sq_low + A_T_low*C_T_low;
  Area_n_Centroid.den  =  A_Sq_low + A_T_low;
  return Area_n_Centroid;  
}

DefuzzArgs Area_Mid(float y_mid){
  DefuzzArgs Area_n_Centroid = {0.0f,0.0f};
  float x_mid0 = f_Mid1(y_mid);
  float x_mid1 = f_Mid2(y_mid);
  // Calcular areas 
  float A_TS = (x_mid1-x_mid0)*(1-y_mid)/2.0f;
  float A_TT = (Mid.R-Mid.L)/2.0f;
  // Centroides
  float C_TS = Centroid_T(x_mid0, x_mid1, Mid.C);
  float C_TT = Centroid_T(Mid.L, Mid.R, Mid.C);

  Area_n_Centroid.num  = A_TT*C_TT - A_TS*C_TS;
  Area_n_Centroid.den  = A_TT - A_TS;
  return Area_n_Centroid;
}

DefuzzArgs Area_High(float y_high){
  DefuzzArgs Area_n_Centroid = {0.0f,0.0f};

  float x_high= f_High(y_high);
  // Area
  float A_Sq_high = (limit_High-x_high) * y_high;
  float A_T_high = (x_high-High.L) * y_high / 2.0f;
  
  //Centroides 
  float C_Sq_high = Centroid_Sq(x_high, limit_High);
  float C_T_high = Centroid_T_R(High.L, x_high);

  Area_n_Centroid.num  = A_Sq_high*C_Sq_high + A_T_high*C_T_high;
  Area_n_Centroid.den  = A_Sq_high + A_T_high;
  return Area_n_Centroid;

}

DefuzzArgs Area_InterLm(float y_low, float y_mid){
  DefuzzArgs Area_n_Centroid = {0.0f,0.0f};
  float y_lm = fmin(y_low,y_mid);
  
  float x_lm0 = f_Mid1(y_lm);  
  float x_lm1 = f_Low(y_lm);
  float X_1 = f_Mid1(Y1); // Constante 
  float A_T1;
  if(y_lm<Y1) 
     A_T1 = (x_lm1-x_lm0)*(Y1-y_lm)/2.0f;
  else A_T1 = 0;

  float A_LowMid = TLeft-A_T1;
  
  // Centroides 
  float C_T1 = Centroid_T(x_lm0, x_lm1, X1);
  float C_TLeft = Centroid_T(Low.R, Mid.L, X1);
  
  Area_n_Centroid.num  = TLeft *C_TLeft - A_T1*C_T1;
  Area_n_Centroid.den  = TLeft - A_T1;
  return Area_n_Centroid;
}

DefuzzArgs Area_InterMh(float y_mid, float y_high){
  DefuzzArgs Area_n_Centroid = {0.0f,0.0f};
  float y_mh = fmin(y_mid,y_high);

  float x_mh0 =  f_High(y_mh);  
  float x_mh1 = f_Mid2(y_mh); 
  float X_2 = f_High(Y2);
  float T2;
  if(y_mh<Y1) 
     T2 = (x_mh1 - x_mh0)*(Y2-y_mh)/2.0f;
  else T2 = 0;

  float TRight = (Mid.R-High.L * Y2)/2.0f;
  float A_MidHigh = TRight-T2;
  
  // Centroides 
  float C_T2 = Centroid_T(x_mh0, x_mh1, X2);
  float C_TRight = Centroid_T(Mid.R, High.L, X2);
  
  Area_n_Centroid.num  = TRight *C_TRight - T2*C_T2;
  Area_n_Centroid.den  = TRight - T2;
  return Area_n_Centroid;
}

