#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include "Chunk.h"

Chunk::Chunk()
{
	numChunk[0] = 0; numChunk[1] = 0;
	coords[0] = 0; coords[1] = 0;
}

// Construye el Chunk a partir de una Posición cualquiera dentro del Chunk
Chunk::Chunk(double posX, double posZ)
{
	numChunk[0] = int(floor(posX / tamCHUNK)); numChunk[1] = int(floor(posZ / tamCHUNK));
	coords[0] = numChunk[0] * tamCHUNK; coords[1] = numChunk[1] * tamCHUNK;
	chunkVecinoX = NULL;
	chunkVecinox = NULL;
	chunkVecinoZ = NULL;
	chunkVecinoz = NULL;

	// Se crea el mundo con una 1º capa de Bedrock, una superficie de Grass
	// una capa inferior a la superficie de Dirt, y lo demas Stone
	for (int i = 0; i < tamCHUNK; i++)
		for (int j = 0; j < alturaMundo; j++)
			for (int k = 0; k < tamCHUNK; k++)
			{
				igvPunto3D p(coords[0] + i, j, coords[1] + k);
				if (j == 0)
					bloques.insert(pair<igvPunto3D, Bloque*>(
						convertirCoordRelativa(p), new Bloque(p, Bedrock)));
				if (j > 0 && j < alturaMundo - 2)
					bloques.insert(pair<igvPunto3D, Bloque*>(
						convertirCoordRelativa(p), new Bloque(p, Stone)));
				if (j == alturaMundo - 2)
					bloques.insert(pair<igvPunto3D, Bloque*>(
						convertirCoordRelativa(p), new Bloque(p, Dirt)));
				if (j == alturaMundo - 1)
					bloques.insert(pair<igvPunto3D, Bloque*>(
						convertirCoordRelativa(p), new Bloque(p, Grass)));
			}
}

// Constructor COPIA
Chunk::Chunk(const Chunk& c) : Chunk((double)c.coords[0], (double)c.coords[1])
{
}

// Hace una copia de un chunk a una distancia X * tamCHUNK, Z * tamCHUNK
Chunk::Chunk(const Chunk& c, int vecinoX, int vecinoZ) :
	Chunk(c.coords[0] + tamCHUNK * vecinoX, c.coords[1] + tamCHUNK * vecinoZ)
{
}

// Operador de ASIGNACIÓN
Chunk& Chunk::operator=(const Chunk& c)
{
	if (!this == NULL) {
		bloques.clear();
		coords[0] = c.coords[0]; coords[1] = c.coords[1];
		numChunk[0] = c.numChunk[0]; numChunk[1] = c.numChunk[1];
		bloques = c.bloques;
	}
	return *this;
}

Chunk::~Chunk()
{
}

// Busca en el mapa si hay un hueco en cualquiera de las 6 caras del bloque:
// X+1, Y+1, Z+1,
// X-1, Y-1, Z-1
bool Chunk::ladoExterior(igvPunto3D p, tipoMovimiento side) { // p es la clave del bloque (posicion relativa al chunk)
	igvPunto3D posVecino;
	switch (side) {
	case arriba:
		posVecino = igvPunto3D(p[X], p[Y] + 1, p[Z]);
		break;
	case abajo:
		posVecino = igvPunto3D(p[X], p[Y] - 1, p[Z]);
		break;
	case adelante:
		if (p[X] == 0) // Borde CHUNK
			if (chunkVecinox != NULL && chunkVecinox->existe_bloque(igvPunto3D(tamCHUNK - 1, p[Y], p[Z]))) return false;
			else return true;
		else
			posVecino = igvPunto3D(p[X] - 1, p[Y], p[Z]);
		break;
	case atras:
		if (p[X] == tamCHUNK - 1) // Borde CHUNK
			if (chunkVecinoX != NULL && chunkVecinoX->existe_bloque(igvPunto3D(0, p[Y], p[Z]))) return false;
			else return true;
		else
			posVecino = igvPunto3D(p[X] + 1, p[Y], p[Z]);
		break;
	case izquierda:
		if (p[Z] == 0) // Borde CHUNK
			if (chunkVecinoz != NULL && chunkVecinoz->existe_bloque(igvPunto3D(p[X], p[Y], tamCHUNK - 1))) return false;
			else return true;
		else
			posVecino = igvPunto3D(p[X], p[Y], p[Z] - 1);
		break;
	case derecha:
		if (p[Z] == tamCHUNK - 1) // Borde CHUNK
			if (chunkVecinoZ != NULL && chunkVecinoZ->existe_bloque(igvPunto3D(p[X], p[Y], 0))) return false;
			else return true;
		else
			posVecino = igvPunto3D(p[X], p[Y], p[Z] + 1);
		break;
	}
	if (!existe_bloque(posVecino)) return true;
	return false;
}

