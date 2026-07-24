#define WIN32_LEAN_AND_MEAN

#include "GestorRed.h"
#include <WinSock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Aqui se conecta con Estructuras.h (porcentajeX)
void iniciarServidorRed(int& porcentajeX) {

	// 1) Preparamos la red
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData); //Encender el sistema de red

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
		}
		else {
			porcentajeX = -1;
		}


	}

}