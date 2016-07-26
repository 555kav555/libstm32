#ifndef __STM32F10x_USB_H
#define __STM32F10x_USB_H

#ifndef USB_MAXPACKETSIZE0
#define USB_MAXPACKETSIZE0 8 // Windows XP can't recognize a device if the packet size equals to 16... Thanks to Microsoft.
#endif

#define USB_EP_SLOT_NUM 8

#include <sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#define STM32DRV_INLINE
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usb.h"

#if(!defined USB_PULLUP_PORT || !defined USB_PULLUP_PIN)
#define USB_PULLUP_PORT GPIOA
#define USB_PULLUP_PIN GPIO_Pin_9
#endif

#define USB_RET_ERROR -1
#define USB_EP_FREESLOT 0xFF

#ifdef __cplusplus
extern "C" {
#endif

/* Exported constants --------------------------------------------------------*/
#define USB_RegBase  (APB1PERIPH_BASE+0x5C00)  /* USB_IP Peripheral Registers base address */
#define USB_PMAAddr  (APB1PERIPH_BASE+0x6000)  /* USB_IP Packet Memory Area base address   */

/******************************************************************************/
/*                         General registers                                  */
/******************************************************************************/

typedef struct {
	__IO uint32_t EPR[8];
	uint32_t RESERVED0[8];
	__IO uint32_t CNTR;
	__IO uint32_t ISTR;
	__IO uint32_t FNR;
	__IO uint32_t DADDR;
	__IO uint32_t BTABLE;
} USB_TypeDef;

/* Endpoint Buffer Descriptor */
typedef struct {
	__IO uint32_t ADDR_TX;
	__IO uint32_t COUNT_TX;
	__IO uint32_t ADDR_RX;
	__IO uint32_t COUNT_RX;
} USB_BTABLE_entry_t;

#define USB ((USB_TypeDef*)USB_RegBase)

/*******************  Bit definition for USB_EPR registers  *******************/
#define  USB_EPR_EA                         ((uint16_t)0x000F)            /*!<Endpoint Address */

#define  USB_EPR_STAT_TX                    ((uint16_t)0x0030)            /*!<STAT_TX[1:0] bits (Status bits, for transmission transfers) */
#define  USB_EPR_STAT_TX_0                  ((uint16_t)0x0010)            /*!<Bit 0 */
#define  USB_EPR_STAT_TX_1                  ((uint16_t)0x0020)            /*!<Bit 1 */

#define  USB_EPR_STAT_TX_DISABLED           ((uint16_t)0)
#define  USB_EPR_STAT_TX_STALL              (USB_EPR_STAT_TX_0)
#define  USB_EPR_STAT_TX_NAK                (USB_EPR_STAT_TX_1)
#define  USB_EPR_STAT_TX_VALID              (USB_EPR_STAT_TX_0|USB_EPR_STAT_TX_1)

#define  USB_EPR_DTOG_TX                    ((uint16_t)0x0040)            /*!<Data Toggle, for transmission transfers */
#define  USB_EPR_CTR_TX                     ((uint16_t)0x0080)            /*!<Correct Transfer for transmission */
#define  USB_EPR_EP_KIND                    ((uint16_t)0x0100)            /*!<Endpoint Kind */

#define  USB_EPR_EP_TYPE                    ((uint16_t)0x0600)            /*!<EP_TYPE[1:0] bits (Endpoint type) */
#define  USB_EPR_EP_TYPE_0                  ((uint16_t)0x0200)            /*!<Bit 0 */
#define  USB_EPR_EP_TYPE_1                  ((uint16_t)0x0400)            /*!<Bit 1 */

#define  USB_EPR_EP_TYPE_BULK               ((uint16_t)0)
#define  USB_EPR_EP_TYPE_CONTROL            (USB_EPR_EP_TYPE_0)
#define  USB_EPR_EP_TYPE_ISO                (USB_EPR_EP_TYPE_1)
#define  USB_EPR_EP_TYPE_INTERRUPT          (USB_EPR_EP_TYPE_0|USB_EPR_EP_TYPE_1)

#define  USB_EPR_SETUP                      ((uint16_t)0x0800)            /*!<Setup transaction completed */

#define  USB_EPR_STAT_RX                    ((uint16_t)0x3000)            /*!<STAT_RX[1:0] bits (Status bits, for reception transfers) */
#define  USB_EPR_STAT_RX_0                  ((uint16_t)0x1000)            /*!<Bit 0 */
#define  USB_EPR_STAT_RX_1                  ((uint16_t)0x2000)            /*!<Bit 1 */

#define  USB_EPR_STAT_RX_DISABLED           ((uint16_t)0)
#define  USB_EPR_STAT_RX_STALL              (USB_EPR_STAT_RX_0)
#define  USB_EPR_STAT_RX_NAK                (USB_EPR_STAT_RX_1)
#define  USB_EPR_STAT_RX_VALID              (USB_EPR_STAT_RX_0|USB_EPR_STAT_RX_1)

#define  USB_EPR_DTOG_RX                    ((uint16_t)0x4000)            /*!<Data Toggle, for reception transfers */
#define  USB_EPR_CTR_RX                     ((uint16_t)0x8000)            /*!<Correct Transfer for reception */

#define  USB_EPR_IMASK                      (USB_EPR_CTR_TX|USB_EPR_CTR_RX)
#define  USB_EPR_TMASK                      (USB_EPR_STAT_TX_0|USB_EPR_STAT_TX_1|USB_EPR_DTOG_TX|USB_EPR_STAT_RX_0|USB_EPR_STAT_RX_1|USB_EPR_DTOG_RX)

#define  USB_BTABLE_COUNT_RX_COUNTM             ((uint16_t)0x03FF)            /*!<Reception Byte Count mask */

typedef enum {
	USB_EVT_SETUP,
	USB_EVT_IN,
	USB_EVT_OUT
} USB_event_t;

typedef struct {
	uint8_t *data;
	uint16_t length;
} USB_buffer_t;

typedef void(*USB_EP_H_t)(uint8_t ep_slot, USB_event_t evt);

#ifdef USB_DOUBLEBUFFER

typedef struct {
	USB_EP_H_t handler;
	union {
		uint16_t size_tx;
		uint16_t size; // double buffered mode
	};
	uint16_t size_rx : 15;
	uint16_t db : 1; // double buffered mode flag
} USB_EP_slot_t;

#else

typedef struct {
	USB_EP_H_t handler;
	uint16_t size_tx;
	uint16_t size_rx;
} USB_EP_slot_t;

#endif

typedef enum {
	USB_CTL_SETUP,
	USB_CTL_DATA,
	USB_CTL_LASTDATA,
	USB_CTL_EXACTDATA,
	USB_CTL_STATUS
} USB_controlState_t;

typedef void(*USB_onControlRequest_t)();
typedef void(*USB_onControlRequestData_t)();

typedef struct {
	USB_buffer_t databuf;
	USB_onControlRequestData_t onData;
	USB_request_t request;
	USB_controlState_t stage;
	bool done;
} USB_control_t;

extern USB_control_t USB_control;

#define USB_CONTROL_PIPE 0,USB_control

#define USB_DESCRIPTOR_TABLE_NUM		8
#define USB_DESCRIPTOR_NUM			8

#define USB_DEVICE_DESCRIPTOR_TABLE		0
#define USB_CONFIGURATION_DESCRIPTOR_TABLE	1
#define USB_STRING_DESCRIPTOR_TABLE		2

#define USB_ADESC(X) (X),(sizeof(X))
#define USB_DESC(X) (const uint8_t*)(&(X)),(sizeof(X))

extern USB_buffer_t USB_descriptorTable[USB_DESCRIPTOR_TABLE_NUM][USB_DESCRIPTOR_NUM];

typedef void(*USB_onConfig_t)(uint8_t cfg_num);

extern uint8_t USB_EP_slotNum[16];
extern USB_EP_slot_t USB_EP_slot[USB_EP_SLOT_NUM];

static inline uint8_t USB_getEPSlotByNum(uint8_t ep_num) {
	return USB_EP_slotNum[ep_num & 0x0F];
}

static inline uint8_t USB_getEPNumBySlot(uint8_t ep_slot) {
	return USB->EPR[ep_slot] & ~USB_EPR_TMASK;
}

USB_BTABLE_entry_t* USB_getBTABLEEntry(uint8_t ep_slot);


uint16_t USB_EP_toRead(uint8_t ep_slot);
uint16_t USB_EP_read(uint8_t ep_slot, uint8_t *buf, uint16_t len, uint16_t offset);
uint16_t USB_EP_write(uint8_t ep_slot, const uint8_t *buf, uint16_t len, uint16_t offset);
void USB_EP_writeEmpty(uint8_t ep_slot);
void USB_EP_setTXState(uint8_t ep_slot, uint16_t flags);
void USB_EP_setRXState(uint8_t ep_slot, uint16_t flags);
void USB_EP_setState(uint8_t ep_slot, uint16_t flags);

uint16_t USB_EP_getTXState(uint8_t ep_slot);
uint16_t USB_EP_getRXState(uint8_t ep_slot);

static inline void USB_EP_setTXDone(uint8_t ep_slot) {
#ifdef USB_DOUBLEBUFFER
	if(!USB_EP_slot[ep_slot].db)
#endif
		USB_EP_setTXState(ep_slot, USB_EPR_STAT_TX_VALID);
	// <TODO> Double buffered bulk endpoints
}

static inline void USB_EP_setRXDone(uint8_t ep_slot) {
#ifdef USB_DOUBLEBUFFER
	if(!USB_EP_slot[ep_slot].db)
#endif
		USB_EP_setRXState(ep_slot, USB_EPR_STAT_RX_VALID);
	// <TODO> Double buffered bulk endpoints
}


void USB_reset();
void USB_suspend();
void USB_resume();

void USB_ctlInit();
void USB_ctlH(uint8_t ep_slot, USB_event_t evt);

void USB_ctl_inProto(uint8_t ep_slot, USB_control_t *control);
void USB_ctl_outProto(uint8_t ep_slot, USB_control_t *control);
void USB_ctl_inStatus(uint8_t ep_slot, USB_control_t *control, uint16_t flags);
void USB_ctl_outStatus(uint8_t ep_slot, USB_control_t *control, uint16_t flags);
void USB_ctl_inData(uint8_t ep_slot, USB_control_t *control, const uint8_t *data, uint16_t len);
void USB_ctl_outData(uint8_t ep_slot, USB_control_t *control, uint8_t *data, uint16_t len);
static inline void USB_ctl_outDataH(uint8_t ep_slot, USB_control_t *control, uint8_t *data, uint16_t len, USB_onControlRequestData_t handler) {
	USB_ctl_outData(ep_slot, control, data, len);
	control->onData = handler;
}
static inline void USB_ctl_inDataWait(uint8_t num, USB_control_t *control) {
	control->done = 1;
	control->databuf.length = 0;
	control->stage = USB_CTL_DATA;
	USB_EP_setState(num, USB_EPR_STAT_TX_NAK | USB_EPR_STAT_RX_STALL);
}
#define USB_ctl_inDataBuf(A,B,C) USB_ctl_inData((A),(B),(C).data,(C).length)
#define USB_ctl_outDataBuf(A,B,C) USB_ctl_outData((A),(B),(C).data,(C).length)

static inline void USB_CAN_intDeInit() {
	NVIC_InitTypeDef cfg;
	cfg.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	cfg.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&cfg);
	cfg.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	cfg.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&cfg);
}

