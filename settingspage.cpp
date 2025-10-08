#include "settingspage.h"
#include "ui_settingspage.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>
#include <QCoreApplication>

SettingsPage::SettingsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPage)
{
    ui->setupUi(this);
    
    // Set initial page
    ui->settingsStackedWidget->setCurrentIndex(0);
    highlightMenuButton(ui->profileButton);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::switchToPage(int index)
{
    ui->settingsStackedWidget->setCurrentIndex(index);
}

void SettingsPage::highlightMenuButton(QPushButton *button)
{
    resetMenuButtons();
    button->setStyleSheet(
        "QPushButton {"
        "    background-color: #4A90E2;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "    text-align: left;"
        "    padding-left: 20px;"
        "    font-size: 15px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #3A7BC8;"
        "}"
    );
}

void SettingsPage::resetMenuButtons()
{
    QString defaultStyle = 
        "QPushButton {"
        "    background-color: #ECF0F1;"
        "    color: #2C3E50;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 10px;"
        "    text-align: left;"
        "    padding-left: 20px;"
        "    font-size: 15px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4A90E2;"
        "    color: white;"
        "}";
    
    ui->profileButton->setStyleSheet(defaultStyle);
    ui->themeButton->setStyleSheet(defaultStyle);
    ui->notificationsButton->setStyleSheet(defaultStyle);
    ui->securityButton->setStyleSheet(defaultStyle);
    ui->databaseButton->setStyleSheet(defaultStyle);
    ui->aboutButton->setStyleSheet(defaultStyle);
}

void SettingsPage::on_profileButton_clicked()
{
    switchToPage(0);
    highlightMenuButton(ui->profileButton);
}

void SettingsPage::on_themeButton_clicked()
{
    switchToPage(1);
    highlightMenuButton(ui->themeButton);
}

void SettingsPage::on_notificationsButton_clicked()
{
    switchToPage(2);
    highlightMenuButton(ui->notificationsButton);
}

void SettingsPage::on_securityButton_clicked()
{
    switchToPage(3);
    highlightMenuButton(ui->securityButton);
}

void SettingsPage::on_databaseButton_clicked()
{
    switchToPage(4);
    highlightMenuButton(ui->databaseButton);
}

void SettingsPage::on_aboutButton_clicked()
{
    switchToPage(5);
    highlightMenuButton(ui->aboutButton);
}

void SettingsPage::on_saveProfileButton_clicked()
{
    QString email = ui->emailLineEdit->text();
    QString phone = ui->phoneLineEdit->text();
    
    if (email.isEmpty() || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse email valide");
        return;
    }
    
    QMessageBox::information(this, "Profil", 
        "✅ Profil mis à jour avec succès!\n\n"
        "Email: " + email + "\n"
        "Téléphone: " + phone);
}

void SettingsPage::on_changePasswordButton_clicked()
{
    bool ok;
    QString oldPassword = QInputDialog::getText(this, "Changer le mot de passe",
                                                 "Ancien mot de passe:", 
                                                 QLineEdit::Password, "", &ok);
    
    if (!ok || oldPassword.isEmpty()) return;
    
    if (oldPassword != "0000") {
        QMessageBox::warning(this, "Erreur", "Ancien mot de passe incorrect!");
        return;
    }
    
    QString newPassword = QInputDialog::getText(this, "Changer le mot de passe",
                                                 "Nouveau mot de passe:", 
                                                 QLineEdit::Password, "", &ok);
    
    if (!ok || newPassword.isEmpty()) return;
    
    if (newPassword.length() < 4) {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 4 caractères!");
        return;
    }
    
    QString confirmPassword = QInputDialog::getText(this, "Changer le mot de passe",
                                                     "Confirmer le nouveau mot de passe:", 
                                                     QLineEdit::Password, "", &ok);
    
    if (!ok || confirmPassword != newPassword) {
        QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas!");
        return;
    }
    
    QMessageBox::information(this, "Succès", 
        "🔑 Mot de passe changé avec succès!\n\n"
        "Votre nouveau mot de passe a été enregistré.");
}

void SettingsPage::on_backupButton_clicked()
{
    QMessageBox::information(this, "Sauvegarde",
        "Mode interface uniquement: aucune base de données à sauvegarder.");
}

void SettingsPage::on_restoreButton_clicked()
{
    QMessageBox::information(this, "Restauration",
        "Mode interface uniquement: aucune base de données à restaurer.");
}

void SettingsPage::on_clearDataButton_clicked()
{
    QMessageBox::information(this, "Nettoyage",
        "Mode interface uniquement: aucune donnée persistante à supprimer.");
}

void SettingsPage::on_lightThemeRadio_clicked()
{
    QMessageBox::information(this, "Thème", 
        "☀️ Thème clair activé!\n\n"
        "(Fonctionnalité à implémenter)");
}

void SettingsPage::on_darkThemeRadio_clicked()
{
    QMessageBox::information(this, "Thème", 
        "🌙 Thème sombre sélectionné!\n\n"
        "(Fonctionnalité à implémenter - nécessite un redémarrage)");
}

void SettingsPage::on_languageComboBox_currentIndexChanged(int index)
{
    if (index < 0) return;
    
    QString language;
    switch(index) {
        case 0: language = "Français"; break;
        case 1: language = "English"; break;
        case 2: language = "العربية"; break;
        default: return;
    }
    
    // Don't show message on first load
    static bool firstLoad = true;
    if (firstLoad) {
        firstLoad = false;
        return;
    }
    
    QMessageBox::information(this, "Langue", 
        "Langue changée: " + language + "\n\n"
        "(Fonctionnalité à implémenter - nécessite un redémarrage)");
}
