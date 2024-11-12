#include <cstdlib>
#include <stdio.h>

#include "Enderman.h"
#include <iostream>

// Metodos constructores

Enderman::Enderman() : Enderman( // (X,Y,Z, orientacion en º)
	posInicialJugador[X], posInicialJugador[Y], posInicialJugador[Z], orientacionInicial * 180 / PI)
{
}

Enderman::Enderman(double posX, double posY, double posZ, double ori) {
	// POSICION
	posicion = igvPunto3D(posX, posY, posZ);
	orientacionH = ori;
	orientacionVCabeza = 0;

	// Control de Animacion en false todo, quieto true
	enfadado = false;
	dab = false;
	andando = false;
	sentando = false;
	agarrando = false;
	quieto = true;

	// Control de GRADOS DE LIBERTAD del modelo
	elevacionBrazo1 = 0;
	elevacionBrazo2 = 0;
	agachado = 0;
	paso = 0;

	// Control de movimiento
	velocidadMovimiento = velocidadBase;
	sentidoMovimiento = 1;

	// Limitadores de Movimiento
	MAXIMA_ELEVACION_BRAZOS = 250;
	MINIMA_ELEVACION_BRAZOS = 0;
	MAXIMO_AGACHE = 80;
	MAXIMO_PASO = 30;

	firstPerson = false;

	bloqueUsado = Wood;
}

Enderman::~Enderman() {
}

void Enderman::arbol_escena() {
	glPushMatrix();
	glTranslatef(posicion[X], posicion[Y], posicion[Z]); // POSICION DEL MODELO

	// TORSO
	transformacionesTorso();
	if (!firstPerson)
		torso();

	// MANDIBULA
	if (enfadado) {
		glPushMatrix();
		transformacionesMandibula();
		mandibula();
		glPopMatrix();
	}
	// CABEZA
	glPushMatrix();
	transformacionesCabeza();
	cabeza();
	glPopMatrix();

	int lado;
	for (int i = 0; i < 2; i++) {
		lado = (int)pow(-1, i); // Simetria de extremidades

		glPushMatrix();		//BRAZOS
		transformacionesBrazoAnterior(lado);
		brazo();

		glPushMatrix();
		transformacionesBrazoPosterior(lado);
		brazo();
		// CUBO EN LA MANO
		if (firstPerson && lado > 0) {
			glPushMatrix();
			// Lo coloco en las manos
			glTranslatef(-tamTorso[X] / 2 - tamBrazo[X] / 2, -tamBrazo[Y] / 2, 0);
			glRotatef(70, 1, 0, 0);
			glScalef(0.7, 0.7, 0.7);
			// Dibujo el cubo
			Bloque b(igvPunto3D(-0.5, -0.5, -0.5), bloqueUsado);
			b.visualizar();
			// Vuelvo a aplicar la textura del enderman
			texturasEnderman[1].aplicar();

			glPopMatrix();
		}
		glPopMatrix();

		glPopMatrix();

		if (!firstPerson) {
			glPushMatrix();		// PIERNAS
			transformacionesPiernaAnterior(lado);
			pierna();

			glPushMatrix();
			transformacionesPiernaPosterior();
			pierna();
			glPopMatrix();

			glPopMatrix();
		}
	}

	glPopMatrix();
}

//// CONSTRUCTORES DE PARTES (FORMA Y TAMAÑO)
void Enderman::torso() {
	glPushMatrix();
	glScalef(tamTorso[X], tamTorso[Y], tamTorso[Z]);
	drawSolidCube(1);
	glPopMatrix();
}

void Enderman::cabeza() {
	if (enfadado) glTranslatef(0, 0.8, 0);
	glScalef(tamCabeza[X], tamCabeza[Y], tamCabeza[Z]);
	drawSolidCube(1);
	texturasEnderman[0].aplicar();
	drawFace();
	texturasEnderman[1].aplicar();
}

void Enderman::brazo() {
	glPushMatrix();
	glScalef(tamBrazo[X], tamBrazo[Y], tamBrazo[Z]);
	drawSolidCube(1);
	glPopMatrix();
}

void Enderman::pierna() {
	glPushMatrix();
	glScalef(tamPierna[X], tamPierna[Y], tamPierna[Z]);
	drawSolidCube(1);
	glPopMatrix();
}

