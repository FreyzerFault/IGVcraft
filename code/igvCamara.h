#ifndef __IGVCAMARA
#define __IGVCAMARA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvEscena3D.h"

typedef enum {
	PANORAMICA,
	CENITAL,
	LIBRE,
	JUGADOR
}tipoVista;

typedef enum {
	IGV_PARALELA,
	IGV_FRUSTUM,
	IGV_PERSPECTIVA
} tipoCamara;

class igvCamara {
public:
	// atributos
	tipoVista vista;		// Posición de la cámara

	tipoCamara tipo;	// paralela o perspectiva

	// ventana de visión: parametros proyección paralela y frustum
	GLdouble xwmin, xwmax, ywmin, ywmax;

	// ventana de visión: parametros proyección perspectiva
	GLdouble 	angulo, raspecto;

	// distancias de planos cercano y lejano
	GLdouble znear, zfar;

	// punto de visión
	igvPunto3D P0;

	// punto de referencia de visión
	igvPunto3D r;

	// vector arriba
	igvPunto3D V;

	// Orientación de la cámara (respecto el eje X (yaw) y Y (pitch))
	GLdouble yaw, pitch;

	// Metodos

public:
	// Constructores por defecto y destructor
	igvCamara();
	~igvCamara();

	// Otros constructores
	igvCamara(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V);

	// Metodos
	// define la posición de la cámara
	void set(igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V);

	// define una cámara de tipo paralela o frustum
	void set(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
		double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar);

	// define una cámara de tipo perspectiva
	void set(tipoCamara _tipo, tipoVista _vista, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
		double _angulo, double _raspecto, double _znear, double _zfar);

	// define parámetros de cada tipo de cámara
	void set_perspectiva(double _angulo, double _raspecto, double _znear, double _zfar);
	void set_paralela(double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar);

	void set(igvCamara& cam);

	void aplicar();

	// MOVIMIENTOS
	void zoom(double factor); // realiza un zoom sobre la cámara
	void rotar(double moveX, double moveY);
	void mover(double moveX, double moveY, double moveZ);

	// GETTERs / SETTERs
	void set_proyeccion(tipoCamara t) { tipo = t; };
	tipoCamara get_proyeccion() { return tipo; };
	void set_vista(tipoVista x) { vista = x; };
	tipoVista get_vista() { return vista; };
	string print_vista();

	void set_pos(igvPunto3D pos) { P0 = pos; };
	igvPunto3D get_pos() { return P0; };
	void set_r(igvPunto3D ref) { r = ref; };
	igvPunto3D get_r() { return r; };
	void set_v(igvPunto3D v) { V = v; };
	igvPunto3D get_v() { return V; };

	void set_yaw(GLdouble YAW) { yaw = YAW; };
	GLdouble get_yaw() { return yaw; };
	void set_pitch(GLdouble PITCH) { pitch = PITCH; };
	GLdouble get_pitch() { return pitch; };
};

#endif
