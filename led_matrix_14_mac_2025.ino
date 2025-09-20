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

// this is just a sample for pure clock code 
// #include <ThreeWire.h>
// #include <RtcDS1302.h>

// ThreeWire myWire(3, 2, 4);  // DAT, CLK, CE
// RtcDS1302<ThreeWire> rtc(myWire);

// void setup() {
//   Serial.begin(9600);
//   rtc.Begin();

//   if (!rtc.GetIsRunning()) {
//     rtc.SetIsRunning(true);
//     rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
//     Serial.println("RTC initialized.");
//   }
// }

// void loop() {
//   RtcDateTime now = rtc.GetDateTime();
//   Serial.print(now.Year(), DEC); Serial.print('/');
//   Serial.print(now.Month(), DEC); Serial.print('/');
//   Serial.print(now.Day(), DEC); Serial.print(' ');
//   Serial.print(now.Hour(), DEC); Serial.print(':');
//   Serial.print(now.Minute(), DEC); Serial.print(':');
//   Serial.print(now.Second(), DEC); Serial.println();
//   delay(1000);
// }




// this code is updated at 20 sept 2025
// look at the connection for clock at ThreeWire myWire(3, 2, 4)
// look at the connection for led at define part, 11,13,10
// all connection at digital pin
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

// ---------- RTC setup ----------
ThreeWire myWire(3, 2, 4);  // DAT, CLK, CE
RtcDS1302<ThreeWire> rtc(myWire);

// ---------- LED matrix setup ----------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 11
#define CLK_PIN 13
#define CS_PIN 10

MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Timer for controlling scroll
unsigned long lastScrollTime = 0;
const unsigned long scrollInterval = 10000; // 5 seconds

void setup() {
  Serial.begin(9600);

  // Initialize RTC
  rtc.Begin();
  if (!rtc.GetIsRunning()) {
    rtc.SetIsRunning(true);
    rtc.SetDateTime(RtcDateTime(__DATE__, __TIME__));
    Serial.println("RTC initialized.");
  }

  // Initialize LED matrix
  P.begin();
  P.setIntensity(5);  // Brightness 0-15
  P.displayClear();
}

void loop() {
  unsigned long currentMillis = millis();

  // Start scroll every 5 seconds
  if (currentMillis - lastScrollTime >= scrollInterval) {
    lastScrollTime = currentMillis;

    // Get current time from RTC
    RtcDateTime now = rtc.GetDateTime();
    char timeString[16];
    sprintf(timeString, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());

    // Build scrolling text with current time
    char displayString[128];
    sprintf(displayString, "Time: %s, Date: %02d/%02d/%04d, Suhu: 31C, pH Water: 3.7, Last Pump: 6:50 PM",
            timeString, now.Day(), now.Month(), now.Year());

    Serial.println(displayString); // Optional: print to Serial

    // Start scrolling on LED matrix
    P.displayText(displayString,
                  PA_CENTER,
                  50,       // scroll speed
                  0,        // pause at end
                  PA_SCROLL_LEFT,
                  PA_SCROLL_LEFT);
  }

  // Animate the current scroll (must be called continuously)
  P.displayAnimate();
}
