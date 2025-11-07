#include "incidentdialog.h"
#include "ui_incidentdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

IncidentDialog::IncidentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog),
    editMode(false),
    incidentId(-1)
{
    ui->setupUi(this);
    setupDialog();

    qDebug() << "[IncidentDialog] Constructeur - Mode ajout";
}

IncidentDialog::IncidentDialog(const QVariantMap &incidentData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncidentDialog),
    editMode(true)
{
    ui->setupUi(this);
    setupDialog();
    loadIncidentData(incidentData);

    qDebug() << "[IncidentDialog] Constructeur - Mode édition, ID:" << incidentData["id"].toInt();
}

IncidentDialog::~IncidentDialog()
{
    delete ui;
}

void IncidentDialog::setupDialog()
{
    // Titre du dialogue
    if (editMode) {
        setWindowTitle("Modifier l'incident");
        ui->dialogTitle->setText("Modifier l'Incident");
    } else {
        setWindowTitle("Déclarer un incident");
        ui->dialogTitle->setText("Nouvel Incident");
    }

    // Définir la date et l'heure actuelles
    ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());

    // Valeurs par défaut pour un nouvel incident
    if (!editMode) {
        ui->graviteSpinBox->setValue(3);
        ui->statutCombo->setCurrentIndex(0); // "En Attente"
    }

    // Configurer les limites de date
    ui->dateHeureEdit->setMaximumDateTime(QDateTime::currentDateTime().addDays(1));
    ui->dateHeureEdit->setMinimumDateTime(QDateTime::currentDateTime().addYears(-1));

    qDebug() << "[IncidentDialog] Configuration du dialogue terminée";
}

void IncidentDialog::loadIncidentData(const QVariantMap &data)
{
    incidentId = data["id"].toInt();

    ui->typeCombo->setCurrentText(data["type"].toString());
    ui->localisationEdit->setText(data["localisation"].toString());

    // Parser la date depuis la base de données
    QString dateStr = data["date_heure"].toString();
    QDateTime dateTime = QDateTime::fromString(dateStr, "dd/MM/yyyy HH:mm");

    if (!dateTime.isValid()) {
        // Essayer d'autres formats si le premier échoue
        dateTime = QDateTime::fromString(dateStr, "yyyy-MM-dd HH:mm:ss");
    }

    if (dateTime.isValid()) {
        ui->dateHeureEdit->setDateTime(dateTime);
    } else {
        qWarning() << "[IncidentDialog] Format de date invalide:" << dateStr;
        ui->dateHeureEdit->setDateTime(QDateTime::currentDateTime());
    }

    ui->graviteSpinBox->setValue(data["niveau_gravite"].toInt());
    ui->statutCombo->setCurrentText(data["statut"].toString());

    qDebug() << "[IncidentDialog] Données chargées pour l'incident ID:" << incidentId;
}

QVariantMap IncidentDialog::getIncidentData() const
{
    QVariantMap data;

    if (editMode) {
        data["id"] = incidentId;
    }

    data["type"] = ui->typeCombo->currentText();
    data["localisation"] = ui->localisationEdit->text().trimmed();
    data["date_heure"] = ui->dateHeureEdit->dateTime().toString("dd/MM/yyyy HH:mm");
    data["niveau_gravite"] = ui->graviteSpinBox->value();
    data["statut"] = ui->statutCombo->currentText();

    qDebug() << "[IncidentDialog] Données collectées:" << data;

    return data;
}

