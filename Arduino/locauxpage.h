#ifndef LOCAUXPAGE_H
#define LOCAUXPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LocauxPage; }
QT_END_NAMESPACE

class LocauxPage : public QWidget
{
    Q_OBJECT

public:
    explicit LocauxPage(QWidget *parent = nullptr);
    ~LocauxPage();

private slots:
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void on_supprimerButton_clicked();
    void on_consulterButton_clicked();
    void on_exportButton_clicked();
    void on_searchLineEdit_textChanged(const QString &arg1);
    void on_sortComboBox_currentIndexChanged(int index);

private:
    Ui::LocauxPage *ui;
    
    void setupTable();
    void loadLocaux();
    void clearForm();
    void populateForm(int row);
};

#endif // LOCAUXPAGE_H