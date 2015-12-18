/*
  VectorM.h - Library for working with vectors.
  Created by Rafael Araujo Lehmkuhl, January 24, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "VectorM.h"

VectorM Vector;

void VectorM::crossProduct(double vector1[3], double vector2[3], double cross_product[3])
{
  cross_product[0] = vector1[1]*vector2[2] - vector1[2]*vector2[1];
  cross_product[1] = vector1[2]*vector2[0] - vector1[0]*vector2[2];
  cross_product[2] = vector1[0]*vector2[1] - vector1[1]*vector2[0];
}

double VectorM::dotProduct(double vector1[3], double vector2[3])
{
  double dot_Product=0;

 
  for(int k=0;k<3;k++)
  {
    dot_Product += vector1[k] * vector2[k];
  }
  return dot_Product;
}

void VectorM::equal(double vector1[3], double vector2[3])
{
  for(int i=0; i<3; i++)
  {
    vector1[i] = vector2[i];
  }
}

void VectorM::normalize(double vector[3])
{
  double magnitude = 0;

  for(int i=0;i<3;i++)
  {
    magnitude += vector[i] * vector[i];
  }
  
  magnitude = sqrt(magnitude);
  
  for(int i=0;i<3;i++)
  {
    vector[i] = vector[i] / magnitude;
  }  
}

void VectorM::scale(double vector[3], double scale)
{
  for(int i=0; i<3; i++)
  {
    vector[i] = scale * vector[i];
  }
}

void VectorM::sum(double vector1[3], double vector2[3], double vector_result[3])
{
  for(int i=0; i<3; i++)
  {
    vector_result[i] = vector1[i] + vector2[i];
  }
}

void VectorM::toMatrix(double vector1[3], double vector2[3], double vector3[3], double matrix[3][3])
{
  for(int k=0;k<3;k++)
  {
    matrix[0][k]=vector1[k];
    matrix[1][k]=vector2[k];
    matrix[2][k]=vector3[k];
  }
}

void VectorM::zero(double vector[3])
{
  for(int i=0;i<3;i++)
  {
    vector[i] = 0;
  }
}

void VectorM::printData(double vector[3])
{
  for(int i=0;i<3;i++)
  {
    Serial.print(vector[i]);
    if (i!=2)
    {
      Serial.print(";");
    }  
  }
  Serial.println();
}


