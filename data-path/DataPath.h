#ifndef __PINTO_DATAPATH_H
#define __PINTO_DATAPATH_H
#include "Observable.h"
#include "Writers.h"
#include "TraceModeSingleton.h"

// DataPathWriteObject must has a ->write call
// This class is only outbound to network
template <class WriteObject, template<class> class DataPathWriteObject=Writer> 
class DataPath : public Observable {
    public:
        DataPath(): writer(NULL), ready(false) {}
        DataPath(DataPathWriteObject<WriteObject>* writer): writer(writer), ready(false) {}
        // Would make this const, but cloud client doesnt like const correctness
        size_t write(void * data, size_t len){
            // Dont over trace
            mbed_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
            size_t len_written = writer->write(data, len);
            mbed_trace_config_set(trace_mode);
            return len_written;
        }
        virtual size_t notify(void* data, size_t len) { return write(data, len); }
        void init(void* client) { writer->init(client); ready = true; }
        bool is_ready() const { return ready; }

    private:
        DataPathWriteObject<WriteObject> *const writer;
        TraceModeSingleton trace_mode;
        bool ready;
};

//template <class WriteObject>
//size_t DataPath<WriteObject>::write(void * data, size_t len) 
//{ 
//}

#endif
