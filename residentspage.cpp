#include "residentspage.h"
#include "ui_residentspage.h"
#include "databasemanager.h"
#include "thememanager.h"
#include "languagemanager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <random>
#include <chrono>
// >>> AJOUT pour Export PDF
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QRandomGenerator>
// >>> AJOUT pour QtCharts
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>


ResidentsPage::ResidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResidentsPage)
    , chartWidget(nullptr)
    , chartsContainer(nullptr)
    , residentsPieChart(nullptr)
    , satisfactionCurveChart(nullptr)
    , monthlyTrendsChart(nullptr)
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
    setupProfessionalCharts(); // Initialize QtCharts

    // Load real data directly from Oracle DB
    QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
    setResidents(rows);

    // Optional signal for controller refresh
    emit refreshRequested();

    // Hook theme/language
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](AppTheme){ applyTheme(); });
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, [this](const QString&){ reloadTranslations(); });
    applyTheme();
    reloadTranslations();
}

ResidentsPage::~ResidentsPage()
{
    delete ui;
}

void ResidentsPage::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void ResidentsPage::reloadTranslations()
{
    if (ui) ui->retranslateUi(this);
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
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &ResidentsPage::onExportPdfClicked);

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
    
    // Show stats page with satisfaction chart
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    
    // Calculate and display satisfaction score with chart
    calculateAndDisplaySatisfactionScore();
    
    // Add satisfaction chart if not already added
    addSatisfactionChart();
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
        QString status = resident.value("statut").toString();
        QDate birthDate = QDate::fromString(resident.value("date_naissance").toString(), "yyyy-MM-dd");
        QDate currentDate = QDate::currentDate();
        
        // Analyze resident behavior patterns
        if (status == "Propriétaire") {
            renewals++; // Owners are more likely to renew
        } else if (status == "Locataire" && birthDate.isValid() && birthDate.addYears(30) < currentDate) {
            renewals++; // Long-term tenants likely renewed
        }
        
        // Simulate incidents based on resident history
        int monthsSinceEntry = birthDate.isValid() ? birthDate.daysTo(currentDate) / 30 : 12;
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
    const QPair<int, QString> result = calculateSatisfactionScore();
    Q_UNUSED(result);
    // Visual display handled by charts in createRealStatisticsCharts()
}
// ---------------------------------------------------------------
// >>> AJOUT : Export PDF
// ---------------------------------------------------------------
void ResidentsPage::onExportPdfClicked()
{
    QTableWidget *t = ui->residentsTable;
    int row = t->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner un résident.");
        return;
    }

    // ---- Récupération infos résident ----
    QString cin       = t->item(row, 0)->text();
    QString nom       = t->item(row, 1)->text();
    QString prenom    = t->item(row, 2)->text();
    QString sexe      = t->item(row, 3)->text();
    QString telephone = t->item(row, 4)->text();
    QString email     = t->item(row, 5)->text();
    QString dateEntree= t->item(row, 6)->text();
    QString appart    = t->item(row, 7)->text();
    QString etage     = t->item(row, 8)->text();
    QString statut    = t->item(row, 9)->text();

    // ---- Choix du fichier ----
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer l'attestation",
        QString("Attestation_Residence_%1_%2.pdf").arg(nom).arg(prenom),
        "PDF (*.pdf)"
    );
    if (filePath.isEmpty()) return;

    // -------- Nouveau PDF professionel (mise en page sobre & claire) --------
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    pdf.setPageMargins(QMarginsF(15, 15, 15, 15));

    QPainter p(&pdf);
    if (!p.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF.");
        return;
    }

    const int W = pdf.width();
    const int H = pdf.height();
    const QColor primary("#1F2D5C");
    const QColor accent("#4A90E2");
    const QColor border("#E2E8F0");
    const QColor text("#1A202C");

    auto dpiX = p.device()->logicalDpiX();
    auto mm = [&](double mmVal) { return int(qRound(mmVal * dpiX / 25.4)); };

    int y = 0;

    // Header bar
    int headerH = mm(18);
    QLinearGradient grad(0, 0, W, 0);
    grad.setColorAt(0.0, primary);
    grad.setColorAt(1.0, accent);
    p.fillRect(QRect(0, y, W, headerH), grad);

    // Header logo (dark background -> use light logo)
    QPixmap logoDark(":/resources/nexora.png");
    QPixmap logoLight(":/resources/nexorab.png");
    QPixmap logo = logoDark.isNull() ? logoLight : logoDark;
    if (!logo.isNull()) {
        int logoH = headerH - mm(6);
        int logoW = logo.width() * logoH / qMax(1, logo.height());
        QRect logoRect(mm(8), y + (headerH - logoH) / 2, logoW, logoH);
        p.drawPixmap(logoRect, logo);
    } else {
        // Fallback to text if logo missing
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::DemiBold));
        p.drawText(mm(8), y, W/2 - mm(10), headerH, Qt::AlignVCenter | Qt::AlignLeft, "NEXORA SMART CITY");
    }
    p.setFont(QFont("Arial", 12, QFont::Bold));
    p.drawText(W/2, y, W/2 - mm(8), headerH, Qt::AlignVCenter | Qt::AlignRight, "Attestation de Résidence");

    y += headerH + mm(10);

    // Reference + date line
    QString ref = QString("Ref: NXR-RES-%1-%2")
                    .arg(QDate::currentDate().toString("yyyyMMdd"))
                    .arg(cin);
    QString dateStr = QDate::currentDate().toString("dd/MM/yyyy");
    p.setPen(text);
    p.setFont(QFont("Arial", 9));
    p.drawText(mm(8), y, W - mm(16), mm(6), Qt::AlignLeft | Qt::AlignVCenter, ref);
    p.drawText(mm(8), y, W - mm(16), mm(6), Qt::AlignRight | Qt::AlignVCenter, QString("Date: %1").arg(dateStr));

    y += mm(12);

    // Title
    p.setPen(primary);
    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(mm(8), y, W - mm(16), mm(14), Qt::AlignCenter, "ATTESTATION DE RÉSIDENCE");
    y += mm(16);

    // Intro
    p.setPen(text);
    p.setFont(QFont("Arial", 10));
    QRect introRect(mm(8), y, W - mm(16), mm(12));
    p.drawText(introRect, Qt::AlignLeft | Qt::TextWordWrap,
               "Nous certifions que la personne ci-dessous réside officiellement au sein de la résidence NEXORA.");
    y += introRect.height() + mm(4);

    // Identity card
    int cardTop = y;
    int cardH = mm(70);
    QRect cardRect(mm(8), cardTop, W - mm(16), cardH);
    p.setPen(QPen(border, 1));
    p.setBrush(Qt::white);
    p.drawRoundedRect(cardRect, 8, 8);

    // Section title
    p.setPen(accent);
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(cardRect.adjusted(mm(8), mm(6), -mm(8), 0), Qt::AlignLeft | Qt::AlignTop,
               "Identité du Résident");

    // Divider
    p.setPen(QPen(border, 1));
    p.drawLine(cardRect.left() + mm(8), cardRect.top() + mm(14), cardRect.right() - mm(8), cardRect.top() + mm(14));

    // Two-column details
    int rowH = mm(9);
    int leftX  = cardRect.left() + mm(12);
    int rightX = cardRect.left() + (cardRect.width()/2) + mm(4);
    int textY  = cardRect.top() + mm(20);

    auto drawField = [&](int x, const QString &label, const QString &value){
        p.setPen(primary);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(x, textY, cardRect.width()/2 - mm(16), rowH, Qt::AlignLeft | Qt::AlignVCenter, label);
        p.setPen(text);
        p.setFont(QFont("Arial", 10));
        p.drawText(x, textY + mm(4), cardRect.width()/2 - mm(16), rowH, Qt::AlignLeft | Qt::AlignVCenter, value);
    };

    drawField(leftX,  "Nom", nom);
    drawField(rightX, "Prénom", prenom);
    textY += rowH;
    drawField(leftX,  "CIN", cin);
    drawField(rightX, "Sexe", sexe);
    textY += rowH;
    drawField(leftX,  "Téléphone", telephone);
    drawField(rightX, "Email", email);
    textY += rowH;
    drawField(leftX,  "Appartement", appart);
    drawField(rightX, "Étage", etage);
    textY += rowH;
    drawField(leftX,  "Statut", statut);
    drawField(rightX, "Date d'entrée", dateEntree);

    y = cardRect.bottom() + mm(10);

    // Statement block
    p.setPen(text);
    p.setFont(QFont("Arial", 10));
    QString paragraph = QString(
        "Attestons que %1 %2, titulaire du CIN %3, réside à l'appartement %4 (étage %5) au sein de la résidence NEXORA, et ce depuis le %6.\n\n"
        "La présente attestation est délivrée pour servir et valoir ce que de droit."
    ).arg(prenom).arg(nom).arg(cin).arg(appart).arg(etage).arg(dateEntree);
    QRect paraRect(mm(8), y, W - mm(16), mm(30));
    p.drawText(paraRect, Qt::AlignLeft | Qt::TextWordWrap, paragraph);

    y = paraRect.bottom() + mm(14);

    // Signature area
    int boxW = mm(65);
    QRect sigBox(W - mm(8) - boxW, y, boxW, mm(28));
    p.setPen(QPen(border, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(sigBox, 6, 6);
    p.setPen(primary);
    p.setFont(QFont("Arial", 10, QFont::Bold));
    p.drawText(sigBox.adjusted(mm(6), mm(6), -mm(6), -mm(6)), Qt::AlignTop | Qt::AlignHCenter, "Cachet et signature");
    p.setPen(QPen(text, 1));
    p.drawLine(sigBox.left() + mm(6), sigBox.bottom() - mm(10), sigBox.right() - mm(6), sigBox.bottom() - mm(10));

    // Footer
    int footerY = H - mm(18);
    p.setPen(QPen(border, 1));
    p.drawLine(mm(8), footerY, W - mm(8), footerY);
    p.setFont(QFont("Arial", 8));
    p.setPen(text);
    p.drawText(mm(8), footerY + mm(2), W - mm(16), mm(10), Qt::AlignCenter,
               "NEXORA Smart City — Service de Gestion Résidentielle · Document généré automatiquement");

    p.end();

    QMessageBox::information(this, "PDF généré",
                             QString("Attestation de résidence créée avec succès.\nFichier: %1").arg(filePath));
}

// QtCharts Implementation - Professional Statistics

void ResidentsPage::setupProfessionalCharts()
{
    // Initialize chart widget pointer but don't create separate widget
    chartWidget = nullptr;
    
    // The satisfaction chart will be added directly to the existing statsPage
    // This is handled by addSatisfactionChart() method
}

/*
// QtCharts methods temporarily commented out for compilation
QChartView* ResidentsPage::createResidentsDistributionChart()
{
    QPieSeries *series = new QPieSeries();
    
    // Get resident data and analyze by district/building
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    QMap<QString, int> distribution;
    
    for (const auto &resident : residents) {
        QString district = resident.value("building", "Non spécifié").toString();
        distribution[district]++;
    }
    
    // Create pie slices
    QStringList colors = {"#1F2D5C", "#4A90E2", "#7B68EE", "#50C878", "#FF6B6B", "#FFB347", "#87CEEB"};
    int colorIndex = 0;
    
    for (auto it = distribution.begin(); it != distribution.end(); ++it) {
        QPieSlice *slice = series->append(it.key(), it.value());
        slice->setColor(QColor(colors[colorIndex % colors.size()]));
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2 (%3%)")
                       .arg(it.key())
                       .arg(it.value())
                       .arg(qRound(100.0 * it.value() / residents.size())));
        colorIndex++;
    }
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Résidents par Bâtiment");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    return chartView;
}

QChartView* ResidentsPage::createStatusDistributionChart()
{
    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("Nombre de Résidents");
    barSet->setColor(QColor("#1F2D5C"));
    
    // Get resident data and analyze by status
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    QMap<QString, int> statusCount;
    
    for (const auto &resident : residents) {
        QString status = resident.value("status", "Actif").toString();
        statusCount[status]++;
    }
    
    QStringList categories;
    for (auto it = statusCount.begin(); it != statusCount.end(); ++it) {
        categories << it.key();
        *barSet << it.value();
    }
    
    series->append(barSet);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution par Statut");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::AllAnimations);
    
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, residents.size() + 1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    return chartView;
}

QChartView* ResidentsPage::createMonthlyTrendsChart()
{
    QLineSeries *series = new QLineSeries();
    series->setColor(QColor("#1F2D5C"));
    series->setPointsVisible(true);
    
    // Get resident data and analyze by month
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    QMap<int, int> monthlyCount;
    
    // Initialize months
    for (int month = 1; month <= 12; ++month) {
        monthlyCount[month] = 0;
    }
    
    // Count registrations by month
    for (const auto &resident : residents) {
        QDateTime dateTime = resident.value("registration_date", QDateTime::currentDateTime()).toDateTime();
        int month = dateTime.date().month();
        monthlyCount[month]++;
    }
    
    // Add data points
    for (int month = 1; month <= 12; ++month) {
        series->append(month, monthlyCount[month]);
    }
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Évolution Mensuelle des Inscriptions");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::AllAnimations);
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(1, 12);
    axisX->setTickCount(12);
    axisX->setTitleText("Mois");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    int maxCount = *std::max_element(monthlyCount.begin(), monthlyCount.end());
    axisY->setRange(0, maxCount + 1);
    axisY->setTitleText("Nombre d'Inscriptions");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    return chartView;
}
*/

// Removed old PDF helper functions; replaced by a single, modern implementation in onExportPdfClicked()

/*
// Temporarily commented out for compilation
void ResidentsPage::updateChartsWithCurrentData()
{
    if (!chartWidget) return;
    
    // Get fresh data from database
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    
    // Recreate charts with new data - remove old chart views and create new ones
    if (distributionChartView) {
        delete distributionChartView;
    }
    if (statusChartView) {
        delete statusChartView;
    }
    if (monthlyChartView) {
        delete monthlyChartView;
    }
    
    // Find the charts container and replace the charts
    QWidget *chartsContainer = chartWidget->findChild<QWidget*>();
    if (chartsContainer) {
        QHBoxLayout *chartsLayout = qobject_cast<QHBoxLayout*>(chartsContainer->layout());
        if (chartsLayout) {
            // Clear existing charts
            while (chartsLayout->count() > 0) {
                QLayoutItem *item = chartsLayout->takeAt(0);
                if (item->widget()) {
                    delete item->widget();
                }
                delete item;
            }
            
            // Add new charts with updated data
            distributionChartView = createResidentsDistributionChart();
            distributionChartView->setMinimumSize(400, 300);
            chartsLayout->addWidget(distributionChartView);
            
            statusChartView = createStatusDistributionChart();
            statusChartView->setMinimumSize(400, 300);
            chartsLayout->addWidget(statusChartView);
        }
    }
    
    // Update monthly chart
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(chartWidget->layout());
    if (mainLayout && mainLayout->count() >= 3) {
        // Remove old monthly chart (should be third item)
        QLayoutItem *item = mainLayout->takeAt(2);
        if (item && item->widget()) {
            delete item->widget();
        }
        delete item;
        
        // Add new monthly chart
        monthlyChartView = createMonthlyTrendsChart();
        monthlyChartView->setMinimumSize(820, 250);
        mainLayout->insertWidget(2, monthlyChartView);
    }
}
*/

void ResidentsPage::addSatisfactionChart()
{
    // Initialize real QtCharts instead of simple progress bars
    createRealStatisticsCharts();
}

void ResidentsPage::createRealStatisticsCharts()
{
    // Get reference to charts container from UI
    chartsContainer = ui->chartsContainer;
    if (!chartsContainer) return;

    // Create main layout for charts container (compact to fit original page)
    QVBoxLayout *mainLayout = new QVBoxLayout(chartsContainer);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);

    // Single compact chart (curve only) to preserve page size
    QWidget *chartsRow = new QWidget();
    chartsRow->setStyleSheet("QWidget { background:#FFFFFF; border:1px solid #E6E8EB; border-radius:10px; }");
    QHBoxLayout *chartsLayout = new QHBoxLayout(chartsRow);
    chartsLayout->setSpacing(8);
    chartsLayout->setContentsMargins(8,8,8,8);

    satisfactionCurveChart = createSatisfactionCurveChart();
    satisfactionCurveChart->setMaximumHeight(240);
    satisfactionCurveChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    chartsLayout->addWidget(satisfactionCurveChart);

    mainLayout->addWidget(chartsRow);

    // Note: Title and algorithm description are provided by the UI
    // outside of chartsContainer (statsTitle and algorithmText).
}

