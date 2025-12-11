# RAPPORT DE MISE EN PLACE - SYSTÈME RFID SMART CITY
## Version 1.0 - Décembre 2025

---

## 📋 RÉSUMÉ EXÉCUTIF

Un système de sécurité RFID complet a été intégré dans l'application Qt QTAdmin pour la gestion des accès d'ascenseur et la détection automatique des tentatives d'accès non autorisées.

### Objectifs Réalisés ✅
1. **Lecture RFID via Arduino** - Communication série COM3 pour capturer les UIDs
2. **Suivi des tentatives** - Comptage automatique des accès refusés par UID
3. **Création automatique d'incidents** - Après 3 tentatives échouées en 5 minutes
4. **Intégration Oracle** - Sauvegarde dans table INCIDENTS avec tous les détails
5. **Affichage des rôles utilisateurs** - Correction de l'interface pour afficher le rôle réel de l'utilisateur connecté

---

## 🏗️ ARCHITECTURE TECHNIQUE

### Composants Principaux

#### 1. **Arduino.cpp/h** - Communication Hardware
- Port: COM3, Vitesse: 9600 baud
- Types de messages:
  - `UID:<value>` - Identification de carte RFID
  - `APPROVED:<value>` - Accès accordé
  - `DENIED:<value>` - Accès refusé
  - `PANNE:<value>` - Défaut ascenseur
- Signal Qt: `onArduinoMessage(QString msg)`

#### 2. **IncidentsPage.cpp/h** - Logique de Sécurité
**Nouveaux membres (incidentspage.h):**
```cpp
QMap<QString, int> failedAttempts;           // Compteur tentatives par UID
QMap<QString, QDateTime> lastAttempt;        // Horodatage dernière tentative
static const int MAX_FAILED_ATTEMPTS = 3;    // Seuil avant incident
static const int RESET_INTERVAL_MINUTES = 5; // Réinitialisation après
```

**Nouvelles méthodes:**
```cpp
void checkAndResetFailedAttempts(const QString &uid);
void createSecurityIncident(const QString &uid);
```

**Logique onArduinoMessage():**
- UID reçu → stocké dans map
- APPROVED → compteur réinitialisé, log OK
- DENIED → compteur incrémenté
  - Si compte = 3 → `createSecurityIncident()`
  - Si compte > 3 → ignoré jusqu'à réinitialisation
- PANNE → incident de panne automatique

#### 3. **DatabaseManager.cpp** - Accès Oracle
**Correction critique:**
- Méthode: `addIncident()`
- **Problème initial:** Envoi de `QVariant()` = BINARY au lieu de NUMBER
- **Solution:** Binding conditionnel pour NULL ID_RESIDENT
```cpp
if (idResident == -1) {
    q.bindValue(":idResident", QVariant(QVariant::Int)); // NULL
} else {
    q.bindValue(":idResident", idResident);
}
```

#### 4. **NexoraDashboard.cpp/h** - Affichage Utilisateur
**Correction:** 
- Méthode `updateUserDisplayInfo()` dynamique
- HTML formaté avec rôle et description
- Utilise `m_userRole` de UserRole enum
- 5 types de rôles supportés

#### 5. **SettingsPage.cpp** - Configuration Utilisateur
- `loadUserData()` popule `roleLineEdit`
- Affichage du rôle réel de l'utilisateur connecté

---

## 🔄 FLUX DE FONCTIONNEMENT

### Scénario 1: Accès Autorisé ✅
```
[RFID Reader] → "UID:8F6720C5"
↓
Arduino détecte carte valide
↓
[RFID Reader] → "APPROVED:8F6720C5"
↓
IncidentsPage reçoit message
↓
failedAttempts["8F6720C5"] = 0 (réinitialisation)
↓
✅ Accès accordé - Pas d'incident
```

### Scénario 2: Tentatives Refusées (Alerte de Sécurité) ⚠️
```
[RFID Reader] → "UID:UNAUTHORIZED01"
↓
[RFID Reader] → "DENIED:UNAUTHORIZED01" [x3]
↓
Tentative 1: failedAttempts["UNAUTHORIZED01"] = 1
Tentative 2: failedAttempts["UNAUTHORIZED01"] = 2
Tentative 3: failedAttempts["UNAUTHORIZED01"] = 3
↓
Seuil atteint! → createSecurityIncident("UNAUTHORIZED01")
↓
INSERT INTO INCIDENTS:
- TYPE: "Tentative d'accès non autorisé"
- NIVEAU_GRAVITE: 3
- DESCRIPTION: "3 tentatives échouées - UID: UNAUTHORIZED01"
- DATE: SYSDATE
↓
🚨 Incident créé automatiquement
```

### Scénario 3: Panne Ascenseur 🔧
```
[RFID Reader] → "PANNE:8F6720C5"
↓
IncidentsPage détecte message PANNE
↓
INSERT INTO INCIDENTS:
- TYPE: "Panne d'ascenseur"
- NIVEAU_GRAVITE: 2
- DESCRIPTION: "Panne ascenseur - UID: 8F6720C5"
↓
🔧 Incident de maintenance créé
```

---

## 📊 STRUCTURE BASE DE DONNÉES

### Table INCIDENTS
```sql
CREATE TABLE INCIDENTS (
    ID_INCIDENT      NUMBER PRIMARY KEY,
    TYPE_INCIDENT    VARCHAR2(100),
    NIVEAU_GRAVITE   NUMBER(1),
    DATE_INCIDENT    DATE,
    DESCRIPTION      VARCHAR2(500),
    STATUT          VARCHAR2(20),
    ID_RESIDENT     NUMBER,  -- Peut être NULL pour incidents sans résidents
    ...
);
```

