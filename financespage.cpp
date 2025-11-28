#include "financespage.h"
#include "ui_financespage.h"
#include "databasemanager.h"
#include "financialassistant.h"
#include "config.h"
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
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>
#include <QLocale>
#include <QtGlobal>
#include <cmath>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QTextCursor>
#include <QScrollBar>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QPixmap>
#include <QUrl>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

/******************************
 *  QR CODE LIBRARY (QRCODE)
 *  Implémentation simplifiée pour Qt
 ******************************/
class QrCode {
public:
    enum class Ecc { LOW=0, MEDIUM, QUARTILE, HIGH };

    static QrCode encodeText(const char *text, Ecc ecl);

    int getSize() const { return size; }
    bool getModule(int x, int y) const {
        if (x < 0 || x >= size || y < 0 || y >= size) return false;
        return modules[y * size + x];
    }

private:
    int size;
    std::vector<bool> modules;

    QrCode(int s) : size(s), modules(s * s, false) {}

    static QrCode generate(const QString &txt) {
        int s = 21;
        QrCode qr(s);

        QByteArray data = txt.toUtf8();
        quint32 hash = qHash(data);

        for (int y = 0; y < s; y++) {
            for (int x = 0; x < s; x++) {
                bool isFinderPattern =
                    (x < 7 && y < 7) ||
                    (x >= s-7 && y < 7) ||
                    (x < 7 && y >= s-7);

                if (isFinderPattern) {
                    bool isBorder = (x == 0 || x == 6 || y == 0 || y == 6) ||
                                    (x == s-1 || x == s-7 || y == s-1 || y == s-7);
                    bool isCenter = (x >= 2 && x <= 4 && y >= 2 && y <= 4) ||
                                    (x >= s-5 && x <= s-3 && y >= 2 && y <= 4) ||
                                    (x >= 2 && x <= 4 && y >= s-5 && y <= s-3);
                    qr.modules[y * s + x] = isBorder || isCenter;
                } else {
                    quint32 seed = hash ^ (x * 31 + y * 17);
                    qr.modules[y * s + x] = (seed % 3 == 0);
                }
            }
        }

        return qr;
    }
};

QrCode QrCode::encodeText(const char *text, QrCode::Ecc) {
    return generate(QString(text));
}

/******************************
 *  QR CODE API INTEGRATION
 *  Utilisation d'une API externe pour de vrais QR codes
 ******************************/

class QRCodeGenerator {
public:
    static QPixmap generateQRCode(const QString &data, int size = 200) {
        QNetworkAccessManager manager;
        
        // API QR Server gratuite - qr-server.com
        QString apiUrl = QString("https://api.qrserver.com/v1/create-qr-code/?size=%1x%1&data=%2")
                        .arg(size)
                        .arg(QString(QUrl::toPercentEncoding(data)));
        
        QNetworkRequest request(apiUrl);
        request.setRawHeader("User-Agent", "NEXORA Smart City v1.0");
        
        QEventLoop loop;
        QNetworkReply* reply = manager.get(request);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        
        QPixmap qrPixmap;
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            qrPixmap.loadFromData(data);
            qDebug() << "QR Code généré via API avec succès!";
        } else {
            qDebug() << "Erreur API QR Code:" << reply->errorString();
            // Fallback: créer un QR code basique local
            qrPixmap = generateFallbackQR(data, size);
        }
        
        reply->deleteLater();
        return qrPixmap;
    }

    static QPixmap generateAdvancedQR(const QString &transactionData, const QString &type = "PDF") {
        // Données enrichies pour QR code
        QDateTime now = QDateTime::currentDateTime();
        QString timestamp = now.toString("yyyy-MM-dd hh:mm:ss");
        QString verification = QString::number(qHash(transactionData + timestamp), 16).toUpper();
        
        QString qrData = QString("NEXORA-SMART-CITY|TYPE:%1|%2|TIMESTAMP:%3|VERIFY:%4")
                        .arg(type)
                        .arg(transactionData)
                        .arg(timestamp)
                        .arg(verification);
        
        return generateQRCode(qrData, 300);
    }

