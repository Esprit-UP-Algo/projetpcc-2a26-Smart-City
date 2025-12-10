# GUIDE DE DÉMARRAGE RAPIDE - SYSTÈME RFID

## ⚡ Commandes Essentielles

### 1️⃣ Tester l'Accès Autorisé (2 secondes)
```powershell
powershell -ExecutionPolicy Bypass -File ".\simulate_rfid.ps1" -scenario approved
```
✅ Résultat attendu: Message "[OK] Test terminé - Accès devrait être accordé"

### 2️⃣ Tester la Détection d'Intrusion (15 secondes)
```powershell
powershell -ExecutionPolicy Bypass -File ".\simulate_rfid.ps1" -scenario denied
```
✅ Résultat attendu: "Un incident de sécurité devrait être créé" après 3 tentatives

### 3️⃣ Vérifier l'Incident en Base de Données
```sql
-- Dans SQL*Plus ou SQL Developer
SELECT ID_INCIDENT, TYPE_INCIDENT, NIVEAU_GRAVITE, DATE_INCIDENT 
FROM INCIDENTS 
WHERE TYPE_INCIDENT LIKE '%accès%' 
ORDER BY DATE_INCIDENT DESC 
FETCH FIRST 1 ROWS ONLY;
```

### 4️⃣ Afficher l'Interface de Test
```powershell
powershell -ExecutionPolicy Bypass -File ".\test_rfid_manual.ps1"
```

---

## 🐛 DÉPANNAGE RAPIDE

### Problème: "Les messages ne sont pas reçus"
**Solution:**
1. Vérifier Arduino branché sur COM3: `Get-PnpDevice -FriendlyName "*COM*"`
2. Relancer l'application
3. Vérifier Arduino n'est pas utilisé par autre application

### Problème: "L'incident n'est pas créé"
**Solution:**
1. Vérifier Oracle connectée: Aller à "Admin BD" dans l'app
2. Vérifier table RESIDENTS existe: `SELECT COUNT(*) FROM RESIDENTS;`
3. Vérifier colonne RFID_UID existe: `SELECT RFID_UID FROM RESIDENTS WHERE ROWNUM = 1;`
4. Vérifier que 3 tentatives sont bien envoyées (attendre complètement)

### Problème: "Erreur Oracle ORA-00932"
**Solution:** Récompiler le projet
```bash
cd projetpcc-2a26-Smart-City-qtadmin
mingw32-make.exe clean
mingw32-make.exe debug
```

### Problème: "Le rôle utilisateur ne s'affiche pas"
**Solution:**
1. Vérifier que l'utilisateur a un rôle défini en base
2. Relancer l'application
3. Aller à "Paramètres" → voir le rôle dans le champ "Rôle"

---

## 📊 MONITORING

### Voir les Messages en Temps Réel
Ouvrir la console Qt Creator:
- Menu: Debug → Fenêtre de sortie
- Chercher les messages: `[RFID]` ou `[SÉCURITÉ]`

### Voir les Incidents Créés
```sql
SELECT * FROM INCIDENTS 
WHERE DATE_INCIDENT > SYSDATE - 1/24  -- Dernière heure
ORDER BY DATE_INCIDENT DESC;
```

### Compteur des Incidents par Type
```sql
SELECT TYPE_INCIDENT, COUNT(*) as NB, AVG(NIVEAU_GRAVITE) as GRAVITE_MOY
FROM INCIDENTS
WHERE DATE_INCIDENT > SYSDATE - 7  -- 7 derniers jours
GROUP BY TYPE_INCIDENT
ORDER BY NB DESC;
```

---

## 🎯 CHECKLIST DE MISE EN PRODUCTION

- [ ] Arduino branché sur COM3
- [ ] Application compilée en mode Debug
- [ ] Oracle 11g+ connectée et accessible
- [ ] Table INCIDENTS existe
- [ ] Table RESIDENTS a colonne RFID_UID
- [ ] Données de test dans RESIDENTS (CIN 12345678 avec UID 8F6720C5)
- [ ] Test "approved" réussit
- [ ] Test "denied" crée un incident après 3 tentatives
- [ ] Vérification en base de données réussit
- [ ] Rôle utilisateur s'affiche correctement

---

## 📱 FICHIERS DE SUPPORT

| Fichier | Utilité |
|---------|---------|
| `simulate_rfid.ps1` | Envoyer messages RFID par COM3 |
| `test_rfid_manual.ps1` | Afficher guide complet |
| `verify_incidents.sql` | Vérifier incidents en BD |
| `RAPPORT_RFID.md` | Documentation complète |
| `README_RFID.txt` | CE FICHIER |

---

## 🔗 RESSOURCES

- **Communication Arduino:** Arduino.cpp/h
- **Logique Sécurité:** IncidentsPage.cpp/h
- **Accès Base:** DatabaseManager.cpp
- **Configuration:** config.h (si nécessaire)

---

**Créé:** 08/12/2025  
**Version:** 1.0  
**Support:** Voir RAPPORT_RFID.md pour documentation complète
