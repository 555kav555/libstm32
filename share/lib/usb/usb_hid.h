/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 *      Name:    HID.H
 *      Purpose: USB HID (Human Interface Device) Definitions
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *
 *      Copyright (c) 2005-2007 Keil Software.
 *---------------------------------------------------------------------------*/

#ifndef __USB_HID_H
#define __USB_HID_H


#include "usb.h"


#define USB_HID_DESC_OFFSET              0x0012
#define USB_HID_DESC_SIZE               (sizeof(USB_HID_descriptor_t))


/* HID Subclass Codes */
#define USB_HID_SUBCLASS_NONE               0x00
#define USB_HID_SUBCLASS_BOOT               0x01

/* HID Protocol Codes */
#define USB_HID_PROTOCOL_NONE               0x00
#define USB_HID_PROTOCOL_KEYBOARD           0x01
#define USB_HID_PROTOCOL_MOUSE              0x02


/* HID Descriptor Types */
#define USB_HID_DESCRIPTOR_TYPE         0x21
#define USB_HID_REPORT_DESCRIPTOR_TYPE      0x22
#define USB_HID_PHYSICAL_DESCRIPTOR_TYPE    0x23


/* HID Descriptor */
typedef struct __attribute__((packed)){
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint16_t  bcdHID;
  uint8_t  bCountryCode;
  uint8_t  bNumDescriptors;
  /* Array of one or more descriptors */
  struct __attribute__((packed)){
    uint8_t  bDescriptorType;
    uint16_t  wDescriptorLength;
  }DescriptorList[1];
}USB_HID_descriptor_t;


/* HID Request Codes */
#define USB_HID_REQUEST_GET_REPORT          0x01
#define USB_HID_REQUEST_GET_IDLE            0x02
#define USB_HID_REQUEST_GET_PROTOCOL        0x03
#define USB_HID_REQUEST_SET_REPORT          0x09
#define USB_HID_REQUEST_SET_IDLE            0x0A
#define USB_HID_REQUEST_SET_PROTOCOL        0x0B

/* HID Report Types */
#define USB_HID_REPORT_INPUT                0x01
#define USB_HID_REPORT_OUTPUT               0x02
#define USB_HID_REPORT_FEATURE              0x03


/* Usage Pages */
#define USB_HID_USAGE_PAGE_UNDEFINED        0x00
#define USB_HID_USAGE_PAGE_GENERIC          0x01
#define USB_HID_USAGE_PAGE_SIMULATION       0x02
#define USB_HID_USAGE_PAGE_VR               0x03
#define USB_HID_USAGE_PAGE_SPORT            0x04
#define USB_HID_USAGE_PAGE_GAME             0x05
#define USB_HID_USAGE_PAGE_DEV_CONTROLS     0x06
#define USB_HID_USAGE_PAGE_KEYBOARD         0x07
#define USB_HID_USAGE_PAGE_LED              0x08
#define USB_HID_USAGE_PAGE_BUTTON           0x09
#define USB_HID_USAGE_PAGE_ORDINAL          0x0A
#define USB_HID_USAGE_PAGE_TELEPHONY        0x0B
#define USB_HID_USAGE_PAGE_CONSUMER         0x0C
#define USB_HID_USAGE_PAGE_DIGITIZER        0x0D
#define USB_HID_USAGE_PAGE_UNICODE          0x10
#define USB_HID_USAGE_PAGE_ALPHANUMERIC     0x14
/* ... */


