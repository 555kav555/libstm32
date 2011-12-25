#ifndef __STM32F10x_USB_H
#define __STM32F10x_USB_H

#define USB_MAXPACKETSIZE0 8 // Windows XP can't recognize the device if packet size equals to 16... Thanks to Microsoft.

#define USB_EP_NUM 8

#include <sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#define STM32DRV_INLINE
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usb.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	USB_EVT_SETUP,
	USB_EVT_IN,
	USB_EVT_OUT
}USB_event_t;

/* Exported constants --------------------------------------------------------*/
#define USB_RegBase  (APB1PERIPH_BASE+0x5C00)  /* USB_IP Peripheral Registers base address */
#define USB_PMAAddr  (APB1PERIPH_BASE+0x6000)  /* USB_IP Packet Memory Area base address   */

/******************************************************************************/
/*                         General registers                                  */
/******************************************************************************/

typedef struct{
	__IO uint32_t EPR[8];
	uint32_t RESERVED0[8];
	__IO uint32_t CNTR;
	__IO uint32_t ISTR;
	__IO uint32_t FNR;
	__IO uint32_t DADDR;
	__IO uint32_t BTABLE;
}USB_TypeDef;

/* Endpoint Buffer Descriptor */
typedef struct{
	__IO uint32_t ADDR_TX;
	__IO uint32_t COUNT_TX;
	__IO uint32_t ADDR_RX;
	__IO uint32_t COUNT_RX;
}USB_BTABLE_entry_t;

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

typedef struct{
	uint8_t *data;
	uint16_t length;
}USB_buffer_t;

typedef void(*USB_EP_H_t)(uint8_t,USB_event_t);

typedef struct{
	USB_EP_H_t handler;
	uint16_t size_tx;
	uint16_t size_rx;
}USB_EP_t;

typedef enum{
	USB_CTL_SETUP,USB_CTL_DATA,USB_CTL_LASTDATA,USB_CTL_EXACTDATA,USB_CTL_STATUS
}USB_controlState_t;

typedef struct{
	USB_buffer_t databuf;
	USB_request_t request;
	USB_controlState_t stage;
	bool done;
}USB_control_t;

extern USB_control_t USB_control;

typedef void(*USB_onControlRequest_t)();

#define USB_CONTROL_PIPE 0,USB_control

#define USB_DESCRIPTOR_TABLE_NUM		8
#define USB_DESCRIPTOR_NUM			8

#define USB_DEVICE_DESCRIPTOR_TABLE		0
#define USB_CONFIGURATION_DESCRIPTOR_TABLE	1
#define USB_STRING_DESCRIPTOR_TABLE		2

#define USB_ADESC(X) (X),(sizeof(X))
#define USB_DESC(X) (const uint8_t*)(&(X)),(sizeof(X))

extern USB_buffer_t USB_descriptorTable[USB_DESCRIPTOR_TABLE_NUM][USB_DESCRIPTOR_NUM];

typedef void(*USB_onConfig_t)(uint8_t);

extern USB_EP_t USB_EP[USB_EP_NUM];

USB_BTABLE_entry_t* USB_getBTABLEEntry(uint8_t num);

void USB_reset();
void USB_suspend();
void USB_resume();

void USB_ctlInit();
void USB_ctlH(uint8_t,USB_event_t evt);

void USB_ctl_inStatus(uint8_t num,USB_control_t *control,uint16_t flags);
void USB_ctl_outStatus(uint8_t num,USB_control_t *control,uint16_t flags);
void USB_ctl_inProto(uint8_t num,USB_control_t *control);
void USB_ctl_outProto(uint8_t num,USB_control_t *control);
void USB_ctl_inData(uint8_t num,USB_control_t *control,const uint8_t *data,uint16_t len);
void USB_ctl_outData(uint8_t num,USB_control_t *control,uint8_t *data,uint16_t len);
#define USB_ctl_inDataBuf(A,B,C) USB_ctl_inData((A),(B),(C).data,(C).length)
#define USB_ctl_outDataBuf(A,B,C) USB_ctl_outData((A),(B),(C).data,(C).length)

static inline void USB_CAN_intDeInit(){
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static inline void USB_CAN_intInit(int pPri,int sPri){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pPri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USB_init();
void USB_LP_Int();
void USB_connect(bool);
void USB_setDescriptor(uint8_t type,uint8_t slot,const uint8_t *data,uint16_t len);
void USB_EP_setup(uint8_t num,uint16_t flags,uint16_t size_tx,uint16_t size_rx,USB_EP_H_t);
void USB_EP_clearAll();
extern USB_onConfig_t USB_onConfig;
#define USB_ON_CONTROL_REQUEST_NUM 4
extern USB_onControlRequest_t USB_onControlRequest[USB_ON_CONTROL_REQUEST_NUM];

uint16_t USB_EP_toRead(uint8_t num);
uint16_t USB_EP_read(uint8_t num,uint8_t *buf,uint16_t len);
uint16_t USB_EP_write(uint8_t num,const uint8_t *buf,uint16_t len);
void USB_EP_TXHandshake(uint8_t num,uint16_t flags);
void USB_EP_RXHandshake(uint8_t num,uint16_t flags);
void USB_EP_handshake(uint8_t num,uint16_t flags);

uint16_t USB_EP_TXState(uint8_t num);
uint16_t USB_EP_RXState(uint8_t num);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_USB_H */
