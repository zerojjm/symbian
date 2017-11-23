#include "ButtonFace.h"

ButtonFace::ButtonFace(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("ButtonFace");

    QString areaStyle = "#ButtonFace{border: 3px solid #FF0000;border-radius: 10px;}";
    setStyleSheet(areaStyle);

    QGridLayout* lo= new QGridLayout(this);
    lblText = new QLabel(this);
    lblText->setAlignment(Qt::AlignCenter);
    lblText->setText("");
    lblText->setStyleSheet("color:white");
    QFont font=lblText->font();
    font.setPixelSize(25);
    lblText->setFont(font);
    lo->addWidget(lblText,0,0);
}
/*bool ButtonFace::event(QEvent *event)
 {
     if (event->type() == QEvent::MouseButtonPress) {
         emit mousePressed(index, (QMouseEvent*) event);
        event->accept();
        return true;
     }

     return QFrame::event(event);
 }*/

void ButtonFace::setNumberMode(int index)
{
    //QString buttonStyle = "#ButtonFace{border: 3px solid #8f8f91;border-radius: 28px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #000022, stop: 1 #0000aa);}";
    //setStyleSheet(buttonStyle);
    setColorMode(index);
    lblText->setText(QString::number(index+1));

}

void ButtonFace::setColorMode(int index)
{
    lblText->setText("");
    QString buttonStyle;
    if(index<4)
    {
        colorIndex = 0;
        buttonStyle = "#ButtonFace{border: 3px solid #8f8f91;border-radius: 35px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #000066, stop: 1 #0000ff);}";
    }
    else if(index<8)
    {
        colorIndex = 1;
        buttonStyle = "#ButtonFace{border: 3px solid #8f8f91;border-radius: 35px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);}";
    }
    else if(index<12)
    {
        colorIndex = 2;
        buttonStyle = "#ButtonFace{border: 3px solid #8f8f91;border-radius: 35px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #006600, stop: 1 #00ff00);}";
    }
    else if(index<16)
    {
        colorIndex = 3;
        buttonStyle = "#ButtonFace{border: 3px solid #8f8f91;border-radius: 35px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #666600, stop: 1 #ffff00);}";
    }
    setStyleSheet(buttonStyle);
}
