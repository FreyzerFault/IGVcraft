/************************************************************************************************/
/* CHUNK forma un espacio de tamCHUNK x tamCHUNK x maxHeight, controla los bloques				*/
/************************************************************************************************/

#ifndef __CHUNK
#define __CHUNK

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif
#include "Bloque.h"
#include <map>

using namespace std;

// Comparador de puntos en 3D para ordenar el mapa de bloques
struct ComparePunto3D {
	bool operator()(const igvPunto3D& a, const igvPunto3D& b) const {
		if (a[1] == b[1])
			if (a[2] == b[2])
				return a[0] < b[0];
			else return a[2] < b[2];
		else return a[1] < b[1];
	}
};

class Chunk {
private:
	// Coordenadas de inicio del chunk (esquina de menor X,Z)
	int coords[2];
	// Clave del chunk (coords[X,Z] / MAX_X,Z)
	int numChunk[2];

	// La clave es el Punto Relativo al Chunk (coords[X,Y,Z] % MAX_X,Y,Z)
	map<igvPunto3D, Bloque*, ComparePunto3D> bloques;

	// Vecinos del chunk en X, -x, Z, -z
	Chunk* chunkVecinoX;
	Chunk* chunkVecinox;
	Chunk* chunkVecinoZ;
	Chunk* chunkVecinoz;

public:
	// Constructores
	Chunk();
	Chunk(double x, double z);

	// Constructor de copia
	Chunk(const Chunk& c);
	Chunk(const Chunk& c, int vecinoX, int vecinoZ);

	// Operador de asignaci�n
	Chunk& operator = (const Chunk& c);

	// Destructor
	~Chunk();

	// Operadores
	int operator[] (const int coord) { return coords[coord]; };

	auto get_chunk() { return numChunk; };
	auto get_coords() { return coords; };

	// Busca un bloque en el mapa por su clave (punto relativo al Chunk)
	Bloque* get_bloque(igvPunto3D p) { return bloques.find(p)->second; };
	// Busca un bloque en el mapa y devuelve si existe o no
	bool existe_bloque(igvPunto3D p) { return bloques.count(p); };

	// Comprueba si la cara "side" del bloque con clave p (punto relativo al Chunk) no es tapada por un bloque
	bool ladoExterior(igvPunto3D p, tipoMovimiento side);

	// Control de Normales
	void actualizarNormales();
	void actualizarNormales(igvPunto3D p, bool eliminado);
	void actualizarNormalesVecinos(igvPunto3D p, bool eliminado);
	void actualizarParesNormales(Chunk* c1, Chunk* c2, igvPunto3D key1, igvPunto3D key2, bool eliminado);

	// Añade un bloque al mapa
	bool addBloque(Bloque b);
	// Elimina un bloque del mapa
	void deleteBloque(Bloque b);

	// Vecino en +X, -X, +Z o -Z
	void addVecino(Chunk* vecino, int x, int z);
	Chunk* getVecino(tipoMovimiento side);

	// Convierte una coordenada Global en la relativa al chunk, usada por el map de bloques como clave
	igvPunto3D convertirCoordRelativa(igvPunto3D p);

	// Visualiza todos los bloques en el mapa con sus texturas
	void visualizar();
};
#endif
