#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "usb/dfu_handler.h"
#include "usb/usb_scan.h"
#include <libusb-1.0/libusb.h>
#include <QStringList>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->dfuButton, &QPushButton::clicked, this, &MainWindow::on_enterDfuButton_clicked);
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::on_checkConnectionButton_clicked);
    connect(ui->alertButton, &QPushButton::clicked, this, &MainWindow::on_alertButton_clicked);
    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::on_scanButton_clicked);

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



bool MainWindow::sendDfuCommand() {
    auto device = findUsbDevice();
    if (!device) {
        QMessageBox::warning(this, "Device", "No USB device detected");
        return false;
    }
    auto [vid, pid] = *device;
    return sendDfuUsbCommand(vid, pid);
}

bool MainWindow::isDeviceConnected() {
    auto device = findUsbDevice();
    if (!device)
        return false;
    auto [vid, pid] = *device;
    return checkDeviceConnection(vid, pid);
}

void MainWindow::on_scanButton_clicked() {
    // Example: scan for USB devices and show a message
    auto device = findUsbDevice();
    if (device) {
        auto [vid, pid] = *device;
        QMessageBox::information(this, "USB Scan", QString("Found device: VID=%1 PID=%2")
                                 .arg(vid, 4, 16, QChar('0'))
                                 .arg(pid, 4, 16, QChar('0')));
    } else {
        QMessageBox::warning(this, "USB Scan", "No USB devices found.");
    }
}