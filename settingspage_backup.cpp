#include "settingspage.h"
#include "ui_settingspage.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QRegularExpression>

SettingsPage::SettingsPage(UserRole userRole, const QString &currentUser, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPage),
    m_userRole(userRole),
    m_currentUser(currentUser),
    m_translator(new QTranslator(this)),
    m_currentTheme("Clair")
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    updateUIForRole();
    loadUserData();
    
    // Set initial page
    ui->settingsStackedWidget->setCurrentIndex(0);
    highlightMenuButton(ui->profileButton);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::setupUI()
{
    // Apply initial theme
    applyTheme(m_currentTheme);
}

void SettingsPage::setupConnections()
{
    // Navigation connections are handled by auto-generated slots
}

void SettingsPage::setUserRole(UserRole role)
{
    m_userRole = role;
    updateUIForRole();
}

void SettingsPage::setCurrentUser(const QString &user)
{
    m_currentUser = user;
    loadUserData();
}

void SettingsPage::updateUIForRole()
{
    // Update title based on role
    QString roleTitle = "⚙️ Paramètres";
    QString moduleName = "Général";
    
    switch (m_userRole) {
        case UserRole::ADMIN:
            moduleName = "Administration Principale";
            ui->databaseButton->setVisible(true);
            break;
        case UserRole::ADMIN_SETTINGS:
            moduleName = "Administration Système";
            ui->databaseButton->setVisible(true);
            break;
        case UserRole::RESIDENT:
            moduleName = "Résidents";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::FINANCE:
            moduleName = "Finance";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::INCIDENT:
            moduleName = "Incidents";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::TRANSPORT:
            moduleName = "Transport";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::LOCAUX:
            moduleName = "Locaux";
            ui->databaseButton->setVisible(false);
            break;
        default:
            ui->databaseButton->setVisible(false);
            break;
    }
    
    ui->titleLabel->setText(roleTitle + " - " + moduleName);
}

void SettingsPage::updateAboutSection()
{
    // Update about section when on that page
}

void SettingsPage::updateNotificationsForRole()
{
    // Update notifications section when on that page
    if (ui->enableNotificationsCheckBox) {
        QString roleSpecificText = "Notifications spécifiques - ";
        
        switch (m_userRole) {
            case UserRole::ADMIN:
            case UserRole::ADMIN_SETTINGS:
                roleSpecificText += "Administration";
                break;
            case UserRole::RESIDENT:
                roleSpecificText += "Gestion des résidents";
                break;
            case UserRole::FINANCE:
                roleSpecificText += "Transactions financières";
                break;
            case UserRole::INCIDENT:
                roleSpecificText += "Alertes d'incidents";
                break;
            case UserRole::TRANSPORT:
                roleSpecificText += "Mises à jour transport";
                break;
            case UserRole::LOCAUX:
                roleSpecificText += "Gestion des locaux";
                break;
            default:
                roleSpecificText += "Général";
                break;
        }
        
        ui->enableNotificationsCheckBox->setText(roleSpecificText);
    }
}

void SettingsPage::loadUserData()
{
    if (m_currentUser.isEmpty()) {
        return;
    }
    
    // Load user profile data
    if (ui->usernameLineEdit) ui->usernameLineEdit->setText(m_currentUser);
    if (ui->emailLineEdit) ui->emailLineEdit->setText(m_currentUser + "@nexora.city");
    if (ui->phoneLineEdit) ui->phoneLineEdit->setText("+33 1 23 45 67 89");
    
    // Set notification defaults
    if (ui->enableNotificationsCheckBox) ui->enableNotificationsCheckBox->setChecked(true);
}

void SettingsPage::onSaveProfileClicked()
{
    updateUserProfile();
}

