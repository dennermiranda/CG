#ifndef BED_H
#define BED_H

#include "object.h"

class Bed : public Object {
    public:
        Bed();
        Bed(Material);
};

#endif // BED_H
