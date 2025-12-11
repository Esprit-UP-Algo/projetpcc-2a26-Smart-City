# Script de Test RFID pour Smart City QTAdmin
# Teste les scenarios de securite du systeme RFID

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Test RFID Smart City QTAdmin" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Configuration
$comPort = "COM3"
$baudRate = 9600

# Messages de test
$testMessages = @(
    @{
        name = "1. Acces AUTORISE"
        messages = @("UID:8F6720C5", "APPROVED:8F6720C5")
        expected = "Acces accorde, aucun incident"
    },
    @{
        name = "2. Tentatives REFUSEES (x3)"
        messages = @("UID:UNAUTHORIZED01", "DENIED:UNAUTHORIZED01", "DENIED:UNAUTHORIZED01", "DENIED:UNAUTHORIZED01")
        expected = "Incident de securite cree (3 tentatives echouees)"
    },
    @{
        name = "3. Panne Ascenseur"
        messages = @("PANNE:8F6720C5")
        expected = "Incident de panne cree"
    }
)

Write-Host "SCENARIOS DE TEST:" -ForegroundColor Yellow
Write-Host ""

foreach ($scenario in $testMessages) {
    Write-Host "[*] $($scenario.name)" -ForegroundColor Green
    Write-Host "    Messages:" -ForegroundColor Gray
    foreach ($msg in $scenario.messages) {
        Write-Host "    -> $msg" -ForegroundColor Cyan
    }
    Write-Host "    Resultat attendu: $($scenario.expected)" -ForegroundColor Yellow
    Write-Host ""
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "INSTRUCTIONS POUR TESTER:" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Option 1 - Avec lecteur RFID physique:" -ForegroundColor Yellow
Write-Host "  1. Connecter le lecteur RFID sur COM3"
Write-Host "  2. Lancer l'application QTAdmin"
Write-Host "  3. Aller à la page Incidents"
Write-Host "  4. Utiliser le lecteur RFID"
Write-Host ""

Write-Host "Option 2 - Simulation logicielle:" -ForegroundColor Yellow
Write-Host "  1. Ouvrir une connexion serie vers COM3 a 9600 baud"
Write-Host "  2. Envoyer les messages de test ci-dessus"
Write-Host "  3. Observer les logs dans la console"
Write-Host "  4. Verifier la creation des incidents en base de donnees"
Write-Host ""

Write-Host "VERIFICATION EN BASE DE DONNEES:" -ForegroundColor Yellow
Write-Host "  SELECT * FROM INCIDENTS WHERE DATE_INCIDENT > SYSDATE-1" -ForegroundColor Cyan
Write-Host "  Doit afficher les incidents de securite crees automatiquement"
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Notes importantes:" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "[OK] Chaque tentative refusee incremente le compteur" -ForegroundColor Green
Write-Host "[OK] Apres 3 tentatives refusees = Incident cree" -ForegroundColor Green
Write-Host "[OK] Compteur reinitialise apres 5 minutes d'inactivite" -ForegroundColor Green
Write-Host "[OK] Message APPROVED reinitialise le compteur immediatement" -ForegroundColor Green
Write-Host ""
