#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>

#include "Flock.h"

namespace Ui {
class DisplayWindow;
}

class DisplayWindow : public QWidget
{
    Q_OBJECT

public:
    //Declaring the constructor
    explicit DisplayWindow(Flock* system, QWidget *parent = 0);

    //Declaring the destructor
    ~DisplayWindow();

private slots:
    //Declaring slots

    //Paints when called by Update
    void paintEvent(QPaintEvent *);

private:
    //Declaring data members
    Ui::DisplayWindow *ui;

    Flock* fFlock;
};

#endif // DISPLAYWINDOW_H
