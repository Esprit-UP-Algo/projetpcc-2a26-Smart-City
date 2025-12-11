#include "residentspage.h"
#include "ui_residentspage.h"
#include "databasemanager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <random>
#include <chrono>

ResidentsPage::ResidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResidentsPage)
{
    ui->setupUi(this);

    // Force apply custom styling to ensure it works at runtime
    // Use !important equivalent in Qt by applying after setupUi and with setAttribute
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet(R"(
        QWidget#ResidentsPage {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #E3F2FD, stop:0.3 #BBDEFB, stop:0.7 #90CAF9, stop:1 #64B5F6);
            font-family: "Segoe UI";
        }
    )");

    // Force update the addTitle styling
    ui->addTitle->setStyleSheet("QLabel { font-size:16px; font-weight:600; color:#1F2D5C; margin-bottom:8px; }");

    // Default visual state
    ui->tabListButton->setChecked(true);
    ui->tabAddButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    // Load real data directly from Oracle DB
    QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
    setResidents(rows);

    // Optional signal for controller refresh
    emit refreshRequested();
}

ResidentsPage::~ResidentsPage()
{
    delete ui;
}

// ---------------------------------------------------------------
// Table setup
// ---------------------------------------------------------------
void ResidentsPage::setupTableHeaders()
{
    QTableWidget *t = ui->residentsTable;
    t->setColumnCount(10);
    QStringList headers{
        "CIN","Nom","Prénom","Sexe","Téléphone",
        "Email","Date d'entrée","Appartement","Étage","Statut"
    };
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setStretchLastSection(false);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSortingEnabled(false);
}

// ---------------------------------------------------------------
// UI behavior
// ---------------------------------------------------------------
void ResidentsPage::setupUiBehavior()
{
    connect(ui->tabAddButton,  &QPushButton::clicked, this, &ResidentsPage::onTabAdd);
    connect(ui->tabListButton, &QPushButton::clicked, this, &ResidentsPage::onTabList);
    connect(ui->tabStatsButton,&QPushButton::clicked, this, &ResidentsPage::onTabStats);

    connect(ui->saveFormButton,   &QPushButton::clicked, this, &ResidentsPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &ResidentsPage::onCancelForm);

    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ResidentsPage::onFilterTextChanged);
    connect(ui->sortCombo,  QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ResidentsPage::onSortIndexChanged);

    connect(ui->editResidentButton,   &QPushButton::clicked, this, &ResidentsPage::onEditClicked);
    connect(ui->deleteResidentButton, &QPushButton::clicked, this, &ResidentsPage::onDeleteClicked);
}

void ResidentsPage::setupValidation()
{
    // Hide error message initially
    ui->errorLabel->hide();

    // Set input restrictions
    ui->cinEdit->setMaxLength(8);
    ui->cinEdit->setPlaceholderText("8 chiffres");
    ui->emailEdit->setPlaceholderText("exemple@gmail.com");

    // Connect validation events
    connect(ui->cinEdit, &QLineEdit::textChanged, this, &ResidentsPage::onCinTextChanged);
    connect(ui->emailEdit, &QLineEdit::textChanged, this, &ResidentsPage::onEmailTextChanged);
}

// ---------------------------------------------------------------
// Fill table with DB rows
// ---------------------------------------------------------------
void ResidentsPage::setResidents(const QList<QVariantMap> &rows)
{
    QTableWidget *t = ui->residentsTable;
    t->setRowCount(0);
    t->setSortingEnabled(false);

    t->setRowCount(rows.size());
    for (int r = 0; r < rows.size(); ++r) {
        const QVariantMap &m = rows[r];
        auto set = [&](int col, const QString &key){
            auto *item = new QTableWidgetItem(m.value(key).toString());
            t->setItem(r, col, item);
        };
        set(0, "cin");
        set(1, "nom");
        set(2, "prenom");
        set(3, "sexe");
        set(4, "telephone");
        set(5, "email");
        set(6, "date_entree");
        set(7, "appartement");
        set(8, "etage");
        set(9, "statut");
    }
    t->resizeColumnsToContents();
}

// ---------------------------------------------------------------
// Navigation
// ---------------------------------------------------------------
void ResidentsPage::onTabAdd()
{
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

void ResidentsPage::onTabList()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    // Refresh DB data each time user returns
    QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
    setResidents(rows);
}

void ResidentsPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);

    // Calculate and display real-time satisfaction score
    calculateAndDisplaySatisfactionScore();
}

// ---------------------------------------------------------------
// Form logic
// ---------------------------------------------------------------
void ResidentsPage::onSaveForm()
{
    // Validate form before saving
    if (!validateForm()) {
        return; // Error message already shown by validateForm
    }

    QString cin = ui->cinEdit->text().trimmed();

    QVariantMap data;
    data["cin"]         = cin;
    data["nom"]         = ui->nomEdit->text().trimmed();
    data["prenom"]      = ui->prenomEdit->text().trimmed();
    data["sexe"]        = ui->sexeCombo->currentText();
    data["telephone"]   = ui->telephoneEdit->text().trimmed();
    data["email"]       = ui->emailEdit->text().trimmed();
    data["date_entree"] = ui->dateEdit->date().toString("yyyy-MM-dd");
    data["appartement"] = ui->appartementEdit->text().trimmed();
    data["etage"]       = ui->etageLineEdit->text().trimmed();
    data["statut"]      = ui->statutComboBox->currentText();

    // Check if resident exists in DB
    QVariantMap existing = DatabaseManager::instance().getResident(cin);
    bool success = false;

    if (!existing.isEmpty()) {
        // resident exists  update
        success = DatabaseManager::instance().updateResident(cin, data);
    } else {
        // new resident
        success = DatabaseManager::instance().addResident(data);
    }

    if (success)
        QMessageBox::information(this, tr("Succès"), tr("Données enregistrées avec succès."));
    else
        QMessageBox::warning(this, tr("Erreur"), tr("Échec de l'enregistrement."));

    clearForm();
    QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
    setResidents(rows);
    onTabList();
}

void ResidentsPage::onCancelForm()
{
    clearForm();
    onTabList();
}

void ResidentsPage::clearForm()
{
    ui->cinEdit->clear();
    ui->nomEdit->clear();
    ui->prenomEdit->clear();
    ui->telephoneEdit->clear();
    ui->emailEdit->clear();
    ui->appartementEdit->clear();
    ui->etageLineEdit->clear();
    ui->sexeCombo->setCurrentIndex(0);
    ui->statutComboBox->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
    hideError(); // Clear any error messages
}

// ---------------------------------------------------------------
// Search / Sort
// ---------------------------------------------------------------
void ResidentsPage::onFilterTextChanged(const QString &text)
{
    const QString s = text.trimmed();
    QTableWidget *t = ui->residentsTable;
    for (int r = 0; r < t->rowCount(); ++r) {
        const QString cin  = t->item(r, 0) ? t->item(r,0)->text() : "";
        const QString nom  = t->item(r, 1) ? t->item(r,1)->text() : "";
        bool match = s.isEmpty()
                     || cin.contains(s, Qt::CaseInsensitive)
                     || nom.contains(s, Qt::CaseInsensitive);
        t->setRowHidden(r, !match);
    }
}

int ResidentsPage::sortColumnForIndex(int index) const
{
    switch (index) {
    case 0: return 0; // CIN
    case 1: return 1; // Nom
    case 2: return 7; // Appartement
    case 3: return 6; // Date d'entrée
    default: return 0;
    }
}

void ResidentsPage::onSortIndexChanged(int index)
{
    QTableWidget *t = ui->residentsTable;
    int col = sortColumnForIndex(index);
    t->setSortingEnabled(true);
    t->sortItems(col, Qt::AscendingOrder);
}

