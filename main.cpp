#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);
const int signalPin = A0;
const int sampleSize = 100;

float medirAmplitud(int *muestras, int tamano) {
    int maxVal = 0;
    for (int i = 0; i < tamano; i++) {
        if (muestras[i] > maxVal) {
            maxVal = muestras[i];
        }
    }
    return (maxVal * (5.0 / 1023.0));
}
