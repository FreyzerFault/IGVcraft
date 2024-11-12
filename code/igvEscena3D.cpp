#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"
#include <iostream>

// Metodos constructores

igvEscena3D::igvEscena3D() {
	ejes = true;

	posJugador = posInicialJugador;
	yawJugador = orientacionInicial;

	// Creación de un modelo para el Jugador
	enderman = new Enderman();

	// Creación Inicial de x CHUNKS [-ditanciaChunks, distanciaChunks]
	distanciaChunks = 4;
	int cont = 0;
	for (int i = -distanciaChunks; i < distanciaChunks; i++)
		for (int j = -distanciaChunks; j < distanciaChunks; j++)
		{
			auto it = addChunk(igvPunto3D(i * tamCHUNK, 0, j * tamCHUNK));
			cout << "Chunk creado: " << it->second->get_chunk()[0]
				<< ", " << it->second->get_chunk()[1] << endl;
		};
	// Actualizacion de normales de todos los bloques
	for (auto it = chunks.begin(); it != chunks.end(); it++)
	{
		it->second->actualizarNormales();
	}
}

igvEscena3D::~igvEscena3D() {
	delete enderman;
}

// Ejes X Y Z
void pintar_ejes(void) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	glBegin(GL_LINES);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, verde);
	glBegin(GL_LINES);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);
	glEnd();
}

///// RESET
void igvEscena3D::resetMundo() {
	// Reseteamos al enderman y lo colocamos en (0,0,0)
	enderman->reset();
	enderman->set_posicion(posInicialJugador);
	enderman->set_orientacionH(orientacionInicial);
}

// Añade el chunk y actualiza la vecindad
// Comprueba a cada lado si hay chunk,
//  lo añade como vecino y añade el chunk como vecino de su vecino
// Devuelve el iterador al Chunk añadido
map<igvPunto3D, Chunk*>::iterator igvEscena3D::addChunk(igvPunto3D p) {
	Chunk* chunk = new Chunk(p[X], p[Z]);

	// Añado el chunk al mapa
	map<igvPunto3D, Chunk*>::iterator it = chunks.insert(pair<igvPunto3D, Chunk*>(
		igvPunto3D(chunk->get_chunk()[0], 0, chunk->get_chunk()[1]),
		chunk)).first;

	// Añado los vecinos
	map<igvPunto3D, Chunk*>::iterator vecino =
		chunks.find(igvPunto3D(it->first[X] + 1, 0, it->first[Z]));
	if (vecino != chunks.end()) {
		it->second->addVecino(vecino->second, 1, 0);
		vecino->second->addVecino(it->second, -1, 0);
	}

	vecino = chunks.find(igvPunto3D(it->first[X] - 1, 0, it->first[Z]));
	if (vecino != chunks.end()) {
		it->second->addVecino(vecino->second, -1, 0);
		vecino->second->addVecino(it->second, 1, 0);
	}

	vecino = chunks.find(igvPunto3D(it->first[X], 0, it->first[Z] + 1));
	if (vecino != chunks.end()) {
		it->second->addVecino(vecino->second, 0, 1);
		vecino->second->addVecino(it->second, 0, -1);
	}

	vecino = chunks.find(igvPunto3D(it->first[X], 0, it->first[Z] - 1));
	if (vecino != chunks.end()) {
		it->second->addVecino(vecino->second, 0, -1);
		vecino->second->addVecino(it->second, 0, 1);
	}
	// Devuelvo la posicion en el mapa del Chunk añadido
	return it;
}

// Devuelve el Chunk en el que se encuentra el jugador
Chunk* igvEscena3D::chunkJugador() {
	return chunks.find(posJugador / tamCHUNK)->second;
}

// Devuelve el Chunk al que pertenece una Coordenada
Chunk* igvEscena3D::get_chunk(igvPunto3D p) {
	map<igvPunto3D, Chunk*, ComparePunto3D>::iterator it = chunks.find(p / tamCHUNK);
	if (it != chunks.end())
		return it->second;
	else
		return NULL;
}

void igvEscena3D::visualizar() {
	glPushMatrix();

	glShadeModel(GL_SMOOTH);

	// Ejes
	if (ejes) pintar_ejes();

	// Material Basico en Blanco para contrastar con la iluminacion y la textura
	materialBasico.aplicar();
	// Fuente de Luz simulando el sol
	sol.aplicar();

	// Jugador
	enderman->visualizar();

	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		// Comprueba si el chunk esta detras del jugador, si no lo renderiza
		if (abs(it->second->get_chunk()[0] - floor(posJugador[X] / tamCHUNK)) <= distanciaRender
			&& abs(it->second->get_chunk()[1] - floor(posJugador[Z] / tamCHUNK)) <= distanciaRender) {
			if (cos(yawJugador) >= 0 && sin(yawJugador) >= 0) {		// +X +Z
				if (it->second->get_coords()[0] + tamCHUNK > posJugador[X] ||
					it->second->get_coords()[1] + tamCHUNK > posJugador[Z])
					it->second->visualizar();
			}
			if (cos(yawJugador) >= 0 && sin(yawJugador) < 0) {		// +X -Z
				if (it->second->get_coords()[0] + tamCHUNK > posJugador[X] ||
					it->second->get_coords()[1] - tamCHUNK < posJugador[Z])
					it->second->visualizar();
			}
			if (cos(yawJugador) < 0 && sin(yawJugador) >= 0) {		// -X +Z
				if (it->second->get_coords()[0] - tamCHUNK < posJugador[X] ||
					it->second->get_coords()[1] + tamCHUNK > posJugador[Z])
					it->second->visualizar();
			}
			if (cos(yawJugador) < 0 && sin(yawJugador) < 0) {		// -X -Z
				if (it->second->get_coords()[0] - tamCHUNK < posJugador[X] ||
					it->second->get_coords()[1] - tamCHUNK < posJugador[Z])
					it->second->visualizar();
			}
		}
	}

	glPopMatrix();
}