#ifndef AGENT_H
#define AGENT_H

#include "TwoVector.h"

class Agent {
public:
    //Declaring the constructors
    Agent();
    Agent(TwoVector position, TwoVector velocity);

    //Declaring the destructor
    virtual ~Agent();

    //Declaring access methods
    //Get and set methods have been implemented with their declaration for faster executions, as they will be called often, and to differentiate them from other access methods
    //Const used wherever possible (methods where nothing is modified) so that the compiler can perform some optimization

    //Get and set methods for position
    const TwoVector& GetPosition() const {
        return fPosition;
    }

    void SetPosition(const TwoVector& position) {
        fPosition = position;
    }

    //Get and set methods for velocity
    const TwoVector& GetVelocity() const {
        return fVelocity;
    }

    void SetVelocity(const TwoVector& velocity) {
        fVelocity = velocity;
    }

    //Distance from Agent to another position
    double Distance(TwoVector position);

    //Sets a position TwoVector to the closest position to the Agent
    //This is used for the choice to have a looping reality, where birds that go off one side of the display come in on the opposite side of the display
    //Hence, a bird near one side of the screen may need to consider the properties of a bird on the other side of the screen
    //For the position of the other bird to be relevant for each Agents behaviour in the simulation, it is considered as the closest point
    void ClosestPoint(TwoVector& position);

private:
    //Declaring data members
    TwoVector fPosition;
    TwoVector fVelocity;
};
#endif // AGENT_H