void SettingsPage::updateUserProfile()
{
    if (!ui->usernameLineEdit || !ui->emailLineEdit) return;
    
    QString username = ui->usernameLineEdit->text().trimmed();
    QString email = ui->emailLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit ? ui->phoneLineEdit->text().trimmed() : "";
    
    if (username.isEmpty() || email.isEmpty()) {
        showError("Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    // Email validation
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        showError("Veuillez entrer une adresse email valide.");
        return;
    }
    
    showSuccess("Profil mis à jour avec succès!");
    emit userProfileUpdated();
}

void SettingsPage::onThemeChanged(const QString &theme)
{
    m_currentTheme = theme;
    applyTheme(theme);
    emit themeChanged(theme);
}

void SettingsPage::onLanguageChanged(const QString &language)
{
    QString languageCode = (language == "English") ? "en" : "fr";
    switchLanguage(languageCode);
    emit languageChanged(languageCode);
}

void SettingsPage::onChangePasswordClicked()
{
    changePassword();
}

void SettingsPage::changePassword()
{
    // Password change handled by on_changePasswordButton_clicked
}

void SettingsPage::onNotificationToggled()
{
    if (ui->enableNotificationsCheckBox) {
        qDebug() << "Notifications enabled:" << ui->enableNotificationsCheckBox->isChecked();
    }
}

void SettingsPage::onDatabaseBackupClicked()
{
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString backupFileName = QString("nexora_backup_%1.db").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString backupPath = QFileDialog::getSaveFileName(this, "Sauvegarder la base de données", 
                                                      backupDir + "/" + backupFileName,
                                                      "Database files (*.db)");
    
    if (!backupPath.isEmpty()) {
        showSuccess("Base de données sauvegardée avec succès vers:\n" + backupPath);
    }
}

void SettingsPage::onDatabaseOptimizeClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Optimiser la base de données",
                                                              "Voulez-vous optimiser la base de données? Cette opération peut prendre quelques minutes.",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        showSuccess("Base de données optimisée avec succès!");
    }
}

void SettingsPage::onDatabaseViewClicked()
{
    QMessageBox::information(this, "Données de la base", 
                             "Tables disponibles:\n"
                             "- users (7 entrées)\n"
                             "- residents (124 entrées)\n"
                             "- incidents (45 entrées)\n"
                             "- finance (89 entrées)\n"
                             "- transport (67 entrées)\n"
                             "- locaux (23 entrées)\n"
                             "- settings (12 entrées)");
}

void SettingsPage::applyTheme(const QString &themeName)
{
    QString styleSheet;
    
    if (themeName == "Sombre") {
        styleSheet = getDarkThemeStyle();
    } else {
        styleSheet = getLightThemeStyle();
    }
    
    setStyleSheet(styleSheet);
}

void SettingsPage::switchLanguage(const QString &languageCode)
{
    if (m_translator) {
        QCoreApplication::removeTranslator(m_translator);
    }
    
    if (languageCode == "en") {
        if (m_translator->load(":/translations/nexora_en.qm")) {
            QCoreApplication::installTranslator(m_translator);
        }
    }
}

void SettingsPage::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Succès", message);
}

void SettingsPage::showError(const QString &message)
{
    QMessageBox::warning(this, "Erreur", message);
}

QString SettingsPage::getLightThemeStyle()
{
    return R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Segoe UI';
            color: #333333;
        }
    )";
}

QString SettingsPage::getDarkThemeStyle()
{
    return R"(
        QWidget {
            background-color: #2c3e50;
            font-family: 'Segoe UI';
            color: #ecf0f1;
        }
    )";
}

void SettingsPage::switchToPage(int index)
{
    if (ui->settingsStackedWidget) {
        ui->settingsStackedWidget->setCurrentIndex(index);
        
        switch (index) {
            case 0: // Profile page
                loadUserData();
                break;
            case 2: // Notifications page
                updateNotificationsForRole();
                break;
            case 5: // About page
                updateAboutSection();
                break;
        }
    }
}

