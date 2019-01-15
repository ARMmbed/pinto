#include "PintoLog.h"

//Retarget the console
//TODO Wrap in an IFDEF
#if MBED_CONF_PINTO_ENABLE
mbed::FileHandle *mbed::mbed_override_console(int fd){
    return &ConsoleSingleton::Instance();
}
#endif
