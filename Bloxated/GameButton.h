#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QFrame>
#include <QLabel>
#include <QPoint>
#include <QEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <ButtonFace.h>

struct Position{
    QPoint p;
    int index;
};

class GameButton : public QFrame
{
    Q_OBJECT
public:
    explicit GameButton(QWidget *parent = 0);

    int index;
    Position* newPos;
    bool isCorrectColorPos();
signals:
     void mousePressed(int ind);
protected:
     bool event(QEvent *event);

public slots:
    void setNumberMode();
    void setColorMode();
    void moveToPos(bool randTime = false);
private:
    QPoint clickStart;
    QEasingCurve easCurve;
    ButtonFace* face;

};

#endif // GAMEBUTTON_H
