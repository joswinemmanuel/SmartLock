#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          10          

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  while (!Serial);    // Wait for serial port to connect
  
  Serial.println(F("RFID RC522 Diagnostic Test"));
  
  // Initialize SPI
  SPI.begin();
  Serial.println(F("SPI initialized"));
  
  // Initialize MFRC522
  mfrc522.PCD_Init();
  
  // Get the MFRC522 firmware version
  byte v = mfrc522.PCD_ReadRegister(MFRC522::VersionReg);
  
  Serial.print(F("Firmware Version: 0x"));
  Serial.println(v, HEX);
  
  if(v == 0x00 || v == 0xFF) {
    Serial.println(F("WARNING: Communication failure, check connections"));
    Serial.println(F("Common issues:"));
    Serial.println(F("1. Check if SS and RST pins are correctly connected"));
    Serial.println(F("2. Make sure module is properly powered (3.3V)"));
    Serial.println(F("3. Check if SPI pins (MOSI, MISO, SCK) are correct"));
    while(1); // Stop here
  }
  
  Serial.println(F("\nRFID Reader detected and initialized!"));
  
  // Perform self-test
  bool selfTestPass = mfrc522.PCD_PerformSelfTest();
  if(selfTestPass) {
    Serial.println(F("Self test: PASSED"));
  } else {
    Serial.println(F("Self test: FAILED"));
    Serial.println(F("Check if the module is genuine and properly powered"));
    while(1);
  }
  
  Serial.println(F("\nReady to scan cards..."));
  Serial.println(F("When scanning a card, you should see the LED on RC522 blink"));
}

void loop() {
  // Check if a card is present
  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(F("Card detected!"));
    
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.print(F("Card UID: "));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }
  
  // Print a status dot every second to show the program is running
  static unsigned long lastDot = 0;
  if (millis() - lastDot > 1000) {
    Serial.print(".");
    lastDot = millis();
  }
}
