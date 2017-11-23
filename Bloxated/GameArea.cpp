#include "GameArea.h"
#include <QMouseEvent>
#include <QTimer>
#include "math.h"
#include "time.h"



GameArea::GameArea(QWidget *parent) :
    QFrame(parent)
{
    setObjectName("GameArea");
    QString areaStyle = "#GameArea{border: 3px solid #8f8f91;border-radius: 4px;}";
    setStyleSheet(areaStyle);


    for(int i=0;i<5;++i)
    {
        FrameLines* lines = new FrameLines(this);
        lines->index = i;
        frameLines.push_back(lines);
    }
    for(int i=0;i<16;++i)
    {
        GameButton* btn = new GameButton(this);
        btn->index = i;
        buttons.push_back(btn);
        connect(btn, SIGNAL(mousePressed(int)), this, SLOT(startDragging(int)));
    }

    dragging = false;
    dragIndex = -1;
    gameMode = 1;
    srand(time(0));
    scoreFile = QApplication::applicationDirPath() + "/score.ini";
    scores.loadScores(scoreFile, "Bloxated");
    dragLength = 10;
    gameOn = false;

    lblSolved = new QLabel(this);
    lblSolved->setText("Solved!");
    QFont font = lblSolved->font();
    font.setPointSize(28);
    lblSolved->setFont(font);
    lblSolved->setAlignment(Qt::AlignHCenter);
    lblSolved->setMinimumSize(200,100);
}

bool GameArea::event(QEvent *event)
{
    if(!gameOn)
        return QFrame::event(event);

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mE = (QMouseEvent*) event;
        clickStart = mE->pos();
        /*if(dragIndex<0)
            dragIndex = solveDragIndex();*/
        dragging = true;
    }
    if (event->type() == QEvent::MouseMove && dragging && dragIndex >=0)
    {
        QMouseEvent* mE = (QMouseEvent*) event;

        if(abs(mE->x()-clickStart.x())> dragLength ||
                abs(mE->y()-clickStart.y())> dragLength)
        {
            int divX = mE->x()-clickStart.x();
            int divY = mE->y()-clickStart.y();
            if(abs(divX)>abs(divY))
                draggedHorizontal(divX>0);
            else
                draggedVertical(divY>0);
            dragging = false;
            //clickStart.setX(mE->x()+2*divX);
            //clickStart.setY(mE->y()+2*divY);
            //event->accept();
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        dragging = false;
        dragIndex = -1;
    }
    return QFrame::event(event);
}

void GameArea::startDragging(int index)
{
    dragIndex = index;
    dragging = true;

}

void GameArea::setInitialPositions(int areaWidth)
{
    areaSize = areaWidth;
    positions.clear();
    double xIter=0;
    double yIter=0;

    double tmpSize = (double)areaSize;
    for(int i=0;i<16;i++)
    {
        Position pos;
        //point.setX(xIter* (6.0/24.0)*tmpSize + (1.0/24.0)*tmpSize);
        //point.setY(yIter* (6.0/24.0)*tmpSize + (1.0/24.0)*tmpSize);
        pos.p.setX(xIter* (1.0/4.0)*tmpSize);
        pos.p.setY(yIter* (1.0/4.0)*tmpSize);
        pos.index = i;
        positions.push_back(pos);
        pos.p.setY(-100);
        upPositions.push_back(pos);
        xIter++;
        if(xIter>=4)
        {
            xIter = 0;
            yIter++;
        }
    }
    int btnWidth = (int)(((double)areaWidth)/4.0);
    for(int i=0;i<16;++i)
    {
        buttons.at(i)->setMinimumSize(btnWidth,btnWidth);
        buttons.at(i)->setMaximumSize(btnWidth,btnWidth);
        buttons.at(i)->resize(btnWidth,btnWidth);
        buttons.at(i)->newPos = &upPositions[i];
        buttons.at(i)->moveToPos();
    }

    int frameWidth = (1.0/4.0)*tmpSize+8;
    foreach(FrameLines* line, frameLines)
    {
        line->setMinimumSize(frameWidth,frameWidth);
        line->setMaximumSize(frameWidth,frameWidth);
        line->resize(frameWidth, frameWidth);
    }
    int coord58 = (5.0/8.0)*tmpSize-5;
    int coord18 = (1.0/8.0)*tmpSize-5;
    int coord38 = (3.0/8.0)*tmpSize-5;

    frameLines.at(0)->move(coord18, coord18);
    frameLines.at(1)->move(coord58, coord18);
    frameLines.at(2)->move(coord18, coord58);
    frameLines.at(3)->move(coord58, coord58);
    frameLines.at(4)->move(coord38, coord38);
    lblSolved->move(areaWidth/2-lblSolved->minimumWidth()/2,-200);

}

