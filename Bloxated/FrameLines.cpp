#include "FrameLines.h"

FrameLines::FrameLines(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("FrameLines");
    QString areaStyle = "#FrameLines{border: 3px solid #8f8f91;border-radius: 10px;}";
    setStyleSheet(areaStyle);

}
void FrameLines::setColorMode()
{
    QString areaStyle;
    if(index==0)
    {
        areaStyle =  "#FrameLines{border: 3px solid #0000CC;border-radius: 10px;}";
    }
    else if(index==1)
    {
        areaStyle =  "#FrameLines{border: 3px solid #CC0000;border-radius: 10px;}";
    }
    else if(index==2)
    {
        areaStyle =  "#FrameLines{border: 3px solid #00CC00;border-radius: 10px;}";
    }
    else if(index==3)
    {
        areaStyle =  "#FrameLines{border: 3px solid #CCCC00;border-radius: 10px;}";
    }
    else
        areaStyle = "#FrameLines{border: 3px solid #8f8f91;border-radius: 10px;}";
    setStyleSheet(areaStyle);
}

void FrameLines::setNumberMode()
{
    QString areaStyle = "#FrameLines{border: 3px solid #8f8f91;border-radius: 10px;}";
    setStyleSheet(areaStyle);
}
