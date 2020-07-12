#include <stdlib.h>
#include <time.h>

#include "Flock.h"

//Declaring random function for use in simulation
//Returns a random double value between the given min and max values
double RandomSim(double min, double max);

//Implementation of constructors
Flock::Flock() {}

Flock::Flock(int Blue_Number, int Green_Number, double Blue_Detection, double Green_Detection, double Blue_Separation, double Green_Separation, double Blue_Max_Speed, double Green_Max_Speed, double Blue_Min_Speed, double Green_Min_Speed)
    :fBlue_Number(Blue_Number), fGreen_Number(Green_Number), fBlue_Detection(Blue_Detection), fGreen_Detection(Green_Detection), fBlue_Separation(Blue_Separation), fGreen_Separation(Green_Separation), fBlue_Max_Speed(Blue_Max_Speed), fGreen_Max_Speed(Green_Max_Speed), fBlue_Min_Speed(Blue_Min_Speed), fGreen_Min_Speed(Green_Min_Speed) {}

//Implementation of destructor
Flock::~Flock() {}

//Implementation of Simulate method
void Flock::Simulate(double t) {
    //Dimension Limits chosen for simulation
    double DimensionLimits = 1000;

    //Seeding random number generator
    srand(time(0));

    //Declaring the the macroscopic weights for the system
    //These correspond to the different affecting behaviours for bird velocity and have been manually adjusted to achieve physically realistic results
    //The magnitudes differ significantly as the formulae for each behaviour provide highly varying magnitudes
    //These behaviours ar not simply normalised to unit vectors since their magnitudes are significant as well as their directions
    double Initial_Weight = 80;
    double Separation_Weight = 100;
    double Alignment_Weight = 150;
    double Cohesion_Weight = 6;
    double Other_Bird_Weight = 100;
    double Random_Weight = 0.001;

    //For loop to advance the position and velocity of each BlueBird in the Flock
    for (int i = 0; i < fBlue_Number; i++) {
        //Considering the i-th BlueBird in BlueFlock as the CurrentBird and getting it's Position and Velocity
        BlueBird CurrentBird = fBlueFlock[i];
        TwoVector Position = CurrentBird.GetPosition();
        TwoVector Velocity = CurrentBird.GetVelocity();

        //Declaring default TwoVectors for the behaviours affecting bird velocity
        TwoVector Separation_Velocity = TwoVector();
        TwoVector Alignment_Velocity = TwoVector();
        TwoVector Cohesion_Velocity = TwoVector();
        TwoVector Other_Bird_Velocity = TwoVector();
        TwoVector Random_Velocity = TwoVector();

        //Declaring counting variables for the number of birds causing the different behaviours affecting CurrentBird velocity
        double Separation_Count = 0;
        double Alignment_Count = 0;
        double Cohesion_Count = 0;
        double Other_Bird_Count = 0;

        //Considering the affect of all other BlueBirds on the CurrentBird
        for (int j = 0; j < fBlue_Number; j++) {
            if (j !=i) {
                //Considering the j-th BlueBird in BlueFlock as the BlueOther, getting it's position, finding the closest point of this position to the CurrentBird and setting BlueOther's position accordingly
                //This is done to correctly consider the affect of each BlueBird from it's closest position to the CurrentBird without changing the actual values of position
                BlueBird BlueOther = fBlueFlock[j];
                TwoVector BlueOtherPos = BlueOther.GetPosition();
                CurrentBird.ClosestPoint(BlueOtherPos);
                BlueOther.SetPosition(BlueOtherPos);

                //Applies Separation, the behaviour that causes birds to steer away from nearby birds of the same type, if BlueOther is within the Separation range of CurrentBird
                if (CurrentBird.Distance(BlueOther.GetPosition()) < fBlue_Separation) {
                    //Instance weighting to ensure a stronger steering away from closer birds by a distance squared rule
                    //Factor of 2.5 has been manually adjusted to ensure a strong enough repulsion of nearby birds to avoid collisions using default settings
                    double Separation_Instance_Weight = 2.5*pow(1 - CurrentBird.Distance(BlueOther.GetPosition())/fBlue_Separation,2);

                    //Contributions of the instance to the Separation_Velocity components are calculated as the components of the distance from the CurrentBird multiplied by the instance weight
                    double xContribution = (BlueOther.GetPosition().X()-Position.X())*Separation_Instance_Weight;
                    double yContribution = (BlueOther.GetPosition().Y()-Position.Y())*Separation_Instance_Weight;

                    //Separation_Velocity components increased by the contributions
                    Separation_Velocity.SetX(Separation_Velocity.X()+xContribution);
                    Separation_Velocity.SetY(Separation_Velocity.Y()+yContribution);

                    //Separation_Count of the number of instances increased
                    Separation_Count++;
                }

                //Applies Alignment, the behaviour that causes birds to steer towards the average heading of nearby birds of the same type, if BlueOther is within the Detection range of CurrentBird
                if (CurrentBird.Distance(BlueOther.GetPosition()) < fBlue_Detection) {
                    //Alignment_Velocity components are increased by the contributions, calculated as the components of the velocity of BlueOther
                    Alignment_Velocity.SetX(Alignment_Velocity.X()+BlueOther.GetVelocity().X());
                    Alignment_Velocity.SetY(Alignment_Velocity.Y()+BlueOther.GetVelocity().Y());

                    //Alignment_Count of the number of instances increased
                    Alignment_Count++;
                }

                //Applies Cohesion, the behaviour that causes birds to steer towards the average position of nearby birds of the same type, if BlueOther is within the Detection range and outside of the Separation range of CurrentBird
                if (CurrentBird.Distance(BlueOther.GetPosition()) < fBlue_Detection && CurrentBird.Distance(BlueOther.GetPosition()) > fBlue_Separation) {
                    //Cohesion_Velocity components are increased by the contributions, calculated as the components of the position of BlueOther
                    Cohesion_Velocity.SetX(Cohesion_Velocity.X()+BlueOther.GetPosition().X());
                    Cohesion_Velocity.SetY(Cohesion_Velocity.Y()+BlueOther.GetPosition().Y());

                    //Cohesion_Count of the number of instances increased
                    Cohesion_Count++;
                }
            }
        }

        //Considering the affect of all GreenBirds on the CurrentBird
        for (int k = 0; k < fGreen_Number; k++) {
            //Considering the k-th GreenBird in GreenFlock as the GreenOther, getting it's position, finding the closest point of this position to the CurrentBird and setting GreenOther's position accordingly
            //This is done to correctly consider the affect of each GreenBird from it's closest position to the CurrentBird without changing the actual values of position
            GreenBird GreenOther = fGreenFlock[k];
            TwoVector GreenOtherPos = GreenOther.GetPosition();
            CurrentBird.ClosestPoint(GreenOtherPos);
            GreenOther.SetPosition(GreenOtherPos);

            //Applies Other_Bird Separation, the behaviour that causes birds to steer away from nearby birds of a different type, if GreenOther is within the Separation range of CurrentBird
            if (CurrentBird.Distance(GreenOther.GetPosition()) < fBlue_Separation) {
                //Instance weighting to ensure a stronger steering away from closer birds by a distance squared rule
                //Unlike Separation from other BlueBird's, there was no need for a factor to increase this weighting for default settings since consistent collisions were only prevalent once large flock of similar attracting birds had formed
                double Other_Bird_Instance_Weight = pow(1 - CurrentBird.Distance(GreenOther.GetPosition())/fBlue_Separation,2);

                //Contributions of the instance to the Other_Bird_Velocity components are calculated as the components of the distance from the CurrentBird multiplied by the instance weight
                double xContribution = (GreenOther.GetPosition().X()-Position.X())*Other_Bird_Instance_Weight;
                double yContribution = (GreenOther.GetPosition().Y()-Position.Y())*Other_Bird_Instance_Weight;

                //Other_Bird_Velocity components increased by the contributions
                Other_Bird_Velocity.SetX(Other_Bird_Velocity.X()+xContribution);
                Other_Bird_Velocity.SetY(Other_Bird_Velocity.Y()+yContribution);

                //Other_Bird_Count of the number of instances increased
                Other_Bird_Count++;
            }
        }

        //If there were any instances causing Separation the components of Separation_Velocity are inversed, to point away from the positions of nearby birds of the same type, and divided by the Separation_Count
        if (Separation_Count > 0) {
            Separation_Velocity.SetXY(-Separation_Velocity.X()/Separation_Count,-Separation_Velocity.Y()/Separation_Count);
        }

        //If there were any instances causing Alignment the components of Alignment_Velocity are divided by the Alignment_Count
        if (Alignment_Count > 0) {
            Alignment_Velocity.SetXY(Alignment_Velocity.X()/Alignment_Count,Alignment_Velocity.Y()/Alignment_Count);
        }

        //If there were any instances causing Cohesion the components of Cohesion_Velocity are divided by the Cohesion_Count and then the CurrentBird Position components are subtracted, to give the direction towards the average position of nearby birds of the same type rather than the average position itself
        if (Cohesion_Count > 0) {
            Cohesion_Velocity.SetXY(Cohesion_Velocity.X()/Cohesion_Count-Position.X(),Cohesion_Velocity.Y()/Cohesion_Count-Position.Y());
        }

        //If there were any instances causing Other_Bird Separation the components of Other_Bird_Velocity are inversed, to point away from the positions of nearby birds of a different type, and divided by the Other_Bird_Count
        if (Other_Bird_Count > 0) {
            Other_Bird_Velocity.SetXY(-Other_Bird_Velocity.X()/Other_Bird_Count,-Other_Bird_Velocity.Y()/Other_Bird_Count);
        }

        //Calculates a random velocity between Blue_Min_Speed and Blue_Max_Speed using the RandomSim function
        //From this, calculates an X component by multiplying the velocity by another random double between -1 and 1
        //Finally, the remaining Y component of this velocity is calculated by trigonometry, with a sign randomly given by ((rand()%2)*2 - 1), which gives either 1 or -1
        double Vel = RandomSim(fBlue_Min_Speed,fBlue_Max_Speed);
        double xVel = Vel*RandomSim(-1,1);
        double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

        //Random_Velocity components are set to the components of this random velocity
        Random_Velocity.SetXY(xVel,yVel);

        //CurrentBird Velocity components are set, considering it's Initial Velocity, Separation_Velocity, Alignment_Velocity, Cohesion_Velocity, Other_Bird_Velocity and Random_Velocity, each with their own macroscopic weightings
        Velocity.SetX(Velocity.X()*Initial_Weight+Separation_Velocity.X()*Separation_Weight+Alignment_Velocity.X()*Alignment_Weight+Cohesion_Velocity.X()*Cohesion_Weight+Other_Bird_Velocity.X()*Other_Bird_Weight+Random_Velocity.X()*Random_Weight);
        Velocity.SetY(Velocity.Y()*Initial_Weight+Separation_Velocity.Y()*Separation_Weight+Alignment_Velocity.Y()*Alignment_Weight+Cohesion_Velocity.Y()*Cohesion_Weight+Other_Bird_Velocity.Y()*Other_Bird_Weight+Random_Velocity.Y()*Random_Weight);

        //Velocity constraints are applied to keep the CurrentBird physically realistic, enforcing that it can only fly as fast as it's maximum speed and must also move at a certain minimum speed to continue flying
        //If CurrentBird Velocity magnitude is greater than Blue_Max_Speed it is reduced to Blue_Max_Speed
        if (Velocity.Mag() > fBlue_Max_Speed) {
            double factor = fBlue_Max_Speed/Velocity.Mag();
            Velocity.SetXY(Velocity.X()*factor,Velocity.Y()*factor);
        }

        //If CurrentBird Velocity magnitude is less than Blue_Min_Speed it is increased to Blue_Min_Speed
        if (Velocity.Mag() < fBlue_Min_Speed) {
            double factor = fBlue_Min_Speed/Velocity.Mag();
            Velocity.SetXY(Velocity.X()*factor,Velocity.Y()*factor);
        }

        //CurrentBird Position components are set, considering it's Initial Position plus it's Velocity multiplied by the timestep of the simulation
        Position.SetXY(Position.X()+Velocity.X()*t,Position.Y()+Velocity.Y()*t);

        //Position constraints are applied so that if the CurrentBird position goes beyond the display in a direction, it enters from the opposite direction, creating a looping reality
        //If CurrentBird X component of Position is greater than the DimensionLimits it is decreased by the DimensionLimits
        if (Position.X() > DimensionLimits) {
            double xValid = Position.X() - DimensionLimits;
            Position.SetX(xValid);
        }

        //If CurrentBird Y component of Position is greater than the DimensionLimits it is decreased by the DimensionLimits
        if (Position.Y() > DimensionLimits) {
            double yValid = Position.Y() - DimensionLimits;
            Position.SetY(yValid);
        }

        //If CurrentBird X component of Position is less than 0 it is increased by the DimensionLimits
        if (Position.X() < 0) {
            double xValid = Position.X() + DimensionLimits;
            Position.SetX(xValid);
        }

        //If CurrentBird Y component of Position is less than 0 it is increased by the DimensionLimits
        if (Position.Y() < 0) {
            double yValid = Position.Y() + DimensionLimits;
            Position.SetY(yValid);
        }

        //The i-th Bird in BlueFlock that corresponds to CurrentBird is set to it's new Position and Velocity
        fBlueFlock[i].SetPosition(Position);
        fBlueFlock[i].SetVelocity(Velocity);
    }

    //For loop to advance the position and velocity of each GreenBird in the Flock
    for (int i = 0; i < fGreen_Number; i++) {
        //Considering the i-th GreenBird in GreenFlock as the CurrentBird and getting it's Position and Velocity
        GreenBird CurrentBird = fGreenFlock[i];
        TwoVector Position = CurrentBird.GetPosition();
        TwoVector Velocity = CurrentBird.GetVelocity();

        //Declaring default TwoVectors for the behaviours affecting bird velocity
        TwoVector Separation_Velocity = TwoVector();
        TwoVector Alignment_Velocity = TwoVector();
        TwoVector Cohesion_Velocity = TwoVector();
        TwoVector Other_Bird_Velocity = TwoVector();
        TwoVector Random_Velocity = TwoVector();

        //Declaring counting variables for the number of birds causing the different behaviours affecting CurrentBird velocity
        double Separation_Count = 0;
        double Alignment_Count = 0;
        double Cohesion_Count = 0;
        double Other_Bird_Count = 0;

        //Considering the affect of all other GreenBirds on the CurrentBird
        for (int j = 0; j < fGreen_Number; j++) {
            if (j !=i) {
                //Considering the j-th GreenBird in GreenFlock as the GreenOther, getting it's position, finding the closest point of this position to the CurrentBird and setting GreenOther's position accordingly
                //This is done to correctly consider the affect of each GreenBird from it's closest position to the CurrentBird without changing the actual values of position
                GreenBird GreenOther = fGreenFlock[j];
                TwoVector GreenOtherPos = GreenOther.GetPosition();
                CurrentBird.ClosestPoint(GreenOtherPos);
                GreenOther.SetPosition(GreenOtherPos);

                //Applies Separation, the behaviour that causes birds to steer away from nearby birds of the same type, if GreenOther is within the Separation range of CurrentBird
                if (CurrentBird.Distance(GreenOther.GetPosition()) < fGreen_Separation) {
                    //Instance weighting to ensure a stronger steering away from closer birds by a distance squared rule
                    //Factor of 2.5 has been manually adjusted to ensure a strong enough repulsion of nearby birds to avoid collisions using default settings
                    double Separation_Instance_Weight = 2.5*pow(1 - CurrentBird.Distance(GreenOther.GetPosition())/fGreen_Separation,2);

                    //Contributions of the instance to the Separation_Velocity components are calculated as the components of the distance from the CurrentBird multiplied by the instance weight
                    double xContribution = (GreenOther.GetPosition().X()-Position.X())*Separation_Instance_Weight;
                    double yContribution = (GreenOther.GetPosition().Y()-Position.Y())*Separation_Instance_Weight;

                    //Separation_Velocity components increased by the contributions
                    Separation_Velocity.SetX(Separation_Velocity.X()+xContribution);
                    Separation_Velocity.SetY(Separation_Velocity.Y()+yContribution);

                    //Separation_Count of the number of instances increased
                    Separation_Count++;
                }

                //Applies Alignment, the behaviour that causes birds to steer towards the average heading of nearby birds of the same type, if GreenOther is within the Detection range of CurrentBird
                if (CurrentBird.Distance(GreenOther.GetPosition()) < fGreen_Detection) {
                    //Alignment_Velocity components are increased by the contributions, calculated as the components of the velocity of GreenOther
                    Alignment_Velocity.SetX(Alignment_Velocity.X()+GreenOther.GetVelocity().X());
                    Alignment_Velocity.SetY(Alignment_Velocity.Y()+GreenOther.GetVelocity().Y());

                    //Alignment_Count of the number of instances increased
                    Alignment_Count++;
                }

                //Applies Cohesion, the behaviour that causes birds to steer towards the average position of nearby birds of the same type, if GreenOther is within the Detection range and outside of the Separation range of CurrentBird
                if (CurrentBird.Distance(GreenOther.GetPosition()) < fGreen_Detection && CurrentBird.Distance(GreenOther.GetPosition()) > fGreen_Separation) {
                    //Cohesion_Velocity components are increased by the contributions, calculated as the components of the position of GreenOther
                    Cohesion_Velocity.SetX(Cohesion_Velocity.X()+GreenOther.GetPosition().X());
                    Cohesion_Velocity.SetY(Cohesion_Velocity.Y()+GreenOther.GetPosition().Y());

                    //Cohesion_Count of the number of instances increased
                    Cohesion_Count++;
                }
            }
        }

        //Considering the affect of all BlueBirds on the CurrentBird
        for (int k = 0; k < fBlue_Number; k++) {
            //Considering the k-th BlueBird in BlueFlock as the BlueOther, getting it's position, finding the closest point of this position to the CurrentBird and setting BlueOther's position accordingly
            //This is done to correctly consider the affect of each BlueBird from it's closest position to the CurrentBird without changing the actual values of position
            BlueBird BlueOther = fBlueFlock[k];
            TwoVector BlueOtherPos = BlueOther.GetPosition();
            CurrentBird.ClosestPoint(BlueOtherPos);
            BlueOther.SetPosition(BlueOtherPos);

            //Applies Other_Bird Separation, the behaviour that causes birds to steer away from nearby birds of a different type, if BlueOther is within the Separation range of CurrentBird
            if (CurrentBird.Distance(BlueOther.GetPosition()) < fGreen_Separation) {
                //Instance weighting to ensure a stronger steering away from closer birds by a distance squared rule
                //Unlike Separation from other GreenBird's, there was no need for a factor to increase this weighting for default settings since consistent collisions were only prevalent once large flock of similar attracting birds had formed
                double Other_Bird_Instance_Weight = pow(1 - CurrentBird.Distance(BlueOther.GetPosition())/fGreen_Separation,2);

                //Contributions of the instance to the Other_Bird_Velocity components are calculated as the components of the distance from the CurrentBird multiplied by the instance weight
                double xContribution = (BlueOther.GetPosition().X()-Position.X())*Other_Bird_Instance_Weight;
                double yContribution = (BlueOther.GetPosition().Y()-Position.Y())*Other_Bird_Instance_Weight;

                //Other_Bird_Velocity components increased by the contributions
                Other_Bird_Velocity.SetX(Other_Bird_Velocity.X()+xContribution);
                Other_Bird_Velocity.SetY(Other_Bird_Velocity.Y()+yContribution);

                //Other_Bird_Count of the number of instances increased
                Other_Bird_Count++;
            }
        }

        //If there were any instances causing Separation the components of Separation_Velocity are inversed, to point away from the positions of nearby birds of the same type, and divided by the Separation_Count
        if (Separation_Count > 0) {
            Separation_Velocity.SetXY(-Separation_Velocity.X()/Separation_Count,-Separation_Velocity.Y()/Separation_Count);
        }

        //If there were any instances causing Alignment the components of Alignment_Velocity are divided by the Alignment_Count
        if (Alignment_Count > 0) {
            Alignment_Velocity.SetXY(Alignment_Velocity.X()/Alignment_Count,Alignment_Velocity.Y()/Alignment_Count);
        }

        //If there were any instances causing Cohesion the components of Cohesion_Velocity are divided by the Cohesion_Count and then the CurrentBird Position components are subtracted, to give the direction towards the average position of nearby birds of the same type rather than the average position itself
        if (Cohesion_Count > 0) {
            Cohesion_Velocity.SetXY(Cohesion_Velocity.X()/Cohesion_Count-Position.X(),Cohesion_Velocity.Y()/Cohesion_Count-Position.Y());
        }

        //If there were any instances causing Other_Bird Separation the components of Other_Bird_Velocity are inversed, to point away from the positions of nearby birds of a different type, and divided by the Other_Bird_Count
        if (Other_Bird_Count > 0) {
            Other_Bird_Velocity.SetXY(-Other_Bird_Velocity.X()/Other_Bird_Count,-Other_Bird_Velocity.Y()/Other_Bird_Count);
        }

        //Calculates a random velocity between Green_Min_Speed and Green_Max_Speed using the RandomSim function
        //From this, calculates an X component by multiplying the velocity by another random double between -1 and 1
        //Finally, the remaining Y component of this velocity is calculated by trigonometry, with a sign randomly given by ((rand()%2)*2 - 1), which gives either 1 or -1
        double Vel = RandomSim(fGreen_Min_Speed,fGreen_Max_Speed);
        double xVel = Vel*RandomSim(-1,1);
        double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

        //Random_Velocity components are set to the components of this random velocity
        Random_Velocity.SetXY(xVel,yVel);

        //CurrentBird Velocity components are set, considering it's Initial Velocity, Separation_Velocity, Alignment_Velocity, Cohesion_Velocity, Other_Bird_Velocity and Random_Velocity, each with their own macroscopic weightings
        Velocity.SetX(Velocity.X()*Initial_Weight+Separation_Velocity.X()*Separation_Weight+Alignment_Velocity.X()*Alignment_Weight+Cohesion_Velocity.X()*Cohesion_Weight+Other_Bird_Velocity.X()*Other_Bird_Weight+Random_Velocity.X()*Random_Weight);
        Velocity.SetY(Velocity.Y()*Initial_Weight+Separation_Velocity.Y()*Separation_Weight+Alignment_Velocity.Y()*Alignment_Weight+Cohesion_Velocity.Y()*Cohesion_Weight+Other_Bird_Velocity.Y()*Other_Bird_Weight+Random_Velocity.Y()*Random_Weight);

        //Velocity constraints are applied to keep the CurrentBird physically realistic, enforcing that it can only fly as fast as it's maximum speed and must also move at a certain minimum speed to continue flying
        //If CurrentBird Velocity magnitude is greater than Green_Max_Speed it is reduced to Green_Max_Speed
        if (Velocity.Mag() > fGreen_Max_Speed) {
            double factor = fGreen_Max_Speed/Velocity.Mag();
            Velocity.SetXY(Velocity.X()*factor,Velocity.Y()*factor);
        }

        //If CurrentBird Velocity magnitude is less than Green_Min_Speed it is increased to Green_Min_Speed
        if (Velocity.Mag() < fGreen_Min_Speed) {
            double factor = fGreen_Min_Speed/Velocity.Mag();
            Velocity.SetXY(Velocity.X()*factor,Velocity.Y()*factor);
        }

        //CurrentBird Position components are set, considering it's Initial Position plus it's Velocity multiplied by the timestep of the simulation
        Position.SetXY(Position.X()+Velocity.X()*t,Position.Y()+Velocity.Y()*t);

        //Position constraints are applied so that if the CurrentBird position goes beyond the display in a direction, it enters from the opposite direction, creating a looping reality
        //If CurrentBird X component of Position is greater than the DimensionLimits it is decreased by the DimensionLimits
        if (Position.X() > DimensionLimits) {
            double xValid = Position.X() - DimensionLimits;
            Position.SetX(xValid);
        }

        //If CurrentBird Y component of Position is greater than the DimensionLimits it is decreased by the DimensionLimits
        if (Position.Y() > DimensionLimits) {
            double yValid = Position.Y() - DimensionLimits;
            Position.SetY(yValid);
        }

        //If CurrentBird X component of Position is less than 0 it is increased by the DimensionLimits
        if (Position.X() < 0) {
            double xValid = Position.X() + DimensionLimits;
            Position.SetX(xValid);
        }

        //If CurrentBird Y component of Position is less than 0 it is increased by the DimensionLimits
        if (Position.Y() < 0) {
            double yValid = Position.Y() + DimensionLimits;
            Position.SetY(yValid);
        }

        //The i-th Bird in GreenFlock that corresponds to CurrentBird is set to it's new Position and Velocity
        fGreenFlock[i].SetPosition(Position);
        fGreenFlock[i].SetVelocity(Velocity);
    }
}

//Defining random function for use in simulation
//Returns a random double value between the given min and max values
double RandomSim(double min, double max){
    //Calculates a random double factor between 0 and 1
    double f = (double)rand()/RAND_MAX;

    //Returns min plus the difference between max and min multiplied by the factor, hence a random double value between min and max
    return min + f*(max - min);
}
