#ifndef __PINTO_TRACEMODESINGLETON_H_
#define __PINTO_TRACEMODESINGLETON_H_
#include <stdint.h>
#include "mbed_trace.h"

/** \addtogroup control_plane
 *
 * @{
 */

/**
 * Private class representing shared global trace mode.
 */
class TraceModeSingleton {
    public:
        TraceModeSingleton();
        TraceModeSingleton(uint8_t trace_mode);

        /** Allow TraceModeSingleton to be read as if it were an int */
        operator uint8_t() const ;
        /** Allow TraceModeSingleton to be written to as if it were an int */
        TraceModeSingleton& operator =(uint8_t trace_mode);
        //static uint8_t Instance()  { return _trace_mode; }

    private:
        static uint8_t _trace_mode;
};

/**
 * }@
 */

#endif
