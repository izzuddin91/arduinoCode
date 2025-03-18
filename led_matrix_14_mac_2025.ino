#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// Define hardware type and connections
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  // Adjust if needed
#define MAX_DEVICES 4  // Number of matrices (each matrix is 8x8)

#define DATA_PIN 11  // DIN
#define CLK_PIN 13   // CLK
#define CS_PIN 10    // CS

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

void setup() {
  P.begin();
  P.setIntensity(5);  // Brightness (0-15)
  P.displayText("TARIKH: 23 DECEMBER 2025, SUHU: 31°, pH Water: 3.7, Last water pumped: 6:50 pm", PA_CENTER, 100, 1000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop() {
  if (P.displayAnimate()) {
    P.displayReset();
  }
}
