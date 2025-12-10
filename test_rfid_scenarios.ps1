# Script PowerShell pour tester le système de détection d'incidents RFID
# À utiliser avec l'application Smart City

Write-Host "=== TEST SYSTÈME RFID SMART CITY ===" -ForegroundColor Cyan
Write-Host "Ce script simule les scénarios de test suivants:" -ForegroundColor Yellow
Write-Host "1. Carte autorisée (UID: 8F6720C5 - CIN: 12345678) → APPROVED" -ForegroundColor Green
Write-Host "2. Carte non autorisée (UID: XXXXXXXX) → 3x DENIED → Incident automatique" -ForegroundColor Red
Write-Host ""

Write-Host "INSTRUCTIONS:" -ForegroundColor Yellow
Write-Host "1. Démarrez l'application Smart City Qt"
Write-Host "2. Allez sur la page 'Incidents'"
Write-Host "3. Dans l'application, cliquez sur 'Simuler' et entrez ces messages:"
Write-Host ""

Write-Host "=== SCÉNARIO 1: ACCÈS AUTORISÉ ===" -ForegroundColor Green
Write-Host "   UID:8F6720C5"
Write-Host "   APPROVED:8F6720C5"
Write-Host ""

Write-Host "=== SCÉNARIO 2: TENTATIVES NON AUTORISÉES ===" -ForegroundColor Red
Write-Host "   UID:12345ABC"
Write-Host "   DENIED:12345ABC"
Write-Host "   DENIED:12345ABC" 
Write-Host "   DENIED:12345ABC"
Write-Host "   → Un incident sera automatiquement créé après la 3e tentative" -ForegroundColor Red
Write-Host ""

Write-Host "=== SCÉNARIO 3: PANNE ASCENSEUR ===" -ForegroundColor Orange
Write-Host "   PANNE:8F6720C5"
Write-Host ""

Write-Host "RÉSULTATS ATTENDUS:" -ForegroundColor Cyan
Write-Host "✅ Carte autorisée: Accès accordé, pas d'incident"
Write-Host "❌ 3 tentatives refusées: Incident de sécurité automatique créé"
Write-Host "🔧 Panne: Incident de panne d'ascenseur créé"
Write-Host ""

Write-Host "Pour simuler dans l'application:" -ForegroundColor Yellow
Write-Host "- Utilisez arduino->simulateReceived(QString) dans le code"
Write-Host "- Ou ajoutez un bouton de test dans l'interface"

Read-Host "Appuyez sur Entrée pour continuer"