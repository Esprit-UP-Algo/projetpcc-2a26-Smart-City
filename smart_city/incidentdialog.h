#ifndef INCIDENTDIALOG_H
#define INCIDENTDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class IncidentDialog;
}

class IncidentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IncidentDialog(QWidget *parent = nullptr);
    explicit IncidentDialog(const QVariantMap &incidentData, QWidget *parent = nullptr);
    ~IncidentDialog();

    QVariantMap getIncidentData() const;

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::IncidentDialog *ui;
    bool editMode;
    int incidentId;

    void setupDialog();
    void loadIncidentData(const QVariantMap &data);
    bool validateInput();
};

#endif // INCIDENTDIALOG_H