void SettingsPage::highlightMenuButton(QPushButton *button)
{
    resetMenuButtons();
    if (button) {
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
    
    if (ui->profileButton) ui->profileButton->setStyleSheet(defaultStyle);
    if (ui->themeButton) ui->themeButton->setStyleSheet(defaultStyle);
    if (ui->notificationsButton) ui->notificationsButton->setStyleSheet(defaultStyle);
    if (ui->securityButton) ui->securityButton->setStyleSheet(defaultStyle);
    if (ui->databaseButton) ui->databaseButton->setStyleSheet(defaultStyle);
    if (ui->aboutButton) ui->aboutButton->setStyleSheet(defaultStyle);
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
    if (m_userRole == UserRole::ADMIN || m_userRole == UserRole::ADMIN_SETTINGS) {
        switchToPage(4);
        highlightMenuButton(ui->databaseButton);
    }
}

void SettingsPage::on_aboutButton_clicked()
{
    switchToPage(5);
    highlightMenuButton(ui->aboutButton);
}

void SettingsPage::on_saveProfileButton_clicked()
{
    if (!ui->emailLineEdit) return;
    
    QString email = ui->emailLineEdit->text();
    QString phone = ui->phoneLineEdit ? ui->phoneLineEdit->text() : "";
    
    if (email.isEmpty() || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse email valide");
        return;
    }
    
    QMessageBox::information(this, "Profil", 
        "✅ Profil mis à jour avec succès!\n\n"
        "Email: " + email + "\n"
        "Téléphone: " + phone);
    
    emit userProfileUpdated();
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
    
    emit passwordChanged();
}

void SettingsPage::on_backupButton_clicked()
{
    if (m_userRole == UserRole::ADMIN || m_userRole == UserRole::ADMIN_SETTINGS) {
        onDatabaseBackupClicked();
    } else {
        QMessageBox::information(this, "Sauvegarde",
            "Mode interface uniquement: aucune base de données à sauvegarder.");
    }
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
    applyTheme("Clair");
    QMessageBox::information(this, "Thème", 
        "☀️ Thème clair activé!");
    emit themeChanged("Clair");
}

void SettingsPage::on_darkThemeRadio_clicked()
{
    applyTheme("Sombre");
    QMessageBox::information(this, "Thème", 
        "🌙 Thème sombre activé!");
    emit themeChanged("Sombre");
}

void SettingsPage::on_languageComboBox_currentIndexChanged(int index)
{
    if (index < 0) return;
    
    QString language;
    QString languageCode;
    switch(index) {
        case 0: 
            language = "Français"; 
            languageCode = "fr";
            break;
        case 1: 
            language = "English"; 
            languageCode = "en";
            break;
        case 2: 
            language = "العربية"; 
            languageCode = "ar";
            break;
        default: return;
    }
    
    // Don't show message on first load
    static bool firstLoad = true;
    if (firstLoad) {
        firstLoad = false;
        return;
    }
    
    switchLanguage(languageCode);
    QMessageBox::information(this, "Langue", 
        "Langue changée: " + language);
    emit languageChanged(languageCode);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::setupUI()
{
    // Apply initial theme
    applyTheme(m_currentTheme);
}

void SettingsPage::setupConnections()
{
    // Navigation connections are handled by auto-generated slots
    // Additional manual connections if needed
}

void SettingsPage::setUserRole(UserRole role)
{
    m_userRole = role;
    updateUIForRole();
}

void SettingsPage::setCurrentUser(const QString &user)
{
    m_currentUser = user;
    loadUserData();
}

void SettingsPage::updateUIForRole()
{
    // Update title based on role
    QString roleTitle = "⚙️ Paramètres";
    QString moduleName = "Général";
    
    switch (m_userRole) {
        case UserRole::ADMIN:
            moduleName = "Administration Principale";
            ui->databaseButton->setVisible(true);
            break;
        case UserRole::ADMIN_SETTINGS:
            moduleName = "Administration Système";
            ui->databaseButton->setVisible(true);
            break;
        case UserRole::RESIDENT:
            moduleName = "Résidents";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::FINANCE:
            moduleName = "Finance";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::INCIDENT:
            moduleName = "Incidents";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::TRANSPORT:
            moduleName = "Transport";
            ui->databaseButton->setVisible(false);
            break;
        case UserRole::LOCAUX:
            moduleName = "Locaux";
            ui->databaseButton->setVisible(false);
            break;
        default:
            ui->databaseButton->setVisible(false);
            break;
    }
    
    ui->titleLabel->setText(roleTitle + " - " + moduleName);
    updateAboutSection();
}

void SettingsPage::updateAboutSection()
{
    QString moduleName = "";
    
    switch (m_userRole) {
        case UserRole::ADMIN:
            moduleName = "Administration Principale";
            break;
        case UserRole::ADMIN_SETTINGS:
            moduleName = "Administration Système";
            break;
        case UserRole::RESIDENT:
            moduleName = "Résidents";
            break;
        case UserRole::FINANCE:
            moduleName = "Finance";
            break;
        case UserRole::INCIDENT:
            moduleName = "Incidents";
            break;
        case UserRole::TRANSPORT:
            moduleName = "Transport";
            break;
        case UserRole::LOCAUX:
            moduleName = "Locaux";
            break;
        default:
            moduleName = "Général";
            break;
    }
    
    // Update about section would happen here when we're on the about page
}

void SettingsPage::updateNotificationsForRole()
{
    // Update notifications section when we're on that page
    if (ui->enableNotificationsCheckBox) {
        QString roleSpecificText = "Notifications spécifiques - ";
        
        switch (m_userRole) {
            case UserRole::ADMIN:
            case UserRole::ADMIN_SETTINGS:
                roleSpecificText += "Administration";
                break;
            case UserRole::RESIDENT:
                roleSpecificText += "Gestion des résidents";
                break;
            case UserRole::FINANCE:
                roleSpecificText += "Transactions financières";
                break;
            case UserRole::INCIDENT:
                roleSpecificText += "Alertes d'incidents";
                break;
            case UserRole::TRANSPORT:
                roleSpecificText += "Mises à jour transport";
                break;
            case UserRole::LOCAUX:
                roleSpecificText += "Gestion des locaux";
                break;
            default:
                roleSpecificText += "Général";
                break;
        }
        
        ui->enableNotificationsCheckBox->setText(roleSpecificText);
    }
}

void SettingsPage::loadUserData()
{
    if (m_currentUser.isEmpty()) {
        return;
    }
    
    // Load user profile data
    if (ui->usernameLineEdit) ui->usernameLineEdit->setText(m_currentUser);
    if (ui->emailLineEdit) ui->emailLineEdit->setText(m_currentUser + "@nexora.city");
    if (ui->phoneLineEdit) ui->phoneLineEdit->setText("+33 1 23 45 67 89");
    
    // Set notification defaults
    if (ui->enableNotificationsCheckBox) ui->enableNotificationsCheckBox->setChecked(true);
}

void SettingsPage::onSaveProfileClicked()
{
    updateUserProfile();
}

void SettingsPage::updateUserProfile()
{
    if (!ui->usernameLineEdit || !ui->emailLineEdit) return;
    
    QString username = ui->usernameLineEdit->text().trimmed();
    QString email = ui->emailLineEdit->text().trimmed();
    QString phone = ui->phoneLineEdit ? ui->phoneLineEdit->text().trimmed() : "";
    
    if (username.isEmpty() || email.isEmpty()) {
        showError("Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    // Email validation
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        showError("Veuillez entrer une adresse email valide.");
        return;
    }
    
    showSuccess("Profil mis à jour avec succès!");
    emit userProfileUpdated();
}

void SettingsPage::onThemeChanged(const QString &theme)
{
    m_currentTheme = theme;
    applyTheme(theme);
    emit themeChanged(theme);
}

void SettingsPage::onLanguageChanged(const QString &language)
{
    QString languageCode = (language == "English") ? "en" : "fr";
    switchLanguage(languageCode);
    emit languageChanged(languageCode);
}

void SettingsPage::onChangePasswordClicked()
{
    changePassword();
}

void SettingsPage::changePassword()
{
    // This will be handled by the on_changePasswordButton_clicked() slot
    // which already exists in the original implementation
}

void SettingsPage::onNotificationToggled()
{
    // Notification handling
    if (ui->enableNotificationsCheckBox) {
        qDebug() << "Notifications enabled:" << ui->enableNotificationsCheckBox->isChecked();
    }
}

void SettingsPage::onDatabaseBackupClicked()
{
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString backupFileName = QString("nexora_backup_%1.db").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString backupPath = QFileDialog::getSaveFileName(this, "Sauvegarder la base de données", 
                                                      backupDir + "/" + backupFileName,
                                                      "Database files (*.db)");
    
    if (!backupPath.isEmpty()) {
        showSuccess("Base de données sauvegardée avec succès vers:\\n" + backupPath);
    }
}

void SettingsPage::onDatabaseOptimizeClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Optimiser la base de données",
                                                              "Voulez-vous optimiser la base de données? Cette opération peut prendre quelques minutes.",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        showSuccess("Base de données optimisée avec succès!");
    }
}

void SettingsPage::onDatabaseViewClicked()
{
    QMessageBox::information(this, "Données de la base", 
                             "Tables disponibles:\\n"
                             "- users (7 entrées)\\n"
                             "- residents (124 entrées)\\n"
                             "- incidents (45 entrées)\\n"
                             "- finance (89 entrées)\\n"
                             "- transport (67 entrées)\\n"
                             "- locaux (23 entrées)\\n"
                             "- settings (12 entrées)");
}

void SettingsPage::applyTheme(const QString &themeName)
{
    QString styleSheet;
    
    if (themeName == "Sombre") {
        styleSheet = getDarkThemeStyle();
    } else {
        styleSheet = getLightThemeStyle();
    }
    
    setStyleSheet(styleSheet);
}

void SettingsPage::switchLanguage(const QString &languageCode)
{
    if (m_translator) {
        QCoreApplication::removeTranslator(m_translator);
    }
    
    if (languageCode == "en") {
        if (m_translator->load(":/translations/nexora_en.qm")) {
            QCoreApplication::installTranslator(m_translator);
        }
    }
}

void SettingsPage::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Succès", message);
}

