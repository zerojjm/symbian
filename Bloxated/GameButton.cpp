#include "GameButton.h"
#include <QGridLayout>
#include "math.h"

static const int moveAnimDuration = 220;

GameButton::GameButton(QWidget *parent) :
    QFrame(parent)
{
    /*setObjectName("GameButton");
    QString buttonStyle = "#GameButton{border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);}";
    setStyleSheet(buttonStyle);*/

    QGridLayout* lo= new QGridLayout(this);
    lo->setMargin(10);
    face = new ButtonFace(this);
    lo->addWidget(face,0,0);
    easCurve.setType(QEasingCurve::OutCubic);

}

bool GameButton::event(QEvent *event)
 {
     if (event->type() == QEvent::MouseButtonPress) {
         emit mousePressed(index);
     }
     /*if (event->type() == QEvent::MouseMove) {
         emit mousePressed(index);
     }*/
     return QFrame::event(event);
 }
void GameButton::setNumberMode()
{
    face->setNumberMode(index);
}

void GameButton::setColorMode()
{
    face->setColorMode(index);
}

void GameButton::moveToPos(bool randTime)
{
    if(!newPos)
        return;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    if(randTime)
    {
        int timeDiff = moveAnimDuration+200 - rand()%200;
        animation->setDuration(timeDiff);
    }
    else
        animation->setDuration(moveAnimDuration);
    animation->setStartValue(this->pos());
    animation->setEndValue(newPos->p);
    animation->setEasingCurve(easCurve);
    animation->start();
    //move(pos->x(), pos->y());
}

bool GameButton::isCorrectColorPos()
{
    int currentIndex = newPos->index;
    if(currentIndex == 0 || currentIndex == 1 ||
            currentIndex == 4||  currentIndex == 5)
    {
        if(face->colorIndex==0)
            return true;
        return false;
    }
    if(currentIndex == 2 || currentIndex == 3 ||
            currentIndex == 6||  currentIndex == 7)
    {
        if(face->colorIndex==1)
            return true;
        return false;
    }
    if(currentIndex == 8 || currentIndex == 9 ||
            currentIndex == 12||  currentIndex == 13)
    {
        if(face->colorIndex==2)
            return true;
        return false;
    }
    if(currentIndex == 10 || currentIndex == 11 ||
            currentIndex == 14||  currentIndex == 15)
    {
        if(face->colorIndex==3)
            return true;
        return false;
    }
    return false;
}
