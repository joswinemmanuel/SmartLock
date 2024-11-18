#include <SPI.h>
#include <MFRC522.h>

// Pin definitions for RC522
#define RST_PIN         9           
#define SS_PIN          10          

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);         // Initialize serial communication
  SPI.begin();               // Initialize SPI bus
  mfrc522.PCD_Init();       // Initialize MFRC522
  delay(4);                  // Optional delay
  
  Serial.println(F("RFID Reader initialized."));
  Serial.println(F("Tap a card to read..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show card details
  Serial.println(F("\n==== Card Detected! ===="));
  
  // Show UID
  Serial.print(F("Card UID: "));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Show card type
  Serial.print(F("Card type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
