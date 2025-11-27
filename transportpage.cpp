#include "transportpage.h"
#include "ui_transportpage.h"
#include "databasemanager.h"
#include "thememanager.h"
#include "languagemanager.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QSqlError>
#include <QTextDocument>
#include <QDateTime>
#include <QMap>
#include <limits>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QRegularExpressionValidator>
#include <QDir>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QStandardPaths>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
TransportPage::TransportPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportPage)
    , m_trayIcon(nullptr)
    , m_maintenanceCheckTimer(nullptr)
{
    ui->setupUi(this);

    if (!DatabaseManager::instance().database().isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée !");
    } else {
        qDebug() << "✅ Base connectée dans TransportPage";
    }

    this->setAttribute(Qt::WA_StyledBackground, true);

    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);

    setupTableHeaders();
    setupUiBehavior();
    setupValidation();
    setupNotificationSystem();
    initialiserFichierHistorique();

    // SUPPRIMEZ cet appel car vous n'avez plus de QComboBox
    // chargerCodesUniquesFinance();

    connect(&DatabaseManager::instance(), &DatabaseManager::dataChanged,
            this, &TransportPage::onDataChanged);

    QTimer::singleShot(100, this, &TransportPage::loadAllVehicules);

    qDebug() << "✅ TransportPage initialisé avec gestion d'historique et notifications";

    // Hook theme/language
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this, [this](AppTheme){ applyTheme(); });
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, [this](const QString&){ reloadTranslations(); });
    applyTheme();
    reloadTranslations();
}
TransportPage::~TransportPage()
{
    delete ui;
}

void TransportPage::applyTheme()
{
    ThemeManager::instance().applyTheme(this);
}

void TransportPage::reloadTranslations()
{
    if (ui) ui->retranslateUi(this);
}

// === SYSTÈME DE NOTIFICATIONS AVANCÉ ===

void TransportPage::setupNotificationSystem()
{
    setupTrayIcon();

    m_maintenanceCheckTimer = new QTimer(this);
    m_maintenanceCheckTimer->setInterval(30000);
    connect(m_maintenanceCheckTimer, &QTimer::timeout, this, &TransportPage::checkMaintenanceVehicles);
    m_maintenanceCheckTimer->start();

    m_knownMaintenanceVehicles.clear();

    qDebug() << "🔔 Système de notifications initialisé";
}

void TransportPage::setupTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qDebug() << "❌ Systray non disponible sur ce système";
        return;
    }

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QApplication::windowIcon());
    m_trayIcon->setToolTip("NEXORA Transport - Gestion des Véhicules");

    QMenu *trayMenu = new QMenu(this);

    QAction *showAction = new QAction("📋 Afficher l'application", this);
    QAction *quitAction = new QAction("❌ Quitter", this); // SUPPRIMEZ maintenanceAction

    connect(showAction, &QAction::triggered, this, [this]() {
        if (this->parentWidget()) {
            this->parentWidget()->show();
            this->parentWidget()->activateWindow();
        }
    });

    // SUPPRIMEZ CETTE LIGNE :
    // connect(maintenanceAction, &QAction::triggered, this, &TransportPage::afficherHistoriqueMaintenance);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    // SUPPRIMEZ CETTE LIGNE :
    // trayMenu->addAction(maintenanceAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TransportPage::onTrayIconActivated);

    m_trayIcon->show();
    qDebug() << "📌 Icône de notification installée dans la barre système";
}

void TransportPage::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        if (this->parentWidget()) {
            this->parentWidget()->show();
            this->parentWidget()->activateWindow();
        }
        break;
    case QSystemTrayIcon::Trigger:
        if (m_trayIcon) {
            m_trayIcon->showMessage(
                "NEXORA Transport",
                QString("%1 véhicule(s) en maintenance").arg(m_knownMaintenanceVehicles.size()),
                QSystemTrayIcon::Information,
                2000
                );
        }
        break;
    default:
        break;
    }
}

void TransportPage::checkMaintenanceVehicles()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();
    int newMaintenanceCount = 0;

    for (const auto &vehicule : vehicules) {
        QString statut = vehicule.value("statut").toString().toLower();
        QString vehicleId = vehicule.value("id_vehicule").toString();

        if (statut.contains("maintenance")) {
            if (!m_knownMaintenanceVehicles.contains(vehicleId)) {
                // Nouveau véhicule en maintenance détecté
                m_knownMaintenanceVehicles.insert(vehicleId);
                showMaintenanceNotification(vehicleId);
                newMaintenanceCount++;
                logMaintenanceEvent(vehicleId);
                qDebug() << "🔄 Timer: Nouveau véhicule en maintenance:" << vehicleId;
            }
        } else {
            // Véhicule n'est plus en maintenance
            m_knownMaintenanceVehicles.remove(vehicleId);
        }
    }

    // Mettre à jour le tooltip du tray icon
    if (m_trayIcon) {
        m_trayIcon->setToolTip(QString("NEXORA Transport - %1 véhicule(s) en maintenance")
                                   .arg(m_knownMaintenanceVehicles.size()));
    }

    if (newMaintenanceCount > 0) {
        qDebug() << "🔔" << newMaintenanceCount << "nouveau(x) véhicule(s) en maintenance détecté(s)";
    }
}
void TransportPage::showMaintenanceNotification(const QString &vehicleId)
{
    QString title = "🚨 Véhicule en Maintenance";
    QString message = QString("Le véhicule %1 nécessite une intervention technique.\nVeuillez vérifier son statut dans l'application.")
                          .arg(vehicleId);

    if (m_trayIcon) {
        m_trayIcon->showMessage(title, message, QSystemTrayIcon::Critical, 10000);
    }

    playNotificationSound();
    showPopupNotification(title, message);

    qDebug() << "🚨 Notification maintenance:" << vehicleId;
}

void TransportPage::playNotificationSound()
{
    // Son système simple sans QSoundEffect
    QApplication::beep();
}

void TransportPage::showPopupNotification(const QString &title, const QString &message)
{
    QDialog *notificationDialog = new QDialog(this);
    notificationDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    notificationDialog->setFixedSize(350, 150);
    notificationDialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #FF6B6B, stop:1 #FF8E8E);
            border: 2px solid #FF4757;
            border-radius: 15px;
        }
        QLabel {
            color: white;
            font-weight: bold;
        }
        QPushButton {
            background: white;
            border: none;
            border-radius: 10px;
            padding: 8px 15px;
            font-weight: bold;
            color: #FF4757;
        }
        QPushButton:hover {
            background: #F1F2F6;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(notificationDialog);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 16px; padding: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QLabel *messageLabel = new QLabel(message);
    messageLabel->setStyleSheet("font-size: 12px; padding: 5px;");
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);
    layout->addWidget(messageLabel);

    QPushButton *closeButton = new QPushButton("✓ Compris");
    connect(closeButton, &QPushButton::clicked, notificationDialog, &QDialog::close);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    QPoint position = this->mapToGlobal(QPoint(this->width() - 370, this->height() - 170));
    notificationDialog->move(position);

    QTimer::singleShot(10000, notificationDialog, &QDialog::close);

    notificationDialog->show();
}

