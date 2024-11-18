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
#define GPS_RX          7
#define GPS_TX          6

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
  Serial.println(F("Tap an RFID card to get GPS and RFID data"));
  Serial.println(F("------------------------"));
  
  // Show RFID reader details
  mfrc522.PCD_DumpVersionToSerial();
}

void loop() {
  // Check for RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show RFID card detection message
    Serial.println(F("\n------------------------"));
    Serial.println(F("RFID Card Detected!"));
    Serial.println(F("Card Details:"));
    
    // Dump debug info about the card
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    
    // Read and show current GPS data
    Serial.println(F("GPS Data:"));
    readGPSData();
    
    Serial.println(F("------------------------\n"));










    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
  }
}

void readGPSData() {
  Serial.print(F("Latitude, Longitude: "));
  
  // Read GPS data from the serial buffer
  String gpsString = "";
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    if (c == '\n') { // End of the GPS sentence
      break;
    }
    gpsString += c;
  }
  
  // Print GPS data if available
  if (gpsString.length() > 0) {
    Serial.println(gpsString);
  } else {
    Serial.println(F("No GPS data available"));
  }
}
