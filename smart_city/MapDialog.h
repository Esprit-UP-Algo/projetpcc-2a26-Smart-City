#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QMap>
#include <QString>
#include <QRect>
#include <QPoint>
#include <QColor>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>  // ← AJOUT
#include <QFont>        // ← AJOUT

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);
    void addBuilding(const QString &name, const QRect &rect);
    void addIncident(const QString &localisation, const QString &type = "");

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPixmap mapImage;
    QPixmap incidentIcon;
    QMap<QString, QRect> buildings;
    QList<QPoint> incidentPositions;
    QList<QColor> incidentColors;
    QList<QString> incidentTypes;
    QList<QString> incidentLocalisations;

    // Navigation
    double scaleFactor;
    QPoint lastMousePos;
    QPoint mapOffset;
    bool isDragging;

    QString detectBuildingFromLocalisation(const QString &localisation);
    void drawMap(QPainter &painter);

    // Méthodes de transformation de coordonnées
    QPoint toMapCoordinates(const QPoint &widgetPos);
    QPoint toWidgetCoordinates(const QPoint &mapPos);

    // NOUVELLE MÉTHODE POUR CLIC SUR INCIDENT
    int getIncidentAtPosition(const QPoint &pos);  // ← AJOUT
};

#endif // MAPDIALOG_H
