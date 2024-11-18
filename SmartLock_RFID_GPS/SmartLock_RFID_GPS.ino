/*
 * Combined GPS NEO 6M and RFID-RC522 reader program
 * for Arduino UNO
 */

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define RST_PIN         9          
#define SS_PIN          10         

// GPS pins
#define GPS_RX          4
#define GPS_TX          3

// Initialize instances
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); // GPS serial connection

void setup() {
  // Initialize serial communications with PC
  Serial.begin(9600);
  while (!Serial);    // Wait for serial port to connect
  
  // Initialize GPS
  gpsSerial.begin(9600);
  
  // Initialize RFID
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  delay(4);           // Optional delay
  
  // Print initial messages
  Serial.println(F("GPS and RFID Reader initialized"));
  Serial.println(F("------------------------"));
  Serial.println(F("GPS Data will stream continuously"));
  Serial.println(F("RFID Card data will be shown when card is detected"));
  Serial.println(F("------------------------"));
  
  // Show RFID reader details
  mfrc522.PCD_DumpVersionToSerial();
}

void loop() {
  // Read GPS data if available
  readGPSData();
  
  // Check for RFID cards
  checkRFIDCard();
}

void readGPSData() {
  while (gpsSerial.available() > 0) {
    byte gpsData = gpsSerial.read();
    Serial.write(gpsData);
  }
}

void checkRFIDCard() {
  // Return if no card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  // Show RFID card detection message
  Serial.println(F("\n------------------------"));
  Serial.println(F("RFID Card Detected!"));
  Serial.println(F("Card Details:"));
  
  // Dump debug info about the card
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  Serial.println(F("------------------------\n"));
  
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}