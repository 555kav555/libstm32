#include "stm32f10x_usb.h"

USB_buffer_t USB_descriptorTable[USB_DESCRIPTOR_TABLE_NUM][USB_DESCRIPTOR_NUM] = {
	[0 ... USB_DESCRIPTOR_TABLE_NUM - 1] = {
		[0 ... USB_DESCRIPTOR_NUM - 1] = {0, 0}
	}
};

USB_control_t USB_control;

static uint8_t config_num=0;
USB_onConfig_t USB_onConfig=NULL;
USB_onControlRequest_t USB_onControlRequest[USB_ON_CONTROL_REQUEST_NUM]={[0 ... USB_ON_CONTROL_REQUEST_NUM-1]=NULL};

static const uint8_t zeroes[]={0,0,0,0,0,0,0,0};
static const uint16_t dev_status=1;

void USB_inData(uint8_t num,USB_buffer_t *buf){
	uint16_t len=buf->length;
	len=USB_EP_write(num,buf->data,len,0);
	buf->data+=len;
	buf->length-=len;
	USB_EP_setState(num,USB_EPR_STAT_TX_VALID|USB_EPR_STAT_RX_STALL);
}

uint16_t USB_outData(uint8_t num,USB_buffer_t *buf){
	uint16_t len=buf->length;
	len=USB_EP_read(num,buf->data,len,0);
	buf->data+=len;
	buf->length-=len;
	return len;
}

void USB_ctl_inStatus(uint8_t num,USB_control_t *control,uint16_t flags){
	control->done=1;
	control->stage=USB_CTL_STATUS;
//	USB->EPR[num]|=USB_EPR_EP_KIND&~USB_EPR_TMASK|USB_EPR_IMASK;
	USB_EP_setState(num,flags|USB_EPR_STAT_TX_STALL);
}

void USB_ctl_outStatus(uint8_t num,USB_control_t *control,uint16_t flags){
	control->done=1;
	control->stage=USB_CTL_STATUS;
	USB_getBTABLEEntry(num)->COUNT_TX=0;
	USB_EP_setState(num,flags|USB_EPR_STAT_RX_STALL);
}

void USB_ctl_inProto(uint8_t num,USB_control_t *control){
	control->done=1;
	uint16_t block_size=USB_EP_slot[num].size_tx;
	switch(control->stage){
	case USB_CTL_SETUP:
		return;
	case USB_CTL_STATUS:
		control->stage=USB_CTL_SETUP;
		USB_EP_setRXState(num,USB_EPR_STAT_RX_STALL);
		return;
	case USB_CTL_LASTDATA:
		USB_ctl_inStatus(num,control,USB_EPR_STAT_RX_VALID);
		return;
	case USB_CTL_EXACTDATA:
		if(block_size>=control->databuf.length)control->stage=USB_CTL_LASTDATA;
		break;
	default:
		if(block_size>control->databuf.length)control->stage=USB_CTL_LASTDATA;
	}
	USB_inData(0,&control->databuf);
}

void USB_ctl_outProto(uint8_t num,USB_control_t *control){
	control->done=1;
	switch(control->stage){
	case USB_CTL_SETUP:
		return;
	case USB_CTL_STATUS:
		control->stage=USB_CTL_SETUP;
		USB_EP_setRXState(num,USB_EPR_STAT_RX_STALL);
		return;
	default:{
		USB_outData(0,&control->databuf);
		if(control->databuf.length==0){
			if(control->onData){
				control->onData();
				control->onData=NULL;
			}else{
				USB_ctl_outStatus(num,control,USB_EPR_STAT_TX_VALID);
			}
		}else{
			USB_EP_setState(num,USB_EPR_STAT_RX_VALID|USB_EPR_STAT_TX_STALL);
		}
	}
	}
}

void USB_ctl_inData(uint8_t num,USB_control_t *control,const uint8_t *data,uint16_t len){
	control->done=1;
	control->databuf.data=(uint8_t*)data;
	control->databuf.length=len;
	if(control->databuf.length>=control->request.wLength){//was '>'
		control->databuf.length=control->request.wLength;
		control->stage=USB_CTL_EXACTDATA;
	}else{
		control->stage=USB_CTL_DATA;
	}
	USB_ctl_inProto(num,control);
}