void TransportPage::logMaintenanceEvent(const QString &vehicleId)
{
    QString logFile = QApplication::applicationDirPath() + "/data/maintenance_log.json";
    QDir().mkpath(QFileInfo(logFile).path());

    QFile file(logFile);
    QJsonArray logArray;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isNull()) {
            logArray = doc.array();
        }
        file.close();
    }

    QJsonObject logEntry;
    logEntry["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    logEntry["vehicle_id"] = vehicleId;
    logEntry["type"] = "MAINTENANCE_DETECTED";

    logArray.prepend(logEntry);

    while (logArray.size() > 100) {
        logArray.removeLast();
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(logArray);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}
bool TransportPage::isNewMaintenanceEvent(const QString &vehicleId)
{
    // Vérifier si le véhicule n'était pas déjà connu comme étant en maintenance
    bool isNew = !m_knownMaintenanceVehicles.contains(vehicleId);

    if (isNew) {
        qDebug() << "🆕 Nouveau véhicule en maintenance détecté:" << vehicleId;
    } else {
        qDebug() << "ℹ Véhicule déjà connu en maintenance:" << vehicleId;
    }

    return isNew;
}



// === MÉTHODES POUR L'HISTORIQUE ===

QString TransportPage::getFichierHistorique() const
{
    QString chemin = QApplication::applicationDirPath() + "/data";
    QDir repertoire(chemin);

    if (!repertoire.exists()) {
        repertoire.mkpath(".");
        qDebug() << "📁 Répertoire data créé:" << chemin;
    }

    return chemin + "/historique_vehicules.json";
}

void TransportPage::initialiserFichierHistorique()
{
    QString cheminFichier = getFichierHistorique();
    QFile fichier(cheminFichier);

    if (!fichier.exists()) {
        qDebug() << "🆕 Création du fichier d'historique:" << cheminFichier;

        QJsonArray historiqueArray;
        QJsonDocument doc(historiqueArray);

        if (fichier.open(QIODevice::WriteOnly)) {
            fichier.write(doc.toJson(QJsonDocument::Indented));
            fichier.close();
            qDebug() << "✅ Fichier d'historique créé avec succès";
        } else {
            qDebug() << "❌ Erreur création fichier historique:" << fichier.errorString();
        }
    } else {
        qDebug() << "📁 Fichier d'historique existe déjà:" << cheminFichier;
    }
}
void TransportPage::genererHistoriqueHTML(const QList<QVariantMap> &historique)
{
    // Chemin vers le dossier Historique sur le Bureau
    QString bureauPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString historiqueDir = bureauPath + "/Historique";

    // Créer le dossier s'il n'existe pas
    QDir dir(historiqueDir);
    if (!dir.exists()) {
        dir.mkpath(".");
        qDebug() << "📁 Dossier Historique créé:" << historiqueDir;
    }

    // Chemin du fichier HTML
    QString htmlFilePath = historiqueDir + "/historique_vehicules.html";
    QFile htmlFile(htmlFilePath);

    if (!htmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "❌ Erreur création fichier HTML:" << htmlFile.errorString();
        return;
    }

    QTextStream out(&htmlFile);
    out.setEncoding(QStringConverter::Utf8);

    // Générer le contenu HTML avec un style professionnel
    out << "<!DOCTYPE html>\n";
    out << "<html lang='fr'>\n";
    out << "<head>\n";
    out << "    <meta charset='UTF-8'>\n";
    out << "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    out << "    <title>Historique des Véhicules - NEXORA TRANSPORT</title>\n";
    out << "    <style>\n";
    out << "        * {\n";
    out << "            margin: 0;\n";
    out << "            padding: 0;\n";
    out << "            box-sizing: border-box;\n";
    out << "        }\n";
    out << "        body {\n";
    out << "            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;\n";
    out << "            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);\n";
    out << "            min-height: 100vh;\n";
    out << "            padding: 20px;\n";
    out << "        }\n";
    out << "        .container {\n";
    out << "            max-width: 1400px;\n";
    out << "            margin: 0 auto;\n";
    out << "            background: white;\n";
    out << "            border-radius: 15px;\n";
    out << "            box-shadow: 0 20px 40px rgba(0,0,0,0.1);\n";
    out << "            overflow: hidden;\n";
    out << "        }\n";
    out << "        .header {\n";
    out << "            background: linear-gradient(135deg, #1F2D5C 0%, #2D3748 100%);\n";
    out << "            color: white;\n";
    out << "            padding: 30px;\n";
    out << "            text-align: center;\n";
    out << "        }\n";
    out << "        .header h1 {\n";
    out << "            font-size: 2.5em;\n";
    out << "            margin-bottom: 10px;\n";
    out << "            font-weight: 700;\n";
    out << "        }\n";
    out << "        .header p {\n";
    out << "            font-size: 1.2em;\n";
    out << "            opacity: 0.9;\n";
    out << "        }\n";
    out << "        .summary {\n";
    out << "            background: #F7FAFC;\n";
    out << "            padding: 20px;\n";
    out << "            border-bottom: 1px solid #E2E8F0;\n";
    out << "        }\n";
    out << "        .stats-grid {\n";
    out << "            display: grid;\n";
    out << "            grid-template-columns: repeat(4, 1fr);\n";
    out << "            gap: 15px;\n";
    out << "            margin-bottom: 20px;\n";
    out << "        }\n";
    out << "        .stat-card {\n";
    out << "            background: white;\n";
    out << "            padding: 20px;\n";
    out << "            border-radius: 10px;\n";
    out << "            text-align: center;\n";
    out << "            box-shadow: 0 4px 6px rgba(0,0,0,0.05);\n";
    out << "            border-left: 4px solid #4299E1;\n";
    out << "        }\n";
    out << "        .stat-card.ajout { border-left-color: #48BB78; }\n";
    out << "        .stat-card.modif { border-left-color: #ED8936; }\n";
    out << "        .stat-card.supp { border-left-color: #F56565; }\n";
    out << "        .stat-number {\n";
    out << "            font-size: 2em;\n";
    out << "            font-weight: bold;\n";
    out << "            color: #2D3748;\n";
    out << "        }\n";
    out << "        .stat-label {\n";
    out << "            color: #718096;\n";
    out << "            font-size: 0.9em;\n";
    out << "            margin-top: 5px;\n";
    out << "        }\n";
    out << "        .table-container {\n";
    out << "            padding: 20px;\n";
    out << "            overflow-x: auto;\n";
    out << "        }\n";
    out << "        table {\n";
    out << "            width: 100%;\n";
    out << "            border-collapse: collapse;\n";
    out << "            background: white;\n";
    out << "            border-radius: 10px;\n";
    out << "            overflow: hidden;\n";
    out << "            box-shadow: 0 4px 6px rgba(0,0,0,0.05);\n";
    out << "        }\n";
    out << "        th {\n";
    out << "            background: linear-gradient(135deg, #4299E1 0%, #3182CE 100%);\n";
    out << "            color: white;\n";
    out << "            padding: 15px 12px;\n";
    out << "            text-align: left;\n";
    out << "            font-weight: 600;\n";
    out << "            font-size: 0.9em;\n";
    out << "            text-transform: uppercase;\n";
    out << "            letter-spacing: 0.5px;\n";
    out << "            border: none;\n";
    out << "        }\n";
    out << "        td {\n";
    out << "            padding: 12px;\n";
    out << "            border-bottom: 1px solid #E2E8F0;\n";
    out << "            font-size: 0.9em;\n";
    out << "            color: #2D3748;\n";
    out << "            border: 1px solid #E2E8F0;\n";
    out << "        }\n";
    out << "        tr:nth-child(even) {\n";
    out << "            background: #F7FAFC;\n";
    out << "        }\n";
    out << "        tr:hover {\n";
    out << "            background: #EDF2F7;\n";
    out << "            transform: translateY(-1px);\n";
    out << "            transition: all 0.2s;\n";
    out << "        }\n";
    out << "        .action-ajout { background-color: #F0FFF4 !important; color: #22543D; font-weight: bold; }\n";
    out << "        .action-modif { background-color: #FFFAF0 !important; color: #744210; font-weight: bold; }\n";
    out << "        .action-supp { background-color: #FFF5F5 !important; color: #742A2A; font-weight: bold; }\n";
    out << "        .footer {\n";
    out << "            background: #2D3748;\n";
    out << "            color: white;\n";
    out << "            text-align: center;\n";
    out << "            padding: 20px;\n";
    out << "            margin-top: 20px;\n";
    out << "        }\n";
    out << "        .timestamp {\n";
    out << "            font-size: 0.8em;\n";
    out << "            color: #A0AEC0;\n";
    out << "            margin-top: 5px;\n";
    out << "        }\n";
    out << "        .badge {\n";
    out << "            padding: 4px 8px;\n";
    out << "            border-radius: 12px;\n";
    out << "            font-size: 0.8em;\n";
    out << "            font-weight: bold;\n";
    out << "        }\n";
    out << "        .badge-ajout { background: #C6F6D5; color: #22543D; }\n";
    out << "        .badge-modif { background: #FEEBC8; color: #744210; }\n";
    out << "        .badge-supp { background: #FED7D7; color: #742A2A; }\n";
    out << "    </style>\n";
    out << "</head>\n";
    out << "<body>\n";
    out << "    <div class='container'>\n";
    out << "        <div class='header'>\n";
    out << "            <h1>🚗 NEXORA TRANSPORT</h1>\n";
    out << "            <p>Historique Complet des Véhicules</p>\n";
    out << "            <div class='timestamp'>\n";
    out << "                Généré le " << QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm:ss") << "\n";
    out << "            </div>\n";
    out << "        </div>\n";

    // Calcul des statistiques
    int ajouts = 0, modifications = 0, suppressions = 0;
    for (const auto &v : historique) {
        QString action = v.value("historique_action").toString();
        if (action == "AJOUT") ajouts++;
        else if (action == "MODIFICATION") modifications++;
        else if (action == "SUPPRESSION") suppressions++;
    }

    out << "        <div class='summary'>\n";
    out << "            <div class='stats-grid'>\n";
    out << "                <div class='stat-card'>\n";
    out << "                    <div class='stat-number'>" << historique.size() << "</div>\n";
    out << "                    <div class='stat-label'>Total des Opérations</div>\n";
    out << "                </div>\n";
    out << "                <div class='stat-card ajout'>\n";
    out << "                    <div class='stat-number'>" << ajouts << "</div>\n";
    out << "                    <div class='stat-label'>Ajouts</div>\n";
    out << "                </div>\n";
    out << "                <div class='stat-card modif'>\n";
    out << "                    <div class='stat-number'>" << modifications << "</div>\n";
    out << "                    <div class='stat-label'>Modifications</div>\n";
    out << "                </div>\n";
    out << "                <div class='stat-card supp'>\n";
    out << "                    <div class='stat-number'>" << suppressions << "</div>\n";
    out << "                    <div class='stat-label'>Suppressions</div>\n";
    out << "                </div>\n";
    out << "            </div>\n";
    out << "        </div>\n";

    out << "        <div class='table-container'>\n";
    out << "            <table>\n";
    out << "                <thead>\n";
    out << "                    <tr>\n";
    out << "                        <th>Action</th>\n";
    out << "                        <th>Date/Heure</th>\n";
    out << "                        <th>ID Véhicule</th>\n";
    out << "                        <th>Type</th>\n";
    out << "                        <th>Capacité</th>\n";
    out << "                        <th>Zone</th>\n";
    out << "                        <th>Horaire</th>\n";
    out << "                        <th>Statut</th>\n";
    out << "                        <th>Date Ajout</th>\n";
    out << "                        <th>Temps Utilisé (h)</th>\n";
    out << "                        <th>Kilométrage (km)</th>\n";
    out << "                        <th>Code Unique</th>\n";
    out << "                    </tr>\n";
    out << "                </thead>\n";
    out << "                <tbody>\n";

    // Remplissage du tableau avec les données
    for (int i = 0; i < historique.size(); ++i) {
        const auto &v = historique[i];
        QString type = v.value("type").toString();
        double tempsUtilise = v.value("temps_utilise").toDouble();
        double kilometrage = calculerKilometrage(type, tempsUtilise);
        QString action = v.value("historique_action").toString();
        QString timestamp = v.value("historique_timestamp").toString();

        QString actionClass;
        QString badgeClass;
        if (action == "AJOUT") {
            actionClass = "action-ajout";
            badgeClass = "badge-ajout";
        } else if (action == "MODIFICATION") {
            actionClass = "action-modif";
            badgeClass = "badge-modif";
        } else if (action == "SUPPRESSION") {
            actionClass = "action-supp";
            badgeClass = "badge-supp";
        }

        out << "                    <tr class='" << actionClass << "'>\n";
        out << "                        <td><span class='badge " << badgeClass << "'>" << action << "</span></td>\n";
        out << "                        <td>" << timestamp << "</td>\n";
        out << "                        <td><strong>" << v.value("id_vehicule").toString() << "</strong></td>\n";
        out << "                        <td>" << type << "</td>\n";
        out << "                        <td>" << v.value("capacite").toString() << "</td>\n";
        out << "                        <td>" << v.value("zone").toString() << "</td>\n";
        out << "                        <td>" << v.value("horaire").toString() << "</td>\n";
        out << "                        <td>" << v.value("statut").toString() << "</td>\n";
        out << "                        <td>" << v.value("date_ajout").toString() << "</td>\n";
        out << "                        <td>" << v.value("temps_utilise").toString() << "</td>\n";
        out << "                        <td>" << QString::number(kilometrage, 'f', 1) << "</td>\n";
        out << "                        <td><code>" << v.value("code_unique").toString() << "</code></td>\n";
        out << "                    </tr>\n";
    }

    out << "                </tbody>\n";
    out << "            </table>\n";
    out << "        </div>\n";
    out << "        <div class='footer'>\n";
    out << "            <p>NEXORA Smart City Management System - Tous droits réservés © " << QDate::currentDate().year() << "</p>\n";
    out << "            <p>Système de gestion des véhicules de transport</p>\n";
    out << "        </div>\n";
    out << "    </div>\n";
    out << "</body>\n";
    out << "</html>\n";

    htmlFile.close();

    qDebug() << "✅ Fichier HTML généré avec succès:" << htmlFilePath;
    qDebug() << "📊 Statistiques - Total:" << historique.size()
             << "| Ajouts:" << ajouts
             << "| Modifications:" << modifications
             << "| Suppressions:" << suppressions;
}
void TransportPage::sauvegarderHistorique(const QVariantMap &vehicule, const QString &action)
{
    QString cheminFichier = getFichierHistorique();
    QFile fichier(cheminFichier);
    QJsonArray historiqueArray;

    qDebug() << "💾 Sauvegarde historique vers:" << cheminFichier;

    // 1. SAUVEGARDE DANS LE FICHIER JSON (existant)
    if (fichier.exists()) {
        if (fichier.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(fichier.readAll());
            if (!doc.isNull()) {
                historiqueArray = doc.array();
            }
            fichier.close();
        } else {
            qDebug() << "❌ Impossible d'ouvrir le fichier historique en lecture";
            return;
        }
    }

    QJsonObject entree;
    entree["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    entree["action"] = action;

    QJsonObject dataVehicule;
    for (auto it = vehicule.begin(); it != vehicule.end(); ++it) {
        dataVehicule[it.key()] = QJsonValue::fromVariant(it.value());
    }
    entree["vehicule"] = dataVehicule;

    historiqueArray.prepend(entree);

    while (historiqueArray.size() > 1000) {
        historiqueArray.removeLast();
    }

    if (fichier.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(historiqueArray);
        qint64 bytesEcris = fichier.write(doc.toJson(QJsonDocument::Indented));
        fichier.close();

        if (bytesEcris > 0) {
            qDebug() << "✅ Historique JSON sauvegardé - Action:" << action
                     << "Véhicule:" << vehicule["id_vehicule"].toString();

            // 2. METTRE À JOUR LE FICHIER HTML EXISTANT SUR LE BUREAU
            QList<QVariantMap> historiqueComplet = getHistoriqueComplet();
            if (!historiqueComplet.isEmpty()) {
                genererHistoriqueHTML(historiqueComplet);
                qDebug() << "✅ Historique HTML mis à jour sur le Bureau";
            }
        } else {
            qDebug() << "❌ Erreur écriture historique JSON";
        }
    } else {
        qDebug() << "❌ Impossible d'ouvrir le fichier historique en écriture:"
                 << fichier.errorString();
    }
}
void TransportPage::mettreAJourHistoriqueHTML()
{
    QList<QVariantMap> historiqueComplet = getHistoriqueComplet();
    if (!historiqueComplet.isEmpty()) {
        genererHistoriqueHTML(historiqueComplet);
        qDebug() << "✅ Historique HTML mis à jour sur le Bureau -"
                 << historiqueComplet.size() << "entrées";
    }
}
QList<QVariantMap> TransportPage::getHistoriqueComplet()
{
    QList<QVariantMap> historique;
    QString cheminFichier = getFichierHistorique();
    QFile fichier(cheminFichier);

    if (!fichier.exists()) {
        qDebug() << "ℹ Aucun fichier d'historique trouvé";
        return historique;
    }

    if (fichier.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(fichier.readAll());
        if (doc.isNull()) {
            qDebug() << "❌ Erreur lecture JSON historique";
            fichier.close();
            return historique;
        }

        QJsonArray historiqueArray = doc.array();
        qDebug() << "📖 Historique chargé:" << historiqueArray.size() << "entrées";

        for (const QJsonValue &value : historiqueArray) {
            QJsonObject entree = value.toObject();
            QJsonObject dataVehicule = entree["vehicule"].toObject();

            QVariantMap vehicule;
            for (auto it = dataVehicule.begin(); it != dataVehicule.end(); ++it) {
                vehicule[it.key()] = it.value().toVariant();
            }

            vehicule["historique_action"] = entree["action"].toString();
            vehicule["historique_timestamp"] = entree["timestamp"].toString();

            historique.append(vehicule);
        }
        fichier.close();
    } else {
        qDebug() << "❌ Impossible d'ouvrir le fichier historique en lecture";
    }

    return historique;
}

// === SLOTS ===

void TransportPage::onHistoriqueClicked()
{
    // Récupérer l'historique
    QList<QVariantMap> historique = getHistoriqueComplet();

    if (historique.isEmpty()) {
        QMessageBox::information(this, "Historique", "Aucun historique disponible.");
        return;
    }

    // Générer le fichier HTML
    genererHistoriqueHTML(historique);

    // Chemin vers le fichier HTML généré
    QString bureauPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString htmlFilePath = bureauPath + "/Historique/historique_vehicules.html";

    // Ouvrir directement le fichier HTML
    QDesktopServices::openUrl(QUrl::fromLocalFile(htmlFilePath));
}

void TransportPage::onTabStats()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(true);
    ui->stackedWidget->setCurrentWidget(ui->statsPage);
    calculateAndDisplayStatistics();
}
// === MÉTHODES MANQUANTES ===

void TransportPage::onTabAdd()
{
    ui->tabAddButton->setChecked(true);
    ui->tabListButton->setChecked(false);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->addPage);
}

