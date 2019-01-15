#ifndef __PINTO_TRACEMODESINGLETON_H_
#define __PINTO_TRACEMODESINGLETON_H_
#include <cstdint>

class TraceModeSingleton {
    public:
        TraceModeSingleton();
        TraceModeSingleton(uint8_t trace_mode);
        operator uint8_t() const ;
        TraceModeSingleton& operator =(uint8_t trace_mode);
        //static uint8_t Instance()  { return _trace_mode; }

    private:
        static uint8_t _trace_mode;
};

#endif
