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

    QVariantMap getIncidentData();

    // Méthodes CRUD Oracle
    bool connecterOracle();
    void creerTableIncident();
    bool insererIncidentOracle();
    bool modifierIncidentOracle(int id);
    bool supprimerIncidentOracle(int id);
    QList<QVariantMap> listerIncidentsOracle();
    
    // Méthodes Fallback SQLite
    bool connecterSQLite();
    bool insererIncidentSQLite();
    QList<QVariantMap> listerIncidentsSQLite();

private slots:
    void on_saveButton_clicked();

private:
    Ui::IncidentDialog *ui;
    bool validateForm();
};

#endif // INCIDENTDIALOG_H