bool IncidentDialog::validateInput()
{
    qDebug() << "[IncidentDialog] Validation des données...";

    // 1. Validation du type d'incident
    if (ui->typeCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ Veuillez sélectionner un type d'incident.");
        ui->typeCombo->setFocus();
        qWarning() << "[IncidentDialog] Validation échouée: Type d'incident vide";
        return false;
    }

    // 2. Validation de la localisation
    QString localisation = ui->localisationEdit->text().trimmed();

    if (localisation.isEmpty()) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ Veuillez entrer la localisation de l'incident.\n\n"
                             "Exemple: Bâtiment A - Étage 3 - Appartement 301");
        ui->localisationEdit->setFocus();
        qWarning() << "[IncidentDialog] Validation échouée: Localisation vide";
        return false;
    }

    if (localisation.length() < 5) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ La localisation doit contenir au moins 5 caractères.\n\n"
                             "Veuillez fournir des informations plus détaillées.");
        ui->localisationEdit->setFocus();
        ui->localisationEdit->selectAll();
        qWarning() << "[IncidentDialog] Validation échouée: Localisation trop courte ("
                   << localisation.length() << " caractères)";
        return false;
    }

    if (localisation.length() > 500) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ La localisation ne peut pas dépasser 500 caractères.\n\n"
                             "Longueur actuelle: " + QString::number(localisation.length()) + " caractères");
        ui->localisationEdit->setFocus();
        ui->localisationEdit->selectAll();
        qWarning() << "[IncidentDialog] Validation échouée: Localisation trop longue ("
                   << localisation.length() << " caractères)";
        return false;
    }

    // Vérifier que la localisation contient des lettres et pas seulement des chiffres ou symboles
    QRegularExpression lettersRegex("[a-zA-ZÀ-ÿ]");
    if (!lettersRegex.match(localisation).hasMatch()) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ La localisation doit contenir au moins une lettre.\n\n"
                             "Veuillez fournir une description valide.");
        ui->localisationEdit->setFocus();
        ui->localisationEdit->selectAll();
        qWarning() << "[IncidentDialog] Validation échouée: Localisation sans lettres";
        return false;
    }

    // 3. Validation de la date et heure
    QDateTime selectedDateTime = ui->dateHeureEdit->dateTime();
    QDateTime currentDateTime = QDateTime::currentDateTime();

    if (!selectedDateTime.isValid()) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ La date et l'heure sélectionnées sont invalides.\n\n"
                             "Veuillez sélectionner une date et heure valides.");
        ui->dateHeureEdit->setFocus();
        qWarning() << "[IncidentDialog] Validation échouée: Date/heure invalide";
        return false;
    }

    // Vérifier que la date n'est pas dans le futur
    if (selectedDateTime > currentDateTime.addSecs(300)) { // 5 minutes de tolérance
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ La date et l'heure ne peuvent pas être dans le futur.\n\n"
                             "Date sélectionnée: " + selectedDateTime.toString("dd/MM/yyyy HH:mm") + "\n"
                                                                                   "Date actuelle: " + currentDateTime.toString("dd/MM/yyyy HH:mm"));
        ui->dateHeureEdit->setFocus();
        ui->dateHeureEdit->setDateTime(currentDateTime);
        qWarning() << "[IncidentDialog] Validation échouée: Date dans le futur";
        return false;
    }

    // Vérifier que la date n'est pas trop ancienne (plus d'un an)
    if (selectedDateTime < currentDateTime.addYears(-1)) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Confirmation",
            "⚠ La date sélectionnée remonte à plus d'un an.\n\n"
            "Date sélectionnée: " + selectedDateTime.toString("dd/MM/yyyy HH:mm") + "\n\n"
                                                                  "Êtes-vous sûr de vouloir continuer?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            ui->dateHeureEdit->setFocus();
            qDebug() << "[IncidentDialog] Validation annulée: Date trop ancienne";
            return false;
        }
    }

    // 4. Validation du niveau de gravité
    int gravite = ui->graviteSpinBox->value();

    if (gravite < 1 || gravite > 5) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ Le niveau de gravité doit être compris entre 1 et 5.\n\n"
                             "Valeur actuelle: " + QString::number(gravite));
        ui->graviteSpinBox->setFocus();
        ui->graviteSpinBox->setValue(3);
        qWarning() << "[IncidentDialog] Validation échouée: Gravité hors limites (" << gravite << ")";
        return false;
    }

    // 5. Validation du statut
    if (ui->statutCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Erreur de validation",
                             "⚠ Veuillez sélectionner un statut pour l'incident.");
        ui->statutCombo->setFocus();
        qWarning() << "[IncidentDialog] Validation échouée: Statut vide";
        return false;
    }

    // 6. Avertissement pour les incidents critiques
    if (gravite >= 4) {
        QMessageBox::StandardButton reply = QMessageBox::warning(
            this,
            "⚠ Incident Grave/Critique",
            "Vous êtes sur le point de déclarer un incident de niveau " + QString::number(gravite) + ".\n\n"
                                                                                                     "Type: " + ui->typeCombo->currentText() + "\n"
                                                 "Localisation: " + localisation + "\n\n"
                                 "Un incident de ce niveau nécessite une intervention immédiate.\n"
                                 "Avez-vous alerté les services d'urgence si nécessaire?\n\n"
                                 "Confirmer la déclaration?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

        if (reply == QMessageBox::No) {
            qDebug() << "[IncidentDialog] Validation annulée: Incident critique non confirmé";
            return false;
        }
    }

    qDebug() << "[IncidentDialog] ✓ Validation réussie";
    return true;
}

void IncidentDialog::on_saveButton_clicked()
{
    qDebug() << "[IncidentDialog] Bouton Enregistrer cliqué";

    if (!validateInput()) {
        qDebug() << "[IncidentDialog] Validation échouée, enregistrement annulé";
        return;
    }

    qDebug() << "[IncidentDialog] Validation réussie, acceptation du dialogue";
    accept();
}

void IncidentDialog::on_cancelButton_clicked()
{
    qDebug() << "[IncidentDialog] Bouton Annuler cliqué";

    // Demander confirmation si des données ont été saisies
    if (!editMode) {
        QString localisation = ui->localisationEdit->text().trimmed();

        if (!localisation.isEmpty()) {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Confirmation",
                "Des données ont été saisies.\n\n"
                "Êtes-vous sûr de vouloir annuler?",
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No
                );

            if (reply == QMessageBox::No) {
                qDebug() << "[IncidentDialog] Annulation refusée par l'utilisateur";
                return;
            }
        }
    }

    qDebug() << "[IncidentDialog] Dialogue rejeté";
    reject();
}
