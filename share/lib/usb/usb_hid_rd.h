#ifndef __USB_HID_RD_H
#define __USB_HID_RD_H

//[][][][] [][] [][]
//  tag    type size

#define F_CONST 1
#define F_VAR 2
#define F_REL 4
#define F_WRAP 8
#define F_NL 16
#define F_NOPREF 32
#define F_NULLST 64
#define F2_BUFFER 1

#define SZ1 1
#define SZ2 2
#define SZ4 3

#define BYTE |1,
#define WORD |2,
#define DWORD |3,

#define PHYSICAL |1,0x00
#define APPLICATION |1,0x01
#define LOGICAL |1,0x02
#define REPORT |1,0x03
#define NAMED_ARRAY |1,0x04
#define USAGE_SWITCH |1,0x05
#define USAGE_MODIFIER |1,0x06
#define VENDOR_COLL |1,0x80

//Main
#define INPUT 0x80
#define OUTPUT 0x90
#define FEATURE 0xB0
#define COLLECTION 0xA0
#define END_COLL 0xC0

//Global
#define USAGE_PAGE 0x04
#define LOGICAL_MIN 0x14
#define LOGICAL_MAX 0x24
#define PHYSICAL_MIN 0x34
#define PHYSICAL_MAX 0x44
#define UNIT_EXP 0x54
#define UNIT 0x64
#define REPORT_SIZE 0x74
#define REPORT_ID 0x84
#define REPORT_COUNT 0x94
#define PUSH 0xA4
#define POP 0xB4

//Local
#define USAGE 0x08
#define USAGE_MIN 0x18
#define USAGE_MAX 0x28
#define DESIGNATOR_ID 0x38
#define DESIGNATOR_MIN 0x48
#define DESIGNATOR_MAX 0x58
#define STRING_ID 0x78
#define STRING_MIN 0x88
#define STRING_MAX 0x98
#define DELIMITER 0xA8

#endif
