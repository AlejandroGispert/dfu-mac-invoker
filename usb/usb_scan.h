#pragma once
#include <optional>
#include <utility>
#include <cstdint>
#include <vector>
#include <QString>

// Returns first detected USB device VID/PID or std::nullopt if none found
std::optional<std::pair<uint16_t, uint16_t>> findUsbDevice();

struct ThunderboltDeviceInfo {
    QString status;
    QString deviceName;
    bool targetDiskMode = false;
};

std::vector<ThunderboltDeviceInfo> scanThunderboltDevices();