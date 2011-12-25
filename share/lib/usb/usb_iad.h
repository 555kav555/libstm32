#ifndef __USB_IAD_H
#define __USB_IAD_H

#include "usb.h"

/* Device class */
#define USB_IAD_DEVICE_CLASS 0xEF
#define USB_IAD_DEVICE_SUBCL 0x02
#define USB_IAD_DEVICE_PROTO 0x01

#define USB_IAD_DESCRIPTOR_TYPE 0x0B

typedef struct __attribute__((packed)){
	uint8_t bLength;		// Size of this Descriptor in Bytes
	uint8_t bDescriptorType;	// Descriptor Type (=11)
	uint8_t bFirstInterface;	// Interface number of the first one associated with this function
	uint8_t bInterfaceCount;	// Numver of contiguous interface associated with this function
	uint8_t bFunctionClass;		// The class triad of this interface,
	uint8_t bFunctionSubClass;	//   usually same as the triad of the first interface
	uint8_t bFunctionProcotol;
	uint8_t iInterface;		// Index of String Desc for this function
}USB_IAD_descriptor_t;
#define USB_IAD_DESC_SIZE (sizeof(USB_IAD_descriptor_t))

#endif
