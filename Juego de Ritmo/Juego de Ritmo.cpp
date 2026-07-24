#define WIN32_LEAN_AND_MEAN // Reduce la basura que trae Windows
#define NOGDI               // Apaga los gráficos de Windows (libera "Rectangle")
#define NOUSER              // Apaga las ventanas de Windows (libera "CloseWindow")

#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector> // Para el sistema de partículas
#include <WinSock2.h>
#include "raylib.h" // Es una librería de C para videojuegos, que nos permite crear videojuegos de manera sencilla y rápida.

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// ============================ ESTRUCTURAS ================================
struct Particula {
	float x, y;
	float vx, vy; // Velocidad en X e Y
	float vida;	  // Duración de la partícula	
};

// ========================== VARIABLES GLOBALES ======================================
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
vector<Particula> particulas;

// ========================== PROTOTIPOS DE FUNCIÓN ===================================
void recibirCoordenadas();
void inicializarJuego();
void actualizarJuego();
void verificarColisiones();
void dibujarPuntaje();
void dibujarJuego();

int main() {
	
	thread hiloRed(recibirCoordenadas);
	hiloRed.detach();
	
	inicializarJuego();
	
	// Bucle principal del juego
	while (!WindowShouldClose()) {
		actualizarJuego();
		dibujarJuego();
	}

	CloseWindow(); // Cierra la ventana y libera recursos
	return 0;
}

void recibirCoordenadas() {
	// 1) Preparamos la red
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData); //Encender el sistema de red

	// Creamos el socket UDP
	SOCKET receptor = socket(AF_INET, SOCK_DGRAM, 0);

	// Configuramos el puerto 5052
	sockaddr_in direccion;
	direccion.sin_family = AF_INET;
	direccion.sin_port = htons(5052);
	direccion.sin_addr.s_addr = INADDR_ANY; // Escuchar de cualquier lado

	// Atamos el socket al puerto
	bind(receptor, (sockaddr*)&direccion, sizeof(direccion));

	cout << "C++ escuchando en el puerto 5052..." << endl;

	// 2) Bucle de lectura
	char buffer[1024]; //Aquí guardamos el texto crudo que llegue
	sockaddr_in emisor;
	int tamanoEmisor = sizeof(emisor);

	while (true) {
		memset(buffer, 0, sizeof(buffer)); //Limpiamos la memoria vieja

		// Nos quedamos esperando hasta que llegue un mensaje
		int bytesRecibidos = recvfrom(receptor, buffer, sizeof(buffer), 0, (sockaddr*)&emisor, &tamanoEmisor);
		
		if (bytesRecibidos > 0) {

			string mensaje(buffer);
			try {
				porcentajeX = stoi(mensaje);
			}
			catch (...) {
				porcentajeX = -1;
			}
		} else {
			porcentajeX = -1;
		}
			
		
	}
}

void inicializarJuego() {
	InitWindow(800, 600, "Juego de Ritmo");

	// Configuración Responsive
	int monitor = GetCurrentMonitor();
	screenWidth = GetMonitorWidth(monitor);
	screenHeight = GetMonitorHeight(monitor);
	gameWidth = screenWidth /2 ;
	gameHeight = screenHeight;

	SetWindowSize(gameWidth, gameHeight);
	SetWindowPosition(gameWidth, 0);
	SetTargetFPS(60);

	// Calculos iniciales
	laneWidth = gameWidth / numLanes;
	buttonY = gameHeight - 100;
}

void actualizarJuego() {
	noteY += noteSpeed;

	// Si la nota sale de la pantalla (El jugador falló)
	if (noteY > gameHeight) {
		noteY = -50.0;
		noteLane = GetRandomValue(0, numLanes - 1); // Raylib: Genera un carril aleatorio
	}

	// Actualizar Rectángulos para colisión
	int anchoBarra = laneWidth - 20;
	int altoBarra = 20;

	// Definimos el hitbox de la nota
	notaActual = { (float)(noteLane * laneWidth + 10),
					noteY,
					(float)(laneWidth - 20),
					(float)noteHeight};

	// Definimos el hitbox del jugador
	if (porcentajeX >= 0 && porcentajeX <= 100) {

		posicionDestinoX = (porcentajeX * gameWidth) / 100.0f;

		if (posicionRealX < 0) {
			posicionRealX = posicionDestinoX;
		}

	}

	if (posicionRealX >= 0) {
		posicionRealX += (posicionDestinoX - posicionRealX) * 0.4f;

		barraJugador = { (float)(posicionRealX - (anchoBarra / 2)),
						(float)(buttonY - (altoBarra / 2)),
						(float)anchoBarra,
						(float)altoBarra };
	}

	verificarColisiones();

	// Actualizamos las partículas
	for (int i = 0; i < particulas.size();) {
		particulas[i].x += particulas[i].vx;
		particulas[i].y += particulas[i].vy;
		particulas[i].vida -= 0.03f; //Velocidad a lo que devanece

		if (particulas[i].vida <= 0) {
			particulas.erase(particulas.begin() + i);
		}
		else {
			i++;
		}
	}
}

void verificarColisiones() {
	// Obtenemos el rectangulo de intersección
	Rectangle interseccion = GetCollisionRec(barraJugador, notaActual);

	// Verificamos que si haya un choque y que cubra el 80% de la nota

	if (interseccion.height > 0 && interseccion.width >= (notaActual.width * 0.7f)) {
		puntaje++;

		// Generamos el efecto de partículas
		for (int i = 0; i < 15; i++) {
			Particula p;
			// Nacen dentro de la nota
			p.x = notaActual.x + (notaActual.width / 2);
			p.y = notaActual.y + (notaActual.height / 2);

			// Velocidad aleatoria dispersa en todas direcciones
			p.vx = GetRandomValue(-50, 50) / 10.0f;
			p.vy = GetRandomValue(-50, 50) / 10.0f;
			p.vida = 1.0f;
			particulas.push_back(p);
		}

		noteY = -50.0;
		noteLane = GetRandomValue(0, numLanes - 1);
	}
}

void dibujarPuntaje() {

	string texto = "Puntaje: " + to_string(puntaje);

	//Dibujamos en la esquina superior derecha
	DrawText(texto.c_str(), gameWidth - 150, 20, 24, GREEN);
}

void dibujarJuego() {

	BeginDrawing();
	ClearBackground(BLACK);

	// Carriles
	for (int i = 0; i < numLanes; i++) {
		DrawLine(i * laneWidth, 0, i * laneWidth, gameHeight, ColorAlpha(DARKGRAY, 0.5f));
	}

	// Línea de meta
	DrawLine(0, buttonY, gameWidth, buttonY, WHITE);
	
	// Nota cayendo
	DrawRectangle(noteLane * laneWidth + 10, (int)noteY, laneWidth - 20, noteHeight, RED);

	// Dibujar la barra del jugador
	int anchoBarra = laneWidth - 20;
	int altoBarra = 20;

	if (posicionRealX >= 0) {
		DrawRectangleRec(barraJugador, SKYBLUE);
	}

	// Dibujamos partículas
	for (const auto& p : particulas) {
		// La función Fade oscurece el color segun su vida restante
		DrawRectangle(p.x, p.y, 6, 6, Fade(YELLOW, p.vida));
	}

	dibujarPuntaje();

	EndDrawing();
}
