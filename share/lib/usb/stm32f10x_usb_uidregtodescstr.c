#include "stm32f10x_usb.h"

static uint8_t USB_serialNumberStrDesc[64];

static const uint8_t *U_ID = (uint8_t*)0x1FFFF7E8;

static uint8_t nibbleToSym(uint8_t v) {
	v &= 0x0F;
	return ((v < 10) ? '0' : 'A') + v;
}

unsigned USB_UIDRegToDescStr(uint8_t *buf, unsigned len, const char *prefix) {
	uint8_t *p = buf;
	unsigned i = 0;
	if(prefix){
		for(; ((p - buf) < ((int)len - 1)) && *prefix; p += 2, ++prefix) {
			p[0] = *prefix;
			p[1] = 0;
		}
	}
	for(; ((p - buf) < ((int)len - 3)) && (i < 12); p += 4, ++i) {
		p[0] = nibbleToSym(U_ID[i] >> 4);
		p[1] = 0;
		p[2] = nibbleToSym(U_ID[i]);
		p[3] = 0;
	}
	return p - buf;
}

void USB_setStringDescriptorToUIDReg(uint8_t slot, const char *prefix) {
	unsigned len = USB_UIDRegToDescStr(USB_serialNumberStrDesc + 2, sizeof(USB_serialNumberStrDesc) - 2, prefix);
	USB_serialNumberStrDesc[0] = len + 2;
	USB_serialNumberStrDesc[1] = USB_STRING_DESCRIPTOR_TYPE;
	USB_setDescriptor(USB_STRING_DESCRIPTOR_TABLE, slot, USB_serialNumberStrDesc, len + 2);
}