void GameArea::newGame()
{
    if(gameMode == 0)
        setColorMode();
    else
        setNumberMode();

    QPropertyAnimation *animation = new QPropertyAnimation(lblSolved, "pos");
    animation->setDuration(500);
    animation->setStartValue(lblSolved->pos());
    animation->setEndValue(QPoint(width()/2-lblSolved->minimumWidth()/2,-200));
    QEasingCurve curve;
    curve.setType(QEasingCurve::InBack);
    animation->setEasingCurve(curve);

    animation->start();

    shuffle();
    gameTime.start();
    gameOn = true;

    //lblSolved->move(areaWidth/2-lblSolved->minimumWidth()/2,-200);


}

void GameArea::shuffle()
{
    for(int i=0;i<16;++i)
    {
        buttons.at(i)->newPos = &upPositions[i];
    }
    foreach(GameButton* btn, buttons)
        btn->moveToPos();

    QTimer::singleShot(400, this, SLOT(launchShuffle()));

}

void GameArea::launchShuffle()
{
    QList<int> tmpPosIndex;
    for(int i=0;i<16;++i)
        tmpPosIndex.push_back(i);

    for(int i=0;i<16;++i)
    {
        buttons.at(i)->newPos = &positions[tmpPosIndex.takeAt(rand()%(tmpPosIndex.size()))];
    }

    foreach(GameButton* btn, buttons)
        btn->moveToPos(true);
}

void GameArea::setNumberMode()
{
    gameMode = 1;
    for(int i=0;i<16;++i)
    {
        buttons.at(i)->setNumberMode();
    }
    foreach(FrameLines* line, frameLines)
        line->setNumberMode();
}

void GameArea::setColorMode()
{
    gameMode = 0;
    for(int i=0;i<16;++i)
    {
        buttons.at(i)->setColorMode();
    }
    foreach(FrameLines* line, frameLines)
        line->setColorMode();

}

void GameArea::draggedHorizontal(bool dragRight)
{
    int pos = -1;

    for(int i=0;i<16;++i)
    {
        if(positions[i].index == buttons.at(dragIndex)->newPos->index)
            pos = i;
    }
    if(pos<0)
        return;
    if(dragRight)
    {
        if(pos ==0 || pos == 4)
            rotateButtons(pos ==0, 0);
        else if(pos == 1)
            rotateButtons(true, 0);
        else if(pos == 2 || pos == 6)
            rotateButtons(pos == 2, 1);
        else if(pos == 3 || pos == 7)
            rotateButtons(pos ==3, 1);
        else if(pos == 5 || pos == 9)
            rotateButtons(pos == 5, 4);
        else if(pos == 8 || pos == 12)
            rotateButtons(pos == 8, 2);
        else if(pos == 13)
            rotateButtons(false, 2);
        else if(pos == 10 || pos == 14)
            rotateButtons(pos == 10, 3);
        else if(pos == 11 || pos == 15)
            rotateButtons(pos == 11, 3);
        else
            return;
    }
    else
    {
        if(pos ==0 || pos == 4)
            rotateButtons(pos ==4, 0);
        else if(pos ==1 || pos == 5)
            rotateButtons(pos ==5, 0);
        else if(pos == 2)
            rotateButtons(false, 1);
        else if(pos == 3 || pos == 7)
            rotateButtons(pos ==7, 1);
        else if(pos == 6 || pos == 10)
            rotateButtons(pos == 10, 4);
        else if(pos == 8 || pos == 12)
            rotateButtons(pos == 12, 2);
        else if(pos == 9 || pos == 13)
            rotateButtons(pos == 13, 2);
        else if(pos == 11 || pos == 15)
            rotateButtons(pos == 15, 3);
        else if(pos == 14)
            rotateButtons(true, 3);
        else
            return;
    }
}

