#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("TEST CLAVIER");
  
  Serial.println("=== TEST CLAVIER DEBUT ===");
}

void loop() {
  char key = keypad.getKey();
  
  if (key) {
    Serial.print("TOUCHE: ");
    Serial.println(key);
    
    lcd.setCursor(0, 1);
    lcd.print("Touche: ");
    lcd.print(key);
    delay(500);
    lcd.setCursor(8, 1);
    lcd.print("        ");  // Effacer
  }
}