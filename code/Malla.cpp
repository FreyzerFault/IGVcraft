#include "Malla.h"

Malla::Malla() {
	mallaTriangulos = new int[36]{	// 6 caras * 2 Triangulos * 3 vertices
		// CARA INFERIOR
		0,2,1,
		0,3,2,
		// CARA SUPERIOR
		4,6,7,
		4,5,6,
		// CARA FRONTAL
		0,5,4,
		0,1,5,
		// CARA DERECHA
		1,6,5,
		1,2,6,
		// CARA TRASERA
		2,7,6,
		2,3,7,
		// CARA LEFT
		3,4,7,
		3,0,4
	};
	up = new int[6]{
		4,6,7,
		4,5,6
	};
	down = new int[6]{
		0,2,1,
		0,3,2
	};
	front = new int[6]{
		0,5,4,
		0,1,5
	};
	back = new int[6]{
		2,7,4,
		2,3,7
	};
	left = new int[6]{
		3,4,7,
		3,0,4
	};
	right = new int[6]{
		1,6,5,
		1,2,6
	};

	mallaCara = new int[6]{ // Adaptado a 4 vertices
		0,1,3,
		1,2,3
	};

	texCoords = new int[16]{ // 4 vertices * 2 coords de Textura
		0,0,
		0,1,
		1,1,
		1,0,
	};
}

Malla::~Malla() {
	delete[] mallaTriangulos;
}