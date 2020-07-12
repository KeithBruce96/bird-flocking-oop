#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTimer>

#include "ControlWindow.h"
#include "ui_ControlWindow.h"

#include "DisplayWindow.h"

//Declaring random function for use in initialisation
//Returns a random double value between the given min and max values
double Random(double min, double max);

//Implementation of constructor
ControlWindow::ControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);

    //Setting aesthetic features for buttons
    ui->runButton->setStyleSheet("background-color:lightGray");
    ui->resetButton->setStyleSheet("background-color:lightGray");

    //Default initial values for system
    int Blue_Number = 75;
    int Green_Number = 50;
    double Blue_Detection = 200;
    double Green_Detection = 150;
    double Blue_Separation = 100;
    double Green_Separation = 75;
    double Blue_Max_Speed = 6;
    double Green_Max_Speed = 8;
    double Blue_Min_Speed = 2;
    double Green_Min_Speed = 5;

    //Creating an instance of Flock from initial values
    Flock* flock = new Flock(Blue_Number, Green_Number, Blue_Detection, Green_Detection, Blue_Separation, Green_Separation, Blue_Max_Speed, Green_Max_Speed, Blue_Min_Speed, Green_Min_Speed);

    //Setting system to flock
    fSystem = flock;

    //Read system as defined by slot
    ReadSystem(flock);

    //Connect a timer to trigger Simulate slot every 10 microseconds
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Simulate()));
    timer->start(10);
}

//Implementation of destructor
ControlWindow::~ControlWindow()
{
    delete ui;
}

//Implementation of ReadSystem slot
void ControlWindow::ReadSystem(Flock *sys) {
    //Reads the current values from the system and set them in the corresponding input fields
    ui->lineEdit->setText(QString::number(sys->GetBlueNumber()));
    ui->lineEdit_2->setText(QString::number(sys->GetBlueDetection()));
    ui->lineEdit_3->setText(QString::number(sys->GetBlueSeparation()));
    ui->lineEdit_4->setText(QString::number(sys->GetBlueMaxSpeed()));
    ui->lineEdit_5->setText(QString::number(sys->GetBlueMinSpeed()));
    ui->lineEdit_6->setText(QString::number(sys->GetGreenNumber()));
    ui->lineEdit_7->setText(QString::number(sys->GetGreenDetection()));
    ui->lineEdit_8->setText(QString::number(sys->GetGreenSeparation()));
    ui->lineEdit_9->setText(QString::number(sys->GetGreenMaxSpeed()));
    ui->lineEdit_10->setText(QString::number(sys->GetGreenMinSpeed()));
}

//Implementation of Simulate slot
void ControlWindow::Simulate() {
    //Simulates the system through timestep 0.1 when status is run
    if (fStatus == kRun) {
        fSystem->Simulate(0.1);
    }
}

//Implementation of on_runButton_clicked slot
void ControlWindow::on_runButton_clicked()
{
    //If Status is Idle, initialises flock, displays it and sets Status to run
    if (fStatus == kIdle) {
        //Dimension Limits chosen for simulation
        double DimensionLimits = 1000;

        //Seeding random number generator
        srand(time(0));

        //Setting aesthetic features for runButton
        ui->runButton->setStyleSheet("background-color:red");
        ui->runButton->setText("Stop");

        //Setting input fields for number of birds to ReadOnly to prevent simultation crashing when editted whilst running, and change aesthetic features accordingly
        ui->lineEdit->setReadOnly(true);
        ui->lineEdit_6->setReadOnly(true);
        ui->lineEdit->setStyleSheet("background-color:lightGray");
        ui->lineEdit_6->setStyleSheet("background-color:lightGray");

        //Setting instance of Flock to system
        Flock* flock = fSystem;

        //Declaring instances of other variable types
        BlueBird bluebird;
        GreenBird greenbird;

        //Initialising BlueFlock of flock using system parameters and the Random function
        for (int i = 0; i < flock->GetBlueNumber(); i++) {
            double xPos = Random(0,DimensionLimits);
            double yPos = Random(0,DimensionLimits);
            double Vel = Random(flock->GetBlueMinSpeed(),flock->GetBlueMaxSpeed());
            double xVel = Vel*Random(-1,1);
            double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

            TwoVector Position (xPos, yPos);
            TwoVector Velocity (xVel, yVel);

            bluebird = BlueBird(Position,Velocity);
            flock->AddBlueBird(bluebird);
        }

        //Initialising GreenFlock of flock using system parameters and the Random function
        for (int i = 0; i < flock->GetGreenNumber(); i++) {
            double xPos = Random(0,DimensionLimits);
            double yPos = Random(0,DimensionLimits);
            double Vel = Random(flock->GetGreenMinSpeed(),flock->GetGreenMaxSpeed());
            double xVel = Vel*Random(-1,1);
            double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

            TwoVector Position (xPos, yPos);
            TwoVector Velocity (xVel, yVel);

            greenbird = GreenBird(Position,Velocity);
            flock->AddGreenBird(greenbird);
        }

        //Opens a DisplayWindow for flock
        DisplayWindow* Win = new DisplayWindow(flock);

        //Setting Status to run
        fStatus = kRun;
    }

    //If Status is Run, changes status
    else {
        //Setting aesthetic features for runButton
        ui->runButton->setStyleSheet("background-color:lightGray");
        ui->runButton->setText("Run");

        //Setting input fields for number of birds to Editable to allow changes to be made, and change aesthetic features accordingly
        ui->lineEdit->setReadOnly(false);
        ui->lineEdit_6->setReadOnly(false);
        ui->lineEdit->setStyleSheet("background-color:white");
        ui->lineEdit_6->setStyleSheet("background-color:white");

        //Setting Status to idle
        fStatus = kIdle;
    }
}

