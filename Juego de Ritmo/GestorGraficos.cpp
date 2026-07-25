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

void dibujarBotonUI(Rectangle limites, const char* texto, Color colorBase, Color colorHover) {
    Vector2 raton = GetMousePosition();
    bool colision = CheckCollisionPointRec(raton, limites);

    // Si el mouse pasa por el boton, cambia de color
    if (colision == true) {
        DrawRectangleRec(limites, colorHover);
    }
    else {
        DrawRectangleRec(limites, colorBase);
    }

    int anchoTexto = MeasureText(texto, 20);
    DrawText(texto, limites.x + (limites.width / 2) - (anchoTexto / 2), limites.y + (limites.height / 2) - 10, 20, WHITE);
}

void dibujarMenuPrincipal(const EstadoJuego& estado) {
    BeginDrawing();
    ClearBackground(DARKBLUE);

    const char* titulo = "RHYTHM CATCH";
    int anchoTitulo = MeasureText(titulo, 40);
    DrawText(titulo, estado.gameWidth / 2 - anchoTitulo / 2, 100, 40, RAYWHITE);

    int anchoBoton = 200;
    int altoBoton = 50;
    int centroX = estado.gameWidth / 2 - anchoBoton / 2;

    // Pintamos los botones en las mismas coordenadas que usamos en la lógica
    dibujarBotonUI({ (float)centroX, 250, (float)anchoBoton, (float)altoBoton }, "JUGAR", DARKGRAY, GRAY);
    dibujarBotonUI({ (float)centroX, 320, (float)anchoBoton, (float)altoBoton }, "PERSONALIZACION", DARKGRAY, GRAY);
    dibujarBotonUI({ (float)centroX, 390, (float)anchoBoton, (float)altoBoton }, "SALIR", MAROON, RED);
    dibujarBotonUI({ (float)estado.gameWidth - 160, 20, 140, 40 }, "CONFIG.", DARKGRAY, GRAY);

    EndDrawing();
}

void dibujarSeleccionNivel(const EstadoJuego& estado) {
    BeginDrawing();
    ClearBackground(DARKPURPLE);

    DrawText("SELECCIONA UNA CANCION", 50, 50, 30, WHITE);

    dibujarBotonUI({ 50, 150, 300, 50 }, "Nivel 1", DARKGRAY, GREEN);
    dibujarBotonUI({ 50, 220, 300, 50 }, "Nivel 2", DARKGRAY, ORANGE);
    dibujarBotonUI({ 50, 400, 150, 40 }, "VOLVER", DARKGRAY, GRAY);

    EndDrawing();
}