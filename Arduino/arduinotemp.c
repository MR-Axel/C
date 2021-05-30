
/*
	Capitulo 23 de Arduino desde cero en Español.
	Programa que utiliza el sensor de temperatura analogico LM35 para tomar 5
	muestras de temperatura y mostrar el promedio de las mismas en el monitor serial
	de manera de evitar variaciones indeseables.

	Autor: bitwiseAr

*/

int SENSOR;		// variable almacena valor leido de entrada analogica A0
float TEMPERATURA;	// valor de temperatura en grados centigrados
float SUMA;		// valor de la suma de las 5 lecturas de temperatura

void setup(){
  Serial.begin(9600);	// inicializacion de comunicacion serie a 9600 bps
  // entradas analógicas no requieren inicialización
}

void loop(){
  SUMA = 0;					// valor inicial de SUMA en cero
  for (int i=0; i < 5; i++){			// bucle que repite 5 veces
  SENSOR = analogRead(A0);			// lectura de entrada analogica A0	
  TEMPERATURA = ((SENSOR * 5000.0) / 1023) / 10;// formula para convertir valor leido
						// de entrada A0 en grados centigrados
  SUMA = TEMPERATURA + SUMA;			// suma de cada lectura de temperatura
  delay(500);					// demora de medio seg. entre lecturas
  }
  Serial.println(SUMA/5.0, 1);			// imprime en monitor serial valor promedio de
						// las 5 lecturas de temperatura con un decimal
}