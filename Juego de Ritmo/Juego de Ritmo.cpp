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
#include "GestorMenu.h"
#include "GestorAudio.h"

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

    inicializarAudio(estado);
    cambiarMusica(estado, CANCION_MENU);

    // 5. Bucle Principal
    while (!WindowShouldClose() && estado.pantalla != CERRAR_JUEGO) {

        actualizarMusica(estado);
        verificarFinDeCancion(estado);
        
        switch (estado.pantalla) {
            
            case MENU_PRINCIPAL:
                actualizarMenuPrincipal(estado);
                dibujarMenuPrincipal(estado);
                break;

            case SELECCION_NIVEL:
                actualizarSeleccionNivel(estado);
                dibujarSeleccionNivel(estado);
                break;

            case JUGANDO:
                actualizarJuego(estado, porcentajeX);
                dibujarJuego(estado);
                break;

            case PERSONALIZACION:
                // Aun no tenemos nada en personalizacion
                break;

            case CONFIGURACION:
                // Aun no tenemos nada en configuracion
                break;
        }

    }

    cerrarAudio(estado);
    CloseWindow();
    return 0;
}