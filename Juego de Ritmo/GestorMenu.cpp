#include "GestorMenu.h"
#include "GestorAudio.h"

// Función auxiliar para dibujar botones interactivos
bool checkClick(Rectangle limites) {

	Vector2 raton = GetMousePosition();
	bool colision = CheckCollisionPointRec(raton, limites);

	// Si hay colision y se hace click izquierdo, devolvemos true
	if (colision && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		return true;
	}
	return false;
}

void actualizarMenuPrincipal(EstadoJuego& estado) {
	int anchoBoton = 200;
	int altoBoton = 50;
	int centroX = estado.gameWidth / 2 - anchoBoton / 2;

	// Botones
	Rectangle botonPlay = { (float)centroX, 250, (float)anchoBoton, (float)altoBoton };
	Rectangle botonCustom = { (float)centroX, 320, (float)anchoBoton, (float)altoBoton };
	Rectangle botonExit = { (float)centroX, 390, (float)anchoBoton, (float)altoBoton };
	Rectangle botonSettings = { (float)estado.gameWidth - 160, 20, 140, 40 };

	if (checkClick(botonPlay)) {
		PlaySound(estado.sonidoBoton);
		estado.pantalla = SELECCION_NIVEL;
	}
	if (checkClick(botonCustom)) {
		estado.pantalla = EDITOR_NIVELES;
		StopMusicStream(estado.musicaMenu);
	}
	if (checkClick(botonExit)) {
		estado.pantalla = CERRAR_JUEGO;
	}
	if (checkClick(botonSettings)) {
		estado.pantalla = CONFIGURACION;
	}
}

void actualizarSeleccionNivel(EstadoJuego& estado) {
	Rectangle botonNivel1 = { 50, 150, 300, 50 };
	Rectangle botonNivel2 = { 50, 220, 300, 50 };
	Rectangle botonVolver = { 50, 400, 150, 40 };

	if (checkClick(botonNivel1)) {
		PlaySound(estado.sonidoBoton);
		estado.noteSpeed = 6.0f;
		estado.pantalla = JUGANDO;
		cambiarMusica(estado, CANCION_NIVEL1);
	}

	if (checkClick(botonNivel2)) {
		PlaySound(estado.sonidoBoton);
		estado.noteSpeed = 10.0f;
		estado.pantalla = JUGANDO;
	}

	if (checkClick(botonVolver)) {
		PlaySound(estado.sonidoBoton);
		estado.pantalla = MENU_PRINCIPAL;
	}
}

