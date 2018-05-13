#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();  
private slots:
    void on_SearchHashtagButton_clicked();

    void on_ChangeHashtagButton_clicked();

    void on_TurnOnSplashScreenButton_clicked();

    void on_SettingsButton_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
