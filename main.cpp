#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);
const int signalPin = A0;
const int sampleSize = 100;

float medirAmplitud(int *muestras, int tamano) {
    int maxVal = 0;
    int minVal = 1023;
    for (int i = 0; i < tamano; i++) {
        if (muestras[i] > maxVal) {
            maxVal = muestras[i];
        }
        if (muestras[i] < minVal) {
            minVal = muestras[i];
        }
    }
    float amplitud = (maxVal - minVal) * (5.0 / 1023.0) / 2.0;
    return amplitud;
}


float medirFrecuencia(int *muestras, int tamano, float tiempoMuestreo) {
    int crucesPorCero = 0;
    bool cruzandoCero = false;

    for (int i = 1; i < tamano; i++) {
        if ((muestras[i - 1] < 512 && muestras[i] >= 512) || 
            (muestras[i - 1] >= 512 && muestras[i] < 512)) {
            if (!cruzandoCero) {
                crucesPorCero++;
                cruzandoCero = true;
            }
        } else {
            cruzandoCero = false;
        }
    }
    
    if (crucesPorCero >= 2) {
        float tiempoTotal = tiempoMuestreo * tamano;
        float periodo = tiempoTotal / (crucesPorCero / 2.0);  
        return (1.0 / periodo);  
    } else {
        return 0.0; 
    }
}


void tipoSenal(int *muestras, int tamano) {
   int ascenso = 0, descenso = 0, saltos = 0, i = 1;
  
   while (i < tamano){
        float residuo = muestras[i] - muestras[i - 1];
     
     	if (residuo > 0) {
            ascenso++;
        } else if (residuo < 0) {
            descenso++;
        }
        
        if (abs(residuo) > 1.0) {  
            saltos++;
        }
     	i++;
   }
  
  if (ascenso == descenso || abs(ascenso - descenso) == 1) { 
      	lcd.setCursor(0, 1);
  		lcd.print("Senal cuadrada");
   } else if (ascenso > descenso && saltos == 0) {
       lcd.setCursor(0, 1);
       lcd.print("Senal senoidal");
   } else if (ascenso > descenso && saltos > 0) {
    	lcd.setCursor(0, 1);
     	lcd.print("Senal triangular");
   } else {
     	lcd.setCursor(0, 1);
     	lcd.print("Senal desconocida");
   }
}

void setup() {
    lcd.begin(16, 2);
    lcd.print("Iniciando...");
    delay(2000);
    lcd.clear();
    Serial.begin(9600);
}

void loop() {
    int *muestras = new int[sampleSize];
    float tiempoMuestreo = 0.001;

    for (int i = 0; i < sampleSize; i++) {
        muestras[i] = analogRead(signalPin);
        delay(tiempoMuestreo * 1000);
    }

    float amplitud = medirAmplitud(muestras, sampleSize);
    float frecuencia = medirFrecuencia(muestras, sampleSize, tiempoMuestreo);
	
    lcd.setCursor(0, 0);
    lcd.print("Amp: ");
    lcd.print(amplitud);
    lcd.print(" V");

    lcd.setCursor(0, 1);
    lcd.print("Freq: ");
    lcd.print(frecuencia);
    lcd.print(" Hz");
  	
  	tipoSenal(muestras, sampleSize);

    delete[] muestras;
    delay(1000);
}