void GameArea::draggedVertical(bool dragDown)
{
    int pos = -1;

    for(int i=0;i<16;++i)
    {
        if(positions[i].index == buttons.at(dragIndex)->newPos->index)
            pos = i;
    }
    if(pos<0)
        return;

    if(dragDown)
    {
        if(pos ==0 || pos == 1)
            rotateButtons(pos ==1, 0);
        else if(pos == 2 || pos == 3)
            rotateButtons(pos ==3, 1);
        else if(pos == 4)
            rotateButtons(false, 0);
        else if(pos == 5 || pos == 6)
            rotateButtons(pos == 6, 4);
        else if(pos == 7)
            rotateButtons(true, 1);
        else if(pos == 8 || pos == 9)
            rotateButtons(pos == 9, 2);
        else if(pos == 10 || pos == 11)
            rotateButtons(pos == 11, 3);
        else if(pos == 12 || pos == 13)
            rotateButtons(pos == 13, 2);
        else if(pos == 14 || pos == 15)
            rotateButtons(pos == 15, 3);
        else
            return;
    }
    else
    {
        if(pos ==0 || pos == 1)
            rotateButtons(pos ==0, 0);
        if(pos ==2 || pos == 3)
            rotateButtons(pos ==2, 1);
        if(pos ==4 || pos == 5)
            rotateButtons(pos ==4, 0);
        else if(pos == 6 || pos == 7)
            rotateButtons(pos ==6, 1);
        else if(pos == 8)
            rotateButtons(true, 2);
        else if(pos == 9 || pos == 10)
            rotateButtons(pos == 9, 4);
        else if(pos == 11)
            rotateButtons(false, 3);
        else if(pos == 12 || pos == 13)
            rotateButtons(pos == 12, 2);
        else if(pos == 14 || pos == 15)
            rotateButtons(pos == 14, 3);
        else
            return;
    }
}

