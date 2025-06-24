// dfu_handler.cpp
#include "dfu_handler.h"
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>
#include <iostream>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/USBSpec.h>
 // Replace with your USB product ID
#include "dfu_handler.h"
#include <libusb-1.0/libusb.h>

// dfu_handler.cpp
#include "dfu_handler.h"
#include <libusb-1.0/libusb.h>
#include <iostream>

bool sendDfuUsbCommand(uint16_t vendorId, uint16_t productId) {
    libusb_context* ctx = nullptr;
    libusb_device_handle* handle = nullptr;
    bool success = false;

    if (libusb_init(&ctx) < 0) {
        std::cerr << "Failed to initialize libusb." << std::endl;
        return false;
    }

    handle = libusb_open_device_with_vid_pid(ctx, vendorId, productId);
    if (!handle) {
        std::cerr << "Could not open USB device with VID:PID = "
                  << std::hex << vendorId << ":" << productId << std::endl;
        libusb_exit(ctx);
        return false;
    }

    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(handle, 0) == 1) {
        libusb_detach_kernel_driver(handle, 0);
    }

    // Send the VDM command: func_1000015e0(device, id, 0x56444d73);
    // 0x56444d73 => 'sVDM' in ASCII, little-endian
    uint8_t bmRequestType = 0x40; // Vendor, Host to Device, Device Recipient
    uint8_t bRequest = 0x01;      // Assumed request code (may need adjustment)
    uint16_t wValue = 0x5644;     // Upper 16 bits of the 32-bit value
    uint16_t wIndex = 0x4d73;     // Lower 16 bits
    unsigned char* data = nullptr;
    uint16_t length = 0;
    unsigned int timeout = 1000;  // milliseconds

    int r = libusb_control_transfer(
        handle,
        bmRequestType,
        bRequest,
        wValue,
        wIndex,
        data,
        length,
        timeout
    );

    if (r >= 0) {
        std::cout << "DFU VDM command sent successfully." << std::endl;
        success = true;
    } else {
        std::cerr << "Failed to send DFU command. libusb error: " << r << std::endl;
    }

    libusb_close(handle);
    libusb_exit(ctx);
    return success;
}

bool checkDeviceConnection(uint16_t vendorId, uint16_t productId) {
    libusb_context* ctx = nullptr;
    libusb_device_handle* handle = nullptr;

    if (libusb_init(&ctx) < 0)
        return false;

    handle = libusb_open_device_with_vid_pid(ctx, vendorId, productId);

    bool connected = (handle != nullptr);

    if (handle)
        libusb_close(handle);
    libusb_exit(ctx);

    return connected;
}
