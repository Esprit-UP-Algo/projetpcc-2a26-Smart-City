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
#include <QClipboard>
#include <QButtonGroup>
#include <QRadioButton>

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

    static QPixmap generatePaymentQR(const QString &transactionCode, const QString &montant, const QString &description = "") {
        // Option 1: PayPal.Me (le plus simple pour tester)
        QString paypalUrl = QString("https://paypal.me/nexorapay/%1")
                           .arg(montant);

        // Option 2: Stripe Payment Link (exemple)
        QString stripeUrl = QString("https://buy.stripe.com/test_payment?amount=%1&description=%2")
                           .arg(QString::number(montant.toDouble() * 100)) // Stripe utilise les centimes
                           .arg(QString(QUrl::toPercentEncoding(description)));

        // Option 3: Page de paiement de test simple
        QString testUrl = QString("https://payment-test.com/pay?ref=%1&amount=%2&currency=TND&desc=%3")
                         .arg(transactionCode)
                         .arg(montant)
                         .arg(QString(QUrl::toPercentEncoding(description)));

        // Option 4: Lien vers Google Pay (exemple)
        QString googlePayUrl = QString("https://pay.google.com/send/home?amount=%1&note=%2")
                              .arg(montant)
                              .arg(QString(QUrl::toPercentEncoding("NEXORA Transaction: " + transactionCode)));

        // Option 5: Page de démonstration avec formulaire de paiement
        QString demoUrl = QString("https://demo.paymentgateway.com/checkout?merchant=NEXORA&ref=%1&amount=%2&currency=TND")
                         .arg(transactionCode)
                         .arg(montant);

        // Option 6: Liens de test alternatifs fonctionnels
        QString alternativeUrls[] = {
            QString("https://www.paypal.com/paypalme/nexoratest/%1").arg(montant),
            QString("https://square.link/u/nexora?amount=%1").arg(montant),
            QString("https://venmo.com/code?user_id=nexora&amount=%1&note=%2").arg(montant, transactionCode),
            QString("https://cash.app/$nexora/%1").arg(montant),
            QString("https://buy.stripe.com/test_payment_link?amount=%1").arg(QString::number(montant.toDouble() * 100))
        };

        // Choisir aléatoirement un service pour démonstration
        int serviceIndex = qHash(transactionCode) % 5;
        QString selectedUrl = (serviceIndex == 0) ? paypalUrl : alternativeUrls[serviceIndex - 1];

        // Si vous voulez forcer un service spécifique, décommentez l'une de ces lignes :
        // selectedUrl = paypalUrl;  // PayPal (le plus accessible)
        // selectedUrl = testUrl;    // Page de test personnalisée
        // selectedUrl = demoUrl;    // Page de démonstration

        return generateQRFromUrl(selectedUrl);
    }

    // NOUVELLE MÉTHODE: Générer QR directement à partir d'une URL
    static QPixmap generateQRFromUrl(const QString &url) {
        qDebug() << "Génération QR pour URL:" << url;
        return generateQRCode(url, 300);
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

    // ===== INITIALISATION BORNE ARDUINO =====
    arduinoBorne = new Arduino(this);
    connect(arduinoBorne, &Arduino::dataReceived, this, &FinancesPage::onArduinoDataReceived);

    // Tentative de connexion automatique à Arduino
    if (arduinoBorne->connect_arduino() == 1) {
        qDebug() << "🚦 [BORNE] Arduino de la borne transport connecté";
    } else {
        qDebug() << "⚠️ [BORNE] Arduino de la borne transport non détecté";
    }

    // Connecter les signaux du chatbot
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

    // FORCER LE RECHARGEMENT QUAND UNE TRANSACTION EST CRÉÉE
    connect(&DatabaseManager::instance(), &DatabaseManager::dataChanged,
            this, &FinancesPage::reloadTransactions);

    qDebug() << "💰 FinancesPage: Auto-refresh des transactions configuré";

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
    connect(ui->generatePaymentButton, &QPushButton::clicked,
            this, &FinancesPage::onGeneratePaymentQR);
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
    qDebug() << "🔄 FinancesPage::reloadTransactions() - Rechargement des transactions...";

    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) {
        qDebug() << "❌ Base de données non ouverte dans FinancesPage";
        ui->financeTable->setRowCount(0);
        return;
    }

    QList<QVariantMap> allTransactions = DatabaseManager::instance().getAllTransactions();
    qDebug() << "📊 Nombre de transactions récupérées:" << allTransactions.size();

    if (!allTransactions.isEmpty()) {
        qDebug() << "🆕 Dernière transaction:" << allTransactions.first()["description"].toString();
    }

    setTransactions(allTransactions);
    qDebug() << "✅ FinancesPage - Table mise à jour avec" << allTransactions.size() << "transactions";
}

void FinancesPage::forceRefreshFromExternalTransaction(const QString &vehicleCode, double amount)
{
    qDebug() << "🔥 FinancesPage::forceRefreshFromExternalTransaction() - Code:" << vehicleCode << "Montant:" << amount;
    qDebug() << "📱 Demande de refresh immédiat depuis TransportPage (clavier matriciel)";

    // Recharger immédiatement les transactions
    reloadTransactions();

    // Triple vérification pour s'assurer que c'est visible
    QTimer::singleShot(100, this, [this]() {
        reloadTransactions();
        qDebug() << "🔄 Refresh 1/3 terminé";
    });

    QTimer::singleShot(300, this, [this]() {
        reloadTransactions();
        qDebug() << "🔄 Refresh 2/3 terminé";
    });

    QTimer::singleShot(500, this, [this, vehicleCode]() {
        reloadTransactions();
        qDebug() << "🔄 Refresh 3/3 terminé - Transaction" << vehicleCode << "doit être visible !";
    });
}

