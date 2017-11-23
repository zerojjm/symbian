#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "GameArea.h"
#include "flickcharm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();
public slots:
    void showGamePage();
    void showHelpPage();
    void showScoresPage();
    void showGamePageTimed();
    void showHelpPageTimed();
    void showScoresPageTimed();
    //void hideTitlePage();
    void startSwitchAnim();
    void updateScores();
    void screenRelease();
    void screenLock();
private:
    Ui::MainWindow *ui;
    GameArea* gameArea;
    int areaSize;
    FlickCharm* charm;
};

#endif // MAINWINDOW_H
