#include "ControlPlane.h"


uint8_t ControlPlane::parse_trace_mode(char x){
    uint8_t mode = 0;
    switch(x) {
        case 'd': mode =TRACE_ACTIVE_LEVEL_DEBUG;
                  break;
        case 'i': mode =TRACE_ACTIVE_LEVEL_INFO;
                  break;
        case 'w': mode =TRACE_ACTIVE_LEVEL_WARN;
                  break;
        case 'e': mode =TRACE_ACTIVE_LEVEL_ERROR;
                  break;
        case 'c': mode =TRACE_ACTIVE_LEVEL_CMD;
                  break;
    }
    return mode;
}

void ControlPlane::initialize_trace(void) {
    mbed_trace_config_set(trace_mode);
    if(application_init_mbed_trace() != 0) {
        printf("Failed initializing mbed trace\n" );
        return;
    }
}

void ControlPlane::remote_trace_mode_update_cb(void* data) {
    //Process control plane stuffs
    uint8_t mode = 0x00;
    printf("Starting trace callback\n");
    String command_string = trace_res->get_value_string();
    printf("POST received %s\n", command_string.c_str());
    if(command_string.compare(0, 5, "start") == 0){
        //Change trace dynamically
        printf("start trace received\n");
        if(command_string.length() >= 7){
            for(int i = 6; i < command_string.length(); i++){
                mode |= get_trace_mode(command_string[i]);
            }
            printf("Setting trace config %hhx\n", mode);
            mbed_trace_config_set(mode);
            printf("Trace config set\n");
        }else{
            mode = TRACE_ACTIVE_LEVEL_DEBUG;
            mbed_trace_config_set(mode);
        }
        trace_mode = mode;
    } else{
       // application_stop_mbed_trace();
        mode = TRACE_ACTIVE_LEVEL_NONE;
        mbed_trace_config_set(mode);
       // trace_enabled = false;
    }
}

ControlPlane::ControlPlane(MbedCloudClient*const cloudClient) : cloudClient(cloudClient) {
    initialize_trace();

    M2MObject* pinto_control_object = M2MInterfaceFactory::create_object("26250");
    M2MObjectInstance* pinto_control_i = pinto_control_object->create_object_instance();
    trace_res = pinto_control_i->create_dynamic_resource("4015", "tracing", M2MObjectInstance::STRING, false);
    trace_res->set_operation(M2MBase::PUT_POST_ALLOWED);
    trace_res->set_execute_function(execute_callback(this, &ControlPlane::remote_trace_mode_update_cb);

    M2MObjectList obj_list;
    obj_list.push_back(pinto_control_object);
    cloudClient->add_objects(obj_list);

}