//// MODIFICADORES DE PARTES (POSICION Y ROTACION)

// TORSO
void Enderman::transformacionesTorso() {
	glTranslatef(0, tamPierna[Y] * 2 + tamTorso[Y] / 2, 0); // Colocar en la altura correspondiente
	if (enfadado) glTranslatef(pow(-1, rand()) * 0.1 * (rand() % temblor), 0, pow(-1, rand()) * 0.1 * (rand() % temblor)); // Hace temblar de forma aleatoria todo el cuerpo
	glTranslatef(0, agachado * 0.01 * 2, 0);
	glRotatef(orientacionH - 90, 0, 1, 0);	// Giro horizontal antihorario (orientacion=0 -> mira al eje Z)
}

// MANDIBULA
void Enderman::transformacionesMandibula() {
	if (dab) glRotatef(20, 1, 0, 0);
	glTranslatef(0, tamTorso[Y] / 2, 0); //Posicion relativa al torso
}

// CABEZA
void Enderman::transformacionesCabeza() {
	if (dab) glRotatef(20, 1, 0, 0);
	glTranslatef(0, tamTorso[Y] / 2 + tamCabeza[Y] / 2, 0);	//Posicion relativa al torso
	// Rotacion para mirar arriba y abajo (centro = centroCabeza)
	// Hacerlo con centro de articulacion el cuello
	//  complicaria el calculo de la rotacion de la camara
	glRotatef(-orientacionVCabeza, 1, 0, 0);
}

// BRAZO ANTERIOR
void Enderman::transformacionesBrazoAnterior(int lado) {
	glTranslatef(lado * (tamTorso[X] / 2 + tamBrazo[X] / 2), tamTorso[Y] / 2 - tamBrazo[Y] / 2, 0); // Posicion relativa al torso

	glTranslatef(0, tamBrazo[Y] / 2, 0); // Al terminar las rotaciones deshace el movimiento
	if (agarrando) {
		glRotatef(-orientacionVCabeza - 70, 1, 0, 0);
	}
	else {
		if (dab) {
			if (lado < 0) glRotatef(-110, 0, 0, 1);
			if (lado > 0)  glRotatef(-90, 1, 0, 0);
		}
		if (lado < 0) glRotatef(-elevacionBrazo1 * 0.01 * 45, 1, 0, 0); // Sube/Baja los brazos
		if (lado > 0) glRotatef(-elevacionBrazo2 * 0.01 * 45, 1, 0, 0); // Sube/Baja los brazos
		glRotatef(lado * 5, 0, 0, 1);
	}
	glTranslatef(0, -tamBrazo[Y] / 2, 0); // Coloca la articulacion como centro de las rotaciones
}

// BRAZO POSTERIOR
void Enderman::transformacionesBrazoPosterior(int lado) {
	glTranslatef(0, -tamBrazo[Y], 0);  // Posicion relativa al torso

	glTranslatef(0, tamBrazo[Y] / 2, 0); // Al terminar las rotaciones deshace el movimiento
	if (!agarrando) {
		if (dab && lado > 0) {
			glRotatef(-90, 0, 0, 1);
			glRotatef(-20, 1, 0, 0);
		}
		if (!andando) {
			if (lado < 0) glRotatef(-elevacionBrazo1 * 0.01 * 45, 1, 0, 0); // Sube/Baja los brazos respecto al antebrazo
			if (lado > 0) glRotatef(-elevacionBrazo2 * 0.01 * 45, 1, 0, 0); // Sube/Baja los brazos respecto al antebrazo
		}
	}
	glTranslatef(0, -tamBrazo[1] / 2, 0); // Coloca la articulacion como centro de las rotaciones
}

// PIERNA ANTERIOR
void Enderman::transformacionesPiernaAnterior(int lado) {
	glTranslatef(lado * tamTorso[X] / 4, -tamTorso[Y] / 2 - tamPierna[Y] / 2, 0); // Posicion relativa al torso

	glTranslatef(0, tamPierna[Y] / 2, 0); // Al terminar las rotaciones deshace el movimiento

	glRotatef(-lado * paso, 1, 0, 0);	// ANDAR
	glRotatef(agachado * 0.01 * 90, 1, 0, 0); // AGACHARSE

	glTranslatef(0, -tamPierna[Y] / 2, 0); // Coloca la articulacion como centro de las rotaciones
}

