#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

using namespace std;

// Para acceder a los atributos y metodos desde metodos estaticos
extern igvInterfaz interfaz;

igvInterfaz::igvInterfaz() {
	inicializarCamaras();

	// Camara Inicial:
	cam = &camaraJugador;
	// Al iniciar desde camara jugador activamos la 1º persona
	escena.get_enderman()->agarrar();
	escena.get_enderman()->set_firstPerson(true);

	bloqueSeleccionado = NULL;
	caraSeleccionada = arriba;
	bloqueUsado = Wood;

	moviendome = false;
	seleccionandoCara = false;
}

igvInterfaz::~igvInterfaz() {}

void igvInterfaz::crear_mundo(void) {
	interfaz.camaraLibre.set_perspectiva(60, 1.5, 0.1, 100);
	interfaz.camaraJugador.set_perspectiva(60, 1.5, distanciaOjos + 0.1, 100);
	interfaz.camaraPanoramica.set_perspectiva(90, 1.5, 0.1, 100);
	interfaz.camaraCenital.set_paralela(
		-1 * amplitudMapa, 1 * amplitudMapa, -1 * amplitudMapa, 1 * amplitudMapa, 1, 100);
}

void igvInterfaz::configura_entorno(int argc, char** argv,
	int _ancho_ventana, int _alto_ventana,
	int _pos_X, int _pos_Y,
	string _titulo) {
	// inicialización de las variables de la interfaz
	ancho_ventana = _ancho_ventana;
	alto_ventana = _alto_ventana;

	centroX = ancho_ventana / 2;
	centroY = alto_ventana / 2;

	// inicialización de la ventana de visualización
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_ancho_ventana, _alto_ventana);
	glutInitWindowPosition(_pos_X, _pos_Y);
	glutCreateWindow(_titulo.c_str());

	glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
	glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

	glEnable(GL_LIGHTING);	// Iluminacion
	glEnable(GL_NORMALIZE); // Normaliza los vectores normales para calculo iluminacion
	glEnable(GL_TEXTURE_2D);// Texturas
	glEnable(GL_CULL_FACE);// Culling (ocultamiento de objetos no visibles y caras traseras)

	// Posicion inicial del cursor
	glutWarpPointer(ancho_ventana / 2, alto_ventana / 2);

	crear_mundo(); // Crea el mundo a visualizar en la ventana
}

void igvInterfaz::inicia_bucle_visualizacion() {
	glutMainLoop(); // Inicia el bucle de visualizacion de OpenGL
}

void igvInterfaz::set_glutKeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
		// Cambio de Camara
	case 'c':
		interfaz.cambiarCamara();
		cout << "Camara " << interfaz.cam->print_vista() << " activada" << endl;
		break;

		// =============================================== MOVIMIENTO ===============================================
	case 'w':
		interfaz.moverCamara(adelante);
		interfaz.moviendome = true;
		break;
	case 'a':
		interfaz.moverCamara(izquierda);
		interfaz.moviendome = true;
		break;
	case 's':
		interfaz.moverCamara(atras);
		interfaz.moviendome = true;
		break;
	case 'd':
		interfaz.moverCamara(derecha);
		interfaz.moviendome = true;
		break;
	case ' ':
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			interfaz.moverCamara(abajo);
		}
		else {
			interfaz.moverCamara(arriba);
		}
		break;

		// =============================================== ANIMACIONES ===============================================
		// Levanta Brazo Derecho
	case ',':
		interfaz.escena.get_enderman()->elevarBrazo(10, 1);		// IZQUIERDO SUBE
		break;
		// Baja Brazo Derecho
	case ';':
		interfaz.escena.get_enderman()->elevarBrazo(-10, 1);	// IZQUIERDO BAJA
		break;
		// Levanta Brazo Izquierdo
	case '.':
		interfaz.escena.get_enderman()->elevarBrazo(10, 2);		// DERECHO SUBE
		break;
		// Baja Brazo Izquierdo
	case ':':
		interfaz.escena.get_enderman()->elevarBrazo(-10, 2);	// DERECHO BAJA
		break;

		// ANDAR
	case '1':
		interfaz.escena.get_enderman()->reset();
		interfaz.escena.get_enderman()->set_MAX_brazo(100);
		interfaz.escena.get_enderman()->set_MIN_brazo(-100);
		interfaz.escena.get_enderman()->set_andando(true);
		break;
		// SENTADILLAS
	case '2':
		interfaz.escena.get_enderman()->reset();
		interfaz.escena.get_enderman()->set_sentando(true);
		break;
		// HACER UN DAB
	case '3':
		interfaz.escena.get_enderman()->reset();
		interfaz.escena.get_enderman()->hacer_dab();
		break;

		// Modo ENFADADO
	case 'r':
		interfaz.escena.get_enderman()->enfado();
		break;

	case 'e': // activa/desactiva la visualizacion de los ejes
		interfaz.escena.set_ejes(interfaz.escena.get_ejes() ? false : true);
		break;
	case 27: // tecla de escape para SALIR
		exit(1);
		break;
	}
	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

