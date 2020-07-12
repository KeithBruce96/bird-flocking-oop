#include "Agent.h"

//Implementation of constructors
Agent::Agent() {}

Agent::Agent(TwoVector position, TwoVector velocity)
    :fPosition(position), fVelocity(velocity) {}

//Implementation of destructor
Agent::~Agent() {}

//Implementation of Distance method
double Agent::Distance(TwoVector position) {
    double xDisp = position.X()-fPosition.X();
    double yDisp = position.Y()-fPosition.Y();

    TwoVector Displacement (xDisp, yDisp);

    return Displacement.Mag();
}

//Implementation of ClosestPoint method
void Agent::ClosestPoint(TwoVector& position) {
    //Dimension Limits chosen for simulation
    double DimensionLimits = 1000;
    double dx = fabs(position.X()-fPosition.X());
    double dy = fabs(position.Y()-fPosition.Y());
    double x = position.X();
    double y = position.Y();

    //Minimises the x and y components of distance from the Agent to the position, finding the closest consideration for position
    if ((DimensionLimits-dx) < dx) {
        dx = DimensionLimits-dx;
        x = x - DimensionLimits;
    }

    if((DimensionLimits+dx) < dx) {
        dx = DimensionLimits+dx;
        x = x + DimensionLimits;
    }

    if ((DimensionLimits-dy) < dy) {
        dy = DimensionLimits-dy;
        y = y - DimensionLimits;
    }

    if((DimensionLimits+dy) < dy) {
        dy = DimensionLimits+dy;
        y = y + DimensionLimits;
    }

    //Sets the components of position to their closest to the Agent, considering all possible distances to the point with respect to the looping reality
    position.SetXY(x,y);
}