void GameArea::rotateButtons(bool clockWise, int rectIndex)
{
    if(rectIndex == 0)
    {
        int index0 = findButtonIndex(0);
        int index1 = findButtonIndex(1);
        int index4 = findButtonIndex(4);
        int index5 = findButtonIndex(5);
        if(clockWise)
        {
            buttons[index0]->newPos = &positions[1];
            buttons[index1]->newPos = &positions[5];
            buttons[index4]->newPos = &positions[0];
            buttons[index5]->newPos = &positions[4];
        }
        else
        {
            buttons[index0]->newPos = &positions[4];
            buttons[index1]->newPos = &positions[0];
            buttons[index4]->newPos = &positions[5];
            buttons[index5]->newPos = &positions[1];
        }
    }
    else if(rectIndex == 1)
    {
        int index2 = findButtonIndex(2);
        int index3 = findButtonIndex(3);
        int index6 = findButtonIndex(6);
        int index7 = findButtonIndex(7);
        if(clockWise)
        {
            buttons[index2]->newPos = &positions[3];
            buttons[index3]->newPos = &positions[7];
            buttons[index6]->newPos = &positions[2];
            buttons[index7]->newPos = &positions[6];
        }
        else
        {
            buttons[index2]->newPos = &positions[6];
            buttons[index3]->newPos = &positions[2];
            buttons[index6]->newPos = &positions[7];
            buttons[index7]->newPos = &positions[3];
        }
    }
    else if(rectIndex == 2)
    {
        int index8 = findButtonIndex(8);
        int index9 = findButtonIndex(9);
        int index12 = findButtonIndex(12);
        int index13 = findButtonIndex(13);
        if(clockWise)
        {
            buttons[index8]->newPos = &positions[9];
            buttons[index9]->newPos = &positions[13];
            buttons[index12]->newPos = &positions[8];
            buttons[index13]->newPos = &positions[12];
        }
        else
        {
            buttons[index8]->newPos = &positions[12];
            buttons[index9]->newPos = &positions[8];
            buttons[index12]->newPos = &positions[13];
            buttons[index13]->newPos = &positions[9];
        }
    }
    else if(rectIndex == 3)
    {
        int index10 = findButtonIndex(10);
        int index11 = findButtonIndex(11);
        int index14 = findButtonIndex(14);
        int index15 = findButtonIndex(15);
        if(clockWise)
        {
            buttons[index10]->newPos = &positions[11];
            buttons[index11]->newPos = &positions[15];
            buttons[index14]->newPos = &positions[10];
            buttons[index15]->newPos = &positions[14];
        }
        else
        {
            buttons[index10]->newPos = &positions[14];
            buttons[index11]->newPos = &positions[10];
            buttons[index14]->newPos = &positions[15];
            buttons[index15]->newPos = &positions[11];
        }
    }
    else if(rectIndex == 4)
    {
        int index5 = findButtonIndex(5);
        int index6 = findButtonIndex(6);
        int index9 = findButtonIndex(9);
        int index10 = findButtonIndex(10);
        if(clockWise)
        {
            buttons[index5]->newPos = &positions[6];
            buttons[index6]->newPos = &positions[10];
            buttons[index9]->newPos = &positions[5];
            buttons[index10]->newPos = &positions[9];
        }
        else
        {
            buttons[index5]->newPos = &positions[9];
            buttons[index6]->newPos = &positions[5];
            buttons[index9]->newPos = &positions[10];
            buttons[index10]->newPos = &positions[6];
        }
    }
    foreach(GameButton* btn, buttons)
        btn->moveToPos();

    checkEndingAndHighScore();
}

int GameArea::findButtonIndex(int index)
{
    for(int i=0;i<buttons.size();++i)
    {
        if(buttons.at(i)->newPos->index == positions.at(index).index)
            return i;
    }
    return 0;
}
void GameArea::moveButtonsUp()
{
    for(int i=0;i<buttons.size();++i)
    {
        buttons.at(i)->newPos = &upPositions[i];
    }

    foreach(GameButton* btn, buttons)
        btn->moveToPos();
}

void GameArea::checkEndingAndHighScore()
{
    if(!gameOn)
        return;

    if(gameMode == 0)
    {
        foreach(GameButton *btn, buttons)
        {
            if(!btn->isCorrectColorPos())
                return;
        }
    }
    else if(gameMode==1)
    {
        foreach(GameButton *btn, buttons)
        {
            if(btn->newPos->index != btn->index)
                return;
        }
    }

    scoreSeconds = gameTime.elapsed();
    gameOn = false;

    QPropertyAnimation *animation = new QPropertyAnimation(lblSolved, "pos");
    animation->setDuration(1000);
    animation->setStartValue(lblSolved->pos());
    animation->setKeyValueAt(0.75, QPoint(this->width()/2-lblSolved->width()/2,
                                          this->height()-lblSolved->height()/2));
    animation->setEndValue(QPoint(this->width()/2-lblSolved->width()/2,
                                  this->height()/2-lblSolved->height()/2));
    QEasingCurve curve;
    curve.setType(QEasingCurve::OutBack);
    animation->setEasingCurve(curve);
    animation->start();

    if(scores.isHighScore(gameMode,(double)scoreSeconds))
    {
        QTimer::singleShot(1000, this, SLOT(getScoreNameAndSave()));
    }
}
void GameArea::getScoreNameAndSave()
{
    emit releaseScreen();

    QString name = QInputDialog::getText(this, "High score",
                                         "Name",QLineEdit::Normal,
                                         defName);
    emit lockScreen();

    name.truncate(20);
    if(name.isEmpty())
        name = "Unknown";
    else
        defName = name;
    scores.addToList(gameMode,name,(double) scoreSeconds);
    scores.saveScores(scoreFile, "Bloxated");
}
