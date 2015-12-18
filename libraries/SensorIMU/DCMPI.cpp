/*
  DCMPI.h - Library for PI controlling DCM matrices.
  Created by Rafael Araujo Lehmkuhl, January 25, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "DCMPI.h"
#include "MatrixM.h"
#include "VectorM.h"

DCMPI DCMinstance;

void DCMPI::PI_controller(double DCM[3][3], double YcoG, double acc_vector[3], double dt, double W_Ico[3], double W_co[3], double W_RP, double W_Y, double K_P, double K_I)
{
  double Tco[3];
  double Yco[3];
  double RPco[3];
  
  //Compute correction Yaw
  double DCMx[3];
  Matrix.RowToVector(DCM, DCMx, 2);
  
  Vector.equal(Yco, DCMx);
  Vector.scale(Yco, YcoG);   // Aplica a correção em YAW do plano da Terra no plano da aeronave

  //Compute correction RollPitch
  double DCMz[3];
  Matrix.RowToVector(DCM, DCMz, 2);
  
  Vector.crossProduct(DCMz, acc_vector, RPco);  // Calcula o produto vetorial dos vetores de Roll e Pitch do acelerômetro com os da matriz DCM e armazena no vetor RPco
  
  Vector.scale(Yco, W_Y);           // Pondera o erro calculado anteriormente através do peso W_Y
  Vector.scale(RPco, W_RP);         // Pondera o erro calculado anteriormente através do peso W_RP

  Vector.sum(RPco, Yco, Tco);       // Soma os erros calculados anteriormente 
  
  double W_Pco[3];
  //double W_Ico[3];
  //double W_co[3];
  
  double Tco1[3];
  double Tco2[3];
  
  Vector.equal(Tco1, Tco);          // Copia o valor da correção total para uma variável temporária 
  Vector.equal(Tco2, Tco);          // Copia o valor da correção total para outra variável temporária 
  
  Vector.scale(Tco1, K_P);          // Pondera a correção total com o Ponderador Proporcional
  Vector.equal(W_Pco, Tco1);
  
  Vector.scale(Tco2, K_I);          // Pondera a correção total com o Ponderador Integral  
  Vector.scale(Tco2, dt);           // Pondera a correção total com o tempo entre loops dt 
  Vector.sum(W_Ico, Tco2, W_Ico);   // Soma a correção integral anterior com a nova correção integral
  
  Vector.sum(W_Pco, W_Ico, W_co);   //Soma as correções Proporcional (W_Pco) e Integral (W_Ico) numa correção Total (W_co)
}