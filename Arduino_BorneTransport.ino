#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// =======================
// LCD I2C
// =======================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =======================
// CLAVIER MATRICIEL 4x4
// =======================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Pins du clavier
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String code = "";
bool firstCharOK = false;

void setup() {
  Serial.begin(9600);
  
  // Attendre que le port série soit prêt
  while (!Serial) {
    delay(10);
  }

  // Init LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BORNE TRANSPORT");
  lcd.setCursor(0,1);
  lcd.print("Pret...");
  
  // Message initial pour Qt
  Serial.println("STATUS:Borne transport prete");
  Serial.println("DEBUG:Arduino demarre");
  
  delay(2000);
  lcd.clear();
  lcd.print("Tapez code:");
  
  Serial.println("DEBUG:Pret a recevoir codes");
}

void loop() {
  // Toujours vérifier les messages Qt en premier
  checkQtMessages();
  
  char key = keypad.getKey();
  if (!key) return;

  // Debug: afficher chaque touche pressée
  Serial.print("DEBUG:Touche pressee = ");
  Serial.println(key);
  
  lcd.setCursor(15, 0);  // Coin supérieur droit
  lcd.print(key);        // Afficher la touche sur LCD

  // Effacer
  if (key == '*') {
    code = "";
    firstCharOK = false;
    lcd.clear();
    lcd.print("Efface");
    delay(300);
    lcd.clear();
    lcd.print("Tapez code:");
    return;
  }

  // Valider
  if (key == '#') {
    if (code.length() < 2 || !firstCharOK) {
      lcd.clear();
      lcd.print("Code invalide!");
      delay(800);
      lcd.clear();
      lcd.print("Tapez code:");
      code = "";
      firstCharOK = false;
      return;
    }

    // Envoi direct pour ajout automatique en finance
    lcd.clear();
    lcd.print("Validation...");
    lcd.setCursor(0,1);
    lcd.print("Envoi a Qt...");
    
    // Format attendu par FinancesPage.cpp pour ajout automatique
    Serial.print("VEHICLE_CODE:");
    Serial.print(code);
    Serial.println(":5.00");  // Code + montant automatique
    
    // Debug
    Serial.println("DEBUG:Transaction automatique envoyee");
    Serial.println("DEBUG:Code=" + code + " Montant=5.00");
    
    code = "";
    firstCharOK = false;
    return;
  }

  // Première lettre A/B/C/D
  if (code.length() == 0) {
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      firstCharOK = true;
      code += key;
      Serial.print("DEBUG:Premiere lettre OK = ");
      Serial.println(key);
      lcd.clear();
      lcd.print("Code: ");
      lcd.print(code);
    } else {
      Serial.print("DEBUG:Lettre invalide = ");
      Serial.println(key);
      lcd.clear();
      lcd.print("1ere lettre ?");
      delay(600);
      lcd.clear();
      lcd.print("A B C D seulement");
      delay(600);
      lcd.clear();
      lcd.print("Tapez code:");
    }
    return;
  }

  // Après la lettre → chiffres uniquement
  if (isdigit(key)) {
    code += key;
    Serial.print("DEBUG:Chiffre ajoute = ");
    Serial.print(key);
    Serial.print(" Code = ");
    Serial.println(code);
    lcd.clear();
    lcd.print("Code: ");
    lcd.print(code);
  }

  // Lecture messages de Qt - toujours vérifier
  checkQtMessages();
}

// Fonction séparée pour lire les messages Qt
void checkQtMessages() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    Serial.print("DEBUG:Message recu de QT = ");
    Serial.println(command);
    
    if (command.startsWith("SUCCESS:")) {
      // Extraire ID et MONTANT TOTAL du message
      // Format: SUCCESS:ID:B888 TOTAL:15.00DT
      int idPos = command.indexOf("ID:");
      int totalPos = command.indexOf("TOTAL:");
      
      String vehiculeID = "";
      String montantTotal = "";
      
      if (idPos != -1 && totalPos != -1) {
        vehiculeID = command.substring(idPos + 3, totalPos - 1);
        vehiculeID.trim();
        montantTotal = command.substring(totalPos + 6);
        montantTotal.trim();
      }
      
      // Afficher le succès
      lcd.clear();
      lcd.print("* PAIEMENT OK *");
      delay(1500);
      
      // Afficher ID et montant total
      lcd.clear();
      lcd.print("ID: ");
      lcd.print(vehiculeID);
      lcd.setCursor(0,1);
      lcd.print("Total: ");
      lcd.print(montantTotal);
      delay(3000);
      
      lcd.clear();
      lcd.print("Tapez code:");
    }
    else if (command.startsWith("ERROR:")) {
      lcd.clear();
      
      // Vérifier le type d'erreur
      if (command.indexOf("CODE INEXISTANT") != -1) {
        lcd.print("*** ERREUR ***");
        lcd.setCursor(0,1);
        lcd.print("Code inexistant");
      } else {
        lcd.print("*** ERREUR ***");
        lcd.setCursor(0,1);
        lcd.print("Erreur systeme");
      }
      
      delay(2500);
      lcd.clear();
      lcd.print("Tapez code:");
    }
    else if (command.startsWith("VEHICLE_ADDED:")) {
      lcd.clear();
      lcd.print("Nouveau vehicule");
      lcd.setCursor(0,1);
      lcd.print("+ Paiement OK");
      delay(2000);
      lcd.clear();
      lcd.print("Tapez code:");
    }
    else if (command.startsWith("QT_CONNECTED")) {
      Serial.println("DEBUG:QT connexion confirmee");
      lcd.clear();
      lcd.print("QT CONNECTE!");
      delay(1000);
      lcd.clear();
      lcd.print("Tapez code:");
    }
  }
}
