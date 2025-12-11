/*
 * SMART CITY OLED DISPLAY - RFID & ELEVATOR ACCESS SYSTEM
 * 
 * Hardware:
 * - Arduino Uno/Nano
 * - SH1106 OLED Display (128x64) via I2C - SDA: A4, SCL: A5
 * - MFRC522 RFID Module - SS: Pin 10, RST: Pin 9, SPI: 11,12,13
 * 
 * Communication:
 * - Serial 9600 baud with Qt Application
 * 
 * Messages from Qt App:
 * - APPROVED:NOM:PRENOM  → Display resident name with scroll animation
 * - DENIED:reason        → Display access denied message
 * - INCIDENT:message     → Display incident alert with scroll
 * - PANNE:message        → Display failure/maintenance message
 * - WELCOME              → Return to idle screen
 */

#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <U8g2lib.h>

// ============ PIN CONFIGURATION ============
#define RST_PIN 9
#define SS_PIN 10

// ============ RFID SETUP ============
MFRC522 rfid(SS_PIN, RST_PIN);

// ============ OLED SETUP - SH1106 128x64 with U8G2 ============
U8G2_SH1106_128X64_NONAME_1_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

// ============ ANIMATION & TIMING ============
#define SCROLL_SPEED 30       // milliseconds between scroll frames
#define DISPLAY_TIMEOUT 5000  // 5 seconds before return to idle
#define MAX_SCROLL_POS 300    // max scroll position

// ============ GLOBAL STATE ============
String lastUID = "";
String currentResidentName = "";
String currentIncidentMsg = "";
unsigned long lastEventTime = 0;
int scrollOffset = 0;
bool isScrolling = false;
int deniedCount = 0;
const String BLOCKED_UID = "A79BF604";  // UID to block

enum DisplayState {
  STATE_IDLE,
  STATE_WELCOME,
  STATE_DENIED,
  STATE_INCIDENT,
  STATE_PANNE
};

DisplayState currentState = STATE_IDLE;

// ============ FORWARD DECLARATIONS ============
void displayIdle();
void displayWelcomeWithScroll(String name);
void displayDeniedWithScroll(String reason);
void displayIncidentWithScroll(String message);
void displayPanne(String message);
void readRFIDCard();
void handleSerialCommand();
void sendToQtApp(String message);

// ============ SETUP ============
void setup() {
  Serial.begin(9600);
  delay(500);
  
  // Initialize OLED
  oled.begin();
  oled.setFont(u8g2_font_ncenB08_tr);
  
  // Initialize RFID
  SPI.begin();
  delay(200);
  rfid.PCD_Init();
  delay(200);
  
  Serial.println("[SYSTEM] OLED & RFID Ready");
  Serial.println("[CONFIG] OLED: SH1106 128x64, RFID: MFRC522");
  
  displayIdle();
}

// ============ MAIN LOOP ============
void loop() {
  // Check serial commands from Qt App
  handleSerialCommand();
  
  // Read RFID cards
  readRFIDCard();
  
  // Update animation
  unsigned long currentTime = millis();
  
  if (isScrolling && currentTime - lastEventTime < DISPLAY_TIMEOUT) {
    // Continue scrolling animation
    switch (currentState) {
      case STATE_WELCOME:
        displayWelcomeWithScroll(currentResidentName);
        break;
      case STATE_DENIED:
        displayDeniedWithScroll(currentResidentName);
        break;
      case STATE_INCIDENT:
        displayIncidentWithScroll(currentIncidentMsg);
        break;
      default:
        break;
    }
    delay(SCROLL_SPEED);
  } 
  // Timeout - return to idle
  else if (currentTime - lastEventTime > DISPLAY_TIMEOUT && currentState != STATE_IDLE) {
    currentState = STATE_IDLE;
    isScrolling = false;
    scrollOffset = 0;
    displayIdle();
  }
  
  delay(10);
}

// ============ DISPLAY: IDLE SCREEN ============
void displayIdle() {
  oled.firstPage();
  do {
    // Draw title
    oled.setFont(u8g2_font_ncenB14_tr);
    oled.drawStr(8, 22, "SMART");
    oled.drawStr(8, 38, "CITY");
    
    // Draw subtitle
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(15, 55, "Passe une carte");
  } while (oled.nextPage());
}

