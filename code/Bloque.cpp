#include "Bloque.h"
#include <iostream>

Bloque::Bloque() : Bloque(igvPunto3D(0, 0, 0))
{
}

Bloque::Bloque(igvPunto3D coord) : Bloque(coord, Dirt)
{
}

Bloque::Bloque(igvPunto3D coord, tipoBloque t)
{
	seleccionado = false;

	coords = coord;
	tipo = t;
	mallaTriangulos = malla; // Coge la malla de las Constantes

	// Calcula vertices y normales
	crearVertices();
	crearNormales();
}

// Constructor COPIA
Bloque::Bloque(const Bloque& c) : Bloque(c.coords, c.tipo) {
}

// Crea una copia del Bloque en una posición vecina
Bloque::Bloque(const Bloque& b, int vecinoX, int vecinoY, int vecinoZ) :
	Bloque(igvPunto3D(b.coords[X] + vecinoX, b.coords[Y] + vecinoY, b.coords[Z] + vecinoZ), b.tipo)
{
}

// Copia las propiedades del Bloque
Bloque& Bloque::operator=(const Bloque& b)
{
	if (!this == NULL) {
		this->tipo = b.tipo;
		// COPIAR PROPIEDADES
	}
	return *this;
}

Bloque::~Bloque()
{
	delete[] vertices;
	delete[] verticesUp;
	delete[] verticesDown;
	delete[] verticesFront;
	delete[] verticesBack;
	delete[] verticesLeft;
	delete[] verticesRight;

	delete[] normales;
	delete[] normalesUp;
	delete[] normalesDown;
	delete[] normalesFront;
	delete[] normalesBack;
	delete[] normalesLeft;
	delete[] normalesRight;
}

void Bloque::crearVertices() {
	vertices = new double[24]{	// 8 vertices * 3 coords
		coords[X],		coords[Y],		coords[Z],		// 0,0,0
		coords[X],		coords[Y],		coords[Z] + 1,	// 0,0,1
		coords[X] + 1,	coords[Y],		coords[Z] + 1,	// 1,0,1
		coords[X] + 1,	coords[Y],		coords[Z],		// 1,0,0
		coords[X],		coords[Y] + 1,	coords[Z],		// 0,1,0
		coords[X],		coords[Y] + 1,	coords[Z] + 1,	// 0,1,1
		coords[X] + 1,	coords[Y] + 1,	coords[Z] + 1,	// 1,1,1
		coords[X] + 1,	coords[Y] + 1,	coords[Z],		// 1,1,0
	};

	verticesUp = new double[12];
	verticesDown = new double[12];
	verticesFront = new double[12];
	verticesBack = new double[12];
	verticesLeft = new double[12];
	verticesRight = new double[12];

	for (int j = 0; j < 3; j++)
	{
		verticesUp[0 * 3 + j] = vertices[7 * 3 + j];
		verticesUp[1 * 3 + j] = vertices[4 * 3 + j];
		verticesUp[2 * 3 + j] = vertices[5 * 3 + j];
		verticesUp[3 * 3 + j] = vertices[6 * 3 + j];

		verticesDown[0 * 3 + j] = vertices[0 * 3 + j];
		verticesDown[1 * 3 + j] = vertices[3 * 3 + j];
		verticesDown[2 * 3 + j] = vertices[2 * 3 + j];
		verticesDown[3 * 3 + j] = vertices[1 * 3 + j];

		verticesFront[0 * 3 + j] = vertices[4 * 3 + j];
		verticesFront[1 * 3 + j] = vertices[0 * 3 + j];
		verticesFront[2 * 3 + j] = vertices[1 * 3 + j];
		verticesFront[3 * 3 + j] = vertices[5 * 3 + j];

		verticesBack[0 * 3 + j] = vertices[6 * 3 + j];
		verticesBack[1 * 3 + j] = vertices[2 * 3 + j];
		verticesBack[2 * 3 + j] = vertices[3 * 3 + j];
		verticesBack[3 * 3 + j] = vertices[7 * 3 + j];

		verticesLeft[0 * 3 + j] = vertices[7 * 3 + j];
		verticesLeft[1 * 3 + j] = vertices[3 * 3 + j];
		verticesLeft[2 * 3 + j] = vertices[0 * 3 + j];
		verticesLeft[3 * 3 + j] = vertices[4 * 3 + j];

		verticesRight[0 * 3 + j] = vertices[5 * 3 + j];
		verticesRight[1 * 3 + j] = vertices[1 * 3 + j];
		verticesRight[2 * 3 + j] = vertices[2 * 3 + j];
		verticesRight[3 * 3 + j] = vertices[6 * 3 + j];
	}
}