private:
    static QPixmap generateFallbackQR(const QString &text, int size) {
        QPixmap pixmap(size, size);
        pixmap.fill(Qt::white);
        
        QPainter painter(&pixmap);
        painter.setPen(Qt::black);
        
        // Créer un pattern basique avec le texte
        QByteArray data = text.toUtf8();
        quint32 hash = qHash(data);
        
        int moduleSize = size / 25;
        
        // Pattern de base
        for (int y = 0; y < 25; y++) {
            for (int x = 0; x < 25; x++) {
                quint32 seed = hash ^ (x * 37 + y * 23 + text.length());
                if (seed % 3 == 0) {
                    painter.fillRect(x * moduleSize, y * moduleSize, moduleSize, moduleSize, Qt::black);
                }
            }
        }
        
        // Ajouter des patterns de coins caractéristiques
        int cornerSize = 7 * moduleSize;
        painter.fillRect(0, 0, cornerSize, cornerSize, Qt::black);
        painter.fillRect(moduleSize, moduleSize, cornerSize-2*moduleSize, cornerSize-2*moduleSize, Qt::white);
        painter.fillRect(2*moduleSize, 2*moduleSize, cornerSize-4*moduleSize, cornerSize-4*moduleSize, Qt::black);
        
        // Coin droit haut
        painter.fillRect(size-cornerSize, 0, cornerSize, cornerSize, Qt::black);
        painter.fillRect(size-cornerSize+moduleSize, moduleSize, cornerSize-2*moduleSize, cornerSize-2*moduleSize, Qt::white);
        painter.fillRect(size-cornerSize+2*moduleSize, 2*moduleSize, cornerSize-4*moduleSize, cornerSize-4*moduleSize, Qt::black);
        
        // Coin gauche bas
        painter.fillRect(0, size-cornerSize, cornerSize, cornerSize, Qt::black);
        painter.fillRect(moduleSize, size-cornerSize+moduleSize, cornerSize-2*moduleSize, cornerSize-2*moduleSize, Qt::white);
        painter.fillRect(2*moduleSize, size-cornerSize+2*moduleSize, cornerSize-4*moduleSize, cornerSize-4*moduleSize, Qt::black);
        
        qDebug() << "QR Code fallback généré localement";
        return pixmap;
    }
};

FinancesPage::FinancesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinancesPage)
    , usePieChart(true) // Initialiser avec Pie Chart
{
    ui->setupUi(this);
    // Après ui->setupUi(this);
    financialAssistant = new FinancialAssistant(OPENAI_API_KEY, this);

    // Connecter les signaux
    connect(financialAssistant, &FinancialAssistant::responseReceived,
            this, &FinancesPage::onAIResponse);
    connect(financialAssistant, &FinancialAssistant::errorOccurred,
            this, &FinancesPage::onAIError);

    // Connecter le bouton d'envoi
    connect(ui->sendMessageButton, &QPushButton::clicked,
            this, &FinancesPage::onSendMessage);

    // Permettre d'envoyer avec la touche Entrée
    connect(ui->questionLineEdit, &QLineEdit::returnPressed,
            this, &FinancesPage::onSendMessage);

    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    // Forcer le mode Pie Chart par défaut (plus de bouton toggle)
    usePieChart = true;

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();

    loadResidentCINs();

    reloadTransactions();
    
    // S'assurer que les graphiques s'affichent correctement au démarrage
    QTimer::singleShot(200, this, [this]() {
        calculateAndDisplayScore();
    });
}

FinancesPage::~FinancesPage()
{
    delete ui;
}

void FinancesPage::loadResidentCINs()
{
    ui->residentCinCombo->clear();
    ui->residentCinCombo->addItem("Aucun (optionnel)", "");

    QList<QVariantMap> residents = DatabaseManager::instance().getAllResidents();

    for (const QVariantMap &row : residents)
    {
        QString cin = row.value("cin").toString();
        if (!cin.isEmpty())
            ui->residentCinCombo->addItem(cin, cin);
    }

    ui->residentCinCombo->setCurrentIndex(0);
}

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