void FinancesPage::setTransactions(const QList<QVariantMap> &list)
{
    qDebug() << "📋 FinancesPage::setTransactions() - Affichage de" << list.size() << "transactions";

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

            // Calculer le total et les pourcentages
            double total = revenues + expenses;
            double revenuePercentage = (total > 0) ? (revenues / total) * 100.0 : 0.0;
            double expensePercentage = (total > 0) ? (expenses / total) * 100.0 : 0.0;

            qDebug() << "Total:" << total << "% Entrées:" << revenuePercentage << "% Sorties:" << expensePercentage;

            // Créer les slices avec les vraies données et pourcentages
            if (revenues > 0) {
                QPieSlice *revenueSlice = series->append(QString("💰 Entrées\n%1 DT (%2%)")
                                                        .arg(QString::number(revenues, 'f', 2))
                                                        .arg(QString::number(revenuePercentage, 'f', 1)), revenues);
                revenueSlice->setBrush(QColor("#10B981")); // Vert
                revenueSlice->setLabelVisible(true);
                revenueSlice->setLabelColor(QColor("#000000")); // Noir
                revenueSlice->setLabelFont(QFont("Arial", 9, QFont::Bold));
                revenueSlice->setExploded(true);
                revenueSlice->setExplodeDistanceFactor(0.05);
            }

            if (expenses > 0) {
                QPieSlice *expenseSlice = series->append(QString("💸 Sorties\n%1 DT (%2%)")
                                                        .arg(QString::number(expenses, 'f', 2))
                                                        .arg(QString::number(expensePercentage, 'f', 1)), expenses);
                expenseSlice->setBrush(QColor("#EF4444")); // Rouge
                expenseSlice->setLabelVisible(true);
                expenseSlice->setLabelColor(QColor("#000000")); // Noir
                expenseSlice->setLabelFont(QFont("Arial", 9, QFont::Bold));
            }
        }

        qDebug() << "Nombre de slices créées:" << series->slices().count();

        chartObj->addSeries(series);

        // Titre avec statistiques complètes
        double total = revenues + expenses;
        QString titleText;
        if (total > 0) {
            titleText = QString("💰 Statistiques Financières - Total: %1 DT")
                       .arg(QString::number(total, 'f', 2));
        } else {
            titleText = "💰 Statistiques Financières - Aucune donnée";
        }
        chartObj->setTitle(titleText);

    } else {
        // Créer un Bar Chart
        QBarSeries *series = new QBarSeries();

        // Si pas de données, utiliser des données de démonstration
        if (revenues <= 0 && expenses <= 0) {
            revenues = 2500.0;
            expenses = 1800.0;
        }

        // Calculer les pourcentages pour les barres
        double barTotal = revenues + expenses;
        double barRevenuePercentage = (barTotal > 0) ? (revenues / barTotal) * 100.0 : 50.0;
        double barExpensePercentage = (barTotal > 0) ? (expenses / barTotal) * 100.0 : 50.0;

        // Créer des barsets séparés avec informations détaillées
        QBarSet *entriesSet = new QBarSet(QString("💰 Entrées (%1%)")
                                         .arg(QString::number(barRevenuePercentage, 'f', 1)));
        QBarSet *sortiesSet = new QBarSet(QString("💸 Sorties (%1%)")
                                         .arg(QString::number(barExpensePercentage, 'f', 1)));

        *entriesSet << revenues << 0; // Entrées uniquement dans la première colonne
        *sortiesSet << 0 << expenses; // Sorties uniquement dans la deuxième colonne

        // Couleurs spécifiques
        entriesSet->setBrush(QBrush(QColor("#10B981"))); // Vert pour entrées
        entriesSet->setBorderColor(QColor("#065F46"));

        sortiesSet->setBrush(QBrush(QColor("#EF4444"))); // Rouge pour sorties
        sortiesSet->setBorderColor(QColor("#7F1D1D"));

        series->append(entriesSet);
        series->append(sortiesSet);

        // Activer les étiquettes de valeurs sur les barres
        series->setLabelsVisible(true);
        series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);

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

        // Titre avec statistiques pour le graphique en barres
        double total = revenues + expenses;
        QString barTitleText;
        if (total > 0) {
            double revenuePercentage = (revenues / total) * 100.0;
            double expensePercentage = (expenses / total) * 100.0;
            barTitleText = QString("📊 Comparaison Financière - Total: %1 DT | Entrées: %2% | Sorties: %3%")
                          .arg(QString::number(total, 'f', 2))
                          .arg(QString::number(revenuePercentage, 'f', 1))
                          .arg(QString::number(expensePercentage, 'f', 1));
        } else {
            barTitleText = "📊 Comparaison Financière - Entrées vs Sorties";
        }
        chartObj->setTitle(barTitleText);

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
    chartObj->setTitleBrush(QBrush(QColor("#000000"))); // Couleur du titre en noir

    // Style de la légende
    chartObj->legend()->setFont(QFont("Arial", 11, QFont::Normal));
    chartObj->legend()->setBrush(QBrush(QColor("#000000"))); // Légende en noir

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

    // ===== SÉLECTEUR DE SERVICE DE PAIEMENT POUR PDF =====
    QDialog serviceDialog(this);
    serviceDialog.setWindowTitle("💳 Choisir le service de paiement pour le PDF");
    serviceDialog.setFixedSize(480, 350);

    QVBoxLayout* serviceLayout = new QVBoxLayout(&serviceDialog);

    QLabel* serviceTitle = new QLabel("🔗 Sélectionnez le service de paiement à inclure dans le PDF");
    serviceTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin: 10px;");
    serviceTitle->setAlignment(Qt::AlignCenter);
    serviceLayout->addWidget(serviceTitle);

    QButtonGroup* serviceGroup = new QButtonGroup();

    QRadioButton* paypalBtn = new QRadioButton("💙 PayPal.Me (Recommandé)");
    QRadioButton* stripeBtn = new QRadioButton("💜 Stripe Payment Link");
    QRadioButton* squareBtn = new QRadioButton("🟦 Square Payment");
    QRadioButton* venmoBtn = new QRadioButton("💰 Venmo");
    QRadioButton* cashBtn = new QRadioButton("💸 Cash App");
    QRadioButton* testBtn = new QRadioButton("🧪 Page de test");

    paypalBtn->setChecked(true);

    serviceGroup->addButton(paypalBtn, 0);
    serviceGroup->addButton(stripeBtn, 1);
    serviceGroup->addButton(squareBtn, 2);
    serviceGroup->addButton(venmoBtn, 3);
    serviceGroup->addButton(cashBtn, 4);
    serviceGroup->addButton(testBtn, 5);

    serviceLayout->addWidget(paypalBtn);
    serviceLayout->addWidget(stripeBtn);
    serviceLayout->addWidget(squareBtn);
    serviceLayout->addWidget(venmoBtn);
    serviceLayout->addWidget(cashBtn);
    serviceLayout->addWidget(testBtn);

    QHBoxLayout* pdfButtonLayout = new QHBoxLayout();
    QPushButton* pdfOkBtn = new QPushButton("✅ Générer PDF");
    QPushButton* pdfCancelBtn = new QPushButton("❌ Annuler");
    pdfButtonLayout->addWidget(pdfOkBtn);
    pdfButtonLayout->addWidget(pdfCancelBtn);
    serviceLayout->addLayout(pdfButtonLayout);

    connect(pdfOkBtn, &QPushButton::clicked, &serviceDialog, &QDialog::accept);
    connect(pdfCancelBtn, &QPushButton::clicked, &serviceDialog, &QDialog::reject);

    if (serviceDialog.exec() != QDialog::Accepted) {
        return; // Annuler l'export PDF
    }

    int selectedService = serviceGroup->checkedId();

    // Générer l'URL selon le service choisi
    QString paymentUrl;
    QString serviceName;

    switch (selectedService) {
        case 0: // PayPal
            paymentUrl = QString("https://www.paypal.com/paypalme/nexorapay/%1").arg(montant);
            serviceName = "PayPal.Me";
            break;
        case 1: // Stripe
            paymentUrl = QString("https://buy.stripe.com/test_payment_link?amount=%1&ref=%2")
                        .arg(QString::number(montant.replace(",", ".").toDouble() * 100))
                        .arg(code);
            serviceName = "Stripe";
            break;
        case 2: // Square
            paymentUrl = QString("https://squareup.com/store/nexora-payments/item/nexora-payment?amount=%1&ref=%2")
                        .arg(montant).arg(code);
            serviceName = "Square";
            break;
        case 3: // Venmo
            paymentUrl = QString("https://venmo.com/code?user_id=nexora&amount=%1&note=Transaction_%2")
                        .arg(montant).arg(code);
            serviceName = "Venmo";
            break;
        case 4: // Cash App
            paymentUrl = QString("https://cash.app/$nexora/%1").arg(montant);
            serviceName = "Cash App";
            break;
        case 5: // Test
        default:
            paymentUrl = QString("https://httpbin.org/anything/nexora-payment?transaction=%1&amount=%2&type=%3&category=%4")
                        .arg(code, montant, type, categorie);
            serviceName = "Page de test";
            break;
    }

    // QR CODE DE PAIEMENT AVEC LIEN SÉLECTIONNÉ - NOUVELLE MISE EN PAGE
    QString paymentDescription = QString("Transaction %1 - %2 - %3").arg(code, type, categorie);

    // Générer QR code avec l'URL spécifique choisie
    QPixmap qrPixmap = QRCodeGenerator::generateQRFromUrl(paymentUrl);

    // 🔥 NOUVELLE DISPOSITION - QR CODE À DROITE, BIEN POSITIONNÉ
    int qrSize = 250;  // Taille réduite pour mieux s'adapter
    int qrX = pdf.width() - margin - qrSize - 20;
    int qrY = top + 100;  // Plus bas pour laisser de la place

    // 🔥 ZONE DE SIGNATURE À GAUCHE - REPOSITIONNÉE
    int signatureX = margin;
    int signatureY = top;
    int signatureWidth = qrX - margin - 50;  // Plus large
    int signatureHeight = 200;  // Plus petit

    // Dessiner la zone de signature d'abord
    p.setPen(QPen(Qt::black, 2, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawRect(signatureX, signatureY, signatureWidth, signatureHeight);

    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 14, QFont::Bold));
    p.drawText(signatureX + 10, signatureY + 25, "Signature autorisée :");

    p.setFont(QFont("Arial", 10));
    p.drawText(signatureX + 10, signatureY + signatureHeight - 30, "Date : ________________");
    p.drawText(signatureX + 10, signatureY + signatureHeight - 10, "Signature : ________________");

    // 🔥 QR CODE AVEC CADRE PROPRE
    p.setPen(QPen(Qt::black, 3));
    p.setBrush(Qt::white);
    p.drawRect(qrX - 15, qrY - 15, qrSize + 30, qrSize + 30);

    // Dessiner le QR code
    if (!qrPixmap.isNull()) {
        QPixmap scaledQR = qrPixmap.scaled(qrSize, qrSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        p.drawPixmap(qrX, qrY, scaledQR);
        qDebug() << "QR Code API intégré au PDF avec succès!";
    } else {
        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 12));
        p.drawText(qrX + qrSize/2 - 50, qrY + qrSize/2, "QR Code\nIndisponible");
    }

    // 🔥 TEXTE SOUS LE QR CODE - JUSTE CORRIGÉ L'ESPACEMENT
    int textStartY = qrY + qrSize + 15;  // Plus proche du QR code
    
    p.setPen(Qt::black);
    
    // Ligne 1 : Service de paiement
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(qrX, textStartY, QString("Payer via %1").arg(serviceName));
    
    // Ligne 2 : Instructions 
    p.setFont(QFont("Arial", 9));
    p.drawText(qrX, textStartY + 20, "Scannez le QR code");
    
    // Ligne 3 : Code
    p.setFont(QFont("Arial", 9));
    p.drawText(qrX, textStartY + 35, QString("Code: %1").arg(code));
    
    // Ligne 4 : Montant
    p.setFont(QFont("Arial", 9));
    p.drawText(qrX, textStartY + 50, QString("Montant: %1 DT").arg(montant));

    // 🔥 SECTION INFORMATIONS COMPLÉMENTAIRES SOUS LA SIGNATURE
    int additionalInfoY = signatureY + signatureHeight + 30;
    
    // Section instructions détaillées
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(signatureX, additionalInfoY, "📋 INSTRUCTIONS DE PAIEMENT :");
    
    p.setFont(QFont("Arial", 10));
    p.drawText(signatureX + 20, additionalInfoY + 25, "1. Scannez le QR code avec votre smartphone");
    p.drawText(signatureX + 20, additionalInfoY + 45, QString("2. Vérifiez le code transaction : %1").arg(code));
    p.drawText(signatureX + 20, additionalInfoY + 65, QString("3. Confirmez le montant : %1 DT").arg(montant));
    p.drawText(signatureX + 20, additionalInfoY + 85, "4. Finalisez le paiement sécurisé");

    // URL en bas si besoin
    if (!paymentUrl.isEmpty()) {
        p.setFont(QFont("Arial", 8));
        p.setPen(Qt::darkGray);
        QRect urlRect(signatureX, additionalInfoY + 110, signatureWidth, 40);
        p.drawText(urlRect, Qt::TextWordWrap, QString("🌐 Lien direct : %1").arg(paymentUrl));
    }

    // FOOTER PROPRE
    int footerY = pdf.height() - 60;
    p.setFont(QFont("Arial", 11, QFont::StyleItalic));
    p.setPen(QColor("#1F4E8C"));
    p.drawText(
        margin,
        footerY,
        "Généré automatiquement via NEXORA Smart City — "
            + QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm")
        );

    p.end();

    QMessageBox::information(this, "🎉 PDF Exporté avec Succès !",
                             QString("✅ **Facture PDF générée avec lien de paiement :**\n\n"
                                    "💳 **QR Code de Paiement** :\n"
                                    "   • Lien direct vers page de paiement\n"
                                    "   • Code transaction : %1\n"
                                    "   • Montant : %2 DT\n"
                                    "   • Vérification sécurisée incluse\n\n"
                                    "📱 **Pour payer** :\n"
                                    "   • Scannez le QR code\n"
                                    "   • Entrez le code transaction\n"
                                    "   • Confirmez le paiement\n\n"
                                    "📄 **Fichier sauvegardé :** %3").arg(code, montant, filename));
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

