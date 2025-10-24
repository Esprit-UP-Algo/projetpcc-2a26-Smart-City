#include "transactiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDate>

TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent), isEditMode(false)
{
    setupUi();
    setWindowTitle("Nouvelle Transaction");
}

TransactionDialog::TransactionDialog(const QVariantMap &transactionData, QWidget *parent)
    : QDialog(parent), isEditMode(true)
{
    setupUi();
    setWindowTitle("Modifier Transaction");
    populateFields(transactionData);
    codeEdit->setReadOnly(true); // Code cannot be changed in edit mode
}

void TransactionDialog::setupUi()
{
    // Allow the layout to determine a comfortable size.
    // Previously fixed to a small value which caused fields to be compressed.
    setMinimumSize(600, 520);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(22);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    QLabel *titleLabel = new QLabel(isEditMode ? "Modifier la Transaction" : "Nouvelle Transaction");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C3E50;");
    mainLayout->addWidget(titleLabel);

    // Form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Code
    codeEdit = new QLineEdit();
    codeEdit->setMinimumHeight(48);
    codeEdit->setPlaceholderText("Ex: TRN001");
    codeEdit->setStyleSheet(R"(
        QLineEdit {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Code *:", codeEdit);

    // Montant
    montantSpinBox = new QDoubleSpinBox();
    montantSpinBox->setMinimumHeight(48);
    montantSpinBox->setRange(0, 1000000);
    montantSpinBox->setDecimals(2);
    montantSpinBox->setSuffix(" DT");
    montantSpinBox->setStyleSheet(R"(
        QDoubleSpinBox {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QDoubleSpinBox:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Montant *:", montantSpinBox);

    // Date
    dateEdit = new QDateEdit();
    dateEdit->setMinimumHeight(48);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setStyleSheet(R"(
        QDateEdit {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QDateEdit:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Date *:", dateEdit);

    // Type
    typeCombo = new QComboBox();
    typeCombo->setMinimumHeight(48);
    typeCombo->addItems({"Entree", "Sortie"});
    typeCombo->setStyleSheet(R"(
        QComboBox {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QComboBox:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Type *:", typeCombo);

    // Catégorie
    categorieCombo = new QComboBox();
    categorieCombo->setMinimumHeight(48);
    categorieCombo->addItems({"Loyer", "Charges", "Services", "Maintenance", "Autre"});
    categorieCombo->setStyleSheet(R"(
        QComboBox {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QComboBox:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Catégorie *:", categorieCombo);

    // Description
    descriptionEdit = new QLineEdit();
    descriptionEdit->setMinimumHeight(48);
    descriptionEdit->setPlaceholderText("Ex: Achat fournitures, Paiement service...");
    descriptionEdit->setStyleSheet(R"(
        QLineEdit {
            padding: 10px;
            border: 1px solid #BDC3C7;
            border-radius: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #4A90E2;
        }
    )");
    formLayout->addRow("Description :", descriptionEdit);

    mainLayout->addLayout(formLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    cancelButton = new QPushButton("Annuler");
    cancelButton->setFixedHeight(48);
    cancelButton->setStyleSheet(R"(
        QPushButton {
            background-color: #95A5A6;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 0 30px;
        }
        QPushButton:hover {
            background-color: #7F8C8D;
        }
    )");
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    saveButton = new QPushButton("Enregistrer");
    saveButton->setFixedHeight(48);
    saveButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4A90E2;
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 0 30px;
        }
        QPushButton:hover {
            background-color: #357ABD;
        }
    )");
    connect(saveButton, &QPushButton::clicked, this, [this]() {
        if (validateInput()) {
            accept();
        }
    });

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    // Set main style
    setStyleSheet("background-color: white;");
}

void TransactionDialog::populateFields(const QVariantMap &data)
{
    codeEdit->setText(data["code"].toString());
    montantSpinBox->setValue(data["montant"].toDouble());
    dateEdit->setDate(QDate::fromString(data["date"].toString(), "yyyy-MM-dd"));

    int typeIndex = typeCombo->findText(data["type"].toString());
    if (typeIndex >= 0) typeCombo->setCurrentIndex(typeIndex);

    int catIndex = categorieCombo->findText(data["categorie"].toString());
    if (catIndex >= 0) categorieCombo->setCurrentIndex(catIndex);

    descriptionEdit->setText(data["description"].toString());
}

bool TransactionDialog::validateInput()
{
    if (codeEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Le code est obligatoire.");
        codeEdit->setFocus();
        return false;
    }

    if (montantSpinBox->value() <= 0) {
        QMessageBox::warning(this, "Validation", "Le montant doit être supérieur à 0.");
        montantSpinBox->setFocus();
        return false;
    }

    return true;
}

QVariantMap TransactionDialog::getTransactionData() const
{
    QVariantMap data;
    data["code"] = codeEdit->text().trimmed();
    data["montant"] = montantSpinBox->value();
    data["date"] = dateEdit->date().toString("yyyy-MM-dd");
    data["type"] = typeCombo->currentText();
    data["categorie"] = categorieCombo->currentText();
    data["description"] = descriptionEdit->text().trimmed();
    return data;
}
