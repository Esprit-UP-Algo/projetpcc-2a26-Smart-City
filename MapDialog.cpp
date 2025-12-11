#include "MapDialog.h"
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>

MapDialog::MapDialog(QWidget *parent)
    : QDialog(parent), scaleFactor(0.3), isDragging(false)
{
    setWindowTitle("Carte des incidents - NEXORA (Navigation: Molette = Zoom, Clic = Déplacer)");

    // Taille fixe raisonnable pour la fenêtre
    setFixedSize(800, 600);

    // Charger la map
    mapImage.load(":/resources/map.png");
    if (mapImage.isNull()) {
        qDebug() << "❌ Map image not loaded!";
    } else {
        qDebug() << "✅ Map image loaded - Original size:" << mapImage.size();
    }

    // Charger l'icône incident
    incidentIcon.load(":/resources/incident.png");
    if (!incidentIcon.isNull()) {
        incidentIcon = incidentIcon.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        qDebug() << "✅ Incident icon loaded";
    }

    // Centrer la map initialement
    if (!mapImage.isNull()) {
        mapOffset.setX((width() - mapImage.width() * scaleFactor) / 2);
        mapOffset.setY((height() - mapImage.height() * scaleFactor) / 2);
    }

    setMouseTracking(true);
}

void MapDialog::addBuilding(const QString &name, const QRect &rect)
{
    buildings[name] = rect;
}

void MapDialog::addIncident(const QString &localisation, const QString &type)
{
    QString buildingName = detectBuildingFromLocalisation(localisation);

    if (buildings.contains(buildingName))
    {
        QRect r = buildings[buildingName];
        QPoint center(r.x() + r.width() / 2 - 20, r.y() + r.height() / 2 - 20);
        incidentPositions.append(center);

        QColor color = Qt::red;
        QString typeLower = type.toLower();

        if (typeLower.contains("électri") || typeLower.contains("electric")) {
            color = QColor(255, 165, 0);
        }
        else if (typeLower.contains("eau") || typeLower.contains("fuite")) {
            color = QColor(0, 100, 255);
        }
        else if (typeLower.contains("bruit") || typeLower.contains("excessif")) {
            color = QColor(255, 0, 255);
        }
        else if (typeLower.contains("ascenseur")) {
            color = QColor(128, 0, 128);
        }
        else {
            color = Qt::red;
        }

        incidentColors.append(color);
        incidentTypes.append(type);
        incidentLocalisations.append(localisation);
    }

    update();
}

