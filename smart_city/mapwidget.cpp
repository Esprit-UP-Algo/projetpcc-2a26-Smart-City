#include "mapwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

MapWidget::MapWidget(QWidget *parent) : QWidget(parent)
{
    // Charge l'image depuis les ressources
    mapImage.load(":/resources/map.png");
    setMinimumSize(mapImage.size());
}

void MapWidget::addBuilding(const QString &name, const QRect &rect)
{
    buildings[name] = rect;
    update(); // Redessine
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, mapImage); // Dessine la map

    // Dessine les bâtiments (en semi-transparent rouge)
    painter.setBrush(QColor(255, 0, 0, 100));
    painter.setPen(Qt::red);
    for (auto it = buildings.begin(); it != buildings.end(); ++it) {
        painter.drawRect(it.value());
    }
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    for (auto it = buildings.begin(); it != buildings.end(); ++it) {
        if (it.value().contains(pos)) {
            emit buildingClicked(it.key());
            break;
        }
    }
}
