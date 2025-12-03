#include "residentspage.h"
#include "ui_residentspage.h"
#include "databasemanager.h"
#include "thememanager.h"
#include "languagemanager.h"
#include <QTimer>
#include <QApplication>

#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>

// QtCharts
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QRandomGenerator>
#include <QSqlQuery>
#include <QSqlError>

#include "arduino.h"


// ======================================================================
//  CONSTRUCTEUR
// ======================================================================

ResidentsPage::ResidentsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ResidentsPage)
    , arduino(new Arduino(this))      // INITIALISATION ARDUINO
    , deniedCount(0)                  // compteur d'échecs RFID
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

    ui->tabListButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    // Load residents asynchronously
    QTimer::singleShot(100, this, [this]() {
        QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
        setResidents(rows);
    });

    applyTheme();
    reloadTranslations();

    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, [this](AppTheme){ applyTheme(); });

    // =====================================================
    //                🔵 ARDUINO CONNECT
    // =====================================================
    connect(arduino, &Arduino::connectionStatusChanged,
            this, [=](bool ok){
                if(ok)
                    qDebug() << "[ResidentsPage] Arduino connected (COM3)";
                else
                    qDebug() << "[ResidentsPage] Arduino disconnected";
            });

    // Quand un UID RFID arrive depuis Arduino
    connect(arduino, &Arduino::dataReceived,
            this, &ResidentsPage::onArduinoData);

    // Connecter Arduino au port COM3

}

ResidentsPage::~ResidentsPage()
{
    delete ui;
}


// ======================================================================
//  THEME + TRANSLATION
// ======================================================================

void ResidentsPage::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void ResidentsPage::reloadTranslations()
{
    ui->retranslateUi(this);
}



// ======================================================================
//  TABLE SETUP
// ======================================================================

void ResidentsPage::setupTableHeaders()
{
    QTableWidget *t = ui->residentsTable;
    t->setColumnCount(10);

    QStringList headers = {
        "CIN","Nom","Prénom","Sexe","Téléphone","Email",
        "Date d'entrée","Appartement","Étage","Statut"
    };

    t->setHorizontalHeaderLabels(headers);

    // IMPORTANT : évite les freezes
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
}



// ======================================================================
//  UI BEHAVIOR
// ======================================================================

void ResidentsPage::setupUiBehavior()
{
    connect(ui->tabAddButton, &QPushButton::clicked, this, &ResidentsPage::onTabAdd);
    connect(ui->tabListButton, &QPushButton::clicked, this, &ResidentsPage::onTabList);
    connect(ui->tabStatsButton,&QPushButton::clicked, this, &ResidentsPage::onTabStats);

    connect(ui->saveFormButton, &QPushButton::clicked, this, &ResidentsPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &ResidentsPage::onCancelForm);

    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ResidentsPage::onFilterTextChanged);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ResidentsPage::onSortIndexChanged);

    connect(ui->editResidentButton,   &QPushButton::clicked, this, &ResidentsPage::onEditClicked);
    connect(ui->deleteResidentButton, &QPushButton::clicked, this, &ResidentsPage::onDeleteClicked);
    connect(ui->exportPdfButton,      &QPushButton::clicked, this, &ResidentsPage::onExportPdfClicked);
}



// ======================================================================
//  VALIDATION
// ======================================================================

void ResidentsPage::setupValidation()
{
    ui->errorLabel->hide();
    ui->cinEdit->setMaxLength(8);

    // Use Qt::QueuedConnection to prevent immediate recursive calls
    connect(ui->cinEdit, &QLineEdit::textChanged, this, &ResidentsPage::onCinTextChanged, Qt::QueuedConnection);
    connect(ui->emailEdit, &QLineEdit::textChanged, this, &ResidentsPage::onEmailTextChanged, Qt::QueuedConnection);
}



// ======================================================================
//  POPULATE TABLE
// ======================================================================

