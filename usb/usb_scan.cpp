#include <libusb-1.0/libusb.h>
#include <optional>
#include <utility> // for std::pair
#include <vector>
#include <string>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QRegularExpression>

// Returns first detected USB device VID/PID or std::nullopt if none found
std::optional<std::pair<uint16_t, uint16_t>> findUsbDevice() {
    libusb_context* ctx = nullptr;
    libusb_device **list = nullptr;
    ssize_t cnt;

    if (libusb_init(&ctx) < 0) {
        return std::nullopt;
    }

    cnt = libusb_get_device_list(ctx, &list);
    if (cnt < 0) {
        libusb_exit(ctx);
        return std::nullopt;
    }

    std::optional<std::pair<uint16_t, uint16_t>> foundDevice = std::nullopt;

    for (ssize_t i = 0; i < cnt; ++i) {
        libusb_device *device = list[i];
        libusb_device_descriptor desc;
        if (libusb_get_device_descriptor(device, &desc) == 0) {
            printf("Found device: VID=%04x PID=%04x\n", desc.idVendor, desc.idProduct);
            // You can filter here by VID/PID if you want:
            // if (desc.idVendor == YOUR_VENDOR && desc.idProduct == YOUR_PRODUCT) { ... }
            // For now, just pick first device:
            foundDevice = std::make_pair(desc.idVendor, desc.idProduct);
            break;
        }
    }

    libusb_free_device_list(list, 1);
    libusb_exit(ctx);

    return foundDevice;
}

struct ThunderboltDeviceInfo {
    QString status;
    QString deviceName;
    bool targetDiskMode = false;
};

std::vector<ThunderboltDeviceInfo> scanThunderboltDevices() {
    std::vector<ThunderboltDeviceInfo> devices;
    QProcess process;
    process.start("system_profiler", QStringList() << "SPThunderboltDataType");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n');
    ThunderboltDeviceInfo current;
    bool inDevice = false;
    for (const QString& line : lines) {
        if (line.contains("Device Name:")) {
            current.deviceName = line.section(":", 1).trimmed();
            inDevice = true;
        }
        if (line.contains("Status:")) {
            current.status = line.section(":", 1).trimmed();
        }
        if (line.contains("Target Disk Mode:")) {
            current.targetDiskMode = true;
        }
        if (inDevice && line.trimmed().isEmpty()) {
            if (!current.deviceName.isEmpty()) {
                devices.push_back(current);
            }
            current = ThunderboltDeviceInfo();
            inDevice = false;
        }
    }
    // Add last device if not already added
    if (inDevice && !current.deviceName.isEmpty()) {
        devices.push_back(current);
    }
    return devices;
}
