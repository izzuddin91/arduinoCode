#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10  // SDA to D10
#define RST_PIN 9  // RST to D9

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    
    Serial.println("🔄 Bring an RFID card near the reader...");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent()) return; // No card detected
    if (!rfid.PICC_ReadCardSerial()) return;   // Read error

    Serial.print("🔑 Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    rfid.PICC_HaltA(); // Stop reading
    delay(1000);
}
