#ifndef FLOCK_H
#define FLOCK_H

#include <vector>

#include "BlueBird.h"
#include "GreenBird.h"

class Flock {
public:
    //Declaring the constructors
    Flock();
    Flock(int Blue_Number, int Green_Number, double Blue_Detection, double Green_Detection, double Blue_Separation, double Green_Separation, double Blue_Max_Speed, double Green_Max_Speed, double Blue_Min_Speed, double Green_Min_Speed);

    //Declaring the destructor
    virtual ~Flock();

    //Declaring access methods
    //Get, set, clear and add component methods have been implemented with their declaration for faster executions, as they will be called often, and to differentiate them from other access methods
    //Const used wherever possible (methods where nothing is modified) so that the compiler can perform some optimization

    //Get and set methods for Blue_Number
    const int& GetBlueNumber() const {
        return fBlue_Number;
    }

    void SetBlueNumber(const int& Blue_Number) {
        fBlue_Number = Blue_Number;
    }

    //Get and set methods for Green_Number
    const int& GetGreenNumber() const {
        return fGreen_Number;
    }

    void SetGreenNumber(const int& Green_Number) {
        fGreen_Number = Green_Number;
    }

    //Get and set methods for _BlueDetection
    const double& GetBlueDetection() const {
        return fBlue_Detection;
    }

    void SetBlueDetection(const double& Blue_Detection) {
        fBlue_Detection = Blue_Detection;
    }

    //Get and set methods for Green_Detection
    const double& GetGreenDetection() const {
        return fGreen_Detection;
    }

    void SetGreenDetection(const double& Green_Detection) {
        fGreen_Detection = Green_Detection;
    }

    //Get and set methods for Blue_Separation
    const double& GetBlueSeparation() const {
        return fBlue_Separation;
    }

    void SetBlueSeparation(const double& Blue_Separation) {
        fBlue_Separation = Blue_Separation;
    }

    //Get and set methods for Green_Separation
    const double& GetGreenSeparation() const {
        return fGreen_Separation;
    }

    void SetGreenSeparation(const double& Green_Separation) {
        fGreen_Separation = Green_Separation;
    }

    //Get and set methods for Blue_Max_Speed
    const double& GetBlueMaxSpeed() const {
        return fBlue_Max_Speed;
    }

    void SetBlueMaxSpeed(const double& Blue_Max_Speed) {
        fBlue_Max_Speed = Blue_Max_Speed;
    }

    //Get and set methods for Green_Max_Speed
    const double& GetGreenMaxSpeed() const {
        return fGreen_Max_Speed;
    }

    void SetGreenMaxSpeed(const double& Green_Max_Speed) {
        fGreen_Max_Speed = Green_Max_Speed;
    }

    //Get and set methods for Blue_Min_Speed
    const double& GetBlueMinSpeed() const {
        return fBlue_Min_Speed;
    }

    void SetBlueMinSpeed(const double& Blue_Min_Speed) {
        fBlue_Min_Speed = Blue_Min_Speed;
    }

    //Get and set methods for Green_Min_Speed
    const double& GetGreenMinSpeed() const {
        return fGreen_Min_Speed;
    }

    void SetGreenMinSpeed(const double& Green_Min_Speed) {
        fGreen_Min_Speed = Green_Min_Speed;
    }

    //Get method for a BlueFlock BlueBird
    const BlueBird& GetBlueFlockBird(int i) {
        return fBlueFlock[i];
    }

    //Get method for a GreenFlock GreenBird
    const GreenBird& GetGreenFlockBird(int i) {
        return fGreenFlock[i];
    }

    //Clear method for BlueFlock and GreenFlock, used to reset the simulation with current settings
    void ClearFlock() {
        fBlueFlock.clear();
        fGreenFlock.clear();
    }

    //Add component method for BlueFlock
    void AddBlueBird(BlueBird bluebird) {fBlueFlock.push_back(bluebird);}

    //Add component method for GreenFlock
    void AddGreenBird(GreenBird greenbird) {fGreenFlock.push_back(greenbird);}

    //Simulate function to advance the flock by timestep t
    void Simulate(double t);

private:
    //Declaring data members
    int fBlue_Number, fGreen_Number;
    double fBlue_Detection, fGreen_Detection;
    double fBlue_Separation, fGreen_Separation;
    double fBlue_Max_Speed, fGreen_Max_Speed;
    double fBlue_Min_Speed, fGreen_Min_Speed;
    std::vector<BlueBird> fBlueFlock;
    std::vector<GreenBird> fGreenFlock;
};

#endif // FLOCK_H
