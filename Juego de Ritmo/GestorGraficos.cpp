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

void dibujarEditor(const EstadoJuego& estado) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // Dibujar los carriles (Igual que en el juego principal)
    for (int i = 0; i < estado.numLanes; i++) {
        DrawLine(i * estado.laneWidth, 0, i * estado.laneWidth, estado.gameHeight, LIGHTGRAY);
    }

    // Dibujamos la línea de meta (Donde debes presionar las notas)
    DrawLine(0, estado.buttonY, estado.gameWidth, estado.buttonY, WHITE);

    // Dibujar las notas que hemos guardado
    // Recorremos nuestra lista de notasGuardadas una por una
    for (int i = 0; i < estado.notasGuardadas.size(); i++) {
        NotaDatos nota = estado.notasGuardadas[i];

        // LÓGICA DE LA LÍNEA DE TIEMPO
        // Si el tiempo de la nota es igual al tiempo actual, la nota está en la linea de meta
        // Si el tiempo de la nota es Mayor al actual, la nota debe estar más arriba en la pantalla
        // Multiplicamos por 200 para exagerar la distancia visualmente (px por segundo)
        float diferenciaTiempo = nota.tiempo - estado.tiempoEditor;
        float posicionY = estado.buttonY - (diferenciaTiempo * 200.0f);

        // Solo la dibujamos si esta dentro de la pantalla
        if (posicionY > -100 && posicionY < estado.gameHeight + 100) {
            float posicionX = nota.carril * estado.laneWidth + 10;
            DrawRectangle(posicionX, posicionY, estado.laneWidth - 20, estado.noteHeight, GREEN);
        }
    }

    // Dibujar textos de ayuda en pantalla
    DrawText("MODO EDITOR", estado.gameWidth + 20, 20, 30, YELLOW);

    // Mostramos el tiempo actual
    std::string textoTiempo = "Tiempo: " + std::to_string(estado.tiempoEditor);
    DrawText(textoTiempo.c_str(), estado.gameWidth + 20, 70, 20, WHITE);

    if (estado.reproduciendoEditor) {
        DrawText("REPRODUCIENDO", estado.gameWidth + 20, 110, 20, GREEN);
    }
    else {
        DrawText("Estado: PAUSADO", estado.gameWidth + 20, 110, 20, RED);
    }

    DrawText("Controles:", estado.gameWidth + 20, 200, 20, LIGHTGRAY);
    DrawText("- ESPACIO: Play / Pausa", estado.gameWidth + 20, 230, 20, LIGHTGRAY);
    DrawText("- CLIC IZQ: Poner Nota", estado.gameWidth + 20, 260, 20, LIGHTGRAY);
    DrawText("- ESC: Volver al Menu", estado.gameWidth + 20, 290, 20, LIGHTGRAY);

    EndDrawing();


}