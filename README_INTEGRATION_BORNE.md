# INTÉGRATION BORNE TRANSPORT - SYSTÈME ARDUINO DOUBLE

## 📋 Vue d'ensemble

Le système intègre maintenant **2 Arduinos** fonctionnant simultanément :

### Arduino 1 - RFID + OLED (COM3)
- **Fichier** : `SmartCity_OLED.ino`
- **Port** : COM3
- **Fonction** : Contrôle d'accès par carte RFID avec affichage OLED
- **Composants** :
  - MFRC522 RFID (SPI)
  - SH1106 OLED 128x64 (I2C)
- **Page Qt** : IncidentsPage

### Arduino 2 - Borne Transport (COM5)  
- **Fichier** : `Arduino_BorneTransport.ino`
- **Port** : COM5
- **Fonction** : Borne de paiement transport avec clavier
- **Composants** :
  - Clavier matriciel 4x4
  - LCD I2C 16x2 (0x27)
- **Page Qt** : FinancesPage

---

## 🔌 Branchements Arduino Borne Transport

### LCD I2C 16x2
- VCC → 5V
- GND → GND
- SDA → A4
- SCL → A5

### Clavier Matriciel 4x4
- **Lignes** : Pins 9, 8, 7, 6
- **Colonnes** : Pins 5, 4, 3, 2

---

## 💻 Code Qt - Nouveaux Fichiers

### ArduinoTransport (nouvelle classe)
**Fichiers** : `arduinotransport.h` / `arduinotransport.cpp`

Gère la communication série avec la borne transport (COM5).

**Méthodes principales** :
- `connectArduino(QString portName)` - Connexion COM5
- `sendSuccess(QString vehicleID, QString montantTotal)` - Succès paiement
- `sendError(QString errorMsg)` - Erreur
- `vehicleCodeReceived(QString code, QString montant)` - Signal émis quand code reçu

### DatabaseManager - Nouvelles méthodes
**Fichiers** : `databasemanager.h` / `databasemanager.cpp`

```cpp
QVariantMap getVehiculeByCode(const QString &code);
double getMontantTotalVehicule(const QString &code);
```

### FinancesPage - Intégration borne
**Fichiers** : `financespage.h` / `financespage.cpp`

**Nouveau membre** :
```cpp
ArduinoTransport *arduinoTransport;
```

**Nouveau slot** :
```cpp
void onVehicleCodeReceived(QString code, QString montant);
```

**Logique** :
1. Reçoit `VEHICLE_CODE:B888:5.00` depuis Arduino
2. Cherche le véhicule dans table `TRANSPORT`
3. Crée automatiquement une transaction
4. Calcule le montant total payé
5. Renvoie `SUCCESS:ID:B888 TOTAL:15.00DT` ou `ERROR:...`

---

## 📡 Protocole de Communication

### Arduino → Qt

| Message | Format | Exemple |
|---------|--------|---------|
| Code véhicule | `VEHICLE_CODE:CODE:MONTANT` | `VEHICLE_CODE:B888:5.00` |
| Debug | `DEBUG:message` | `DEBUG:Touche pressee = 5` |
| Status | `STATUS:message` | `STATUS:Borne transport prete` |

### Qt → Arduino

| Message | Format | Exemple |
|---------|--------|---------|
| Succès | `SUCCESS:ID:CODE TOTAL:MONTANT` | `SUCCESS:ID:B888 TOTAL:15.00DT` |
| Erreur | `ERROR:message` | `ERROR:CODE INEXISTANT - Verifiez le code` |
| Connexion | `QT_CONNECTED` | `QT_CONNECTED` |

---

## 🎯 Utilisation de la Borne

### Saisie d'un code véhicule

1. **Format** : 1 lettre (A/B/C/D) + chiffres
   - Exemples valides : `A123`, `B888`, `C45`, `D7890`

2. **Touches spéciales** :
   - `*` : Effacer
   - `#` : Valider

3. **Séquence** :
   ```
   Utilisateur tape : B 8 8 8 #
   LCD affiche : "Code: B888"
   Arduino envoie : VEHICLE_CODE:B888:5.00
   Qt traite → vérifie véhicule → crée transaction
   Qt répond : SUCCESS:ID:B888 TOTAL:15.00DT
   LCD affiche : "* PAIEMENT OK *"
                 "ID: B888"
                 "Total: 15.00DT"
   ```

---

## 🗄️ Base de Données

### Table TRANSPORT
```sql
CODE_UNIQUE VARCHAR2(50)  -- Cherché par borne (ex: B888)
TYPE VARCHAR2(100)
MARQUE VARCHAR2(100)
MODELE VARCHAR2(100)
IMMATRICULATION VARCHAR2(50)
STATUT VARCHAR2(50)
DATE_AJOUT DATE
DERNIERE_MAINTENANCE DATE
CIN_RESIDENT VARCHAR2(20)
```

