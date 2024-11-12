#ifndef __IGVFUENTELUZ
#define __IGVFUENTELUZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvPunto3D.h"
#include "igvColor.h"

class igvFuenteLuz {
protected:
	unsigned int idLuz; // GL_LIGHT0 a GL_LIGHT7

	igvPunto3D posicion; // posición de la luz

	igvColor colorAmbiente;		// color ambiental de la luz
	igvColor colorDifuso;		// color difuso de la luz
	igvColor colorEspecular;	// color especular de la luz

	double aten_a0; // coeficiente de atenuación a0
	double aten_a1; // coeficiente de atenuación a1
	double aten_a2; // coeficiente de atenuación a2

	// parámetros para definir un FOCO
	igvPunto3D direccion_foco;
	double angulo_foco;
	double exponente_foco;

	bool encendida; // Indica si la luz está encendida o no

public:
	igvFuenteLuz();

	// Luz puntual
	igvFuenteLuz(const unsigned int _idLuz,
		const igvPunto3D& _posicion,
		const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
		const double a0, const double a1, const double a2);

	// FOCO
	igvFuenteLuz(const unsigned int _idLuz,
		const igvPunto3D& _posicion,
		const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp,
		const double a0, const double a1, const double a2,
		const igvPunto3D& dir_foco, const double ang_foco, const double exp_foco);

	// Posicion
	igvPunto3D& getPosicion(void);
	void setPosicion(igvPunto3D pos);

	// Colores de luz Ambiental, Difusa y Especular
	void set(const igvColor& cAmb, const igvColor& cDif, const igvColor& cEsp);
	void setAmbiental(const igvColor& cAmb);
	void setDifuso(const igvColor& cDif);
	void setEspecular(const igvColor& cEsp);
	igvColor& getAmbiental(void);
	igvColor& getDifuso(void);
	igvColor& getEspecular(void);

	// Coeficientes de Atenuacion
	void setAtenuacion(double a0, double a1, double a2);
	void getAtenuacion(double& a0, double& a1, double& a2);

	void encender(void);
	void apagar(void);
	unsigned int esta_encendida(void);

	void aplicar(void);
};

#endif
