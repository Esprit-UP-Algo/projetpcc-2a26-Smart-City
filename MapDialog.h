#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QMap>
#include <QRect>

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);

    void addBuilding(const QString &name, const QRect &rect);
    void addIncident(const QString &buildingName);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap mapImage;       // Carte
    QPixmap incidentIcon;   // Icône incident
    QMap<QString, QRect> buildings;
    QList<QPoint> incidentPositions;
};

#endif // MAPDIALOG_H
