#ifndef GREENBIRD_H
#define GREENBIRD_H

#include "Agent.h"

//Inherits from Agent
class GreenBird: public Agent {
public:
    //Declaring the constructors
    GreenBird();
    GreenBird(TwoVector position, TwoVector velocity);

    //Declaring the destructor
    virtual ~GreenBird();
};

#endif // GREENBIRD_H
