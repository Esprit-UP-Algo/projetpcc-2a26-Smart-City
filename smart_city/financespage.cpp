#include "financespage.h"
#include "ui_financespage.h"
#include "databasemanager.h"

#include <QMessageBox>
#include <QDebug>
#include <QDate>
#include <QDoubleValidator>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlDatabase>
#include <QtCharts/qchartglobal.h>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QLocale>
#include <QtGlobal>
#include <cmath>

FinancesPage::FinancesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinancesPage)
{
    ui->setupUi(this);

    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    reloadTransactions();
    calculateAndDisplayScore();
}

FinancesPage::~FinancesPage()
{
    delete ui;
}

// ----------------------------------------------------------
// CONFIGURATION TABLE
// ----------------------------------------------------------
void FinancesPage::setupTableHeaders()
{
    QTableWidget *t = ui->financeTable;
    t->setColumnCount(7);
    QStringList headers = {
        tr("Code"),
        tr("Montant (DT)"),
        tr("Type"),
        tr("Catégorie"),
        tr("Description"),
        tr("Date"),
        tr("CIN résident")
    };
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    t->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    t->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    t->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    t->horizontalHeader()->setStretchLastSection(true);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setSortingEnabled(true);
}

// ----------------------------------------------------------
// COMPORTEMENT DES BOUTONS
// ----------------------------------------------------------
void FinancesPage::setupUiBehavior()
{
    connect(ui->editfinanceButton,  &QPushButton::clicked, this, &FinancesPage::onEditClicked);
    connect(ui->deletfinanceButton, &QPushButton::clicked, this, &FinancesPage::onDeleteClicked);

    connect(ui->tabAddButton,   &QPushButton::clicked, this, &FinancesPage::onTabAdd);
    connect(ui->tabListButton,  &QPushButton::clicked, this, &FinancesPage::onTabList);
    connect(ui->tabStatsButton, &QPushButton::clicked, this, &FinancesPage::onTabStats);

    connect(ui->saveFormButton,   &QPushButton::clicked, this, &FinancesPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &FinancesPage::onCancelForm);

    connect(ui->searchEdit, &QLineEdit::textChanged, this, &FinancesPage::onFilterTextChanged);
    connect(ui->searchEdit, &QLineEdit::returnPressed, this, [this]() {
        onFilterTextChanged(ui->searchEdit->text());
    });
    connect(ui->searchButton, &QPushButton::clicked, this, [this]() {
        onFilterTextChanged(ui->searchEdit->text());
    });
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FinancesPage::onSortIndexChanged);

    auto hideErrorOnChange = [this]() { hideError(); };
    connect(ui->codeEdit, &QLineEdit::textChanged, this, hideErrorOnChange);
    connect(ui->typeEdit, &QLineEdit::textChanged, this, hideErrorOnChange);
    connect(ui->descriptionEdit, &QLineEdit::textChanged, this, hideErrorOnChange);
    connect(ui->montantEdit, &QLineEdit::textChanged, this, hideErrorOnChange);
    connect(ui->residentCinEdit, &QLineEdit::textChanged, this, hideErrorOnChange);
}

// ----------------------------------------------------------
// VALIDATION
// ----------------------------------------------------------
void FinancesPage::setupValidation()
{
    auto *amountValidator = new QDoubleValidator(0.0, 1e12, 2, ui->montantEdit);
    amountValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->montantEdit->setValidator(amountValidator);

    QRegularExpression codeRegex(R"(^[A-Za-z0-9_-]{3,20}$)");
    auto *codeValidator = new QRegularExpressionValidator(codeRegex, ui->codeEdit);
    ui->codeEdit->setValidator(codeValidator);

    QRegularExpression cinRegex(R"(^[0-9]{0,8}$)");
    auto *cinValidator = new QRegularExpressionValidator(cinRegex, ui->residentCinEdit);
    ui->residentCinEdit->setValidator(cinValidator);
    ui->residentCinEdit->setPlaceholderText(tr("CIN résident (optionnel)"));

    ui->dateEdit->setDate(QDate::currentDate());
    hideError();
}

