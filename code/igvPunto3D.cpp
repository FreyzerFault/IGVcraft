#include <stdio.h>
#include <math.h>

#include "igvPunto3D.h"

// Constructores
igvPunto3D::igvPunto3D() {
	c[X] = c[Y] = c[Z] = 0.0;
}

igvPunto3D::igvPunto3D(const double& x, const double& y, const double& z) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
}

// Constructor de copia
igvPunto3D::igvPunto3D(const igvPunto3D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
}

// Operador de asignación
igvPunto3D& igvPunto3D::operator = (const igvPunto3D& p) {
	c[X] = p.c[X];
	c[Y] = p.c[Y];
	c[Z] = p.c[Z];
	return(*this);
}

int igvPunto3D::operator == (const igvPunto3D& p) {
	return ((fabs(c[X] - p[X]) < IGV_EPSILON) && (fabs(c[Y] - p[Y]) < IGV_EPSILON) && (fabs(c[Z] - p[Z]) < IGV_EPSILON));
}

int igvPunto3D::operator != (const igvPunto3D& p) {
	return ((fabs(c[X] - p[X]) >= IGV_EPSILON) || (fabs(c[Y] - p[Y]) >= IGV_EPSILON) || (fabs(c[Z] - p[Z]) >= IGV_EPSILON));
}

// Destructor
igvPunto3D::~igvPunto3D() {
}

// Operadores

void igvPunto3D::set(const double& x, const double& y, const double& z) {
	c[X] = x;
	c[Y] = y;
	c[Z] = z;
}

float* igvPunto3D::cloneToFloatArray() const
{
	float* res = new float[4]{ (float)c[0], (float)c[1], (float)c[2], 1.0 };
	return res;
}

// Sirve para reducir una coordenada a la escala de los chunks (altura 0)
igvPunto3D igvPunto3D::operator / (double x) {
	igvPunto3D p(floor(c[X] / x), 0, floor(c[Z] / x));
	return p;
}

// Avanza tanta distancia en el espacio tridimensional segun una orientacion
// Por defecto la distancia sera 1
void igvPunto3D::avanzar(double yaw, double pitch, double distancia) {
	c[X] = c[X] + cos(yaw) * cos(pitch) * distancia;
	c[Y] = c[Y] + sin(pitch) * distancia;
	c[Z] = c[Z] + sin(yaw) * cos(pitch) * distancia;
}