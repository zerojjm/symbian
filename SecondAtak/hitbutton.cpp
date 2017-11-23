#include "hitbutton.h"

HitButton::HitButton(QWidget *parent) :
    QPushButton(parent)
{
    QString buttonStyle = "border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);";
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);

    hide();
    setStyleSheet( buttonStyle);
    posValid = false;
}