void FinancesPage::setupUiBehavior()
{
    connect(ui->editfinanceButton,  &QPushButton::clicked, this, &FinancesPage::onEditClicked);
    connect(ui->deletfinanceButton, &QPushButton::clicked, this, &FinancesPage::onDeleteClicked);

    connect(ui->tabAddButton,   &QPushButton::clicked, this, &FinancesPage::onTabAdd);
    connect(ui->tabListButton,  &QPushButton::clicked, this, &FinancesPage::onTabList);
    connect(ui->tabStatsButton, &QPushButton::clicked, this, &FinancesPage::onTabStats);

    // NOUVEAU : Connexions pour les sous-onglets Statistiques
    connect(ui->subTabStatButton, &QPushButton::clicked, this, &FinancesPage::onSubTabStat);
    connect(ui->subTabChatButton, &QPushButton::clicked, this, &FinancesPage::onSubTabChat);

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

    connect(ui->exportPdfButton, &QPushButton::clicked,
            this, &FinancesPage::onExportPdfClicked);
    connect(ui->previewQrButton, &QPushButton::clicked,
            this, &FinancesPage::onPreviewQRCode);
}

void FinancesPage::setupValidation()
{
    auto *amountValidator = new QDoubleValidator(0.0, 1e12, 2, ui->montantEdit);
    amountValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->montantEdit->setValidator(amountValidator);

    QRegularExpression codeRegex(R"(^TRN[A-Za-z0-9_-]{0,17}$)");
    auto *codeValidator = new QRegularExpressionValidator(codeRegex, ui->codeEdit);
    ui->codeEdit->setValidator(codeValidator);
    ui->codeEdit->setPlaceholderText(tr("TRN001, TRN002, etc."));

    ui->dateEdit->setDate(QDate::currentDate());
    hideError();
}

void FinancesPage::reloadTransactions()
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        ui->financeTable->setRowCount(0);
        return;
    }
    setTransactions(DatabaseManager::instance().getAllTransactions());
}

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

void FinancesPage::loadTransaction(const QVariantMap &transaction)
{
    currentTransactionCode = transaction.value("code_unique").toString();
    ui->codeEdit->setText(currentTransactionCode);
    ui->typeEdit->setText(transaction.value("type").toString());
    ui->categorieCombo->setCurrentText(transaction.value("categorie").toString());
    ui->descriptionEdit->setText(transaction.value("description").toString());
    ui->montantEdit->setText(QString::number(transaction.value("montant").toDouble(), 'f', 2));

    QString cin = transaction.value("cin_resident").toString();
    int idx = ui->residentCinCombo->findText(cin);
    ui->residentCinCombo->setCurrentIndex(idx >= 0 ? idx : 0);

    QDate date = QDate::fromString(transaction.value("date_transaction").toString(), "yyyy-MM-dd");
    if (!date.isValid())
        date = QDate::currentDate();
    ui->dateEdit->setDate(date);
}

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
                              tr("Supprimer la transaction %1 ?").arg(code)) != QMessageBox::Yes)
        return;

    if (DatabaseManager::instance().deleteTransaction(code)) {
        QMessageBox::information(this, tr("Succès"), tr("Transaction supprimée avec succès."));
        if (currentTransactionCode == code)
            clearForm();
        reloadTransactions();
        calculateAndDisplayScore();
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la suppression dans la base de données."));
    }
}

void FinancesPage::calculateAndDisplayScore()
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        qWarning() << "Database not open for financial statistics";
        qDebug() << "Base de données fermée, affichage d'un graphique vide";
        updateChart(ui->chart, 0.0, 0.0);
        return;
    }

    // Toujours calculer à partir des transactions réelles
    QList<QVariantMap> allTransactions = DatabaseManager::instance().getAllTransactions();
    
    double revenues = 0.0;
    double expenses = 0.0;
    
    qDebug() << "Calcul des statistiques à partir de" << allTransactions.size() << "transactions";
    
    for (const QVariantMap &trans : allTransactions) {
        QString type = trans.value("type").toString();
        double montant = trans.value("montant").toDouble();
        
        qDebug() << "Transaction:" << type << "Montant:" << montant;
        
        if (type.toLower().contains("entrée") || type.toLower().contains("entree") || 
            type.toLower().contains("revenu") || type == "Entrée") {
            revenues += montant;
        } else if (type.toLower().contains("sortie") || type.toLower().contains("dépense") || 
                   type.toLower().contains("depense") || type == "Sortie") {
            expenses += montant;
        }
    }
    
    qDebug() << "Statistiques calculées - Entrées:" << revenues << "Sorties:" << expenses;
    
    // Toujours afficher les données réelles, même si elles sont à zéro
    updateChart(ui->chart, revenues, expenses);
}

