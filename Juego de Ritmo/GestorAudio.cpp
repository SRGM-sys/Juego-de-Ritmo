#include "GestorAudio.h"

void inicializarAudio(EstadoJuego& std) {
	InitAudioDevice(); //Encender el sistema de audio de Raylib

	//Cargamos la música
	std.musicaMenu = LoadMusicStream("assets/menu.mp3");
	std.musicaNivel1 = LoadMusicStream("assets/level1.mp3");

	// Tambien puedo cargar aqui efectos de sonido
    std.sonidoBoton = LoadSound("assets/boton.mp3");
    std.sonidoColision = LoadSound("assets/colision.mp3");

}

void cambiarMusica(EstadoJuego& std, CancionActual nuevaCancion) {
    // 1. Detenemos la canción que estaba sonando usando ifs simples
    if (std.cancionActiva == CANCION_MENU) {
        StopMusicStream(std.musicaMenu);
    }
    else if (std.cancionActiva == CANCION_NIVEL1) {
        StopMusicStream(std.musicaNivel1);
    }
    else if (std.cancionActiva == CANCION_NIVEL2) {
        StopMusicStream(std.musicaNivel2);
    }

    // Actualizamos el estado
    std.cancionActiva = nuevaCancion;

    // Reproducimos la nueva cancion
    if (std.cancionActiva == CANCION_MENU) {
        PlayMusicStream(std.musicaMenu);
    }
    else if (std.cancionActiva == CANCION_NIVEL1) {
        PlayMusicStream(std.musicaNivel1);
    }
    else if (std.cancionActiva == CANCION_NIVEL2) {
        PlayMusicStream(std.musicaNivel2);
    }
}

// Raylib necesita que la música se actualice constantemente cuadro a cuadro
void actualizarMusica(EstadoJuego& estado) {
    if (estado.cancionActiva == CANCION_MENU) {
        UpdateMusicStream(estado.musicaMenu);
    }
    else if (estado.cancionActiva == CANCION_NIVEL1) {
        UpdateMusicStream(estado.musicaNivel1);
    }
    else if (estado.cancionActiva == CANCION_NIVEL2) {
        UpdateMusicStream(estado.musicaNivel2);
    }
}

// El nivel termina cuando la música termina
void verificarFinDeCancion(EstadoJuego& estado) {
    if (estado.pantalla == JUGANDO) {
        float tiempoJugado = 0.0f;
        float tiempoTotal = 0.0f;

        if (estado.cancionActiva == CANCION_NIVEL1) {
            tiempoJugado = GetMusicTimePlayed(estado.musicaNivel1);
            tiempoTotal = GetMusicTimeLength(estado.musicaNivel1);
        }
        else if (estado.cancionActiva == CANCION_NIVEL2) {
            tiempoJugado = GetMusicTimePlayed(estado.musicaNivel2);
            tiempoTotal = GetMusicTimeLength(estado.musicaNivel2);
        }

        // Le damos un pequeño margen de 0.1 segundos para evitar cortes bruscos
        if (tiempoJugado > 0.0f && tiempoJugado >= (tiempoTotal - 0.1f)) {
            // El nivel terminó, volvemos al menú y cambiamos la música
            estado.pantalla = MENU_PRINCIPAL;
            cambiarMusica(estado, CANCION_MENU);
        }
    }
}

void cerrarAudio(EstadoJuego& estado) {
    // Descargamos todo de la memoria RAM
    UnloadMusicStream(estado.musicaMenu);
    UnloadMusicStream(estado.musicaNivel1);
    //UnloadMusicStream(estado.musicaNivel2);

    UnloadSound(estado.sonidoBoton);
    UnloadSound(estado.sonidoColision);

    CloseAudioDevice(); // Apagamos el sistema de audio
}