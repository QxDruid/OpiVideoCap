#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
#define SS_PIN 10
#define RST_PIN 9
#define IrqPin 8


// initialize MRFC522
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
// Init array that will store new NUID 
byte nuidPICC[4];


void setup() {
  
  pinMode(IrqPin, OUTPUT);

  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.begin(4800); // Serial port init
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  }
  
}  // end of setup


void loop() {

// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    } 
    printHex(rfid.uid.uidByte, rfid.uid.size);
    


  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


/*
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
   digitalWrite(IrqPin, LOW);
   delayMicroseconds(300000);
   digitalWrite(IrqPin, HIGH);   
   Serial.print(0b01010101); // для установки стабильности на usart
   Serial.print(0b01010101);
   for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
    }
}
