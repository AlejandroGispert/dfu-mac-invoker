#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "usb/dfu_handler.h"
#include "usb/usb_scan.h"
#include <libusb-1.0/libusb.h>
#include <QStringList>
#include <QProcess>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->dfuButton, &QPushButton::clicked, this, &MainWindow::on_enterDfuButton_clicked);
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::on_checkConnectionButton_clicked);
    connect(ui->alertButton, &QPushButton::clicked, this, &MainWindow::on_alertButton_clicked);
    connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::on_scanButton_clicked);

    // Add this for automatic status updates
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &MainWindow::checkDeviceStatus);
    statusTimer->start(2000); // check every 2 seconds (adjust as needed)
}

MainWindow::~MainWindow() = default;

void MainWindow::on_enterDfuButton_clicked() {
    if (sendDfuCommand()) {
        QMessageBox::information(this, "DFU", "DFU command sent.");
    } else {
        QMessageBox::critical(this, "DFU", "Failed to send DFU command.");
    }
    checkDeviceStatus();
}

void MainWindow::on_checkConnectionButton_clicked() {
    if (isDeviceConnected()) {
        QMessageBox::information(this, "Connection", "Device connected.");
    } else {
        QMessageBox::warning(this, "Connection", "Device not found.");
    }
    checkDeviceStatus();
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
    checkDeviceStatus();
}

void MainWindow::updateUsbStatusButton(bool detected) {
    if (detected) {
        ui->usbStatusButton->setStyleSheet("background-color: green; color: white;");
        ui->usbStatusButton->setText("USB Device Detected");
    } else {
        ui->usbStatusButton->setStyleSheet("background-color: gray; color: black;");
        ui->usbStatusButton->setText("USB Device Not Detected");
    }
}

void MainWindow::updateThunderboltStatusButton(bool detected) {
    if (detected) {
        ui->thunderboltStatusButton->setStyleSheet("background-color: green; color: white;");
        ui->thunderboltStatusButton->setText("Thunderbolt Device Detected");
    } else {
        ui->thunderboltStatusButton->setStyleSheet("background-color: gray; color: black;");
        ui->thunderboltStatusButton->setText("Thunderbolt Device Not Detected");
    }
}

void MainWindow::updateThunderboltInfoText() {
    auto devices = scanThunderboltDevices();
    QString info;
    for (const auto& device : devices) {
        info += QString("Device Name: %1\n").arg(device.deviceName);
        info += QString("Status: %1\n").arg(device.status);
        info += QString("Target Disk Mode: %1\n\n").arg(device.targetDiskMode ? "Yes" : "No");
    }
    if (info.isEmpty()) {
        info = "No Thunderbolt devices detected.";
    }
    ui->thunderboltInfoTextEdit->setPlainText(info);
}

void MainWindow::updateThunderboltGlowButton(bool detected) {
    ui->thunderboltGlowButton->setVisible(detected);
}

void MainWindow::checkDeviceStatus() {
    bool usbDetected = isDeviceConnected();
    auto thunderboltDevices = scanThunderboltDevices();
    bool thunderboltDetected = false;
    for (const auto& device : thunderboltDevices) {
        if (device.deviceName.compare("Macintosh", Qt::CaseInsensitive) == 0 && device.targetDiskMode) {
            thunderboltDetected = true;
            break;
        }
    }
    updateUsbStatusButton(usbDetected);
    updateThunderboltStatusButton(thunderboltDetected);
    updateThunderboltInfoText();
    updateThunderboltGlowButton(thunderboltDetected);
}

bool MainWindow::isDeviceOnThunderbolt() {
    QProcess process;
    process.start("system_profiler", QStringList() << "SPThunderboltDataType");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    // TODO: Replace 'YourDeviceNameOrVIDPID' with a unique identifier for your device
    return output.contains("YourDeviceNameOrVIDPID", Qt::CaseInsensitive);
}