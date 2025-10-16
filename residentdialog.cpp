#include "residentdialog.h"
#include "ui_residentdialog.h"
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

ResidentDialog::ResidentDialog(QWidget *parent, const QVariantMap &residentData)
    : QDialog(parent), ui(new Ui::ResidentDialog), editMode(!residentData.isEmpty()), residentData(residentData)
{
    ui->setupUi(this);
    setupValidation();
    setupConnections();
    if (editMode) {
        loadResidentData(residentData);
        ui->titleLabel->setText("✏️ Modifier un Résident");
    } else {
        ui->titleLabel->setText("👤 Ajouter un Résident");
        ui->dateEntreeEdit->setDate(QDate::currentDate());
    }
}

ResidentDialog::~ResidentDialog() 
{
    delete ui;
}

void ResidentDialog::setupValidation()
{
    // Setup validators for input fields
    QRegularExpression cinRegex("[0-9]{8}");
    QRegularExpressionValidator *cinValidator = new QRegularExpressionValidator(cinRegex, this);
    ui->cinEdit->setValidator(cinValidator);
    
    QRegularExpression nameRegex("[A-Za-zÀ-ÿ\\s]{2,30}");
    QRegularExpressionValidator *nameValidator = new QRegularExpressionValidator(nameRegex, this);
    ui->nomEdit->setValidator(nameValidator);
    ui->prenomEdit->setValidator(nameValidator);
    
    QRegularExpression phoneRegex("[+]?[0-9\\s-]{8,15}");
    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    ui->telephoneEdit->setValidator(phoneValidator);
    
    QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    QRegularExpressionValidator *emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->emailEdit->setValidator(emailValidator);
    
    QRegularExpression appartementRegex("[A-Za-z0-9]{1,10}");
    QRegularExpressionValidator *appartementValidator = new QRegularExpressionValidator(appartementRegex, this);
    ui->appartementEdit->setValidator(appartementValidator);
    
    // Set date range
    ui->dateEntreeEdit->setDateRange(QDate(2020, 1, 1), QDate::currentDate().addYears(1));
}

void ResidentDialog::setupConnections()
{
    // Connect validation events
    connect(ui->cinEdit, &QLineEdit::textChanged, this, &ResidentDialog::validateInput);
    connect(ui->nomEdit, &QLineEdit::textChanged, this, &ResidentDialog::validateInput);
    connect(ui->prenomEdit, &QLineEdit::textChanged, this, &ResidentDialog::validateInput);
    connect(ui->sexeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ResidentDialog::validateInput);
    connect(ui->appartementEdit, &QLineEdit::textChanged, this, &ResidentDialog::validateInput);
    
    // Connect save button to validation check
    connect(ui->saveButton, &QPushButton::clicked, this, &ResidentDialog::onSaveClicked);
}

void ResidentDialog::validateInput()
{
    bool isValid = true;
    
    // Validate required fields
    if (ui->cinEdit->text().length() != 8) {
        ui->cinEdit->setStyleSheet("border: 2px solid #EF4444; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #FEF2F2;");
        isValid = false;
    } else {
        ui->cinEdit->setStyleSheet("border: 2px solid #10B981; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #F0FDF4;");
    }
    
    if (ui->nomEdit->text().trimmed().isEmpty()) {
        ui->nomEdit->setStyleSheet("border: 2px solid #EF4444; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #FEF2F2;");
        isValid = false;
    } else {
        ui->nomEdit->setStyleSheet("border: 2px solid #10B981; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #F0FDF4;");
    }
    
    if (ui->prenomEdit->text().trimmed().isEmpty()) {
        ui->prenomEdit->setStyleSheet("border: 2px solid #EF4444; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #FEF2F2;");
        isValid = false;
    } else {
        ui->prenomEdit->setStyleSheet("border: 2px solid #10B981; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #F0FDF4;");
    }
    
    if (ui->sexeCombo->currentIndex() == 0) {
        ui->sexeCombo->setStyleSheet("border: 2px solid #EF4444; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #FEF2F2;");
        isValid = false;
    } else {
        ui->sexeCombo->setStyleSheet("border: 2px solid #10B981; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #F0FDF4;");
    }
    
    if (ui->appartementEdit->text().trimmed().isEmpty()) {
        ui->appartementEdit->setStyleSheet("border: 2px solid #EF4444; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #FEF2F2;");
        isValid = false;
    } else {
        ui->appartementEdit->setStyleSheet("border: 2px solid #10B981; border-radius: 8px; padding: 12px; font-size: 14px; background-color: #F0FDF4;");
    }
    
    // Enable/disable save button
    ui->saveButton->setEnabled(isValid);
    if (isValid) {
        ui->saveButton->setStyleSheet("background-color: #3168E0; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;");
    } else {
        ui->saveButton->setStyleSheet("background-color: #9CA3AF; color: white; border: none; border-radius: 10px; font-weight: bold; font-size: 14px; padding: 12px 25px;");
    }
}