// PIERNA POSTERIOR
void Enderman::transformacionesPiernaPosterior() {
	glTranslatef(0, -(tamPierna[Y]), 0); // Posicion relativa al torso

	glTranslatef(0, tamPierna[Y] / 2, 0); // Al terminar las rotaciones deshace el movimiento

	glRotatef(-agachado * 0.01 * 160, 1, 0, 0); // AGACHARSE

	glTranslatef(0, -tamPierna[Y] / 2, 0); // Coloca la articulacion como centro de las rotaciones
}

// MANDIBULA (solo se ve al pulsar R)
void Enderman::mandibula() {
	for (int i = 0; i < 2; i++) {	// LATERALES
		int lado = (int)pow(-1, i); // lado de la simetria en x
		glBegin(GL_POLYGON);
		glVertex3f(0.45 * lado, 0, -0.45);
		glVertex3f(0.45 * lado, 0.1, -0.45);
		glVertex3f(0.45 * lado, 0.1, 0.45);
		glVertex3f(0.45 * lado, 0, 0.45);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.45 * lado, 0.5, 0.45);
		glVertex3f(0.45 * lado, 0, 0.45);
		glVertex3f(0.45 * lado, 0, 0.35);
		glVertex3f(0.45 * lado, 0.5, 0.35);
		glEnd();
	}
	for (int i = 0; i < 2; i++) {	// FRONTAL
		int lado = (int)pow(-1, i); // lado de la simetria en x
		glBegin(GL_POLYGON);
		glVertex3f(lado * 0.45, 0, 0.45);
		glVertex3f(lado * 0.45, 0.4, 0.45);
		glVertex3f(lado * 0.35, 0.4, 0.45);
		glVertex3f(lado * 0.35, 0.1, 0.45);
		glVertex3f(0, 0.1, 0.45);
		glVertex3f(0, 0, 0.45);
		glEnd();
		glBegin(GL_POLYGON);	// Dientes
		glVertex3f(0.2, 0, 0.45);
		glVertex3f(0.2, 0.2, 0.45);
		glVertex3f(-0.2, 0.2, 0.45);
		glVertex3f(-0.2, 0, 0.45);
		glEnd();
	}
	for (int i = 0; i < 2; i++) {	// TRASERO
		int lado = (int)pow(-1, i); // lado de la simetria en x
		glBegin(GL_POLYGON);
		glVertex3f(lado * 0.45, 0, -0.45);
		glVertex3f(lado * 0.45, 0.9, -0.45);
		glVertex3f(lado * 0.35, 0.9, -0.45);
		glVertex3f(lado * 0.35, 0.2, -0.45);
		glVertex3f(lado * 0.2, 0.2, -0.45);
		glVertex3f(lado * 0.2, 0.1, -0.45);
		glVertex3f(0, 0.1, -0.45);
		glVertex3f(0, 0, -0.45);
		glEnd();
	}
}

//// GRADOS DE LIBERTAD

// MOVIMIENTO DEL BRAZO
void Enderman::elevarBrazo(double elevacion, int brazo) {
	if (brazo == 1 && (elevacion > 0 && elevacionBrazo1 < MAXIMA_ELEVACION_BRAZOS || elevacion < 0 && elevacionBrazo1 > MINIMA_ELEVACION_BRAZOS)) {	// Nos aseguramos que no se eleve fuera de un rango [MINIMO%, MAXIMO%]
		elevacionBrazo1 += elevacion;
	}
	if (brazo == 2 && (elevacion > 0 && elevacionBrazo2 < MAXIMA_ELEVACION_BRAZOS || elevacion < 0 && elevacionBrazo2 > MINIMA_ELEVACION_BRAZOS)) {
		elevacionBrazo2 += elevacion;
	}
}
bool Enderman::limiteAlcanzadoBrazo(int brazo) { // TRUE si el limite de rotacion se ha alcanzado/superado (usado para cambiar el sentido de animacion)
	if (brazo == 1 && (elevacionBrazo1 >= MAXIMA_ELEVACION_BRAZOS || elevacionBrazo1 <= MINIMA_ELEVACION_BRAZOS)) return true;
	if (brazo == 2 && (elevacionBrazo2 >= MAXIMA_ELEVACION_BRAZOS || elevacionBrazo2 <= MINIMA_ELEVACION_BRAZOS)) return true;
	else return false;
}