// ----------------------------------------------------------
// RECHARGER LES DONNÉES
// ----------------------------------------------------------
void FinancesPage::reloadTransactions()
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        qWarning() << "[FinancesPage] Base de données non ouverte";
        ui->financeTable->setRowCount(0);
        return;
    }
    setTransactions(DatabaseManager::instance().getAllTransactions());
}

// ----------------------------------------------------------
// AFFICHAGE DES TRANSACTIONS
// ----------------------------------------------------------
void FinancesPage::setTransactions(const QList<QVariantMap> &list)
{
    QTableWidget *t = ui->financeTable;
    bool wasSorting = t->isSortingEnabled();
    t->setSortingEnabled(false);
    t->clearContents();
    t->setRowCount(list.size());

    QLocale locale;
    for (int r = 0; r < list.size(); ++r) {
        const QVariantMap &row = list[r];
        auto *codeItem = new QTableWidgetItem(row.value("code_unique").toString());
        auto *amountItem = new QTableWidgetItem(locale.toString(row.value("montant").toDouble(), 'f', 2));
        auto *typeItem = new QTableWidgetItem(row.value("type").toString());
        auto *catItem = new QTableWidgetItem(row.value("categorie").toString());
        auto *descItem = new QTableWidgetItem(row.value("description").toString());
        auto *dateItem = new QTableWidgetItem(row.value("date_transaction").toString());
        auto *cinItem = new QTableWidgetItem(row.value("cin_resident").toString());

        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        dateItem->setTextAlignment(Qt::AlignCenter);
        cinItem->setTextAlignment(Qt::AlignCenter);

        t->setItem(r, 0, codeItem);
        t->setItem(r, 1, amountItem);
        t->setItem(r, 2, typeItem);
        t->setItem(r, 3, catItem);
        t->setItem(r, 4, descItem);
        t->setItem(r, 5, dateItem);
        t->setItem(r, 6, cinItem);
    }

    t->setSortingEnabled(wasSorting);
    t->resizeColumnsToContents();
}

// ----------------------------------------------------------
// CHARGER UNE TRANSACTION
// ----------------------------------------------------------
void FinancesPage::loadTransaction(const QVariantMap &transaction)
{
    currentTransactionCode = transaction.value("code_unique").toString();
    ui->codeEdit->setText(currentTransactionCode);
    ui->typeEdit->setText(transaction.value("type").toString());
    ui->categorieCombo->setCurrentText(transaction.value("categorie").toString());
    ui->descriptionEdit->setText(transaction.value("description").toString());
    ui->montantEdit->setText(QString::number(transaction.value("montant").toDouble(), 'f', 2));
    ui->residentCinEdit->setText(transaction.value("cin_resident").toString());

    QDate date = QDate::fromString(transaction.value("date_transaction").toString(), "yyyy-MM-dd");
    if (!date.isValid()) {
        date = QDate::currentDate();
    }
    ui->dateEdit->setDate(date);
}

// ----------------------------------------------------------
// MODIFIER TRANSACTION
// ----------------------------------------------------------
void FinancesPage::onEditClicked()
{
    QTableWidget *t = ui->financeTable;
    int row = t->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, tr("Aucune sélection"), tr("Veuillez sélectionner une transaction à modifier."));
        return;
    }

    QString code = t->item(row, 0)->text();
    QVariantMap data = DatabaseManager::instance().getTransaction(code);
    if (data.isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de charger la transaction sélectionnée."));
        return;
    }

    loadTransaction(data);
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