QString MapDialog::detectBuildingFromLocalisation(const QString &localisation)
{
    QString locLower = localisation.toLower();
    qDebug() << "Détection bâtiment pour:" << localisation;

    // Détection plus flexible
    if (locLower.contains("batiment 1") || locLower.contains("bâtiment 1") || locLower.contains("bat1") || locLower.contains("bât1")) {
        qDebug() << "→ Bâtiment 1 détecté";
        return "Batiment 1";
    }
    else if (locLower.contains("batiment 2") || locLower.contains("bâtiment 2") || locLower.contains("bat2") || locLower.contains("bât2")) {
        qDebug() << "→ Bâtiment 2 détecté";
        return "Batiment 2";
    }
    else if (locLower.contains("batiment 3") || locLower.contains("bâtiment 3") || locLower.contains("bat3") || locLower.contains("bât3")) {
        qDebug() << "→ Bâtiment 3 détecté";
        return "Batiment 3";
    }
    else if (locLower.contains("batiment 4") || locLower.contains("bâtiment 4") || locLower.contains("bat4") || locLower.contains("bât4")) {
        qDebug() << "→ Bâtiment 4 détecté";
        return "Batiment 4";
    }
    else if (locLower.contains("parking")) {
        qDebug() << "→ Parking 1 détecté";
        return "Parking 1";
    }

    qDebug() << "→ Aucun bâtiment spécifique détecté, utilisation par défaut: Batiment 1";
    return "Batiment 1";
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

    // AFFICHER LES NOMS DES BÂTIMENTS (SANS BACKGROUND)
    painter.setPen(QPen(Qt::darkBlue, 2)); // Texte bleu foncé
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    for (auto it = buildings.begin(); it != buildings.end(); ++it) {
        QString name = it.key();
        QRect rect = it.value();

        // Calculer la position du texte (centré en bas du bâtiment)
        QPoint widgetPos = toWidgetCoordinates(QPoint(rect.center().x(), rect.bottom() - 10));

        // Dessiner le texte SANS fond
        painter.drawText(widgetPos.x() - 40, widgetPos.y() + 15, name);
    }

    // Dessiner les incidents
    for (int i = 0; i < incidentPositions.size(); ++i) {
        QPoint widgetPos = toWidgetCoordinates(incidentPositions[i]);
        QColor color = incidentColors[i];
        QString type = incidentTypes[i];

        if (!incidentIcon.isNull()) {
            // Ajuster la taille de l'icône selon le zoom
            int iconSize = 40 * scaleFactor;
            if (iconSize < 20) iconSize = 20;
            if (iconSize > 60) iconSize = 60;

            QPixmap scaledIcon = incidentIcon.scaled(iconSize, iconSize,
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation);
            painter.drawPixmap(widgetPos.x() - iconSize/2,
                               widgetPos.y() - iconSize/2,
                               scaledIcon);

            // Cercle coloré
            painter.setBrush(QColor(color.red(), color.green(), color.blue(), 80));
            painter.setPen(QPen(color, 2));
            painter.drawEllipse(widgetPos.x() - iconSize/2 - 5,
                                widgetPos.y() - iconSize/2 - 5,
                                iconSize + 10, iconSize + 10);
        }

        // Texte seulement si zoom suffisant
        if (scaleFactor > 0.2) {
            // TEXTE DES INCIDENTS AVEC FOND (pour lisibilité)
            painter.setBrush(QColor(255, 255, 255, 200));
            painter.setPen(Qt::NoPen);
            painter.drawRect(widgetPos.x() - 20, widgetPos.y() + 25,
                             type.length() * 7, 18);

            painter.setPen(Qt::black);
            painter.setFont(QFont("Arial", 8, QFont::Bold));
            painter.drawText(widgetPos.x() - 15, widgetPos.y() + 35, type);
        }
    }
}
void MapDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // Fond gris pour les zones hors map
    painter.fillRect(rect(), QColor(240, 240, 240));

    drawMap(painter);

    // LÉGENDE (toujours visible)
    painter.setBrush(QColor(255, 255, 255, 220));
    painter.setPen(Qt::NoPen);
    painter.drawRect(15, 15, 120, 130);

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.drawText(20, 30, "LÉGENDE:");

    painter.setFont(QFont("Arial", 9));
    int y = 50;

    QList<QPair<QString, QColor>> legend = {
        {"Électrique", QColor(255, 165, 0)},
        {"Eau", QColor(0, 100, 255)},
        {"Bruit", QColor(255, 0, 255)},
        {"Ascenseur", QColor(128, 0, 128)},
        {"Autre", Qt::red}
    };

    for (const auto &item : legend) {
        painter.setBrush(item.second);
        painter.setPen(Qt::black);
        painter.drawEllipse(20, y, 12, 12);
        painter.setPen(Qt::black);
        painter.drawText(40, y + 10, item.first);
        y += 20;
    }

    // Indicateur de zoom
    painter.setPen(Qt::black);
    painter.drawText(width() - 100, 30,
                     QString("Zoom: %1%").arg(int(scaleFactor * 100)));
}

// NAVIGATION
void MapDialog::wheelEvent(QWheelEvent *event)
{
    double zoomFactor = 1.1;
    QPoint mousePos = event->position().toPoint();
    QPoint mapPosBeforeZoom = toMapCoordinates(mousePos);

    if (event->angleDelta().y() > 0) {
        // Zoom in
        scaleFactor *= zoomFactor;
        if (scaleFactor > 2.0) scaleFactor = 2.0; // Zoom max 200%
    } else {
        // Zoom out
        scaleFactor /= zoomFactor;
        if (scaleFactor < 0.1) scaleFactor = 0.1; // Zoom min 10%
    }

    // Recentrer sur la position de la souris
    QPoint mapPosAfterZoom = toMapCoordinates(mousePos);
    mapOffset += (mapPosAfterZoom - mapPosBeforeZoom) * scaleFactor;

    update();
    event->accept();
}

// Détecte quel incident est cliqué
int MapDialog::getIncidentAtPosition(const QPoint &pos)
{
    for (int i = 0; i < incidentPositions.size(); ++i) {
        QPoint widgetPos = toWidgetCoordinates(incidentPositions[i]);

        // Vérifier si le clic est sur l'icône (cercle de 40px)
        int distance = QLineF(pos, widgetPos).length();
        if (distance < 20) {
            return i;
        }
    }
    return -1; // Aucun incident cliqué
}

// Gestion du clic souris
void MapDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // Vérifier si un incident est cliqué
        int incidentIndex = getIncidentAtPosition(event->pos());

        if (incidentIndex != -1) {
            // Afficher les détails de l'incident
            QString details = QString(
                                  "🔍 DÉTAILS DE L'INCIDENT\n\n"
                                  "📍 Localisation: %1\n"
                                  "🚨 Type: %2\n"
                                  "🏢 Bâtiment: %3\n"
                                  "🎨 Statut: Actif"
                                  ).arg(
                                      incidentLocalisations[incidentIndex],
                                      incidentTypes[incidentIndex],
                                      detectBuildingFromLocalisation(incidentLocalisations[incidentIndex])
                                      );

            QMessageBox::information(this, "Détails de l'incident", details);
        } else {
            // Navigation normale (déplacement de la carte)
            isDragging = true;
            lastMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        }
    }
}

// Modifiez aussi mouseReleaseEvent
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
