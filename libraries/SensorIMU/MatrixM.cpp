/*
  Matrix.cpp - Library for working with matrices.
  Created by Rafael Araujo Lehmkuhl, January 9, 2015.
  Released into the public domain.
*/

#include "Arduino.h"
#include "MatrixM.h"
#include "VectorM.h"

MatrixM Matrix;

void MatrixM::equal(double matrix1[3][3], double matrix2[3][3])
{
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3; j++)
    {
      matrix1[i][j] = matrix2[i][j];
    }
  }
}

void MatrixM::multiply(double multi_matrix[3][3], double matrix1[3][3], double matrix2[3][3])
{
  
  Matrix.zero(multi_matrix);
  
  for(int m=0;m<3;m++)
  {
    for(int n=0;n<3;n++)
    {
      for(int k=0;k<3;k++)
      {
        multi_matrix[m][n] += matrix1[m][k]*matrix2[k][n];
      }
    }
  }
}

void MatrixM::normalize(double matrix[3][3])
{
  double magnitude[3];
  Vector.zero(magnitude);
  
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      magnitude[i] += matrix[i][j] * matrix[i][j];
      if(j==2)
      {
        magnitude[i] = sqrt(magnitude[i]);
      }
    }
  }
  
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      matrix[i][j] = matrix[i][j] / magnitude[i];
    }
  }  
}

void MatrixM::orthogonalize(double matrix[3][3])
{
  double row1[3];
  double row2[3];
  double row1_ort[3];
  double row2_ort[3];
  double row3_ort[3]={1,2,3};
  double error;
  
  Matrix.RowToVector(matrix, row1, 0);
  Matrix.RowToVector(matrix, row2, 1);
  
  error = Vector.dotProduct(row1, row2);
  
  for(int k=0;k<3;k++)
  {
    row1_ort[k] = row1[k]-(error/2)*row2[k];
    row2_ort[k] = row2[k]-(error/2)*row1[k];
  }
  
  Vector.crossProduct(row1_ort, row2_ort, row3_ort);
  
  Vector.toMatrix(row1_ort, row2_ort, row3_ort, matrix);
}

void MatrixM::printMatrix(double matrix[3][3])
{
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      Serial.print(matrix[i][j]);
      Serial.print(" ");
      if (j==2)
      {
        Serial.println();
      }
    }
  }
  Serial.println();
}

void MatrixM::printData(double matrix[3][3])
{
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      Serial.print(matrix[i][j]);
      
      if ((j!=2) || (i!=2))
      {
        Serial.print(";");
      }
      
      if ((j==2) && (i!=2))
      {
        Serial.print("/");
      }
    }
  }
  Serial.println();
}

void MatrixM::scale(double matrix[3][3], double scale)
{
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3; j++)
    {
      matrix[i][j] = scale * matrix[i][j];
    }
  }
}

void MatrixM::sum(double matrix1[3][3], double matrix2[3][3], double matrix_result[3][3])
{
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3; j++)
    {
      matrix_result[i][j] = matrix1[i][j] + matrix2[i][j];
    }
  }
}

void MatrixM::RowToVector(double matrix[3][3], double vector[3], int row)
{
  for(int k=0;k<3;k++)
  {
    vector[k] = matrix[row][k];
  }
}

void MatrixM::ColumnToVector(double matrix[3][3], double vector[3], int column)
{
  for(int k=0;k<3;k++)
  {
    vector[k] = matrix[k][column];
  }
}

void MatrixM::zero(double matrix[3][3])
{
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      matrix[i][j] =0;
    }
  }
}

void MatrixM::multiplyVector(double out_vector[3], double matrix[3][3], double vector[3])
{
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<3;j++)
    {
      out_vector[i] = out_vector[i] + (matrix[i][j] * vector[i]);
    }
  }
}
