#ifndef __STM32F10x_USB_CDC_H
#define __STM32F10x_USB_CDC_H

#include "stm32f10x_usb.h"
#include "usb_cdc.h"

extern USB_CDC_lineCoding_t USB_CDC_lineCoding;

void USB_CDC_controlRequestHandler();

#endif