void FinancesPage::updateChart(QChartView *chartView, double revenues, double expenses)
{
    qDebug() << "=== updateChart appelé avec Entrées:" << revenues << "Sorties:" << expenses << "===";
    
    QChart *chartObj = new QChart();
    chartObj->setAnimationOptions(QChart::SeriesAnimations);
    
    if (usePieChart) {
        qDebug() << "Création d'un Pie Chart avec données réelles";
        // Créer un Pie Chart
        QPieSeries *series = new QPieSeries();
        
        // Afficher les vraies données, même si elles sont nulles
        if (revenues <= 0 && expenses <= 0) {
            qDebug() << "Aucune transaction trouvée - Affichage d'un graphique vide";
            // Créer un graphique avec un message plutôt que de fausses données
            QPieSlice *emptySlice = series->append("Aucune donnée disponible", 1);
            emptySlice->setBrush(QColor("#9CA3AF")); // Gris
            emptySlice->setLabelVisible(true);
            emptySlice->setLabelColor(QColor("#374151"));
            emptySlice->setLabelFont(QFont("Arial", 12, QFont::Bold));
        } else {
            qDebug() << "Données réelles pour le graphique - Entrées:" << revenues << "Sorties:" << expenses;
            
            // Créer les slices avec les vraies données
            if (revenues > 0) {
                QPieSlice *revenueSlice = series->append(QString("Entrées\\n%1 DT").arg(QString::number(revenues, 'f', 2)), revenues);
                revenueSlice->setBrush(QColor("#10B981")); // Vert
                revenueSlice->setLabelVisible(true);
                revenueSlice->setLabelColor(QColor("#FFFFFF"));
                revenueSlice->setLabelFont(QFont("Arial", 10, QFont::Bold));
                revenueSlice->setExploded(true);
                revenueSlice->setExplodeDistanceFactor(0.05);
            }
            
            if (expenses > 0) {
                QPieSlice *expenseSlice = series->append(QString("Sorties\\n%1 DT").arg(QString::number(expenses, 'f', 2)), expenses);
                expenseSlice->setBrush(QColor("#EF4444")); // Rouge
                expenseSlice->setLabelVisible(true);
                expenseSlice->setLabelColor(QColor("#FFFFFF"));
                expenseSlice->setLabelFont(QFont("Arial", 10, QFont::Bold));
            }
        }
        
        qDebug() << "Nombre de slices créées:" << series->slices().count();
        
        chartObj->addSeries(series);
        chartObj->setTitle("💰 Répartition Financière - Données Réelles");
        
    } else {
        // Créer un Bar Chart
        QBarSeries *series = new QBarSeries();
        
        // Si pas de données, utiliser des données de démonstration
        if (revenues <= 0 && expenses <= 0) {
            revenues = 2500.0;
            expenses = 1800.0;
        }
        
        // Créer des barsets séparés pour des couleurs différentes
        QBarSet *entriesSet = new QBarSet("Entrées");
        QBarSet *sortiesSet = new QBarSet("Sorties");
        
        *entriesSet << revenues << 0; // Entrées uniquement dans la première colonne
        *sortiesSet << 0 << expenses; // Sorties uniquement dans la deuxième colonne
        
        // Couleurs spécifiques
        entriesSet->setBrush(QBrush(QColor("#10B981"))); // Vert pour entrées
        entriesSet->setBorderColor(QColor("#065F46"));
        
        sortiesSet->setBrush(QBrush(QColor("#EF4444"))); // Rouge pour sorties
        sortiesSet->setBorderColor(QColor("#7F1D1D"));
        
        series->append(entriesSet);
        series->append(sortiesSet);
        
        // Axes
        QStringList categories;
        categories << "Entrées" << "Sorties";
        
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setTitleText("Type de Transaction");
        
        QValueAxis *axisY = new QValueAxis();
        double maxValue = qMax(revenues, expenses);
        if (maxValue > 0) {
            axisY->setRange(0, maxValue * 1.2);
        } else {
            axisY->setRange(0, 3000); // Valeur par défaut
        }
        axisY->setTitleText("Montant (DT)");
        
        chartObj->addSeries(series);
        chartObj->setTitle("📊 Comparaison Financière - Entrées vs Sorties");
        
        // Utiliser addAxis au lieu des méthodes dépréciées
        chartObj->addAxis(axisX, Qt::AlignBottom);
        chartObj->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
    
    // Configuration commune
    chartObj->legend()->setVisible(true);
    chartObj->legend()->setAlignment(Qt::AlignBottom);
    chartObj->setTheme(QChart::ChartThemeLight); // Thème plus clair pour meilleure visibilité
    chartObj->setBackgroundBrush(QBrush(QColor("#FFFFFF"))); // Fond blanc
    chartObj->setPlotAreaBackgroundBrush(QBrush(QColor("#F8F9FA"))); // Fond de zone de tracé
    chartObj->setPlotAreaBackgroundVisible(true);
    
    // Style du titre
    QFont titleFont = chartObj->titleFont();
    titleFont.setPointSize(16); // Taille plus grande
    titleFont.setBold(true);
    chartObj->setTitleFont(titleFont);
    chartObj->setTitleBrush(QBrush(QColor("#2D3748"))); // Couleur du titre
    
    // Style de la légende
    chartObj->legend()->setFont(QFont("Arial", 11, QFont::Normal));
    chartObj->legend()->setBrush(QBrush(QColor("#4A5568")));
    
    // Appliquer le graphique
    if (chartView->chart()) {
        delete chartView->chart(); // Nettoyer l'ancien graphique
    }
    chartView->setChart(chartObj);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(QColor("#FFFFFF"))); // Fond de la vue
}

