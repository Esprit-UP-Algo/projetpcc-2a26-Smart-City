#ifndef INCIDENTDIALOG_H
#define INCIDENTDIALOG_H

#include <QDialog>
#include <QVariantMap>

class QComboBox;
class QLineEdit;
class QDateTimeEdit;
class QSpinBox;

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
    bool editMode;
    int incidentId;
    
    // UI components
    QComboBox *typeCombo;
    QLineEdit *localisationEdit;
    QDateTimeEdit *dateHeureEdit;
    QSpinBox *graviteSpinBox;
    QComboBox *statutCombo;

    void setupDialog();
    void loadIncidentData(const QVariantMap &data);
    bool validateInput();
};

#endif // INCIDENTDIALOG_H