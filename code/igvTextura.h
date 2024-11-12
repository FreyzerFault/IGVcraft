#ifndef __IGVTEXTURA
#define __IGVTEXTURA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
using namespace std;

class igvTextura {
protected:

	// ID de la textura
	unsigned int idTextura;
	// Almacena la imagen para poderla aplicar cuando se quiera sin necesitar leer el archivo
	SDL_Surface* imagen = NULL;

public:
	igvTextura();
	igvTextura(const char* fichero);
	~igvTextura();

	void aplicar(void);
	void setIdTextura(unsigned int id) { this->idTextura = id; }
	unsigned int getIdTextura() { return this->idTextura; }

	unsigned int get_ancho() { return imagen->w; };
	unsigned int get_alto() { return imagen->h; };
};

#endif