void ResidentDialog::onSaveClicked()
{
    // Final validation before save
    if (ui->cinEdit->text().length() != 8) {
        QMessageBox::warning(this, "Validation", "Le CIN doit contenir exactement 8 chiffres.");
        ui->cinEdit->setFocus();
        return;
    }
    
    if (ui->nomEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le nom est obligatoire.");
        ui->nomEdit->setFocus();
        return;
    }
    
    if (ui->prenomEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le prénom est obligatoire.");
        ui->prenomEdit->setFocus();
        return;
    }
    
    if (ui->sexeCombo->currentIndex() == 0) {
        QMessageBox::warning(this, "Validation", "Veuillez sélectionner le sexe.");
        ui->sexeCombo->setFocus();
        return;
    }
    
    if (ui->appartementEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "L'appartement est obligatoire.");
        ui->appartementEdit->setFocus();
        return;
    }
    
    // Validate email if provided
    if (!ui->emailEdit->text().trimmed().isEmpty()) {
        QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
        if (!emailRegex.match(ui->emailEdit->text()).hasMatch()) {
            QMessageBox::warning(this, "Validation", "Format d'email invalide.");
            ui->emailEdit->setFocus();
            return;
        }
    }
    
    accept(); // Close dialog with accept
}

void ResidentDialog::loadResidentData(const QVariantMap &data)
{
    ui->cinEdit->setText(data.value("cin").toString());
    ui->nomEdit->setText(data.value("nom").toString());
    ui->prenomEdit->setText(data.value("prenom").toString());
    
    // Set sexe
    if (data.value("sexe").toString() == "M") {
        ui->sexeCombo->setCurrentIndex(1);
    } else if (data.value("sexe").toString() == "F") {
        ui->sexeCombo->setCurrentIndex(2);
    }
    
    ui->telephoneEdit->setText(data.value("telephone").toString());
    ui->emailEdit->setText(data.value("email").toString());
    ui->appartementEdit->setText(data.value("appartement").toString());
    ui->etageSpinBox->setValue(data.value("etage").toInt());
    ui->dateEntreeEdit->setDate(QDate::fromString(data.value("date_entree").toString(), "yyyy-MM-dd"));
    
    // Trigger validation
    validateInput();
}

QVariantMap ResidentDialog::getResidentData() const
{
    QVariantMap data;
    data["cin"] = ui->cinEdit->text();
    data["nom"] = ui->nomEdit->text().trimmed();
    data["prenom"] = ui->prenomEdit->text().trimmed();
    
    // Get sexe value
    if (ui->sexeCombo->currentIndex() == 1) {
        data["sexe"] = "M";
    } else if (ui->sexeCombo->currentIndex() == 2) {
        data["sexe"] = "F";
    }
    
    data["telephone"] = ui->telephoneEdit->text();
    data["email"] = ui->emailEdit->text();
    data["appartement"] = ui->appartementEdit->text();
    data["etage"] = ui->etageSpinBox->value();
    data["statut"] = "Actif"; // Default status
    data["date_entree"] = ui->dateEntreeEdit->date().toString("yyyy-MM-dd");
    
    return data;
}