// ============ DISPLAY: WELCOME WITH SCROLLING TEXT ============
void displayWelcomeWithScroll(String name) {
  oled.firstPage();
  do {
    // Top banner
    oled.drawBox(0, 0, 128, 12);
    oled.setDrawColor(0);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(30, 10, "BIENVENUE");
    oled.setDrawColor(1);
    
    // Scrolling text - continuous horizontal scroll
    oled.setFont(u8g2_font_ncenB14_tr);
    
    // Calculate scroll position (left to right, continuous)
    int nameWidth = name.length() * 8;  // Approximate
    int maxScroll = nameWidth + 140;
    int displayPos = 130 - scrollOffset;
    
    // Draw scrolling name
    if (displayPos < 0 && displayPos + nameWidth > 0) {
      oled.drawStr(displayPos, 42, name.c_str());
    } else if (displayPos >= 0 && displayPos < 128) {
      oled.drawStr(displayPos, 42, name.c_str());
    }
    
    // Advance scroll
    scrollOffset += 2;
    if (scrollOffset > maxScroll) {
      scrollOffset = 0;
    }
    
    // Bottom bar
    oled.drawLine(0, 48, 128, 48);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(10, 62, "Porte debloquee");
    
  } while (oled.nextPage());
}

// ============ DISPLAY: DENIED WITH SCROLLING ============
void displayDeniedWithScroll(String reason) {
  oled.firstPage();
  do {
    // Top banner
    oled.drawBox(0, 0, 128, 12);
    oled.setDrawColor(0);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(40, 10, "REFUSE");
    oled.setDrawColor(1);
    
    // Scrolling reason text
    oled.setFont(u8g2_font_ncenB10_tr);
    
    int reasonWidth = reason.length() * 6;
    int maxScroll = reasonWidth + 140;
    int displayPos = 130 - scrollOffset;
    
    if (displayPos < 0 && displayPos + reasonWidth > 0) {
      oled.drawStr(displayPos, 35, reason.c_str());
    } else if (displayPos >= 0 && displayPos < 128) {
      oled.drawStr(displayPos, 35, reason.c_str());
    }
    
    scrollOffset += 2;
    if (scrollOffset > maxScroll) {
      scrollOffset = 0;
    }
    
    // Warning symbol
    oled.drawCircle(64, 55, 5);
    
  } while (oled.nextPage());
}

// ============ DISPLAY: INCIDENT WITH SCROLLING ============
void displayIncidentWithScroll(String message) {
  oled.firstPage();
  do {
    // Top banner - INCIDENT
    oled.drawBox(0, 0, 128, 12);
    oled.setDrawColor(0);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(32, 10, "INCIDENT !");
    oled.setDrawColor(1);
    
    // Scrolling message
    oled.setFont(u8g2_font_ncenB08_tr);
    
    int msgWidth = message.length() * 6;
    int maxScroll = msgWidth + 150;
    int displayPos = 130 - scrollOffset;
    
    // Draw alert on left, message scrolls
    oled.drawStr(5, 30, "!");
    
    if (displayPos < 0 && displayPos + msgWidth > 0) {
      oled.drawStr(displayPos, 30, message.c_str());
    } else if (displayPos >= 0 && displayPos < 128) {
      oled.drawStr(displayPos, 30, message.c_str());
    }
    
    scrollOffset += 2;
    if (scrollOffset > maxScroll) {
      scrollOffset = 0;
    }
    
    // Bottom status
    oled.drawLine(0, 45, 128, 45);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(15, 62, "Alerte Securite");
    
  } while (oled.nextPage());
}

// ============ DISPLAY: SYSTEM FAILURE / MAINTENANCE ============
void displayPanne(String message) {
  oled.firstPage();
  do {
    // Top banner
    oled.drawBox(0, 0, 128, 12);
    oled.setDrawColor(0);
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(40, 10, "PANNE");
    oled.setDrawColor(1);
    
    // Message
    oled.setFont(u8g2_font_ncenB10_tr);
    oled.drawStr(10, 32, message.c_str());
    
    // Service indicator
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(35, 55, "SERVICE");
    
  } while (oled.nextPage());
}

