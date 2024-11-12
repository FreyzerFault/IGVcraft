#include "igvTextura.h"

igvTextura::igvTextura() {
}

igvTextura::igvTextura(const char* fichero) {
	if (!glIsTexture(idTextura)) {
		// Se guarda la imagen como atributo una vez se extrae la informacion del archivo
		imagen = IMG_Load(fichero);
		if (!imagen)
		{
			/* Fallo de carga de la imagen */
			printf("Error al cargar la imagen: %s\n", IMG_GetError());
			return;
		}
		/* Muestra alguna información sobre la imagen */
		printf("Cargando %s: %dx%d %dbpp\n", fichero,
			imagen->w, imagen->h, imagen->format->BitsPerPixel);

		// Generacion de ID
		glGenTextures(1, &idTextura);
	}
}

// Aplica la textura a partir del objeto SDL_Surface guardado
// Aplica parametros, environment y enlaza su id
void igvTextura::aplicar(void) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imagen->w, imagen->h, 0, GL_RGB, GL_UNSIGNED_BYTE, imagen->pixels);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, idTextura);
}

// Elimina la textura y libera el SDL_Surface
igvTextura::~igvTextura() {
	glDeleteTextures(1, &idTextura);
	SDL_FreeSurface(imagen);
}