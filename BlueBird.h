#ifndef BLUEBIRD_H
#define BLUEBIRD_H

#include "Agent.h"

//Inherits from Agent
class BlueBird: public Agent {
public:
    //Declaring the constructors
    BlueBird();
    BlueBird(TwoVector position, TwoVector velocity);

    //Declaring the destructor
    virtual ~BlueBird();
};

#endif // BLUEBIRD_H
