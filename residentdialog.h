#ifndef RESIDENTDIALOG_H
#define RESIDENTDIALOG_H

#include <QDialog>
#include <QVariantMap>

QT_BEGIN_NAMESPACE
namespace Ui { class ResidentDialog; }
QT_END_NAMESPACE

class ResidentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResidentDialog(QWidget *parent = nullptr, const QVariantMap &residentData = QVariantMap());
    ~ResidentDialog();

    QVariantMap getResidentData() const;
    bool isEditMode() const { return editMode; }

private slots:
    void onSaveClicked();
    void validateInput();

private:
    Ui::ResidentDialog *ui;
    void setupValidation();
    void setupConnections();
    void loadResidentData(const QVariantMap &data);

    bool editMode;
    QVariantMap residentData;
};

#endif // RESIDENTDIALOG_H
