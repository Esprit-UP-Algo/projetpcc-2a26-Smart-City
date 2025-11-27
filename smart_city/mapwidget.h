#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QRect>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);

    void addBuilding(const QString &name, const QRect &rect);

signals:
    void buildingClicked(const QString &name);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QMap<QString, QRect> buildings;
    QPixmap mapImage;
};

#endif // MAPWIDGET_H
