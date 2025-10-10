#include "residentdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QDate>

ResidentDialog::ResidentDialog(QWidget *parent, const QVariantMap &data)
    : QDialog(parent), editMode(!data.isEmpty())
{
    setupUI();
    
    if (editMode) {
        loadResidentData(data);
        cinEdit->setReadOnly(true); // CIN cannot be changed in edit mode
        setWindowTitle("Modifier Résident");
    } else {
        setWindowTitle("Ajouter Résident");
    }
    
    setModal(true);
    resize(600, 500);
}

ResidentDialog::~ResidentDialog()
{
}

void ResidentDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title
    QLabel *titleLabel = new QLabel(editMode ? "Modifier Résident" : "Ajouter Résident");
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 24px;
            font-weight: bold;
            color: #4A90E2;
            padding-bottom: 10px;
        }
    )");
    mainLayout->addWidget(titleLabel);
    
    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);
    
    // CIN
    cinEdit = new QLineEdit();
    cinEdit->setPlaceholderText("Ex: 12345678");
    cinEdit->setStyleSheet(R"(
        QLineEdit {
            padding: 10px;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("CIN *:", cinEdit);
    
    // Nom
    nomEdit = new QLineEdit();
    nomEdit->setPlaceholderText("Nom de famille");
    nomEdit->setStyleSheet(cinEdit->styleSheet());
    formLayout->addRow("Nom *:", nomEdit);
    
    // Prénom
    prenomEdit = new QLineEdit();
    prenomEdit->setPlaceholderText("Prénom");
    prenomEdit->setStyleSheet(cinEdit->styleSheet());
    formLayout->addRow("Prénom *:", prenomEdit);
    
    // Sexe
    sexeCombo = new QComboBox();
    sexeCombo->addItems({"Homme", "Femme"});
    sexeCombo->setStyleSheet(R"(
        QComboBox {
            padding: 10px;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            font-size: 14px;
        }
        QComboBox:focus {
            border: 2px solid #4A90E2;
        }
        QComboBox::drop-down {
            border: none;
        }
    )");
    formLayout->addRow("Sexe *:", sexeCombo);
    
    // Téléphone
    telephoneEdit = new QLineEdit();
    telephoneEdit->setPlaceholderText("+216 XX XXX XXX");
    telephoneEdit->setStyleSheet(cinEdit->styleSheet());
    formLayout->addRow("Téléphone *:", telephoneEdit);
    
    // Email
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("exemple@email.com");
    emailEdit->setStyleSheet(cinEdit->styleSheet());
    formLayout->addRow("Email *:", emailEdit);
    
    // Appartement
    appartementEdit = new QLineEdit();
    appartementEdit->setPlaceholderText("Ex: A101");
    appartementEdit->setStyleSheet(cinEdit->styleSheet());
    formLayout->addRow("Appartement *:", appartementEdit);
    
    // Étage
    etageSpinBox = new QSpinBox();
    etageSpinBox->setRange(-1, 20); // -1 for basement
    etageSpinBox->setValue(0);
    etageSpinBox->setPrefix("Étage ");
    etageSpinBox->setStyleSheet(R"(
        QSpinBox {
            padding: 10px;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            font-size: 14px;
        }
        QSpinBox:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Étage *:", etageSpinBox);
    
    // Statut
    statutCombo = new QComboBox();
    statutCombo->addItems({"Locataire", "Proprietaire"});
    statutCombo->setStyleSheet(sexeCombo->styleSheet());
    formLayout->addRow("Statut *:", statutCombo);
    
    // Date d'entrée
    dateEntreeEdit = new QDateEdit();
    dateEntreeEdit->setDate(QDate::currentDate());
    dateEntreeEdit->setCalendarPopup(true);
    dateEntreeEdit->setDisplayFormat("dd/MM/yyyy");
    dateEntreeEdit->setStyleSheet(R"(
        QDateEdit {
            padding: 10px;
            border: 2px solid #E0E0E0;
            border-radius: 8px;
            font-size: 14px;
        }
        QDateEdit:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Date d'entrée *:", dateEntreeEdit);
    
    mainLayout->addLayout(formLayout);
    
    // Required fields note
    QLabel *noteLabel = new QLabel("* Champs obligatoires");
    noteLabel->setStyleSheet("QLabel { color: #E74C3C; font-size: 12px; font-style: italic; }");
    mainLayout->addWidget(noteLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    cancelButton = new QPushButton("Annuler");
    cancelButton->setStyleSheet(R"(
        QPushButton {
            background-color: #E0E0E0;
            color: #333;
            border: none;
            border-radius: 8px;
            padding: 12px 30px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #C0C0C0;
        }
    )");
    connect(cancelButton, &QPushButton::clicked, this, &ResidentDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);
    
    saveButton = new QPushButton(editMode ? "Enregistrer" : "Ajouter");
    saveButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4A90E2;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 12px 30px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3A80D2;
        }
    )");
    connect(saveButton, &QPushButton::clicked, this, &ResidentDialog::onSaveClicked);
    buttonLayout->addWidget(saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    setStyleSheet("QDialog { background-color: white; }");
}

void ResidentDialog::loadResidentData(const QVariantMap &data)
{
    cinEdit->setText(data["cin"].toString());
    nomEdit->setText(data["nom"].toString());
    prenomEdit->setText(data["prenom"].toString());
    sexeCombo->setCurrentText(data["sexe"].toString());
    telephoneEdit->setText(data["telephone"].toString());
    emailEdit->setText(data["email"].toString());
    appartementEdit->setText(data["appartement"].toString());
    etageSpinBox->setValue(data["etage"].toInt());
    statutCombo->setCurrentText(data["statut"].toString());
    dateEntreeEdit->setDate(QDate::fromString(data["date_entree"].toString(), "yyyy-MM-dd"));
}

bool ResidentDialog::validateForm()
{
    // Check required fields
    if (cinEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le CIN est obligatoire.");
        cinEdit->setFocus();
        return false;
    }
    
    if (nomEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le nom est obligatoire.");
        nomEdit->setFocus();
        return false;
    }
    
    if (prenomEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le prénom est obligatoire.");
        prenomEdit->setFocus();
        return false;
    }
    
    if (telephoneEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le téléphone est obligatoire.");
        telephoneEdit->setFocus();
        return false;
    }
    
    if (emailEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "L'email est obligatoire.");
        emailEdit->setFocus();
        return false;
    }
    
    // Validate email format
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(emailEdit->text().trimmed()).hasMatch()) {
        QMessageBox::warning(this, "Validation", "Format d'email invalide.");
        emailEdit->setFocus();
        return false;
    }
    
    if (appartementEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "L'appartement est obligatoire.");
        appartementEdit->setFocus();
        return false;
    }
    
    return true;
}

void ResidentDialog::onSaveClicked()
{
    if (!validateForm()) {
        return;
    }
    
    residentData.clear();
    residentData["cin"] = cinEdit->text().trimmed();
    residentData["nom"] = nomEdit->text().trimmed();
    residentData["prenom"] = prenomEdit->text().trimmed();
    residentData["sexe"] = sexeCombo->currentText();
    residentData["telephone"] = telephoneEdit->text().trimmed();
    residentData["email"] = emailEdit->text().trimmed();
    residentData["appartement"] = appartementEdit->text().trimmed();
    residentData["etage"] = etageSpinBox->value();
    residentData["statut"] = statutCombo->currentText();
    residentData["date_entree"] = dateEntreeEdit->date().toString("yyyy-MM-dd");
    
    accept();
}

void ResidentDialog::onCancelClicked()
{
    reject();
}

QVariantMap ResidentDialog::getResidentData() const
{
    return residentData;
}