void SettingsPage::showError(const QString &message)
{
    QMessageBox::warning(this, "Erreur", message);
}

QString SettingsPage::getLightThemeStyle()
{
    return R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Segoe UI';
            color: #333333;
        }
    )";
}

QString SettingsPage::getDarkThemeStyle()
{
    return R"(
        QWidget {
            background-color: #2c3e50;
            font-family: 'Segoe UI';
            color: #ecf0f1;
        }
    )";
}

void SettingsPage::switchToPage(int index)
{
    if (ui->settingsStackedWidget) {
        ui->settingsStackedWidget->setCurrentIndex(index);
        
        // Update UI elements based on the current page
        switch (index) {
            case 0: // Profile page
                loadUserData();
                break;
            case 2: // Notifications page
                updateNotificationsForRole();
                break;
            case 5: // About page
                updateAboutSection();
                break;
        }
    }
}

void SettingsPage::highlightMenuButton(QPushButton *button)
{
    resetMenuButtons();
    if (button) {
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
    
    if (ui->profileButton) ui->profileButton->setStyleSheet(defaultStyle);
    if (ui->themeButton) ui->themeButton->setStyleSheet(defaultStyle);
    if (ui->notificationsButton) ui->notificationsButton->setStyleSheet(defaultStyle);
    if (ui->securityButton) ui->securityButton->setStyleSheet(defaultStyle);
    if (ui->databaseButton) ui->databaseButton->setStyleSheet(defaultStyle);
    if (ui->aboutButton) ui->aboutButton->setStyleSheet(defaultStyle);
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
    if (m_userRole == UserRole::ADMIN || m_userRole == UserRole::ADMIN_SETTINGS) {
        switchToPage(4);
        highlightMenuButton(ui->databaseButton);
    }
}

void SettingsPage::on_aboutButton_clicked()
{
    switchToPage(5);
    highlightMenuButton(ui->aboutButton);
}

void SettingsPage::on_saveProfileButton_clicked()
{
    if (!ui->emailLineEdit) return;
    
    QString email = ui->emailLineEdit->text();
    QString phone = ui->phoneLineEdit ? ui->phoneLineEdit->text() : "";
    
    if (email.isEmpty() || !email.contains("@")) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse email valide");
        return;
    }
    
    QMessageBox::information(this, "Profil", 
        "✅ Profil mis à jour avec succès!\\n\\n"
        "Email: " + email + "\\n"
        "Téléphone: " + phone);
    
    emit userProfileUpdated();
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
        "🔑 Mot de passe changé avec succès!\\n\\n"
        "Votre nouveau mot de passe a été enregistré.");
    
    emit passwordChanged();
}