/* Generic Desktop Page (0x01) */
#define USB_HID_USAGE_GENERIC_POINTER               0x01
#define USB_HID_USAGE_GENERIC_MOUSE                 0x02
#define USB_HID_USAGE_GENERIC_JOYSTICK              0x04
#define USB_HID_USAGE_GENERIC_GAMEPAD               0x05
#define USB_HID_USAGE_GENERIC_KEYBOARD              0x06
#define USB_HID_USAGE_GENERIC_KEYPAD                0x07
#define USB_HID_USAGE_GENERIC_X                     0x30
#define USB_HID_USAGE_GENERIC_Y                     0x31
#define USB_HID_USAGE_GENERIC_Z                     0x32
#define USB_HID_USAGE_GENERIC_RX                    0x33
#define USB_HID_USAGE_GENERIC_RY                    0x34
#define USB_HID_USAGE_GENERIC_RZ                    0x35
#define USB_HID_USAGE_GENERIC_SLIDER                0x36
#define USB_HID_USAGE_GENERIC_DIAL                  0x37
#define USB_HID_USAGE_GENERIC_WHEEL                 0x38
#define USB_HID_USAGE_GENERIC_HATSWITCH             0x39
#define USB_HID_USAGE_GENERIC_COUNTED_BUFFER        0x3A
#define USB_HID_USAGE_GENERIC_BYTE_COUNT            0x3B
#define USB_HID_USAGE_GENERIC_MOTION_WAKEUP         0x3C
#define USB_HID_USAGE_GENERIC_VX                    0x40
#define USB_HID_USAGE_GENERIC_VY                    0x41
#define USB_HID_USAGE_GENERIC_VZ                    0x42
#define USB_HID_USAGE_GENERIC_VBRX                  0x43
#define USB_HID_USAGE_GENERIC_VBRY                  0x44
#define USB_HID_USAGE_GENERIC_VBRZ                  0x45
#define USB_HID_USAGE_GENERIC_VNO                   0x46
#define USB_HID_USAGE_GENERIC_SYSTEM_CTL            0x80
#define USB_HID_USAGE_GENERIC_SYSCTL_POWER          0x81
#define USB_HID_USAGE_GENERIC_SYSCTL_SLEEP          0x82
#define USB_HID_USAGE_GENERIC_SYSCTL_WAKE           0x83
#define USB_HID_USAGE_GENERIC_SYSCTL_CONTEXT_MENU   0x84
#define USB_HID_USAGE_GENERIC_SYSCTL_MAIN_MENU      0x85
#define USB_HID_USAGE_GENERIC_SYSCTL_APP_MENU       0x86
#define USB_HID_USAGE_GENERIC_SYSCTL_HELP_MENU      0x87
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_EXIT      0x88
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_SELECT    0x89
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_RIGHT     0x8A
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_LEFT      0x8B
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_UP        0x8C
#define USB_HID_USAGE_GENERIC_SYSCTL_MENU_DOWN      0x8D
/* ... */

/* Simulation Controls Page (0x02) */
/* ... */
#define USB_HID_USAGE_SIMULATION_RUDDER             0xBA
#define USB_HID_USAGE_SIMULATION_THROTTLE           0xBB
/* ... */

/* Virtual Reality Controls Page (0x03) */
/* ... */

/* Sport Controls Page (0x04) */
/* ... */

/* Game Controls Page (0x05) */
/* ... */

/* Generic Device Controls Page (0x06) */
/* ... */

/* Keyboard/Keypad Page (0x07) */

/* Error "keys" */
#define USB_HID_USAGE_KEYBOARD_NOEVENT              0x00
#define USB_HID_USAGE_KEYBOARD_ROLLOVER             0x01
#define USB_HID_USAGE_KEYBOARD_POSTFAIL             0x02
#define USB_HID_USAGE_KEYBOARD_UNDEFINED            0x03

/* Letters */
#define USB_HID_USAGE_KEYBOARD_aA                   0x04
#define USB_HID_USAGE_KEYBOARD_zZ                   0x1D

/* Numbers */
#define USB_HID_USAGE_KEYBOARD_ONE                  0x1E
#define USB_HID_USAGE_KEYBOARD_ZERO                 0x27

#define USB_HID_USAGE_KEYBOARD_RETURN               0x28
#define USB_HID_USAGE_KEYBOARD_ESCAPE               0x29
#define USB_HID_USAGE_KEYBOARD_DELETE               0x2A

