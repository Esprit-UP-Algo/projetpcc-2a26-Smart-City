#include "financespage.h"
#include "ui_financespage.h"
#include "transactiondialog.h"
#include "databasemanager.h"

#include <QMessageBox>
#include <QDate>
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QTableWidgetItem>
#include <QTextCursor>

FinancesPage::FinancesPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::FinancesPage)
{
    ui->setupUi(this);
    setupUi();

    // CHARGE LES DONNÉES AU DÉMARRAGE
    loadTransactions();
    refreshStatistics();
}

FinancesPage::~FinancesPage()
{
    delete ui;
}

void FinancesPage::setupUi()
{
    ui->transactionsTable->setColumnCount(6);
    QStringList headers = {"Code", "Date", "Type", "Montant", "Catégorie", "Description"};
    ui->transactionsTable->setHorizontalHeaderLabels(headers);
    ui->transactionsTable->horizontalHeader()->setStretchLastSection(true);
    ui->transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->transactionsTable->setSortingEnabled(true);

    ui->sortComboBox->clear();
    ui->sortComboBox->addItems({"Date (récent)", "Date (ancien)", "Montant (haut)", "Montant (bas)", "Code"});

    QString buttonStyle = R"(
        QPushButton {
            padding: 10px 15px;
            border: none;
            border-radius: 6px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton:hover { opacity: 0.9; }
    )";

    ui->addTransactionButton->setStyleSheet(buttonStyle + "background-color: #27AE60; color: white;");
    ui->modifyButton->setStyleSheet(buttonStyle + "background-color: #3498DB; color: white;");
    ui->deleteButton->setStyleSheet(buttonStyle + "background-color: #E74C3C; color: white;");
    ui->searchButton->setStyleSheet(buttonStyle + "background-color: #F39C12; color: white;");
    ui->exportPDFButton->setStyleSheet(buttonStyle + "background-color: #9B59B6; color: white;");
    ui->sendButton->setStyleSheet(buttonStyle + "background-color: #16A085; color: white;");
}

// CHARGE TOUTES LES TRANSACTIONS DEPUIS LA BDD
void FinancesPage::loadTransactions()
{
    ui->transactionsTable->setRowCount(0);  // Vide le tableau

    QList<QVariantMap> transactions = DatabaseManager::instance().getAllTransactions();
    qDebug() << "[loadTransactions] Nombre de transactions :" << transactions.size();

    for (const QVariantMap &t : transactions) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);

        // CODE
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(t["code"].toString()));

        // DATE
        QDate date = QDate::fromString(t["date"].toString(), "yyyy-MM-dd");
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(date.toString("dd/MM/yyyy")));

        // TYPE
        QString type = t["type"].toString() == "Entree" ? "Entrée" : "Sortie";
        QTableWidgetItem *typeItem = new QTableWidgetItem(type);
        typeItem->setForeground(QBrush(type == "Entrée" ? QColor("#27AE60") : QColor("#E74C3C")));
        ui->transactionsTable->setItem(row, 2, typeItem);

        // MONTANT
        double montant = t["montant"].toDouble();
        QTableWidgetItem *montantItem = new QTableWidgetItem(QString::number(montant, 'f', 2) + " DT");
        montantItem->setForeground(QBrush(type == "Entrée" ? QColor("#27AE60") : QColor("#E74C3C")));
        montantItem->setData(Qt::UserRole, montant);
        ui->transactionsTable->setItem(row, 3, montantItem);

        // CATÉGORIE
        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(t["categorie"].toString()));

        // DESCRIPTION
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(t["description"].toString()));
    }

    ui->transactionsTable->resizeColumnsToContents();
    qDebug() << "[loadTransactions] Tableau mis à jour avec" << transactions.size() << "lignes";
}

void FinancesPage::refreshStatistics()
{
    updateFinancialIndicators();
    updateChart();
}

