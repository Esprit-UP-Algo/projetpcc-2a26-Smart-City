#ifndef VEHICULEDIALOG_H
#define VEHICULEDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class VehiculeDialog;
}

class VehiculeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VehiculeDialog(QWidget *parent = nullptr);
    explicit VehiculeDialog(const QVariantMap &vehiculeData, QWidget *parent = nullptr);
    ~VehiculeDialog();

    QVariantMap getVehiculeData() const;

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::VehiculeDialog *ui;
    bool editMode;
    QString vehiculeId;

    void setupDialog();
    void loadVehiculeData(const QVariantMap &data);
    bool validateInput();
};

#endif // VEHICULEDIALOG_H