void TransportPage::onTabList()
{
    ui->tabAddButton->setChecked(false);
    ui->tabListButton->setChecked(true);
    ui->tabStatsButton->setChecked(false);
    ui->stackedWidget->setCurrentWidget(ui->listPage);
    loadAllVehicules();
}

void TransportPage::onDataChanged()
{
    qDebug() << "📢 Signal dataChanged reçu - Rechargement automatique";
    loadAllVehicules();
}
void TransportPage::onSaveForm()
{
    qDebug() << "=== DÉBUT SAUVEGARDE ===";

    if (!validateForm()) {
        qDebug() << "❌ Validation échouée";
        return;
    }

    // CORRIGÉ : Pour QLineEdit, utilisez text() au lieu de currentText()
    QString codeUnique = ui->idEdit->text().trimmed();
    if (codeUnique.isEmpty() || codeUnique == "AUCUN_CODE_DISPONIBLE") {
        QMessageBox::warning(this, "Code manquant",
                             "Veuillez saisir un code unique valide.");
        return;
    }

    QString id = ui->idEdit->text().trimmed().toUpper();
    QString statut = ui->statutCombo->currentText().toLower();

    QVariantMap data;
    data["id_vehicule"] = id;
    data["type"] = ui->typeCombo->currentText();
    bool ok = false;
    int capacite = ui->capaciteEdit->text().trimmed().toInt(&ok);
    data["capacite"] = ok ? capacite : 0;
    data["zone"] = ui->zoneEdit->text().trimmed();
    data["horaire"] = ui->horaireEdit->text().trimmed();
    data["statut"] = ui->statutCombo->currentText();
    data["date_ajout"] = ui->dateAjoutEdit->date().toString("yyyy-MM-dd");
    double tempsUtilise = ui->tempsUtiliseEdit->text().trimmed().toDouble(&ok);
    data["temps_utilise"] = ok ? tempsUtilise : 0.0;
    data["code_unique"] = codeUnique;

    bool isUpdate = !DatabaseManager::instance().getTransportVehicule(id).isEmpty();

    bool success = false;
    if (isUpdate) {
        qDebug() << "🔄 Mise à jour du véhicule" << id;
        success = DatabaseManager::instance().updateTransportVehicule(id, data);
    } else {
        qDebug() << "➕ Ajout du véhicule" << id;
        success = DatabaseManager::instance().addTransportVehicule(data);
    }

    if (success) {
        QString action = isUpdate ? "MODIFICATION" : "AJOUT";
        sauvegarderHistorique(data, action);

        // Vérifier et notifier si le véhicule est en maintenance
        if (statut.contains("maintenance") && isNewMaintenanceEvent(id)) {
            m_knownMaintenanceVehicles.insert(id);
            showMaintenanceNotification(id);
            logMaintenanceEvent(id);
            qDebug() << "🚨 Notification maintenance déclenchée pour:" << id;
        }

        QMessageBox::information(this, "Succès",
                                 isUpdate ? "Véhicule modifié avec succès !"
                                          : "Véhicule ajouté avec succès !");
        clearForm();
        loadAllVehicules();
        onTabList();
    } else {
        QString errorMsg = DatabaseManager::instance().database().lastError().text();
        if (errorMsg.isEmpty()) {
            errorMsg = "Vérifiez que tous les champs sont correctement remplis et que le code transaction existe (si fourni).";
        }
        QMessageBox::critical(this, "Erreur",
                              QString("Échec de l'enregistrement !\n\n%1").arg(errorMsg));
    }

    qDebug() << "=== FIN SAUVEGARDE ===";
}
void TransportPage::onCancelForm()
{
    clearForm();
    onTabList();
}