// Actualiza las normales de todos los bloques del Chunk
// (Se utiliza al generar el Chunk)
void Chunk::actualizarNormales() {
	for (auto it = bloques.begin(); it != bloques.end(); it++) {
		actualizarNormales(it->first, 0);
	}
}

// Actualiza las normales de un Bloque respecto a sus Vecinos
//  y las normales de los Vecinos respecto del Bloque
// Vecinos = X+1, X-1, Y+1, Y-1, Z+1, Z-1 (un vecino por cara del bloque)
// "eliminado" indica si la actualizacion es por eliminacion del bloque o por colocacion:
//  Si el bloque se borra eliminado = true, si se ha añadido eliminado = false
void Chunk::actualizarNormales(igvPunto3D p, bool eliminado) {
	Chunk* chunkVecino = this;
	// +X
	igvPunto3D vecino = igvPunto3D(p[X] + 1, p[Y], p[Z]);
	if (p[X] == tamCHUNK - 1) {	// Si es borde comprobamos el chunk vecino respectivo
		vecino[X] = 0;
		chunkVecino = getVecino(atras); // Atras es +X
		actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	}
	else {
		actualizarParesNormales(this, this, p, vecino, eliminado);
	}
	// -X
	vecino = igvPunto3D(p[X] - 1, p[Y], p[Z]);
	if (p[X] == 0) {
		vecino[X] = tamCHUNK - 1;
		chunkVecino = getVecino(adelante); // Adelante es -X
		actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	}
	else {
		actualizarParesNormales(this, this, p, vecino, eliminado);
	}
	// +Y
	vecino = igvPunto3D(p[X], p[Y] + 1, p[Z]);
	actualizarParesNormales(this, this, p, vecino, eliminado);
	// -Y
	vecino = igvPunto3D(p[X], p[Y] - 1, p[Z]);
	actualizarParesNormales(this, this, p, vecino, eliminado);
	// +Z
	vecino = igvPunto3D(p[X], p[Y], p[Z] + 1);
	if (p[Z] == tamCHUNK - 1) {
		vecino[Z] = 0;
		chunkVecino = getVecino(derecha); // Derecha es +Z
		actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	}
	else {
		actualizarParesNormales(this, this, p, vecino, eliminado);
	}
	// -Z
	vecino = igvPunto3D(p[X], p[Y], p[Z] - 1);
	if (p[Z] == 0) {
		vecino[Z] = tamCHUNK - 1;
		chunkVecino = getVecino(izquierda); // Izquierda es -Z
		actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	}
	else {
		actualizarParesNormales(this, this, p, vecino, eliminado);
	}
	// DIAGONALES:
	// +X +Y
	//		vecino = igvPunto3D(p[X] + 1, p[Y] + 1, p[Z]);
	//		if (p[X] == tamCHUNK - 1) {
	//			vecino[X] = 0;
	//			chunkVecino = getVecino(atras);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// +X -Y
	//		vecino = igvPunto3D(p[X] + 1, p[Y] - 1, p[Z]);
	//		if (p[X] == tamCHUNK - 1) {
	//			vecino[X] = 0;
	//			chunkVecino = getVecino(atras);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// +X +Z
	//		vecino = igvPunto3D(p[X] + 1, p[Y], p[Z] + 1);
	//		if (p[X] == tamCHUNK - 1 || p[Z] == tamCHUNK - 1) {
	//			chunkVecino = this;
	//			if (p[X] == tamCHUNK - 1) {
	//				vecino[X] = 0;
	//				chunkVecino = getVecino(atras);
	//			}
	//			if (p[Z] == tamCHUNK - 1 && chunkVecino != NULL) {
	//				vecino[Z] = 0;
	//				chunkVecino = chunkVecino->getVecino(derecha);
	//			}
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// +X -Z
	//		vecino = igvPunto3D(p[X] + 1, p[Y], p[Z] - 1);
	//		if (p[X] == tamCHUNK - 1 || p[Z] == 0) {
	//			chunkVecino = this;
	//			if (p[X] == tamCHUNK - 1) {
	//				vecino[X] = 0;
	//				chunkVecino = getVecino(atras);
	//			}
	//			if (p[Z] == 0 && chunkVecino != NULL) {
	//				vecino[Z] = tamCHUNK - 1;
	//				chunkVecino = chunkVecino->getVecino(izquierda);
	//			}
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -X +Y
	//		vecino = igvPunto3D(p[X] - 1, p[Y] + 1, p[Z]);
	//		if (p[X] == 0) {
	//			vecino[X] = tamCHUNK - 1;
	//			chunkVecino = getVecino(adelante);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -X -Y
	//		vecino = igvPunto3D(p[X] - 1, p[Y] - 1, p[Z]);
	//		if (p[X] == 0) {
	//			vecino[X] = tamCHUNK - 1;
	//			chunkVecino = getVecino(adelante);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -X +Z
	//		vecino = igvPunto3D(p[X] - 1, p[Y], p[Z] + 1);
	//		if (p[X] == 0 || p[Z] == tamCHUNK - 1) {
	//			chunkVecino = this;
	//			if (p[X] == 0) {
	//				vecino[X] = tamCHUNK - 1;
	//				chunkVecino = getVecino(adelante);
	//			}
	//			if (p[Z] == tamCHUNK - 1 && chunkVecino != NULL) {
	//				vecino[Z] = 0;
	//				chunkVecino = chunkVecino->getVecino(derecha);
	//			}
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -X -Z
	//		vecino = igvPunto3D(p[X] - 1, p[Y], p[Z] - 1);
	//		if (p[X] == 0 || p[Z] == 0) {
	//			chunkVecino = this;
	//			if (p[X] == 0) {
	//				vecino[X] = tamCHUNK - 1;
	//				chunkVecino = getVecino(adelante);
	//			}
	//			if (p[Z] == 0 && chunkVecino != NULL) {
	//				vecino[Z] = tamCHUNK - 1;
	//				chunkVecino = chunkVecino->getVecino(izquierda);
	//			}
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// +Y +Z
	//		vecino = igvPunto3D(p[X], p[Y] + 1, p[Z] + 1);
	//		if (p[Z] == tamCHUNK - 1) {
	//			vecino[Z] = 0;
	//			chunkVecino = getVecino(derecha);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// +Y -Z
	//		vecino = igvPunto3D(p[X], p[Y] + 1, p[Z] - 1);
	//		if (p[Z] == 0) {
	//			vecino[Z] = tamCHUNK - 1;
	//			chunkVecino = getVecino(izquierda);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -Y +Z
	//		vecino = igvPunto3D(p[X], p[Y] - 1, p[Z] + 1);
	//		if (p[Z] == tamCHUNK - 1) {
	//			vecino[Z] = 0;
	//			chunkVecino = getVecino(derecha);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
	//// -Y -Z
	//		vecino = igvPunto3D(p[X], p[Y] - 1, p[Z] - 1);
	//		if (p[Z] == 0) {
	//			vecino[Z] = tamCHUNK - 1;
	//			chunkVecino = getVecino(izquierda);
	//			actualizarParesNormales(this, chunkVecino, p, vecino, eliminado);
	//		}
	//		else {
	//			actualizarParesNormales(this, this, p, vecino, eliminado);
	//		}
}

