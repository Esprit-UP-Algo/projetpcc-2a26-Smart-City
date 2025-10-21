#include "vehiculedialog.h"
#include "ui_vehiculedialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QDate>
#include <QLabel>

VehiculeDialog::VehiculeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehiculeDialog),
    editMode(false)
{
    ui->setupUi(this);
    setupDialog();
}

VehiculeDialog::VehiculeDialog(const QVariantMap &vehiculeData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehiculeDialog),
    editMode(true)
{
    ui->setupUi(this);
    setupDialog();
    loadVehiculeData(vehiculeData);
}

VehiculeDialog::~VehiculeDialog()
{
    delete ui;
}

void VehiculeDialog::setupDialog()
{
    setWindowTitle(editMode ? "Modifier le véhicule" : "Ajouter un véhicule");
    
    // Change the label text from "Kilométrage" to "Date"
    // Try different possible label names
    QList<QLabel*> labels = findChildren<QLabel*>();
    for (QLabel* label : labels) {
        if (label->text().contains("Kilométrage") || label->text().contains("kilometrage")) {
            label->setText("Date *:");
            break;
        }
    }
    
    // Alternative approach: try common label names
    QLabel* label = findChild<QLabel*>("label_6"); // Common auto-generated name
    if (!label) label = findChild<QLabel*>("kilometrageLabel");
    if (!label) label = findChild<QLabel*>("kmLabel");
    if (label) {
        label->setText("Date *:");
    }
    
    // Configure the kilometrage spinbox as a date selector
    // Set range to represent dates (format: YYYYMMDD as numbers)
    ui->kilometrageSpinBox->setRange(20200101, 20301231); // Valid date range
    ui->kilometrageSpinBox->setDecimals(0); // No decimals for dates
    ui->kilometrageSpinBox->setSuffix(""); // Remove any suffix
    ui->kilometrageSpinBox->setToolTip("Entrez la date au format AAAAMMJJ (ex: 20251007 pour 07/10/2025)");
    
    // Set default values
    if (!editMode) {
        ui->capaciteSpinBox->setValue(4);
        // Set current date as numeric value (YYYYMMDD format)
        ui->kilometrageSpinBox->setValue(QDate::currentDate().toString("yyyyMMdd").toDouble());
        ui->tempsUtiliseSpinBox->setValue(0.0);
        ui->statutCombo->setCurrentIndex(0); // "Disponible"
    }
}

void VehiculeDialog::loadVehiculeData(const QVariantMap &data)
{
    vehiculeId = data["id"].toString();
    
    ui->idEdit->setText(data["id"].toString());
    ui->idEdit->setReadOnly(true); // ID cannot be changed in edit mode

    ui->typeCombo->setCurrentText(data["type"].toString());
    ui->capaciteSpinBox->setValue(data["capacite"].toInt());
    ui->zoneEdit->setText(data["zone"].toString());
    ui->horaireEdit->setText(data["horaire"].toString());
    ui->statutCombo->setCurrentText(data["statut"].toString());
    
    // Load date from data and convert to numeric format for spinbox
    QString dateStr = data["date"].toString();
    if (!dateStr.isEmpty()) {
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (date.isValid()) {
            // Convert date to numeric format YYYYMMDD for display in spinbox
            ui->kilometrageSpinBox->setValue(date.toString("yyyyMMdd").toDouble());
        } else {
            ui->kilometrageSpinBox->setValue(QDate::currentDate().toString("yyyyMMdd").toDouble());
        }
    } else {
        ui->kilometrageSpinBox->setValue(QDate::currentDate().toString("yyyyMMdd").toDouble());
    }
    
    ui->tempsUtiliseSpinBox->setValue(data["temps_utilise"].toDouble());
}

QVariantMap VehiculeDialog::getVehiculeData() const
{
    QVariantMap data;
    
    data["id"] = ui->idEdit->text();
    data["type"] = ui->typeCombo->currentText();
    data["capacite"] = ui->capaciteSpinBox->value();
    data["zone"] = ui->zoneEdit->text();
    data["horaire"] = ui->horaireEdit->text();
    data["statut"] = ui->statutCombo->currentText();
    
    // Convert numeric value back to date format
    double numericDate = ui->kilometrageSpinBox->value();
    QString dateStr = QString::number((int)numericDate);
    if (dateStr.length() == 8) {
        // Parse YYYYMMDD format
        QDate date = QDate::fromString(dateStr, "yyyyMMdd");
        if (date.isValid()) {
            data["date"] = date.toString("dd/MM/yyyy");
        } else {
            data["date"] = QDate::currentDate().toString("dd/MM/yyyy");
        }
    } else {
        data["date"] = QDate::currentDate().toString("dd/MM/yyyy");
    }
    
    data["temps_utilise"] = ui->tempsUtiliseSpinBox->value();
    
    return data;
}

bool VehiculeDialog::validateInput()
{
    if (ui->idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID du véhicule (ex: VH001)");
        ui->idEdit->setFocus();
        return false;
    }

    if (ui->zoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer la zone d'opération");
        ui->zoneEdit->setFocus();
        return false;
    }

    if (ui->horaireEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'horaire (ex: 08:00-18:00)");
        ui->horaireEdit->setFocus();
        return false;
    }
    
    return true;
}

void VehiculeDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }
    
    accept();
}

void VehiculeDialog::on_cancelButton_clicked()
{
    reject();
}