// Activa eventos al soltar teclas
void igvInterfaz::set_glutKeyboardUpFunc(unsigned char key, int x, int y) {
	switch (key) {
		// =============================================== MOVIMIENTO ===============================================
	case 'w':
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case 'a':
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case 's':
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case 'd':
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	}
	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

// Control de las Flechas del Teclado
void igvInterfaz::set_glutSpecialFunc(GLint key, GLint x, GLint y) {
	switch (key) {
	case GLUT_KEY_UP:
		interfaz.moverCamara(adelante);
		interfaz.moviendome = true;
		break;
	case GLUT_KEY_DOWN:
		interfaz.moverCamara(atras);
		interfaz.moviendome = true;
		break;
	case GLUT_KEY_LEFT:
		interfaz.moverCamara(izquierda);
		interfaz.moviendome = true;
		break;
	case GLUT_KEY_RIGHT:
		interfaz.moverCamara(derecha);
		interfaz.moviendome = true;
		break;
	}
	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

// Activa eventos al soltar las flechas
void igvInterfaz::set_glutSpecialUpFunc(GLint key, GLint x, GLint y) {
	switch (key) {
	case GLUT_KEY_UP:
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case GLUT_KEY_DOWN:
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case GLUT_KEY_LEFT:
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	case GLUT_KEY_RIGHT:
		interfaz.escena.get_enderman()->set_andando(false);
		interfaz.escena.get_enderman()->reset();
		interfaz.moviendome = false;
		break;
	}
	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

// =============================================== MOUSE ===============================================

// Eventos al clickar con botones y hacer scroll con rueda
void igvInterfaz::set_glutMouseFunc(GLint boton, GLint estado, GLint x, GLint y) {
	if (boton == GLUT_LEFT_BUTTON) {
		if (estado == GLUT_DOWN) { // AL PULSAR
			interfaz.bloqueSeleccionado = interfaz.seleccionar(); // Devuelve NULL si no lo encuentra

			if (interfaz.bloqueSeleccionado != NULL) {
				Chunk* chunk = interfaz.escena.get_chunk(interfaz.bloqueSeleccionado->get_coords());
				chunk->deleteBloque(chunk->convertirCoordRelativa(interfaz.bloqueSeleccionado->get_coords()));
			}

			interfaz.bloqueSeleccionado = NULL;
			// Como esta eliminado no hace falta deseleccionarlo poniendo su seleccionado a false
		}
		else { // AL SOLTAR
		}
	}
	if (boton == GLUT_RIGHT_BUTTON) {
		if (estado == GLUT_DOWN) { // AL PULSAR
			interfaz.bloqueSeleccionado = interfaz.seleccionar();
			if (interfaz.bloqueSeleccionado != NULL) {
				interfaz.seleccionandoCara = true;

				glutPostRedisplay();
			}
		}
		else { // AL SOLTAR
		}
	}
	if (boton == 3 && estado == GLUT_DOWN) {			// RUEDA ADELANTE
		// Para texturas complejas salta de 2 en 2
		if (interfaz.bloqueUsado == Grass || interfaz.bloqueUsado == Log)
			interfaz.bloqueUsado = (tipoBloque)((interfaz.bloqueUsado + 2) % numTexturasBloques);
		else
			interfaz.bloqueUsado = (tipoBloque)(interfaz.bloqueUsado + 1);
		// Cambia la textura del bloque que agarra el jugador
		interfaz.escena.get_enderman()->set_bloqueUsado(interfaz.bloqueUsado);
	}
	if (boton == 4 && estado == GLUT_DOWN) {			// RUEDA ATRAS
		// Para texturas complejas salta de 2 en 2
		if (interfaz.bloqueUsado == Log)
			interfaz.bloqueUsado = (tipoBloque)(interfaz.bloqueUsado - 2);
		else
			if (interfaz.bloqueUsado == 0)
				interfaz.bloqueUsado = (tipoBloque)(numTexturasBloques - 2);
			else
				interfaz.bloqueUsado = (tipoBloque)(interfaz.bloqueUsado - 1);
		// Cambia la textura del bloque que agarra el jugador
		interfaz.escena.get_enderman()->set_bloqueUsado(interfaz.bloqueUsado);
	}
	glutPostRedisplay();
}

// MOVER CURSOR sin pulsar botones
void igvInterfaz::set_glutPassiveMotionFunc(GLint x, GLint y) {
	if (interfaz.cam != &interfaz.camaraCenital) {
		// Movemos la camara una distancia proporcional a la sensibilidad (Constantes.h)
		double movimientoX = (interfaz.centroX - x) / factor_sensibilidad;
		double movimientoY = (interfaz.centroY - y) / factor_sensibilidad;

		if (movimientoX != 0 || movimientoY != 0) {
			interfaz.cam->rotar(
				-movimientoX,
				movimientoY);
			if (interfaz.cam->get_vista() != LIBRE) {
				// Si no es la LIBRE estara enlazada al jugador
				interfaz.escena.set_yaw(interfaz.escena.get_yaw() - movimientoX);
				interfaz.escena.get_enderman()->set_orientacionH(
					movimientoX * 180 / PI + interfaz.escena.get_enderman()->get_orientacionH());
				interfaz.escena.get_enderman()->set_orientacionVCabeza(
					movimientoY * 180 / PI + interfaz.escena.get_enderman()->get_orientacionVCabeza());
			}
		}

		// Devolvemos el cursor al centro
		glutWarpPointer(interfaz.ancho_ventana / 2, interfaz.alto_ventana / 2);
		glutPostRedisplay();
	}
}

// MOVER CURSOR con un boton pulsado
void igvInterfaz::set_glutMotionFunc(GLint x, GLint y) {
	glutPostRedisplay();
}

// Reescalado de la ventana
void igvInterfaz::set_glutReshapeFunc(int w, int h) {
	// Dimensiona el viewport al nuevo ancho y alto de la ventana
	// Guardamos valores nuevos de la ventana de visualizacion
	interfaz.set_ancho_ventana(w);
	interfaz.set_alto_ventana(h);

	// Establece los parámetros de la cámara y de la proyección
	interfaz.cam->aplicar();
}

// Visualizacion de la ventana
void igvInterfaz::set_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer

	// VIEWPORT PRINCIPAL
	glViewport(0, 0, interfaz.get_ancho_ventana(), interfaz.get_alto_ventana());

	// Si estamos seleccionando cara
	if (interfaz.seleccionandoCara) {
		// Desactivamos luces y texturas y vaciamos los buffer
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		interfaz.cam->aplicar();
		// Visualizamos solo el bloque seleccionado sin texturas, cara a cara con distinto color
		glPushMatrix();
		interfaz.bloqueSeleccionado->visualizar(arriba);
		interfaz.bloqueSeleccionado->visualizar(abajo);
		interfaz.bloqueSeleccionado->visualizar(adelante);
		interfaz.bloqueSeleccionado->visualizar(atras);
		interfaz.bloqueSeleccionado->visualizar(izquierda);
		interfaz.bloqueSeleccionado->visualizar(derecha);
		glPopMatrix();

		// Comprobamos el color del pixel en el cursor
		unsigned char pixel[3];
		glReadPixels(interfaz.ancho_ventana / 2, interfaz.alto_ventana / 2, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		// Comprobamos que cara corresponde con el color
		for (int i = 0; i < 6; i++)
		{
			GLubyte colorbyte[3] = {
				(GLubyte)(colorCara[i][0] * 255),
				(GLubyte)(colorCara[i][1] * 255),
				(GLubyte)(colorCara[i][2] * 255)
			};

			if (colorbyte[0] == pixel[0] && colorbyte[1] == pixel[1] && colorbyte[2] == pixel[2]) {
				// El indice i esta relacionado con el lado de la cara
				interfaz.caraSeleccionada = (tipoMovimiento)i;
				break;
			}
			if (i == 5) std::cout << "Ninguna cara seleccionada!!!!" << std::endl;
		}
		// Vuelvo a habilitar luces y texturas
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		// Deselecciono el bloque y la cara
		interfaz.seleccionandoCara = false;
		interfaz.bloqueSeleccionado->set_seleccionado(false);

		// Coloco el bloque adyacente a la cara seleccionada
		interfaz.colocarBloque(interfaz.caraSeleccionada);
		interfaz.bloqueSeleccionado = NULL;

		glutPostRedisplay();
	}
	else {
		// CAM PRINCIPAL
		interfaz.cam->aplicar();
		interfaz.escena.visualizar();

		// MINIMAPA
		glViewport(interfaz.get_ancho_ventana() - 200, interfaz.get_alto_ventana() - 200, 200, 200);
		interfaz.camaraCenital.aplicar();
		interfaz.escena.visualizar();

		// Refresca la ventana
		glutSwapBuffers();
	}
}

// Animaciones
void igvInterfaz::set_glutIdleFunc() {
	if (interfaz.escena.get_enderman()->get_sentando()) {
		interfaz.escena.get_enderman()->hacer_sentadillas();
	}
	if (interfaz.moviendome) {
		if (interfaz.cam->get_vista() != LIBRE)
			interfaz.escena.get_enderman()->andar();
	}

	glutPostRedisplay();
}

// Establece los Callbacks de OpenGL
void igvInterfaz::inicializa_callbacks() {
	glutKeyboardFunc(set_glutKeyboardFunc);
	glutKeyboardUpFunc(set_glutKeyboardUpFunc);
	glutSpecialFunc(set_glutSpecialFunc);
	glutSpecialUpFunc(set_glutSpecialUpFunc);
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);

	glutMouseFunc(set_glutMouseFunc);
	glutPassiveMotionFunc(set_glutPassiveMotionFunc);
	glutMotionFunc(set_glutMotionFunc);
	glutIdleFunc(set_glutIdleFunc);
}

// METODOS PARA MODIFICAR CAMARAS:

// INICIALIZA CAMARAS
void igvInterfaz::inicializarCamaras() {
	// CAM del JUGADOR
	alturaOjos = tamPierna[Y] * 2 + tamTorso[Y] + tamCabeza[Y] / 2;
	distanciaOjos = tamCabeza[Z] / 2 + 0.00001;
	camaraJugador = igvCamara(
		IGV_PERSPECTIVA, JUGADOR,
		igvPunto3D(
			posInicialJugador[X],
			posInicialJugador[Y] + alturaOjos,
			posInicialJugador[Z]),
		igvPunto3D(
			posInicialJugador[X] + cos(orientacionInicial),
			posInicialJugador[Y] + alturaOjos,
			posInicialJugador[Z] + sin(orientacionInicial)),
		igvPunto3D(0, 1, 0));

	// CAM LIBRE
	camaraLibre = igvCamara(
		IGV_PERSPECTIVA, LIBRE,
		igvPunto3D(5, alturaMundo + 2, 0),
		igvPunto3D(4, alturaMundo + 2, 0),
		igvPunto3D(0, 1, 0));

	// CAM PANORAMICA
	camaraPanoramica = camaraJugador;
	camaraPanoramica.mover(10, 5, 10);
	camaraPanoramica.rotar(PI * 3 / 4, -0.5);
	camaraPanoramica.set_vista(PANORAMICA);

	//CAM CENITAL (MINIMAPA)
	camaraCenital = igvCamara(
		IGV_PARALELA, CENITAL,
		igvPunto3D(posInicialJugador[X] - 0.1, posInicialJugador[Y] + 20, posInicialJugador[Z]),
		posInicialJugador,
		igvPunto3D(0, 1, 0));
}

// ALTERNA ENTRE CAMARAS
tipoVista igvInterfaz::cambiarCamara() {
	bool cambiada = false;
	if (cam == &camaraLibre && !cambiada) {
		cam = &camaraJugador;
		cambiada = true;
	}
	if (cam == &camaraJugador && !cambiada) {
		cam = &camaraPanoramica;
		cambiada = true;
	}
	if (cam == &camaraPanoramica && !cambiada) {
		cam = &camaraLibre;
		cambiada = true;
	}
	// Si activa la camara del jugador activamos la primera persona con el bloque seleccionado agarrado
	if (cam->get_vista() == JUGADOR) {
		escena.get_enderman()->agarrar();
		escena.get_enderman()->set_firstPerson(true);
	}
	else {
		if (escena.get_enderman()->get_agarrando())
			escena.get_enderman()->agarrar();
		escena.get_enderman()->set_firstPerson(false);
	}
	cam->aplicar();
	// Devuelve el tipo de vista que se activo
	return cam->get_vista();
}

// Mueve la camara hacia una dirección
void igvInterfaz::moverCamara(tipoMovimiento move) {
	if (!moviendome) { // Si no se estaba moviendo anteriormente activamos la animacion de andar
		interfaz.escena.get_enderman()->reset();
		interfaz.escena.get_enderman()->set_MAX_brazo(100);
		interfaz.escena.get_enderman()->set_MIN_brazo(-100);
		interfaz.escena.get_enderman()->set_andando(true);
	}

	switch (move) {
	case arriba:
		cam->mover(0, velocidadMovimiento, 0);
		break;
	case abajo:
		cam->mover(0, -velocidadMovimiento, 0);
		break;
	case izquierda:					// <--
		cam->mover(
			cos(cam->get_yaw() - PI / 2) * velocidadMovimiento,
			0,
			sin(cam->get_yaw() - PI / 2) * velocidadMovimiento);
		break;
	case derecha:					// -->
		cam->mover(
			cos(cam->get_yaw() + PI / 2) * velocidadMovimiento,
			0,
			sin(cam->get_yaw() + PI / 2) * velocidadMovimiento);
		break;
	case adelante:					//  ^
		cam->mover(
			cos(cam->get_yaw()) * velocidadMovimiento,
			0,
			sin(cam->get_yaw()) * velocidadMovimiento);
		break;
	case atras:						//  v
		cam->mover(
			-cos(cam->get_yaw()) * velocidadMovimiento,
			0,
			-sin(cam->get_yaw()) * velocidadMovimiento);
	}

	// Movemos las camaras enlazadas al jugador conforme se movio la actual
	if (cam == &camaraJugador) {
		moverJugador(move);
		moverRespectoCam(&camaraCenital, move);
		moverRespectoCam(&camaraPanoramica, move);
	}

	if (cam == &camaraCenital) {
		moverJugador(move);
		moverRespectoCam(&camaraJugador, move);
		moverRespectoCam(&camaraPanoramica, move);
	}
	if (cam == &camaraPanoramica) {
		moverJugador(move);
		moverRespectoCam(&camaraJugador, move);
		moverRespectoCam(&camaraCenital, move);
	}

	cam->aplicar();
}

// Mover una camara respecto a la camara actual
// Funciona igual que el anterior metodo pero aplica los movimientos a otra camara
void igvInterfaz::moverRespectoCam(igvCamara* c, tipoMovimiento move) {
	switch (move) {
	case arriba:
		c->mover(0, velocidadMovimiento, 0);
		break;
	case abajo:
		c->mover(0, -velocidadMovimiento, 0);
		break;
	case izquierda:					// <--
		c->mover(
			cos(cam->get_yaw() - PI / 2) * velocidadMovimiento,
			0,
			sin(cam->get_yaw() - PI / 2) * velocidadMovimiento);
		break;
	case derecha:					// -->
		c->mover(
			cos(cam->get_yaw() + PI / 2) * velocidadMovimiento,
			0,
			sin(cam->get_yaw() + PI / 2) * velocidadMovimiento);
		break;
	case adelante:					//  ^
		c->mover(
			cos(cam->get_yaw()) * velocidadMovimiento,
			0,
			sin(cam->get_yaw()) * velocidadMovimiento);
		break;
	case atras:						//  v
		c->mover(
			-cos(cam->get_yaw()) * velocidadMovimiento,
			0,
			-sin(cam->get_yaw()) * velocidadMovimiento);
		break;
	}
}

// MOVIMIENTO DE JUGADOR
void igvInterfaz::moverJugador(tipoMovimiento move) {
	// Mueve el jugador segun su orientacion y su posicion
	switch (move) {
	case arriba:
		interfaz.escena.get_enderman()->mover(0, velocidadMovimiento, 0);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X], escena.get_posJ()[Y] + velocidadMovimiento, escena.get_posJ()[Z]));
		break;
		break;
	case abajo:
		interfaz.escena.get_enderman()->mover(0, -velocidadMovimiento, 0);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X], escena.get_posJ()[Y] - velocidadMovimiento, escena.get_posJ()[Z]));
		break;
	case izquierda:
		interfaz.escena.get_enderman()->mover(
			cos(interfaz.cam->get_yaw() - PI / 2) * velocidadMovimiento,
			0,
			sin(interfaz.cam->get_yaw() - PI / 2) * velocidadMovimiento);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X] + cos(interfaz.cam->get_yaw() - PI / 2) * velocidadMovimiento,
			escena.get_posJ()[Y],
			escena.get_posJ()[Z] + sin(interfaz.cam->get_yaw() - PI / 2) * velocidadMovimiento));
		break;
	case derecha:
		interfaz.escena.get_enderman()->mover(
			cos(interfaz.cam->get_yaw() + PI / 2) * velocidadMovimiento,
			0,
			sin(interfaz.cam->get_yaw() + PI / 2) * velocidadMovimiento);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X] + cos(interfaz.cam->get_yaw() + PI / 2) * velocidadMovimiento,
			escena.get_posJ()[Y],
			escena.get_posJ()[Z] + sin(interfaz.cam->get_yaw() + PI / 2) * velocidadMovimiento));
		break;
	case adelante:
		interfaz.escena.get_enderman()->mover(
			cos(interfaz.cam->get_yaw()) * velocidadMovimiento,
			0,
			sin(interfaz.cam->get_yaw()) * velocidadMovimiento);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X] + cos(interfaz.cam->get_yaw()) * velocidadMovimiento,
			escena.get_posJ()[Y],
			escena.get_posJ()[Z] + sin(interfaz.cam->get_yaw()) * velocidadMovimiento));
		break;
	case atras:
		interfaz.escena.get_enderman()->mover(
			-cos(interfaz.cam->get_yaw()) * velocidadMovimiento,
			0,
			-sin(interfaz.cam->get_yaw()) * velocidadMovimiento);
		escena.set_posJ(igvPunto3D(
			escena.get_posJ()[X] - cos(interfaz.cam->get_yaw()) * velocidadMovimiento,
			escena.get_posJ()[Y],
			escena.get_posJ()[Z] - sin(interfaz.cam->get_yaw()) * velocidadMovimiento));
		break;
	}
}