/* Funtion keys */
#define USB_HID_USAGE_KEYBOARD_F1                   0x3A
#define USB_HID_USAGE_KEYBOARD_F12                  0x45

#define USB_HID_USAGE_KEYBOARD_PRINT_SCREEN         0x46

/* Modifier Keys */
#define USB_HID_USAGE_KEYBOARD_LCTRL                0xE0
#define USB_HID_USAGE_KEYBOARD_LSHFT                0xE1
#define USB_HID_USAGE_KEYBOARD_LALT                 0xE2
#define USB_HID_USAGE_KEYBOARD_LGUI                 0xE3
#define USB_HID_USAGE_KEYBOARD_RCTRL                0xE4
#define USB_HID_USAGE_KEYBOARD_RSHFT                0xE5
#define USB_HID_USAGE_KEYBOARD_RALT                 0xE6
#define USB_HID_USAGE_KEYBOARD_RGUI                 0xE7
#define USB_HID_USAGE_KEYBOARD_SCROLL_LOCK          0x47
#define USB_HID_USAGE_KEYBOARD_NUM_LOCK             0x53
#define USB_HID_USAGE_KEYBOARD_CAPS_LOCK            0x39

/* ... */

/* LED Page (0x08) */
#define USB_HID_USAGE_LED_NUM_LOCK                  0x01
#define USB_HID_USAGE_LED_CAPS_LOCK                 0x02
#define USB_HID_USAGE_LED_SCROLL_LOCK               0x03
#define USB_HID_USAGE_LED_COMPOSE                   0x04
#define USB_HID_USAGE_LED_KANA                      0x05
#define USB_HID_USAGE_LED_POWER                     0x06
#define USB_HID_USAGE_LED_SHIFT                     0x07
#define USB_HID_USAGE_LED_DO_NOT_DISTURB            0x08
#define USB_HID_USAGE_LED_MUTE                      0x09
#define USB_HID_USAGE_LED_TONE_ENABLE               0x0A
#define USB_HID_USAGE_LED_HIGH_CUT_FILTER           0x0B
#define USB_HID_USAGE_LED_LOW_CUT_FILTER            0x0C
#define USB_HID_USAGE_LED_EQUALIZER_ENABLE          0x0D
#define USB_HID_USAGE_LED_SOUND_FIELD_ON            0x0E
#define USB_HID_USAGE_LED_SURROUND_FIELD_ON         0x0F
#define USB_HID_USAGE_LED_REPEAT                    0x10
#define USB_HID_USAGE_LED_STEREO                    0x11
#define USB_HID_USAGE_LED_SAMPLING_RATE_DETECT      0x12
#define USB_HID_USAGE_LED_SPINNING                  0x13
#define USB_HID_USAGE_LED_CAV                       0x14
#define USB_HID_USAGE_LED_CLV                       0x15
#define USB_HID_USAGE_LED_RECORDING_FORMAT_DET      0x16
#define USB_HID_USAGE_LED_OFF_HOOK                  0x17
#define USB_HID_USAGE_LED_RING                      0x18
#define USB_HID_USAGE_LED_MESSAGE_WAITING           0x19
#define USB_HID_USAGE_LED_DATA_MODE                 0x1A
#define USB_HID_USAGE_LED_BATTERY_OPERATION         0x1B
#define USB_HID_USAGE_LED_BATTERY_OK                0x1C
#define USB_HID_USAGE_LED_BATTERY_LOW               0x1D
#define USB_HID_USAGE_LED_SPEAKER                   0x1E
#define USB_HID_USAGE_LED_HEAD_SET                  0x1F
#define USB_HID_USAGE_LED_HOLD                      0x20
#define USB_HID_USAGE_LED_MICROPHONE                0x21
#define USB_HID_USAGE_LED_COVERAGE                  0x22
#define USB_HID_USAGE_LED_NIGHT_MODE                0x23
#define USB_HID_USAGE_LED_SEND_CALLS                0x24
#define USB_HID_USAGE_LED_CALL_PICKUP               0x25
#define USB_HID_USAGE_LED_CONFERENCE                0x26
#define USB_HID_USAGE_LED_STAND_BY                  0x27
#define USB_HID_USAGE_LED_CAMERA_ON                 0x28
#define USB_HID_USAGE_LED_CAMERA_OFF                0x29
#define USB_HID_USAGE_LED_ON_LINE                   0x2A
#define USB_HID_USAGE_LED_OFF_LINE                  0x2B
#define USB_HID_USAGE_LED_BUSY                      0x2C
#define USB_HID_USAGE_LED_READY                     0x2D
#define USB_HID_USAGE_LED_PAPER_OUT                 0x2E
#define USB_HID_USAGE_LED_PAPER_JAM                 0x2F
#define USB_HID_USAGE_LED_REMOTE                    0x30
#define USB_HID_USAGE_LED_FORWARD                   0x31
#define USB_HID_USAGE_LED_REVERSE                   0x32
#define USB_HID_USAGE_LED_STOP                      0x33
#define USB_HID_USAGE_LED_REWIND                    0x34
#define USB_HID_USAGE_LED_FAST_FORWARD              0x35
#define USB_HID_USAGE_LED_PLAY                      0x36
#define USB_HID_USAGE_LED_PAUSE                     0x37
#define USB_HID_USAGE_LED_RECORD                    0x38
#define USB_HID_USAGE_LED_ERROR                     0x39
#define USB_HID_USAGE_LED_SELECTED_INDICATOR        0x3A
#define USB_HID_USAGE_LED_IN_USE_INDICATOR          0x3B
#define USB_HID_USAGE_LED_MULTI_MODE_INDICATOR      0x3C
#define USB_HID_USAGE_LED_INDICATOR_ON              0x3D
#define USB_HID_USAGE_LED_INDICATOR_FLASH           0x3E
#define USB_HID_USAGE_LED_INDICATOR_SLOW_BLINK      0x3F
#define USB_HID_USAGE_LED_INDICATOR_FAST_BLINK      0x40
#define USB_HID_USAGE_LED_INDICATOR_OFF             0x41
#define USB_HID_USAGE_LED_FLASH_ON_TIME             0x42
#define USB_HID_USAGE_LED_SLOW_BLINK_ON_TIME        0x43
#define USB_HID_USAGE_LED_SLOW_BLINK_OFF_TIME       0x44
#define USB_HID_USAGE_LED_FAST_BLINK_ON_TIME        0x45
#define USB_HID_USAGE_LED_FAST_BLINK_OFF_TIME       0x46
#define USB_HID_USAGE_LED_INDICATOR_COLOR           0x47
#define USB_HID_USAGE_LED_RED                       0x48
#define USB_HID_USAGE_LED_GREEN                     0x49
#define USB_HID_USAGE_LED_AMBER                     0x4A
#define USB_HID_USAGE_LED_GENERIC_INDICATOR         0x4B