void TransportPage::onFilterTextChanged(const QString &text)
{
    const QString s = text.trimmed().toLower();
    QTableWidget *t = ui->vehiculesTable;

    for (int r = 0; r < t->rowCount(); ++r) {
        bool match = false;
        for (int c = 0; c < t->columnCount(); ++c) {
            if (t->item(r, c) && t->item(r, c)->text().toLower().contains(s)) {
                match = true;
                break;
            }
        }
        t->setRowHidden(r, !match);
    }
}

void TransportPage::onSortIndexChanged(int index)
{
    int col = (index == 0) ? 6 : 1;
    ui->vehiculesTable->sortItems(col, Qt::AscendingOrder);
}

void TransportPage::onExportPdf()
{
    QString file = QFileDialog::getSaveFileName(this, "Exporter PDF", "vehicules_transport.pdf", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter writer(file);
    writer.setPageSize(QPageSize::A4);
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));
    writer.setResolution(150);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int pageWidth = writer.width();
    int pageHeight = writer.height();

    QColor primaryColor(41, 128, 185);
    QColor secondaryColor(52, 152, 219);
    QColor accentColor(46, 204, 113);
    QColor warningColor(241, 196, 15);
    QColor lightGray(248, 249, 250);
    QColor darkGray(52, 58, 64);

    painter.fillRect(0, 0, pageWidth, 200, primaryColor);

    QFont titleFont("Arial", 24, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 50, pageWidth, 60), Qt::AlignCenter, "NEXORA TRANSPORT");

    QFont subtitleFont("Arial", 14);
    painter.setFont(subtitleFont);
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(QRect(0, 110, pageWidth, 40), Qt::AlignCenter, "Gestion des Véhicules - Rapport Détaillé");

    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(QRect(0, 150, pageWidth, 30), Qt::AlignCenter, dateStr);

    int yPos = 220;

    QTableWidget *t = ui->vehiculesTable;
    int totalVehicules = 0;
    int disponibles = 0;
    int reserves = 0;
    int maintenance = 0;

    for (int r = 0; r < t->rowCount(); ++r) {
        if (t->isRowHidden(r)) continue;
        totalVehicules++;
        QString statut = t->item(r, 5) ? t->item(r, 5)->text().toLower() : "";
        if (statut.contains("disponible")) disponibles++;
        else if (statut.contains("réservé") || statut.contains("reserve")) reserves++;
        else if (statut.contains("maintenance")) maintenance++;
    }

    int boxWidth = (pageWidth - 40) / 3;
    int boxHeight = 80;

    painter.fillRect(20, yPos, boxWidth, boxHeight, secondaryColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(20, yPos, boxWidth, 30), Qt::AlignCenter, "TOTAL");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(20, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(totalVehicules));

    painter.fillRect(30 + boxWidth, yPos, boxWidth, boxHeight, accentColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos, boxWidth, 30), Qt::AlignCenter, "DISPONIBLES");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(disponibles));

    painter.fillRect(40 + boxWidth * 2, yPos, boxWidth, boxHeight, warningColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(40 + boxWidth * 2, yPos, boxWidth, 30), Qt::AlignCenter, "RÉSERVÉS");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(40 + boxWidth * 2, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(reserves));

    yPos += boxHeight + 30;

    if (totalVehicules > 0) {
        QStringList headers = {
            "ID", "Type", "Capacité", "Zone", "Horaire",
            "Statut", "Date", "Temps(h)", "Km", "Code"
        };

        QList<int> columnWidths = {
            pageWidth * 8 / 100,
            pageWidth * 10 / 100,
            pageWidth * 8 / 100,
            pageWidth * 15 / 100,
            pageWidth * 12 / 100,
            pageWidth * 12 / 100,
            pageWidth * 10 / 100,
            pageWidth * 8 / 100,
            pageWidth * 8 / 100,
            pageWidth * 9 / 100
        };

        painter.fillRect(0, yPos, pageWidth, 40, darkGray);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));

        int xPos = 0;
        for (int i = 0; i < headers.size(); ++i) {
            painter.drawText(QRect(xPos, yPos, columnWidths[i], 40),
                             Qt::AlignCenter, headers[i]);
            painter.setPen(QPen(Qt::white, 1));
            painter.drawLine(xPos + columnWidths[i], yPos, xPos + columnWidths[i], yPos + 40);
            xPos += columnWidths[i];
        }

        yPos += 40;

        painter.setFont(QFont("Arial", 9));
        int rowHeight = 35;
        int rowsPerPage = (pageHeight - yPos - 50) / rowHeight;
        int currentRow = 0;

        for (int r = 0; r < t->rowCount(); ++r) {
            if (t->isRowHidden(r)) continue;

            if (currentRow >= rowsPerPage) {
                writer.newPage();
                yPos = 50;
                currentRow = 0;

                painter.fillRect(0, yPos, pageWidth, 40, darkGray);
                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", 10, QFont::Bold));

                xPos = 0;
                for (int i = 0; i < headers.size(); ++i) {
                    painter.drawText(QRect(xPos, yPos, columnWidths[i], 40),
                                     Qt::AlignCenter, headers[i]);
                    painter.drawLine(xPos + columnWidths[i], yPos, xPos + columnWidths[i], yPos + 40);
                    xPos += columnWidths[i];
                }
                yPos += 40;
            }

            if (currentRow % 2 == 0) {
                painter.fillRect(0, yPos, pageWidth, rowHeight, lightGray);
            } else {
                painter.fillRect(0, yPos, pageWidth, rowHeight, Qt::white);
            }

            QString statut = t->item(r, 5) ? t->item(r, 5)->text() : "";
            QColor statutColor = Qt::black;
            if (statut.toLower().contains("disponible")) statutColor = accentColor;
            else if (statut.toLower().contains("réservé") || statut.toLower().contains("reserve")) statutColor = warningColor;
            else if (statut.toLower().contains("maintenance")) statutColor = QColor(231, 76, 60);

            xPos = 0;
            painter.setPen(Qt::black);

            for (int col = 0; col < t->columnCount(); ++col) {
                QString text = t->item(r, col) ? t->item(r, col)->text() : "";

                if (col == 5) {
                    painter.setPen(statutColor);
                    painter.setFont(QFont("Arial", 9, QFont::Bold));
                } else {
                    painter.setPen(Qt::black);
                    painter.setFont(QFont("Arial", 9));
                }

                Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter;
                if (col == 2 || col == 7 || col == 8) alignment = Qt::AlignRight | Qt::AlignVCenter;
                else if (col == 0 || col == 9) alignment = Qt::AlignCenter;

                QRect textRect(xPos + 5, yPos, columnWidths[col] - 10, rowHeight);
                painter.drawText(textRect, alignment, text);

                painter.setPen(QPen(QColor(200, 200, 200), 1));
                painter.drawLine(xPos + columnWidths[col], yPos, xPos + columnWidths[col], yPos + rowHeight);

                xPos += columnWidths[col];
            }

            painter.setPen(QPen(QColor(200, 200, 200), 1));
            painter.drawLine(0, yPos + rowHeight, pageWidth, yPos + rowHeight);

            yPos += rowHeight;
            currentRow++;
        }
    } else {
        painter.setPen(darkGray);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(QRect(0, yPos, pageWidth, 100), Qt::AlignCenter, "Aucun véhicule à afficher");
    }

    int footerY = pageHeight - 40;
    painter.setPen(QColor(150, 150, 150));
    painter.setFont(QFont("Arial", 8));
    painter.drawLine(0, footerY, pageWidth, footerY);
    painter.drawText(QRect(0, footerY + 10, pageWidth / 2, 30),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "NEXORA Smart City Management System");
    painter.drawText(QRect(0, footerY + 10, pageWidth, 30),
                     Qt::AlignRight | Qt::AlignVCenter,
                     QString("Page 1 • Généré le %1").arg(
                         QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm")));

    painter.end();

    QMessageBox::information(this, "Export PDF",
                             QString("📊 PDF exporté avec succès !\n\n"
                                     "• Total: %1 véhicules\n"
                                     "• Disponibles: %2\n"
                                     "• Réservés: %3\n"
                                     "• Maintenance: %4\n\n"
                                     "📁 Fichier: %5")
                                 .arg(totalVehicules)
                                 .arg(disponibles)
                                 .arg(reserves)
                                 .arg(maintenance)
                                 .arg(file));
}

void TransportPage::onEditClicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Modifier", "Veuillez sélectionner un véhicule.");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();
    QVariantMap v = DatabaseManager::instance().getTransportVehicule(id);

    if (v.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Véhicule introuvable !");
        return;
    }

    loadVehicule(v);
    onTabAdd();
}