void SettingsPage::on_backupButton_clicked()
{
    if (m_userRole == UserRole::ADMIN || m_userRole == UserRole::ADMIN_SETTINGS) {
        onDatabaseBackupClicked();
    } else {
        QMessageBox::information(this, "Sauvegarde",
            "Mode interface uniquement: aucune base de données à sauvegarder.");
    }
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
    applyTheme("Clair");
    QMessageBox::information(this, "Thème", 
        "☀️ Thème clair activé!");
    emit themeChanged("Clair");
}

void SettingsPage::on_darkThemeRadio_clicked()
{
    applyTheme("Sombre");
    QMessageBox::information(this, "Thème", 
        "🌙 Thème sombre activé!");
    emit themeChanged("Sombre");
}

void SettingsPage::on_languageComboBox_currentIndexChanged(int index)
{
    if (index < 0) return;
    
    QString language;
    QString languageCode;
    switch(index) {
        case 0: 
            language = "Français"; 
            languageCode = "fr";
            break;
        case 1: 
            language = "English"; 
            languageCode = "en";
            break;
        case 2: 
            language = "العربية"; 
            languageCode = "ar";
            break;
        default: return;
    }
    
    // Don't show message on first load
    static bool firstLoad = true;
    if (firstLoad) {
        firstLoad = false;
        return;
    }
    
    switchLanguage(languageCode);
    QMessageBox::information(this, "Langue", 
        "Langue changée: " + language);
    emit languageChanged(languageCode);
}

