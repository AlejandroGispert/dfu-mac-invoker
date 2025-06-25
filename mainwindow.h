#pragma once

#include <QMainWindow>
#include <QTimer>

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
    void on_scanButton_clicked();

private:
    QTimer* statusTimer = nullptr;
    Ui::MainWindow *ui;
    bool sendDfuCommand();
    bool isDeviceConnected();
    bool isDeviceOnThunderbolt();
    void scanUsbDevices();
    void updateUsbStatusButton(bool detected);
    void updateThunderboltStatusButton(bool detected);
    void checkDeviceStatus();
    void updateThunderboltInfoText();
    void updateThunderboltGlowButton(bool detected);
};