void USB_ctl_outData(uint8_t num,USB_control_t *control,uint8_t *data,uint16_t len){
	control->done=1;
	control->databuf.data=data;
	control->databuf.length=len;
	control->stage=USB_CTL_DATA;
	USB_EP_setState(num,USB_EPR_STAT_RX_VALID|USB_EPR_STAT_TX_STALL);
}

void USB_ctlInit() {
	config_num = 0;
	USB_EP_setup(0, 0,
		USB_EPR_EP_TYPE_CONTROL | USB_EPR_STAT_TX_STALL | USB_EPR_STAT_RX_STALL,
		USB_MAXPACKETSIZE0, USB_MAXPACKETSIZE0, USB_ctlH);
	USB_control.stage = USB_CTL_SETUP;
	USB_control.onData = NULL;
}

void USB_ctlH(uint8_t num,USB_event_t evt){
	USB_control.done=0;
	switch(evt){
	case USB_EVT_SETUP:{
		uint16_t len=USB_EP_toRead(0);
		if(len!=sizeof(USB_request_t)){
			USB_EP_setState(0,USB_EPR_STAT_TX_STALL|USB_EPR_STAT_RX_STALL);
			return;
		}
		USB_EP_read(0,(uint8_t*)&USB_control.request,sizeof(USB_request_t),0);
		int i;
		for(i=0;i<USB_ON_CONTROL_REQUEST_NUM;++i){
			if(USB_onControlRequest[i])USB_onControlRequest[i]();
//			else break;
			if(USB_control.done)return;
		}
		switch(USB_control.request.bmRequestType.Type){
		case USB_REQUEST_TYPE_STANDARD:{
			if(USB_control.request.bmRequestType.Dir){
				switch(USB_control.request.bRequest){
				case USB_REQUEST_GET_STATUS:
					if(USB_control.request.bmRequestType.Recipient==USB_REQUEST_TYPE_TO_DEVICE){
						USB_ctl_inData(0,&USB_control,(const uint8_t*)&dev_status,2);
					}else{
						USB_ctl_inData(0,&USB_control,zeroes,2);
					}
					break;
				case USB_REQUEST_GET_DESCRIPTOR:{
					switch(USB_control.request.wValue.H){
					case USB_DEVICE_DESCRIPTOR_TYPE:
						USB_ctl_inDataBuf(0, &USB_control,
							USB_descriptorTable[USB_DEVICE_DESCRIPTOR_TABLE]
							[0]);
						break;
					case USB_CONFIGURATION_DESCRIPTOR_TYPE:
						if(USB_control.request.wValue.L >= USB_DESCRIPTOR_NUM) break;
						USB_ctl_inDataBuf(0, &USB_control,
							USB_descriptorTable[USB_CONFIGURATION_DESCRIPTOR_TABLE]
							[USB_control.request.wValue.L]);
						break;
					case USB_STRING_DESCRIPTOR_TYPE:
						if(USB_control.request.wValue.L >= USB_DESCRIPTOR_NUM) break;
						USB_ctl_inDataBuf(0, &USB_control,
							USB_descriptorTable[USB_STRING_DESCRIPTOR_TABLE]
							[USB_control.request.wValue.L]);
						break;
					default:
						break;
					}
				}break;
				case USB_REQUEST_GET_CONFIGURATION:
						USB_ctl_inData(0,&USB_control,&config_num,1);
					break;
				case USB_REQUEST_GET_INTERFACE:
				case USB_REQUEST_SYNC_FRAME:
				default:
					break;
				}
			}else{
				switch(USB_control.request.bRequest){
				case USB_REQUEST_SET_ADDRESS:
					//patience, just after STATUS stage
					USB_ctl_outStatus(0,&USB_control,USB_EPR_STAT_TX_VALID);
					break;
				case USB_REQUEST_SET_CONFIGURATION:
					config_num=USB_control.request.wValue.L;
					USB_EP_clearAll();
					if(USB_onConfig)USB_onConfig(config_num);
					USB_ctl_outStatus(0,&USB_control,USB_EPR_STAT_TX_VALID);
					break;
				case USB_REQUEST_CLEAR_FEATURE:
				case USB_REQUEST_SET_FEATURE:
				case USB_REQUEST_SET_DESCRIPTOR:
				case USB_REQUEST_SET_INTERFACE:
				default:
					break;
				}
			}
		}break;
		default:
			break;
		}
		if(USB_control.done)return;
		if(USB_control.request.bmRequestType.Dir){
			USB_ctl_inStatus(0,&USB_control,USB_EPR_STAT_RX_STALL);
		}else{
			USB_ctl_outStatus(0,&USB_control,USB_EPR_STAT_TX_STALL);
		}
	}break;
	case USB_EVT_IN:
		if((USB_control.stage==USB_CTL_STATUS) && (USB_control.request.wRequestPattern==USB_REQUEST_PAT_SET_ADDRESS)){
			USB->DADDR=USB_control.request.wValue.L|USB_DADDR_EF;
		}
		USB_ctl_inProto(0,&USB_control);
		break;
	case USB_EVT_OUT:
		USB_ctl_outProto(0,&USB_control);
		break;
	}
}

