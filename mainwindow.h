#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_enterDfuButton_clicked();
    void on_checkConnectionButton_clicked();
    void on_alertButton_clicked();

private:
    Ui::MainWindow *ui;
    bool sendDfuCommand();
    bool isDeviceConnected();
};
