#include "financespage.h"
#include "ui_financespage.h"
#include "transactiondialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QDate>
#include <QRandomGenerator>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QCloseEvent>   // ✅ nécessaire pour closeEvent

FinancesPage::FinancesPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::FinancesPage)
{
    ui->setupUi(this);

    createStatisticsCard();
    loadTransactions();
    refreshStatistics();

    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FinancesPage::on_sortComboChanged);
}

FinancesPage::~FinancesPage()
{
    delete ui;
}

void FinancesPage::createStatisticsCard()
{
    chart = new QChart();
    chart->setTitle("Évolution mensuelle des revenus et dépenses");
    chart->setTheme(QChart::ChartThemeLight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->chartContainer) {
        QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
        layout->addWidget(chartView);
    }
}

void FinancesPage::loadTransactions()
{
    ui->transactionsTable->setRowCount(0);
    QList<QVariantMap> transactions = DatabaseManager::instance().getAllTransactions();

    for (const QVariantMap &transaction : transactions) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(transaction["code"].toString()));
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(transaction["type"].toString()));
        ui->transactionsTable->setItem(row, 2, new QTableWidgetItem(transaction["categorie"].toString()));
        ui->transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(transaction["montant"].toDouble(), 'f', 2)));
        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(transaction["description"].toString()));
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(transaction["date"].toString()));
    }

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
        ui->revenueLabel->setText(QString("📈 Revenus : %1 DT").arg(revenue, 0, 'f', 2));

    if (ui->expensesLabel)
        ui->expensesLabel->setText(QString("📉 Dépenses : %1 DT").arg(expenses, 0, 'f', 2));

    if (ui->balanceLabel)
        ui->balanceLabel->setText(QString("💰 Solde : %1 DT").arg(balance, 0, 'f', 2));
}

void FinancesPage::updateChart()
{
    if (!chart) return;
    chart->removeAllSeries();

    QLineSeries *revenus = new QLineSeries();
    QLineSeries *depenses = new QLineSeries();

    revenus->setName("Revenus");
    depenses->setName("Dépenses");

    QDate today = QDate::currentDate();
    int year = today.year();
    QVariantMap evolution = DatabaseManager::instance().getMonthlyEvolution(year);

    for (int i = 1; i <= 12; ++i) {
        QString key = QString("month_%1").arg(i);
        QVariantMap m = evolution[key].toMap();
        revenus->append(i, m["revenue"].toDouble());
        depenses->append(i, m["expenses"].toDouble());
    }

    chart->addSeries(revenus);
    chart->addSeries(depenses);
    chart->createDefaultAxes();
}

void FinancesPage::on_addTransactionButton_clicked()
{
    TransactionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getTransactionData();
        if (DatabaseManager::instance().addTransaction(data)) {
            QMessageBox::information(this, "Succès", "Transaction ajoutée !");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de l'ajout !");
        }
    }
}

void FinancesPage::on_modifyButton_clicked()
{
    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez une transaction à modifier !");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    QVariantMap currentData = DatabaseManager::instance().getTransaction(code);

    TransactionDialog dialog(currentData, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap data = dialog.getTransactionData();
        if (DatabaseManager::instance().updateTransaction(code, data)) {
            QMessageBox::information(this, "Succès", "Transaction modifiée !");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la modification !");
        }
    }
}

void FinancesPage::on_deleteButton_clicked()
{
    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez une transaction à supprimer !");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    auto rep = QMessageBox::question(this, "Confirmation", "Supprimer cette transaction ?", QMessageBox::Yes | QMessageBox::No);

    if (rep == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteTransaction(code)) {
            QMessageBox::information(this, "Succès", "Transaction supprimée !");
            loadTransactions();
            refreshStatistics();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de suppression !");
        }
    }
}

void FinancesPage::on_searchButton_clicked()
{
    QString term = ui->searchLineEdit->text().trimmed();
    ui->transactionsTable->setRowCount(0);

    QList<QVariantMap> transactions = DatabaseManager::instance().searchTransactions(term);

    for (const QVariantMap &transaction : transactions) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(transaction["code"].toString()));
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(transaction["type"].toString()));
        ui->transactionsTable->setItem(row, 2, new QTableWidgetItem(transaction["categorie"].toString()));
        ui->transactionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(transaction["montant"].toDouble(), 'f', 2)));
        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(transaction["description"].toString()));
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(transaction["date"].toString()));
    }
}

void FinancesPage::closeEvent(QCloseEvent *event)
{
    saveAllToDatabase();
    event->accept();
}

void FinancesPage::saveAllToDatabase()
{
    if (!ui->transactionsTable) return;

    for (int row = 0; row < ui->transactionsTable->rowCount(); ++row) {
        QVariantMap data;
        data["code"] = ui->transactionsTable->item(row, 0)->text();
        data["type"] = ui->transactionsTable->item(row, 1)->text();
        data["categorie"] = ui->transactionsTable->item(row, 2)->text();
        data["montant"] = ui->transactionsTable->item(row, 3)->text().toDouble();
        data["description"] = ui->transactionsTable->item(row, 4)->text();
        data["date"] = ui->transactionsTable->item(row, 5)->text();

        QVariantMap existing = DatabaseManager::instance().getTransaction(data["code"].toString());
        if (existing.isEmpty())
            DatabaseManager::instance().addTransaction(data);
        else
            DatabaseManager::instance().updateTransaction(data["code"].toString(), data);
    }

    QSqlDatabase::database().commit(); // ✅ Commit pour Oracle
    QMessageBox::information(this, "Sauvegarde", "Toutes les transactions ont été enregistrées dans Oracle !");
}

// ✅ Fonctions manquantes corrigées pour éviter les erreurs de linkage
void FinancesPage::on_sortComboChanged(int index)
{
    if (!ui->transactionsTable) return;
    int column = (index == 0) ? 5 : (index == 1) ? 3 : 1;
    ui->transactionsTable->sortItems(column, Qt::DescendingOrder);
}

void FinancesPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "Export PDF", "Fonctionnalité d'export PDF à venir !");
}

void FinancesPage::on_sendMessageButton_clicked()
{
    QString question = ui->questionLineEdit->text().trimmed();
    if (question.isEmpty()) return;

    ui->chatTextEdit->append("<b>Vous :</b> " + question);
    QString response = getAssistantResponse(question);
    ui->chatTextEdit->append("<b>🤖 Assistant :</b> " + response);
}

QString FinancesPage::getAssistantResponse(const QString &question)
{
    QString q = question.toLower();

    if (q.contains("solde")) {
        QDate d = QDate::currentDate();
        QVariantMap stats = DatabaseManager::instance().getFinancialStatistics(d.year(), d.month());
        return QString("Le solde actuel est de %1 DT").arg(stats["balance"].toDouble(), 0, 'f', 2);
    }

    if (q.contains("revenus")) {
        QDate d = QDate::currentDate();
        double r = DatabaseManager::instance().getTotalRevenue(d.year(), d.month());
        return QString("Revenus totaux : %1 DT").arg(r, 0, 'f', 2);
    }

    if (q.contains("dépenses") || q.contains("depenses")) {
        QDate d = QDate::currentDate();
        double e = DatabaseManager::instance().getTotalExpenses(d.year(), d.month());
        return QString("Dépenses totales : %1 DT").arg(e, 0, 'f', 2);
    }

    return "Je peux vous informer sur le solde, les revenus, les dépenses ou les tendances du mois !";
}