static inline void USB_CAN_intInit(int pPri, int sPri) {
	NVIC_InitTypeDef cfg;
	cfg.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	cfg.NVIC_IRQChannelPreemptionPriority = pPri;
	cfg.NVIC_IRQChannelSubPriority = sPri;
	cfg.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&cfg);
	cfg.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	cfg.NVIC_IRQChannelPreemptionPriority = pPri;
	cfg.NVIC_IRQChannelSubPriority = sPri;
	cfg.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&cfg);
}

void USB_init();
void USB_LP_Int();
#define USB_HP_Int USB_LP_Int
void USB_connect(bool);

static inline void USB_clearDescriptors() {
	memset(USB_descriptorTable, 0, sizeof(USB_descriptorTable));
}

static inline void USB_setDescriptor(uint8_t type, uint8_t slot, const uint8_t *data, uint16_t len) {
	USB_descriptorTable[type][slot].data = (uint8_t*)data;
	USB_descriptorTable[type][slot].length = len;
}

void USB_EP_setup(uint8_t ep_slot, uint8_t ep_num, uint16_t flags, uint16_t size_tx, uint16_t size_rx, USB_EP_H_t handler);
#ifdef USB_DOUBLEBUFFER
void USB_EP_setup_db(uint8_t ep_slot, uint8_t ep_num, uint16_t flags, uint16_t size, USB_EP_H_t handler);
#endif
void USB_EP_clearAll();
extern USB_onConfig_t USB_onConfig;
#define USB_ON_CONTROL_REQUEST_NUM 4
extern USB_onControlRequest_t USB_onControlRequest[USB_ON_CONTROL_REQUEST_NUM];


/* Utils */
int USB_EP_getFreeSlot();
static inline int USB_EP_addBidir(uint8_t ep_num, uint16_t flags, uint16_t size_tx, uint16_t size_rx, USB_EP_H_t handler) {
	int r = USB_EP_getFreeSlot();
	if(r == USB_RET_ERROR) return r;
	USB_EP_setup(r, ep_num & 0x0F, flags, size_tx, size_rx, handler);
	return r;
}


/* Unique device ID as serial number */
unsigned USB_UIDRegToDescStr(uint8_t *buf, unsigned len, const char *prefix);
void USB_setStringDescriptorToUIDReg(uint8_t slot, const char *prefix);


static inline unsigned USB_getDevAddr() {
	return USB->DADDR & USB_DADDR_ADD;
}

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_USB_H */
