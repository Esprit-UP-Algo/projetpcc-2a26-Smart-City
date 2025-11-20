#include "MapDialog.h"
#include <QPainter>

MapDialog::MapDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Carte des incidents");

    // Taille raisonnable au centre : PAS PLEIN ÉCRAN
    setFixedSize(900, 600);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // Charger la map
    mapImage.load(":/resources/map.png");

    // Charger l’icône incident
    incidentIcon.load(":/resources/incident.png");
    incidentIcon = incidentIcon.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void MapDialog::addBuilding(const QString &name, const QRect &rect)
{
    buildings[name] = rect;
}

void MapDialog::addIncident(const QString &buildingName)
{
    if (buildings.contains(buildingName))
    {
        QRect r = buildings[buildingName];

        // Positionner l’icône au centre du bâtiment
        QPoint center(r.x() + r.width() / 2 - 20, r.y() + r.height() / 2 - 20);

        incidentPositions.append(center);
    }

    update();
}

void MapDialog::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    // Afficher la map ajustée à la taille du dialog
    p.drawPixmap(rect(), mapImage);

    // Afficher les icônes incidents
    for (const QPoint &pos : incidentPositions)
    {
        p.drawPixmap(pos, incidentIcon);
    }
}
