#ifndef LAMP_H
#define LAMP_H

#include "object.h"

class Lamp : public Object {
    public:
        Lamp();
        Lamp(Material);
};

#endif // LAMP_H
