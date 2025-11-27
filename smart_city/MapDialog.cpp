#include "MapDialog.h"
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include <cmath>

MapDialog::MapDialog(QWidget *parent)
    : QDialog(parent), scaleFactor(0.6), isDragging(false)
{
    setWindowTitle("Carte des incidents - NEXORA (Navigation: Molette = Zoom, Clic = Déplacer)");

    // Agrandir la fenêtre pour mieux voir la map
    setFixedSize(900, 600);

    // Charger la map
    mapImage.load(":/resources/map.png");
    if (mapImage.isNull()) {
        qDebug() << "❌ Map image not loaded!";
        // Créer une image de test si la map n'est pas trouvée
        mapImage = QPixmap(800, 600);
        mapImage.fill(QColor(200, 220, 255));
        QPainter p(&mapImage);
        p.setPen(Qt::blue);
        p.drawText(mapImage.rect(), Qt::AlignCenter, "MAP NEXORA\n(Texte de remplacement)");
    } else {
        qDebug() << "✅ Map image loaded - Original size:" << mapImage.size();
    }

    // Centrer la map initialement pour qu'elle remplisse mieux la fenêtre
    if (!mapImage.isNull()) {
        mapOffset.setX((width() - mapImage.width() * scaleFactor) / 2);
        mapOffset.setY((height() - mapImage.height() * scaleFactor) / 2);
    }

    setMouseTracking(true);
}

void MapDialog::addBuilding(const QString &name, const QRect &rect)
{
    buildings[name] = rect;
    qDebug() << "🏢 Bâtiment ajouté:" << name << rect;
}

QPixmap MapDialog::getIconForType(const QString &type, const QColor &color)
{
    QPixmap icon(32, 32);
    icon.fill(Qt::transparent);

    QPainter painter(&icon);
    painter.setRenderHint(QPainter::Antialiasing);

    QString typeLower = type.toLower();

    if (typeLower.contains("électri") || typeLower.contains("electric") || typeLower.contains("panne")) {
        // Icône électricité (éclair)
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 1));
        QPolygon lightning;
        lightning << QPoint(16, 6) << QPoint(22, 16) << QPoint(18, 16)
                  << QPoint(26, 26) << QPoint(16, 18) << QPoint(20, 18);
        painter.drawPolygon(lightning);
    }
    else if (typeLower.contains("eau") || typeLower.contains("fuite")) {
        // Icône eau (goutte)
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 1));
        painter.drawEllipse(8, 8, 16, 16);
        QPolygon drop;
        drop << QPoint(16, 24) << QPoint(12, 20) << QPoint(20, 20);
        painter.drawPolygon(drop);
    }
    else if (typeLower.contains("bruit") || typeLower.contains("excessif")) {
        // Icône bruit (ondes sonores)
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(color, 2));
        painter.drawEllipse(8, 8, 16, 16);
        painter.drawEllipse(4, 4, 24, 24);
        painter.drawEllipse(0, 0, 32, 32);
    }
    else if (typeLower.contains("ascenseur")) {
        // Icône ascenseur (rectangle avec flèches)
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(8, 6, 16, 20);
        // Flèches haut/bas
        painter.setPen(QPen(Qt::white, 2));
        painter.drawLine(16, 10, 12, 14);
        painter.drawLine(16, 10, 20, 14);
        painter.drawLine(16, 22, 12, 18);
        painter.drawLine(16, 22, 20, 18);
    }
    else {
        // Icône par défaut (alerte)
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 1));
        painter.drawEllipse(4, 4, 24, 24);
        painter.setPen(QPen(Qt::white, 2));
        painter.drawLine(16, 10, 16, 18);
        painter.drawLine(16, 22, 16, 22);
    }

    return icon;
}

QColor MapDialog::getColorForType(const QString &type)
{
    QString typeLower = type.toLower();

    if (typeLower.contains("électri") || typeLower.contains("electric")) {
        return QColor(255, 165, 0);    // Orange
    }
    else if (typeLower.contains("eau") || typeLower.contains("fuite")) {
        return QColor(0, 100, 255);    // Bleu
    }
    else if (typeLower.contains("bruit") || typeLower.contains("excessif")) {
        return QColor(255, 0, 255);    // Magenta
    }
    else if (typeLower.contains("ascenseur")) {
        return QColor(128, 0, 128);    // Violet
    }
    else {
        return Qt::red;                // Rouge par défaut
    }
}