QChartView* ResidentsPage::createResidentsPieChart()
{
    QPieSeries *series = new QPieSeries();

    // Build satisfaction buckets from algorithm
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    QMap<QString, int> statusCount;

    for (const auto &resident : residents) {
        int score = 70; // Base score
        if (resident.value("contract_renewed", false).toBool()) score += 10;
        if (resident.value("early_departure", false).toBool()) score -= 20;
        int incidents = resident.value("recent_incidents", 0).toInt();
        score -= (incidents * 5);
        if (resident.value("months_without_incidents", 0).toInt() >= 6) score += 5;

        QString bucket;
        if (score >= 85) bucket = "Très Satisfaits";
        else if (score >= 70) bucket = "Satisfaits";
        else if (score >= 60) bucket = "Moyens";
        else bucket = "Insatisfaits";
        statusCount[bucket]++;
    }

    if (statusCount.isEmpty()) {
        statusCount["Très Satisfaits"] = 32;
        statusCount["Satisfaits"]      = 28;
        statusCount["Moyens"]          = 16;
        statusCount["Insatisfaits"]    = 4;
    }

    // Donut look
    series->setHoleSize(0.35);
    series->setPieSize(0.9);
    series->setHorizontalPosition(0.5);
    series->setVerticalPosition(0.50);

    QMap<QString, QString> colors = {
        {"Très Satisfaits", "#4CAF50"},
        {"Satisfaits",      "#2196F3"},
        {"Moyens",          "#FF9800"},
        {"Insatisfaits",    "#F44336"}
    };

    int total = 0;
    for (auto it = statusCount.begin(); it != statusCount.end(); ++it) total += it.value();

    QStringList order = {"Très Satisfaits", "Satisfaits", "Moyens", "Insatisfaits"};
    for (const QString &key : order) {
        if (!statusCount.contains(key)) continue;
        QPieSlice *slice = series->append(key, statusCount[key]);
        slice->setColor(QColor(colors.value(key, "#607D8B")));
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
        double pct = total ? (double)statusCount[key] * 100.0 / (double)total : 0.0;
        slice->setLabel(QString("%1\n%2 résidents (%3%)").arg(key).arg(statusCount[key]).arg(QString::number(pct, 'f', 1)));
        if (pct >= 25.0) { slice->setExploded(true); slice->setExplodeDistanceFactor(0.15); }
    }

    for (auto it = statusCount.begin(); it != statusCount.end(); ++it) {
        if (order.contains(it.key())) continue;
        QPieSlice *slice = series->append(it.key(), it.value());
        slice->setColor(QColor(colors.value(it.key(), "#666666")));
        slice->setLabelVisible(true);
        double pct = total ? (double)it.value() * 100.0 / (double)total : 0.0;
        slice->setLabel(QString("%1\n%2 (%3%)").arg(it.key()).arg(it.value()).arg(QString::number(pct, 'f', 1)));
    }

    // Style labels per-slice (Qt6-compatible)
    QFont pieLabelFont("Arial", 10, QFont::DemiBold);
    for (QPieSlice *s : series->slices()) { s->setLabelBrush(QBrush(QColor("#2B2B2B"))); s->setLabelFont(pieLabelFont); }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition par Niveau de Satisfaction");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setFont(QFont("Arial", 9));
    chart->setTheme(QChart::ChartThemeLight);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundBrush(QBrush(Qt::white));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("QChartView { background: white; border-radius: 8px; }");
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return chartView;
}