void FinancesPage::updateFinancialIndicators()
{
    double totalRevenus = 0, totalDepenses = 0;
    int nbTransactions = 0;

    QList<QVariantMap> transactions = DatabaseManager::instance().getAllTransactions();
    for (const QVariantMap &t : transactions) {
        double montant = t["montant"].toDouble();
        if (t["type"].toString() == "Entree") totalRevenus += montant;
        else totalDepenses += montant;
        nbTransactions++;
    }

    double balance = totalRevenus - totalDepenses;

    ui->revenueLabel->setText(QString("Revenus: %1 DT").arg(totalRevenus, 0, 'f', 2));
    ui->expensesLabel->setText(QString("Dépenses: %1 DT").arg(totalDepenses, 0, 'f', 2));
    ui->balanceLabel->setText(QString("Solde: %1 DT").arg(balance, 0, 'f', 2));

    ui->statusBadge->setText(balance >= 0 ? "Tendance: Excédent" : "Tendance: Déficit");
    ui->statusBadge->setStyleSheet(balance >= 0 ?
                                       "background-color: #27AE60; color: white; padding: 10px; border-radius: 8px;" :
                                       "background-color: #E74C3C; color: white; padding: 10px; border-radius: 8px;");

    ui->processingDelayLabel->setText(QString("Total transactions: %1").arg(nbTransactions));
}

void FinancesPage::updateChart()
{
    // À implémenter plus tard
}

// AJOUT DE TRANSACTION – CORRIGÉ
void FinancesPage::on_addTransactionButton_clicked()
{
    TransactionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        qDebug() << "[on_addTransactionButton_clicked] Annulé par l'utilisateur";
        return;
    }

    QVariantMap data = dialog.getTransactionData();

    // GÉNÉRER UN CODE UNIQUE
    if (data["code"].toString().isEmpty()) {
        data["code"] = "TRN" + QDateTime::currentDateTime().toString("yyMMddhhmmss");
    }

    qDebug() << "[on_addTransactionButton_clicked] Données à insérer :" << data;

    // AJOUT DANS LA BDD
    if (!DatabaseManager::instance().addTransaction(data)) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout en base de données.");
        qDebug() << "[ERREUR] addTransaction() a échoué";
        return;
    }

    qDebug() << "[SUCCÈS] Transaction ajoutée en BDD avec code :" << data["code"].toString();

    // RECHARGEMENT COMPLET DU TABLEAU
    loadTransactions();           // CRUCIAL
    refreshStatistics();          // Met à jour les stats

    // MESSAGE DANS LE CHAT
    double montant = data["montant"].toDouble();
    QString type = data["type"].toString() == "Entree" ? "Entrée" : "Sortie";

    ui->chatTextEdit->append(QString(
                                 "<p style='color: #27AE60; background: #EAFAF1; padding: 10px; border-radius: 6px; margin: 5px;'>"
                                 "<b>Transaction ajoutée :</b> %1 | %2 DT | %3</p>"
                                 ).arg(data["code"].toString(), QString::number(montant, 'f', 2), type));

    qDebug() << "[on_addTransactionButton_clicked] Tableau rafraîchi avec succès";
}

void FinancesPage::on_searchButton_clicked()
{
    QString term = ui->searchLineEdit->text().trimmed();
    if (term.isEmpty()) {
        loadTransactions();
        return;
    }

    ui->transactionsTable->setRowCount(0);
    auto results = DatabaseManager::instance().searchTransactions(term);
    for (const auto &t : results) {
        int row = ui->transactionsTable->rowCount();
        ui->transactionsTable->insertRow(row);
        ui->transactionsTable->setItem(row, 0, new QTableWidgetItem(t["code"].toString()));
        ui->transactionsTable->setItem(row, 1, new QTableWidgetItem(
                                                   QDate::fromString(t["date"].toString(), "yyyy-MM-dd").toString("dd/MM/yyyy")));

        QString type = t["type"].toString() == "Entree" ? "Entrée" : "Sortie";
        QTableWidgetItem *typeItem = new QTableWidgetItem(type);
        typeItem->setForeground(QBrush(type == "Entrée" ? QColor("#27AE60") : QColor("#E74C3C")));
        ui->transactionsTable->setItem(row, 2, typeItem);

        double montant = t["montant"].toDouble();
        QTableWidgetItem *montantItem = new QTableWidgetItem(QString::number(montant, 'f', 2) + " DT");
        montantItem->setForeground(QBrush(type == "Entrée" ? QColor("#27AE60") : QColor("#E74C3C")));
        ui->transactionsTable->setItem(row, 3, montantItem);

        ui->transactionsTable->setItem(row, 4, new QTableWidgetItem(t["categorie"].toString()));
        ui->transactionsTable->setItem(row, 5, new QTableWidgetItem(t["description"].toString()));
    }
}

