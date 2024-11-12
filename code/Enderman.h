#ifndef __ENDERMAN
#define __ENDERMAN

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "Constantes.h"
#include "Bloque.h"
#include <iostream>

// Dimensiones del CUERPO
static const double escala = 0.61;

static const igvPunto3D tamTorso = igvPunto3D(0.8 * escala, 1.5 * escala, 0.5 * escala);	// TORSO
static const igvPunto3D tamCabeza = igvPunto3D(1 * escala, 1 * escala, 1 * escala);		// CABEZA
static const igvPunto3D tamBrazo = igvPunto3D(0.2 * escala, 1.7 * escala, 0.2 * escala);	// BRAZO
static const igvPunto3D tamPierna = igvPunto3D(0.2 * escala, 1.5 * escala, 0.2 * escala);	// PIERNA

static const double velocidadBase = 3;
static const int temblor = 2; // Intensidad del temblor cuando esta enfadado

class Enderman {
private:
	// POSICION
	igvPunto3D posicion;		// Posición del modelo respecto al Origen
	double orientacionH;		// Orientacion horizontal del modelo
	double orientacionVCabeza;	// Orientacion vertical de la cabeza

	// % ELEVACIÓN de brazo
	double elevacionBrazo1;
	double elevacionBrazo2;
	int MAXIMA_ELEVACION_BRAZOS;	//Limita el movimiento a un %
	int MINIMA_ELEVACION_BRAZOS;

	// % de AGACHADO
	double agachado;
	int MAXIMO_AGACHE;  //Limita el movimiento a un %

	// % de PASO en animacion de ANDAR
	double paso;
	int MAXIMO_PASO; // Tamaño del paso

	double velocidadMovimiento;	// Limita el movimiento a el % que recorre cada movimiento
	int sentidoMovimiento;  // Dicta el sentido de las animaciones

	// Control de Animacion
	bool enfadado;
	bool dab;
	bool quieto;
	bool andando;
	bool sentando;
	bool agarrando;

	// Activado cuando la camara esta en el jugador
	bool firstPerson;

	tipoBloque bloqueUsado; // se utiliza para poner textura al bloque de la mano

public:

	// Constructores por defecto y destructor
	Enderman(double posX, double posY, double posZ, double ori);
	Enderman();
	~Enderman();

	//// Métodos para visualizar al enderman
	void visualizar(void);

	void arbol_escena();

	// CONSTRUCTORES (FORMA Y TAMAÑO):
	void cabeza();
	void torso();
	void brazo();
	void pierna();
	void mandibula();
	// MODIFICADORES (POSICION Y ROTACION):
	void transformacionesTorso();
	void transformacionesMandibula();
	void transformacionesCabeza();
	void transformacionesBrazoAnterior(int lado);
	void transformacionesBrazoPosterior(int lado);
	void transformacionesPiernaAnterior(int lado);
	void transformacionesPiernaPosterior();

	////// Métodos para modificar los GRADOS DE LIBERTAD del modelo
	void elevarBrazo(double elevacion, int brazo);
	bool limiteAlcanzadoBrazo(int brazo);
	void agacharse(double altura);
	bool limiteAlcanzadoAgache();
	void avanzarPaso(double distanciaPaso);
	bool limiteAlcanzadoPaso();

	// ANIMACIONES ESTÁTICAS
	void enfado();
	void hacer_dab() { if (dab) dab = false; else dab = true; };
	void agarrar();

	// ANIMACIONES DINÁMICAS
	void hacer_sentadillas();
	void andar();

	////// GETTERS Y SETTERS
	// CONTROL DE ANIMACION
	bool get_enfadado() { return enfadado; };
	bool get_dab() { return dab; };
	bool get_andando() { return andando; };
	bool get_sentando() { return sentando; };
	bool get_agarrando() { return agarrando; };
	bool get_quieto() { return quieto; };
	void set_andando(bool a) { andando = a; };
	void set_sentando(bool a) { sentando = a; };
	// LIMITADORES DE MOVIMIENTO
	int get_MAX_brazo() { return MAXIMA_ELEVACION_BRAZOS; }
	void set_MAX_brazo(int max) { MAXIMA_ELEVACION_BRAZOS = max; };
	int get_MIN_brazo() { return MINIMA_ELEVACION_BRAZOS; };
	void set_MIN_brazo(int min) { MINIMA_ELEVACION_BRAZOS = min; };
	int get_MAX_agache() { return MAXIMO_AGACHE; };
	void set_MAX_agache(int max) { MAXIMO_AGACHE = max; };
	// CONTROL DE VELOCIDAD DE ANIMACION
	double get_velocidad() { return velocidadMovimiento; };
	void set_velocidad(double v) { velocidadMovimiento = v; };
	void cambiar_sentido_animacion() { sentidoMovimiento = -sentidoMovimiento; };

	//Posicion
	igvPunto3D get_posicion() { return posicion; };
	void set_posicion(igvPunto3D p) { posicion = p; }
	auto get_orientacionH() { return orientacionH; };
	void set_orientacionH(double ori) { orientacionH = ori; };
	auto get_orientacionVCabeza() { return orientacionVCabeza; };
	void set_orientacionVCabeza(double ori) { if (ori < 90 && ori > -90) orientacionVCabeza = ori; };

	// Resetea parametros, posicion y animaciones
	void reset();

	// Movimiento lateral y vertical
	void mover(GLdouble movX, GLdouble movY, GLdouble movZ);

	// Activado cuando la camara se cambia al jugador
	void set_firstPerson(bool f) { firstPerson = f; };

	// Elije un bloque para agarrar
	void set_bloqueUsado(tipoBloque t) { bloqueUsado = t; };
};

#endif
