#ifndef __CONSTANTES
#define __CONSTANTES

#include "Malla.h"
#include "igvPunto3D.h"
#include "igvTextura.h"
#include "igvMaterial.h"
#include "igvFuenteLuz.h"
#include "igvColor.h"

enum tipoMovimiento {
	arriba,
	abajo,
	adelante,
	atras,
	izquierda,
	derecha,
};

static const double PI = 3.141592;

// TAMAÑO CHUNK
static const int tamCHUNK = 4;
static const int maxHeight = 64;

static const int alturaMundo = 5; //Altura en bloques a la que se generara el mundo

static int distanciaRender = 2;	// Distancia en chunks a la que se renderiza el mundo

static const int amplitudMapa = 16; // Rango de Vision del mapa

static const double alcance = 6;	// Alcance en bloques al que puede romper/colocar bloques

static const double precision = 10; // Numero de comprobaciones en seleccion = alcance * precision
// A mas precision menos probabilidad de seleccionar un bloque de detras al apuntar en diagonal en una esquina

static const double sensibilidad = 50; // [0,100]
static double factor_sensibilidad = 1000 - sensibilidad * 10; // Para calcular rotacion de camara

static double velocidadMovimiento = 0.5;

static const igvPunto3D posInicialJugador(0, alturaMundo, 0);

// Parametros de posicionamiento de camara del jugador y sincronizacion con el modelo 3D
static double alturaOjos;
static double distanciaOjos; // Distancia Ojos - CentroCabeza para el plano Near de camara
static double orientacionInicial = PI / 2; // Orientacion del jugador al inicio (radianes)

// Contiene los arrays con índices a vértices para la visualización del bloque y las coordenadas de textura de una cara
static Malla* malla = new Malla();

// Colores para cada cara de un Bloque, que permite diferenciarlas en la Selección
static GLfloat colorCara[6][3] = {
	{ 1,0,0 },
	{ 0,1,0 },
	{ 0,0,1 },
	{ 1,1,0 },
	{ 0,1,1 },
	{ 1,0,1 }
};

static const int numTexturasBloques = 11; // Nº de bloques distintos

// Texturas cargadas antes de empezar
static igvTextura texturasBloques[numTexturasBloques] = {
	igvTextura("textures\\dirt.jpg"),
	igvTextura("textures\\stone.jpg"),
	igvTextura("textures\\cobblestone.jpg"),
	igvTextura("textures\\leaves.jpg"),
	igvTextura("textures\\wood.jpg"),
	igvTextura("textures\\bedrock.jpg"),
	igvTextura("textures\\glowstone.jpg"),
	igvTextura("textures\\grassUp.jpg"),
	igvTextura("textures\\grassSide.jpg"),
	igvTextura("textures\\logUp.jpg"),
	igvTextura("textures\\logSide.jpg"),
};

static igvTextura texturasEnderman[2] = {
	igvTextura("textures\\cara.jpg"),
	igvTextura("textures\\enderman.jpg"),
};

static igvMaterial materialBasico(igvColor(0.5, 0.5, 0.5), igvColor(1, 1, 1), igvColor(1, 1, 1), 120);

static igvFuenteLuz sol = igvFuenteLuz(GL_LIGHT1, igvPunto3D(100, 100, 100),
	igvColor(0.1, 0.1, 0.1, 1), igvColor(1, 1, 1, 1), igvColor(1, 1, 1, 1),
	1, 0, 0,
	igvPunto3D(-1, -1, -1), 40, 0);

// METODOS GLOBALES
// Cubo con texturas
static void drawBox(GLfloat size, GLenum type) {
	static GLfloat n[6][3] =
	{
	  {-1.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0},
	  {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0},
	  {0.0, 0.0, 1.0},
	  {0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
	  {0, 1, 2, 3},
	  {3, 2, 6, 7},
	  {7, 6, 5, 4},
	  {4, 5, 1, 0},
	  {5, 6, 2, 1},
	  {7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord2i(0, 0);
		glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2i(1, 0);
		glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2i(1, 1);
		glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2i(0, 1);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
}
// Cuadrado con texturas
static void drawFace() {
	static GLfloat n[4][3] =
	{
	  {-1,-1, 1},
	  { 1,-1, 1},
	  { 1, 1, 1},
	  {-1, 1, 1},
	};
	static GLint face[4] =
	{
	  0, 1, 2, 3,
	};
	GLfloat v[4][3] = {
		{-0.5, -0.5, 0.501},
		{0.5, -0.5, 0.501},
		{0.5, 0.5, 0.501},
		{-0.5, 0.5, 0.501}
	};

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glNormal3fv(n[0]);
	glVertex3fv(v[0]);

	glTexCoord2i(1, 0);
	glNormal3fv(n[1]);
	glVertex3fv(v[1]);

	glTexCoord2i(1, 1);
	glNormal3fv(n[2]);
	glVertex3fv(v[2]);

	glTexCoord2i(0, 1);
	glNormal3fv(n[3]);
	glVertex3fv(v[3]);
	glEnd();
}

static void drawSolidCube(GLdouble size) {
	drawBox(size, GL_QUADS);
}

#endif