void ResidentsPage::setResidents(const QList<QVariantMap> &rows)
{
    QTableWidget *t = ui->residentsTable;

    t->setSortingEnabled(false);
    t->setRowCount(0);
    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        auto set = [&](int col, const QString &key){
            t->setItem(r, col, new QTableWidgetItem(rows[r].value(key).toString()));
        };
        set(0,"cin");
        set(1,"nom");
        set(2,"prenom");
        set(3,"sexe");
        set(4,"telephone");
        set(5,"email");
        set(6,"date_entree");
        set(7,"appartement");
        set(8,"etage");
        set(9,"statut");
    }
}



// ======================================================================
//  NAVIGATION
// ======================================================================

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

    // Use timer to prevent UI freeze during data loading
    QTimer::singleShot(50, this, [this]() {
        QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
        setResidents(rows);
    });
}

void ResidentsPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);

    ui->stackedWidget->setCurrentWidget(ui->statsPage);

    // ANTI-FREEZE FIX
    QTimer::singleShot(80, this, [this]() {
        createRealStatisticsCharts();
    });
}



// ======================================================================
//  SAVE FORM — FREEZE FIX
// ======================================================================

void ResidentsPage::onSaveForm()
{
    if (!validateForm())
        return;

    // Process events to prevent UI freeze
    QApplication::processEvents();

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

    // Disable UI to prevent multiple clicks
    ui->saveFormButton->setEnabled(false);
    ui->cancelFormButton->setEnabled(false);
    
    // Show loading indicator
    ui->saveFormButton->setText("Enregistrement...");
    QApplication::processEvents();

    // Perform database operations ASYNCHRONOUSLY to prevent UI freeze
    QTimer::singleShot(50, this, [this, cin, data]() {
        bool success = false;
        QString errorMessage;
        
        try {
            // Quick check if database is responsive
            QSqlDatabase db = DatabaseManager::instance().database();
            if (!db.isOpen()) {
                success = false;
                errorMessage = "Base de données non connectée";
            } else {
                QVariantMap existing = DatabaseManager::instance().getResident(cin);
                
                if (!existing.isEmpty()) {
                    success = DatabaseManager::instance().updateResident(cin, data);
                    errorMessage = success ? "" : "Erreur lors de la modification du résident";
                } else {
                    success = DatabaseManager::instance().addResident(data);
                    errorMessage = success ? "" : "Erreur lors de l'ajout du résident";
                }
            }
        } catch (...) {
            success = false;
            errorMessage = "Erreur inattendue lors de l'opération";
        }

        // Re-enable UI (this runs on main thread)
        ui->saveFormButton->setEnabled(true);
        ui->cancelFormButton->setEnabled(true);
        ui->saveFormButton->setText("Enregistrer");

        // Show result
        if (success) {
            QMessageBox::information(this, "Succès", "Données enregistrées avec succès.");
            clearForm();
            
            // Reload data
            QTimer::singleShot(100, this, [this]() {
                QList<QVariantMap> rows = DatabaseManager::instance().getAllResidents();
                setResidents(rows);
            });
            
            ui->stackedWidget->setCurrentWidget(ui->listPage);
            ui->tabListButton->setChecked(true);
        } else {
            QMessageBox::warning(this, "Erreur", errorMessage.isEmpty() ? "Erreur lors de l'enregistrement" : errorMessage);
        }
    });
}



// ======================================================================
//  CANCEL
// ======================================================================

void ResidentsPage::onCancelForm()
{
    clearForm();
    onTabList();
}



// ======================================================================
//  CLEAR FORM
// ======================================================================

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
    hideError();
}



// ======================================================================
//  SEARCH + SORT
// ======================================================================

