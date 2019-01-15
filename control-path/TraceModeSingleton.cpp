#include "TraceModeSingleton.h"


//Default to No TRACE active
uint8_t TraceModeSingleton::_trace_mode = TRACE_ACTIVE_LEVEL_NONE;

TraceModeSingleton::TraceModeSingleton() {}
TraceModeSingleton::TraceModeSingleton(uint8_t trace_mode) { TraceModeSingleton::_trace_mode = trace_mode; }
TraceModeSingleton::operator uint8_t() const { return TraceModeSingleton::_trace_mode; }
TraceModeSingleton& TraceModeSingleton::operator =(uint8_t trace_mode) { TraceModeSingleton::_trace_mode = trace_mode; return *this;}

