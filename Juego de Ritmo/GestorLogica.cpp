#include "GestorLogica.h"


void inicializarJuego(EstadoJuego& estado) {
	InitWindow(800, 600, "Juego de Ritmo");

	// Configuración Responsive
	int monitor = GetCurrentMonitor();
	estado.screenWidth = GetMonitorWidth(monitor);
	estado.screenHeight = GetMonitorHeight(monitor);
	estado.gameWidth = estado.screenWidth / 2;
	estado.gameHeight = estado.screenHeight;

	SetWindowSize(estado.gameWidth, estado.gameHeight);
	SetWindowPosition(estado.gameWidth, 0);
	SetTargetFPS(60);

	// Calculos iniciales
	estado.laneWidth = estado.gameWidth / estado.numLanes;
	estado.buttonY = estado.gameHeight - 100;
}

void verificarColisiones(EstadoJuego& estado) {
	// Obtenemos el rectangulo de intersección
	Rectangle interseccion = GetCollisionRec(estado.barraJugador, estado.notaActual);

	// Verificamos que si haya un choque y que cubra el 80% de la nota

	if (interseccion.height > 0 && interseccion.width >= (estado.notaActual.width * 0.7f)) {
		estado.puntaje++;

		// Generamos el efecto de partículas
		for (int i = 0; i < 15; i++) {
			Particula p;
			// Nacen dentro de la nota
			p.x = estado.notaActual.x + (estado.notaActual.width / 2);
			p.y = estado.notaActual.y + (estado.notaActual.height / 2);

			// Velocidad aleatoria dispersa en todas direcciones
			p.vx = GetRandomValue(-50, 50) / 10.0f;
			p.vy = GetRandomValue(-50, 50) / 10.0f;
			p.vida = 1.0f;
			estado.particulas.push_back(p);
		}

		estado.noteY = -50.0;
		estado.noteLane = GetRandomValue(0, estado.numLanes - 1);
	}
}

void actualizarJuego(EstadoJuego& estado, int porcentajeX) {
	estado.noteY += estado.noteSpeed;

	// Si la nota sale de la pantalla (El jugador falló)
	if (estado.noteY > estado.gameHeight) {
		estado.noteY = -50.0;
		estado.noteLane = GetRandomValue(0, estado.numLanes - 1); // Raylib: Genera un carril aleatorio
	}

	// Actualizar Rectángulos para colisión
	int anchoBarra = estado.laneWidth - 20;
	int altoBarra = 20;

	// Definimos el hitbox de la nota
	estado.notaActual = { (float)(estado.noteLane * estado.laneWidth + 10),
					estado.noteY,
					(float)(estado.laneWidth - 20),
					(float)estado.noteHeight };

	// Definimos el hitbox del jugador
	if (porcentajeX >= 0 && porcentajeX <= 100) {

		estado.posicionDestinoX = (porcentajeX * estado.gameWidth) / 100.0f;

		if (estado.posicionRealX < 0) {
			estado.posicionRealX = estado.posicionDestinoX;
		}

	}

	if (estado.posicionRealX >= 0) {
		estado.posicionRealX += (estado.posicionDestinoX - estado.posicionRealX) * 0.4f;

		estado.barraJugador = { (float)(estado.posicionRealX - (anchoBarra / 2)),
						(float)(estado.buttonY - (altoBarra / 2)),
						(float)anchoBarra,
						(float)altoBarra };
	}

	verificarColisiones(estado);

	// Actualizamos las partículas
	for (int i = 0; i < estado.particulas.size();) {
		estado.particulas[i].x += estado.particulas[i].vx;
		estado.particulas[i].y += estado.particulas[i].vy;
		estado.particulas[i].vida -= 0.03f; //Velocidad a lo que devanece

		if (estado.particulas[i].vida <= 0) {
			estado.particulas.erase(estado.particulas.begin() + i);
		}
		else {
			i++;
		}
	}
}