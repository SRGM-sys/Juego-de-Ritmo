#include "GestorEditor.h"
#include "GestorAudio.h"

void actualizarEditor(EstadoJuego& estado) {

	// Control de reproducción (PLAY / PAUSA)
	// Usamos la tecla ESCPACIO para intercalar
	if (IsKeyPressed(KEY_SPACE)) {
		if (estado.reproduciendoEditor) {
			estado.reproduciendoEditor = false;
			PauseMusicStream(estado.musicaNivel1);
		}
		else {
			estado.reproduciendoEditor = true;
			ResumeMusicStream(estado.musicaNivel1);
		}
	}

	// Actualizar el reloj del editor
	if (estado.reproduciendoEditor) {
		estado.tiempoEditor = GetMusicTimePlayed(estado.musicaNivel1);
	}

	// Colocar notas con el ratón
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 raton = GetMousePosition();

		// Verificamos si el click fue dentro de la zona de juego (los carriles)
		if (raton.x >= 0 && raton.x < estado.gameWidth) {

			// Si dividimos la posición X del raton entre el ancho del carril, nos da el numero de carril
			int carrilClic = (int)raton.x / estado.laneWidth;

			// Creamos una nueva nota temporal
			NotaDatos nuevaNota;
			nuevaNota.tiempo = estado.tiempoEditor; // Se guarda en el segundo exacto actual
			nuevaNota.carril = carrilClic;			// Se guarda en el carril donde hicimos clic

			estado.notasGuardadas.push_back(nuevaNota);
		}

	}
}