/* Button Page (0x09) */
/*   There is no need to label these usages. */

/* Ordinal Page (0x0A) */
/*   There is no need to label these usages. */

/* Telephony Device Page (0x0B) */
#define USB_HID_USAGE_TELEPHONY_PHONE               0x01
#define USB_HID_USAGE_TELEPHONY_ANSWERING_MACHINE   0x02
#define USB_HID_USAGE_TELEPHONY_MESSAGE_CONTROLS    0x03
#define USB_HID_USAGE_TELEPHONY_HANDSET             0x04
#define USB_HID_USAGE_TELEPHONY_HEADSET             0x05
#define USB_HID_USAGE_TELEPHONY_KEYPAD              0x06
#define USB_HID_USAGE_TELEPHONY_PROGRAMMABLE_BUTTON 0x07
/* ... */

/* Consumer Page (0x0C) */
#define USB_HID_USAGE_CONSUMER_CONTROL              0x01
/* ... */

/* and others ... */


/* HID Report Item Macros */

/* Main Items */
#define USB_HID_Input(x)           0x81,x
#define USB_HID_Output(x)          0x91,x
#define USB_HID_Feature(x)         0xB1,x
#define USB_HID_Collection(x)      0xA1,x
#define USB_HID_EndCollection      0xC0

/* Data (Input, Output, Feature) */
#define USB_HID_Data               0<<0
#define USB_HID_Constant           1<<0
#define USB_HID_Array              0<<1
#define USB_HID_Variable           1<<1
#define USB_HID_Absolute           0<<2
#define USB_HID_Relative           1<<2
#define USB_HID_NoWrap             0<<3
#define USB_HID_Wrap               1<<3
#define USB_HID_Linear             0<<4
#define USB_HID_NonLinear          1<<4
#define USB_HID_PreferredState     0<<5
#define USB_HID_NoPreferred        1<<5
#define USB_HID_NoNullPosition     0<<6
#define USB_HID_NullState          1<<6
#define USB_HID_NonVolatile        0<<7
#define USB_HID_Volatile           1<<7