void Bloque::crearNormales() {
	normales = new int[24]{	// 8 normales
		-1,-1,-1,

		-1,-1, 1,

		 1,-1, 1,

		 1,-1,-1,

		-1, 1,-1,

		-1, 1, 1,

		 1, 1, 1,

		 1, 1,-1,
	};

	normalesUp = new int[12];
	normalesDown = new int[12];
	normalesFront = new int[12];
	normalesBack = new int[12];
	normalesLeft = new int[12];
	normalesRight = new int[12];

	dividirNormalesEnCaras();
}

// Actualiza las normales del bloque respecto a un Bloque vecino
// El bloque vecino debe estar pegado a una de las caras
// Segun la cara donde se encuentra se actualizan las normales
//  de los vertices de union entre los dos cubos
void Bloque::actualizarNormales(Bloque vecino) {
	int difX, difY, difZ;
	difX = (int)vecino[X] - (int)coords[X];
	difY = (int)vecino[Y] - (int)coords[Y];
	difZ = (int)vecino[Z] - (int)coords[Z];
	if (difY == 0 && difZ == 0) {
		if (difX > 0) {
			normales[2 * 3 + X] = 0;
			normales[3 * 3 + X] = 0;
			normales[6 * 3 + X] = 0;
			normales[7 * 3 + X] = 0;
		}
		if (difX < 0) {
			normales[0 * 3 + X] = 0;
			normales[1 * 3 + X] = 0;
			normales[4 * 3 + X] = 0;
			normales[5 * 3 + X] = 0;
		}
	}
	if (difX == 0 && difZ == 0) {
		if (difY > 0) {
			normales[4 * 3 + Y] = 0;
			normales[5 * 3 + Y] = 0;
			normales[6 * 3 + Y] = 0;
			normales[7 * 3 + Y] = 0;
		}
		if (difY < 0) {
			normales[0 * 3 + Y] = 0;
			normales[1 * 3 + Y] = 0;
			normales[2 * 3 + Y] = 0;
			normales[3 * 3 + Y] = 0;
		}
	}
	if (difX == 0 && difY == 0) {
		if (difZ > 0) {
			normales[1 * 3 + Z] = 0;
			normales[2 * 3 + Z] = 0;
			normales[5 * 3 + Z] = 0;
			normales[6 * 3 + Z] = 0;
		}
		if (difZ < 0) {
			normales[0 * 3 + Z] = 0;
			normales[3 * 3 + Z] = 0;
			normales[4 * 3 + Z] = 0;
			normales[7 * 3 + Z] = 0;
		}
	}
	// Divide las normales en 6 arrays, uno por cara
	dividirNormalesEnCaras();
}

