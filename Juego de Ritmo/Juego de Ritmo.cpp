#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER

#include <iostream>
#include <thread>
#include "raylib.h"

// Importamos nuestros paquetes
#include "Estructuras.h"
#include "GestorRed.h"
#include "GestorLogica.h"
#include "GestorGraficos.h"

using namespace std;

int main() {
    // 1. Creamos la estructura que guarda la información
    EstadoJuego estado;

    // 2. Variable independiente para leer la red
    int porcentajeX = -1;

    // 3. Iniciamos el hilo de red (usamos std::ref para pasar la variable correctamente)
    thread hiloRed(iniciarServidorRed, ref(porcentajeX));
    hiloRed.detach();

    // 4. Inicializamos el entorno gráfico
    inicializarJuego(estado);

    // 5. Bucle Principal
    while (!WindowShouldClose()) {
        actualizarJuego(estado, porcentajeX);
        dibujarJuego(estado);
    }

    CloseWindow();
    return 0;
}