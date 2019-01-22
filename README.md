# Pinto
Pinto is a lightweight log delegation engine designed for constrained devices. All messages written to STDOUT and STDERR get intercepted, processed, and forwarded to any writeable interface (`Serial`, Mbed Cloud Client LWM2M `M2MBase` objects, MQTT, etc.) without having to make any code or test changes. In other words, Pinto enabled devices appear as if they are network-addressable rotating logs located in `/var/log`. 

## Features

### Examples
## Serial Example
This example uses Serial as the DataPlane. This is basically the same behavior found in develop builds
```c++
PintoLogger<Serial> pintoLogger(&mbed_cloud_client);
pintoLogger.init_remote_paths(NULL); //Initialize Data Path Serial doesnt need anything special
//pintoLogger.ready(true); //Writers default to ready.
```

### M2MBase example
This example uses Pelion as the Dataplane
```c++
PintoLogger<M2MResource> pintoLogger(&mbed_cloud_client);
pintoLogger.init_remote_paths(&mbed_cloud_client); //Initialize Data Path
...
//Set ready when Cloud client registered and connected, otherwise logs go nowhere
pintoLogger.ready(true);
```

### User Custom example
Nothing stops us from writing our own DataPathWriters. All they need is a `write(const void* data, size_t len)` method!

```c++
class DoNothing {
    public:
        size_t write(const void* data, size_t len) { return 0; }
};
...

DoNothing slashDevNull;
PintoLogger<DoNothing> pintoLogger(&mbed_cloud_client);
pintoLogger.init_remote_paths(NULL);
```