void ResidentsPage::onFilterTextChanged(const QString &s)
{
    QTableWidget *t = ui->residentsTable;
    for (int r = 0; r < t->rowCount(); ++r)
    {
        bool match =
            t->item(r,0)->text().contains(s, Qt::CaseInsensitive) ||
            t->item(r,1)->text().contains(s, Qt::CaseInsensitive);

        t->setRowHidden(r, !match);
    }
}

int ResidentsPage::sortColumnForIndex(int index) const
{
    switch (index) {
    case 0: return 0;
    case 1: return 1;
    case 2: return 7;
    case 3: return 6;
    }
    return 0;
}

void ResidentsPage::onSortIndexChanged(int index)
{
    ui->residentsTable->setSortingEnabled(true);
    ui->residentsTable->sortItems(sortColumnForIndex(index));
}



// ======================================================================
//  EDIT & DELETE
// ======================================================================

void ResidentsPage::onEditClicked()
{
    QTableWidget *t = ui->residentsTable;
    if (t->currentRow() < 0) {
        QMessageBox::warning(this, "Modifier", "Sélectionnez un résident.");
        return;
    }

    // Process events to ensure UI responsiveness
    QApplication::processEvents();
    
    QString cin = t->item(t->currentRow(),0)->text();
    
    // Use timer to prevent freeze during database access
    QTimer::singleShot(10, this, [this, cin]() {
        QVariantMap r = DatabaseManager::instance().getResident(cin);

        if (r.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Impossible de charger les données.");
            return;
        }

        loadResident(r);

        ui->tabAddButton->setChecked(true);
        ui->tabListButton->setChecked(false);
        ui->tabStatsButton->setChecked(false);
        ui->stackedWidget->setCurrentWidget(ui->addPage);
    });
}
void ResidentsPage::loadResident(const QVariantMap &r)
{
    ui->cinEdit->setText(r["cin"].toString());
    ui->nomEdit->setText(r["nom"].toString());
    ui->prenomEdit->setText(r["prenom"].toString());
    ui->telephoneEdit->setText(r["telephone"].toString());
    ui->emailEdit->setText(r["email"].toString());
    ui->sexeCombo->setCurrentText(r["sexe"].toString());
    ui->statutComboBox->setCurrentText(r["statut"].toString());
    ui->appartementEdit->setText(r["appartement"].toString());
    ui->etageLineEdit->setText(r["etage"].toString());

    // -----------------------------------------
    //  FIX ANTI-FREEZE DATE_ENTREE
    // -----------------------------------------

    QString d = r["date_entree"].toString();
    QDate date;

    // Format normal envoyé par Qt
    date = QDate::fromString(d, "yyyy-MM-dd");

    // Ancien format Oracle DD/MM/YY
    if (!date.isValid())
        date = QDate::fromString(d, "dd/MM/yy");

    // Format DD/MM/YYYY (certains Oracle Developer)
    if (!date.isValid())
        date = QDate::fromString(d, "dd/MM/yyyy");

    // Dernier recours → éviter freeze
    if (!date.isValid())
        date = QDate::currentDate();

    ui->dateEdit->setDate(date);
}

void ResidentsPage::onDeleteClicked()
{
    QTableWidget *t = ui->residentsTable;
    if (t->currentRow() < 0) {
        QMessageBox::warning(this, "Supprimer", "Sélectionnez un résident.");
        return;
    }

    QString cin = t->item(t->currentRow(),0)->text();

    if (QMessageBox::question(this,"Confirmation",
                              "Supprimer le résident ?") == QMessageBox::Yes)
    {
        DatabaseManager::instance().deleteResident(cin);
        onTabList();
    }
}



// ======================================================================
//  ERROR LABEL
// ======================================================================

void ResidentsPage::onCinTextChanged()
{
    hideError();
}

void ResidentsPage::onEmailTextChanged()
{
    hideError();
}

void ResidentsPage::showError(const QString &msg)
{
    ui->errorLabel->setText(msg);
    ui->errorLabel->show();
}

void ResidentsPage::hideError()
{
    ui->errorLabel->clear();
    ui->errorLabel->hide();
}