// Actualiza las normales un bloque respecto el otro y viceversa,
//  eliminado indica si la actualizacion es por colocacion del 2º bloque (false),
//  o por eliminacion (true)
void Chunk::actualizarParesNormales(Chunk* c1, Chunk* c2, igvPunto3D key1, igvPunto3D key2, bool eliminado) {
	if (c1 != NULL && c2 != NULL && c1->existe_bloque(key1) && c2->existe_bloque(key2)) {
		if (!eliminado)
			c1->get_bloque(key1)->actualizarNormales(*c2->get_bloque(key2), eliminado);
		c2->get_bloque(key2)->actualizarNormales(*c1->get_bloque(key1), eliminado);
	}
}

// Añade el Bloque al Chunk,
//  actualiza las normales del bloque y sus vecinos
//  y devuelve TRUE si todo ha ido bien, FALSE si ya habia un bloque en esa posicion
bool Chunk::addBloque(Bloque b) {
	igvPunto3D key(convertirCoordRelativa(b.get_coords()));

	// Insert devuelve TRUE si se insertó correctamente,
	//  y FALSE si había un bloque en esa posición, y no se inserta
	bool insertado = bloques.insert(pair<igvPunto3D, Bloque*>(key, new Bloque(b))).second;

	// Actualizo las normales del bloque respecto a sus vecinos
	//  y las normales de sus vecinos respecto del nuevo bloque
	actualizarNormales(key, 0);

	return insertado;
}

// Actualiza las normales de los vecinos del bloque eliminado
//  y lo elimina del mapa
void Chunk::deleteBloque(Bloque b) {
	actualizarNormales(convertirCoordRelativa(b.get_coords()), 1);
	bloques.erase(bloques.find(convertirCoordRelativa(b.get_coords())));
}