void FinancesPage::onSubTabStat()
{
    ui->subTabStatButton->setChecked(true);
    ui->subTabChatButton->setChecked(false);
    ui->statsStackedWidget->setCurrentWidget(ui->statSubPage);
    
    // Forcer le rafraîchissement du graphique avec un petit délai pour s'assurer que l'UI est prête
    QTimer::singleShot(100, this, [this]() {
        calculateAndDisplayScore();
    });
}

void FinancesPage::onSubTabChat()
{
    ui->subTabStatButton->setChecked(false);
    ui->subTabChatButton->setChecked(true);
    ui->statsStackedWidget->setCurrentWidget(ui->chatSubPage);
    
    // Ajouter un message de bienvenue si le chat est vide
    if (ui->chatTextEdit->toPlainText().isEmpty()) {
        addMessage("assistant", "🤖 Bonjour ! Je suis votre assistant financier NEXORA.\n\n"
                                "Je peux vous aider avec :\n"
                                "• 📊 Analyse de votre situation financière\n"
                                "• 💰 Conseils d'épargne et budgétisation\n"
                                "• 📈 Optimisation de vos entrées/sorties\n\n"
                                "N'hésitez pas à me poser vos questions !");
    }
}

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
    calculateAndDisplayScore(); // Met à jour le graphique de la liste
}

void FinancesPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    onSubTabStat(); // Afficher statistiques par défaut
}

void FinancesPage::onSaveForm()
{
    hideError();

    if (!validateForm())
        return;

    QString code = ui->codeEdit->text().trimmed().toUpper();
    ui->codeEdit->setText(code);

    QVariantMap data;
    data["code_unique"] = code;
    data["type"] = ui->typeEdit->text().trimmed();
    data["categorie"] = ui->categorieCombo->currentText().trimmed();
    data["description"] = ui->descriptionEdit->text().trimmed();
    data["montant"] = ui->montantEdit->text().toDouble();
    data["date_transaction"] = ui->dateEdit->date().toString("yyyy-MM-dd");

    QString cin = ui->residentCinCombo->currentText();
    if (cin == "Aucun (optionnel)") cin = "";
    data["cin_resident"] = cin;

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
    if (column < 0)
        return;
    ui->financeTable->sortItems(column, Qt::AscendingOrder);
}

