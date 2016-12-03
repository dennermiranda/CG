#ifndef CHAIR_H
#define CHAIR_H

#include "object.h"

class Chair : public Object {
    public:
        Chair();
        Chair(Material);
};

#endif // CHAIR_H
