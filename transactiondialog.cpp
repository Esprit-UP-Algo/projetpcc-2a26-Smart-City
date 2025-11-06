#include "transactiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QDebug>

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
    codeEdit->setReadOnly(true);
}

void TransactionDialog::setupUi()
{
    setFixedSize(650, 600);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *titleLabel = new QLabel(isEditMode ? "Modifier la Transaction" : "Nouvelle Transaction");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #2C3E50;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    errorLabel = new QLabel();
    errorLabel->setVisible(false);
    mainLayout->addWidget(errorLabel);

    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight);

    codeEdit = new QLineEdit();
    codeEdit->setPlaceholderText("Ex: TRN001");
    codeEdit->setMaxLength(20);
    form->addRow("Code *:", codeEdit);

    montantSpinBox = new QDoubleSpinBox();
    montantSpinBox->setRange(0, 1000000);
    montantSpinBox->setDecimals(2);
    montantSpinBox->setSuffix(" DT");
    form->addRow("Montant *:", montantSpinBox);

    dateEdit = new QDateEdit();
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    form->addRow("Date *:", dateEdit);

    typeCombo = new QComboBox();
    typeCombo->addItems({"Entrée", "Sortie"});
    form->addRow("Type *:", typeCombo);

    categorieCombo = new QComboBox();
    categorieCombo->setEditable(true);
    categorieCombo->addItems({"Loyer", "Charges", "Maintenance", "Autre"});
    form->addRow("Catégorie *:", categorieCombo);

    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Description...");
    form->addRow("Description :", descriptionEdit);

    mainLayout->addLayout(form);

    QHBoxLayout *btns = new QHBoxLayout();
    cancelButton = new QPushButton("Annuler");
    saveButton = new QPushButton("Enregistrer");
    saveButton->setEnabled(true);  // Toujours activé

    cancelButton->setStyleSheet("background: #95A5A6; color: white; border-radius: 8px; padding: 12px;");
    saveButton->setStyleSheet("background: #27AE60; color: white; border-radius: 8px; padding: 12px;");

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveButton, &QPushButton::clicked, this, &QDialog::accept);

    btns->addStretch();
    btns->addWidget(cancelButton);
    btns->addWidget(saveButton);
    mainLayout->addLayout(btns);

    setStyleSheet("background-color: #F8F9F9;");
}

void TransactionDialog::populateFields(const QVariantMap &data)
{
    codeEdit->setText(data["code"].toString());
    montantSpinBox->setValue(data["montant"].toDouble());
    QDate date = QDate::fromString(data["date"].toString(), "yyyy-MM-dd");
    if (date.isValid()) dateEdit->setDate(date);
    typeCombo->setCurrentText(data["type"].toString() == "Entree" ? "Entrée" : "Sortie");
    categorieCombo->setCurrentText(data["categorie"].toString());
    descriptionEdit->setText(data["description"].toString());
}

QVariantMap TransactionDialog::getTransactionData() const
{
    QVariantMap data;
    data["code"] = codeEdit->text().trimmed().toUpper();
    if (data["code"].toString().isEmpty()) data["code"] = "TEMP_" + QDateTime::currentDateTime().toString("hhmmss");
    data["montant"] = montantSpinBox->value();
    data["date"] = dateEdit->date().toString("yyyy-MM-dd");
    data["type"] = typeCombo->currentText() == "Entrée" ? "Entree" : "Sortie";
    data["categorie"] = categorieCombo->currentText().trimmed();
    if (data["categorie"].toString().isEmpty()) data["categorie"] = "Non classé";
    data["description"] = descriptionEdit->text().trimmed();
    return data;
}

// === FONCTIONS VIDES (DÉSactivées) ===
void TransactionDialog::validateForm() { updateSaveButton(); }
void TransactionDialog::updateSaveButton() { saveButton->setEnabled(true); }
bool TransactionDialog::validateInput() { return true; }
void TransactionDialog::showError(const QString &) { errorLabel->setVisible(false); }
void TransactionDialog::clearError() { errorLabel->setVisible(false); }
