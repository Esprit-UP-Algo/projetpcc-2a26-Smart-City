#ifndef INCIDENTSPAGE_H
#define INCIDENTSPAGE_H
#include "arduino.h"

#include <QWidget>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class IncidentsPage; }
QT_END_NAMESPACE

class IncidentsPage : public QWidget
{
    Q_OBJECT
public:
    explicit IncidentsPage(QWidget *parent = nullptr);
    ~IncidentsPage();
    void testQtCharts();


private slots:
    void onSaveIncident();

    void onSearch();
    void onDeleteIncident();
    void onEditIncident();
    void onExportPdf();
    void onSortIndexChanged(int index);
    // ← AJOUTEZ CETTE LIGNE

    void initializeStatsChart();
    void updateStats();


    bool sendEmail(const QString &to, const QString &subject, const QString &body);
    void onArduinoMessage(QString msg);


private:
    void refreshList();
    void refreshStats();
    QString removeAccents(const QString &input);
    QChart *statsChart = nullptr;
    QString getServiceEmail(const QString &incidentType);



    void clearForm();
    int getNextId();

    // Nouvelles fonctions pour le rapport PDF
    QString genererRetourTechnique(const QString& statut, const QString& type, const QString& niveau);
    QString genererDecisionFinale(const QString& statut, const QString& type, const QString& niveau);
    QString getNomResidentFromCIN(const QString& cin);

    Ui::IncidentsPage *ui;
    int currentEditId = -1; // -1 = nouveau, >0 = modification

    // Theme/Language helpers
    void applyTheme();
    void reloadTranslations();
    Arduino *arduino;
};

#endif // INCIDENTSPAGE_H
