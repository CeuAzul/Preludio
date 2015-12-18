/*
  MatrixM.h - Library for working with matrices.
  Created by Rafael Araujo Lehmkuhl, January 9, 2015.
  Released into the public domain.
*/

#ifndef MatrixM_h
#define MatrixM_h

#include "Arduino.h"

class MatrixM
{
  public:
    void equal(double matrix1[3][3], double matrix2[3][3]);
    void multiply(double multi_matrix[3][3], double matrix1[3][3], double matrix2[3][3]);
	void normalize(double matrix[3][3]);
	void orthogonalize(double matrix[3][3]);
	void printMatrix(double matrix[3][3]);
	void printData(double matrix[3][3]);
	void scale(double matrix[3][3], double scale);
	void sum(double matrix1[3][3], double matrix2[3][3], double matrix_result[3][3]);
	void RowToVector(double matrix[3][3], double vector[3], int row);
	void ColumnToVector(double matrix[3][3], double vector[3], int row);
	void multiplyVector(double out_vector[3], double matrix[3][3], double vector[3]);
	void zero(double matrix[3][3]);
};

extern MatrixM Matrix;

#endif
