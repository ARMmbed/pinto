#ifndef __PINTO_DATAPATH_H
#define __PINTO_DATAPATH_H
#include "Observable.h"
#include "Writers.h"
#include "TraceModeSingleton.h"
#include "mbed.h"
#include "compression.h"

/** \addtogroup datapath
 *
 *  @{
 */
/**
 * Private DataPath object handles the routing from stdout/error console to remote.
 * DataPathWriteObject must has a ->write call
 * This class is only outbound to network
 */
template <class WriteObject, class CompressEngine, template<class> class DataPathWriteObject=Writer> 
class DataPath : public Observable {
    public:
        DataPath(): writer(new DataPathWriteObject<WriteObject>()), ready(false), pc(USBTX, USBRX, 115200) {}
        ~DataPath() { delete writer; }
        //DataPath(DataPathWriteObject<WriteObject>* writer): writer(writer), ready(false){}
        // Would make this const, but cloud client doesnt like const correctness
        /** Write data to WriteObjects
         *  Disable mbed trace temporarily to prevent forkbomb-esque logging.
         */
        size_t write(void * data, size_t len){
            size_t len_written = 0;
            size_t len_compress = 0;
            void* compressed_data = NULL;
            // Dont over trace
            mbed_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
            if (is_ready())
                len_compress = compress(data, len, compressed_data);
                len_written = writer->write(compressed_data, len_compress);
            mbed_trace_config_set(trace_mode);
            pc.puts(static_cast<char*>(data));
            return len_written;
        }
        /** Callback for observer pattern.  
         *
         * @param data Raw data to write
         * @param len Number of bytes to write
         * @return number of bytes written
         */
        virtual size_t notify(void* data, size_t len) { return write(data, len); }
        
        /** Register optional client into data path
         *
         * @param client Raw pointer to client instance
         */
        virtual void init(void* client) { writer->init(client); ready = true; }
        bool is_ready() const { return ready; }
        void set_ready(bool ready) { this->ready = ready; }

    private:
        DataPathWriteObject<WriteObject> *const writer;
        TraceModeSingleton trace_mode;
        bool ready;
        Serial pc;
        CompressEngine compress;
};

/** @}
*/
//template <class WriteObject>
//size_t DataPath<WriteObject>::write(void * data, size_t len) 
//{ 
//}

#endif
