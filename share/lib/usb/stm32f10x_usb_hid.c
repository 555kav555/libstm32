#include "stm32f10x_usb_hid.h"

void USB_HID_controlRequestHandler(){
	switch(USB_control.request.bmRequestType.Type){
	case USB_REQUEST_TYPE_STANDARD:{
		if(USB_control.request.bmRequestType.Dir){
			switch(USB_control.request.bRequest){
			case USB_REQUEST_GET_DESCRIPTOR:{
				switch(USB_control.request.wValue.H){
				case USB_HID_REPORT_DESCRIPTOR_TYPE:
					USB_ctl_inDataBuf(0,&USB_control,
						USB_descriptorTable[USB_HID_REPORT_DESCRIPTOR_TABLE]
						[USB_control.request.wValue.L]);
					break;
				default:
					break;
				}
			}break;
			default:
				break;
			}
		}else{
			switch(USB_control.request.bRequest){
			default:
				break;
			}
		}
	}break;
	default:
		break;
	}
}