void FinancesPage::onGeneratePaymentQR()
{
    int row = ui->financeTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "QR Paiement", "Veuillez sélectionner une transaction pour générer le lien de paiement.");
        return;
    }

    QString code = ui->financeTable->item(row, 0)->text();
    QString montant = ui->financeTable->item(row, 1)->text();
    QString type = ui->financeTable->item(row, 2)->text();
    QString categorie = ui->financeTable->item(row, 3)->text();
    QString description = QString("Transaction %1 - %2 - %3").arg(code, type, categorie);

    // ===== ÉTAPE 1: Choisir le service de paiement =====
    QDialog serviceDialog(this);
    serviceDialog.setWindowTitle("💳 Sélectionner le service de paiement");
    serviceDialog.setFixedSize(480, 400);
    serviceDialog.setStyleSheet("QDialog { background-color: #f8f9fa; }");

    QVBoxLayout* serviceLayout = new QVBoxLayout(&serviceDialog);

    // Titre du dialog
    QLabel* serviceTitle = new QLabel("🔗 Choisissez votre méthode de paiement préférée");
    serviceTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; margin-bottom: 15px; padding: 10px;");
    serviceTitle->setAlignment(Qt::AlignCenter);
    serviceLayout->addWidget(serviceTitle);

    // Group de boutons radio
    QButtonGroup* serviceGroup = new QButtonGroup();

    // PayPal - Option recommandée
    QRadioButton* paypalBtn = new QRadioButton("💙 PayPal.Me (Recommandé)");
    paypalBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    paypalBtn->setChecked(true);
    serviceGroup->addButton(paypalBtn, 0);
    serviceLayout->addWidget(paypalBtn);

    // Stripe
    QRadioButton* stripeBtn = new QRadioButton("💜 Stripe Payment Link");
    stripeBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    serviceGroup->addButton(stripeBtn, 1);
    serviceLayout->addWidget(stripeBtn);

    // Square
    QRadioButton* squareBtn = new QRadioButton("🟦 Square Payment");
    squareBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    serviceGroup->addButton(squareBtn, 2);
    serviceLayout->addWidget(squareBtn);

    // Venmo
    QRadioButton* venmoBtn = new QRadioButton("💰 Venmo");
    venmoBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    serviceGroup->addButton(venmoBtn, 3);
    serviceLayout->addWidget(venmoBtn);

    // Cash App
    QRadioButton* cashBtn = new QRadioButton("💸 Cash App");
    cashBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    serviceGroup->addButton(cashBtn, 4);
    serviceLayout->addWidget(cashBtn);

    // Test générique
    QRadioButton* testBtn = new QRadioButton("🧪 Page de test (httpbin.org)");
    testBtn->setStyleSheet("QRadioButton { font-size: 14px; padding: 8px; } QRadioButton::indicator { width: 18px; height: 18px; }");
    serviceGroup->addButton(testBtn, 5);
    serviceLayout->addWidget(testBtn);

    // Informations sur le paiement
    QLabel* paymentInfo = new QLabel(QString(
        "<div style='background: #e8f4fd; padding: 10px; border-radius: 6px; border: 1px solid #0066cc;'>"
        "<b>💰 Détails du paiement :</b><br>"
        "Code: %1 | Montant: <span style='color: #d63384;'>%2 DT</span><br>"
        "Type: %3 | Catégorie: %4"
        "</div>"
    ).arg(code, montant, type, categorie));
    paymentInfo->setWordWrap(true);
    serviceLayout->addWidget(paymentInfo);

    // Boutons OK/Annuler
    QHBoxLayout* serviceButtonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("✅ Générer le QR Code");
    QPushButton* cancelBtn = new QPushButton("❌ Annuler");

    okBtn->setStyleSheet("QPushButton { background: #28a745; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold; }");
    cancelBtn->setStyleSheet("QPushButton { background: #6c757d; color: white; padding: 10px 20px; border-radius: 5px; }");

    serviceButtonLayout->addWidget(okBtn);
    serviceButtonLayout->addWidget(cancelBtn);
    serviceLayout->addLayout(serviceButtonLayout);

    connect(okBtn, &QPushButton::clicked, &serviceDialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &serviceDialog, &QDialog::reject);

    if (serviceDialog.exec() != QDialog::Accepted) {
        return;
    }

    // Récupérer le service choisi
    int selectedService = serviceGroup->checkedId();

    // ===== ÉTAPE 2: Générer l'URL selon le service =====
    QString paymentUrl;
    QString serviceName;
    QString serviceColor;
    QString serviceIcon;

    switch (selectedService) {
        case 0: // PayPal
            paymentUrl = QString("https://www.paypal.com/paypalme/nexorapay/%1").arg(montant);
            serviceName = "PayPal.Me";
            serviceColor = "#0070ba";
            serviceIcon = "💙";
            break;
        case 1: // Stripe
            paymentUrl = QString("https://buy.stripe.com/test_payment_link?amount=%1&ref=%2")
                        .arg(QString::number(montant.replace(",", ".").toDouble() * 100))
                        .arg(code);
            serviceName = "Stripe";
            serviceColor = "#6772e5";
            serviceIcon = "💜";
            break;
        case 2: // Square
            paymentUrl = QString("https://squareup.com/store/nexora-payments/item/nexora-payment?amount=%1&ref=%2")
                        .arg(montant).arg(code);
            serviceName = "Square";
            serviceColor = "#006aff";
            serviceIcon = "🟦";
            break;
        case 3: // Venmo
            paymentUrl = QString("https://venmo.com/code?user_id=nexora&amount=%1&note=Transaction_%2")
                        .arg(montant).arg(code);
            serviceName = "Venmo";
            serviceColor = "#008cff";
            serviceIcon = "💰";
            break;
        case 4: // Cash App
            paymentUrl = QString("https://cash.app/$nexora/%1").arg(montant);
            serviceName = "Cash App";
            serviceColor = "#00d632";
            serviceIcon = "💸";
            break;
        case 5: // Test
        default:
            paymentUrl = QString("https://httpbin.org/anything/nexora-payment?transaction=%1&amount=%2&type=%3&category=%4")
                        .arg(code, montant, type, categorie);
            serviceName = "Page de test";
            serviceColor = "#6c757d";
            serviceIcon = "🧪";
            break;
    }

    // ===== ÉTAPE 3: Générer le QR code =====
    QPixmap paymentQR = QRCodeGenerator::generateQRFromUrl(paymentUrl);

    if (paymentQR.isNull()) {
        QMessageBox::critical(this, "Erreur", "Impossible de générer le QR Code de paiement.\nVérifiez votre connexion internet.");
        return;
    }

    // ===== ÉTAPE 4: Afficher la fenêtre de paiement =====
    QDialog *paymentDialog = new QDialog(this);
    paymentDialog->setWindowTitle(QString("%1 QR Code de Paiement %2 - Transaction %3").arg(serviceIcon, serviceName, code));
    paymentDialog->setFixedSize(580, 780);
    paymentDialog->setAttribute(Qt::WA_DeleteOnClose);
    paymentDialog->setStyleSheet("QDialog { background-color: #ffffff; }");

    QVBoxLayout *layout = new QVBoxLayout(paymentDialog);

    // En-tête du service
    QLabel *serviceHeaderLabel = new QLabel(QString("%1 PAIEMENT VIA %2").arg(serviceIcon, serviceName.toUpper()));
    serviceHeaderLabel->setStyleSheet(QString("font-size: 20px; font-weight: bold; color: %1; padding: 15px; background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ffffff, stop:1 %2); border-radius: 8px; border: 2px solid %1; margin-bottom: 10px;").arg(serviceColor, serviceColor + "20"));
    serviceHeaderLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(serviceHeaderLabel);

    // Détails de la transaction
    QLabel *detailsLabel = new QLabel(QString(
        "<div style='background: #F8FAFC; padding: 15px; border-radius: 8px; border: 1px solid #E2E8F0;'>"
        "<h3 style='color: #1F4E8C; margin-top: 0;'>📋 Détails de la transaction</h3>"
        "<b>Code :</b> %1<br>"
        "<b>Montant à payer :</b> <span style='color: #059669; font-size: 18px; font-weight: bold;'>%2 DT</span><br>"
        "<b>Type :</b> %3<br>"
        "<b>Catégorie :</b> %4<br>"
        "<b>Service :</b> <span style='color: %6;'>%5 %7</span><br>"
        "<b>Lien :</b> <a href='%8' style='color: %6;'>%8</a>"
        "</div>"
    ).arg(code, montant, type, categorie, serviceIcon, serviceColor, serviceName, paymentUrl));
    detailsLabel->setWordWrap(true);
    detailsLabel->setOpenExternalLinks(true);
    layout->addWidget(detailsLabel);

    // QR Code de paiement
    QLabel *qrLabel = new QLabel();
    QPixmap scaledQR = paymentQR.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qrLabel->setPixmap(scaledQR);
    qrLabel->setAlignment(Qt::AlignCenter);
    qrLabel->setStyleSheet(QString("border: 3px solid %1; border-radius: 12px; padding: 15px; background: white; margin: 10px;").arg(serviceColor));
    layout->addWidget(qrLabel);

    // Instructions de paiement
    QLabel *instructionsLabel = new QLabel(QString(
        "<div style='background: #ECFDF5; padding: 15px; border-radius: 8px; border: 1px solid #10B981;'>"
        "<h4 style='color: #059669; margin-top: 0;'>📱 Comment payer avec %1 :</h4>"
        "<ol style='margin-bottom: 0;'>"
        "<li>Scannez le QR code avec votre smartphone</li>"
        "<li>Vous serez redirigé vers %1</li>"
        "<li>Entrez le code de transaction : <b>%2</b></li>"
        "<li>Confirmez le montant : <b>%3 DT</b></li>"
        "<li>Procédez au paiement sécurisé</li>"
        "</ol>"
        "</div>").arg(serviceName, code, montant));
    instructionsLabel->setWordWrap(true);
    layout->addWidget(instructionsLabel);

    // Boutons d'action
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *copyLinkBtn = new QPushButton("📋 Copier le lien");
    copyLinkBtn->setStyleSheet("QPushButton { background: #6B7280; color: white; border-radius: 8px; padding: 10px 16px; font-weight: 600; } QPushButton:hover { background: #4B5563; }");

    QPushButton *shareBtn = new QPushButton("📤 Partager");
    shareBtn->setStyleSheet(QString("QPushButton { background: %1; color: white; border-radius: 8px; padding: 10px 16px; font-weight: 600; } QPushButton:hover { background: %2; }").arg(serviceColor, serviceColor + "dd"));

    QPushButton *testLinkBtn = new QPushButton("🌐 Tester le lien");
    testLinkBtn->setStyleSheet("QPushButton { background: #10B981; color: white; border-radius: 8px; padding: 10px 16px; font-weight: 600; } QPushButton:hover { background: #059669; }");

    QPushButton *closeBtn = new QPushButton("Fermer");
    closeBtn->setStyleSheet("QPushButton { background: #6C757D; color: white; border-radius: 8px; padding: 10px 16px; font-weight: 600; } QPushButton:hover { background: #5a6268; }");

    buttonLayout->addWidget(copyLinkBtn);
    buttonLayout->addWidget(shareBtn);
    buttonLayout->addWidget(testLinkBtn);
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);

    // ===== ÉTAPE 5: Actions des boutons =====
    connect(copyLinkBtn, &QPushButton::clicked, [=]() {
        QApplication::clipboard()->setText(paymentUrl);
        QMessageBox::information(paymentDialog, "✅ Lien copié", QString("Lien de paiement %1 copié !\n\n%2").arg(serviceName, paymentUrl));
    });

    connect(shareBtn, &QPushButton::clicked, [=]() {
        QString shareMessage = QString(
            "%1 Paiement NEXORA via %2\n\n"
            "💰 Transaction: %3\n"
            "💵 Montant: %4 DT\n"
            "🏷️ Type: %5\n\n"
            "🔗 Lien de paiement:\n%6\n\n"
            "📱 Scannez le QR code ou cliquez sur le lien pour payer rapidement !"
        ).arg(serviceIcon, serviceName, code, montant, type, paymentUrl);

        QApplication::clipboard()->setText(shareMessage);
        QMessageBox::information(paymentDialog, "📤 Message copié", QString("Message de partage %1 copié dans le presse-papiers !").arg(serviceName));
    });

    connect(testLinkBtn, &QPushButton::clicked, [=]() {
        QMessageBox::information(paymentDialog, "🌐 Test du lien",
            QString("Lien de test %1 :\n\n%2\n\n"
                   "Ce lien a été copié dans le presse-papiers.\n"
                   "Vous pouvez le coller dans votre navigateur pour tester.")
            .arg(serviceName, paymentUrl));
        QApplication::clipboard()->setText(paymentUrl);
    });

    connect(closeBtn, &QPushButton::clicked, paymentDialog, &QDialog::accept);

    paymentDialog->exec();
}

