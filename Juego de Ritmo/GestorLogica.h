#pragma once
#include "Estructuras.h"

// Le pasamos el "EstadoJuego" por referencia (&) para que pueda modificarlo
void inicializarJuego(EstadoJuego& estado);
void verificarColisiones(EstadoJuego& estado);
void actualizarJuego(EstadoJuego& estado, int porcentajeX);