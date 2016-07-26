#include "stm32f10x_usb.h"

uint8_t USB_EP_slotNum[16] = { [0 ... 15] = USB_EP_FREESLOT };
USB_EP_slot_t USB_EP_slot[USB_EP_SLOT_NUM] = { [0 ... USB_EP_SLOT_NUM - 1] = {
	NULL,
#ifdef USB_DOUBLEBUFFER
	{0},
#endif
	0, 0
} };

static uint16_t PMA_alloc_ptr = 0;

USB_BTABLE_entry_t* USB_getBTABLEEntry(uint8_t num) {
	return (USB_BTABLE_entry_t*)(((uint8_t*)USB_PMAAddr) + ((USB->BTABLE + (num << 3)) << 1));
}

static void UserToPMABufferCopy(const uint8_t *from,uint16_t toPMA,uint16_t len){
	if(!len)return;
	uint16_t *to=(uint16_t*)(toPMA*2+USB_PMAAddr);
	if(toPMA&1){
		to--;
		uint16_t t=*to;
		((uint8_t*)&t)[1]=*from;
		*to=t;
		to+=2;
		from++;
		len--;
	}
	len=(len+1)>>1;
	for(;len;len--){
		*to=*(uint16_t*)from;
		to+=2;
		from+=2;
	}
}

static void PMAToUserBufferCopy(uint8_t *to,uint16_t fromPMA,uint16_t len){
	if(!len)return;
	uint16_t *from=(uint16_t*)(fromPMA*2+USB_PMAAddr);
	if(fromPMA&1){
		from--;
		uint16_t t=*from;
		*to=((uint8_t*)&t)[1];
		from+=2;
		to++;
		len--;
	}
	if(len&1){
		uint16_t t=from[len-1];
		to[len-1]=(uint8_t)t;
	}
	len>>=1;
	for(;len;len--){
		*(uint16_t*)to=*from;
		from+=2;
		to+=2;
	}
}

#ifdef USB_DOUBLEBUFFER

uint16_t USB_EP_toRead(uint8_t num) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	return ((USB_EP_slot[num].db && (USB->EPR[num] & USB_EPR_DTOG_RX)) ? e->COUNT_TX : e->COUNT_RX) & USB_BTABLE_COUNT_RX_COUNTM;
}

uint16_t USB_EP_read(uint8_t num, uint8_t *buf, uint16_t len, uint16_t offset) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	uint16_t epBAddr, epBCount;
	if(USB_EP_slot[num].db && (USB->EPR[num] & USB_EPR_DTOG_RX)) {
		epBAddr = e->ADDR_TX; epBCount = e->COUNT_TX;
	} else {
		epBAddr = e->ADDR_RX; epBCount = e->COUNT_RX;
	}
	uint16_t l = (epBCount & USB_BTABLE_COUNT_RX_COUNTM) - offset;
	if(l > len) l = len;
	PMAToUserBufferCopy(buf, epBAddr + offset, l);
	return l;
}

uint16_t USB_EP_write(uint8_t num, const uint8_t *buf, uint16_t len, uint16_t offset) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	uint16_t l = USB_EP_slot[num].size_tx - offset; // Ok for double buffering
	if(l > len) l = len;
	if(USB_EP_slot[num].db && !(USB->EPR[num] & USB_EPR_DTOG_RX)) {
		UserToPMABufferCopy(buf, e->ADDR_RX + offset, l);
		e->COUNT_RX = l + offset;
	} else {
		UserToPMABufferCopy(buf, e->ADDR_TX + offset, l);
		e->COUNT_TX = l + offset;
	}
	return l;
}

void USB_EP_writeEmpty(uint8_t num){
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	if(USB_EP_slot[num].db && !(USB->EPR[num] & USB_EPR_DTOG_RX)) {
		e->COUNT_RX = 0;
	} else {
		e->COUNT_TX = 0;
	}
}

#else

uint16_t USB_EP_toRead(uint8_t num) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	return e->COUNT_RX & USB_BTABLE_COUNT_RX_COUNTM;
}