// ---------------------------------------------------------------
// Row actions
// ---------------------------------------------------------------
void ResidentsPage::onEditClicked()
{
    QTableWidget *t = ui->residentsTable;
    int row = t->currentRow();

    if (row < 0) {
        QMessageBox::information(this, tr("Modifier"), tr("Veuillez sélectionner un résident."));
        return;
    }

    QString cin = t->item(row, 0)->text();

    // Get resident info from DB
    QVariantMap resident = DatabaseManager::instance().getResident(cin);

    if (resident.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de charger les informations du résident."));
        return;
    }

    // Fill form and go to Add/Edit page
    loadResident(resident);

    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

void ResidentsPage::loadResident(const QVariantMap &r)
{
    ui->cinEdit->setText(r.value("cin").toString());
    ui->nomEdit->setText(r.value("nom").toString());
    ui->prenomEdit->setText(r.value("prenom").toString());
    ui->sexeCombo->setCurrentText(r.value("sexe").toString());
    ui->telephoneEdit->setText(r.value("telephone").toString());
    ui->emailEdit->setText(r.value("email").toString());
    ui->dateEdit->setDate(QDate::fromString(r.value("date_entree").toString(), "yyyy-MM-dd"));
    ui->appartementEdit->setText(r.value("appartement").toString());
    ui->etageLineEdit->setText(r.value("etage").toString());
    ui->statutComboBox->setCurrentText(r.value("statut").toString());
}

void ResidentsPage::onDeleteClicked()
{
    QTableWidget *t = ui->residentsTable;
    int row = t->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Supprimer"),
                                 tr("Veuillez sélectionner un résident."));
        return;
    }
    QString cin = t->item(row, 0)->text();
    if (QMessageBox::question(this, tr("Confirmation"),
                              tr("Supprimer le résident %1 ?").arg(cin))
        == QMessageBox::Yes)
    {
        DatabaseManager::instance().deleteResident(cin);
        QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
        setResidents(rows);
    }
}

// ---------------------------------------------------------------
// Validation Methods
// ---------------------------------------------------------------

void ResidentsPage::onCinTextChanged()
{
    QString text = ui->cinEdit->text();
    QString filtered;

    // Keep only digits, max 8 characters
    for (QChar ch : text) {
        if (ch.isDigit() && filtered.length() < 8) {
            filtered += ch;
        }
    }

    if (text != filtered) {
        ui->cinEdit->setText(filtered);
    }

    hideError(); // Hide error when user types
}

void ResidentsPage::onEmailTextChanged()
{
    hideError(); // Hide error when user types
}

bool ResidentsPage::validateForm()
{
    // Check CIN
    if (!validateCin(ui->cinEdit->text())) {
        showError("Erreur : CIN doit contenir exactement 8 chiffres");
        return false;
    }

    // Check Nom
    if (ui->nomEdit->text().trimmed().isEmpty()) {
        showError("Erreur : Le champ Nom est vide");
        return false;
    }

    // Check Prénom
    if (ui->prenomEdit->text().trimmed().isEmpty()) {
        showError("Erreur : Le champ Prénom est vide");
        return false;
    }

    // Check Email
    if (!validateEmail(ui->emailEdit->text())) {
        showError("Erreur : Format email invalide (exemple: nom@gmail.com)");
        return false;
    }

    // Check Date
    if (!ui->dateEdit->date().isValid() || ui->dateEdit->date() > QDate::currentDate()) {
        showError("Erreur : Date d'entrée invalide");
        return false;
    }

    // Check Appartement
    if (ui->appartementEdit->text().trimmed().isEmpty()) {
        showError("Erreur : Le champ Appartement est vide");
        return false;
    }

    // Check Étage
    if (ui->etageLineEdit->text().trimmed().isEmpty()) {
        showError("Erreur : Le champ Étage est vide");
        return false;
    }

    // Check Téléphone
    if (ui->telephoneEdit->text().trimmed().isEmpty()) {
        showError("Erreur : Le champ Téléphone est vide");
        return false;
    }

    return true;
}

bool ResidentsPage::validateCin(const QString &cin)
{
    // Must be exactly 8 digits
    if (cin.length() != 8) return false;

    for (QChar ch : cin) {
        if (!ch.isDigit()) return false;
    }

    return true;
}

bool ResidentsPage::validateEmail(const QString &email)
{
    if (email.trimmed().isEmpty()) return false;

    // Basic email validation with allowed domains
    QStringList allowedDomains = {
        "@gmail.com", "@gmail.tn", "@yahoo.com", "@yahoo.tn",
        "@outlook.com", "@outlook.tn", "@hotmail.com", "@hotmail.tn",
        "@live.com", "@live.tn", "@msn.com", "@msn.tn",
        "@me.com", "@icloud.com", "@mail.com", "@protonmail.com"
    };

    // Check basic email format: xxx@xxx.xxx
    if (!email.contains("@") || email.count("@") != 1) return false;

    QStringList parts = email.split("@");
    if (parts.size() != 2 || parts[0].isEmpty() || parts[1].isEmpty()) return false;

    // Check if domain is allowed
    QString domain = "@" + parts[1].toLower();
    for (const QString &allowedDomain : allowedDomains) {
        if (domain == allowedDomain.toLower()) return true;
    }

    return false;
}

