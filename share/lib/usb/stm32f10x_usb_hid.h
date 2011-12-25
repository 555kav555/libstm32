#ifndef __STM32F10x_USB_HID_H
#define __STM32F10x_USB_HID_H

#include "stm32f10x_usb.h"
#include "usb_hid.h"

#define USB_HID_REPORT_DESCRIPTOR_TABLE 3

void USB_HID_controlRequestHandler();

#endif