// Mismo metodo pero controla si el bloque ha sido añadido o eliminado
//  en caso de ser eliminado, el lateral quedara vacio y las normales serán 1 = ha desaparecido,
//  y si no ha desaparecido, sino que ha aparecido, será 0
void Bloque::actualizarNormales(Bloque vecino, int desaparecido) {
	int difX, difY, difZ;
	difX = (int)vecino[X] - (int)coords[X];
	difY = (int)vecino[Y] - (int)coords[Y];
	difZ = (int)vecino[Z] - (int)coords[Z];
	if (difY == 0 && difZ == 0) {
		if (difX > 0) {
			normales[2 * 3 + X] = desaparecido;
			normales[3 * 3 + X] = desaparecido;
			normales[6 * 3 + X] = desaparecido;
			normales[7 * 3 + X] = desaparecido;
		}
		if (difX < 0) {
			normales[0 * 3 + X] = -desaparecido;
			normales[1 * 3 + X] = -desaparecido;
			normales[4 * 3 + X] = -desaparecido;
			normales[5 * 3 + X] = -desaparecido;
		}
	}
	if (difX == 0 && difZ == 0) {
		if (difY > 0) {
			normales[4 * 3 + Y] = desaparecido;
			normales[5 * 3 + Y] = desaparecido;
			normales[6 * 3 + Y] = desaparecido;
			normales[7 * 3 + Y] = desaparecido;
		}
		if (difY < 0) {
			normales[0 * 3 + Y] = -desaparecido;
			normales[1 * 3 + Y] = -desaparecido;
			normales[2 * 3 + Y] = -desaparecido;
			normales[3 * 3 + Y] = -desaparecido;
		}
	}
	if (difX == 0 && difY == 0) {
		if (difZ > 0) {
			normales[1 * 3 + Z] = desaparecido;
			normales[2 * 3 + Z] = desaparecido;
			normales[5 * 3 + Z] = desaparecido;
			normales[6 * 3 + Z] = desaparecido;
		}
		if (difZ < 0) {
			normales[0 * 3 + Z] = -desaparecido;
			normales[3 * 3 + Z] = -desaparecido;
			normales[4 * 3 + Z] = -desaparecido;
			normales[7 * 3 + Z] = -desaparecido;
		}
	}
	// DIAGONALES
	// X = 0
	if (difY > 0) {
		if (difZ > 0) {
			normales[5 * 3 + Z] = -1;
			normales[6 * 3 + Z] = -1;
		}
		if (difZ < 0) {
			normales[4 * 3 + Z] = +1;
			normales[7 * 3 + Z] = +1;
		}
	}
	if (difY < 0) {
		if (difZ > 0) {
			normales[1 * 3 + Z] = -1;
			normales[2 * 3 + Z] = -1;
		}
		if (difZ < 0) {
			normales[0 * 3 + Z] = +1;
			normales[3 * 3 + Z] = +1;
		}
	}
	// Z = 0
	if (difY > 0) {
		if (difX > 0) {
			normales[6 * 3 + X] = -1;
			normales[7 * 3 + X] = -1;
		}
		if (difX < 0) {
			normales[4 * 3 + X] = +1;
			normales[5 * 3 + X] = +1;
		}
	}
	if (difY < 0) {
		if (difX > 0) {
			normales[2 * 3 + X] = -1;
			normales[3 * 3 + X] = -1;
		}
		if (difX < 0) {
			normales[0 * 3 + X] = +1;
			normales[1 * 3 + X] = +1;
		}
	}
	// Y = 0
	if (difZ > 0) {
		if (difX > 0) {
			normales[2 * 3 + X] = -1;
			normales[3 * 3 + X] = -1;
			normales[2 * 3 + Z] = +1;
			normales[3 * 3 + Z] = +1;
		}
		if (difX < 0) {
			normales[1 * 3 + X] = +1;
			normales[5 * 3 + X] = +1;
			normales[2 * 3 + Z] = +1;
			normales[3 * 3 + Z] = +1;
		}
	}
	if (difZ < 0) {
		if (difX > 0) {
			normales[3 * 3 + X] = -1;
			normales[7 * 3 + X] = -1;
			normales[2 * 3 + Z] = -1;
			normales[3 * 3 + Z] = -1;
		}
		if (difX < 0) {
			normales[0 * 3 + X] = +1;
			normales[4 * 3 + X] = +1;
			normales[2 * 3 + Z] = -1;
			normales[3 * 3 + Z] = -1;
		}
	}
	// Divide las normales en 6 arrays, uno por cara
	dividirNormalesEnCaras();
}

