#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include "arduino.h"  // ← ajouter l'inclusion de la classe Arduino

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_fullscreenButton_clicked();

private:
    Ui::MainWindow *ui;
    void shakeWidget(QWidget *widget);
    void performLogin();
    void showErrorMessage(const QString &message);
    bool isLoggingIn;

    Arduino myArduino;  // ← déclaration de l'objet Arduino ici
};

#endif // MAINWINDOW_H