void TransportPage::onDeleteClicked()
{
    int row = ui->vehiculesTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Supprimer", "Veuillez sélectionner un véhicule.");
        return;
    }

    QString id = ui->vehiculesTable->item(row, 0)->text();

    if (QMessageBox::question(this, "Confirmer",
                              QString("Voulez-vous vraiment supprimer le véhicule %1 ?").arg(id),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QVariantMap vehicule = DatabaseManager::instance().getTransportVehicule(id);

    if (DatabaseManager::instance().deleteTransportVehicule(id)) {
        if (!vehicule.isEmpty()) {
            sauvegarderHistorique(vehicule, "SUPPRESSION");
        }

        QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès !");
        loadAllVehicules();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
    }
}


void TransportPage::onIdTextChanged()
{
    hideError();
}

void TransportPage::onCapaciteTextChanged()
{
    hideError();
}

void TransportPage::onAfficherStatistiques()
{
    afficherGraphiqueKilometrage();
}

void TransportPage::onHoraireTextChanged()
{
    hideError();

    QString horaire = ui->horaireEdit->text().trimmed();

    if (!horaire.isEmpty()) {
        if (!validateHoraire(horaire)) {
            showError("Format d'horaire invalide. Utilisez HH:MM-HH:MM (ex: 08:00-18:00)");
            return;
        }
    }
}

// === MÉTHODES PRIVÉES ===

void TransportPage::setupTableHeaders()
{
    QTableWidget *t = ui->vehiculesTable;
    t->setColumnCount(10);
    QStringList headers{
        "ID Véhicule", "Type", "Capacité", "Zone", "Horaire",
        "Statut", "Date Ajout", "Temps Utilisé (h)", "Kilométrage (km)", "Code Unique"
    };
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setAlternatingRowColors(true);
}

void TransportPage::setupUiBehavior()
{
    connect(ui->tabAddButton, &QPushButton::clicked, this, &TransportPage::onTabAdd);
    connect(ui->tabListButton, &QPushButton::clicked, this, &TransportPage::onTabList);
    connect(ui->tabStatsButton, &QPushButton::clicked, this, &TransportPage::onTabStats);
    connect(ui->saveFormButton, &QPushButton::clicked, this, &TransportPage::onSaveForm);
    connect(ui->cancelFormButton, &QPushButton::clicked, this, &TransportPage::onCancelForm);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &TransportPage::onFilterTextChanged);
    connect(ui->sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransportPage::onSortIndexChanged);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &TransportPage::onExportPdf);
    connect(ui->editVehiculeButton, &QPushButton::clicked, this, &TransportPage::onEditClicked);
    connect(ui->deleteVehiculeButton, &QPushButton::clicked, this, &TransportPage::onDeleteClicked);
    connect(ui->afficherStatsButton, &QPushButton::clicked, this, &TransportPage::onAfficherStatistiques);
    connect(ui->historiqueButton, &QPushButton::clicked, this, &TransportPage::onHistoriqueClicked);
}
void TransportPage::clearForm()
{
    ui->idEdit->clear();
    ui->capaciteEdit->clear();
    ui->zoneEdit->clear();
    ui->horaireEdit->clear();
    ui->tempsUtiliseEdit->clear();
    ui->codeUniqueEdit->clear();
    ui->typeCombo->setCurrentIndex(0);
    ui->statutCombo->setCurrentIndex(0);
    ui->dateAjoutEdit->setDate(QDate::currentDate());
    hideError();
}
void TransportPage::loadAllVehicules()
{
    qDebug() << "=== CHARGEMENT VÉHICULES ===";

    QTableWidget *t = ui->vehiculesTable;
    const bool blocked = t->signalsBlocked();
    t->blockSignals(true);
    t->setSortingEnabled(false);
    t->clearContents();
    t->setRowCount(0);

    if (!DatabaseManager::instance().database().isOpen()) {
        qDebug() << "❌ Base non connectée";
        t->blockSignals(blocked);
        return;
    }

    QList<QVariantMap> rows = DatabaseManager::instance().getAllTransportVehicules();
    qDebug() << "📊 Trouvé" << rows.size() << "véhicules dans la base";

    if (rows.isEmpty()) {
        qDebug() << "⚠ Aucun véhicule trouvé";
        t->blockSignals(blocked);
        return;
    }

    t->setRowCount(rows.size());

    for (int r = 0; r < rows.size(); ++r) {
        const auto &m = rows[r];

        QString type = m.value("type").toString();
        double tempsUtilise = m.value("temps_utilise").toDouble();
        double kilometrage = calculerKilometrage(type, tempsUtilise);

        t->setItem(r, 0, new QTableWidgetItem(m.value("id_vehicule").toString()));
        t->setItem(r, 1, new QTableWidgetItem(type));
        t->setItem(r, 2, new QTableWidgetItem(m.value("capacite").toString()));
        t->setItem(r, 3, new QTableWidgetItem(m.value("zone").toString()));
        t->setItem(r, 4, new QTableWidgetItem(m.value("horaire").toString()));
        t->setItem(r, 5, new QTableWidgetItem(m.value("statut").toString()));
        t->setItem(r, 6, new QTableWidgetItem(m.value("date_ajout").toString()));
        t->setItem(r, 7, new QTableWidgetItem(m.value("temps_utilise").toString()));
        t->setItem(r, 8, new QTableWidgetItem(QString::number(kilometrage, 'f', 1)));
        t->setItem(r, 9, new QTableWidgetItem(m.value("code_unique").toString()));

        qDebug() << "  ➜ Ligne" << r << ":" << m.value("id_vehicule").toString()
                 << "- Km:" << kilometrage;
    }

    t->blockSignals(blocked);
    t->resizeColumnsToContents();
    t->setSortingEnabled(true);
    t->scrollToTop();
    t->clearSelection();
    t->viewport()->update();

    qDebug() << "✅ Tableau mis à jour avec" << t->rowCount() << "lignes";
    qDebug() << "=== FIN CHARGEMENT ===";
}
void TransportPage::setupValidation()
{
    ui->errorLabel->hide();
    ui->idEdit->setMaxLength(10);
    ui->idEdit->setPlaceholderText("VH001");
    ui->capaciteEdit->setPlaceholderText("Ex: 4, 20, 5");
    // CORRIGÉ : Pour QLineEdit, définissez un placeholder
    // Code unique field is now combined with ID field

    QRegularExpressionValidator *horaireValidator = new QRegularExpressionValidator(
        QRegularExpression("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]-([0-1]?[0-9]|2[0-3]):[0-5][0-9]$"), this);
    ui->horaireEdit->setValidator(horaireValidator);
    ui->horaireEdit->setPlaceholderText("HH:MM-HH:MM (ex: 08:00-18:00)");

    // SUPPRIMEZ ces lignes car c'est un QLineEdit
    // ui->codeUniqueEdit->setEditable(false);
    // ui->codeUniqueEdit->setToolTip("Cliquez pour voir les codes disponibles depuis la table transaction");

    connect(ui->idEdit, &QLineEdit::textChanged, this, &TransportPage::onIdTextChanged);
    connect(ui->capaciteEdit, &QLineEdit::textChanged, this, &TransportPage::onCapaciteTextChanged);
    connect(ui->horaireEdit, &QLineEdit::textChanged, this, &TransportPage::onHoraireTextChanged);

    ui->dateAjoutEdit->setDate(QDate::currentDate());
}
bool TransportPage::validateForm()
{
    if (!validateId(ui->idEdit->text())) {
        showError("L'ID doit commencer par 'VH'");
        return false;
    }

    if (ui->typeCombo->currentText().isEmpty()) {
        showError("Veuillez choisir un type");
        return false;
    }

    if (!validateCapacite(ui->capaciteEdit->text())) {
        showError("Capacité invalide (doit être un nombre positif)");
        return false;
    }

    if (ui->zoneEdit->text().trimmed().isEmpty()) {
        showError("La zone est obligatoire");
        return false;
    }

    // CORRIGÉ : Pour QLineEdit, utilisez text()
    QString codeUnique = ui->idEdit->text().trimmed();
    if (codeUnique.isEmpty()) {
        showError("Veuillez saisir un code unique");
        return false;
    }

    return true;
}
bool TransportPage::validateId(const QString &id)
{
    QString trimmed = id.trimmed().toUpper();
    return trimmed.startsWith("VH") && trimmed.length() >= 3;
}