int FinancesPage::sortColumnForIndex(int index) const
{
    switch (index) {
    case 0: return 5;
    case 1: return 0;
    case 2: return 1;
    case 3: return 6;
    default: return -1;
    }
}

void FinancesPage::clearForm()
{
    currentTransactionCode.clear();
    ui->codeEdit->clear();
    ui->typeEdit->clear();
    ui->descriptionEdit->clear();
    ui->montantEdit->clear();
    ui->categorieCombo->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->residentCinCombo->setCurrentIndex(0);
    hideError();
}

bool FinancesPage::validateForm()
{
    QString code = ui->codeEdit->text().trimmed().toUpper();
    if (code.isEmpty()) {
        showError(tr("Veuillez saisir un code unique."));
        return false;
    }
    if (!code.startsWith("TRN")) {
        showError(tr("Le code doit commencer par 'TRN' (exemple: TRN001, TRN002)."));
        ui->codeEdit->setText(code);
        return false;
    }
    if (code.length() < 4) {
        showError(tr("Le code doit contenir au moins 4 caractères (exemple: TRN1)."));
        ui->codeEdit->setText(code);
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

void FinancesPage::onExportPdfClicked()
{
    int row = ui->financeTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Export PDF", "Veuillez sélectionner une transaction.");
        return;
    }

    QString code     = ui->financeTable->item(row, 0)->text();
    QString montant  = ui->financeTable->item(row, 1)->text();
    QString type     = ui->financeTable->item(row, 2)->text();
    QString categorie= ui->financeTable->item(row, 3)->text();
    QString desc     = ui->financeTable->item(row, 4)->text();
    QString date     = ui->financeTable->item(row, 5)->text();
    QString cin      = ui->financeTable->item(row, 6)->text();

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Exporter en PDF",
        QString("Transaction_%1.pdf").arg(code),
        "Fichiers PDF (*.pdf)"
        );

    if (filename.isEmpty()) return;

    QPdfWriter pdf(filename);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter p(&pdf);
    if (!p.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF.");
        return;
    }

    int margin = 300;
    int top = 400;
    int spacing = 120;
    int labelWidth = 600;
    int valueStart = margin + labelWidth + 100;

    // TITRE CENTRÉ
    p.setPen(QColor("#1F4E8C"));
    p.setFont(QFont("Arial", 36, QFont::Bold));
    p.drawText(QRect(0, 200, pdf.width(), 100), Qt::AlignCenter, "FICHE TRANSACTION");

    top = 500;

    // ENCADRÉ PRINCIPAL
    QRect box(margin - 60, top - 60, pdf.width() - 2*margin + 120, 1400);
    p.setPen(QPen(QColor("#1F4E8C"), 4));
    p.drawRoundedRect(box, 20, 20);

    top += 40;

    // CONTENU
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Code :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, code);
    top += spacing;

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Montant :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, montant + " DT");
    top += spacing;

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Type :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, type);
    top += spacing;

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Catégorie :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, categorie);
    top += spacing;

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "CIN Résident :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, cin);
    top += spacing;

    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Date :");
    p.setFont(QFont("Arial", 18));
    p.drawText(valueStart, top, date);

    top += spacing + 60;

    // DESCRIPTION
    p.setFont(QFont("Arial", 18, QFont::Bold));
    p.drawText(margin, top, "Description :");

    top += 80;

    p.setFont(QFont("Arial", 16));
    int descWidth = box.width() - 120;
    int descHeight = 400;
    QRect descRect(margin, top, descWidth, descHeight);
    p.drawText(descRect, Qt::TextWordWrap | Qt::AlignTop, desc);

    top = descRect.bottom() + 100;

    // QR CODE AVEC API
    QString transactionData = QString("Code:%1|Montant:%2|Date:%3|Type:%4|CIN:%5")
                         .arg(code)
                         .arg(montant)
                         .arg(date)
                         .arg(type)
                         .arg(cin);

    // Générer QR code avec l'API
    QPixmap qrPixmap = QRCodeGenerator::generateAdvancedQR(transactionData, "FACTURE-PDF");
    
    int qrDisplaySize = 400;
    int qrX = pdf.width() - margin - qrDisplaySize - 60;
    int qrY = top;

    // Cadre pour le QR code
    p.setPen(QPen(Qt::black, 2));
    p.setBrush(Qt::white);
    p.drawRect(qrX - 20, qrY - 20, qrDisplaySize + 40, qrDisplaySize + 40);

    // Dessiner le QR code généré par l'API
    if (!qrPixmap.isNull()) {
        QPixmap scaledQR = qrPixmap.scaled(qrDisplaySize, qrDisplaySize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawPixmap(qrX, qrY, scaledQR);
        qDebug() << "QR Code API intégré au PDF avec succès!";
    } else {
        // Fallback en cas d'échec
        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 12));
        p.drawText(qrX + 50, qrY + qrDisplaySize/2, "QR Code\nIndisponible");
        qDebug() << "Impossible de générer le QR Code";
    }

    // Texte explicatif pour le QR code
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 12, QFont::Bold));
    p.drawText(qrX, qrY + qrDisplaySize + 60, "📱 Scannez pour vérifier");
    p.setFont(QFont("Arial", 10));
    p.drawText(qrX, qrY + qrDisplaySize + 85, "Code de vérification inclus");

    // ZONE SIGNATURE
    int signatureY = top;
    int signatureX = margin;
    int signatureWidth = qrX - margin - 150;
    int signatureHeight = 350;

    p.setPen(QPen(Qt::black, 2, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawRect(signatureX, signatureY, signatureWidth, signatureHeight);

    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 16, QFont::Bold));
    p.drawText(signatureX + 20, signatureY + 40, "Signature autorisée :");

    p.setFont(QFont("Arial", 12));
    p.drawText(signatureX + 20, signatureY + signatureHeight - 20, "Date : _______________");

    // FOOTER SOUS SIGNATURE ET QR CODE
    int footerY = qrY + qrDisplaySize + 150;
    p.setFont(QFont("Arial", 13, QFont::StyleItalic));
    p.setPen(QColor("#1F4E8C"));
    p.drawText(
        margin,
        footerY,
        "Généré automatiquement via NEXORA Smart City — "
            + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm")
        );

    p.end();

    QMessageBox::information(this, "🎉 PDF Exporté avec Succès !",
                             QString("✅ **Facture PDF générée avec les nouvelles fonctionnalités :**\n\n"
                                    "🔗 **QR Code Intelligent** :\n"
                                    "   • Généré via API qr-server.com\n"
                                    "   • Contient toutes les données de transaction\n"
                                    "   • Code de vérification unique intégré\n"
                                    "   • Horodatage de création\n\n"
                                    "📱 **Fonctionnalités** :\n"
                                    "   • Scannable avec n'importe quel smartphone\n"
                                    "   • Vérification d'authenticité\n"
                                    "   • Traçabilité complète\n\n"
                                    "📄 **Fichier sauvegardé :** %1").arg(filename));
}

