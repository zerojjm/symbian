#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <QFrame>
#include <QList>
#include <QPoint>
#include <QInputDialog>
#include <QTime>
#include <QApplication>
#include "GameButton.h"
#include "FrameLines.h"
#include "HighScores.h"

class GameArea : public QFrame
{
    Q_OBJECT
public:
    explicit GameArea(QWidget *parent = 0);

    HighScores scores;

signals:
    void releaseScreen();
    void lockScreen();
protected:
    bool event(QEvent *event);
public slots:
    void setInitialPositions(int areaWidth);
    void newGame();
    void shuffle();
    void launchShuffle();
    void setNumberMode();
    void setColorMode();

    void startDragging(int index);

private slots:
    void draggedHorizontal(bool dragRight);
    void draggedVertical(bool dragDown);

    void rotateButtons(bool clockWise, int rectIndex);
    int findButtonIndex(int index);
    void moveButtonsUp();

    void checkEndingAndHighScore();
    void getScoreNameAndSave();
private:
    QList<GameButton*> buttons;
    QList<Position> positions;
    QList<Position> upPositions;
    QList<FrameLines*> frameLines;

    //int solveDragIndex();

    int areaSize;
    bool dragging;
    int dragIndex;
    QPoint clickStart;

    int dragLength;
    int gameMode; //0=colors, 1=numbers

    QTime gameTime;
    int scoreSeconds;

    QString scoreFile;
    QString settingsFile;
    QString defName;
    bool gameOn;
    QLabel* lblSolved;
};

#endif // GAMEAREA_H
