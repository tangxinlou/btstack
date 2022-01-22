
static int signaling_responses_pending;

static btstack_linked_list_t l2cap_services;
static btstack_linked_list_t l2cap_channels;
typedef btstack_linked_item_t * btstack_linked_list_t;

static uint8_t require_security_level2_for_outgoing_sdp;

```c

/e/btstack-master/src/btstack_linked_list.h:51:
typedef struct btstack_linked_item {
    struct btstack_linked_item *next; // <-- next element in list, or NULL
} btstack_linked_item_t;

```


static l2cap_fixed_channel_t l2cap_fixed_channel_connectionless;
 typedef struct l2cap_fixed_channel {
    // linked list - assert: first field
    btstack_linked_item_t    item;
    
    // channel type
    l2cap_channel_type_t channel_type;

    // local cid, primary key for channel lookup
    uint16_t  local_cid;

    // packet handler
    btstack_packet_handler_t packet_handler;

    // send request
    uint8_t waiting_for_can_send_now;

    // -- end of shared prefix

} l2cap_fixed_channel_t;
 

```c

/e/btstack-master/src/l2cap.c:744:
void l2cap_init(void){
    signaling_responses_pending = 0;
    
    l2cap_channels = NULL;

#ifdef ENABLE_CLASSIC
    l2cap_services = NULL;
    require_security_level2_for_outgoing_sdp = 0;

    // Setup Connectionless Channel
    l2cap_fixed_channel_connectionless.local_cid     = L2CAP_CID_CONNECTIONLESS_CHANNEL;
    l2cap_fixed_channel_connectionless.channel_type  = L2CAP_CHANNEL_TYPE_CONNECTIONLESS;
    btstack_linked_list_add(&l2cap_channels, (btstack_linked_item_t *) &l2cap_fixed_channel_connectionless);
#endif

#ifdef ENABLE_LE_DATA_CHANNELS
    l2cap_le_services = NULL;
#endif

#ifdef ENABLE_BLE
    l2cap_event_packet_handler = NULL;
    l2cap_le_custom_max_mtu = 0;

    // Setup fixed ATT Channel
    l2cap_fixed_channel_att.local_cid    = L2CAP_CID_ATTRIBUTE_PROTOCOL;
    l2cap_fixed_channel_att.channel_type = L2CAP_CHANNEL_TYPE_LE_FIXED;
    btstack_linked_list_add(&l2cap_channels, (btstack_linked_item_t *) &l2cap_fixed_channel_att);

    // Setup fixed SM Channel
    l2cap_fixed_channel_sm.local_cid     = L2CAP_CID_SECURITY_MANAGER_PROTOCOL;
    l2cap_fixed_channel_sm.channel_type  = L2CAP_CHANNEL_TYPE_LE_FIXED;
    btstack_linked_list_add(&l2cap_channels, (btstack_linked_item_t *) &l2cap_fixed_channel_sm);
#endif
    
    // 
    // register callback with HCI
    //
    hci_event_callback_registration.callback = &l2cap_hci_event_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    hci_register_acl_packet_handler(&l2cap_acl_handler);

#ifdef ENABLE_CLASSIC
    gap_connectable_control(0); // no services yet
#endif
}

```
















