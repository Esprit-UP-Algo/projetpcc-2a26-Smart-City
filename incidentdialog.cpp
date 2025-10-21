#include "incidentdialog.h"
#include "ui_incidentdialog.h"
#include "databasemanager.h"
#include <QMessageBox>

IncidentDialog::IncidentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog),
    editMode(false),
    incidentId(-1)
{
    ui->setupUi(this);
    setupDialog();
}

IncidentDialog::IncidentDialog(const QVariantMap &incidentData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog),
    editMode(true)
{
    ui->setupUi(this);
    setupDialog();
    loadIncidentData(incidentData);
}

IncidentDialog::~IncidentDialog()
{
    delete ui;
}

void IncidentDialog::setupDialog()
{
    setWindowTitle(editMode ? "Modifier l'incident" : "Déclarer un incident");
    
    // Set current date/time
    ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());
    
    // Set default values
    if (!editMode) {
        ui->graviteSpinBox->setValue(3);
        ui->statutCombo->setCurrentIndex(0); // "En Attente"
    }
}

void IncidentDialog::loadIncidentData(const QVariantMap &data)
{
    incidentId = data["id"].toInt();
    
    ui->typeCombo->setCurrentText(data["type"].toString());
    ui->localisationEdit->setText(data["localisation"].toString());
    ui->dateHeureEdit->setDateTime(QDateTime::fromString(data["date_heure"].toString(), "dd/MM/yyyy HH:mm"));
    ui->graviteSpinBox->setValue(data["niveau_gravite"].toInt());
    ui->statutCombo->setCurrentText(data["statut"].toString());
}

QVariantMap IncidentDialog::getIncidentData() const
{
    QVariantMap data;
    
    if (editMode) {
        data["id"] = incidentId;
    }
    
    data["type"] = ui->typeCombo->currentText();
    data["localisation"] = ui->localisationEdit->text();
    data["date_heure"] = ui->dateHeureEdit->dateTime().toString("dd/MM/yyyy HH:mm");
    data["niveau_gravite"] = ui->graviteSpinBox->value();
    data["statut"] = ui->statutCombo->currentText();
    
    return data;
}

bool IncidentDialog::validateInput()
{
    if (ui->localisationEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer la localisation de l'incident");
        ui->localisationEdit->setFocus();
        return false;
    }
    
    return true;
}

void IncidentDialog::on_saveButton_clicked()
{
    if (!validateInput()) {
        return;
    }
    
    accept();
}

void IncidentDialog::on_cancelButton_clicked()
{
    reject();
}