void ResidentsPage::showError(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void ResidentsPage::hideError()
{
    ui->errorLabel->hide();
    ui->errorLabel->clear();
}

// ---------------------------------------------------------------
// Smart Statistics Calculation
// ---------------------------------------------------------------

QPair<int, QString> ResidentsPage::calculateSatisfactionScore()
{
    DatabaseManager &db = DatabaseManager::instance();

    // Modern random number generator
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> percentDist(0, 99);
    static std::uniform_int_distribution<int> incidentDist(1, 3);

    // Get all residents from database
    QList<QVariantMap> residents = db.getAllResidents();

    if (residents.isEmpty()) {
        return qMakePair(0, "Aucun résident trouvé dans la base de données.");
    }

    int totalScore = 0;
    int residentCount = residents.size();
    int totalPoints = residentCount * 100; // Base score of 100 per resident

    // Algorithm factors
    int renewalBonus = 10;      // +10 points for contract renewal
    int earlyDeparturePenalty = 20;  // -20 points for early departure
    int incidentPenalty = 5;    // -5 points per incident
    int noIncidentBonus = 5;    // +5 points for 6 months without incidents

    int renewals = 0;
    int earlyDepartures = 0;
    int incidents = 0;
    int cleanRecords = 0;

    // Simulated behavior analysis based on resident data
    for (const QVariantMap &resident : residents) {
        QString status = resident.value("STATUT").toString();
        QDate entryDate = resident.value("DATE_ENTREE").toDate();
        QDate currentDate = QDate::currentDate();

        // Analyze resident behavior patterns
        if (status == "Propriétaire") {
            renewals++; // Owners are more likely to renew
        } else if (status == "Locataire" && entryDate.addYears(1) < currentDate) {
            renewals++; // Long-term tenants likely renewed
        }

        // Simulate incidents based on resident age and type
        int monthsSinceEntry = entryDate.daysTo(currentDate) / 30;
        if (monthsSinceEntry >= 6) {
            // 80% chance of clean record for long-term residents
            if (percentDist(gen) < 80) {
                cleanRecords++;
            } else {
                incidents += incidentDist(gen); // 1-3 incidents
            }
        }

        // Simulate early departures (5% chance for tenants)
        if (status == "Locataire" && percentDist(gen) < 5) {
            earlyDepartures++;
        }
    }

    // Calculate final score
    totalScore = totalPoints
                 + (renewals * renewalBonus)
                 + (cleanRecords * noIncidentBonus)
                 - (earlyDepartures * earlyDeparturePenalty)
                 - (incidents * incidentPenalty);

    // Normalize to 0-100 scale
    int finalScore = qMax(0, qMin(100, (totalScore * 100) / totalPoints));

    // Generate analysis text based on score
    QString analysis;
    if (finalScore >= 85) {
        analysis = "Excellente stabilité. Les résidents montrent une très forte satisfaction et fidélité.";
    } else if (finalScore >= 75) {
        analysis = "Bonne stabilité. Quelques incidents mineurs ont été signalés ce mois-ci.";
    } else if (finalScore >= 65) {
        analysis = "Stabilité modérée. Surveillance recommandée des tendances de satisfaction.";
    } else if (finalScore >= 50) {
        analysis = "Attention requise. Plusieurs problèmes de satisfaction détectés.";
    } else {
        analysis = "Situation critique. Action immédiate nécessaire pour améliorer la satisfaction.";
    }

    return qMakePair(finalScore, analysis);
}

void ResidentsPage::calculateAndDisplaySatisfactionScore()
{
    QPair<int, QString> result = calculateSatisfactionScore();
    int score = result.first;
    QString analysis = result.second;

    // Update the UI elements
    ui->satisfactionScore->setText(QString("Score global de satisfaction : %1 / 100").arg(score));
    ui->satisfactionAnalysis->setText(analysis);
}
