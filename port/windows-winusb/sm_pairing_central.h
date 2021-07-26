
// sm_pairing_central.h generated from ../../example/sm_pairing_central.gatt for BTstack
// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
    // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME-READ
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a, 
    // 0x0003 VALUE-GAP_DEVICE_NAME-READ-'SM Pairing Central'
    // READ_ANYBODY
    0x1a, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x2a, 0x53, 0x4d, 0x20, 0x50, 0x61, 0x69, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x43, 0x65, 0x6e, 0x74, 0x72, 0x61, 0x6c, 
    // Dummy Service

    // 0x0004 PRIMARY_SERVICE-0000FF10-0000-1000-8000-00805F9B34FB
    0x18, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x28, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x10, 0xff, 0x00, 0x00, 
    // Dummy Service, Characteristic, with read and notify + authentication
    // 0x0005 CHARACTERISTIC-0000FF12-0000-1000-8000-00805F9B34FB-READ | ENCRYPTION_KEY_SIZE_16
    0x1b, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x02, 0x06, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0xff, 0x00, 0x00, 
    // 0x0006 VALUE-0000FF12-0000-1000-8000-00805F9B34FB-READ | ENCRYPTION_KEY_SIZE_16-'Secure :)'
    // READ_ENCRYPTED, ENCRYPTION_KEY_SIZE=16
    0x1f, 0x00, 0x03, 0xf6, 0x06, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x12, 0xff, 0x00, 0x00, 0x53, 0x65, 0x63, 0x75, 0x72, 0x65, 0x20, 0x3a, 0x29, 

    // END
    0x00, 0x00, 
}; // total size 76 bytes 


//
// list service handle ranges
//
#define ATT_SERVICE_GAP_SERVICE_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_END_HANDLE 0x0003
#define ATT_SERVICE_0000FF10_0000_1000_8000_00805F9B34FB_START_HANDLE 0x0004
#define ATT_SERVICE_0000FF10_0000_1000_8000_00805F9B34FB_END_HANDLE 0x0006

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_0000FF12_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE 0x0006
