
static avdtp_context_t a2dp_source_context;


./src/classic/avdtp_source.c|52| static avdtp_context_t * avdtp_source_context;
```c

/e/btstack-master/src/classic/avdtp.h:522:
typedef struct {
    btstack_linked_list_t connections;
    btstack_linked_list_t stream_endpoints;
    uint16_t stream_endpoints_id_counter;
    uint16_t initiator_transaction_id_counter;
    btstack_packet_handler_t avdtp_callback;
    btstack_packet_handler_t a2dp_callback;
    void (*handle_media_data)(uint8_t local_seid, uint8_t *packet, uint16_t size);
    btstack_packet_handler_t packet_handler;
    
    avdtp_sep_type_t query_role;
    
    // SDP query
    uint16_t avdtp_cid;
    uint16_t avdtp_l2cap_psm;
    uint16_t avdtp_version;
    uint8_t  role_supported;
} avdtp_context_t;


```c

/e/btstack-master/src/classic/avdtp.h:450:
typedef struct avdtp_stream_endpoint {
    btstack_linked_item_t    item;
    
    // original capabilities
    avdtp_sep_t sep;
    avdtp_sep_t remote_sep;
    hci_con_handle_t media_con_handle;
    uint16_t l2cap_media_cid;
    uint16_t l2cap_reporting_cid;
    uint16_t l2cap_recovery_cid;

    avdtp_stream_endpoint_state_t  state;
    avdtp_acceptor_stream_endpoint_state_t  acceptor_config_state;
    avdtp_initiator_stream_endpoint_state_t initiator_config_state;
    a2dp_state_t a2dp_state;
    // active connection
    avdtp_connection_t * connection;
    // currently active remote seid
    // uint8_t remote_sep_index;
    avdtp_capabilities_t remote_capabilities;
    uint16_t remote_capabilities_bitmap;
    
    uint16_t remote_configuration_bitmap;
    avdtp_capabilities_t remote_configuration;  

    // temporary SBC config
    avdtp_media_codec_type_t media_codec_type;
    avdtp_media_type_t media_type;
    uint8_t media_codec_sbc_info[4];

    // temporary reconfigure SBC config used by A2DP
    uint8_t              reconfigure_media_codec_sbc_info[4];

    // preferred sampling frequency
    uint32_t preferred_sampling_frequency; 

    // register request for media L2cap connection release
    uint8_t media_disconnect;
    uint8_t media_connect;
    uint8_t start_stream;
    uint8_t stop_stream;
    uint8_t send_stream;
    uint8_t abort_stream;
    uint8_t suspend_stream;
    uint16_t sequence_number;
} avdtp_stream_endpoint_t;

```

```

```c

/e/btstack-master/src/classic/a2dp_source.c:461:
void a2dp_source_init(void){
    avdtp_source_init(&a2dp_source_context);
}

```

```c

/e/btstack-master/src/classic/avdtp_source.c:164:
void avdtp_source_init(avdtp_context_t * avdtp_context){
    if (!avdtp_context){
        log_error("avdtp_source_context is NULL");
        return;
    }
    avdtp_source_context = avdtp_context;
    avdtp_source_context->stream_endpoints = NULL;
    avdtp_source_context->connections = NULL;
    avdtp_source_context->stream_endpoints_id_counter = 0;
    avdtp_source_context->packet_handler = packet_handler;

    l2cap_register_service(&packet_handler, BLUETOOTH_PROTOCOL_AVDTP, 0xffff, LEVEL_2);
}
 

```


```c

/e/btstack-master/src/classic/a2dp_source.c:465:
avdtp_stream_endpoint_t * a2dp_source_create_stream_endpoint(avdtp_media_type_t media_type, avdtp_media_codec_type_t media_codec_type, 
    uint8_t * codec_capabilities, uint16_t codec_capabilities_len,
    uint8_t * media_codec_info, uint16_t media_codec_info_len){
    avdtp_stream_endpoint_t * local_stream_endpoint = avdtp_source_create_stream_endpoint(AVDTP_SOURCE, media_type);
    if (!local_stream_endpoint){
        return NULL;
    }
    avdtp_source_register_media_transport_category(avdtp_stream_endpoint_seid(local_stream_endpoint));
    avdtp_source_register_media_codec_category(avdtp_stream_endpoint_seid(local_stream_endpoint), media_type, media_codec_type, 
        codec_capabilities, codec_capabilities_len);
    local_stream_endpoint->remote_configuration.media_codec.media_codec_information     = media_codec_info;
    local_stream_endpoint->remote_configuration.media_codec.media_codec_information_len = media_codec_info_len;
    sc.local_stream_endpoint = local_stream_endpoint;                     
    return local_stream_endpoint;
}

```




