bool TransportPage::validateCapacite(const QString &c)
{
    bool ok;
    int val = c.toInt(&ok);
    return ok && val > 0;
}

bool TransportPage::validateHoraire(const QString &horaire)
{
    QRegularExpression regex("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]-([0-1]?[0-9]|2[0-3]):[0-5][0-9]$");
    QRegularExpressionMatch match = regex.match(horaire);

    if (!match.hasMatch()) {
        return false;
    }

    QStringList parties = horaire.split('-');
    if (parties.size() != 2) {
        return false;
    }

    QString debut = parties[0];
    QString fin = parties[1];

    QStringList debutParts = debut.split(':');
    QStringList finParts = fin.split(':');

    if (debutParts.size() != 2 || finParts.size() != 2) {
        return false;
    }

    int heuresDebut = debutParts[0].toInt();
    int minutesDebut = debutParts[1].toInt();
    int heuresFin = finParts[0].toInt();
    int minutesFin = finParts[1].toInt();

    bool debutValide = (heuresDebut >= 0 && heuresDebut <= 23) && (minutesDebut >= 0 && minutesDebut <= 59);
    bool finValide = (heuresFin >= 0 && heuresFin <= 23) && (minutesFin >= 0 && minutesFin <= 59);

    return debutValide && finValide;
}