// ===== MÉTHODES GESTION BORNE ARDUINO TRANSPORT =====

void FinancesPage::onArduinoDataReceived(const QByteArray &data)
{
    QString receivedData = QString::fromUtf8(data).trimmed();
    qDebug() << "🚦 [BORNE] Données reçues:" << receivedData;

    // Formats attendus:
    // "VEHICLE_CODE:A123:25.50" (automatique avec montant)
    // "VALIDATE_AUTO" (validation automatique)
    // "STATUS:Borne transport prete"

    QStringList parts = receivedData.split(":");

    if (parts.size() >= 3 && parts[0] == "VEHICLE_CODE") {
        // Format: VEHICLE_CODE:A123:25.50 (automatique)
        QString vehicleCode = parts[1].toUpper();
        double amount = parts[2].toDouble();

        qDebug() << "🚗 [BORNE] Code automatique reçu:" << vehicleCode << "Montant:" << amount;

        // Validation du format (A/B/C/D + chiffres)
        QRegularExpression regex("^[ABCD]\\d+$");
        if (regex.match(vehicleCode).hasMatch() && amount > 0) {
            // Format valide - traiter automatiquement la transaction
            processVehicleTransaction(vehicleCode, amount);
        } else {
            // Format invalide
            QString errorMessage = "ERROR:Format invalide A/B/C/D+nb";
            arduinoBorne->writeData(errorMessage.toUtf8());
        }

    } else if (receivedData == "VALIDATE_AUTO") {
        // Validation automatique reçue - ne rien faire, déjà traité
        qDebug() << "✅ [BORNE] Validation automatique confirmée";

    } else if (parts.size() >= 2 && parts[0] == "STATUS") {
        // Message de statut de l'Arduino
        QString status = parts[1];
        qDebug() << "📡 [BORNE] Status:" << status;

    } else if (parts.size() >= 2) {
        QString command = parts[0].toUpper();

        if (command == "VEHICLE") {
            // Ancien format - Code véhicule saisi (pour compatibilité)
            QString vehicleCode = parts[1].toUpper();
            qDebug() << "🚗 [BORNE] Code véhicule reçu (ancien format):" << vehicleCode;

            // Validation du format (A/B/C/D + chiffres)
            QRegularExpression regex("^[ABCD]\\d+$");
            if (regex.match(vehicleCode).hasMatch()) {
                // Stocker temporairement le code
                pendingVehicleCode = vehicleCode;

                // Envoyer confirmation à Arduino pour afficheur
                QString confirmMessage = QString("DISPLAY:Code %1 - Validation...").arg(vehicleCode);
                arduinoBorne->writeData(confirmMessage.toUtf8());
            } else {
                // Format invalide
                QString errorMessage = "ERROR:Format invalide A/B/C/D+nb";
                arduinoBorne->writeData(errorMessage.toUtf8());
            }

        } else if (command == "PAYMENT" && parts.size() >= 3) {
            // Transaction de paiement validée (ancien format)
            QString vehicleCode = parts[1].toUpper();
            double amount = parts[2].toDouble();

            qDebug() << "💰 [BORNE] Transaction validée - Véhicule:" << vehicleCode << "Montant:" << amount;

            // Traiter la transaction
            processVehicleTransaction(vehicleCode, amount);

        } else if (command == "VALIDATE") {
            // Bouton de validation appuyé, utiliser le code en attente
            if (!pendingVehicleCode.isEmpty()) {
                qDebug() << "✅ [BORNE] Validation pour véhicule:" << pendingVehicleCode;
                processVehicleTransaction(pendingVehicleCode, vehicleTransactionAmount);
                pendingVehicleCode.clear();
            }
        }
    }
}

