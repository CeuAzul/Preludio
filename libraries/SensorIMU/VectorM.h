/*
  VectorM.h - Library for working with vectors.
  Created by Rafael Araujo Lehmkuhl, January 24, 2015.
  Released into the public domain.
*/

#ifndef VectorM_h
#define VectorM_h

#include "Arduino.h"

class VectorM
{
  public:
    void crossProduct(double vector1[3], double vector2[3], double cross_product[3]);
    double dotProduct(double vector1[3], double vector2[3]);
	void equal(double vector1[3], double vector2[3]);
	void normalize(double vector[3]);
	void scale(double vector[3], double scale);
	void sum(double vector1[3], double vector2[3], double vector_result[3]);
	void toMatrix(double vector1[3], double vector2[3], double vector3[3], double matrix[3][3]);
	void zero(double vector[3]);
	void printData(double vector[3]);
};

extern VectorM Vector;

#endif
