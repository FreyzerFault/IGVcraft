/************************************************************************************************/
/* BLOQUE contiene las propiedades, posicion y el modelado										*/
/************************************************************************************************/

#ifndef __BLOQUE
#define __BLOQUE

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "Constantes.h"
#include <iostream>
#include "igvTextura.h"

using namespace std;

enum tipoBloque {
	Dirt,
	Stone,
	Cobblestone,
	Leaves,
	Wood,
	Bedrock,
	Glowstone,
	Grass,
	GrassSide,	// Bloques con diferentes texturas en cada cara necesitan texturas extra
	Log,
	LogSide,
};

class Bloque {
private:
	// Coordenadas globales del bloque
	igvPunto3D coords;
	tipoBloque tipo;

	// Vertices del bloque completo
	double* vertices;
	// Vertices de cada cara
	double* verticesUp;
	double* verticesDown;
	double* verticesFront;
	double* verticesBack;
	double* verticesLeft;
	double* verticesRight;

	// Normales del bloque completo
	int* normales;
	// Normales de cada cara
	int* normalesUp;
	int* normalesDown;
	int* normalesFront;
	int* normalesBack;
	int* normalesLeft;
	int* normalesRight;

	// Apunta a una malla guardada en Constantes que contiene la malla triangulos y texCoords
	Malla* mallaTriangulos;

	// Si se selecciona se coloreara cada cara con colores distintos
	bool seleccionado;

public:
	// Constructores
	Bloque();
	Bloque(igvPunto3D coord);
	Bloque(igvPunto3D coord, tipoBloque t);

	// Constructor de copia
	Bloque(const Bloque& c);
	Bloque(const Bloque& c, int vecinoX, int vecinoY, int vecinoZ);

	// Operador de asignación
	Bloque& operator = (const Bloque& b);

	// Destructor
	~Bloque();

	// Operadores
	double operator[] (Coord coord) { return coords[coord]; };

	// Coordenadas del Bloque
	igvPunto3D get_coords() { return coords; };

	// Propiedades
	tipoBloque get_tipo() { return tipo; };

	// SELECCION
	bool get_seleccionado() { return seleccionado; };
	void set_seleccionado(bool s) { seleccionado = s; };

	// Estructura
	auto get_mallaTriangulos() { return mallaTriangulos; };
	auto get_vertices() { return vertices; };

	// Inicializadores de vertices y normales
	void crearVertices();
	void crearNormales();

	// Control de normales
	void actualizarNormales(Bloque vecino);
	void actualizarNormales(Bloque vecino, int desaparecido);
	void dividirNormalesEnCaras();

	// Visualizacion
	void visualizar();
	void visualizar(tipoMovimiento side);
};
#endif
