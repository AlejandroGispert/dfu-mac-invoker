#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h" // This must be included so Ui::MainWindow is a complete type
#include "usb/dfu_handler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->dfuButton, &QPushButton::clicked, this, &MainWindow::on_enterDfuButton_clicked);
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::on_checkConnectionButton_clicked);
    connect(ui->alertButton, &QPushButton::clicked, this, &MainWindow::on_alertButton_clicked);
}

MainWindow::~MainWindow() = default;

void MainWindow::on_enterDfuButton_clicked() {
    if (sendDfuCommand()) {
        QMessageBox::information(this, "DFU", "DFU command sent.");
    } else {
        QMessageBox::critical(this, "DFU", "Failed to send DFU command.");
    }
}

void MainWindow::on_checkConnectionButton_clicked() {
    if (isDeviceConnected()) {
        QMessageBox::information(this, "Connection", "Device connected.");
    } else {
        QMessageBox::warning(this, "Connection", "Device not found.");
    }
}

void MainWindow::on_alertButton_clicked() {
    QMessageBox::information(this, "Status", "App is working!");
}

// Dummy implementations - replace with your actual logic
bool MainWindow::sendDfuCommand() {
    return true;
}

bool MainWindow::isDeviceConnected() {
    return false;
}
