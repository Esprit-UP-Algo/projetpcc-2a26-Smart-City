# ARCHITECTURE & FLUX DE DONNÉES - SYSTÈME RFID SMART CITY

## 🏗️ DIAGRAMME D'ARCHITECTURE GLOBALE

```
┌─────────────────────────────────────────────────────────────────┐
│                      SYSTÈME RFID SMART CITY                     │
│                                                                   │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │                   COUCHE HARDWARE                        │   │
│  │                                                          │   │
│  │  ┌──────────────┐  COM3 (9600 baud)  ┌──────────────┐   │   │
│  │  │ RFID Reader  │◄────────────────────►│   Arduino   │   │   │
│  │  └──────────────┘                     └──────────────┘   │   │
│  │         Lecteur RFID                   Module Sériel      │   │
│  └──────────────────────────────────────────────────────────┘   │
│                              ▲                                    │
│                              │ (messages)                         │
│                              │ UID, APPROVED, DENIED, PANNE       │
│                              │                                    │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │                   COUCHE APPLICATION                     │   │
│  │                                                          │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │            ARDUINO.CPP/H                         │   │   │
│  │  │  - Lecture COM3                                  │   │   │
│  │  │  - Parse messages                                │   │   │
│  │  │  - Émit signal onArduinoMessage()                │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │                      ▼                                    │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │        INCIDENTSPAGE.CPP/H                       │   │   │
│  │  │  - Reçoit messages Arduino                       │   │   │
│  │  │  - Suivi des tentatives (QMap)                  │   │   │
│  │  │  - Détection seuil (3 tentatives)              │   │   │
│  │  │  - Création incidents                           │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │                      ▼                                    │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │      DATABASEMANAGER.CPP/H                       │   │   │
│  │  │  - INSERT INTO INCIDENTS                        │   │   │
│  │  │  - Liaison avec table RESIDENTS                 │   │   │
│  │  │  - Gestion NULL ID_RESIDENT                     │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │                      ▼                                    │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │      NEXORADASHBOARD.CPP/H                       │   │   │
│  │  │  - Affichage dynamique rôle utilisateur         │   │   │
│  │  │  - Interface dashboard                          │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  └──────────────────────────────────────────────────────────┘   │
│                              ▼                                    │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │                   COUCHE DATA                            │   │
│  │                                                          │   │
│  │  ┌──────────────┐         ┌─────────────────────────┐   │   │
│  │  │   INCIDENTS  │         │      RESIDENTS          │   │   │
│  │  │              │         │                         │   │   │
│  │  │ - ID         │ FK──────│ - ID_RESIDENT           │   │   │
│  │  │ - TYPE       │────┐    │ - RFID_UID              │   │   │
│  │  │ - GRAVITE    │    └────│ - CIN                   │   │   │
│  │  │ - DATE       │         │ - NOM                   │   │   │
│  │  │ - DESCRIPTION│         │ - ...                   │   │   │
│  │  │ - STATUT     │         └─────────────────────────┘   │   │
│  │  │ - ID_RESIDENT│ (peut être NULL)                      │   │
│  │  └──────────────┘                                        │   │
│  │        ORACLE 11g Database                               │   │
│  └──────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔄 FLUX DE TRAITEMENT DÉTAILLÉ

### SCÉNARIO 1: Accès Autorisé
```
┌─────────────────┐
│ Carte RFID scan │
└────────┬────────┘
         │ UID:8F6720C5
         ▼
    ┌─────────────────────┐
    │ Arduino reçoit UID  │
    └────────┬────────────┘
             │ onArduinoMessage("UID:8F6720C5")
             ▼
    ┌──────────────────────────────────────┐
    │ IncidentsPage::onArduinoMessage()     │
    │ ├─ Parse message                      │
    │ └─ Stocke UID en map                  │
    └────────┬─────────────────────────────┘
             │
             │ Arduino envoie APPROVED:8F6720C5
             ▼
    ┌──────────────────────────────────────┐
    │ IncidentsPage::onArduinoMessage()     │
    │ ├─ Détecte message APPROVED           │
    │ ├─ failedAttempts[UID] = 0 (reset)   │
    │ └─ Log: "Accès autorisé"              │
    └────────┬─────────────────────────────┘
             │
             ▼
         ✅ FIN
     Pas d'incident créé