// X & Z = [-1,1]
// (1,0) vecinoX, (0,1) vecinoZ, (-1,0) vecinox, (0,-1) vecinoz
void Chunk::addVecino(Chunk* vecino, int x, int z) {
	if (x == 1 && z == 0) chunkVecinoX = vecino;
	if (x == -1 && z == 0) chunkVecinox = vecino;
	if (z == 1 && x == 0) chunkVecinoZ = vecino;
	if (z == -1 && x == 0) chunkVecinoz = vecino;
}

Chunk* Chunk::getVecino(tipoMovimiento side) {
	switch (side) {
	case adelante:
		return chunkVecinox;
	case atras:
		return chunkVecinoX;
	case izquierda:
		return chunkVecinoz;
	case derecha:
		return chunkVecinoZ;
	}
}

// Devuelve el punto convertido
//  en caso de ser negativa una coordenada hay que calcularla como si fuera positiva en orden inverso
//  para que la coordenada 0,0 empiece siempre en la esquina de menor X y Z
igvPunto3D Chunk::convertirCoordRelativa(igvPunto3D p) {
	if (p[X] >= 0)
		p[X] = (int)floor(p[X]) % tamCHUNK;
	else
		p[X] = (int)floor(p[X] - tamCHUNK * numChunk[0]) % tamCHUNK;
	if (p[Z] >= 0)
		p[Z] = (int)floor(p[Z]) % tamCHUNK;
	else
		p[Z] = (int)floor(p[Z] - tamCHUNK * numChunk[1]) % tamCHUNK;

	p[Y] = floor(p[Y]);
	return p;
}

// Visualiza todos los bloques del mapa y aplica sus texturas cara a cara
void Chunk::visualizar() {
	map<igvPunto3D, Bloque*, ComparePunto3D>::iterator it;
	map<igvPunto3D, Bloque*, ComparePunto3D>::iterator itAnterior;

	// La primera vez siempre aplica la textura
	bool primeraVez = true;

	// Iterador atrasado una posicion siempre para comprobar si la textura ha cambiado
	itAnterior = it = bloques.begin();

	for (it = bloques.begin(); it != bloques.end(); it++) {
		// Si es Glowstone se activa una Fuente de Luz que genera una luz ambiental amarilla y tenue
		if (it->second->get_tipo() == Glowstone) {
			igvFuenteLuz luzGlowstone(GL_LIGHT2,
				igvPunto3D(igvPunto3D(it->second->get_coords()[X] + 0.5, it->second->get_coords()[Y] + 0.5, it->second->get_coords()[Z] + 0.5)),
				igvColor(1, 1, 0, 1), igvColor(1, 1, 0, 1), igvColor(1, 1, 0, 1),
				1, 1, 1);
			luzGlowstone.aplicar();
		}

		// El primer bloque aplica la textura correspondiente
		//  y por cada vez que un bloque sea distinto del anterior aplicamos la textura
		if (it->second->get_tipo() != itAnterior->second->get_tipo() || primeraVez) {
			texturasBloques[it->second->get_tipo()].aplicar();
			primeraVez = false;
		}
		// Comprueba para cada cara si no estan tapadas, mejora muchisimo el rendimiento
		if (ladoExterior(it->first, arriba)) {
			it->second->visualizar(arriba);
		}
		if (ladoExterior(it->first, abajo)) {
			// Para Grass necesitamos la textura de la Dirt debajo
			if (it->second->get_tipo() == Grass)
				texturasBloques[Dirt].aplicar();
			it->second->visualizar(abajo);
		}
		// Si cualquiera de las caras laterales es visible
		//  se aplica la textura lateral en caso de ser Grass o Log
		if (ladoExterior(it->first, adelante) || ladoExterior(it->first, atras) ||
			ladoExterior(it->first, izquierda) || ladoExterior(it->first, derecha)) {
			if (it->second->get_tipo() == Grass)
				texturasBloques[GrassSide].aplicar();
			if (it->second->get_tipo() == Log)
				texturasBloques[LogSide].aplicar();

			if (ladoExterior(it->first, adelante))
				it->second->visualizar(adelante);
			if (ladoExterior(it->first, atras))
				it->second->visualizar(atras);
			if (ladoExterior(it->first, izquierda))
				it->second->visualizar(izquierda);
			if (ladoExterior(it->first, derecha))
				it->second->visualizar(derecha);
		}
		// Se vuelve a aplicar la textura original para texturas complejas
		if (it->second->get_tipo() == Grass || it->second->get_tipo() == Log)
			texturasBloques[it->second->get_tipo()].aplicar();

		itAnterior = it;
	}
}