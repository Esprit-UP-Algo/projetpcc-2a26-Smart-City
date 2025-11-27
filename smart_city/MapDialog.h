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
#include <QFont>

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);
    void addBuilding(const QString &name, const QRect &rect);
    void addIncident(const QString &localisation, const QString &type);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Structures pour stocker les données
    struct Incident {
        QPoint position;
        QColor color;
        QString type;
        QString localisation;
        QPixmap icon;
    };

    // Membres existants
    QPixmap mapImage;
    QMap<QString, QRect> buildings;
    QVector<Incident> incidents;  // REMPLACEZ les anciens vecteurs

    double scaleFactor;
    QPoint mapOffset;
    bool isDragging;
    QPoint lastMousePos;

    // Nouvelles fonctions
    QString detectBuildingFromLocalisation(const QString &localisation);
    QPoint toMapCoordinates(const QPoint &widgetPos);
    QPoint toWidgetCoordinates(const QPoint &mapPos);
    void drawMap(QPainter &painter);
    int getIncidentAtPosition(const QPoint &pos);

    // Nouvelles fonctions pour l'amélioration
    QPixmap getIconForType(const QString &type, const QColor &color);
    QPoint calculateIncidentPosition(const QString &buildingName, int incidentIndex, int totalIncidents);
    QColor getColorForType(const QString &type);
};

#endif // MAPDIALOG_H
