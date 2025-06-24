#include <libusb-1.0/libusb.h>
#include <optional>
#include <utility> // for std::pair

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