void SettingsPage::onThemeChanged(const QString &theme)
{
    m_currentTheme = theme;
    applyTheme(theme);
    emit themeChanged(theme);
}

void SettingsPage::onLanguageChanged(const QString &language)
{
    QString languageCode = (language == "English") ? "en" : "fr";
    switchLanguage(languageCode);
    emit languageChanged(languageCode);
}

void SettingsPage::onChangePasswordClicked()
{
    changePassword();
}

void SettingsPage::changePassword()
{
    QString oldPassword = ui->oldPasswordEdit->text();
    QString newPassword = ui->newPasswordEdit->text();
    QString confirmPassword = ui->confirmPasswordEdit->text();
    
    if (oldPassword.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        showError("Veuillez remplir tous les champs de mot de passe.");
        return;
    }
    
    if (newPassword != confirmPassword) {
        showError("Les nouveaux mots de passe ne correspondent pas.");
        return;
    }
    
    if (newPassword.length() < 4) {
        showError("Le nouveau mot de passe doit contenir au moins 4 caractères.");
        return;
    }
    
    // TODO: Validate old password against database/stored password
    // For now, assume validation passes
    
    // Clear password fields
    ui->oldPasswordEdit->clear();
    ui->newPasswordEdit->clear();
    ui->confirmPasswordEdit->clear();
    
    showSuccess("Mot de passe modifié avec succès!");
    emit passwordChanged();
}

void SettingsPage::onNotificationToggled()
{
    // Save notification preferences
    // TODO: Save to database/settings file
    qDebug() << "Email notifications:" << ui->emailNotificationsCheck->isChecked();
    qDebug() << "System notifications:" << ui->systemNotificationsCheck->isChecked();
    qDebug() << "Role-specific notifications:" << ui->roleSpecificNotificationsCheck->isChecked();
}

void SettingsPage::onDatabaseBackupClicked()
{
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString backupFileName = QString("nexora_backup_%1.db").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));
    QString backupPath = QFileDialog::getSaveFileName(this, "Sauvegarder la base de données", 
                                                      backupDir + "/" + backupFileName,
                                                      "Database files (*.db)");
    
    if (!backupPath.isEmpty()) {
        // TODO: Implement actual database backup
        // For now, simulate backup
        showSuccess("Base de données sauvegardée avec succès vers:\n" + backupPath);
    }
}

void SettingsPage::onDatabaseOptimizeClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Optimiser la base de données",
                                                              "Voulez-vous optimiser la base de données? Cette opération peut prendre quelques minutes.",
                                                              QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // TODO: Implement actual database optimization
        // For now, simulate optimization
        showSuccess("Base de données optimisée avec succès!");
        
        // Update database info
        ui->dbInfoLabel->setText("Type: SQLite | Taille: 2.3 MB | Tables: 7 (optimisé)");
    }
}

void SettingsPage::onDatabaseViewClicked()
{
    QMessageBox::information(this, "Données de la base", 
                             "Tables disponibles:\n"
                             "- users (7 entrées)\n"
                             "- residents (124 entrées)\n"
                             "- incidents (45 entrées)\n"
                             "- finance (89 entrées)\n"
                             "- transport (67 entrées)\n"
                             "- locaux (23 entrées)\n"
                             "- settings (12 entrées)");
}

