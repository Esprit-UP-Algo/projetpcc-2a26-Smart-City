#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

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
};
#endif // MAINWINDOW_H
