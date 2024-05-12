#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MRFC522 instance.
byte accessUID[4] = {0xE3, 0x41, 0x7A, 0x35};

// initialize the library by associating any LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 6, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

void setup() {
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Hello! Please");

    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    lcd.print("scan your ID");

    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init(); // Init MFRC522 card
    delay(1000);

    Serial.println("CLEARDATA");
    Serial.println("LABEL,Date,Time,RFID UID");
    delay(1000);

    Serial.println("Scan PICC to see UID...");
    Serial.println("");
}
// --------------------------------------------------------------------
void loop() {
    readsuccess = getid();

    if (readsuccess) {
        Serial.println( (String) "DATA,DATE,TIME," + StrUID );
    }
}
// --------------------------------------------------------------------
int getid() {
    if(!mfrc522.PICC_IsNewCardPresent()) {
        return 0;
    }
    if(!mfrc522.PICC_ReadCardSerial()) {
        return 0;
    }


    Serial.println("THE UID OF THE SCANNED CARD IS:");
    if(mfrc522.uid.uidByte[0] == accessUID[0] && mfrc522.uid.uidByte[1] == accessUID[1] && mfrc522.uid.uidByte[2] == accessUID[2] && mfrc522.uid.uidByte[3] == accessUID[3] && mfrc522.uid.uidByte[4] == accessUID[4] && mfrc522.uid.uidByte[5] == accessUID[5] && mfrc522.uid.uidByte[6] == accessUID[6]) {
        for (int i=0;i<4;i++) {
            readcard[i]=mfrc522.uid.uidByte[i]; // storing the UID of the tag in readcard
            array_to_string(readcard, 4, str);
            StrUID = str;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Welcome Niha!");
        }
    } else {
        StrUID = "NOTVALID";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid ID");
        delay(1200);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Go to main");
        lcd.setCursor(0, 1);
        lcd.print("office");
    }

    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello! Please");
    lcd.setCursor(0, 1);
    lcd.print("scan your ID");
    mfrc522.PICC_HaltA();
    return 1;
}
// --------------------------------------------------------------------
void array_to_string(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++) {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        buffer[i*2+1] nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
    }
    buffer[len*2] = '\0';
}
