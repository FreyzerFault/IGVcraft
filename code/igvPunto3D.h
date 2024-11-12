/************************************************************************************************/
/* La clase igvPunto3D implementa la funcionalidad de los objetos tanto Punto como Vector en 3D */
/************************************************************************************************/

#ifndef __IGVPUNTO3D
#define __IGVPUNTO3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#define IGV_EPSILON 0.000001 // para comparaciones con 0

#ifndef __ENUM_XYZ
#define __ENUM_XYZ

enum Coord {
	X,
	Y,
	Z
};
#endif

#include <sstream>
#include <iostream>
using namespace std;

class igvPunto3D {
public:
	double c[3]; // Componentes x, y, z del punto o vector

public:
	// Constructores
	igvPunto3D();
	igvPunto3D(const double& x, const double& y, const double& z);

	// Constructor de copia
	igvPunto3D(const igvPunto3D& p);

	// Operador de asignación
	igvPunto3D& operator = (const igvPunto3D& p);

	// Destructor
	~igvPunto3D();

	// Operadores
	inline double& operator[] (const unsigned char idx) { return c[idx]; };
	inline double operator[] (const unsigned char idx) const { return c[idx]; };

	int operator == (const igvPunto3D& p);
	int operator != (const igvPunto3D& p);

	void set(const double& x, const double& y, const double& z);
	float* cloneToFloatArray() const; //Devuelve el punto en un array de float

	string toString() { return "[" + to_string(c[X]) + "," + to_string(c[Y]) + "," + to_string(c[Z]) + "]"; };

	// Devuelve un punto que representa el Chunk al que pertenece (floor(X / x), 0, floor(Z / x))
	igvPunto3D operator / (double x);

	// Movimiento del punto en una direccion con un modulo = distancia (por defecto 1)
	void avanzar(double yaw, double pitch, double distancia = 1);
};
#endif
