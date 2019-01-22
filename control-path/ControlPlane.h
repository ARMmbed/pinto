#ifndef __PINTO_CONTROLPLANE_H_
#define __PINTO_CONTROLPLANE_H_

#include <stdint.h>
#include "TraceModeSingleton.h"
#include "MbedCloudClient.h"
#include "mbed.h"

//TODO move tracing object into own class
/**
 * \addtogroup control_plane
 *
 * @{
 */

/**
 * private ControlPlane class managed through Pelion.
 * This class gives access to different configuration state, such as:
 * - Trace mode
 * - Compression mode
 * - etc.
 */
class ControlPlane {
    private:
        void initialize_trace(void);
        void remote_trace_mode_update_cb(const char* data);
        uint8_t parse_trace_mode(char x);

    public:
        /**
         * Get reference to Mbed Cloud Client
         */
        ControlPlane(MbedCloudClient*const cloudClient);

        /**
         * Register LWM2M management objects in Pelion
         */
        void init_in_cloud(void);

    private:
        TraceModeSingleton trace_mode;
        M2MResource* trace_res;
        MbedCloudClient*const cloudClient;
};

/**
 * }@
 */

#endif
