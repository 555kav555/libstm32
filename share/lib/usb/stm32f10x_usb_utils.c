#include "stm32f10x_usb.h"

int USB_EP_getFreeSlot() {
	int i;
	for(i = 0; i < USB_EP_SLOT_NUM; ++i) {
		if((USB->EPR[i] & (USB_EPR_STAT_TX | USB_EPR_STAT_RX)) == (USB_EPR_STAT_TX_DISABLED | USB_EPR_STAT_RX_DISABLED)) return i;
	}
	return USB_RET_ERROR;
}
