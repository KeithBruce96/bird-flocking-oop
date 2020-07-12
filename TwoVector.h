#ifndef TWOVECTOR_H
#define TWOVECTOR_H

#include <math.h>

class TwoVector {
public:
    //Declaring the constructors
    TwoVector();
    TwoVector(double x, double y);

    //Declaring the destructor
    virtual ~TwoVector();

    //Declaring access methods
    //Inline used for these methods as they will be called often yet are short, hence faster execution is valuable and the increases in executable length will be acceptable
    //Const used wherever possible (methods where nothing is modified) so that the compiler can perform some optimization

    //X and Y components
    inline double X() const;
    inline double Y() const;

    //Setting X and Y components
    inline void SetX(double);
    inline void SetY(double);
    inline void SetXY(double x, double y);

    //Magnitude squared
    inline double Mag2() const;

    //Magnitude
    inline double Mag() const;

private:
    //Declaring data members
    double fX, fY;
};

//Implementation of all inline access methods
inline double TwoVector::X() const {return fX;}
inline double TwoVector::Y() const {return fY;}

inline void TwoVector::SetX(double xx) {fX = xx;}
inline void TwoVector::SetY(double yy) {fY = yy;}
inline void TwoVector::SetXY(double xx, double yy) {
    fX = xx;
    fY = yy;
}

inline double TwoVector::Mag2() const {return fX*fX + fY*fY;}

inline double TwoVector::Mag() const {return sqrt(Mag2());}

#endif // TWOVECTOR_H
