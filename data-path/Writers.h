#ifndef __PINTO_WRITERS_H_
#define __PINTO_WRITERS_H_

#include <cstring>
#include "MbedCloudClient.h"
#include "mbed.h"

class WriterInterface {
    public:
        virtual ~WriterInterface();
        //virtual void register_in_client() {}
        virtual size_t write(const void* data, size_t len) = 0;
        /**
         *  Initialize the writer in the client, optional
         */
        virtual void init(void* client);
        virtual bool ready(void);

};

/** Exposed selector class for specializing writers on a particular remote client
 * Write Object must have a write method
 *
 */
template <class WriteObject>
class Writer : public WriterInterface {
    public:
        virtual ~Writer() {
            if (writer)
                delete writer;
        }
        //Writer(WriteObject* writer) : writer(writer) {}
        Writer(): writer(new Writer) {}
        size_t write(const void* data, size_t len) { 
            if (writer)
                return writer->write(data, len); 
            else 
                return -1;
        } // Default

    private:
        WriteObject* writer;
};

/** Specialization of WriteInterface to handle Pelion LWM2M over coap as a data plane.
 *  LWM2M objects do not have a default "write" method. For some reason it's call set_value
 *
 */
template <> class Writer<M2MResource> : public WriterInterface {
    public:
        virtual ~Writer() {};
        //Writer(M2MResource* writer) : writer(writer) {}
        size_t write(const void* data, size_t len) { return writer->set_value(static_cast<const uint8_t*>(data), len);}

        /**
         *  Register log object into Pelion
         */
        virtual void init(void* client) { 
            cloudClient = static_cast<MbedCloudClient*>(client);
            //32768/0/4014
            M2MObject* log_object = M2MInterfaceFactory::create_object("32768");
            M2MObjectInstance* log_instance = log_object->create_object_instance();
            writer = log_instance->create_dynamic_resource("4014", "log", M2MResourceInstance::OPAQUE, true);
            writer->set_operation(M2MBase::GET_ALLOWED);
            M2MObjectList obj_list;
            obj_list.push_back(log_object);
            cloudClient->add_objects(obj_list);
        }

    private:
        M2MResource* writer;
        MbedCloudClient* cloudClient;

};

/** Serial writer for debug mode
 */
template <> class Writer<Serial> : public WriterInterface {
    public:
        virtual ~Writer() {}
        Writer(): pc(USBTX, USBRX, 115200) {}
        size_t write(const void* data, size_t len) { return pc.write(static_cast<const uint8_t*>(data), len, NULL); }

    private:
        Serial pc;

};

#endif
