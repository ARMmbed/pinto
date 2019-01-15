#ifndef __PINTO_WRITERS_H_
#define __PINTO_WRITERS_H_

#include <cstring>
#include "MbedCloudClient.h"

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

template <class WriteObject> 
class Writer : public WriterInterface {
    public:
        virtual ~Writer() {}
        Writer(WriteObject* writer) : writer(writer) {}
        size_t write(const void* data, size_t len) { return writer->write(data, len); } // Default

    private:
        WriteObject* writer;
};

template <> class Writer<M2MResource> : public WriterInterface {
    public:
        Writer(M2MResource* writer) : writer(writer) {}
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

#endif