//Implementation of on_resetButton_clicked slot
void ControlWindow::on_resetButton_clicked()
{
    //Dimension Limits chosen for simulation
    double DimensionLimits = 1000;

    //Seeding random number generator
    srand(time(0));

    //Clears system BlueFlock and GreenFlock
    fSystem->ClearFlock();

    //Setting instance of Flock to system
    Flock* flock = fSystem;

    //Declaring instances of other variable types
    BlueBird bluebird;
    GreenBird greenbird;

    //Initialising BlueFlock of flock using system parameters and the Random function
    for (int i = 0; i < flock->GetBlueNumber(); i++) {
        double xPos = Random(0,DimensionLimits);
        double yPos = Random(0,DimensionLimits);
        double Vel = Random(flock->GetBlueMinSpeed(),flock->GetBlueMaxSpeed());
        double xVel = Vel*Random(-1,1);
        double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

        TwoVector Position (xPos, yPos);
        TwoVector Velocity (xVel, yVel);

        bluebird = BlueBird(Position,Velocity);
        flock->AddBlueBird(bluebird);
    }

    //Initialising GreenFlock of flock using system parameters and the Random function
    for (int i = 0; i < flock->GetGreenNumber(); i++) {
        double xPos = Random(0,DimensionLimits);
        double yPos = Random(0,DimensionLimits);
        double Vel = Random(flock->GetGreenMinSpeed(),flock->GetGreenMaxSpeed());
        double xVel = Vel*Random(-1,1);
        double yVel = ((rand()%2)*2 - 1)*sqrt(pow(Vel,2)-pow(xVel,2));

        TwoVector Position (xPos, yPos);
        TwoVector Velocity (xVel, yVel);

        greenbird = GreenBird(Position,Velocity);
        flock->AddGreenBird(greenbird);
    }
}

//Initialising slots for when input fields have finished being edited, setting the values of system data members
void ControlWindow::on_lineEdit_editingFinished()
{
    fSystem->SetBlueNumber(ui->lineEdit->text().toInt());
}

void ControlWindow::on_lineEdit_2_editingFinished()
{
    fSystem->SetBlueDetection(ui->lineEdit_2->text().toDouble());
}

void ControlWindow::on_lineEdit_3_editingFinished()
{
    fSystem->SetBlueSeparation(ui->lineEdit_3->text().toDouble());
}

void ControlWindow::on_lineEdit_4_editingFinished()
{
    fSystem->SetBlueMaxSpeed(ui->lineEdit_4->text().toDouble());
}

void ControlWindow::on_lineEdit_5_editingFinished()
{
    fSystem->SetBlueMinSpeed(ui->lineEdit_5->text().toDouble());
}

void ControlWindow::on_lineEdit_6_editingFinished()
{
    fSystem->SetGreenNumber(ui->lineEdit_6->text().toInt());
}

void ControlWindow::on_lineEdit_7_editingFinished()
{
    fSystem->SetGreenDetection(ui->lineEdit_7->text().toDouble());
}

void ControlWindow::on_lineEdit_8_editingFinished()
{
    fSystem->SetGreenSeparation(ui->lineEdit_8->text().toDouble());
}

void ControlWindow::on_lineEdit_9_editingFinished()
{
    fSystem->SetGreenMaxSpeed(ui->lineEdit_9->text().toDouble());
}

void ControlWindow::on_lineEdit_10_editingFinished()
{
    fSystem->SetGreenMinSpeed(ui->lineEdit_10->text().toDouble());
}

//Defining random function for use in initialisation
//Returns a random double value between the given min and max values
double Random(double min, double max){
    //Calculates a random double factor between 0 and 1
    double f = (double)rand()/RAND_MAX;

    //Returns min plus the difference between max and min multiplied by the factor, hence a random double value between min and max
    return min + f*(max - min);
}
