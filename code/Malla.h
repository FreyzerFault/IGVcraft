/************************************************************************************************/
/* MALLA forma la Malla de triangulos de un Bloque y su textura									*/
/************************************************************************************************/

#ifndef __MALLA
#define __MALLA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

class Malla
{
private:
	// La malla completa del bloque
	int* mallaTriangulos;

	// La malla separada en caras
	int* up;
	int* down;
	int* front;
	int* back;
	int* left;
	int* right;

	// Una malla adaptada a cualquier cara
	int* mallaCara;

	// Coordenadas de Textura adaptadas a cualquier cara
	int* texCoords;

public:
	Malla();
	~Malla();

	auto get_mallaTriangulos() { return mallaTriangulos; };

	auto get_up() { return up; };
	auto get_down() { return down; };
	auto get_front() { return front; };
	auto get_back() { return back; };
	auto get_left() { return left; };
	auto get_right() { return right; };

	auto get_mallaCara() { return mallaCara; };

	auto get_texCoords() { return texCoords; };
};

#endif