// Se encargará de guardar las variables globales

#pragma once
#include <vector>
#include "raylib.h"

struct Particula {
	float x, y;
	float vx, vy; // Velocidad en X e Y
	float vida;	  // Duración de la partícula	
};

struct EstadoJuego {
	int screenWidth, screenHeight;		// Ancho y alto de la pantalla
	int gameWidth, gameHeight;			// Ancho y alto del juego

	int numLanes = 4;
	int laneWidth;
	int buttonHeight = 120, buttonY;

	float noteY = -50.0;
	float noteSpeed = 6.0;
	int noteLane = 1;
	int noteHeight = 50;

	int porcentajeX = -1;
	int puntaje = 0;

	int framesSinMano = 0;
	const int LIMITE_TOLERANCIA = 15;

	float posicionDestinoX = -1000.f;	// Donde python dice que esta la mano
	float posicionRealX = -1000.f;		// Donde se esta dibujando la barra

	// Rectangulos para las colisiones
	Rectangle barraJugador = { -1000, -1000, 0, 0 };
	Rectangle notaActual = { 0,0,0,0 };

	// Vector dinámico para guardar las partículas activas
	std::vector<Particula> particulas;
};