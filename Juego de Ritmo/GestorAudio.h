#pragma once
#include "Estructuras.h"

void inicializarAudio(EstadoJuego& estado);
void cambiarMusica(EstadoJuego& estado, CancionActual nuevaCancion);
void actualizarMusica(EstadoJuego& estado);
void verificarFinDeCancion(EstadoJuego& estado);
void cerrarAudio(EstadoJuego& estado);
