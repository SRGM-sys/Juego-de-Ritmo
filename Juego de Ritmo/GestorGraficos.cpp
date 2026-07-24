#include "GestorGraficos.h"

void dibujarPuntaje(const EstadoJuego& estado) {
    std::string texto = "Puntaje: " + std::to_string(estado.puntaje);
    DrawText(texto.c_str(), estado.gameWidth - 150, 20, 24, GREEN);
}

void dibujarJuego(const EstadoJuego& estado) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Carriles
    for (int i = 0; i < estado.numLanes; i++) {
        DrawLine(i * estado.laneWidth, 0, i * estado.laneWidth, estado.gameHeight, ColorAlpha(DARKGRAY, 0.5f));
    }

    // Línea de meta
    DrawLine(0, estado.buttonY, estado.gameWidth, estado.buttonY, WHITE);

    // Nota cayendo
    DrawRectangleRec(estado.notaActual, RED);

    // Barra del jugador
    if (estado.posicionRealX >= 0) {
        DrawRectangleRec(estado.barraJugador, SKYBLUE);
    }

    // Partículas
    for (const auto& p : estado.particulas) {
        DrawRectangle(p.x, p.y, 6, 6, Fade(YELLOW, p.vida));
    }

    dibujarPuntaje(estado);
    EndDrawing();
}