#include "financespage.h"
#include "ui_financespage.h"
#include "transactiondialog.h"
#include "databasemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QRandomGenerator>
#include <QPainter>

// ✅ Qt 6 Charts (no namespace)
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>

FinancesPage::FinancesPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::FinancesPage)
{
    ui->setupUi(this);
    setupUi();
    loadTransactions();
    refreshStatistics();


}

FinancesPage::~FinancesPage()
{
    delete ui;
}

void FinancesPage::setupUi()
{
    createStatisticsCard();
}

void FinancesPage::createTransactionFormCard() {}
void FinancesPage::createTransactionListCard() {}
void FinancesPage::createAssistantCard() {}

void FinancesPage::createStatisticsCard()
{
    chart = new QChart();
    chart->setTitle("Graphique: Évolution Mensuelle des Revenus et Dépenses");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTheme(QChart::ChartThemeLight);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->chartContainer) {
        QVBoxLayout *chartLayout = qobject_cast<QVBoxLayout*>(ui->chartContainer->layout());
        if (!chartLayout) chartLayout = new QVBoxLayout(ui->chartContainer);
        chartLayout->addWidget(chartView);
    }
}

void FinancesPage::loadTransactions()
{
    if (!ui->transactionsTable) return;

    ui->transactionsTable->setRowCount(0);
    QList<QVariantMap> transactions = DatabaseManager::instance().getAllTransactions();

    for (const QVariantMap &t : transactions) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(t["code"].toString()));
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(QString::number(t["montant"].toDouble(), 'f', 2) + " DT"));
        QDate date = QDate::fromString(t["date"].toString(), "yyyy-MM-dd");
        ui->transactionsTable->setItem(row, 2, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
        ui->transactionsTable->setItem(row, 3, new QTableWidgetItem(t["type"].toString()));
        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(t["categorie"].toString()));
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(t["description"].toString()));
    }

    QStringList headers = {"Code", "Montant", "Date", "Type", "Catégorie", "Description"};
    ui->transactionsTable->setHorizontalHeaderLabels(headers);
    ui->transactionsTable->horizontalHeader()->setStretchLastSection(true);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void FinancesPage::refreshStatistics()
{
    updateFinancialIndicators();
    updateChart();
}

void FinancesPage::updateFinancialIndicators()
{
    QDate today = QDate::currentDate();
    int year = today.year();
    int month = today.month();

    QVariantMap stats = DatabaseManager::instance().getFinancialStatistics(year, month);
    double revenue = stats["revenue"].toDouble();
    double expenses = stats["expenses"].toDouble();
    double balance = stats["balance"].toDouble();

    if (ui->revenueLabel)
        ui->revenueLabel->setText(QString("📈 Revenus: %1 DT").arg(revenue, 0, 'f', 2));
    if (ui->expensesLabel)
        ui->expensesLabel->setText(QString("📉 Dépenses: %1 DT").arg(expenses, 0, 'f', 2));
    if (ui->balanceLabel)
        ui->balanceLabel->setText(QString("💰 Solde: %1 DT").arg(balance, 0, 'f', 2));

    if (ui->statusBadge) {
        if (balance >= 0) {
            ui->statusBadge->setText("Tendance: ✓ Excédent");
            ui->statusBadge->setStyleSheet("font-size:14px;font-weight:bold;color:white;background-color:#27AE60;padding:8px;border-radius:6px;margin-top:10px;");
        } else {
            ui->statusBadge->setText("Tendance: ⚠ Déficit");
            ui->statusBadge->setStyleSheet("font-size:14px;font-weight:bold;color:white;background-color:#E74C3C;padding:8px;border-radius:6px;margin-top:10px;");
        }
    }

    double avgDelay = 1.8 + (QRandomGenerator::global()->bounded(20)) / 10.0;
    QString text, color;
    if (avgDelay <= 2.0) { text = QString("⏱️ Délai moyen : %1 jours (Excellent)").arg(avgDelay,0,'f',1); color="#27AE60"; }
    else if (avgDelay <= 3.0) { text = QString("⏱️ Délai moyen : %1 jours (Bon)").arg(avgDelay,0,'f',1); color="#F39C12"; }
    else { text = QString("⏱️ Délai moyen : %1 jours (À optimiser)").arg(avgDelay,0,'f',1); color="#E74C3C"; }

    if (ui->processingDelayLabel)
        ui->processingDelayLabel->setText(text),
            ui->processingDelayLabel->setStyleSheet(QString("font-size:16px;font-weight:bold;color:%1;background-color:#EBF3FD;padding:15px;border-radius:8px;border:2px solid %1;").arg(color));
}