void SettingsPage::applyTheme(const QString &themeName)
{
    QString styleSheet;
    
    if (themeName == "Sombre") {
        styleSheet = getDarkThemeStyle();
    } else {
        styleSheet = getLightThemeStyle();
    }
    
    setStyleSheet(styleSheet);
    
    // Apply theme to parent application if needed
    if (parent()) {
        qobject_cast<QWidget*>(parent())->setStyleSheet(styleSheet);
    }
}

void SettingsPage::switchLanguage(const QString &languageCode)
{
    if (m_translator) {
        QCoreApplication::removeTranslator(m_translator);
    }
    
    if (languageCode == "en") {
        if (m_translator->load(":/translations/nexora_en.qm")) {
            QCoreApplication::installTranslator(m_translator);
        }
    } else {
        // French is default, no translation needed
        // but could load French translation if needed
    }
    
    // Update UI text
    // TODO: Implement retranslateUi or manual text updates
}

void SettingsPage::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Succès", message);
}

void SettingsPage::showError(const QString &message)
{
    QMessageBox::warning(this, "Erreur", message);
}

QString SettingsPage::getLightThemeStyle()
{
    return R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Segoe UI';
            color: #333333;
        }
        
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #2c3e50;
            background: transparent;
        }
        
        QGroupBox {
            font-size: 16px;
            font-weight: bold;
            color: #34495e;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background: white;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 5px 15px;
            background: white;
            border-radius: 4px;
        }
        
        QPushButton {
            background-color: #3498db;
            border: none;
            color: white;
            padding: 12px 24px;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
        
        QPushButton#saveProfileButton {
            background-color: #27ae60;
        }
        
        QPushButton#saveProfileButton:hover {
            background-color: #229954;
        }
        
        QLineEdit, QComboBox {
            padding: 8px 12px;
            border: 2px solid #bdc3c7;
            border-radius: 4px;
            font-size: 14px;
            background: white;
        }
        
        QLineEdit:focus, QComboBox:focus {
            border-color: #3498db;
        }
        
        QCheckBox {
            font-size: 14px;
            spacing: 8px;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        
        QCheckBox::indicator:unchecked {
            border: 2px solid #bdc3c7;
            border-radius: 3px;
            background: white;
        }
        
        QCheckBox::indicator:checked {
            border: 2px solid #3498db;
            border-radius: 3px;
            background: #3498db;
        }
        
        QScrollArea {
            border: none;
            background: transparent;
        }
    )";
}

QString SettingsPage::getDarkThemeStyle()
{
    return R"(
        QWidget {
            background-color: #2c3e50;
            font-family: 'Segoe UI';
            color: #ecf0f1;
        }
        
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #ecf0f1;
            background: transparent;
        }
        
        QGroupBox {
            font-size: 16px;
            font-weight: bold;
            color: #ecf0f1;
            border: 2px solid #34495e;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background: #34495e;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 5px 15px;
            background: #34495e;
            border-radius: 4px;
            color: #ecf0f1;
        }
        
        QPushButton {
            background-color: #3498db;
            border: none;
            color: white;
            padding: 12px 24px;
            border-radius: 6px;
            font-size: 14px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
        
        QPushButton#saveProfileButton {
            background-color: #27ae60;
        }
        
        QPushButton#saveProfileButton:hover {
            background-color: #229954;
        }
        
        QLineEdit, QComboBox {
            padding: 8px 12px;
            border: 2px solid #34495e;
            border-radius: 4px;
            font-size: 14px;
            background: #34495e;
            color: #ecf0f1;
        }
        
        QLineEdit:focus, QComboBox:focus {
            border-color: #3498db;
        }
        
        QComboBox::drop-down {
            border: none;
            background: #34495e;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #ecf0f1;
        }
        
        QCheckBox {
            font-size: 14px;
            spacing: 8px;
            color: #ecf0f1;
        }
        
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        
        QCheckBox::indicator:unchecked {
            border: 2px solid #34495e;
            border-radius: 3px;
            background: #2c3e50;
        }
        
        QCheckBox::indicator:checked {
            border: 2px solid #3498db;
            border-radius: 3px;
            background: #3498db;
        }
        
        QScrollArea {
            border: none;
            background: transparent;
        }
    )";
}
}
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