uint16_t USB_EP_read(uint8_t num, uint8_t *buf, uint16_t len, uint16_t offset) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	uint16_t l = (e->COUNT_RX & USB_BTABLE_COUNT_RX_COUNTM) - offset;
	if(l > len) l = len;
	PMAToUserBufferCopy(buf, e->ADDR_RX + offset, l);
	return l;
}

uint16_t USB_EP_write(uint8_t num, const uint8_t *buf, uint16_t len, uint16_t offset) {
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	uint16_t l = USB_EP_slot[num].size_tx - offset;
	if(l > len) l = len;
	UserToPMABufferCopy(buf, e->ADDR_TX + offset, l);
	e->COUNT_TX = l + offset;
	return l;
}

void USB_EP_writeEmpty(uint8_t num){
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	e->COUNT_TX = 0;
}

#endif

void USB_deInit() {
	USB_connect(0);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, DISABLE);
}

void USB_init() {
	/* Enable the USB clock */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USB, DISABLE);
}

void USB_connect(bool v) {
	USB->CNTR = USB_CNTR_FRES; /* Force USB Reset */
	USB->ISTR = 0; /* Clear Interrupt Status */
	GPIO_SetBits(USB_PULLUP_PORT, USB_PULLUP_PIN);
	GPIO_InitTypeDef gpioCfg;
	gpioCfg.GPIO_Pin = USB_PULLUP_PIN;
	gpioCfg.GPIO_Speed = GPIO_Speed_50MHz;
	gpioCfg.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USB_PULLUP_PORT, &gpioCfg);
	if(v) {
		uint16_t i = 0;
		while(--i) __NOP();
		gpioCfg.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(USB_PULLUP_PORT, &gpioCfg);
		USB->CNTR = 0; /* USB Reset Interrupt Mask */
		USB->ISTR = 0; /* Clear Interrupt Status */
		USB->CNTR = USB_CNTR_RESETM; /* USB Reset Interrupt Mask */
	} else {
		USB->CNTR = USB_CNTR_FRES|USB_CNTR_PDWN; /* Switch Off USB Device */
	}
}

void USB_reset() {
	USB->ISTR = 0; /* Clear Interrupt Status */

	USB->CNTR = USB_CNTR_CTRM | USB_CNTR_RESETM | USB_CNTR_SUSPM | USB_CNTR_WKUPM/* |
		(USB_SUSPEND_EVENT ? USB_CNTR_SUSPM   : 0) |
		(USB_WAKEUP_EVENT  ? USB_CNTR_WKUPM   : 0) |
		(USB_ERROR_EVENT   ? USB_CNTR_ERRM    : 0) |
		(USB_ERROR_EVENT   ? USB_CNTR_PMAOVRM : 0) |
		(USB_SOF_EVENT     ? USB_CNTR_SOFM    : 0) |
		(USB_SOF_EVENT     ? USB_CNTR_ESOFM   : 0)*/;

	USB->BTABLE = 0;
	PMA_alloc_ptr = (sizeof(USB_BTABLE_entry_t) >> 1) * USB_EP_SLOT_NUM;
	int i;
	for(i = 0; i < 16; ++i) USB_EP_slotNum[i] = USB_EP_FREESLOT;
	USB_ctlInit();
	USB->DADDR = 0 | USB_DADDR_EF;
}

void USB_suspend() {
	USB->CNTR |= USB_CNTR_FSUSP;
//	USB->CNTR |= USB_CNTR_LPMODE;
}

void USB_resume() {
//	USB->CNTR |= USB_CNTR_LPMODE;
//...
	USB->CNTR &= ~USB_CNTR_FSUSP;
}

static void USB_EP_clear(uint8_t num) {
	USB_EP_setState(num, USB_EPR_STAT_TX_DISABLED | USB_EPR_STAT_RX_DISABLED);
	USB_EP_slot[num].handler = NULL;
}

void USB_EP_clearAll() {
	int i;
	for(i = 1; i < USB_EP_SLOT_NUM; ++i) USB_EP_clear(i);
	PMA_alloc_ptr = (sizeof(USB_BTABLE_entry_t) >> 1) * USB_EP_SLOT_NUM + USB_EP_slot[0].size_tx + USB_EP_slot[0].size_rx;
	for(i = 1; i < 16; ++i) USB_EP_slotNum[i] = USB_EP_FREESLOT;
}