/* Collection Data */
#define USB_HID_Physical           0x00
#define USB_HID_Application        0x01
#define USB_HID_Logical            0x02
#define USB_HID_Report             0x03
#define USB_HID_NamedArray         0x04
#define USB_HID_UsageSwitch        0x05
#define USB_HID_UsageModifier      0x06

/* Global Items */
#define USB_HID_UsagePage(x)       0x05,x
#define USB_HID_UsagePageVendor(x) 0x06,x,0xFF
#define USB_HID_LogicalMin(x)      0x15,x
#define USB_HID_LogicalMinS(x)     0x16,(x&0xFF),((x>>8)&0xFF)
#define USB_HID_LogicalMinL(x)     0x17,(x&0xFF),((x>>8)&0xFF),((x>>16)&0xFF),((x>>24)&0xFF)
#define USB_HID_LogicalMax(x)      0x25,x
#define USB_HID_LogicalMaxS(x)     0x26,(x&0xFF),((x>>8)&0xFF)
#define USB_HID_LogicalMaxL(x)     0x27,(x&0xFF),((x>>8)&0xFF),((x>>16)&0xFF),((x>>24)&0xFF)
#define USB_HID_PhysicalMin(x)     0x35,x
#define USB_HID_PhysicalMinS(x)    0x36,(x&0xFF),((x>>8)&0xFF)
#define USB_HID_PhysicalMinL(x)    0x37,(x&0xFF),((x>>8)&0xFF),((x>>16)&0xFF),((x>>24)&0xFF)
#define USB_HID_PhysicalMax(x)     0x45,x
#define USB_HID_PhysicalMaxS(x)    0x46,(x&0xFF),((x>>8)&0xFF)
#define USB_HID_PhysicalMaxL(x)    0x47,(x&0xFF),((x>>8)&0xFF),((x>>16)&0xFF),((x>>24)&0xFF)
#define USB_HID_UnitExponent(x)    0x55,x
#define USB_HID_Unit(x)            0x65,x
#define USB_HID_UnitS(x)           0x66,(x&0xFF),((x>>8)&0xFF)
#define USB_HID_UnitL(x)           0x67,(x&0xFF),((x>>8)&0xFF),((x>>16)&0xFF),((x>>24)&0xFF)
#define USB_HID_ReportSize(x)      0x75,x
#define USB_HID_ReportID(x)        0x85,x
#define USB_HID_ReportCount(x)     0x95,x
#define USB_HID_Push               0xA0
#define USB_HID_Pop                0xB0

/* Local Items */
#define USB_HID_Usage(x)           0x09,x
#define USB_HID_UsageMin(x)        0x19,x
#define USB_HID_UsageMax(x)        0x29,x


#endif  /* __USB_HID_H */
