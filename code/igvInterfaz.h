#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <iostream>
#include "igvCamara.h"

using namespace std;

class igvInterfaz {
protected:
	// Ancho y Alto inicial de la ventana de visualizacion
	int ancho_ventana;
	int alto_ventana;

	// Centro de la ventana donde estara el cursor
	int centroX, centroY;

	// Escena
	igvEscena3D escena;

	// CAMARAS
	igvCamara* cam; // Camara manejada
	igvCamara camaraLibre;		// Libertad de movimiento por toda la escena
	igvCamara camaraJugador;	// Se ancla siempre al jugador (en el centro de la cabeza del Enderman)
	igvCamara camaraCenital;	// Perspectiva CENITAL o Minimapa que sigue al jugador
	igvCamara camaraPanoramica; // Observa la Escena en vista PANORAMICA y sigue al jugador

	// BLOQUE SELECCIONADO
	Bloque* bloqueSeleccionado;
	tipoMovimiento caraSeleccionada;
	// Bloque usado en la colocacion
	tipoBloque bloqueUsado;

	// Activado al moverse para activar la animacion de andar
	bool moviendome;

	// Desactiva luces, texturas, etc para visualizar solo el bloque con sus caras de colores
	bool seleccionandoCara;

public:
	// Constructores por defecto y destructor
	igvInterfaz();
	~igvInterfaz();

	// Define Camara y mas elementos del mundo
	void crear_mundo(void);

	// INICIALIZA todos los PARÁMETROS para crear una VENTANA DE VISUALIZACIÓN
	void configura_entorno(int argc, char** argv,	// MAIN
		int _ancho_ventana, int _alto_ventana,		// ANCHO / ALTO de la ventana
		int _pos_X, int _pos_Y,						// POSICIÓN inicial de la ventana
		string _titulo								// TÍTULO de la ventana
	);

	// VISUALIZA la escena y ESPERA a EVENTOS sobre la interfaz
	void inicia_bucle_visualizacion();

	//// CALLBACKS de EVENTOS
	// TECLADO
	static void set_glutKeyboardFunc(unsigned char key, int x, int y); // Control de pulsaciones de TECLADO
	static void set_glutKeyboardUpFunc(unsigned char key, int x, int y); // Control de soltar teclas de TECLADO
	static void set_glutSpecialFunc(GLint key, GLint x, GLint y);	// Control de teclas especiales
	static void set_glutSpecialUpFunc(GLint key, GLint x, GLint y); // Control de soltar teclas especiales
	// RATON
	static void set_glutMouseFunc(GLint boton, GLint estado, GLint x, GLint y); // Control de Botones del RATON
	static void set_glutPassiveMotionFunc(GLint x, GLint y); // Control de MOVIMIENTO del CURSOR sin pulsar botones (mover camara)
	static void set_glutMotionFunc(GLint x, GLint y); // Control de MOVIMIENTO del CURSOR con un boton pulsado
	// VENTANA
	static void set_glutReshapeFunc(int w, int h); // Redefinicion de CAMARA de vision y el VIEWPORT
												   // (se llama automáticamente cuando se cambia el tamaño de la ventana)
	static void set_glutDisplayFunc(); // Visualizar la ESCENA
	// Animaciones
	static void set_glutIdleFunc();
	// INICIALIZA todos los CALLBACKS
	void inicializa_callbacks();

	// GETTERs / SETTERs
	int get_ancho_ventana() { return ancho_ventana; };
	int get_alto_ventana() { return alto_ventana; };

	void set_ancho_ventana(int _ancho_ventana) { ancho_ventana = _ancho_ventana; };
	void set_alto_ventana(int _alto_ventana) { alto_ventana = _alto_ventana; };

	// Control de CÁMARA
	void inicializarCamaras();
	tipoVista cambiarCamara();
	void moverCamara(tipoMovimiento move);
	void moverRespectoCam(igvCamara* c, tipoMovimiento move);

	// Control de JUGADOR
	void moverJugador(tipoMovimiento move);

	// SELECCION
	Bloque* seleccionar();
	void colocarBloque(tipoMovimiento cara);
};

#endif
