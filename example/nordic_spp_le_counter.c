/*
 * Copyright (C) 2014 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at 
 * contact@bluekitchen-gmbh.com
 *
 */

#define __BTSTACK_FILE__ "nordic_le_counter.c"

// *****************************************************************************
/* EXAMPLE_START(nordic_le_counter): LE Peripheral - Nordic SPP-like profile
 *
 */
 // *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nordic_spp_le_counter.h"
#include "btstack.h"
#include "ble/gatt-service/nordic_spp_service_server.h"

#define HEARTBEAT_PERIOD_MS 1000

/* @section Main Application Setup
 *
 * @text Listing MainConfiguration shows main application code.
 * It initializes L2CAP, the Security Manager and configures the ATT Server with the pre-compiled
 * ATT Database generated from $nordic_le_counter.gatt$. 
 * Additionally, it enables the Battery Service Server with the current battery level.
 * Finally, it configures the advertisements 
 * and the heartbeat handler and boots the Bluetooth stack. 
 * In this example, the Advertisement contains the Flags attribute and the device name.
 * The flag 0x06 indicates: LE General Discoverable Mode and BR/EDR not supported.
 */
 
/* LISTING_START(MainConfiguration): Init L2CAP SM ATT Server and start heartbeat timer */
static btstack_timer_source_t heartbeat;
static hci_con_handle_t con_handle = HCI_CON_HANDLE_INVALID;
static btstack_context_callback_registration_t send_request;
static btstack_packet_callback_registration_t  hci_event_callback_registration;

const uint8_t adv_data[] = {
    // Flags general discoverable, BR/EDR not supported
    2, BLUETOOTH_DATA_TYPE_FLAGS, 0x06, 
    // Name
    8, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'n', 'R', 'F',' ', 'S', 'P', 'P',
    // UUID ...
    17, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS, 0x9e, 0xca, 0xdc, 0x24, 0xe, 0xe5, 0xa9, 0xe0, 0x93, 0xf3, 0xa3, 0xb5, 0x1, 0x0, 0x40, 0x6e,
};
const uint8_t adv_data_len = sizeof(adv_data);

/* LISTING_END */

/*
 * @section Heartbeat Handler
 *
 * @text The heartbeat handler updates the value of the single Characteristic provided in this example,
 * and request a ATT_EVENT_CAN_SEND_NOW to send a notification if enabled see Listing heartbeat.
 */

 /* LISTING_START(heartbeat): Hearbeat Handler */
static int  counter = 0;
static char counter_string[30];
static int  counter_string_len;

static void beat(void){
    counter++;
    counter_string_len = sprintf(counter_string, "BTstack counter %03u", counter);
}

static void nordic_can_send(void * context){
    UNUSED(context);
    printf("SEND: %s\n", counter_string);
    nordic_spp_service_server_send(con_handle, (uint8_t*) counter_string, counter_string_len);
}

static void heartbeat_handler(struct btstack_timer_source *ts){
    if (con_handle != HCI_CON_HANDLE_INVALID) {
        beat();
        send_request.callback = &nordic_can_send;
        nordic_spp_service_server_request_can_send_now(&send_request, con_handle);
    }
    btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(ts);
} 
/* LISTING_END */

static void nordic_data_received(hci_con_handle_t tx_con_handle, const uint8_t * data, uint16_t size){
    if (size == 0 && con_handle == HCI_CON_HANDLE_INVALID ){
        con_handle = tx_con_handle;
        printf("Connected with handle 0x%04x\n", con_handle);        
    } else {
        printf("RECV: ");
        printf_hexdump(data, size);
    }
}

/* 
 * @section Packet Handler
 *
 * @text The packet handler is used to:
 *        - stop the counter after a disconnect
 */

/* LISTING_START(packetHandler): Packet Handler */
static void packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            switch (hci_event_packet_get_type(packet)) {
                case HCI_EVENT_DISCONNECTION_COMPLETE:
                    con_handle = HCI_CON_HANDLE_INVALID;
                    break;
                default:
                    break;
            }
            break;
    }
}
/* LISTING_END */


int btstack_main(void);
int btstack_main(void)
{
    // register for HCI events
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    l2cap_init();

    // setup LE device DB
    le_device_db_init();

    // setup SM: Display only
    sm_init();

    // setup ATT server
    att_server_init(profile_data, NULL, NULL);    

    // setup Nordic SPP service
    nordic_spp_service_server_init(&nordic_data_received);

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
    gap_advertisements_enable(1);

    // set one-shot timer
    heartbeat.process = &heartbeat_handler;
    btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
    btstack_run_loop_add_timer(&heartbeat);

    // beat once
    beat();

    // turn on!
	hci_power_control(HCI_POWER_ON);
	    
    return 0;
}
/* EXAMPLE_END */