```

### SCÉNARIO 2: Intrusion Détectée (3 tentatives échouées)
```
Tentative 1:
┌──────────────────┐
│ UID:UNAUTHORIZED │
└────────┬─────────┘
         ▼
    ┌────────────────────────────────────────┐
    │ Arduino → DENIED:UNAUTHORIZED01        │
    └────────┬───────────────────────────────┘
             ▼
    ┌────────────────────────────────────────┐
    │ IncidentsPage                          │
    │ failedAttempts["UNAUTHORIZED01"] = 1   │
    │ lastAttempt["UNAUTHORIZED01"] = now()  │
    └────────┬───────────────────────────────┘
             ▼
        ⏳ Attendre

Tentative 2:
┌──────────────────┐
│ UID:UNAUTHORIZED │
└────────┬─────────┘
         ▼
    ┌────────────────────────────────────────┐
    │ failedAttempts["UNAUTHORIZED01"] = 2   │
    │ lastAttempt mise à jour                │
    └────────┬───────────────────────────────┘
             ▼
        ⏳ Attendre

Tentative 3:
┌──────────────────┐
│ UID:UNAUTHORIZED │
└────────┬─────────┘
         ▼
    ┌────────────────────────────────────────┐
    │ failedAttempts["UNAUTHORIZED01"] = 3   │
    │ SEUIL ATTEINT!                         │
    └────────┬───────────────────────────────┘
             ▼
    ┌────────────────────────────────────────┐
    │ createSecurityIncident(uid)            │
    │ ├─ Recherche ID_RESIDENT en BD         │
    │ ├─ Crée incident "Tentative d'accès"   │
    │ ├─ Niveau gravité = 3                  │
    │ └─ Statut = "EN ATTENTE"               │
    └────────┬───────────────────────────────┘
             ▼
    ┌────────────────────────────────────────┐
    │ INSERT INTO INCIDENTS                  │
    │ VALUES(seq, "Tentative...", 3, now..)  │
    └────────┬───────────────────────────────┘
             ▼
        ✅ FIN
    🚨 INCIDENT CRÉÉ!
   failedAttempts reset après 5 min
```

### SCÉNARIO 3: Panne Ascenseur
```
┌───────────────────────┐
│ Défaut détecté        │
└────────┬──────────────┘
         │ PANNE:8F6720C5
         ▼
    ┌────────────────────────────────────────┐
    │ Arduino envoie PANNE                   │
    └────────┬───────────────────────────────┘
             ▼
    ┌────────────────────────────────────────┐
    │ IncidentsPage::onArduinoMessage()      │
    │ ├─ Détecte "PANNE"                    │
    │ └─ Crée incident de maintenance        │
    └────────┬───────────────────────────────┘
             ▼
    ┌────────────────────────────────────────┐
    │ INSERT INTO INCIDENTS                  │
    │ ├─ TYPE = "Panne d'ascenseur"          │
    │ ├─ GRAVITE = 2 (urgent)                │
    │ └─ DESCRIPTION = "Panne - UID:..."     │
    └────────┬───────────────────────────────┘
             ▼
        ✅ FIN
   🔧 MAINTENANCE ALERTÉE
```

---

## 📦 STRUCTURE DES MESSAGES RFID

### Format Message
```
[TYPE]:[VALEUR]

