#ifndef CIRCULAR_BUFFER_FILE_H
#define CIRCULAR_BUFFER_FILE_H
#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "CircularLogBuffer.h"
#include "platform/PlatformMutex.h"
#include "Observable.h"
#include "mbed.h"

/** \addtogroup datapath
 *  @{
 */
namespace mbed {
    /** Internal Circular Buffer File class to act as retargeted console (STDOUT/STDERR)
     *
     */
class CircularBufferFile : public FileHandle {
public:
    CircularBufferFile();
    virtual ~CircularBufferFile();
    virtual ssize_t write(const void *buffer, size_t size);
    virtual ssize_t read(void *buffer, size_t size);
    virtual off_t seek(off_t offset, int whence);
    virtual off_t size();
    virtual int isatty();
    virtual int close();

    /** Register single observer on this console. 
     *
     * Will be notified on:
     *  - Buffer full event
     *  - timeout event
     *
     *  @param observer Object with a notify(data, len) method
     */
    void observe(Observable* observer);
    void notify_observer_full(void* data, size_t size);
    void notify_observer_timeout(void);
private:
    /** Acquire mutex */
    virtual void api_lock(void);

    /** Release mutex */
    virtual void api_unlock(void);
    
    void wait_ms(uint32_t millisec);
private:
    CircularLogBuffer<char, MBED_CONF_PINTO_CIRCULAR_BUFFER_FILE_DEPTH> _buffer;
    //Callback<void()> _sigio_cb; // Todo
    PlatformMutex _mutex;
    Observable* observer;
    int _call_id;
    EventQueue q;
    Thread t;
};

}

/**
 * }@
 */
#endif /* CIRCULAR_BUFFER_FILE_H */
