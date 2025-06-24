#ifndef DFU_HANDLER_H
#define DFU_HANDLER_H

#include <cstdint>

bool sendDfuUsbCommand(uint16_t vendorId, uint16_t productId);
bool checkDeviceConnection(uint16_t vendorId, uint16_t productId);

#endif // DFU_HANDLER_H