void TransportPage::showError(const QString &msg)
{
    ui->errorLabel->setText(msg);
    ui->errorLabel->setStyleSheet("color: red; font-weight: bold;");
    ui->errorLabel->show();
}

void TransportPage::hideError()
{
    ui->errorLabel->hide();
}

void TransportPage::loadVehicule(const QVariantMap &v)
{
    ui->idEdit->setText(v.value("id_vehicule").toString());
    ui->typeCombo->setCurrentText(v.value("type").toString());
    ui->capaciteEdit->setText(v.value("capacite").toString());
    ui->zoneEdit->setText(v.value("zone").toString());
    ui->horaireEdit->setText(v.value("horaire").toString());
    ui->statutCombo->setCurrentText(v.value("statut").toString());
    ui->dateAjoutEdit->setDate(QDate::fromString(v.value("date_ajout").toString(), "yyyy-MM-dd"));
    ui->tempsUtiliseEdit->setText(v.value("temps_utilise").toString());

    // Switch to add tab to show the loaded data
    ui->tabAddButton->setChecked(true);
}

double TransportPage::calculerKilometrage(const QString& type, double tempsUtilise) const
{
    QMap<QString, double> vitessesMoyennes = {
        {"taxi", 25.0},
        {"bus", 20.0},
        {"voiture", 35.0}
    };

    QString typeClean = type.trimmed().toLower();
    double vitesse = vitessesMoyennes.value(typeClean, 30.0);

    return tempsUtilise * vitesse;
}

void TransportPage::calculerPourcentagesKilometrage()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        ui->pourcentageKilometrage->setText("Aucun véhicule disponible");
        return;
    }

    double totalKilometrage = 0;
    QMap<QString, double> kmParType;

    for (const auto &vehicule : vehicules) {
        QString type = vehicule.value("type").toString().toLower();
        double tempsUtilise = vehicule.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);

        if (!type.isEmpty()) {
            kmParType[type] += km;
            totalKilometrage += km;
        }
    }

    if (totalKilometrage == 0) {
        ui->pourcentageKilometrage->setText("Aucun kilométrage enregistré");
        return;
    }

    QString pourcentages = "📊 RÉPARTITION DU KILOMÉTRAGE :\n\n";

    QList<QPair<QString, double>> typesTries;
    for (auto it = kmParType.begin(); it != kmParType.end(); ++it) {
        typesTries.append(qMakePair(it.key(), it.value()));
    }

    std::sort(typesTries.begin(), typesTries.end(),
              [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
                  return a.second > b.second;
              });

    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double kmType = pair.second;
        double pourcentage = (kmType / totalKilometrage) * 100;

        QString icone = "🚗";
        if (type == "bus") icone = "🚌";
        else if (type == "taxi") icone = "🚕";

        pourcentages += QString("%1 %2 : %3%\n")
                            .arg(icone)
                            .arg(type.toUpper())
                            .arg(QString::number(pourcentage, 'f', 1));
    }

    pourcentages += QString("\n🌍 Total : %1 km")
                        .arg(QString::number(totalKilometrage, 'f', 1));

    ui->pourcentageKilometrage->setText(pourcentages.trimmed());
}

void TransportPage::calculateAndDisplayStatistics()
{
    calculerPourcentagesKilometrage();

    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        ui->vehiculePlusUtilise->setText("Aucun véhicule");
        ui->vehiculeMoinsUtilise->setText("Aucun véhicule");
        return;
    }

    double maxKm = 0;
    double minKm = std::numeric_limits<double>::max();
    QString vehiculePlusUtilise;
    QString vehiculeMoinsUtilise;

    for (const auto &v : vehicules) {
        QString type = v.value("type").toString();
        double tempsUtilise = v.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);
        QString id = v.value("id_vehicule").toString();

        if (km > maxKm) {
            maxKm = km;
            vehiculePlusUtilise = QString("%1 (%2 km)").arg(id).arg(QString::number(km, 'f', 1));
        }

        if (km < minKm) {
            minKm = km;
            vehiculeMoinsUtilise = QString("%1 (%2 km)").arg(id).arg(QString::number(km, 'f', 1));
        }
    }

    ui->vehiculePlusUtilise->setText(vehiculePlusUtilise);
    ui->vehiculeMoinsUtilise->setText(vehiculeMoinsUtilise);
}

