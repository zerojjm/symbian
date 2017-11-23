#include "hitbutton.h"

HitButton::HitButton(QWidget *parent) :
    QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::NoFocus);
    setRed();
    hide();
    posValid = false;
    QFont font = this->font();
    font.setPointSize(font.pointSize()+2);
    this->setFont(font);
}
void HitButton::setRed()
{
    QString buttonStyle = "border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);";
    setStyleSheet( buttonStyle);
}

void HitButton::setGreen()
{
    QString buttonStyle = "border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #006600, stop: 1 #00ff00);";
    setStyleSheet( buttonStyle);
}
