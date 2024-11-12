#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include "Chunk.h"
#include "Enderman.h"
#include "igvMaterial.h"
#include "igvFuenteLuz.h"

class igvEscena3D {
protected:

	bool ejes;

	// Usado como Jugador
	Enderman* enderman;

	// La clave es (numChunk[0], 0, numChunk[1])
	map<igvPunto3D, Chunk*, ComparePunto3D> chunks;

	// Distancia a la que se crean chunks desde 0,0
	int distanciaChunks;

	// Rastrea la posicion y hacia donde mira para no renderizar chunks que no sean visibles para el jugador
	igvPunto3D posJugador;
	double yawJugador;

public:

	igvEscena3D();
	~igvEscena3D();

	void visualizar(void);

	// Ejes X Y Z
	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

	// Enderman
	auto get_enderman() { return enderman; };

	// Hace reset al Enderman
	void resetMundo();

	// Añade un Chunk al mundo en la posicion p
	map<igvPunto3D, Chunk*>::iterator addChunk(igvPunto3D p);

	// En que chunk estoy
	Chunk* chunkJugador();
	Chunk* get_chunk(igvPunto3D p);

	// Posicion y orientacion del jugador
	igvPunto3D get_posJ() { return posJugador; };
	void set_posJ(igvPunto3D p) { posJugador = p; };
	double get_yaw() { return yawJugador; };
	void set_yaw(double y) { yawJugador = y; };

	// Mapa de chunks
	auto get_chunks() { return chunks; };
};

#endif
