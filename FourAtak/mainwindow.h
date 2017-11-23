#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QTimer>
#include <QTime>
#include <QPropertyAnimation>
#include <QSettings>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QMessageBox>

#include "hitbutton.h"
#include "highscores.h"
#include "time.h"
#include "flickcharm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
     bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_btnBackHelp_clicked();
    void on_btnAboutMenu_clicked();
    void on_btnHelp_clicked();
    void on_btnBackAbout_clicked();
    void on_btnClearScore_clicked();
    void on_btnScoreMenu_clicked();
    void on_btnBackScore_clicked();
    void updateSettings();
    void readOptions();
    void on_btnExit_clicked();
    void on_btnPlayMenu_clicked();
    void on_btnBackGame_clicked();
    void on_btnStart_clicked();
    void buttonHitted();
    void button2Hitted();
    void button3Hitted();
    void button4Hitted();
    void atakButtonHitted(HitButton* btn);
    void timerTimeOut();
    void checkHighScore();
    void updateScoreTable();
    void showButton1();
    void showButton2();
    void showButton1React();
    void showButton2React();
    void showAtakButtons();

    void reactionTestFailed();
    void reactionTestFinished();
    QString askName();
private:
    struct Options{
        int level; //0 = easy, 1=medium, 2=hard
        int mode; // 0 = 100 hits, 1= marathon, 2= reaction
        QString defName;
    };
    bool settingsChanged;
    void showButton(HitButton *btn, bool smallButton);
    void hideButton(HitButton *btn);
    void addScore();

    void loadOptions();
    void saveOptions();
    void loadScore();
    void saveScore();
    void randNumbers(int end, int count, QList<int> &vec);
    Ui::MainWindow *ui;
    HitButton* btnHit;
    HitButton* btnHit2;
    HitButton* btnHit3;
    HitButton* btnHit4;
    QTimer timer;
    QTimer reactionTimer;
    QTime clock;
    QPropertyAnimation *showAnimation[4];
    QPropertyAnimation *hideAnimation[4];

    Options options;

    ScoreList hsHits;
    ScoreList hsReact;
    ScoreList hsMarathon;

    QString settingsFile;
    QString scoreFile;

    bool reactionTest;
    QVector<double> reactionTimes;

    QStringList alphapets;
    void fillAlphapets();

    int atakBtnSize;
    int atakBoxWidth;
    int atakBoxHeight;

    int atakCount;
    int failCount;
    bool atakBtnHitted[4];
    HitButton* btnList[4];

    FlickCharm *charm;
};

#endif // MAINWINDOW_H
