#include <EEPROM.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>

// LED Matrix Configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW  
#define MAX_DEVICES 4  // Number of 8x8 LED matrices

#define DATA_PIN 11  // DIN
#define CLK_PIN 13   // CLK
#define CS_PIN 10    // CS

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Relay and Button Pins
#define RELAY_PIN 2
#define BUTTON_PIN 4

// EEPROM Storage
int counterAddr = 0; // EEPROM address to store the counter
int counter = 0;
bool messageDisplayed = false; // Track if message is being shown

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor

    Serial.begin(9600);

    // Read stored counter from EEPROM
    EEPROM.get(counterAddr, counter);

    // Initialize LED matrix
    P.begin();
    P.setIntensity(5);  // Set brightness (0-15)

    // Display "TEST" on LED Matrix once
    displayText("TEST", true);

    // Activate relay for 5 seconds
    digitalWrite(RELAY_PIN, HIGH);
    delay(5000);
    digitalWrite(RELAY_PIN, LOW);

    // Increment and store counter
    counter++;
    EEPROM.put(counterAddr, counter);
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW && !messageDisplayed) { // Button pressed and message not already showing
        Serial.print("Relay activated ");
        Serial.print(counter);
        Serial.println(" times.");

        // Display the message on LED matrix
        char message[30];
        // sprintf(message, "Relay Count: %d", counter);
        sprintf(message, "water pump No 1: Normal, water pump No 2: Off");
        displayText(message, false); // Show message then turn off

        delay(300); // Debounce delay
        while (digitalRead(BUTTON_PIN) == LOW); // Wait for button release
    }

    // Handle LED matrix animations
    if (P.displayAnimate() && messageDisplayed) {
        P.displayReset();
        messageDisplayed = false; // Turn off display after finishing the message
    }
}

// Function to display text on the LED Matrix
void displayText(const char* text, bool immediateClear) {
    P.displayText(text, PA_CENTER, 100, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    messageDisplayed = true;
    
    if (immediateClear) {
        while (!P.displayAnimate()); // Wait for the animation to finish
        P.displayReset(); // Clear the display
        messageDisplayed = false;
    }
}
