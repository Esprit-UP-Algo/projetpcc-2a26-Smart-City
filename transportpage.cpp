#include "transportpage.h"
#include "ui_transportpage.h"
#include "databasemanager.h"
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
}
TransportPage::~TransportPage()
{
    delete ui;
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

    // Utiliser une icône adaptée à Windows
    QIcon appIcon = QApplication::windowIcon();
    if (appIcon.isNull()) {
        // Créer une icône par défaut si nécessaire
        QPixmap pixmap(32, 32);
        pixmap.fill(Qt::red);
        appIcon = QIcon(pixmap);
    }

    m_trayIcon->setIcon(appIcon);
    m_trayIcon->setToolTip("NEXORA Transport - Gestion des Véhicules");

    QMenu *trayMenu = new QMenu(this);

    QAction *showAction = new QAction("📋 Afficher l'application", this);
    QAction *quitAction = new QAction("❌ Quitter", this);

    connect(showAction, &QAction::triggered, this, [this]() {
        if (this->parentWidget()) {
            this->parentWidget()->show();
            this->parentWidget()->activateWindow();
        }
    });

    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &TransportPage::onTrayIconActivated);

    // Configuration spécifique pour Windows
    m_trayIcon->show();

    // Vérifier que l'icône est bien visible
    QTimer::singleShot(1000, this, [this]() {
        if (m_trayIcon) {
            if (!m_trayIcon->isVisible()) {
                qDebug() << "⚠ Icône tray non visible, tentative de réaffichage";
                m_trayIcon->show();
            }
        }
    });

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
        // Utiliser le style de notification natif de Windows
        m_trayIcon->showMessage(
            title,
            message,
            QSystemTrayIcon::Critical,
            10000 // 10 secondes
            );
    }

    playNotificationSound();

    // Optionnel: garder la popup personnalisée en plus
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
    QString bureauPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString historiqueDir = bureauPath + "/Historique";
    QDir dir(historiqueDir);
    if (!dir.exists()) dir.mkpath(".");

    QString htmlFilePath = historiqueDir + "/historique_vehicules.html";
    QFile htmlFile(htmlFilePath);

    if (!htmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Erreur création fichier HTML:" << htmlFile.errorString();
        return;
    }

    QTextStream out(&htmlFile);
    out.setEncoding(QStringConverter::Utf8);

    // ================== HTML ==================
    out << "<!DOCTYPE html>\n";
    out << "<html lang='fr'>\n";
    out << "<head>\n";
    out << " <meta charset='UTF-8'>\n";
    out << " <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    out << " <title>Historique Véhicules • NEXORA TRANSPORT</title>\n";
    out << " <style>\n";
    out << "  body {font-family:'Segoe UI',Tahoma,sans-serif;background:#f0f2f5;margin:0;padding:20px;}\n";
    out << "  .container {max-width:1600px;margin:auto;background:white;border-radius:16px;overflow:hidden;box-shadow:0 10px 30px rgba(0,0,0,0.1);}\n";
    out << "  header {background:linear-gradient(135deg,#1e3c72,#2a5298);color:white;padding:30px;text-align:center;}\n";
    out << "  h1 {margin:0;font-size:2.4em;}\n";
    out << "  table {width:100%;border-collapse:collapse;margin-top:20px;}\n";
    out << "  th {background:#2c3e50;color:white;padding:15px;text-align:left;font-weight:600;}\n";
    out << "  td {padding:14px 12px;border-bottom:1px solid #eee;vertical-align:top;}\n";
    out << "  tr:nth-child(even){background:#f8f9fa;}\n";
    out << "  .modif   {background:#fff3cd;}\n";
    out << "  .supp    {background:#f8d7da;}\n";
    out << "  .badge   {padding:5px 12px;border-radius:20px;font-weight:bold;font-size:0.85em;}\n";
    out << "  .badge-modif {background:#ffca28;color:#856404;}\n";
    out << "  .badge-supp  {background:#dc3545;color:white;}\n";

    // Style exact de ta capture d’écran
    out << "  .note-change {background:#e8f5e8;border-left:6px solid #27ae60;padding:14px 16px;margin:10px 0;border-radius:0 8px 8px 0;font-family:'Consolas',monospace;line-height:1.6;color:#155724;}\n";
    out << " </style>\n";
    out << "</head>\n";
    out << "<body>\n";
    out << " <div class='container'>\n";
    out << "  <header>\n";
    out << "   <h1>NEXORA TRANSPORT</h1>\n";
    out << "   <p>Historique des modifications & suppressions de véhicules</p>\n";
    out << "   <small>Généré le " << QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm") << "</small>\n";
    out << "  </header>\n";

    out << "  <table>\n";
    out << "   <thead>\n";
    out << "    <tr>\n";
    out << "     <th>Action</th><th>Date/Heure</th><th>ID</th><th>Type</th><th>Capacité</th>\n";
    out << "     <th>Zone</th><th>Horaire</th><th>Statut</th><th>Date ajout</th><th>Temps utilisé</th>\n";
    out << "     <th>Km</th><th>Code unique</th><th style='width:500px;'>Note des changements</th>\n";
    out << "    </tr>\n";
    out << "   </thead>\n";
    out << "   <tbody>\n";

    for (const auto &v : historique) {
        QString action    = v.value("historique_action").toString();
        QString timestamp = v.value("historique_timestamp").toString();
        QString note      = v.value("historique_note").toString();

        QString rowClass   = (action == "MODIFICATION") ? "modif" : "supp";
        QString badgeClass = (action == "MODIFICATION") ? "badge-modif" : "badge-supp";

        double km = calculerKilometrage(v.value("type").toString(),
                                        v.value("temps_utilise").toDouble());

        out << "    <tr class='" << rowClass << "'>\n";
        out << "     <td><span class='badge " << badgeClass << "'>" << action << "</span></td>\n";
        out << "     <td>" << timestamp << "</td>\n";
        out << "     <td><strong>" << v.value("id_vehicule").toString() << "</strong></td>\n";
        out << "     <td>" << v.value("type").toString() << "</td>\n";
        out << "     <td>" << v.value("capacite").toString() << "</td>\n";
        out << "     <td>" << v.value("zone").toString() << "</td>\n";
        out << "     <td>" << v.value("horaire").toString() << "</td>\n";
        out << "     <td>" << v.value("statut").toString() << "</td>\n";           // ← CORRIGÉ
        out << "     <td>" << v.value("date_ajout").toString() << "</td>\n";
        out << "     <td>" << v.value("temps_utilise").toString() << "</td>\n";
        out << "     <td>" << QString::number(km, 'f', 1) << "</td>\n";
        out << "     <td><code>" << v.value("code_unique").toString() << "</code></td>\n";

        // ==== NOTE DES CHANGEMENTS (exactement comme ta capture) ====
        out << "     <td>\n";
        if (note.isEmpty() || note.contains("Aucune modification")) {
            out << "      <em style='color:#95a5a6;'>Aucune note</em>\n";
        } else {
            QString noteHtml = note.toHtmlEscaped().replace("\n", "<br>");
            if (action == "MODIFICATION") {
                out << "      <div class='note-change'>\n";
                out << "       <strong style='color:#27ae60;'>CHANGEMENTS DÉTECTÉS:</strong><br><br>\n";
                out << "       " << noteHtml << "\n";
                out << "      </div>\n";
            } else if (action == "SUPPRESSION") {
                out << "      <div class='note-change' style='background:#ffebee;border-left-color:#c62828;color:#c62828;'>\n";
                out << "       <strong>VÉHICULE SUPPRIMÉ</strong><br><br>\n";
                out << "       " << noteHtml << "\n";
                out << "      </div>\n";
            }
        }
        out << "     </td>\n";
        out << "    </tr>\n";
    }

    out << "   </tbody>\n";
    out << "  </table>\n";
    out << " </div>\n";
    out << "</body>\n";
    out << "</html>\n";

    htmlFile.close();
    qDebug() << "Historique HTML généré →" << htmlFilePath;
}
void TransportPage::sauvegarderHistorique(const QVariantMap &vehicule, const QString &action)
{
    if (action == "AJOUT") return;
    if (action != "MODIFICATION" && action != "SUPPRESSION") return;

    QString cheminFichier = getFichierHistorique();
    QFile fichier(cheminFichier);
    QJsonArray historiqueArray;

    if (fichier.exists() && fichier.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(fichier.readAll());
        if (!doc.isNull()) historiqueArray = doc.array();
        fichier.close();
    }

    QString noteFinale;
    QStringList changements;
    QString idVehicule = vehicule["id_vehicule"].toString();

    if (action == "MODIFICATION") {
        // Récupérer l'ancien état depuis la base de données
        QVariantMap ancien = DatabaseManager::instance().getTransportVehicule(idVehicule);

        if (ancien.isEmpty()) {
            qDebug() << "Impossible de récupérer l'ancien état du véhicule" << idVehicule;
            return;
        }

        // Dictionnaire des noms de champs en français
        QMap<QString, QString> nomsChamps = {
            {"type", "Type de véhicule"},
            {"capacite", "Capacité"},
            {"zone", "Zone d'opération"},
            {"horaire", "Horaire de service"},
            {"statut", "Statut"},
            {"date_ajout", "Date d'ajout"},
            {"temps_utilise", "Temps utilisé (heures)"},
            {"code_unique", "Code unique"}
        };

        // Comparer chaque champ
        QStringList champsModifies;

        for (const QString &champ : ancien.keys()) {
            QVariant ancienneValeur = ancien.value(champ);
            QVariant nouvelleValeur = vehicule.value(champ);

            // Convertir pour la comparaison
            QString ancienneStr, nouvelleStr;

            if (champ == "capacite") {
                ancienneStr = QString::number(ancienneValeur.toInt());
                nouvelleStr = QString::number(nouvelleValeur.toInt());
            }
            else if (champ == "temps_utilise") {
                ancienneStr = QString::number(ancienneValeur.toDouble(), 'f', 2);
                nouvelleStr = QString::number(nouvelleValeur.toDouble(), 'f', 2);
            }
            else if (champ == "date_ajout") {
                QDate ancienneDate = QDate::fromString(ancienneValeur.toString(), "yyyy-MM-dd");
                QDate nouvelleDate = QDate::fromString(nouvelleValeur.toString(), "yyyy-MM-dd");
                ancienneStr = ancienneDate.toString("dd/MM/yyyy");
                nouvelleStr = nouvelleDate.toString("dd/MM/yyyy");
            }
            else {
                ancienneStr = ancienneValeur.toString().trimmed();
                nouvelleStr = nouvelleValeur.toString().trimmed();
            }

            if (ancienneStr.isEmpty()) ancienneStr = "(vide)";
            if (nouvelleStr.isEmpty()) nouvelleStr = "(vide)";

            // Vérifier si le champ a été modifié
            if (ancienneStr != nouvelleStr) {
                QString nomChamp = nomsChamps.value(champ, champ);
                changements << QString("✓ %1 modifié : %2 → %3")
                                   .arg(nomChamp)
                                   .arg(ancienneStr)
                                   .arg(nouvelleStr);
                champsModifies << nomChamp;
            }
        }

        // Formater la note finale
        if (changements.isEmpty()) {
            noteFinale = "Aucun changement détecté";
        } else {
            noteFinale = "CHAMPS MODIFIÉS :\n";
            noteFinale += "====================\n";

            // Résumé des champs modifiés
            noteFinale += QString("Nombre de champs modifiés : %1\n\n")
                              .arg(champsModifies.size());
            noteFinale += changements.join("\n");

            // Ajouter un résumé
            noteFinale += QString("\n\n📋 RÉSUMÉ : %1 champ(s) modifié(s)")
                              .arg(champsModifies.size());
        }
    }
    else if (action == "SUPPRESSION") {
        // Format pour les suppressions
        noteFinale = QString("VÉHICULE SUPPRIMÉ\n")
                         .arg(idVehicule)
                         .arg(vehicule["type"].toString())
                         .arg(vehicule["statut"].toString());
    }

    // === Enregistrement dans le JSON ===
    QJsonObject entree;
    entree["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    entree["action"] = action;
    entree["note"] = noteFinale;

    // Enregistrer également la liste des champs modifiés séparément
    QJsonArray champsModifiesArray;
    QStringList champsListe = noteFinale.split("\n")
                                  .filter("✓")
                                  .replaceInStrings("✓ ", "");

    for (const QString &champ : champsListe) {
        champsModifiesArray.append(champ);
    }
    entree["champs_modifies"] = champsModifiesArray;

    QJsonObject data;
    data["id_vehicule"] = idVehicule;
    data["type"] = vehicule["type"].toString();
    data["capacite"] = vehicule["capacite"].toInt();
    data["zone"] = vehicule["zone"].toString();
    data["horaire"] = vehicule["horaire"].toString();
    data["statut"] = vehicule["statut"].toString();
    data["date_ajout"] = vehicule["date_ajout"].toString();
    data["temps_utilise"] = vehicule["temps_utilise"].toDouble();
    data["code_unique"] = vehicule["code_unique"].toString();

    entree["vehicule"] = data;
    historiqueArray.prepend(entree);

    while (historiqueArray.size() > 1000) {
        historiqueArray.removeLast();
    }

    if (fichier.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(historiqueArray);
        fichier.write(doc.toJson(QJsonDocument::Indented));
        fichier.close();

        // Mettre à jour l'affichage HTML
        QTimer::singleShot(100, this, [this]() {
            genererHistoriqueHTML(getHistoriqueComplet());
        });

        qDebug() << "📝 Historique enregistré →" << action << idVehicule
                 << "| Champs modifiés:" << champsListe.size();
    }
}
void TransportPage::sauvegarderHistoriqueModification(const QVariantMap &ancienVehicule, const QVariantMap &nouveauVehicule)
{
    if (ancienVehicule.isEmpty() || nouveauVehicule.isEmpty()) {
        qDebug() << "❌ Impossible de sauvegarder l'historique : données manquantes";
        return;
    }

    QString idVehicule = ancienVehicule["id_vehicule"].toString();
    qDebug() << "=== SAUVEGARDE HISTORIQUE MODIFICATION ===";
    qDebug() << "ID Véhicule:" << idVehicule;
    qDebug() << "Ancien état:" << ancienVehicule;
    qDebug() << "Nouvel état:" << nouveauVehicule;

    QString cheminFichier = getFichierHistorique();
    QFile fichier(cheminFichier);
    QJsonArray historiqueArray;

    if (fichier.exists() && fichier.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(fichier.readAll());
        if (!doc.isNull()) historiqueArray = doc.array();
        fichier.close();
    }

    QString noteFinale;
    QStringList changements;

    // Dictionnaire des noms de champs en français
    QMap<QString, QString> nomsChamps = {
        {"type", "Type"},
        {"capacite", "Capacité"},
        {"zone", "Zone"},
        {"horaire", "Horaire"},
        {"statut", "Statut"},
        {"date_ajout", "Date ajout"},
        {"temps_utilise", "Temps utilisé"},
        {"code_unique", "Code unique"}
    };

    // Liste des champs à comparer
    QStringList champsAControler = {"type", "capacite", "zone", "horaire",
                                    "statut", "date_ajout", "temps_utilise",
                                    "code_unique"};

    // Comparer chaque champ
    for (const QString &champ : champsAControler) {
        if (!ancienVehicule.contains(champ) || !nouveauVehicule.contains(champ)) {
            qDebug() << "  ⚠ Champ" << champ << "manquant dans l'un des états";
            continue;
        }

        QVariant ancienneValeur = ancienVehicule.value(champ);
        QVariant nouvelleValeur = nouveauVehicule.value(champ);

        QString ancienneStr, nouvelleStr;

        // Conversion selon le type de champ
        if (champ == "capacite") {
            ancienneStr = QString::number(ancienneValeur.toInt());
            nouvelleStr = QString::number(nouvelleValeur.toInt());
        }
        else if (champ == "temps_utilise") {
            ancienneStr = QString::number(ancienneValeur.toDouble(), 'f', 2);
            nouvelleStr = QString::number(nouvelleValeur.toDouble(), 'f', 2);
        }
        else if (champ == "date_ajout") {
            // Normaliser les dates
            QDate dateAncienne = QDate::fromString(ancienneValeur.toString(), "yyyy-MM-dd");
            QDate dateNouvelle = QDate::fromString(nouvelleValeur.toString(), "yyyy-MM-dd");

            ancienneStr = dateAncienne.isValid() ? dateAncienne.toString("dd/MM/yyyy") : "";
            nouvelleStr = dateNouvelle.isValid() ? dateNouvelle.toString("dd/MM/yyyy") : "";
        }
        else {
            ancienneStr = ancienneValeur.toString().trimmed();
            nouvelleStr = nouvelleValeur.toString().trimmed();
        }

        // Pour les champs vides
        if (ancienneStr.isEmpty()) ancienneStr = "(vide)";
        if (nouvelleStr.isEmpty()) nouvelleStr = "(vide)";

        qDebug() << "  Comparaison" << champ << ":" << ancienneStr << "vs" << nouvelleStr;

        // Comparaison exacte
        if (ancienneStr != nouvelleStr) {
            QString nomChamp = nomsChamps.value(champ, champ);
            QString changement = QString("✓ %1 modifié : %2 → %3")
                                     .arg(nomChamp)
                                     .arg(ancienneStr)
                                     .arg(nouvelleStr);

            changements.append(changement);
            qDebug() << "  → Changement détecté:" << changement;
        }
    }

    qDebug() << "Nombre total de changements détectés:" << changements.size();

    // Formater la note finale
    if (changements.isEmpty()) {
        noteFinale = "✓ Aucun changement détecté";
        qDebug() << "Aucun changement détecté pour" << idVehicule;
    } else {
        // En-tête
        noteFinale = QString("CHAMPS MODIFIÉS : %1 changement(s)\n\n")
                         .arg(changements.size());

        // Liste des changements
        noteFinale += changements.join("\n");

        // Résumé
        noteFinale += QString("\n\n📋 RÉSUMÉ : %1 champ(s) modifié(s)")
                          .arg(changements.size());

        qDebug() << "Note finale:" << noteFinale;
    }

    // === Enregistrement dans le JSON ===
    QJsonObject entree;
    entree["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    entree["action"] = "MODIFICATION";
    entree["note"] = noteFinale;
    entree["nombre_changements"] = changements.size();

    QJsonObject data;
    data["id_vehicule"] = idVehicule;
    data["type"] = nouveauVehicule.value("type").toString();
    data["capacite"] = nouveauVehicule.value("capacite").toInt();
    data["zone"] = nouveauVehicule.value("zone").toString();
    data["horaire"] = nouveauVehicule.value("horaire").toString();
    data["statut"] = nouveauVehicule.value("statut").toString();
    data["date_ajout"] = nouveauVehicule.value("date_ajout").toString();
    data["temps_utilise"] = nouveauVehicule.value("temps_utilise").toDouble();
    data["code_unique"] = nouveauVehicule.value("code_unique").toString();

    entree["vehicule"] = data;
    historiqueArray.prepend(entree);

    // Limiter à 1000 entrées
    while (historiqueArray.size() > 1000) {
        historiqueArray.removeLast();
    }

    if (fichier.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(historiqueArray);
        fichier.write(doc.toJson(QJsonDocument::Indented));
        fichier.close();

        // Mettre à jour l'affichage HTML
        QTimer::singleShot(100, this, [this]() {
            genererHistoriqueHTML(getHistoriqueComplet());
        });

        qDebug() << "📝 Historique enregistré → Action: MODIFICATION"
                 << "| ID:" << idVehicule
                 << "| Changements:" << changements.size();
        qDebug() << "=== FIN SAUVEGARDE HISTORIQUE ===";
    } else {
        qDebug() << "❌ Erreur écriture fichier historique";
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

            // 🔴 RÉCUPÉRATION CORRECTE DES INFORMATIONS D'HISTORIQUE
            vehicule["historique_action"] = entree["action"].toString();
            vehicule["historique_timestamp"] = entree["timestamp"].toString();
            vehicule["historique_note"] = entree["note"].toString(); // NOTE DÉTAILLÉE DES CHANGEMENTS

            // Ajouter des informations supplémentaires pour faciliter le traitement
            QString action = entree["action"].toString();
            QString note = entree["note"].toString();

            // Compter le nombre de changements détectés
            if (action == "MODIFICATION") {
                if (note.contains("CHANGEMENTS DÉTECTÉS")) {
                    // Compter le nombre de lignes commençant par "•"
                    int nbChangements = note.count("• ");
                    vehicule["nombre_changements"] = nbChangements > 0 ? nbChangements : 0;
                    qDebug() << "  → Modification avec" << nbChangements << "changement(s) pour"
                             << vehicule["id_vehicule"].toString();
                } else {
                    vehicule["nombre_changements"] = 0;
                }
            } else if (action == "SUPPRESSION") {
                vehicule["nombre_changements"] = -1; // Indicateur pour suppression
            } else {
                vehicule["nombre_changements"] = 0;
            }

            historique.append(vehicule);
        }
        fichier.close();

        // Trier par timestamp décroissant (du plus récent au plus ancien)
        std::sort(historique.begin(), historique.end(), [](const QVariantMap &a, const QVariantMap &b) {
            QString timestampA = a.value("historique_timestamp").toString();
            QString timestampB = b.value("historique_timestamp").toString();
            return timestampA > timestampB; // Tri décroissant
        });

        qDebug() << "✅ Historique complet chargé et trié:" << historique.size() << "entrées";

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
        qDebug() << "Validation échouée";
        return;
    }

    QString codeUnique = ui->codeUniqueEdit->text().trimmed();
    if (codeUnique.isEmpty() || codeUnique == "AUCUN_CODE_DISPONIBLE") {
        QMessageBox::warning(this, "Code manquant", "Veuillez saisir un code unique valide.");
        return;
    }

    QString id = ui->idEdit->text().trimmed().toUpper();

    // Récupérer l'ANCIEN état AVANT la mise à jour
    QVariantMap ancienVehicule = DatabaseManager::instance().getTransportVehicule(id);
    bool isUpdate = !ancienVehicule.isEmpty();

    // === Création des données à partir du formulaire ===
    QVariantMap nouveauVehicule;
    nouveauVehicule["id_vehicule"] = id;
    nouveauVehicule["type"] = ui->typeCombo->currentText();
    nouveauVehicule["capacite"] = ui->capaciteEdit->text().trimmed().toInt();
    nouveauVehicule["zone"] = ui->zoneEdit->text().trimmed();
    nouveauVehicule["horaire"] = ui->horaireEdit->text().trimmed();
    nouveauVehicule["statut"] = ui->statutCombo->currentText();
    nouveauVehicule["date_ajout"] = ui->dateAjoutEdit->date().toString("yyyy-MM-dd");
    nouveauVehicule["temps_utilise"] = ui->tempsUtiliseEdit->text().trimmed().toDouble();
    nouveauVehicule["code_unique"] = codeUnique;

    bool success = false;

    if (isUpdate) {
        success = DatabaseManager::instance().updateTransportVehicule(id, nouveauVehicule);
        if (success) {
            // Sauvegarder l'historique avec comparaison ANCIEN vs NOUVEAU
            sauvegarderHistoriqueModification(ancienVehicule, nouveauVehicule);

            QMessageBox::information(this, "Succès",
                                     "Véhicule modifié avec succès !\nTous les changements sont enregistrés dans l'historique.");
        }
    } else {
        success = DatabaseManager::instance().addTransportVehicule(nouveauVehicule);
        if (success) {
            QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès !");
        }
    }

    if (success) {
        clearForm();
        loadAllVehicules();
        onTabList();

        // Notification maintenance si besoin
        QString statut = isUpdate ? nouveauVehicule["statut"].toString().toLower() : "";
        if (statut.contains("maintenance") && isNewMaintenanceEvent(id)) {
            m_knownMaintenanceVehicles.insert(id);
            showMaintenanceNotification(id);
            logMaintenanceEvent(id);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement !\n" +
                                                  DatabaseManager::instance().database().lastError().text());
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
    QString file = QFileDialog::getSaveFileName(this, "Exporter PDF - Véhicules en Maintenance", "vehicules_maintenance.pdf", "PDF (*.pdf)");
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

    QColor primaryColor(231, 76, 60); // Rouge pour maintenance
    QColor secondaryColor(241, 196, 15); // Orange
    QColor accentColor(230, 126, 34); // Orange foncé
    QColor lightGray(248, 249, 250);
    QColor darkGray(52, 58, 64);

    // En-tête avec fond rouge pour maintenance
    painter.fillRect(0, 0, pageWidth, 200, primaryColor);

    QFont titleFont("Arial", 24, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 50, pageWidth, 60), Qt::AlignCenter, "🚨 VÉHICULES EN MAINTENANCE");

    QFont subtitleFont("Arial", 14);
    painter.setFont(subtitleFont);
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(QRect(0, 110, pageWidth, 40), Qt::AlignCenter, "Rapport des véhicules nécessitant une intervention");

    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(QRect(0, 150, pageWidth, 30), Qt::AlignCenter, dateStr);

    int yPos = 220;

    // Filtrer uniquement les véhicules en maintenance
    QTableWidget *t = ui->vehiculesTable;
    int totalMaintenance = 0;
    QList<int> maintenanceRows;

    for (int r = 0; r < t->rowCount(); ++r) {
        if (t->isRowHidden(r)) continue;

        QString statut = t->item(r, 5) ? t->item(r, 5)->text().toLower() : "";
        if (statut.contains("maintenance")) {
            totalMaintenance++;
            maintenanceRows.append(r);
        }
    }

    // Statistiques uniquement pour la maintenance
    int boxWidth = (pageWidth - 40) / 2;
    int boxHeight = 80;

    painter.fillRect(20, yPos, boxWidth, boxHeight, primaryColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(20, yPos, boxWidth, 30), Qt::AlignCenter, "VÉHICULES EN MAINTENANCE");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(20, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(totalMaintenance));

    // Calcul du kilométrage total des véhicules en maintenance
    double totalKmMaintenance = 0;
    for (int row : maintenanceRows) {
        if (t->item(row, 8)) {
            totalKmMaintenance += t->item(row, 8)->text().toDouble();
        }
    }

    painter.fillRect(30 + boxWidth, yPos, boxWidth, boxHeight, accentColor);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos, boxWidth, 30), Qt::AlignCenter, "KILOMÉTRAGE TOTAL");
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(QRect(30 + boxWidth, yPos + 30, boxWidth, 50), Qt::AlignCenter, QString::number(totalKmMaintenance, 'f', 1) + " km");

    yPos += boxHeight + 30;

    if (totalMaintenance > 0) {
        // En-têtes du tableau avec seulement les colonnes demandées + kilométrage
        QStringList headers = {"ID Véhicule", "Type", "Capacité", "Kilométrage (km)"};

        QList<int> columnWidths = {
            pageWidth * 25 / 100,  // ID
            pageWidth * 25 / 100,  // Type
            pageWidth * 25 / 100,  // Capacité
            pageWidth * 25 / 100   // Kilométrage
        };

        // En-tête du tableau
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

        // Données des véhicules en maintenance
        painter.setFont(QFont("Arial", 9));
        int rowHeight = 35;
        int rowsPerPage = (pageHeight - yPos - 50) / rowHeight;
        int currentRow = 0;

        for (int rowIndex : maintenanceRows) {
            if (currentRow >= rowsPerPage) {
                writer.newPage();
                yPos = 50;
                currentRow = 0;

                // Redessiner l'en-tête du tableau sur la nouvelle page
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

            // Alternance des couleurs de ligne
            if (currentRow % 2 == 0) {
                painter.fillRect(0, yPos, pageWidth, rowHeight, lightGray);
            } else {
                painter.fillRect(0, yPos, pageWidth, rowHeight, Qt::white);
            }

            xPos = 0;
            painter.setPen(Qt::black);

            // Colonne 0: ID Véhicule
            QString id = t->item(rowIndex, 0) ? t->item(rowIndex, 0)->text() : "";
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            QRect idRect(xPos + 5, yPos, columnWidths[0] - 10, rowHeight);
            painter.drawText(idRect, Qt::AlignLeft | Qt::AlignVCenter, id);
            xPos += columnWidths[0];

            // Colonne 1: Type
            QString type = t->item(rowIndex, 1) ? t->item(rowIndex, 1)->text() : "";
            painter.setFont(QFont("Arial", 9));
            QRect typeRect(xPos + 5, yPos, columnWidths[1] - 10, rowHeight);
            painter.drawText(typeRect, Qt::AlignLeft | Qt::AlignVCenter, type);
            xPos += columnWidths[1];

            // Colonne 2: Capacité
            QString capacite = t->item(rowIndex, 2) ? t->item(rowIndex, 2)->text() : "";
            QRect capaciteRect(xPos + 5, yPos, columnWidths[2] - 10, rowHeight);
            painter.drawText(capaciteRect, Qt::AlignCenter | Qt::AlignVCenter, capacite);
            xPos += columnWidths[2];

            // Colonne 3: Kilométrage
            QString kilometrage = t->item(rowIndex, 8) ? t->item(rowIndex, 8)->text() : "0.0";
            painter.setFont(QFont("Arial", 9, QFont::Bold));
            painter.setPen(accentColor);
            QRect kmRect(xPos + 5, yPos, columnWidths[3] - 10, rowHeight);
            painter.drawText(kmRect, Qt::AlignRight | Qt::AlignVCenter, kilometrage + " km");

            // Lignes de séparation
            painter.setPen(QPen(QColor(200, 200, 200), 1));
            for (int i = 0; i <= headers.size(); ++i) {
                int lineX = i * (pageWidth / headers.size());
                painter.drawLine(lineX, yPos, lineX, yPos + rowHeight);
            }
            painter.drawLine(0, yPos + rowHeight, pageWidth, yPos + rowHeight);

            yPos += rowHeight;
            currentRow++;
        }
    } else {
        // Aucun véhicule en maintenance
        painter.setPen(darkGray);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(QRect(0, yPos, pageWidth, 100), Qt::AlignCenter, "✅ Aucun véhicule en maintenance");

        painter.setFont(QFont("Arial", 12));
        painter.setPen(QColor(100, 100, 100));
        painter.drawText(QRect(0, yPos + 50, pageWidth, 50), Qt::AlignCenter,
                         "Tous les véhicules sont opérationnels");
    }

    // Pied de page
    int footerY = pageHeight - 40;
    painter.setPen(QColor(150, 150, 150));
    painter.setFont(QFont("Arial", 8));
    painter.drawLine(0, footerY, pageWidth, footerY);
    painter.drawText(QRect(0, footerY + 10, pageWidth / 2, 30),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "NEXORA - Rapport Maintenance Véhicules");
    painter.drawText(QRect(0, footerY + 10, pageWidth, 30),
                     Qt::AlignRight | Qt::AlignVCenter,
                     QString("Page 1 • Généré le %1").arg(
                         QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm")));

    painter.end();

    // Message de confirmation
    if (totalMaintenance > 0) {
        QMessageBox::information(this, "Export PDF - Maintenance",
                                 QString("🚨 PDF des véhicules en maintenance exporté avec succès !\n\n"
                                         "• Véhicules en maintenance: %1\n"
                                         "• Kilométrage total: %2 km\n\n"
                                         "📁 Fichier: %3")
                                     .arg(totalMaintenance)
                                     .arg(totalKmMaintenance, 0, 'f', 1)
                                     .arg(file));
    } else {
        QMessageBox::information(this, "Export PDF - Maintenance",
                                 QString("✅ Aucun véhicule en maintenance\n\n"
                                         "📁 Fichier: %1")
                                     .arg(file));
    }
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
        // 🔴 SAUVEGARDER LA SUPPRESSION DANS L'HISTORIQUE
        if (!vehicule.isEmpty()) {
            sauvegarderHistorique(vehicule, "SUPPRESSION");
            qDebug() << "📝 Historique: Suppression enregistrée pour" << id;
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
    // CORRIGÉ : Pour QLineEdit, utilisez clear()
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
    // ✅ Changer le placeholder pour montrer la flexibilité
    ui->idEdit->setPlaceholderText("Ex: VH001, BUS001, CAR123");
    ui->capaciteEdit->setPlaceholderText("Ex: 4, 20, 5");
    ui->codeUniqueEdit->setPlaceholderText("Code finance (ex: TRANS001)");

    QRegularExpressionValidator *horaireValidator = new QRegularExpressionValidator(
        QRegularExpression("^([0-1]?[0-9]|2[0-3]):[0-5][0-9]-([0-1]?[0-9]|2[0-3]):[0-5][0-9]$"), this);
    ui->horaireEdit->setValidator(horaireValidator);
    ui->horaireEdit->setPlaceholderText("HH:MM-HH:MM (ex: 08:00-18:00)");

    connect(ui->idEdit, &QLineEdit::textChanged, this, &TransportPage::onIdTextChanged);
    connect(ui->capaciteEdit, &QLineEdit::textChanged, this, &TransportPage::onCapaciteTextChanged);
    connect(ui->horaireEdit, &QLineEdit::textChanged, this, &TransportPage::onHoraireTextChanged);

    ui->dateAjoutEdit->setDate(QDate::currentDate());
}

bool TransportPage::validateForm()
{
    if (!validateId(ui->idEdit->text())) {
        // ✅ Message d'erreur plus générique
        showError("L'ID doit commencer par une lettre et contenir seulement des lettres et chiffres");
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
    QString codeUnique = ui->codeUniqueEdit->text().trimmed();
    if (codeUnique.isEmpty()) {
        showError("Veuillez saisir un code unique");
        return false;
    }

    return true;
}
bool TransportPage::validateId(const QString &id)
{
    QString trimmed = id.trimmed().toUpper();

    // ✅ Accepter n'importe quelle lettre au début (au moins 1 lettre)
    QRegularExpression regex("^[A-Z]{1,}[A-Z0-9]*$");
    QRegularExpressionMatch match = regex.match(trimmed);

    return match.hasMatch() && trimmed.length() >= 2;
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

    QString codeUnique = v.value("code_unique").toString();
    ui->codeUniqueEdit->setText(codeUnique);
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

    // Afficher aussi le dialogue normal
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("📜 Historique Complet des Véhicules");
    dialog->setMinimumSize(1400, 700);
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
    table->setColumnCount(13);
    table->setHorizontalHeaderLabels({
        "Action", "Date/Heure", "ID Véhicule", "Type", "Capacité", "Zone",
        "Horaire", "Statut", "Date Ajout", "Temps Utilisé (h)", "Kilométrage (km)",
        "Code Unique", "📝 Note des Changements"
    });

    table->setRowCount(historique.size());

    for (int i = 0; i < historique.size(); ++i) {
        const auto &v = historique[i];
        QString type = v.value("type").toString();
        double tempsUtilise = v.value("temps_utilise").toDouble();
        double kilometrage = calculerKilometrage(type, tempsUtilise);
        QString action = v.value("historique_action").toString();
        QString timestamp = v.value("historique_timestamp").toString();
        QString note = v.value("historique_note").toString();

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

        // Colonne Note avec tooltip
        QTableWidgetItem *noteItem = new QTableWidgetItem(note.isEmpty() ? "Aucune note" : note);
        noteItem->setToolTip(note);
        QFont noteFont = noteItem->font();
        noteFont.setItalic(true);
        noteItem->setFont(noteFont);
        noteItem->setForeground(QColor(74, 85, 104));
        table->setItem(i, 12, noteItem);

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
    table->setColumnWidth(12, 300); // Largeur fixe pour la colonne Note
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