### Table TRANSACTIONS
```sql
CODE VARCHAR2(50)         -- TRX_B888_20251208202247
MONTANT NUMBER(10, 2)     -- 5.00
TYPE VARCHAR2(100)        -- "Transport"
DATE DATE
CIN_RESIDENT VARCHAR2(20)
DESCRIPTION VARCHAR2(500) -- "Paiement automatique borne transport - Véhicule: B888"
```

---

## ⚙️ Configuration

### Compilation Qt
Fichier `Qtadmin.pro` modifié :
```qmake
SOURCES += \
    arduinotransport.cpp \
    ...

HEADERS += \
    arduinotransport.h \
    ...
```

### Ports série
- **COM3** : Arduino RFID (IncidentsPage)
- **COM5** : Arduino Borne Transport (FinancesPage)

---

## 🔧 Dépannage

### Erreur "Accès refusé COM5"
1. Fermez Arduino IDE si ouvert
2. Fermez le moniteur série
3. Relancez Qtadmin.exe

### Code véhicule non reconnu
- Vérifier que le code existe dans table `TRANSPORT` (colonne `CODE_UNIQUE`)
- Format : 1 lettre + chiffres (A123, B888, etc.)

### LCD ne s'allume pas
- Vérifier adresse I2C : `0x27` (tester avec `I2C_scanner.ino`)
- Vérifier branchements SDA/SCL sur A4/A5

### Clavier ne répond pas
- Vérifier branchements pins 2-9
- Tester avec code simple : `char key = keypad.getKey(); Serial.println(key);`

---

## ✅ Tests de Validation

### Test 1 : Connexion
```
1. Téléverser Arduino_BorneTransport.ino sur Arduino
2. Lancer Qtadmin.exe
3. Ouvrir FinancesPage
4. LCD doit afficher : "QT CONNECTE!"
```

### Test 2 : Code valide
```
1. Créer un véhicule avec code "B888" dans TransportPage
2. Sur la borne, taper : B 8 8 8 #
3. LCD doit afficher : "* PAIEMENT OK *" puis "Total: 5.00DT"
4. Vérifier dans FinancesPage qu'une transaction est créée
```

### Test 3 : Code invalide
```
1. Sur la borne, taper : Z 9 9 9 #
2. LCD doit afficher : "*** ERREUR *** Code inexistant"
```

---

## 📊 Architecture Système

```
┌─────────────────────────────────────────────────────────┐
│                    Qt Application                       │
│                                                         │
│  ┌─────────────────┐        ┌─────────────────┐       │
│  │ IncidentsPage   │        │  FinancesPage   │       │
│  │  - Arduino      │        │  - Arduino      │       │
│  │    (COM3)       │        │    Transport    │       │
│  │  - RFID         │        │    (COM5)       │       │
│  └────────┬────────┘        └────────┬────────┘       │
│           │                          │                 │
│           │      ┌───────────────────┤                 │
│           │      │                   │                 │
│  ┌────────▼──────▼──────┐   ┌───────▼────────┐       │
│  │   DatabaseManager    │   │  ArduinoTransport│      │
│  │   - getVehiculeBy... │   │  - connect...    │      │
│  │   - getMontantTotal  │   │  - sendSuccess   │      │
│  └──────────────────────┘   └──────────────────┘       │
└─────────────────────────────────────────────────────────┘
            │                          │
            │                          │
            ▼                          ▼
   ┌────────────────┐        ┌────────────────┐
   │ Arduino RFID   │        │ Arduino Borne  │
   │ (COM3)         │        │ (COM5)         │
   │ - MFRC522      │        │ - Clavier 4x4  │
   │ - OLED SH1106  │        │ - LCD I2C      │
   └────────────────┘        └────────────────┘
```

---

## 📝 Notes Importantes

1. **Pas de conflit** : Les deux Arduinos fonctionnent indépendamment (ports différents)
2. **Transaction automatique** : Aucune saisie manuelle dans Qt - tout géré par la borne
3. **Montant fixe** : 5.00 DT par défaut (modifiable dans Arduino : ligne `Serial.println(":5.00")`)
4. **Montant total cumulé** : Qt calcule la somme de tous les paiements pour un véhicule
5. **Type transaction** : Automatiquement "Transport"

---

## 🚀 Prochaines Étapes Possibles

- [ ] Ajouter montant variable sur borne (saisie manuelle)
- [ ] Historique des paiements sur LCD
- [ ] Support NFC en plus du clavier
- [ ] Reçu imprimé avec imprimante thermique
- [ ] Statistiques temps réel sur borne

---

**Date d'intégration** : 8 décembre 2025  
**Version** : 1.0  
**Développeur** : Système Smart City Qt Admin