void FinancesPage::on_sortComboBox_currentIndexChanged(int index)
{
    if (ui->transactionsTable->rowCount() == 0) return;
    int col = 1; Qt::SortOrder order = Qt::DescendingOrder;
    switch (index) {
    case 0: col = 1; order = Qt::DescendingOrder; break;
    case 1: col = 1; order = Qt::AscendingOrder; break;
    case 2: col = 3; order = Qt::DescendingOrder; break;
    case 3: col = 3; order = Qt::AscendingOrder; break;
    case 4: col = 0; order = Qt::AscendingOrder; break;
    }
    ui->transactionsTable->sortItems(col, order);
}

void FinancesPage::on_modifyButton_clicked()
{
    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Sélectionnez une transaction à modifier.");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    auto data = DatabaseManager::instance().getTransaction(code);
    if (data.isEmpty()) return;

    TransactionDialog dialog(data, this);
    if (dialog.exec() == QDialog::Accepted) {
        auto newData = dialog.getTransactionData();
        if (DatabaseManager::instance().updateTransaction(code, newData)) {
            loadTransactions();      // Rechargement complet
            refreshStatistics();
            QMessageBox::information(this, "Succès", "Transaction modifiée.");
        }
    }
}

void FinancesPage::on_deleteButton_clicked()
{
    int row = ui->transactionsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Sélectionnez une transaction à supprimer.");
        return;
    }

    QString code = ui->transactionsTable->item(row, 0)->text();
    if (QMessageBox::question(this, "Confirmer", "Supprimer cette transaction ?") == QMessageBox::Yes) {
        if (DatabaseManager::instance().deleteTransaction(code)) {
            loadTransactions();      // Rechargement complet
            refreshStatistics();
            QMessageBox::information(this, "Supprimé", "Transaction supprimée.");
        }
    }
}

void FinancesPage::on_exportPDFButton_clicked()
{
    QMessageBox::information(this, "PDF", "Fonctionnalité à implémenter.");
}

void FinancesPage::on_sendButton_clicked()
{
    QString q = ui->questionLineEdit->text().trimmed();
    if (q.isEmpty()) return;

    ui->chatTextEdit->append(QString(
                                 "<p style='color: #2C3E50; background: #E8F8F5; padding: 10px; border-radius: 6px; margin: 5px;'>"
                                 "<b>Vous :</b> %1</p>"
                                 ).arg(q));

    ui->chatTextEdit->append(QString(
                                 "<p style='color: #34495E; background: #EBF5FB; padding: 10px; border-radius: 6px; margin: 5px;'>"
                                 "<b>Assistant :</b> %1</p>"
                                 ).arg(getAssistantResponse(q)));

    ui->questionLineEdit->clear();

    QTextCursor cursor = ui->chatTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->chatTextEdit->setTextCursor(cursor);
}

QString FinancesPage::getAssistantResponse(const QString &question)
{
    QString q = question.toLower();
    if (q.contains("solde") || q.contains("balance")) return "Le solde est affiché en haut : revenu - dépenses.";
    if (q.contains("ajouter") || q.contains("nouvelle")) return "Cliquez sur 'Nouvelle Transaction' pour ajouter.";
    if (q.contains("total") || q.contains("combien")) return "Consultez les statistiques en haut.";
    if (q.contains("supprimer")) return "Sélectionnez une ligne puis cliquez sur 'Supprimer'.";
    return "Posez-moi une question sur vos finances !";
}
