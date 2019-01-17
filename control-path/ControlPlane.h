#ifndef __PINTO_CONTROLPLANE_H_
#define __PINTO_CONTROLPLANE_H_

#include <stdint.h>
#include "TraceModeSingleton.h"
#include "MbedCloudClient.h"
#include "mbed.h"

//TODO move tracing object into own class
class ControlPlane {
    private:
        void initialize_trace(void);
        void remote_trace_mode_update_cb(void* data);
        uint8_t parse_trace_mode(char x);

    public:
        ControlPlane(MbedCloudClient*const cloudClient);
        void init_in_cloud(void);

    private:
        TraceModeSingleton trace_mode;
        M2MResource* trace_res;
        MbedCloudClient*const cloudClient;
};

#endif
