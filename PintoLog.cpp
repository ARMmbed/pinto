#include "PintoLog.h"

bool mbed_fault_occurred = false;
mbed_error_ctx error_ctx;
mbed_fault_context_t fault_ctx;

//Retarget the console
//TODO Wrap in an IFDEF
#if MBED_CONF_PINTO_ENABLE
mbed::FileHandle *mbed::mbed_override_console(int fd){
    return &ConsoleSingleton::Instance();
}
#endif

//Gets called when mbed reboots
void mbed_error_reboot_callback(mbed_error_ctx *error_context) {
    mbed_fault_occurred = true;
    mbed_get_reboot_error_info(&error_ctx);

}
