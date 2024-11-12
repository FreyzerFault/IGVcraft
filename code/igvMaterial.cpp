#include "igvMaterial.h"

igvMaterial::igvMaterial() {
}

igvMaterial::~igvMaterial() {
}

igvMaterial::igvMaterial(const igvMaterial& p) {	//de copia
	Ka = p.Ka;
	Kd = p.Kd;
	Ks = p.Ks;
	Ns = p.Ns;
}

igvMaterial::igvMaterial(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}

// Metodos publicos

void igvMaterial::aplicar(void) {
	// COEFICIENTES
	float* ka = Ka.cloneToFloatArray();
	float* kd = Kd.cloneToFloatArray();
	float* ks = Ks.cloneToFloatArray();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	// EXPONENTE DE PHONG
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Ns);

	// LUZ EMITIDA
	float* emisionLuz = igvColor(0, 0, 0).cloneToFloatArray();
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emisionLuz);

	// Borramos los vectores dinamicos que crea cloneToFloatArray() porque consume mucho espacio
	delete[] ka;
	delete[] kd;
	delete[] ks;
	delete[] emisionLuz;
}

void igvMaterial::set(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}