// Asigno las normales del bloque a cada una de las caras
//  para poderlas controlar por separado
void Bloque::dividirNormalesEnCaras() {
	for (int j = 0; j < 3; j++)
	{
		normalesUp[0 * 3 + j] = normales[7 * 3 + j];
		normalesUp[1 * 3 + j] = normales[4 * 3 + j];
		normalesUp[2 * 3 + j] = normales[5 * 3 + j];
		normalesUp[3 * 3 + j] = normales[6 * 3 + j];

		normalesDown[0 * 3 + j] = normales[0 * 3 + j];
		normalesDown[1 * 3 + j] = normales[3 * 3 + j];
		normalesDown[2 * 3 + j] = normales[2 * 3 + j];
		normalesDown[3 * 3 + j] = normales[1 * 3 + j];

		normalesFront[0 * 3 + j] = normales[4 * 3 + j];
		normalesFront[1 * 3 + j] = normales[0 * 3 + j];
		normalesFront[2 * 3 + j] = normales[1 * 3 + j];
		normalesFront[3 * 3 + j] = normales[5 * 3 + j];

		normalesBack[0 * 3 + j] = normales[6 * 3 + j];
		normalesBack[1 * 3 + j] = normales[2 * 3 + j];
		normalesBack[2 * 3 + j] = normales[3 * 3 + j];
		normalesBack[3 * 3 + j] = normales[7 * 3 + j];

		normalesLeft[0 * 3 + j] = normales[7 * 3 + j];
		normalesLeft[1 * 3 + j] = normales[3 * 3 + j];
		normalesLeft[2 * 3 + j] = normales[0 * 3 + j];
		normalesLeft[3 * 3 + j] = normales[4 * 3 + j];

		normalesRight[0 * 3 + j] = normales[5 * 3 + j];
		normalesRight[1 * 3 + j] = normales[1 * 3 + j];
		normalesRight[2 * 3 + j] = normales[2 * 3 + j];
		normalesRight[3 * 3 + j] = normales[6 * 3 + j];
	}
}

// Visualizacion del bloque completo con sus texturas
void Bloque::visualizar() {
	texturasBloques[tipo].aplicar();

	visualizar(arriba);
	if (tipo == Grass)
		texturasBloques[Dirt].aplicar();
	visualizar(abajo);

	if (tipo == Grass || tipo == Log)
		texturasBloques[tipo + 1].aplicar();

	visualizar(adelante);
	visualizar(atras);
	visualizar(izquierda);
	visualizar(derecha);
}

// Visualiza una cara independiente del bloque
// Tiene colores asignados en caso de ser seleccionado,
//  que sirven para seleccionar una cara aislada
void Bloque::visualizar(tipoMovimiento side) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// TEXTURAS
	glTexCoordPointer(2, GL_INT, 0, mallaTriangulos->get_texCoords());

	if (seleccionado)	// SELECCION POR COLOR
		glColor3fv(colorCara[side]);

	switch (side) {
	case arriba:
		glVertexPointer(3, GL_DOUBLE, 0, verticesUp);
		glNormalPointer(GL_INT, 0, normalesUp);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	case abajo:
		glVertexPointer(3, GL_DOUBLE, 0, verticesDown);
		glNormalPointer(GL_INT, 0, normalesDown);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	case adelante:
		glVertexPointer(3, GL_DOUBLE, 0, verticesFront);
		glNormalPointer(GL_INT, 0, normalesFront);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	case atras:
		glVertexPointer(3, GL_DOUBLE, 0, verticesBack);
		glNormalPointer(GL_INT, 0, normalesBack);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	case izquierda:
		glVertexPointer(3, GL_DOUBLE, 0, verticesLeft);
		glNormalPointer(GL_INT, 0, normalesLeft);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	case derecha:
		glVertexPointer(3, GL_DOUBLE, 0, verticesRight);
		glNormalPointer(GL_INT, 0, normalesRight);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, mallaTriangulos->get_mallaCara());
		break;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}