/*
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )
*/

void USB_LP_Int(){
	uint32_t istr, num, val;

	istr = USB->ISTR;

	/* USB Reset Request */
	if (istr & USB_ISTR_RESET) {
		USB_reset();
#if USB_RESET_EVENT
		USB_Reset_Event();
#endif
		USB->ISTR = ~USB_ISTR_RESET;
	}

	/* USB Suspend Request */
	if (istr & USB_ISTR_SUSP) {
		USB_suspend();
#if USB_SUSPEND_EVENT
		USB_Suspend_Event();
#endif
		USB->ISTR = ~USB_ISTR_SUSP;
	}

	/* USB Wakeup */
	if (istr & USB_ISTR_WKUP) {
		USB_resume();
#if USB_WAKEUP_EVENT
		USB_WakeUp_Event();
#endif
		USB->ISTR = ~USB_ISTR_WKUP;
	}

	/* Start of Frame */
	if (istr & USB_ISTR_SOF) {
#if USB_SOF_EVENT
		USB_SOF_Event();
#endif
		USB->ISTR = ~USB_ISTR_SOF;
	}

#if USB_ERROR_EVENT
	/* PMA Over/underrun */
	if (istr & USB_ISTR_PMAOVR) {
		USB_Error_Event(1);
		USB->ISTR = ~USB_ISTR_PMAOVR;
	}

	/* Error: No Answer, CRC Error, Bit Stuff Error, Frame Format Error */
	if (istr & USB_ISTR_ERR) {
		USB_Error_Event(0);
		USB->ISTR = ~USB_ISTR_ERR;
	}
#endif

	/* Endpoint Interrupts */
	while((istr=USB->ISTR)&USB_ISTR_CTR){
//		USB->ISTR = ~USB_ISTR_CTR;

		num = USB_getEPSlotByNum(istr & USB_ISTR_EP_ID);
		if(num >= USB_EP_SLOT_NUM) {
			// something goes totally wrong, bailing out
			NVIC_SystemReset();
			while(1);
		}

		val=USB->EPR[num];
		if(istr&USB_ISTR_DIR){
			USB->EPR[num]=(val&~(USB_EPR_CTR_RX|USB_EPR_TMASK))|USB_EPR_CTR_TX;
			if(USB_EP_slot[num].handler){
				if (val&USB_EPR_SETUP) {
					USB_EP_slot[num].handler(num,USB_EVT_SETUP);
				}else{
					USB_EP_slot[num].handler(num,USB_EVT_OUT);
				}
			}
		}else{
			USB->EPR[num]=(val&~(USB_EPR_CTR_TX|USB_EPR_TMASK))|USB_EPR_CTR_RX;
			if(USB_EP_slot[num].handler){
				USB_EP_slot[num].handler(num,USB_EVT_IN);
			}
		}
	}
}