// Seleccion de un Bloque
Bloque* igvInterfaz::seleccionar() {
	// Punto detector que simula un rayo en la direccion adonde mira la camara
	igvPunto3D detector = cam->get_pos();
	// Chunk desde donde sacamos los bloques
	Chunk* chunkDeteccion = escena.get_chunk(detector); // Devuelve NULL si no hay ningun chunk establecido

	for (int i = 0; i < alcance * precision; i++)
	{
		// Avanza una distancia de (1 / precision) en la direccion donde mira
		detector.avanzar(cam->get_yaw(), cam->get_pitch(), 1 / precision);

		// Recalcula el chunk donde se esta detectando bloques por si ha cambiado
		chunkDeteccion = escena.get_chunk(detector);

		if (chunkDeteccion != NULL) {
			// Busca en el chunk el bloque que coincida con la posicion
			if (chunkDeteccion->existe_bloque(chunkDeteccion->convertirCoordRelativa(detector))) {
				Bloque* bloque = chunkDeteccion->get_bloque(chunkDeteccion->convertirCoordRelativa(detector));
				// Cuando es seleccionado se dibujan sus caras con colores distintos para distinguir la cara seleccionada
				bloque->set_seleccionado(true);
				return bloque;
			}
		}
	}
	// Si no ha encontrado ningun bloque devuelve NULL
	return NULL;
}

