-- Script pour ajouter la colonne RFID_UID et des données de test
-- À exécuter dans Oracle SQL Developer

-- 1. Ajouter la colonne RFID_UID si elle n'existe pas déjà
ALTER TABLE RESIDENTS ADD (RFID_UID VARCHAR2(20));

-- 2. Ajouter des données RFID de test pour les résidents existants
-- UID: 8F6720C5 pour le résident CIN: 12345678 (approuvé)
UPDATE RESIDENTS SET RFID_UID = '8F6720C5' WHERE CIN = '12345678';

-- 3. Ajouter un commentaire sur la colonne
COMMENT ON COLUMN RESIDENTS.RFID_UID IS 'Identifiant unique de la carte RFID du résident';

-- 4. Créer un index pour améliorer les performances de recherche
CREATE INDEX IDX_RESIDENTS_RFID_UID ON RESIDENTS(RFID_UID);

-- 5. Afficher les résidents avec leur UID RFID
SELECT CIN, NOM, PRENOM, APPARTEMENT, RFID_UID 
FROM RESIDENTS 
WHERE RFID_UID IS NOT NULL;

COMMIT;