// ----------------------------------------------------------
// SUPPRIMER TRANSACTION
// ----------------------------------------------------------
void FinancesPage::onDeleteClicked()
{
    QTableWidget *t = ui->financeTable;
    int row = t->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, tr("Aucune sélection"), tr("Veuillez sélectionner une transaction à supprimer."));
        return;
    }

    QString code = t->item(row, 0)->text();

    if (QMessageBox::question(this, tr("Suppression"),
                              tr("Supprimer la transaction %1 ?").arg(code)) != QMessageBox::Yes) {
        return;
    }

    if (DatabaseManager::instance().deleteTransaction(code)) {
        QMessageBox::information(this, tr("Succès"), tr("Transaction supprimée avec succès."));
        if (currentTransactionCode == code) {
            clearForm();
        }
        reloadTransactions();
        calculateAndDisplayScore();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la suppression dans la base de données."));
    }
}

// ----------------------------------------------------------
// STATISTIQUES + GRAPHIQUE
// ----------------------------------------------------------
void FinancesPage::calculateAndDisplayScore()
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        ui->satisfactionScore->setText(tr("Score global de satisfaction : - / 100"));
        ui->satisfactionAnalysis->setText(tr("Connexion à la base de données requise pour les statistiques."));
        return;
    }

    QDate today = QDate::currentDate();
    QVariantMap stats = DatabaseManager::instance().getFinancialStatistics(today.year(), today.month());
    double revenues = stats.value("revenu").toDouble();
    double expenses = stats.value("depense").toDouble();

    QPair<int, QString> result = calculateFinancialScore(revenues, expenses);
    ui->satisfactionScore->setText(tr("Score global de satisfaction : %1 / 100").arg(result.first));
    ui->satisfactionAnalysis->setText(result.second);

    auto *series = new QPieSeries();
    series->append(tr("Revenus"), qMax(0.0, revenues));
    series->append(tr("Dépenses"), qMax(0.0, expenses));

    auto *chartObj = new QChart();
    chartObj->addSeries(series);
    chartObj->setTitle(tr("Répartition Financière"));
    chartObj->legend()->setVisible(true);
    chartObj->legend()->setAlignment(Qt::AlignRight);

    ui->chart->setChart(chartObj);
    ui->chart->setRenderHint(QPainter::Antialiasing);
}

// ----------------------------------------------------------
// NAVIGATION ONGLET
// ----------------------------------------------------------
void FinancesPage::onTabAdd()
{
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

void FinancesPage::onTabList()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);
    reloadTransactions();
}

void FinancesPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    calculateAndDisplayScore();
}

// ----------------------------------------------------------
// ENREGISTREMENT FORMULAIRE
// ----------------------------------------------------------
void FinancesPage::onSaveForm()
{
    hideError();

    if (!validateForm()) {
        return;
    }

    QString code = ui->codeEdit->text().trimmed().toUpper();
    ui->codeEdit->setText(code);

    QVariantMap data;
    data["code_unique"] = code;
    data["type"] = ui->typeEdit->text().trimmed();
    data["categorie"] = ui->categorieCombo->currentText().trimmed();
    data["description"] = ui->descriptionEdit->text().trimmed();
    data["montant"] = ui->montantEdit->text().toDouble();
    data["date_transaction"] = ui->dateEdit->date().toString("yyyy-MM-dd");
    data["cin_resident"] = ui->residentCinEdit->text().trimmed();

    bool success = false;
    if (!currentTransactionCode.isEmpty()) {
        if (code != currentTransactionCode) {
            showError(tr("Le code unique d'une transaction existante ne peut pas être modifié."));
            return;
        }
        success = DatabaseManager::instance().updateTransaction(currentTransactionCode, data);
    } else {
        if (!DatabaseManager::instance().getTransaction(code).isEmpty()) {
            showError(tr("Une transaction avec le code %1 existe déjà.").arg(code));
            return;
        }
        success = DatabaseManager::instance().addTransaction(data);
    }

    if (!success) {
        showError(tr("Impossible d'enregistrer la transaction. Veuillez vérifier les données saisies."));
        return;
    }

    QMessageBox::information(this,
                             currentTransactionCode.isEmpty() ? tr("Ajout") : tr("Modification"),
                             currentTransactionCode.isEmpty()
                                 ? tr("Transaction ajoutée avec succès !")
                                 : tr("Transaction mise à jour avec succès !"));

    clearForm();
    reloadTransactions();
    calculateAndDisplayScore();
    onTabList();
}

