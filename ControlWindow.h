#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QMainWindow>

#include <Flock.h>

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Declaring the constructor
    explicit ControlWindow(QWidget *parent = 0);

    //Declaring the destructor
    ~ControlWindow();

private slots:
    //Declaring slots

    //Reads the current values from the system and set them in the corresponding input fields
    void ReadSystem(Flock* sys);

    //Simulates the system when run
    void Simulate();

    //Sets status of the simulation, and initialises the system when run
    void on_runButton_clicked();

    //Clears system BlueFlock and GreenFlock and reinitialises them
    void on_resetButton_clicked();

    //Slots for when input fields have finished being edited, setting the values of system data members
    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_lineEdit_4_editingFinished();

    void on_lineEdit_5_editingFinished();

    void on_lineEdit_6_editingFinished();

    void on_lineEdit_7_editingFinished();

    void on_lineEdit_8_editingFinished();

    void on_lineEdit_9_editingFinished();

    void on_lineEdit_10_editingFinished();

private:
    //Declaring data members
    Ui::ControlWindow *ui;
    Flock* fSystem;

    //Declaring enumerators
    enum{
    kIdle,
    kRun
    };
    int fStatus;
};

#endif // CONTROLWINDOW_H
