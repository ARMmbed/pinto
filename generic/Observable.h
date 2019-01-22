#ifndef __PINTO_OBSERVABLE_H__
#define __PINTO_OBSERVABLE_H__

/** Base observable object
 *
 */
class Observable {
    public:
        virtual ~Observable() {}
        virtual size_t notify(void* data, size_t len) = 0;
};

#endif
