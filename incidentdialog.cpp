#include "incidentdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QDateTime>

IncidentDialog::IncidentDialog(QWidget *parent) :
    QDialog(parent),
    editMode(false),
    incidentId(-1)
{
    setupDialog();
}

IncidentDialog::IncidentDialog(const QVariantMap &incidentData, QWidget *parent) :
    QDialog(parent),
    editMode(true)
{
    setupDialog();
    loadIncidentData(incidentData);
}

IncidentDialog::~IncidentDialog()
{
}

void IncidentDialog::setupDialog()
{
    setWindowTitle(editMode ? "Modifier l'incident" : "Déclarer un incident");
    setFixedSize(500, 400);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    
    // Type combo box
    typeCombo = new QComboBox();
    typeCombo->addItems({"Panne Électricité", "Fuite Eau", "Accident", "Incendie", "Ascenseur", "Sécurité", "Autre"});
    formLayout->addRow("Type *:", typeCombo);
    
    // Location edit
    localisationEdit = new QLineEdit();
    localisationEdit->setPlaceholderText("Ex: Bâtiment A - Étage 3 - Appartement 301");
    formLayout->addRow("Localisation *:", localisationEdit);
    
    // Date time edit
    dateHeureEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    dateHeureEdit->setDisplayFormat("dd/MM/yyyy HH:mm");
    dateHeureEdit->setCalendarPopup(true);
    formLayout->addRow("Date & Heure *:", dateHeureEdit);
    
    // Gravity spin box
    graviteSpinBox = new QSpinBox();
    graviteSpinBox->setRange(1, 5);
    graviteSpinBox->setValue(3);
    formLayout->addRow("Niveau de Gravité *:", graviteSpinBox);
    
    // Status combo box
    statutCombo = new QComboBox();
    statutCombo->addItems({"En Attente", "En Cours", "Traité"});
    formLayout->addRow("Statut *:", statutCombo);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    QPushButton *cancelButton = new QPushButton("Annuler");
    QPushButton *saveButton = new QPushButton("Enregistrer");
    
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Connect buttons
    connect(cancelButton, &QPushButton::clicked, this, &IncidentDialog::reject);
    connect(saveButton, &QPushButton::clicked, this, &IncidentDialog::on_saveButton_clicked);
    
    // Set default values for new incidents
    if (!editMode) {
        graviteSpinBox->setValue(3);
        statutCombo->setCurrentIndex(0); // "En Attente"
    }
}

void IncidentDialog::loadIncidentData(const QVariantMap &data)
{
    incidentId = data["id"].toInt();
    
    typeCombo->setCurrentText(data.value("type_incident").toString());
    localisationEdit->setText(data["localisation"].toString());
    QDateTime dt = QDateTime::fromString(data.value("date_heure").toString(), "yyyy-MM-dd HH:mm:ss");
    if (!dt.isValid()) {
        dt = QDateTime::fromString(data.value("date_heure").toString(), "dd/MM/yyyy HH:mm");
    }
    if (dt.isValid()) {
        dateHeureEdit->setDateTime(dt);
    }
    graviteSpinBox->setValue(data.value("niveau").toInt());
    statutCombo->setCurrentText(data["statut"].toString());
}

QVariantMap IncidentDialog::getIncidentData() const
{
    QVariantMap data;
    
    if (editMode) {
        data["id"] = incidentId;
    }
    
    data["type_incident"] = typeCombo->currentText();
    data["localisation"] = localisationEdit->text();
    data["date_heure"] = dateHeureEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    data["niveau"] = graviteSpinBox->value();
    data["statut"] = statutCombo->currentText();
    
    return data;
}

bool IncidentDialog::validateInput()
{
    if (localisationEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer la localisation de l'incident");
        localisationEdit->setFocus();
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