### Requête de Vérification
```sql
SELECT * FROM INCIDENTS 
WHERE DATE_INCIDENT > TRUNC(SYSDATE) + (SYSDATE - 5/24/60)
ORDER BY DATE_INCIDENT DESC;
```

---

## 🧪 TESTS

### Utilitaires Fournis

#### 1. **test_rfid_manual.ps1**
Affiche les instructions complètes de test avec exemples

#### 2. **simulate_rfid.ps1**
Envoie les messages RFID via COM3
```powershell
# Test accès autorisé
.\simulate_rfid.ps1 -scenario approved

# Test 3 tentatives refusées
.\simulate_rfid.ps1 -scenario denied

# Test panne ascenseur
.\simulate_rfid.ps1 -scenario elevator_failure

# Tous les tests
.\simulate_rfid.ps1 -scenario all
```

#### 3. **verify_incidents.sql**
Lance dans Oracle pour vérifier la création des incidents

---

## 🔍 EXEMPLE DE TEST COMPLET

### Test 1: Vérifier que l'application reçoit les messages
```powershell
# Terminal 1 - Simulation
.\simulate_rfid.ps1 -scenario approved

# Vérification: Check console Qt pour "[RFID] Message reçu: APPROVED:8F6720C5"
```

### Test 2: Vérifier la création automatique d'incidents
```powershell
# Terminal 1 - Simulation des 3 tentatives
.\simulate_rfid.ps1 -scenario denied

# Terminal 2 - Vérification en base
sqlplus user/password@ORCL
> @verify_incidents.sql
```

### Test 3: Vérifier l'affichage du rôle utilisateur
1. Lancer l'application
2. Page "Tableau de bord" → Voir le rôle de l'utilisateur connecté
3. Page "Paramètres" → Voir le rôle dans le champ "Rôle"

---

## ⚙️ CONFIGURATION & DÉPLOIEMENT

### Prérequis
- Qt 6.7.3 avec QtSerialPort
- Arduino/RFID sur COM3, 9600 baud
- Oracle 11g+ avec table INCIDENTS
- Table RESIDENTS avec colonne RFID_UID

### Installation
1. Recompiler le projet
   ```bash
   mingw32-make.exe debug
   ```

2. Vérifier la table RESIDENTS a la colonne RFID_UID
   ```sql
   ALTER TABLE RESIDENTS ADD (RFID_UID VARCHAR2(20));
   ```

3. Lancer l'application
   ```bash
   .\debug\Qtadmin.exe
   ```

4. Aller à la page "Incidents" pour voir la réception des messages

---

## 📝 MODIFICATIONS DE CODE

### Fichiers Modifiés
1. **incidentspage.h** - Ajout QMap et constantes
2. **incidentspage.cpp** - Logique de suivi et création incident
3. **databasemanager.cpp** - Correction type NULL pour Oracle
4. **nexoradashboard.cpp/h** - Affichage dynamique rôle utilisateur
5. **settingspage.cpp** - Chargement rôle utilisateur

### Fichiers Créés
1. **test_rfid_manual.ps1** - Guide de test
2. **simulate_rfid.ps1** - Simulateur RFID
3. **verify_incidents.sql** - Script de vérification

---

## 🔐 SÉCURITÉ

### Améliorations Apportées
✅ Détection automatique des tentatives d'accès non autorisé  
✅ Incrémentation compteur configurable (3 tentatives)  
✅ Timeout automatique (5 minutes)  
✅ Logging complet avec horodatage  
✅ Séparation des responsabilités (Arduino → IncidentsPage → DB)

### Limitations Connues
⚠️ UID non validé contre table RESIDENTS
⚠️ Pas de chiffrement des messages RFID
⚠️ Pas d'alertes temps réel (notification/email)

### Améliorations Futures
- [ ] Validation UID contre base de données
- [ ] Notifications email de sécurité
- [ ] Dashboard de surveillance temps réel
- [ ] Chiffrement communication Arduino

---

## 📞 SUPPORT & DOCUMENTATION

### Logs à Consulter
- Console Qt: Chercher `[RFID]` ou `[SÉCURITÉ]`
- Base de données: Table INCIDENTS
- Fichier Makefile: Configuration compilation

### FAQ

**Q: Les messages RFID ne sont pas reçus?**
A: Vérifier COM3 est libre, Arduino branchée, vitesse 9600 baud

**Q: L'incident n'est pas créé après 3 tentatives?**
A: Vérifier Oracle connectée, table RESIDENTS existe, RFID_UID rempli

**Q: Quel est l'état de la table RESIDENTS?**
A: `SELECT * FROM RESIDENTS WHERE RFID_UID IS NOT NULL;`

---

## ✅ STATUT DE LIVRAISON

| Composant | Statut | Notes |
|-----------|--------|-------|
| Communication Arduino | ✅ Complète | COM3, 9600 baud |
| Suivi tentatives | ✅ Complète | QMap + QDateTime |
| Création incidents | ✅ Complète | Oracle intégré |
| Affichage rôles | ✅ Complète | 5 types supportés |
| Tests | ✅ Fournis | Scripts PowerShell & SQL |
| Documentation | ✅ Complète | Ce fichier |

---

**Version:** 1.0  
**Date:** 08/12/2025  
**Statut:** PRODUCTION  
**Testé sur:** Qt 6.7.3, MinGW 11.2.0, Oracle 11g

---