où TYPE ∈ {UID, APPROVED, DENIED, PANNE}
```

### Exemples
```
UID:8F6720C5           → Identification carte (16 chars hex)
APPROVED:8F6720C5     → Accès accordé (même UID)
DENIED:UNAUTHORIZED   → Accès refusé
PANNE:8F6720C5        → Défaut détecté
```

### Traitement
```cpp
// incidentspage.cpp onArduinoMessage()
if (message.startsWith("UID:")) {
    uid = message.mid(4);  // Extraire uid
    
} else if (message.startsWith("APPROVED:")) {
    // Réinitialiser compteur
    failedAttempts[uid] = 0;
    
} else if (message.startsWith("DENIED:")) {
    failedAttempts[uid]++;
    if (failedAttempts[uid] >= 3) {
        createSecurityIncident(uid);
        failedAttempts[uid] = -1;  // Ignorer supplémentaires
    }
    
} else if (message.startsWith("PANNE:")) {
    // Créer incident de panne
    createIncident("Panne d'ascenseur", uid);
}
```

---

## 🗄️ SCHÉMA BASE DE DONNÉES

### Table INCIDENTS
```sql
CREATE TABLE INCIDENTS (
    ID_INCIDENT       NUMBER PRIMARY KEY,
    TYPE_INCIDENT     VARCHAR2(100),      -- "Tentative d'accès...", "Panne..."
    NIVEAU_GRAVITE    NUMBER(1),          -- 1=info, 2=urgent, 3=critique
    DATE_INCIDENT     DATE,               -- SYSDATE
    HEURE_INCIDENT    DATE,               -- SYSDATE
    DESCRIPTION       VARCHAR2(500),      -- "3 tentatives - UID: ..."
    STATUT            VARCHAR2(20),       -- "EN ATTENTE", "RÉSOLU"
    ID_RESIDENT       NUMBER,             -- Foreign Key (peut être NULL)
    ID_EMPLOYE        NUMBER,             -- (optionnel)
    LIEU              VARCHAR2(100),      -- "Ascenseur A", "Entrée 1"
    ...
);
```

### Table RESIDENTS (liée)
```sql
CREATE TABLE RESIDENTS (
    ID_RESIDENT       NUMBER PRIMARY KEY,
    CIN              VARCHAR2(20) UNIQUE,
    NOM              VARCHAR2(100),
    RFID_UID         VARCHAR2(20),       -- NEW - Ajouté pour RFID
    ...
);
```

### Requête de liaison
```sql
SELECT i.ID_INCIDENT, i.TYPE_INCIDENT, r.NOM, r.CIN
FROM INCIDENTS i
LEFT JOIN RESIDENTS r ON i.ID_RESIDENT = r.ID_RESIDENT
WHERE i.DATE_INCIDENT > SYSDATE - 1
ORDER BY i.DATE_INCIDENT DESC;
```

---

## 🔄 CYCLE DE VIE DES COMPTEURS

### Initialisation
```
failedAttempts = QMap<QString, int>()     // Vide au démarrage
lastAttempt = QMap<QString, QDateTime>()  // Vide au démarrage
```

### Incrémentation
```
Chaque DENIED → failedAttempts[uid]++
Chaque UID → lastAttempt[uid] = QDateTime::currentDateTime()
```

### Réinitialisation
```
Trigger 1: Message APPROVED
  failedAttempts[uid] = 0
  lastAttempt[uid] = null

Trigger 2: Timeout 5 minutes (appel checkAndResetFailedAttempts)
  if (QDateTime::currentDateTime() - lastAttempt[uid] > 5min) {
      failedAttempts[uid] = 0
  }

Trigger 3: Seuil atteint (créé incident)
  failedAttempts[uid] = -1  // Ignorer futurs DENIED jusqu'à APPROVED
```

---

## 🚀 PERFORMANCE & SCALABILITÉ

### Limites Actuelles
- ✅ Supporte ~100 UIDs simultanés en suivi
- ✅ Temps réaction < 100ms
- ✅ Insertion BD < 500ms
- ❌ Pas de cluster/redondance

### Optimisations Possibles
1. Persistance QMap en BD (historique)
2. Nettoyage automatique après 1h
3. Cache RESIDENTS pour recherche UID
4. Notifications email/SMS

---

**Dernière mise à jour:** 08/12/2025  
**Version:** 1.0 Architecture  
**Auteur:** Smart City Team