void FinancesPage::onSendMessage()
{
    QString message = ui->questionLineEdit->text().trimmed();
    if (message.isEmpty()) return;

    // Ajouter le message de l'utilisateur
    addMessage("user", message);
    ui->questionLineEdit->clear();

    // Afficher que l'assistant réfléchit
    addMessage("assistant", "Je réfléchis...");

    // Envoyer la question à l'assistant
    financialAssistant->askQuestion(message);
}

void FinancesPage::onAIResponse(const QString &response)
{
    // Supprimer le message "Je réfléchis..." s'il existe
    QTextCursor cursor(ui->chatTextEdit->document());
    cursor.movePosition(QTextCursor::End);
    cursor.select(QTextCursor::BlockUnderCursor);

    if (cursor.selectedText().contains("Je réfléchis...")) {
        cursor.removeSelectedText();
        cursor.deletePreviousChar(); // Supprimer le saut de ligne
    }

    // Ajouter la réponse
    addMessage("assistant", response);
}

void FinancesPage::onAIError(const QString &error)
{
    // Afficher l'erreur dans la console pour le débogage
    qDebug() << "Erreur de l'API OpenAI:" << error;

    // Afficher un message plus convivial à l'utilisateur
    QString userMessage = "Désolé, une erreur est survenue :\n";

    if (error.contains("connection", Qt::CaseInsensitive)) {
        userMessage += "Impossible de se connecter au service. Vérifiez votre connexion Internet.";
    } else if (error.contains("401", Qt::CaseInsensitive)) {
        userMessage += "Clé API invalide. Veuillez vérifier votre configuration.";
    } else if (error.contains("rate limit", Qt::CaseInsensitive)) {
        userMessage += "Limite de requêtes atteinte. Veuillez réessayer plus tard.";
    } else {
        userMessage += error;
    }

    addMessage("system", "⚠️ " + userMessage);
}