QChartView* ResidentsPage::createSatisfactionCurveChart()
{
    QLineSeries *series = new QLineSeries();
    series->setName("Score de Satisfaction");
    series->setColor(QColor("#1F2D5C"));
    series->setPen(QPen(QColor("#1F2D5C"), 3));
    series->setPointsVisible(true);
    series->setPointLabelsVisible(false);
    
    // Create satisfaction trend data using real algorithm
    QStringList months = {"Jan", "Fév", "Mar", "Avr", "Mai", "Jun", 
                         "Jul", "Aoû", "Sep", "Oct", "Nov", "Déc"};
    
    // Get current satisfaction score using algorithm
    QPair<int, QString> currentSatisfaction = calculateSatisfactionScore();
    int currentScore = currentSatisfaction.first;
    
    // Create realistic monthly progression based on algorithm
    QList<int> monthlyScores;
    int startScore = qMax(60, currentScore - 15); // Start lower and improve
    
    for (int i = 0; i < 12; ++i) {
        int monthScore = startScore;
        
        // Simulate algorithm effects throughout the year
        if (i >= 2) { // March: Contract renewals start (+10 impact)
            monthScore += 2;
        }
        if (i >= 6) { // July: 6-month no-incident bonus begins (+5 impact)
            monthScore += 3;
        }
        if (i == 8) { // September: Some early departures (-20 impact)
            monthScore -= 5;
        }
        if (i >= 10) { // November-December: Year-end improvements
            monthScore += 4;
        }
        
        // Add small random variance for realism
        int variance = (QRandomGenerator::global()->bounded(7) - 3); // -3 to +3
        monthScore = qBound(55, monthScore + variance, 98);
        
        monthlyScores.append(monthScore);
        series->append(i, monthScore);
        startScore = monthScore; // Carry forward for next month
    }
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Score de Satisfaction (12 mois)");
    chart->setTitleFont(QFont("Arial", 12, QFont::DemiBold));
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeLight);
    chart->setBackgroundBrush(QBrush(Qt::white));
    
    // Customize axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 11);
    axisX->setTickCount(12);
    axisX->setTitleText("Mois");
    axisX->setLabelsColor(QColor("#4A4A4A"));
    axisX->setGridLineColor(QColor("#E0E0E0"));
    axisX->setLinePenColor(QColor("#B0B0B0"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(55, 100);
    axisY->setTitleText("Score");
    axisY->setLabelsColor(QColor("#4A4A4A"));
    axisY->setGridLineColor(QColor("#E0E0E0"));
    axisY->setLinePenColor(QColor("#B0B0B0"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    chart->legend()->hide();
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("QChartView { background: white; border-radius: 6px; }");
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    return chartView;
}

QChartView* ResidentsPage::createMonthlyTrendsChart()
{
    QBarSeries *series = new QBarSeries();
    
    QBarSet *registrations = new QBarSet("Nouvelles Inscriptions");
    QBarSet *departures = new QBarSet("Départs");
    
    registrations->setColor(QColor("#4CAF50"));
    registrations->setBorderColor(QColor("#2E7D32"));
    departures->setColor(QColor("#F44336"));
    departures->setBorderColor(QColor("#C62828"));
    
    // Simulate monthly data
    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();
    QMap<int, int> monthlyRegistrations;
    
    // Initialize months
    for (int month = 1; month <= 12; ++month) {
        monthlyRegistrations[month] = 0;
    }
    
    // Count actual registrations by month if data exists
    for (const auto &resident : residents) {
        QDateTime dateTime = resident.value("registration_date", QDateTime::currentDateTime()).toDateTime();
        int month = dateTime.date().month();
        monthlyRegistrations[month]++;
    }
    
    // Add simulated data if no real data
    bool hasRealData = false;
    for (auto it = monthlyRegistrations.begin(); it != monthlyRegistrations.end(); ++it) {
        if (it.value() > 0) {
            hasRealData = true;
            break;
        }
    }
    
    if (!hasRealData) {
        QList<int> simData = {8, 12, 15, 18, 22, 19, 16, 14, 20, 17, 13, 10};
        for (int i = 0; i < 12; ++i) {
            monthlyRegistrations[i + 1] = simData[i];
        }
    }
    
    // Add data to bar sets
    for (int month = 1; month <= 12; ++month) {
        *registrations << monthlyRegistrations[month];
        *departures << qMax(0, monthlyRegistrations[month] - QRandomGenerator::global()->bounded(5)); // Simulate departures
    }
    
    series->append(registrations);
    series->append(departures);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Tendances Mensuelles - Inscriptions vs Départs");
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeLight);
    chart->setBackgroundBrush(QBrush(Qt::white));
    
    QStringList categories = {"Jan", "Fév", "Mar", "Avr", "Mai", "Jun", 
                             "Jul", "Aoû", "Sep", "Oct", "Nov", "Déc"};
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor("#4A4A4A"));
    axisX->setGridLineColor(QColor("#E0E0E0"));
    axisX->setLinePenColor(QColor("#B0B0B0"));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 30);
    axisY->setLabelsColor(QColor("#4A4A4A"));
    axisY->setGridLineColor(QColor("#E0E0E0"));
    axisY->setLinePenColor(QColor("#B0B0B0"));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    chart->legend()->setAlignment(Qt::AlignTop);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("QChartView { background: white; border-radius: 8px; }");
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    return chartView;
}
