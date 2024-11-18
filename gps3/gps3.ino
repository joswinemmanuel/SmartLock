#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  
  Serial.println(F("GPS Module Test"));
  Serial.println(F("Looking for satellites..."));
}

void loop() {
  // Print satellite data every 5 seconds
  static unsigned long lastDebugPrint = 0;
  
  // Feed the GPS parser
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  if (millis() - lastDebugPrint > 5000) {
    lastDebugPrint = millis();
    
    Serial.println("\n=== GPS Status ===");
    Serial.print("Satellites in view: ");
    Serial.println(gps.satellites.value());
    
    Serial.print("Valid Location: ");
    Serial.println(gps.location.isValid() ? "Yes" : "No");
    
    if (gps.location.isValid()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    }
    
    Serial.print("Valid Date/Time: ");
    Serial.println(gps.date.isValid() ? "Yes" : "No");
    
    if (gps.date.isValid() && gps.time.isValid()) {
      Serial.print("Date/Time: ");
      Serial.print(gps.date.year());
      Serial.print("-");
      Serial.print(gps.date.month());
      Serial.print("-");
      Serial.print(gps.date.day());
      Serial.print(" ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }
    
    // Check if GPS is working at all
    if (millis() > 5000 && gps.charsProcessed() < 10) {
      Serial.println("WARNING: No GPS data received!");
      Serial.println("Check wiring and antenna");
    }
  }
}