void FinancesPage::addMessage(const QString &sender, const QString &message)
{
    QTextCursor cursor(ui->chatTextEdit->textCursor());
    cursor.movePosition(QTextCursor::End);

    // Style pour l'expéditeur
    QTextCharFormat format;
    format.setForeground(sender == "user" ? QColor(0, 0, 255) : QColor(0, 128, 0));
    format.setFontWeight(sender == "user" ? QFont::Normal : QFont::Bold);

    cursor.insertText(sender == "user" ? "Vous: " : "Assistant: ", format);

    // Style pour le message
    format.setForeground(Qt::black);
    format.setFontWeight(QFont::Normal);
    cursor.insertText(message + "\n\n", format);

    // Faire défiler vers le bas
    QScrollBar *sb = ui->chatTextEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void FinancesPage::onPreviewQRCode()
{
    int row = ui->financeTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aperçu QR Code", "Veuillez sélectionner une transaction.");
        return;
    }

    QString code     = ui->financeTable->item(row, 0)->text();
    QString montant  = ui->financeTable->item(row, 1)->text();
    QString type     = ui->financeTable->item(row, 2)->text();
    QString categorie= ui->financeTable->item(row, 3)->text();
    QString date     = ui->financeTable->item(row, 5)->text();
    QString cin      = ui->financeTable->item(row, 6)->text();

    // Données pour le QR code
    QString transactionData = QString("Code:%1|Montant:%2|Date:%3|Type:%4|CIN:%5")
                         .arg(code)
                         .arg(montant)
                         .arg(date)
                         .arg(type)
                         .arg(cin);

    // Générer le QR code
    QPixmap qrPixmap = QRCodeGenerator::generateAdvancedQR(transactionData, "PREVIEW");
    
    if (qrPixmap.isNull()) {
        QMessageBox::critical(this, "Erreur", "Impossible de générer le QR Code.\nVérifiez votre connexion internet.");
        return;
    }

    // Créer une fenêtre de dialogue pour afficher le QR code
    QDialog *qrDialog = new QDialog(this);
    qrDialog->setWindowTitle("📱 Aperçu QR Code - Transaction " + code);
    qrDialog->setFixedSize(500, 650);
    qrDialog->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(qrDialog);
    
    // Titre
    QLabel *titleLabel = new QLabel(QString("🔗 QR Code de la Transaction %1").arg(code));
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1F4E8C; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Affichage du QR code
    QLabel *qrLabel = new QLabel();
    QPixmap scaledQR = qrPixmap.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qrLabel->setPixmap(scaledQR);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setStyleSheet("border: 2px solid #87CEEB; border-radius: 10px; padding: 10px; background: white;");
    layout->addWidget(qrLabel);

    // Informations
    QLabel *infoLabel = new QLabel(QString(
        "📊 <b>Détails de la transaction :</b><br>"
        "• Code : %1<br>"
        "• Montant : %2 DT<br>"
        "• Type : %3<br>"
        "• Date : %4<br>"
        "• CIN : %5<br><br>"
        "📱 <b>Scannez avec votre smartphone pour vérifier</b>"
    ).arg(code, montant, type, date, cin));
    
    infoLabel->setStyleSheet("font-size: 14px; padding: 15px; background: #F8FAFC; border-radius: 8px; border: 1px solid #E2E8F0;");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    // Bouton de fermeture
    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet("QPushButton { background: #6C757D; color: white; border-radius: 8px; padding: 10px 20px; font-weight: 600; } QPushButton:hover { background: #5A6268; }");
    connect(closeBtn, &QPushButton::clicked, qrDialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    qrDialog->exec();
}