void TransportPage::afficherGraphiqueKilometrage()
{
    QList<QVariantMap> vehicules = DatabaseManager::instance().getAllTransportVehicules();

    if (vehicules.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucun véhicule disponible pour afficher les statistiques.");
        return;
    }

    QMap<QString, double> kmTotalParType;
    double kilometrageTotalGlobal = 0;

    for (const auto &vehicule : vehicules) {
        QString type = vehicule.value("type").toString().toLower();
        double tempsUtilise = vehicule.value("temps_utilise").toDouble();
        double km = calculerKilometrage(type, tempsUtilise);

        kmTotalParType[type] += km;
        kilometrageTotalGlobal += km;
    }

    if (kilometrageTotalGlobal == 0) {
        QMessageBox::information(this, "Statistiques", "Aucun kilométrage enregistré.");
        return;
    }

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📊 Statistiques de Kilométrage par Type");
    dialog->setMinimumSize(800, 600);
    dialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F8FAFB, stop:1 #E2E8F0);
            font-family: "Segoe UI";
        }
        QLabel {
            font-size: 14px;
            font-weight: 600;
            color: #2D3748;
            padding: 8px;
        }
        QTableWidget {
            border: 1px solid rgba(135,206,235,0.3);
            border-radius: 8px;
            background: white;
        }
        QHeaderView::section {
            background: #87CEEB;
            color: white;
            font-weight: 600;
            padding: 8px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *titleLabel = new QLabel("📊 RÉPARTITION DU KILOMÉTRAGE PAR TYPE DE VÉHICULE");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: 700; color: #1F2D5C; padding: 15px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    QPieSeries *series = new QPieSeries();

    QMap<QString, QColor> couleurs = {
        {"taxi", QColor(255, 193, 7)},
        {"bus", QColor(33, 150, 243)},
        {"voiture", QColor(76, 175, 80)}
    };

    QList<QPair<QString, double>> typesTries;
    for (auto it = kmTotalParType.begin(); it != kmTotalParType.end(); ++it) {
        typesTries.append(qMakePair(it.key(), it.value()));
    }

    std::sort(typesTries.begin(), typesTries.end(),
              [](const QPair<QString, double> &a, const QPair<QString, double> &b) {
                  return a.second > b.second;
              });

    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double km = pair.second;
        double pourcentage = (km / kilometrageTotalGlobal) * 100;

        QString label = type.toUpper() + QString(" (%1%)").arg(QString::number(pourcentage, 'f', 1));
        QPieSlice *slice = series->append(label, km);

        QString icone;
        if (type == "taxi") {
            icone = "🚕";
            slice->setColor(couleurs["taxi"]);
        } else if (type == "bus") {
            icone = "🚌";
            slice->setColor(couleurs["bus"]);
        } else if (type == "voiture") {
            icone = "🚗";
            slice->setColor(couleurs["voiture"]);
        } else {
            icone = "🚙";
            slice->setColor(QColor(158, 158, 158));
        }

        slice->setLabel(icone + " " + label);
        slice->setLabelVisible(true);
        slice->setExploded(true);
        slice->setExplodeDistanceFactor(0.05);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition du Kilométrage");
    chart->setTitleFont(QFont("Segoe UI", 14, QFont::Bold));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);
    layout->addWidget(chartView);

    QLabel *tableTitle = new QLabel("📋 DÉTAIL PAR TYPE");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: 700; color: #1F2D5C; padding: 10px;");
    layout->addWidget(tableTitle);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Type", "Kilométrage", "Pourcentage", "Icône"});
    table->setRowCount(typesTries.size());

    int row = 0;
    for (const auto &pair : typesTries) {
        QString type = pair.first;
        double km = pair.second;
        double pourcentage = (km / kilometrageTotalGlobal) * 100;

        QString icone;
        if (type == "taxi") icone = "🚕";
        else if (type == "bus") icone = "🚌";
        else if (type == "voiture") icone = "🚗";
        else icone = "🚙";

        table->setItem(row, 0, new QTableWidgetItem(type.toUpper()));
        table->setItem(row, 1, new QTableWidgetItem(QString::number(km, 'f', 1) + " km"));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(pourcentage, 'f', 1) + " %"));
        table->setItem(row, 3, new QTableWidgetItem(icone));

        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(row, col);
            if (item) {
                if (type == "taxi") {
                    item->setBackground(QColor(255, 245, 157));
                } else if (type == "bus") {
                    item->setBackground(QColor(197, 202, 233));
                } else if (type == "voiture") {
                    item->setBackground(QColor(200, 230, 201));
                }
            }
        }
        row++;
    }

    table->resizeColumnsToContents();
    layout->addWidget(table);

    QLabel *summaryLabel = new QLabel(
        QString("🌍 KILOMÉTRAGE TOTAL: %1 km\n"
                "📊 NOMBRE DE VÉHICULES: %2")
            .arg(QString::number(kilometrageTotalGlobal, 'f', 1))
            .arg(vehicules.size())
        );
    summaryLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #2D3748; background: #E3F2FD; padding: 12px; border-radius: 8px;");
    summaryLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(summaryLabel);

    dialog->exec();
    delete dialog;
}

void TransportPage::afficherHistoriqueComplet()
{
    QList<QVariantMap> historique = getHistoriqueComplet();

    if (historique.isEmpty()) {
        QMessageBox::information(this, "Historique", "Aucun historique disponible.");
        return;
    }

    // Générer le fichier HTML
    genererHistoriqueHTML(historique);

    // Afficher aussi le dialogue normal (optionnel)
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📜 Historique Complet des Véhicules");
    dialog->setMinimumSize(1200, 700);
    dialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #F8FAFB, stop:1 #E2E8F0);
            font-family: "Segoe UI";
        }
        QLabel {
            font-size: 14px;
            font-weight: 600;
            color: #2D3748;
            padding: 8px;
        }
        QTableWidget {
            border: 2px solid #87CEEB;
            border-radius: 8px;
            background: white;
        }
        QHeaderView::section {
            background: #87CEEB;
            color: white;
            font-weight: 600;
            padding: 8px;
            border: 1px solid #5D9CEC;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QLabel *titleLabel = new QLabel("📜 HISTORIQUE COMPLET DES VÉHICULES");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: 700; color: #1F2D5C; padding: 20px; background: rgba(135,206,235,0.1); border-radius: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Ajouter un message indiquant que le fichier HTML a été généré
    QLabel *infoLabel = new QLabel("✅ Fichier HTML généré dans le dossier 'Historique' sur le Bureau");
    infoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #48BB78; background: #F0FFF4; padding: 15px; border: 2px solid #48BB78; border-radius: 8px;");
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);

    QTableWidget *table = new QTableWidget();
    table->setColumnCount(12);
    table->setHorizontalHeaderLabels({
        "Action", "Date/Heure", "ID Véhicule", "Type", "Capacité", "Zone",
        "Horaire", "Statut", "Date Ajout", "Temps Utilisé (h)", "Kilométrage (km)", "Code Unique"
    });

    table->setRowCount(historique.size());

    for (int i = 0; i < historique.size(); ++i) {
        const auto &v = historique[i];
        QString type = v.value("type").toString();
        double tempsUtilise = v.value("temps_utilise").toDouble();
        double kilometrage = calculerKilometrage(type, tempsUtilise);
        QString action = v.value("historique_action").toString();
        QString timestamp = v.value("historique_timestamp").toString();

        table->setItem(i, 0, new QTableWidgetItem(action));
        table->setItem(i, 1, new QTableWidgetItem(timestamp));
        table->setItem(i, 2, new QTableWidgetItem(v.value("id_vehicule").toString()));
        table->setItem(i, 3, new QTableWidgetItem(type));
        table->setItem(i, 4, new QTableWidgetItem(v.value("capacite").toString()));
        table->setItem(i, 5, new QTableWidgetItem(v.value("zone").toString()));
        table->setItem(i, 6, new QTableWidgetItem(v.value("horaire").toString()));
        table->setItem(i, 7, new QTableWidgetItem(v.value("statut").toString()));
        table->setItem(i, 8, new QTableWidgetItem(v.value("date_ajout").toString()));
        table->setItem(i, 9, new QTableWidgetItem(v.value("temps_utilise").toString()));
        table->setItem(i, 10, new QTableWidgetItem(QString::number(kilometrage, 'f', 1)));
        table->setItem(i, 11, new QTableWidgetItem(v.value("code_unique").toString()));

        QColor rowColor;
        if (action == "AJOUT") rowColor = QColor(200, 230, 201);
        else if (action == "MODIFICATION") rowColor = QColor(255, 245, 157);
        else if (action == "SUPPRESSION") rowColor = QColor(255, 205, 210);

        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem *item = table->item(i, col);
            if (item) {
                item->setBackground(rowColor);
                if (col == 0) {
                    QFont font = item->font();
                    font.setBold(true);
                    item->setFont(font);
                }
            }
        }
    }

    table->resizeColumnsToContents();
    table->setAlternatingRowColors(false);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(table);

    int ajouts = 0, modifications = 0, suppressions = 0;
    for (const auto &v : historique) {
        QString action = v.value("historique_action").toString();
        if (action == "AJOUT") ajouts++;
        else if (action == "MODIFICATION") modifications++;
        else if (action == "SUPPRESSION") suppressions++;
    }

    QLabel *summaryLabel = new QLabel(
        QString("📊 RÉSUMÉ DE L'HISTORIQUE:\n"
                "• Ajouts: %1 | Modifications: %2 | Suppressions: %3\n"
                "• Total des opérations: %4")
            .arg(ajouts).arg(modifications).arg(suppressions).arg(historique.size())
        );
    summaryLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #2D3748; background: #E3F2FD; padding: 15px; border-radius: 8px;");
    summaryLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(summaryLabel);

    dialog->exec();
    delete dialog;
}
void TransportPage::setVehicules(const QList<QVariantMap> &rows)
{
    Q_UNUSED(rows);
    loadAllVehicules();
}

void TransportPage::showEmptyTable()
{
    ui->vehiculesTable->setRowCount(0);
}

int TransportPage::sortColumnForIndex(int index) const
{
    return (index == 0) ? 6 : 1;
}