bool ResidentsPage::validateCin(const QString &cin)
{
    return cin.length() == 8 && cin.toInt() > 0;
}

bool ResidentsPage::validateEmail(const QString &email)
{
    return email.contains("@") && email.contains(".");
}

bool ResidentsPage::validateForm()
{
    if (!validateCin(ui->cinEdit->text())) { showError("CIN invalide."); return false; }
    if (ui->nomEdit->text().isEmpty()) { showError("Nom vide."); return false; }
    if (ui->prenomEdit->text().isEmpty()) { showError("Prénom vide."); return false; }
    if (!validateEmail(ui->emailEdit->text())) { showError("Email invalide."); return false; }
    return true;
}



// ======================================================================
//  STATISTICS — FINAL CHART
// ======================================================================

void ResidentsPage::createRealStatisticsCharts()
{
    QWidget *container = ui->chartsContainer;
    if (!container) return;

    // Clear old
    if (container->layout())
    {
        QLayoutItem *item;
        while ((item = container->layout()->takeAt(0)) != nullptr)
        {
            if (item->widget()) item->widget()->deleteLater();
            delete item;
        }
        delete container->layout();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(10,10,10,10);
    mainLayout->setSpacing(10);

    QChartView *chartView = createSatisfactionCurveChart();
    chartView->setMinimumHeight(270);
    chartView->setMaximumHeight(300);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    mainLayout->addWidget(chartView);
}



// ======================================================================
//  SATISFACTION CURVE
// ======================================================================

QChartView* ResidentsPage::createSatisfactionCurveChart()
{
    QList<int> monthly = DatabaseManager::instance().getMonthlySatisfactionScores();

    QLineSeries *s = new QLineSeries();
    s->setColor(QColor("#1F2D5C"));
    s->setPointsVisible(true);
    s->setPen(QPen(QColor("#1F2D5C"), 3));

    for (int i = 0; i < monthly.size(); i++)
        s->append(i, monthly[i]);

    QChart *chart = new QChart();
    chart->addSeries(s);
    chart->legend()->hide();
    chart->setTitle("Score de Satisfaction — 12 derniers mois");
    chart->setBackgroundBrush(Qt::white);

    QStringList months =
        {"Jan","Fév","Mar","Avr","Mai","Jun","Jul","Aoû","Sep","Oct","Nov","Déc"};

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(40, 100);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    s->attachAxis(axisX);
    s->attachAxis(axisY);

    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    return view;
}



// ======================================================================
//  PDF EXPORT — YOUR FULL VERSION (UNCHANGED)
// ======================================================================

void ResidentsPage::onExportPdfClicked()
{
    QTableWidget *t = ui->residentsTable;
    int row = t->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner un résident.");
        return;
    }

    // Disable button to prevent multiple clicks
    ui->exportPdfButton->setEnabled(false);
    ui->exportPdfButton->setText("Export...");
    QApplication::processEvents();

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

    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Enregistrer l'attestation",
        QString("Attestation_Residence_%1_%2.pdf").arg(nom).arg(prenom),
        "PDF (*.pdf)"
        );

    if (filePath.isEmpty()) {
        // Re-enable button if user cancels
        ui->exportPdfButton->setEnabled(true);
        ui->exportPdfButton->setText("Exporter PDF");
        return;
    }

    // Process events before heavy PDF generation
    QApplication::processEvents();

    try {
        // --- PDF GENERATION (UNCHANGED) ---

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

    QPixmap logoDark(":/resources/nexora.png");
    QPixmap logoLight(":/resources/nexorab.png");
    QPixmap logo = logoDark.isNull() ? logoLight : logoDark;

    if (!logo.isNull()) {
        int logoH = headerH - mm(6);
        int logoW = logo.width() * logoH / qMax(1, logo.height());
        QRect logoRect(mm(8), y + (headerH - logoH) / 2, logoW, logoH);
        p.drawPixmap(logoRect, logo);
    } else {
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 14, QFont::DemiBold));
        p.drawText(mm(8), y, W/2 - mm(10), headerH,
                   Qt::AlignVCenter | Qt::AlignLeft,
                   "NEXORA SMART CITY");
    }

    p.setFont(QFont("Arial", 12, QFont::Bold));
    p.drawText(W/2, y, W/2 - mm(8), headerH,
               Qt::AlignVCenter | Qt::AlignRight,
               "Attestation de Résidence");

    y += headerH + mm(10);

    QString ref = QString("Ref: NXR-RES-%1-%2")
                      .arg(QDate::currentDate().toString("yyyyMMdd"))
                      .arg(cin);

    QString dateStr = QDate::currentDate().toString("dd/MM/yyyy");

    p.setPen(text);
    p.setFont(QFont("Arial", 9));
    p.drawText(mm(8), y, W - mm(16), mm(6),
               Qt::AlignLeft | Qt::AlignVCenter, ref);

    p.drawText(mm(8), y, W - mm(16), mm(6),
               Qt::AlignRight | Qt::AlignVCenter,
               QString("Date: %1").arg(dateStr));

    y += mm(12);

    p.setPen(primary);
    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(mm(8), y, W - mm(16), mm(14),
               Qt::AlignCenter, "ATTESTATION DE RÉSIDENCE");

    y += mm(16);

    p.setPen(text);
    p.setFont(QFont("Arial", 10));
    QRect introRect(mm(8), y, W - mm(16), mm(12));
    p.drawText(introRect, Qt::AlignLeft | Qt::TextWordWrap,
               "Nous certifions que la personne ci-dessous réside "
               "officiellement au sein de la résidence NEXORA.");

    y += introRect.height() + mm(4);

    int cardTop = y;
    int cardH = mm(70);
    QRect cardRect(mm(8), cardTop, W - mm(16), cardH);

    p.setPen(QPen(border, 1));
    p.setBrush(Qt::white);
    p.drawRoundedRect(cardRect, 8, 8);

    p.setPen(accent);
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(cardRect.adjusted(mm(8), mm(6), -mm(8), 0),
               Qt::AlignLeft | Qt::AlignTop,
               "Identité du Résident");

    p.setPen(QPen(border, 1));
    p.drawLine(cardRect.left() + mm(8), cardRect.top() + mm(14),
               cardRect.right() - mm(8), cardRect.top() + mm(14));

    int rowH = mm(9);
    int leftX  = cardRect.left() + mm(12);
    int rightX = cardRect.left() + (cardRect.width()/2) + mm(4);
    int textY  = cardRect.top() + mm(20);

    auto drawField = [&](int x, const QString &label, const QString &value){
        p.setPen(primary);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(
            x, textY,
            cardRect.width()/2 - mm(16),
            rowH,
            Qt::AlignLeft | Qt::AlignVCenter,
            label
            );
        p.setPen(text);
        p.setFont(QFont("Arial", 10));
        p.drawText(
            x, textY + mm(4),
            cardRect.width()/2 - mm(16),
            rowH,
            Qt::AlignLeft | Qt::AlignVCenter,
            value
            );
    };

    drawField(leftX,  "Nom", nom);
    drawField(rightX, "Prénom", prenom); textY += rowH;
    drawField(leftX,  "CIN", cin);
    drawField(rightX, "Sexe", sexe); textY += rowH;
    drawField(leftX,  "Téléphone", telephone);
    drawField(rightX, "Email", email); textY += rowH;
    drawField(leftX,  "Appartement", appart);
    drawField(rightX, "Étage", etage); textY += rowH;
    drawField(leftX,  "Statut", statut);
    drawField(rightX, "Date d'entrée", dateEntree);

    y = cardRect.bottom() + mm(10);

    p.setPen(text);
    p.setFont(QFont("Arial", 10));
    QString paragraph =
        QString("Attestons que %1 %2, titulaire du CIN %3, réside à "
                "l'appartement %4 (étage %5) au sein de la résidence NEXORA, "
                "et ce depuis le %6.\n\n"
                "La présente attestation est délivrée pour servir et valoir "
                "ce que de droit.")
            .arg(prenom).arg(nom).arg(cin).arg(appart).arg(etage).arg(dateEntree);

    QRect paraRect(mm(8), y, W - mm(16), mm(30));
    p.drawText(paraRect, Qt::AlignLeft | Qt::TextWordWrap, paragraph);
    y = paraRect.bottom() + mm(14);

    int boxW = mm(65);
    QRect sigBox(W - mm(8) - boxW, y, boxW, mm(28));

    p.setPen(QPen(border, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(sigBox, 6, 6);

    p.setPen(primary);
    p.setFont(QFont("Arial", 10, QFont::Bold));
    p.drawText(
        sigBox.adjusted(mm(6), mm(6), -mm(6), -mm(6)),
        Qt::AlignTop | Qt::AlignHCenter,
        "Cachet et signature"
        );

    p.setPen(QPen(text, 1));
    p.drawLine(
        sigBox.left() + mm(6),
        sigBox.bottom() - mm(10),
        sigBox.right() - mm(6),
        sigBox.bottom() - mm(10)
        );

    int footerY = H - mm(18);
    p.setPen(QPen(border, 1));
    p.drawLine(mm(8), footerY, W - mm(8), footerY);

    p.setFont(QFont("Arial", 8));
    p.setPen(text);
    p.drawText(
        mm(8), footerY + mm(2),
        W - mm(16), mm(10),
        Qt::AlignCenter,
        "NEXORA Smart City — Service de Gestion Résidentielle · "
        "Document généré automatiquement"
        );

    p.end();

    // Re-enable button after successful PDF generation
    ui->exportPdfButton->setEnabled(true);
    ui->exportPdfButton->setText("Exporter PDF");

    QMessageBox::information(
        this,
        "PDF généré",
        QString("Attestation de résidence créée avec succès.\nFichier: %1")
            .arg(filePath)
        );
    
    } catch (...) {
        // Re-enable button if PDF generation fails
        ui->exportPdfButton->setEnabled(true);
        ui->exportPdfButton->setText("Exporter PDF");
        
        QMessageBox::critical(this, "Erreur", "Erreur lors de la génération du PDF.");
    }
}
void ResidentsPage::onArduinoData(QString data)
{
    data = data.trimmed();
    if (data.isEmpty())
        return;

    QString uid = data.toUpper();
    qDebug() << "[RFID] UID reçu :" << uid;

    // ==============================
    // Vérifier dans la base Oracle
    // ==============================
    QSqlQuery q(DatabaseManager::instance().database());
    q.prepare("SELECT COUNT(*) FROM RESIDENTS WHERE RFID_UID = :u");
    q.bindValue(":u", uid);

    if(!q.exec()) {
        qDebug() << "[RFID] Erreur SQL:" << q.lastError().text();
        return;
    }

    q.next();
    bool exists = q.value(0).toInt() > 0;

    // ==============================
    // Résultat
    // ==============================
    if (exists) {
        deniedCount = 0;
        qDebug() << "[RFID] ACCEPTED ✔ Résident trouvé avec UID =" << uid;
    }
    else {
        deniedCount++;
        qDebug() << "[RFID] DENIED ✖ UID inconnu =" << uid
                 << " tentative =" << deniedCount;

        // 3 échecs → incident
        if (deniedCount >= 3)
        {
            deniedCount = 0;
            qDebug() << "[RFID] ⚠ INCIDENT : 3 échecs successifs pour UID =" << uid;

            // (pas ton rôle — juste log)
            // IncidentManager::instance().addIncident(uid);
        }
    }
}