QPoint MapDialog::calculateIncidentPosition(const QString &buildingName, int incidentIndex, int totalIncidents)
{
    if (!buildings.contains(buildingName)) {
        return QPoint(100, 100); // Position par défaut
    }

    QRect buildingRect = buildings[buildingName];
    QPoint center = buildingRect.center();

    // Si un seul incident, le mettre au centre
    if (totalIncidents == 1) {
        return center;
    }

    // Pour plusieurs incidents, les disposer en cercle autour du centre
    double angle = (2 * M_PI * incidentIndex) / totalIncidents;
    int radius = qMin(buildingRect.width(), buildingRect.height()) / 3;

    int x = center.x() + radius * cos(angle);
    int y = center.y() + radius * sin(angle);

    return QPoint(x, y);
}

void MapDialog::addIncident(const QString &localisation, const QString &type)
{
    QString buildingName = detectBuildingFromLocalisation(localisation);

    if (buildings.contains(buildingName)) {
        // Compter les incidents existants dans ce bâtiment
        int incidentsInBuilding = 0;
        for (const Incident& incident : incidents) {
            QString incidentBuilding = detectBuildingFromLocalisation(incident.localisation);
            if (incidentBuilding == buildingName) {
                incidentsInBuilding++;
            }
        }

        // Calculer la position
        QPoint position = calculateIncidentPosition(buildingName, incidentsInBuilding, incidentsInBuilding + 1);

        // Créer l'incident
        Incident incident;
        incident.position = position;
        incident.color = getColorForType(type);
        incident.type = type;
        incident.localisation = localisation;
        incident.icon = getIconForType(type, incident.color);

        incidents.append(incident);

        qDebug() << "📍 Incident ajouté:" << type << "dans" << buildingName
                 << "à la position" << position << "(incident" << incidentsInBuilding + 1 << "dans ce bâtiment)";
    } else {
        qDebug() << "❌ Bâtiment non trouvé pour l'incident:" << buildingName;
    }

    update();
}

QString MapDialog::detectBuildingFromLocalisation(const QString &localisation)
{
    QString locLower = localisation.toLower();

    if (locLower.contains("batiment 1") || locLower.contains("bâtiment 1") || locLower.contains("bat1") || locLower.contains("bât1")) {
        return "Batiment 1";
    }
    else if (locLower.contains("batiment 2") || locLower.contains("bâtiment 2") || locLower.contains("bat2") || locLower.contains("bât2")) {
        return "Batiment 2";
    }
    else if (locLower.contains("batiment 3") || locLower.contains("bâtiment 3") || locLower.contains("bat3") || locLower.contains("bât3")) {
        return "Batiment 3";
    }
    else if (locLower.contains("batiment 4") || locLower.contains("bâtiment 4") || locLower.contains("bat4") || locLower.contains("bât4")) {
        return "Batiment 4";
    }
    else if (locLower.contains("parking")) {
        return "Parking 1";
    }

    return "Batiment 1"; // Bâtiment par défaut
}

// Transformation des coordonnées
QPoint MapDialog::toMapCoordinates(const QPoint &widgetPos)
{
    return QPoint((widgetPos.x() - mapOffset.x()) / scaleFactor,
                  (widgetPos.y() - mapOffset.y()) / scaleFactor);
}

QPoint MapDialog::toWidgetCoordinates(const QPoint &mapPos)
{
    return QPoint(mapPos.x() * scaleFactor + mapOffset.x(),
                  mapPos.y() * scaleFactor + mapOffset.y());
}

void MapDialog::drawMap(QPainter &painter)
{
    if (mapImage.isNull()) return;

    // Dessiner la map avec le zoom et décalage
    QRectF targetRect(mapOffset.x(), mapOffset.y(),
                      mapImage.width() * scaleFactor,
                      mapImage.height() * scaleFactor);

    painter.drawPixmap(targetRect, mapImage, mapImage.rect());

    // AFFICHER LES NOMS DES BÂTIMENTS
    painter.setPen(QPen(Qt::darkBlue, 2));
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    for (auto it = buildings.begin(); it != buildings.end(); ++it) {
        QString name = it.key();
        QRect rect = it.value();

        QPoint widgetPos = toWidgetCoordinates(QPoint(rect.center().x(), rect.bottom() - 10));
        painter.drawText(widgetPos.x() - 40, widgetPos.y() + 15, name);
    }

    // Dessiner les incidents avec leurs icônes spécifiques (SANS TEXTE)
    for (int i = 0; i < incidents.size(); ++i) {
        const Incident& incident = incidents[i];
        QPoint widgetPos = toWidgetCoordinates(incident.position);

        // Taille de l'icône adaptée au zoom
        int iconSize = 32 * scaleFactor;
        if (iconSize < 20) iconSize = 20;  // Taille minimum augmentée
        if (iconSize > 50) iconSize = 50;  // Taille maximum

        // Dessiner l'icône
        QPixmap scaledIcon = incident.icon.scaled(iconSize, iconSize,
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation);
        painter.drawPixmap(widgetPos.x() - iconSize/2,
                           widgetPos.y() - iconSize/2,
                           scaledIcon);

        // Cercle coloré autour de l'icône (plus visible)
        painter.setBrush(QColor(incident.color.red(), incident.color.green(),
                                incident.color.blue(), 80));  // Opacité augmentée
        painter.setPen(QPen(incident.color, 2));
        painter.drawEllipse(widgetPos.x() - iconSize/2 - 6,
                            widgetPos.y() - iconSize/2 - 6,
                            iconSize + 12, iconSize + 12);

        // SUPPRIMÉ : Affichage du texte du type
        // Maintenant on affiche seulement l'icône avec son cercle coloré
    }
}

void MapDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Fond gris pour les zones hors map
    painter.fillRect(rect(), QColor(240, 240, 240));

    drawMap(painter);

    // LÉGENDE AMÉLIORÉE
    painter.setBrush(QColor(255, 255, 255, 230));
    painter.setPen(Qt::NoPen);
    painter.drawRect(15, 15, 140, 160);

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.drawText(20, 30, "LÉGENDE:");

    painter.setFont(QFont("Arial", 9));
    int y = 50;

    QList<QPair<QString, QColor>> legend = {
        {"⚡ Électrique", QColor(255, 165, 0)},
        {"💧 Eau", QColor(0, 100, 255)},
        {"🔊 Bruit", QColor(255, 0, 255)},
        {"🛗 Ascenseur", QColor(128, 0, 128)},
        {"⚠️ Autre", Qt::red}
    };

    for (const auto &item : legend) {
        painter.setBrush(item.second);
        painter.setPen(Qt::black);
        painter.drawEllipse(20, y, 12, 12);
        painter.setPen(Qt::black);
        painter.drawText(40, y + 10, item.first);
        y += 22;
    }

    // Indicateur de zoom
    painter.setPen(Qt::black);
    painter.drawText(width() - 120, 30,
                     QString("🔍 Zoom: %1%").arg(int(scaleFactor * 100)));

    // Instructions
    painter.drawText(width() - 250, height() - 20,
                     "Molette = Zoom • Clic gauche = Déplacer • Clic icône = Détails");
}

// NAVIGATION
void MapDialog::wheelEvent(QWheelEvent *event)
{
    double zoomFactor = 1.1;
    QPoint mousePos = event->position().toPoint();
    QPoint mapPosBeforeZoom = toMapCoordinates(mousePos);

    if (event->angleDelta().y() > 0) {
        scaleFactor *= zoomFactor;
        if (scaleFactor > 3.0) scaleFactor = 3.0; // Zoom max 300%
    } else {
        scaleFactor /= zoomFactor;
        if (scaleFactor < 0.2) scaleFactor = 0.2; // Zoom min 20%
    }

    QPoint mapPosAfterZoom = toMapCoordinates(mousePos);
    mapOffset += (mapPosAfterZoom - mapPosBeforeZoom) * scaleFactor;

    update();
    event->accept();
}

int MapDialog::getIncidentAtPosition(const QPoint &pos)
{
    for (int i = 0; i < incidents.size(); ++i) {
        QPoint widgetPos = toWidgetCoordinates(incidents[i].position);

        // Vérifier si le clic est sur l'icône
        int iconSize = 32 * scaleFactor;
        if (iconSize < 20) iconSize = 20;
        if (iconSize > 50) iconSize = 50;

        QRect iconRect(widgetPos.x() - iconSize/2, widgetPos.y() - iconSize/2,
                       iconSize, iconSize);

        if (iconRect.contains(pos)) {
            return i;
        }
    }
    return -1;
}

void MapDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        int incidentIndex = getIncidentAtPosition(event->pos());

        if (incidentIndex != -1) {
            const Incident& incident = incidents[incidentIndex];
            QString building = detectBuildingFromLocalisation(incident.localisation);

            QString details = QString(
                                  "🔍 DÉTAILS DE L'INCIDENT\n\n"
                                  "📍 Localisation: %1\n"
                                  "🚨 Type: %2\n"
                                  "🏢 Bâtiment: %3\n"
                                  "🎯 Position: %4\n"
                                  "📊 Statut: Actif\n\n"
                                  "Couleur: %5"
                                  ).arg(
                                      incident.localisation,
                                      incident.type,
                                      building,
                                      QString("(%1, %2)").arg(incident.position.x()).arg(incident.position.y()),
                                      incident.color.name()
                                      );

            QMessageBox::information(this, "Détails de l'incident", details);
        } else {
            isDragging = true;
            lastMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

void MapDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging) {
        isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
}

void MapDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        QPoint delta = event->pos() - lastMousePos;
        mapOffset += delta;
        lastMousePos = event->pos();
        update();
    }
}
