#ifndef CIRCULAR_BUFFER_FILE_H
#define CIRCULAR_BUFFER_FILE_H
#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "CircularLogBuffer.h"
#include "platform/PlatformMutex.h"
#include "Observable.h"

namespace mbed {
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
    void observe(Observable* observer);
    void notify_observer_full(void* data, size_t size);
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
};

}
#endif /* CIRCULAR_BUFFER_FILE_H */
