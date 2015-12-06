#include "stm32f10x_usb_cdc.h"

USB_CDC_lineCoding_t USB_CDC_lineCoding={9600,0,0,8};

void USB_CDC_controlRequestHandler(){
	switch(USB_control.request.bmRequestType.Type){
	case USB_REQUEST_TYPE_CLASS:{
		if(USB_control.request.bmRequestType.Dir){
			switch(USB_control.request.bRequest){
			case USB_CDC_REQUEST_GET_LINE_CODING:
				USB_ctl_inData(0,&USB_control,(const uint8_t*)&USB_CDC_lineCoding,sizeof(USB_CDC_lineCoding));
				break;
			default:
				break;
			}
		}else{
			switch(USB_control.request.bRequest){
			case USB_CDC_REQUEST_SET_LINE_CODING:
				USB_ctl_outData(0,&USB_control,(uint8_t*)&USB_CDC_lineCoding,sizeof(USB_CDC_lineCoding));
				break;
			case USB_CDC_REQUEST_SET_CONTROL_LINE_STATE:
				USB_ctl_outStatus(0,&USB_control,USB_EPR_STAT_TX_VALID);
				break;
			default:
				break;
			}
		}
	}break;
	default:
		break;
	}
}