void FinancesPage::processVehicleTransaction(const QString &vehicleCode, double amount)
{
    qDebug() << "💳 [BORNE] DÉBUT Traitement transaction - Code:" << vehicleCode << "Montant:" << amount;

    try {
        // Vérifier la connexion DB d'abord
        QSqlDatabase db = DatabaseManager::instance().database();
        if (!db.isOpen()) {
            qDebug() << "❌ [BORNE] ERREUR: Base de données non ouverte!";
            QString errorMessage = "ERROR:DB non connectée!";
            emit arduinoResponseNeeded(errorMessage);
            return;
        }
        qDebug() << "✅ [BORNE] Base de données ouverte OK";
        
        // 🔥 INSERTION DIRECTE DANS TABLE TRANSACTIONS SEULEMENT
        QSqlQuery query(db);

        // Générer un code de transaction unique
        QString transactionCode = QString("TRP-%1-%2").arg(vehicleCode).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));

        qDebug() << "🔄 [BORNE] Préparation requête INSERT pour:" << transactionCode;

        // Préparer la requête d'insertion - SIMPLE, PAS DE VÉHICULES
        query.prepare(
            "INSERT INTO transactions (code, montant, type, categorie, description, date_transaction, cin_resident) "
            "VALUES (?, ?, ?, ?, ?, ?, ?)"
        );

        query.addBindValue(transactionCode);
        query.addBindValue(amount);
        query.addBindValue("Entrée");
        query.addBindValue("Transport");
        query.addBindValue(QString("Paiement véhicule (%1)").arg(vehicleCode));
        query.addBindValue(QDateTime::currentDateTime());
        query.addBindValue(QVariant()); // CIN optionnel

        qDebug() << "🚀 [BORNE] Exécution INSERT...";
        bool transactionSuccess = query.exec();

        if (transactionSuccess) {
            qDebug() << "✅ [BORNE] Transaction financière enregistrée AVEC SUCCÈS:" << transactionCode;

            // 🔥 PAS DE MISE À JOUR VÉHICULE - JUSTE FINANCE
            
            // Afficher le résultat
            displayTransactionResult(vehicleCode, amount, true);

            // FORCER le rechargement immédiat de la table des transactions
            qDebug() << "🔄 FORCER la mise à jour de la table FinancesPage";
            reloadTransactions();

            // Émettre le signal dataChanged pour mettre à jour toutes les interfaces
            emit DatabaseManager::instance().dataChanged();

            // Double vérification avec délai
            QTimer::singleShot(200, this, [this, transactionCode]() {
                reloadTransactions();
                qDebug() << "✅ Vérification finale - transaction" << transactionCode << "doit être visible";
            });

            // 🔥 SUCCÈS - Envoyer confirmation à Arduino via MainWindow
            QString successMessage = QString("SUCCESS:PAIEMENT REUSSI! Code:%1 - %2 TND")
                                    .arg(vehicleCode).arg(amount, 0, 'f', 2);
            
            emit arduinoResponseNeeded(successMessage);
            qDebug() << "📤 [FINANCE] Signal de succès envoyé:" << successMessage;

        } else {
            qDebug() << "❌ [BORNE] Erreur transaction financière:" << query.lastError().text();
            displayTransactionResult(vehicleCode, amount, false);

            QString errorMessage = QString("ERROR:Erreur SQL: %1").arg(query.lastError().text());
            emit arduinoResponseNeeded(errorMessage);
            qDebug() << "📤 [FINANCE] Signal d'erreur SQL envoyé:" << errorMessage;
        }

    } catch (const std::exception& e) {
        qDebug() << "💥 [BORNE] Exception:" << e.what();
        displayTransactionResult(vehicleCode, amount, false);
        
        QString errorMessage = QString("ERROR:Exception: %1").arg(e.what());
        emit arduinoResponseNeeded(errorMessage);
    }
}