void USB_EP_setup(uint8_t num, uint8_t ep_num, uint16_t flags, uint16_t size_tx, uint16_t size_rx, USB_EP_H_t handler) {
	USB_EP_slotNum[ep_num & 0x0F] = num;
	USB->EPR[num] = flags | (ep_num & ~USB_EPR_TMASK);
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	e->ADDR_TX = PMA_alloc_ptr;
	PMA_alloc_ptr += ((size_tx + 1) >> 1) << 1;
	e->ADDR_RX = PMA_alloc_ptr;
	PMA_alloc_ptr += ((size_rx + 1) >> 1) << 1;
	e->COUNT_TX = 0;
	if(size_rx > 62) {
		e->COUNT_RX = ((size_rx - 1) << 5) | 0x8000;
	} else {
		e->COUNT_RX = size_rx << 9;
	}
#ifdef USB_DOUBLEBUFFER
	USB_EP_slot[num].db = 0;
#endif
	USB_EP_slot[num].size_tx = size_tx;
	USB_EP_slot[num].size_rx = size_rx;
	USB_EP_slot[num].handler = handler;
	USB_EP_setState(num, flags & USB_EPR_TMASK);
}

#ifdef USB_DOUBLEBUFFER

void USB_EP_setup_db(uint8_t num, uint8_t ep_num, uint16_t flags, uint16_t size, USB_EP_H_t handler) {
	USB_EP_slotNum[ep_num & 0x0F] = num;
	USB->EPR[num] = flags | (ep_num & ~USB_EPR_TMASK) | ((flags & USB_EPR_EP_TYPE_BULK) ? USB_EPR_EP_KIND : 0);
	USB_BTABLE_entry_t *e = USB_getBTABLEEntry(num);
	e->ADDR_TX = PMA_alloc_ptr;
	PMA_alloc_ptr += ((size + 1) >> 1) << 1;
	e->ADDR_RX = PMA_alloc_ptr;
	PMA_alloc_ptr += ((size + 1) >> 1) << 1;
	if(flags & USB_EPR_STAT_RX_VALID) {
		uint16_t v;
		if(size > 62) {
			v = ((size - 1) << 5) | 0x8000;
		} else {
			v = size << 9;
		}
		e->COUNT_TX = v;
		e->COUNT_RX = v;
	}
	USB_EP_slot[num].db = 1;
	USB_EP_slot[num].size = size;
	USB_EP_slot[num].handler = handler;
	USB_EP_setState(num, flags & USB_EPR_TMASK);
}

#endif

void USB_EP_setRXState(uint8_t num, uint16_t flags) {
	USB->EPR[num] = ((USB->EPR[num] ^ flags) & (USB_EPR_STAT_RX_0 | USB_EPR_STAT_RX_1 | ~USB_EPR_TMASK)) | (USB_EPR_CTR_TX | USB_EPR_CTR_RX);
}

void USB_EP_setTXState(uint8_t num, uint16_t flags) {
	USB->EPR[num] = ((USB->EPR[num] ^ flags) & (USB_EPR_STAT_TX_0 | USB_EPR_STAT_TX_1 | ~USB_EPR_TMASK)) | (USB_EPR_CTR_TX | USB_EPR_CTR_RX);
}

void USB_EP_setState(uint8_t num, uint16_t flags) {
	USB->EPR[num] = ((USB->EPR[num] ^ flags) &
		(USB_EPR_STAT_TX_0 | USB_EPR_STAT_TX_1 |
		USB_EPR_STAT_RX_0 | USB_EPR_STAT_RX_1 |
		~USB_EPR_TMASK)) | (USB_EPR_CTR_TX | USB_EPR_CTR_RX);
}

uint16_t USB_EP_getRXState(uint8_t num) {
	return USB->EPR[num] & (USB_EPR_STAT_RX_0 | USB_EPR_STAT_RX_1);
}

uint16_t USB_EP_getTXState(uint8_t num) {
	return USB->EPR[num] & (USB_EPR_STAT_TX_0 | USB_EPR_STAT_TX_1);
}
