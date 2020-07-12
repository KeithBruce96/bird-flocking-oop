#include <QPainter>
#include <QTimer>

#include "DisplayWindow.h"
#include "ui_DisplayWindow.h"

//Implementation of constructor
DisplayWindow::DisplayWindow(Flock* system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayWindow), fFlock(system)
{
    ui->setupUi(this);

    //Declares size and aesthetic features for DisplayWindow
    this->resize(1000,1000);
    this->setFixedSize(1000,1000);
    setStyleSheet("background-color:white");
    setWindowTitle("Blue and Green Bird Flocking");

    //Connect a timer to trigger Update/paintEvent slot every 10 microseconds
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);

    //Show the window
    show();

    //Prevent implicit redraw of the background
    setAttribute(Qt::WA_OpaquePaintEvent);
}

//Implementation of destructor
DisplayWindow::~DisplayWindow()
{
    delete ui;
}

//Implementation of paintEvent slot
void DisplayWindow::paintEvent(QPaintEvent *) {

    //Clears the screen
    QPainter clear(this);
    clear.fillRect(QRect(0, 0, width() - 1, height() - 1), Qt::white);

    //Gets numbers of birds to paint
    int Blue_Number = fFlock->GetBlueNumber();
    int Green_Number = fFlock->GetGreenNumber();

    //Paints BlueBirds
    for (int i = 0; i < Blue_Number; i++) {
        QPainter bluePainter(this);

        TwoVector Position = fFlock->GetBlueFlockBird(i).GetPosition();
        TwoVector Velocity = fFlock->GetBlueFlockBird(i).GetVelocity();

        int xPixels = Position.X();
        int yPixels = Position.Y();

        bluePainter.setBrush(Qt::blue);
        bluePainter.drawEllipse(xPixels-2, yPixels-2, 4, 4);
    }

    //Paints GreenBirds
    for (int i = 0; i < Green_Number; i++) {
        QPainter greenPainter(this);

        TwoVector Position = fFlock->GetGreenFlockBird(i).GetPosition();
        TwoVector Velocity = fFlock->GetGreenFlockBird(i).GetVelocity();

        int xPixels = Position.X();
        int yPixels = Position.Y();

        greenPainter.setBrush(Qt::green);
        greenPainter.drawEllipse(xPixels-2, yPixels-2, 4, 4);
    }
}