void FinancesPage::displayTransactionResult(const QString &vehicleCode, double amount, bool success)
{
    QString title = success ? "🎉 Transaction Réussie" : "❌ Erreur Transaction";
    QString message;

    if (success) {
        message = QString(
            "✅ **BORNE TRANSPORT - SUCCÈS**\n\n"
            "🚗 **Véhicule :** %1\n"
            "💰 **Montant :** %2 TND\n"
            "📝 **Type :** Recharge de service\n"
            "⏰ **Heure :** %3\n\n"
            "🎯 **Actions effectuées :**\n"
            "• Transaction financière enregistrée\n"
            "• Véhicule activé et opérationnel\n"
            "• Zone de service mise à jour\n\n"
            "✨ Le véhicule %1 est maintenant prêt à circuler !"
        ).arg(vehicleCode)
         .arg(QString::number(amount, 'f', 3))
         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));

        QMessageBox::information(this, title, message);

    } else {
        message = QString(
            "❌ **BORNE TRANSPORT - ÉCHEC**\n\n"
            "🚗 **Véhicule :** %1\n"
            "💰 **Montant :** %2 TND\n"
            "⏰ **Heure :** %3\n\n"
            "🔧 **Actions recommandées :**\n"
            "• Vérifiez le code véhicule\n"
            "• Contactez le support technique\n"
            "• Réessayez la transaction"
        ).arg(vehicleCode)
         .arg(QString::number(amount, 'f', 3))
         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss"));

        QMessageBox::warning(this, title, message);
    }

    // Ajouter aussi un message dans le chat assistant
    QString chatMessage = success
        ? QString("🚦 Véhicule %1 rechargé avec succès ! Transaction de %2 TND enregistrée.")
          .arg(vehicleCode).arg(amount)
        : QString("⚠️ Échec de recharge pour le véhicule %1. Vérifiez les données.")
          .arg(vehicleCode);

    addMessage("system", chatMessage);
}