// Colocacion de un Bloque adyacente a la cara seleccionada
void igvInterfaz::colocarBloque(tipoMovimiento cara) {
	// Chunk del bloque seleccionado
	Chunk* chunk = interfaz.escena.get_chunk(interfaz.bloqueSeleccionado->get_coords());

	switch (cara) {
	case arriba:
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X],
			interfaz.bloqueSeleccionado->get_coords()[Y] + 1,
			interfaz.bloqueSeleccionado->get_coords()[Z]),
			bloqueUsado));
		break;
	case abajo:
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X],
			interfaz.bloqueSeleccionado->get_coords()[Y] - 1,
			interfaz.bloqueSeleccionado->get_coords()[Z]),
			bloqueUsado));
		break;
	case adelante:
		// Si el bloque a colocar debe estar en otro chunk cogemos el chunk correspondiente
		if (chunk->convertirCoordRelativa(bloqueSeleccionado->get_coords())[X] == 0)
			chunk = chunk->getVecino(cara);
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X] - 1,
			interfaz.bloqueSeleccionado->get_coords()[Y],
			interfaz.bloqueSeleccionado->get_coords()[Z]),
			bloqueUsado));
		break;
	case atras:
		if (chunk->convertirCoordRelativa(bloqueSeleccionado->get_coords())[X] == tamCHUNK - 1)
			chunk = chunk->getVecino(cara);
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X] + 1,
			interfaz.bloqueSeleccionado->get_coords()[Y],
			interfaz.bloqueSeleccionado->get_coords()[Z]),
			bloqueUsado));
		break;
	case izquierda:
		if (chunk->convertirCoordRelativa(bloqueSeleccionado->get_coords())[Z] == 0)
			chunk = chunk->getVecino(cara);
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X],
			interfaz.bloqueSeleccionado->get_coords()[Y],
			interfaz.bloqueSeleccionado->get_coords()[Z] - 1),
			bloqueUsado));
		break;
	case derecha:
		if (chunk->convertirCoordRelativa(bloqueSeleccionado->get_coords())[Z] == tamCHUNK - 1)
			chunk = chunk->getVecino(cara);
		chunk->addBloque(Bloque(igvPunto3D(
			interfaz.bloqueSeleccionado->get_coords()[X],
			interfaz.bloqueSeleccionado->get_coords()[Y],
			interfaz.bloqueSeleccionado->get_coords()[Z] + 1),
			bloqueUsado));
		break;
	}
}