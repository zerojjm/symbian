#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <math.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageFront));
    QDesktopWidget* desktopWidget = QApplication::desktop();
    //get client rect.
    QRect clientRect = desktopWidget->availableGeometry();
    areaSize = clientRect.width()-40;
    if(areaSize>clientRect.height()-40)
        areaSize = clientRect.height()-40;

    ui->frameGameArea->setMinimumWidth(areaSize);
    ui->frameGameArea->setMinimumHeight(areaSize);
    ui->frameGameArea->setMaximumWidth(areaSize);
    ui->frameGameArea->setMaximumHeight(areaSize);
    ui->frameGameArea->resize(areaSize,areaSize);
    QGridLayout *gameLayout = new QGridLayout(ui->frameGameArea);
    gameLayout->setSpacing(0);
    gameLayout->setMargin(0);
    gameArea = new GameArea(ui->frameGameArea);
    gameLayout->addWidget(gameArea,0,0);

    ui->lblImage->setText("");
    ui->lblImage->setBackgroundRole(QPalette::Base);
    ui->lblImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->lblImage->setScaledContents(true);
    ui->lblImage->setPixmap(QPixmap::fromImage(QImage(":images/logo.png")));

    charm = new FlickCharm(this);
    charm->activateOn(ui->plainTextEdit);

    connect(ui->btnNewGame, SIGNAL(clicked()), gameArea, SLOT(newGame()));
    connect(ui->rdoColors,SIGNAL(clicked()), gameArea, SLOT(setColorMode()));
    connect(ui->rdoNumbers,SIGNAL(clicked()), gameArea, SLOT(setNumberMode()));
    connect(ui->btnBackHelp, SIGNAL(clicked()), this, SLOT(showGamePageTimed()));
    connect(ui->btnBackScores, SIGNAL(clicked()), this, SLOT(showGamePageTimed()));
    connect(ui->btnHelp, SIGNAL(clicked()), this, SLOT(showHelpPageTimed()));
    connect(ui->btnScores, SIGNAL(clicked()), this, SLOT(showScoresPageTimed()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(close()));

    connect(gameArea, SIGNAL(lockScreen()), this, SLOT(screenLock()));
    connect(gameArea, SIGNAL(releaseScreen()), this, SLOT(screenRelease()));
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
    QTimer::singleShot(2000, this, SLOT(showGamePage()));
    gameArea->setInitialPositions(areaSize);
}

/*void MainWindow::hideTitlePage()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->stack, "pos");
    animation->setDuration(600);
    animation->setStartValue(ui->stack->pos());
    animation->setKeyValueAt(0.5, QPoint(ui->stack->x(), -1000));
    animation->setEndValue(ui->stack->pos());
    animation->start();
    //ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageHelp));
    QTimer::singleShot(300, this, SLOT(showGamePage()));
}*/

void MainWindow::showGamePage()
{
    startSwitchAnim();
    QTimer::singleShot(400, this, SLOT(showGamePageTimed()));
}

void MainWindow::showGamePageTimed()
{
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageGame));
    if(ui->rdoColors->isChecked())
        gameArea->setColorMode();
    else
        gameArea->setNumberMode();
}
void MainWindow::showHelpPage()
{
    startSwitchAnim();
    //ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageHelp));
    QTimer::singleShot(400, this, SLOT(showHelpPageTimed()));

}
void MainWindow::showHelpPageTimed()
{
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageHelp));
}

void MainWindow::showScoresPage()
{
    startSwitchAnim();
    //ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageHelp));
    QTimer::singleShot(400, this, SLOT(showScoresPageTimed()));

}
void MainWindow::showScoresPageTimed()
{
    updateScores();
    ui->stack->setCurrentIndex(ui->stack->indexOf(ui->pageScores));
}
void MainWindow::startSwitchAnim()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->stack, "pos");
    animation->setDuration(500);
    animation->setStartValue(ui->stack->pos());
    animation->setKeyValueAt(0.5, QPoint(ui->stack->x(), -1000));
    animation->setEndValue(ui->stack->pos());
    animation->start();
}
void MainWindow::updateScores()
{

    QString strColors,strNumbers;

    int i=1;
    double min;
    double sec;
    foreach(const Player &p, gameArea->scores.scores[0])
    {
        min = ::floor((p.value)/60000.0);
        sec = p.value - min*60000;
        sec = sec/1000.0;
        QString timeString;
        if(min>0)
            timeString += QString::number((int)min)+" min ";
        timeString += QString::number(sec,'f',2)+" sec";
        strColors += QString::number(i)+". "+p.name+" ("+timeString+")\n";
        ++i;
    }
    i=1;
    foreach(const Player &p, gameArea->scores.scores[1])
    {
        min = ::floor((p.value)/60000.0);
        sec = p.value - min*60000;
        sec = sec/1000.0;
        QString timeString;
        if(min>0)
            timeString += QString::number((int)min)+" min ";
        timeString += QString::number(sec,'f',2)+" sec";
        strNumbers += QString::number(i)+". "+p.name+" ("+timeString+")\n";
        ++i;
    }

    ui->edtScoreColors->setPlainText(strColors);
    ui->edtScoreNumbers->setPlainText(strNumbers);
}

void MainWindow::screenRelease()
{
    setOrientation(MainWindow::ScreenOrientationAuto);
}

void MainWindow::screenLock()
{
    setOrientation(MainWindow::ScreenOrientationLockPortrait);
}
