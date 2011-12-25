#ifndef __USB_CDC_H
#define __USB_CDC_H

#include "usb.h"

/* Device class */
#define USB_CDC_DEVICE_CLASS 0x02
#define USB_CDC_DEVICE_SUBCL 0x00
#define USB_CDC_DEVICE_PROTO 0x00

/* Comm interface class */
#define USB_CDC_COMMIF_CLASS 0x02
#define USB_CDC_COMMIF_SUBCL_ABSTRACT 0x02
#define USB_CDC_COMMIF_PROTO_NONE 0x00
#define USB_CDC_COMMIF_PROTO_AT 0x01

/* Data interface class */
#define USB_CDC_DATAIF_CLASS 0x0A
#define USB_CDC_DATAIF_SUBCL 0x00
#define USB_CDC_DATAIF_PROTO_NONE 0x00

#define USB_CDC_CS_INTERFACE 0x24
#define USB_CDC_CS_ENDPOINT 0x25

#define USB_CDC_SCS_HEADER 0x00
#define USB_CDC_SCS_CALL 0x01
#define USB_CDC_SCS_ABSTRACT 0x02
#define USB_CDC_SCS_UNION 0x06

typedef struct __attribute__((packed)){
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t data[0];
}USB_CDC_funcDesc_t;

typedef struct __attribute__((packed)){
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint16_t bcdCDC;
}USB_CDC_funcDesc_header_t;
#define USB_CDC_FUNCDESC_HEADER_SIZE (sizeof(USB_CDC_funcDesc_header_t))

typedef struct __attribute__((packed)){
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
}USB_CDC_funcDesc_call_t;
#define USB_CDC_FUNCDESC_CALL_SIZE (sizeof(USB_CDC_funcDesc_call_t))
#define USB_CDC_FUNCDESC_CALL_DATAIF 2
#define USB_CDC_FUNCDESC_CALL_MGMT 1

typedef struct __attribute__((packed)){
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
}USB_CDC_funcDesc_abstract_t;
#define USB_CDC_FUNCDESC_ABSTRACT_SIZE (sizeof(USB_CDC_funcDesc_abstract_t))
#define USB_CDC_FUNCDESC_ABSTRACT_NETCONN 8
#define USB_CDC_FUNCDESC_ABSTRACT_SENDBREAK 4
#define USB_CDC_FUNCDESC_ABSTRACT_LINE 2
#define USB_CDC_FUNCDESC_ABSTRACT_COMM 1

typedef struct __attribute__((packed)){
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bMasterInterface;
	uint8_t bSlaveInterface[0];
}USB_CDC_funcDesc_union_t;
#define USB_CDC_FUNCDESC_UNION_SIZE(N) (sizeof(USB_CDC_funcDesc_union_t)+N)

/* USB CDC Request Codes */
#define USB_CDC_REQUEST_SET_LINE_CODING 0x20
#define USB_CDC_REQUEST_GET_LINE_CODING 0x21
#define USB_CDC_REQUEST_SET_CONTROL_LINE_STATE 0x22

typedef struct __attribute__((packed)){
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
}USB_CDC_lineCoding_t;

#endif