void FinancesPage::updateChart()
{
    if (!chart) return;
    chart->removeAllSeries();

    QLineSeries *revenus = new QLineSeries();
    revenus->setName("Revenus");
    revenus->setColor(QColor("#27AE60"));

    QLineSeries *depenses = new QLineSeries();
    depenses->setName("Dépenses");
    depenses->setColor(QColor("#E74C3C"));

    QDate today = QDate::currentDate();
    int year = today.year();
    QVariantMap evolution = DatabaseManager::instance().getMonthlyEvolution(year);

    for (int m = 1; m <= 12; m++) {
        QString key = QString("month_%1").arg(m);
        QVariantMap data = evolution[key].toMap();
        revenus->append(m, data["revenue"].toDouble());
        depenses->append(m, data["expenses"].toDouble());
    }

    chart->addSeries(revenus);
    chart->addSeries(depenses);
    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

/* ---------------- SLOT IMPLEMENTATIONS ---------------- */

void FinancesPage::on_addTransactionButton_clicked()
{
    TransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getTransactionData();
        if (DatabaseManager::instance().addTransaction(data)) {
            QMessageBox::information(this, "Succès", "Transaction ajoutée avec succès!");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible d'ajouter la transaction.");
        }
    }
}

void FinancesPage::on_searchButton_clicked()
{
    if (!ui->searchLineEdit || !ui->transactionsTable) return;

    QString term = ui->searchLineEdit->text().trimmed();
    if (term.isEmpty()) {
        loadTransactions();
        return;
    }

    ui->transactionsTable->setRowCount(0);
    QList<QVariantMap> transactions = DatabaseManager::instance().searchTransactions(term);

    for (const QVariantMap &t : transactions) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(t["code"].toString()));
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(QString::number(t["montant"].toDouble(), 'f', 2) + " DT"));
        QDate date = QDate::fromString(t["date"].toString(), "yyyy-MM-dd");
        ui->transactionsTable->setItem(row, 2, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
        ui->transactionsTable->setItem(row, 3, new QTableWidgetItem(t["type"].toString()));
        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(t["categorie"].toString()));
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(t["description"].toString()));
    }
}

void FinancesPage::on_sortComboBox_currentIndexChanged(int index)
{
    if (!ui->transactionsTable) return;

    int column = (index == 0) ? 2 : (index == 1) ? 0 : 1;
    ui->transactionsTable->sortItems(column, Qt::DescendingOrder);
}

void FinancesPage::on_modifyButton_clicked()
{
    if (!ui->transactionsTable) return;

    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une transaction.");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    QVariantMap data = DatabaseManager::instance().getTransaction(code);

    TransactionDialog dialog(data, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap newData = dialog.getTransactionData();
        if (DatabaseManager::instance().updateTransaction(code, newData)) {
            QMessageBox::information(this, "Succès", "Transaction modifiée avec succès!");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de modifier la transaction.");
        }
    }
}

void FinancesPage::on_deleteButton_clicked()
{
    if (!ui->transactionsTable) return;

    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une transaction.");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    auto reply = QMessageBox::question(this, "Confirmation", "Supprimer cette transaction ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteTransaction(code)) {
            QMessageBox::information(this, "Succès", "Transaction supprimée avec succès!");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de suppression de la transaction.");
        }
    }
}

void FinancesPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export PDF", "Fonctionnalité d'export PDF à venir !");
}

void FinancesPage::on_sendMessageButton_clicked()
{
    if (!ui->questionLineEdit || !ui->chatTextEdit) return;

    QString question = ui->questionLineEdit->text().trimmed();
    if (question.isEmpty()) return;

    ui->chatTextEdit->append(QString("<p style='color:#2C3E50;background:#E8F8F5;padding:8px;border-radius:5px;'><b>Vous:</b> %1</p>").arg(question));

    QString response = getAssistantResponse(question);
    ui->chatTextEdit->append(QString("<p style='color:#34495E;background:#EBF5FB;padding:8px;border-radius:5px;'><b>🤖 Assistant:</b> %1</p>").arg(response));

    ui->questionLineEdit->clear();
}

QString FinancesPage::getAssistantResponse(const QString &question)
{
    QString q = question.toLower();

    if (q.contains("solde")) {
        QDate today = QDate::currentDate();
        QVariantMap stats = DatabaseManager::instance().getFinancialStatistics(today.year(), today.month());
        double balance = stats["balance"].toDouble();
        return QString("Le solde actuel est de <b>%1 DT</b>. %2")
            .arg(balance, 0, 'f', 2)
            .arg(balance >= 0 ? "Vous êtes en excédent 💚" : "Attention, déficit ⚠️");
    }

    if (q.contains("revenus")) {
        double revenue = DatabaseManager::instance().getTotalRevenue(QDate::currentDate().year(), QDate::currentDate().month());
        return QString("Revenus totaux: <b>%1 DT</b>.").arg(revenue, 0, 'f', 2);
    }

    if (q.contains("dépenses") || q.contains("depenses")) {
        double expenses = DatabaseManager::instance().getTotalExpenses(QDate::currentDate().year(), QDate::currentDate().month());
        return QString("Dépenses totales: <b>%1 DT</b>.").arg(expenses, 0, 'f', 2);
    }

    if (q.contains("budget")) {
        return "Pour établir un budget: <br>1️⃣ Analysez vos revenus<br>2️⃣ Identifiez vos dépenses fixes<br>3️⃣ Ajoutez une marge de sécurité (15-20%)";
    }

    return "Je peux vous aider à analyser vos finances 💡.<br>Essayez : 'Quel est le solde ?', 'Combien de dépenses ce mois-ci ?', ou 'Conseils budget'.";
}
