import cv2
import mediapipe as mp
import socket

# --- CONFIGURACIÓN DEL MENSAJERO ---
IP = "127.0.0.1"
PUERTO = 5052
mensajero = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# --- CONFIGURACIÓN DE MEDIAPIPE ---
mp_manos = mp.solutions.hands
detector_manos = mp_manos.Hands(
    static_image_mode=False,
    max_num_hands=1,  # ¡Restringido a 1 sola mano!
    min_detection_confidence=0.7,
    min_tracking_confidence=0.7
)
mp_dibujo = mp.solutions.drawing_utils

camara = cv2.VideoCapture(0)
print(f"Enviando porcentaje X (1 Mano) al puerto {PUERTO}...")

while True:
    exito, imagen = camara.read()
    if not exito:
        break

    imagen = cv2.flip(imagen, 1)  # Espejo
    imagen_rgb = cv2.cvtColor(imagen, cv2.COLOR_BGR2RGB)
    resultados = detector_manos.process(imagen_rgb)

    if resultados.multi_hand_landmarks:
        # Como max_num_hands es 1, solo tomamos la primera mano que encuentre
        mano = resultados.multi_hand_landmarks[0]
        mp_dibujo.draw_landmarks(imagen, mano, mp_manos.HAND_CONNECTIONS)

        # Extraemos la X del índice (Landmark 8) y la volvemos porcentaje
        x_porcentaje = int(mano.landmark[8].x * 100)

        # Enviamos el número directamente (sin comas)
        mensajero.sendto(str(x_porcentaje).encode(), (IP, PUERTO))

    cv2.imshow("Sensor de X (1 Mano)", imagen)

    if cv2.waitKey(1) == 27:
        break

camara.release()
cv2.destroyAllWindows()