void FinancesPage::onCancelForm()
{
    clearForm();
    onTabList();
}

// ----------------------------------------------------------
// RECHERCHE / TRI
// ----------------------------------------------------------
void FinancesPage::onFilterTextChanged(const QString &text)
{
    QString term = text.trimmed();
    if (term.isEmpty()) {
        reloadTransactions();
        return;
    }

    setTransactions(DatabaseManager::instance().searchTransactions(term));
}

void FinancesPage::onSortIndexChanged(int index)
{
    int column = sortColumnForIndex(index);
    if (column < 0) {
        return;
    }
    ui->financeTable->sortItems(column, Qt::AscendingOrder);
}

int FinancesPage::sortColumnForIndex(int index) const
{
    switch (index) {
    case 0: return 5; // Date
    case 1: return 0; // Code
    case 2: return 1; // Montant
    case 3: return 6; // CIN résident
    default: return -1;
    }
}

// ----------------------------------------------------------
// UTILITAIRES FORMULAIRE
// ----------------------------------------------------------
void FinancesPage::clearForm()
{
    currentTransactionCode.clear();
    ui->codeEdit->clear();
    ui->typeEdit->clear();
    ui->descriptionEdit->clear();
    ui->montantEdit->clear();
    ui->categorieCombo->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->residentCinEdit->clear();
    hideError();
}

bool FinancesPage::validateForm()
{
    if (ui->codeEdit->text().trimmed().isEmpty()) {
        showError(tr("Veuillez saisir un code unique."));
        return false;
    }
    if (ui->typeEdit->text().trimmed().isEmpty()) {
        showError(tr("Veuillez préciser le type de transaction."));
        return false;
    }
    if (!ui->montantEdit->hasAcceptableInput()) {
        showError(tr("Montant invalide. Saisissez une valeur numérique positive."));
        return false;
    }
    if (ui->montantEdit->text().toDouble() <= 0.0) {
        showError(tr("Le montant doit être supérieur à zéro."));
        return false;
    }
    if (!ui->dateEdit->date().isValid() || ui->dateEdit->date() > QDate::currentDate()) {
        showError(tr("Date de transaction invalide."));
        return false;
    }
    QString cin = ui->residentCinEdit->text().trimmed();
    if (!cin.isEmpty() && cin.length() != 8) {
        showError(tr("Le CIN résident doit contenir exactement 8 chiffres."));
        return false;
    }
    return true;
}

void FinancesPage::showError(const QString &message)
{
    ui->errorLabel->setText(message);
    ui->errorLabel->show();
}

void FinancesPage::hideError()
{
    ui->errorLabel->hide();
    ui->errorLabel->clear();
}

// ----------------------------------------------------------
// CALCUL DU SCORE FINANCIER
// ----------------------------------------------------------
QPair<int, QString> FinancesPage::calculateFinancialScore(double revenues, double expenses)
{
    double total = revenues + expenses;
    double balance = revenues - expenses;
    int score = 50;

    if (total > 0.0) {
        double ratio = balance / total;
        score = qBound(0, static_cast<int>(std::round(50.0 + ratio * 50.0)), 100);
    } else if (revenues > 0.0) {
        score = 70;
    } else if (expenses > 0.0) {
        score = 30;
    }

    QString analysis;
    if (score >= 85) {
        analysis = tr("Situation financière excellente. Les revenus dépassent largement les dépenses.");
    } else if (score >= 70) {
        analysis = tr("Bonne santé financière. Les dépenses restent maîtrisées.");
    } else if (score >= 55) {
        analysis = tr("Équilibre fragile. Surveillez les dépenses et diversifiez les revenus.");
    } else if (score >= 40) {
        analysis = tr("Tension financière détectée. Réduisez les dépenses et relancez les encaissements.");
    } else {
        analysis = tr("Situation critique. Un plan d'action immédiat est recommandé.");
    }

    return qMakePair(score, analysis);
}