// AGACHARSE (un % de la altura)
void Enderman::agacharse(double altura) {
	if (altura > 0 && agachado < 0 || altura < 0 && agachado > -MAXIMO_AGACHE) // Nos aseguramos que no se agache fuera de un rango [0%, MAXIMO%] (0% = erguido)
		agachado += altura;
}
bool Enderman::limiteAlcanzadoAgache() { // TRUE si el limite de rotacion se ha alcanzado/superado (usado para cambiar el sentido de animacion)
	if (agachado <= -MAXIMO_AGACHE || agachado >= 0) return true;
	else return false;
}

// DAR UN PASO (avanzar un % del paso)
void Enderman::avanzarPaso(double distanciaPaso) {
	if (distanciaPaso > 0 && paso < MAXIMO_PASO || distanciaPaso < 0 && paso > -MAXIMO_PASO)
		paso += distanciaPaso;
}
bool Enderman::limiteAlcanzadoPaso() { // TRUE si se ha alcanzado/superado el límite de la distancia que puede hacer un paso en [MINIMO%,MAXIMO]
	if (paso >= MAXIMO_PASO || paso <= -MAXIMO_PASO) return true;
	else return false;
}

///// ANIMACIONES

// SENTADILLAS
void Enderman::hacer_sentadillas() {
	// Los brazos iran un x2.5 más rápido para coordinarlos con las piernas
	elevarBrazo(sentidoMovimiento * velocidadMovimiento * 2.5, 1);
	elevarBrazo(sentidoMovimiento * velocidadMovimiento * 2.5, 2);
	agacharse(-sentidoMovimiento * velocidadMovimiento); // Sentido negativo porque se usa un % de agache de 0% a 100%, a más % ha bajado
	if (limiteAlcanzadoAgache()) cambiar_sentido_animacion();
}

// ANDAR
void Enderman::andar() {
	// Los brazos iran un x2 más rápido para coordinarlos con las piernas
	elevarBrazo(sentidoMovimiento * velocidadMovimiento * 2, 1);
	elevarBrazo(-sentidoMovimiento * velocidadMovimiento * 2, 2); // Los brazos van alternados, uno adelante y otro atrás (sentido negativo)
	avanzarPaso(sentidoMovimiento * velocidadMovimiento);
	if (limiteAlcanzadoBrazo(1) || limiteAlcanzadoPaso()) cambiar_sentido_animacion();
}

// ENFADO (Aumenta la velocidad de animaciones, en el arbol_escena se abre la boca visualizando un nuevo componente (mandibula) y activa los temblores)
void Enderman::enfado() {
	if (!enfadado) {
		enfadado = true;
		velocidadMovimiento = velocidadBase * 3;
	}
	else {
		enfadado = false;
		velocidadMovimiento = velocidadBase;
	}
}

// AGARRAR
void Enderman::agarrar() {
	if (!agarrando) {
		agarrando = true;
		// BLOQUE QUE AGARRA PARA PROYECTO
	}
	else {
		agarrando = false;
	}
}

//// RESET
void Enderman::reset() {
	// Reseteamos todos los atributos relacionados con el movimiento a su valor por defecto
	// (Enfadado puede ser considerado movimiento por la vibracion, pero es compatible con cualquier movimiento, asi que no hace falta resetearlo)
	dab = false;
	andando = false;
	sentando = false;
	quieto = true;
	MAXIMA_ELEVACION_BRAZOS = 250;
	MINIMA_ELEVACION_BRAZOS = 0;
	MAXIMO_AGACHE = 80;
	MAXIMO_PASO = 30;
	elevacionBrazo1 = 0;
	elevacionBrazo2 = 0;
	agachado = 0;
	paso = 0;
	if (!enfadado)
		velocidadMovimiento = velocidadBase;
	sentidoMovimiento = 1;
}

// MOVIMIENTO
void Enderman::mover(GLdouble movX, GLdouble movY, GLdouble movZ) {
	posicion[X] += movX;
	posicion[Y] += movY;
	posicion[Z] += movZ;
}

//// VISUALIZAR EL MODELO (textura y ejecuta el arbol de escena)
void Enderman::visualizar() {
	texturasEnderman[1].aplicar();

	arbol_escena();
}