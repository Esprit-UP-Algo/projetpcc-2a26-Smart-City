# Utilitaire de simulation RFID pour Smart City QTAdmin
# Envoie des messages de test via la connexion serie COM3

param(
    [string]$scenario = "all",
    [int]$port = 3,
    [int]$baud = 9600
)

# Fonction pour envoyer un message sur le port serie
function Send-SerialMessage {
    param(
        [int]$PortNumber,
        [int]$BaudRate,
        [string]$Message
    )
    
    try {
        $portName = "COM$PortNumber"
        Write-Host "[SEND] $portName : $Message" -ForegroundColor Cyan
        
        $port = New-Object System.IO.Ports.SerialPort $portName, $BaudRate
        $port.Open()
        
        # Envoyer le message avec saut de ligne
        $port.WriteLine($Message)
        
        # Attendre un peu
        Start-Sleep -Milliseconds 200
        
        $port.Close()
        $port.Dispose()
        
        Write-Host "[OK] Message envoye" -ForegroundColor Green
    }
    catch {
        Write-Host "[ERROR] Erreur : $_" -ForegroundColor Red
    }
}

# Scenarios de test
Write-Host ""
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host "Simulateur RFID - Smart City QTAdmin" -ForegroundColor Cyan
Write-Host "=====================================" -ForegroundColor Cyan
Write-Host ""

switch ($scenario.ToLower()) {
    "approved" {
        Write-Host "Test 1: Acces AUTORISE (UID authorise)" -ForegroundColor Yellow
        Write-Host ""
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "UID:8F6720C5"
        Start-Sleep -Seconds 1
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "APPROVED:8F6720C5"
        Write-Host ""
        Write-Host "[OK] Test termine - Acces devrait etre accorde" -ForegroundColor Green
    }
    
    "denied" {
        Write-Host "Test 2: Tentatives REFUSEES (accumulation 3x)" -ForegroundColor Yellow
        Write-Host ""
        for ($i = 1; $i -le 3; $i++) {
            Write-Host "  Tentative $i..." -ForegroundColor Yellow
            Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "UID:UNAUTHORIZED01"
            Start-Sleep -Seconds 0.5
            Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "DENIED:UNAUTHORIZED01"
            Start-Sleep -Seconds 1
        }
        Write-Host ""
        Write-Host "[OK] Test termine - Un incident de securite devrait etre cree" -ForegroundColor Green
    }
    
    "elevator_failure" {
        Write-Host "Test 3: Panne d'ascenseur" -ForegroundColor Yellow
        Write-Host ""
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "PANNE:8F6720C5"
        Write-Host ""
        Write-Host "[OK] Test termine - Un incident de panne devrait etre cree" -ForegroundColor Green
    }
    
    "all" {
        Write-Host "Execution de TOUS les scenarios de test..." -ForegroundColor Yellow
        Write-Host ""
        
        # Test 1 - Acces approuve
        Write-Host "[1/3] Test Acces AUTORISE" -ForegroundColor Green
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "UID:8F6720C5"
        Start-Sleep -Seconds 1
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "APPROVED:8F6720C5"
        Start-Sleep -Seconds 2
        
        # Test 2 - Acces refuses
        Write-Host ""
        Write-Host "[2/3] Test Tentatives REFUSEES (3x)" -ForegroundColor Green
        for ($i = 1; $i -le 3; $i++) {
            Write-Host "  Tentative $i sur 3..." -ForegroundColor Cyan
            Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "UID:UNAUTHORIZED01"
            Start-Sleep -Seconds 0.5
            Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "DENIED:UNAUTHORIZED01"
            Start-Sleep -Seconds 1
        }
        Start-Sleep -Seconds 2
        
        # Test 3 - Panne
        Write-Host ""
        Write-Host "[3/3] Test Panne d'ascenseur" -ForegroundColor Green
        Send-SerialMessage -PortNumber $port -BaudRate $baud -Message "PANNE:8F6720C5"
        
        Write-Host ""
        Write-Host "[OK] Tous les tests termines!" -ForegroundColor Green
    }
    
    default {
        Write-Host "Usage: .\simulate_rfid.ps1 -scenario <approved|denied|elevator_failure|all>" -ForegroundColor Yellow
        Write-Host ""
        Write-Host "Exemples:" -ForegroundColor Cyan
        Write-Host "  .\simulate_rfid.ps1 -scenario approved      # Test acces autorise" -ForegroundColor Gray
        Write-Host "  .\simulate_rfid.ps1 -scenario denied        # Test 3 tentatives echouees" -ForegroundColor Gray
        Write-Host "  .\simulate_rfid.ps1 -scenario elevator_failure  # Test panne ascenseur" -ForegroundColor Gray
        Write-Host "  .\simulate_rfid.ps1 -scenario all           # Tous les tests" -ForegroundColor Gray
        Write-Host ""
        Write-Host "Options:" -ForegroundColor Cyan
        Write-Host "  -port   Port serie (defaut: 3 = COM3)" -ForegroundColor Gray
        Write-Host "  -baud   Vitesse en baud (defaut: 9600)" -ForegroundColor Gray
        Write-Host ""
    }
}

Write-Host ""
