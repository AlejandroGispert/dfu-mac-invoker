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

bool sendDfuUsbCommand(uint16_t vendorId, uint16_t productId) {
    // Use vendorId and productId to find device and send DFU command
    libusb_context* ctx = nullptr;
    libusb_device_handle* handle = nullptr;
    bool success = false;

    if (libusb_init(&ctx) < 0)
        return false;

    handle = libusb_open_device_with_vid_pid(ctx, vendorId, productId);
    if (!handle) {
        libusb_exit(ctx);
        return false;
    }

    // Here add your DFU VDM command sending code using libusb_bulk_transfer or control_transfer

    // For example:
    // unsigned char data[] = {...};
    // int transferred;
    // int r = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, data, length, timeout);

    success = true; // Replace with real success condition

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