// ============ READ RFID CARD ============
void readRFIDCard() {
  if (!rfid.PICC_IsNewCardPresent()) {
    delay(30);
    return;
  }
  
  if (!rfid.PICC_ReadCardSerial()) {
    delay(30);
    return;
  }
  
  // Extract UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10)
      uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  
  lastUID = uid;
  lastEventTime = millis();
  scrollOffset = 0;
  
  // Check if UID is blocked
  if (uid == BLOCKED_UID) {
    deniedCount++;
    
    // Afficher message différent selon le nombre de tentatives
    if (deniedCount < 3) {
      // Tentatives 1 et 2 : Erreur - Réessayez
      Serial.print("DENIED:");
      Serial.println(uid);
      
      currentResidentName = "Erreur - Reessayez une autre fois - Porte bloquee";
      currentState = STATE_DENIED;
      isScrolling = true;
      lastEventTime = millis();
      scrollOffset = 0;
      
      // Afficher directement sur l'OLED
      displayDeniedWithScroll(currentResidentName);
      
    } else {
      // 3ème tentative : Déclencher incident
      Serial.println("INCIDENT:Declaration incident Panne ascenseur");
      
      currentIncidentMsg = "Declaration incident Panne ascenseur";
      currentState = STATE_INCIDENT;
      isScrolling = true;
      lastEventTime = millis();
      scrollOffset = 0;
      
      deniedCount = 0;  // Reset counter après incident
    }
    
    delay(200);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(100);
    return;
  }
  
  // Send to Qt App for normal lookup
  Serial.print("UID:");
  Serial.println(uid);
  
  // Show loading screen
  oled.firstPage();
  do {
    oled.setFont(u8g2_font_ncenB10_tr);
    oled.drawStr(25, 30, "Verification");
    oled.setFont(u8g2_font_ncenB08_tr);
    oled.drawStr(30, 50, uid.c_str());
  } while (oled.nextPage());
  
  delay(200);
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(100);
}

// ============ HANDLE SERIAL MESSAGES FROM QT ============
void handleSerialCommand() {
  if (!Serial.available())
    return;
  
  String message = Serial.readStringUntil('\n');
  message.trim();
  
  Serial.println("[QT_MSG] " + message);
  
  // Format: APPROVED:NOM:PRENOM
  if (message.startsWith("APPROVED:")) {
    String info = message.substring(9);
    
    // Parse NOM and PRENOM
    int colonPos = info.indexOf(':');
    String nom = "", prenom = "";
    
    if (colonPos > 0) {
      nom = info.substring(0, colonPos);
      prenom = info.substring(colonPos + 1);
    } else {
      nom = info;
    }
    
    currentResidentName = nom + " " + prenom;
    currentState = STATE_WELCOME;
    isScrolling = true;
    lastEventTime = millis();
    scrollOffset = 0;
    deniedCount = 0;  // Reset counter
    
    Serial.println("[APPROVED] " + currentResidentName);
  }
  
  // Format: DENIED:reason
  else if (message.startsWith("DENIED:")) {
    String reason = message.substring(7);
    
    String deniedMsg = "Erreur - Reessayez une autre fois - Porte bloquee";
    currentResidentName = reason.length() == 0 ? deniedMsg : deniedMsg;
    currentState = STATE_DENIED;
    isScrolling = true;
    lastEventTime = millis();
    scrollOffset = 0;
    
    Serial.println("[DENIED] " + currentResidentName);
  }
  
  // Format: INCIDENT:message
  else if (message.startsWith("INCIDENT:")) {
    currentIncidentMsg = message.substring(9);
    currentState = STATE_INCIDENT;
    isScrolling = true;
    lastEventTime = millis();
    scrollOffset = 0;
    
    Serial.println("[INCIDENT] " + currentIncidentMsg);
  }
  
  // Format: PANNE:message
  else if (message.startsWith("PANNE:")) {
    String panneMsg = message.substring(6);
    currentState = STATE_PANNE;
    isScrolling = false;
    lastEventTime = millis();
    scrollOffset = 0;
    
    displayPanne(panneMsg.length() == 0 ? "Maintenance requise" : panneMsg);
    Serial.println("[PANNE] " + panneMsg);
  }
  
  // Return to idle
  else if (message.startsWith("WELCOME")) {
    currentState = STATE_IDLE;
    isScrolling = false;
    scrollOffset = 0;
    deniedCount = 0;
    displayIdle();
  }
}

// ============ SEND MESSAGE TO QT APP ============
void sendToQtApp(String message) {
  Serial.println(message);
}
