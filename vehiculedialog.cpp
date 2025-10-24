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
    QList<QLabel*> labels = findChildren<QLabel*>();
    for (QLabel* label : labels) {
        if (label->text().contains("Kilométrage") || label->text().contains("kilometrage")) {
            label->setText("Date *:");
            break;
        }
    }

    // Alternative approach: try common label names
    QLabel* label = findChild<QLabel*>("label_6");
    if (!label) label = findChild<QLabel*>("kilometrageFieldLabel");
    if (!label) label = findChild<QLabel*>("kmLabel");
    if (label) {
        label->setText("Date *:");
    }

    // Configure the kilometrage spinbox as a date selector
    ui->kilometrageSpinBox->setRange(20200101, 20301231);
    ui->kilometrageSpinBox->setDecimals(0);
    ui->kilometrageSpinBox->setSuffix("");
    ui->kilometrageSpinBox->setToolTip("Entrez la date au format AAAAMMJJ (ex: 20251022 pour 22/10/2025)");

    // Set default values
    if (!editMode) {
        ui->capaciteSpinBox->setValue(4);
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

    data["id"] = ui->idEdit->text().trimmed();
    data["type"] = ui->typeCombo->currentText();
    data["capacite"] = ui->capaciteSpinBox->value();
    data["zone"] = ui->zoneEdit->text().trimmed();
    data["horaire"] = ui->horaireEdit->text().trimmed();
    data["statut"] = ui->statutCombo->currentText();

    // Convert numeric value back to date format
    double numericDate = ui->kilometrageSpinBox->value();
    QString dateStr = QString::number((int)numericDate);
    if (dateStr.length() == 8) {
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

    // Validate date format
    double numericDate = ui->kilometrageSpinBox->value();
    QString dateStr = QString::number((int)numericDate);
    if (dateStr.length() == 8) {
        QDate date = QDate::fromString(dateStr, "yyyyMMdd");
        if (!date.isValid()) {
            QMessageBox::warning(this, "Erreur", "Date invalide. Utilisez le format AAAAMMJJ (ex: 20251022)");
            ui->kilometrageSpinBox->setFocus();
            return false;
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Date invalide. Utilisez le format AAAAMMJJ (ex: 20251022)");
        ui->kilometrageSpinBox->setFocus();
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
