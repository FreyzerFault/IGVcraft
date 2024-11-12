#include <math.h>
#include <stdio.h>

#include "igvCamara.h"
#include <iostream>

// Metodos constructores

igvCamara::igvCamara() {}

igvCamara::~igvCamara() {}

igvCamara::igvCamara(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	P0 = _P0;
	r = _r;
	V = _V;

	tipo = _tipo;
	vista = _vista;
	// Angulo hacia abajo
	if (vista == CENITAL) {
		yaw = 0;
		pitch = -PI / 2;
	}
	// Angulo segun P0 y r
	if (vista == LIBRE || vista == PANORAMICA || vista == JUGADOR) {
		pitch = asin(r[Y] - P0[Y]);
		yaw = acos((r[X] - P0[X]) / cos(pitch));
	}
}

void igvCamara::set(igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	P0 = _P0;
	r = _r;
	V = _V;
}
void igvCamara::set(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
	double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar) {
	tipo = _tipo;
	vista = _vista;

	P0 = _P0;
	r = _r;
	V = _V;

	xwmin = _xwmin;
	xwmax = _xwmax;
	ywmin = _ywmin;
	ywmax = _ywmax;
	znear = _znear;
	zfar = _zfar;
}

void igvCamara::set(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
	double _angulo, double _raspecto, double _znear, double _zfar) {
	tipo = _tipo;
	vista = _vista;

	P0 = _P0;
	r = _r;
	V = _V;

	angulo = _angulo;
	raspecto = _raspecto;
	znear = _znear;
	zfar = _zfar;
}

void igvCamara::set_perspectiva(double _angulo, double _raspecto, double _znear, double _zfar) {
	angulo = _angulo;
	raspecto = _raspecto;
	znear = _znear;
	zfar = _zfar;
}

void igvCamara::set_paralela(double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar) {
	xwmin = _xwmin;
	xwmax = _xwmax;
	ywmin = _ywmin;
	ywmax = _ywmax;
	znear = _znear;
	zfar = _zfar;
}

void igvCamara::set(igvCamara& cam) {
	P0 = cam.get_pos();
	r = cam.get_r();
	V = cam.get_v();

	yaw = acos((r[X] - P0[X]) / cos(pitch));
	pitch = asin(r[Y] - P0[Y]);
}

void igvCamara::aplicar(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (tipo == IGV_PARALELA) {
		glOrtho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}
	if (tipo == IGV_FRUSTUM) {
		glFrustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}
	if (tipo == IGV_PERSPECTIVA) {
		gluPerspective(angulo, raspecto, znear, zfar);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z]);
}

void igvCamara::zoom(double factor) {
	angulo = angulo - angulo * factor;
	xwmin = xwmin + (factor * (xwmax - xwmin)) / 2;
	xwmax = xwmax - (factor * (xwmax - xwmin)) / 2;
	ywmin = ywmin + (factor * (ywmax - ywmin)) / 2;
	ywmax = ywmax - (factor * (ywmax - ywmin)) / 2;
}

// Rotacion de la camara segun pitch y yaw
void igvCamara::rotar(double moveX, double moveY) {
	pitch += moveY;
	yaw += moveX;

	// Limitamos el angulo de vision vertical para ser mas realista
	if (pitch > PI / 2 - 0.001)
		pitch = PI / 2 - 0.001;
	if (pitch < -PI / 2 + 0.001)
		pitch = -PI / 2 + 0.001;

	r[X] = P0[X] + cos(pitch) * cos(yaw);
	r[Y] = P0[Y] + sin(pitch);
	r[Z] = P0[Z] + cos(pitch) * sin(yaw);
	V[Y] = P0[Y] + sin(pitch + PI / 2); // + 90º

	aplicar();
}

// Movimiento Lateral y Vertical de la cámara (mueve a la par P0 y r)
void igvCamara::mover(double moveX, double moveY, double moveZ) {
	P0[X] += moveX;
	P0[Y] += moveY;
	P0[Z] += moveZ;
	r[X] += moveX;
	r[Y] += moveY;
	r[Z] += moveZ;

	aplicar();
}

string igvCamara::print_vista() {
	switch (vista) {
	case PANORAMICA:
		return "PANORAMICA";
	case LIBRE:
		return "LIBRE";
	case JUGADOR:
		return "JUGADOR";
	case CENITAL:
		return "CENITAL";
	}
	return "[not stated]";
}