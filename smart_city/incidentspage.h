#ifndef INCIDENTSPAGE_H
#define INCIDENTSPAGE_H

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
    void initializeStatsChart();
    void updateStats();
    bool sendEmail(const QString &to, const QString &subject, const QString &body);

private:
    // Fonctions existantes
    void refreshList();
    void refreshStats();
    QString removeAccents(const QString &input);
    QString getServiceEmail(const QString &incidentType);
    void clearForm();
    int getNextId();

    // Nouvelles fonctions pour le rapport PDF
    QString genererRetourTechnique(const QString& statut, const QString& type, const QString& niveau);
    QString genererDecisionFinale(const QString& statut, const QString& type, const QString& niveau);
    QString getNomResidentFromCIN(const QString& cin);

    QChart *statsChart = nullptr;
    Ui::IncidentsPage *ui;
    int currentEditId = -1;
};

#endif // INCIDENTSPAGE_H
