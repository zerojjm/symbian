#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settingsChanged = false;

    QString allBtnStyle = "QPushButton {padding:9px; border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #111111, stop: 1 #555555);} QPushButton:pressed{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #020202, stop: 1 #222222);}";

    QList<QPushButton *> allPButtons = ui->centralWidget->findChildren<QPushButton *>();
    foreach(QPushButton* btn,allPButtons)
    {
        if (ui->btnPlayMenu==btn)
            continue;
        //btn->setMinimumHeight(35);
        btn->setStyleSheet(allBtnStyle);
    }

    reactionTest = false;

    ui->lblGameMessage->hide();
    settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    scoreFile = QApplication::applicationDirPath() + "/score.ini";
    hsHits.biggerBetter = false;
    hsSec.biggerBetter = true;
    hsReact.biggerBetter = false;
    QString buttonStyle = "QPushButton {border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);}QPushButton:pressed{border: 3px solid #8f8f8f;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #330000, stop: 1 #880000);}";
    ui->btnPlayMenu->setStyleSheet( buttonStyle);
    ui->textBrowser->setProperty("FingerScrollable", true);
    ui->tableScores->setProperty("FingerScrollable", true);

    btnHit = new HitButton(ui->frameArea);

    btnHit2 = new HitButton(ui->frameArea);

    showAnimation = new QPropertyAnimation(this);
    timer.setInterval(100);
    reactionTimer.setSingleShot(true);

    connect(btnHit, SIGNAL(pressed()), this, SLOT(buttonHitted()));
    connect(btnHit2, SIGNAL(pressed()), this, SLOT(button2Hitted()));
    connect(&timer,SIGNAL(timeout()),this, SLOT(timerTimeOut()));

    ui->tableScores->setColumnCount(3);
    ui->tableScores->horizontalHeader()->
            setResizeMode(QHeaderView::ResizeToContents);
    ui->tableScores->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableScores->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->frameArea->setAttribute(Qt::WA_AcceptTouchEvents, true);
    ui->frameArea->installEventFilter(this);

    connect(ui->rdoEasyScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoMediumScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoHardScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdo100HitsScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdo10SecScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoReactionScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));

    connect(ui->rdo100hit,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdo10sec,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoReact,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoEasy,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoMedium,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoHard,SIGNAL(clicked()), this, SLOT(readOptions()));

    loadOptions();
    loadScore();

    updateSettings();
    srand(time(0));

}

MainWindow::~MainWindow()
{
    if(settingsChanged)
        saveOptions();
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->frameArea)
    {
         if (event->type() == QEvent::TouchBegin
             || event->type() == QEvent::MouseButtonPress)
         {
             if(reactionTimer.isActive())
             {
                 reactionTestFailed();
                 return true;
             }
             else if(!timer.isActive())
                 on_btnStart_clicked();
         }
    }
    return QObject::eventFilter(obj, event);
 }

void MainWindow::showButton1()
{
    hideButton(btnHit2);
    if(options.level<2 && !reactionTest)
        showButton(btnHit2,true);
    showButton(btnHit,false);
}
void MainWindow::showButton2()
{
    hideButton(btnHit);
    if(options.level<2 && !reactionTest)
        showButton(btnHit,true);
    showButton(btnHit2,false);
}
void MainWindow::showButton1React()
{
    hideButton(btnHit2);
    int time = rand()%4000;
    disconnect(&reactionTimer, SLOT(timeout()));
    connect(&reactionTimer,SIGNAL(timeout()), this, SLOT(showButton1()));
    reactionTimer.start(500+time);
}

void MainWindow::showButton2React()
{
    hideButton(btnHit);
    int time = rand()%4000;
    disconnect(&reactionTimer, SLOT(timeout()));
    connect(&reactionTimer,SIGNAL(timeout()), this, SLOT(showButton2()));
    reactionTimer.start(500+time);
}

void MainWindow::buttonHitted()
{
    addScore();
    if(reactionTest)
    {
        double time = clock.elapsed();
        reactionTimes.push_back(time);
        ui->lcdTime->display(QString::number(time,'f',1));
        if(reactionTimes.size()>=3)
            reactionTestFinished();
        else
            showButton2React();
        return;
    }
    if(hitsFinished())
    {
        hideButton(btnHit);
        hideButton(btnHit2);
        timer.stop();
        checkHighScore();
    }
    else
        showButton2();
}
void MainWindow::button2Hitted()
{
    addScore();
    if(reactionTest)
    {
        double time = clock.elapsed();
        reactionTimes.push_back(time);
        ui->lcdTime->display(QString::number(time,'f',1));
        if(reactionTimes.size()>=3)
            reactionTestFinished();
        else
            showButton1React();
        return;
    }
    if(hitsFinished())
    {
        hideButton(btnHit);
        hideButton(btnHit2);
        timer.stop();
        checkHighScore();
    }
    else
        showButton1();
}

bool MainWindow::hitsFinished()
{
    if(options.mode>0)
        return false;
    if(ui->lcdScore->intValue()>=100)
        return true;
    return false;
}

void MainWindow::addScore()
{
   ui->lcdScore->display(ui->lcdScore->intValue()+1);
}

void MainWindow::showButton(HitButton *btn, bool smallButton)
{
    boxHeight =  int (ui->frameArea->height()/3 -1);
    boxWidth = int (ui->frameArea->width()/4 -1);

    int btnSize = 85;
    if(options.level<1)
        btnSize = 115;
    if(options.level==1)
        btnSize = 95;

    if(btnSize>boxHeight)
        btnSize = boxHeight;
    if(btnSize>boxWidth)
        btnSize = boxWidth;

    if(reactionTest)
    {
        btn->setEnabled(true);
        int mainWidth = ui->frameArea->width();
        int mainHeight = ui->frameArea->height();
        btn->resize(mainWidth-20,mainHeight-20);
        btn->move(QPoint(10,10));
        btn->show();
        clock.start();
        return;
    }
    if(!btn->posValid)
    {
        int box = rand()%12;
        while(box == prevBox)
            box = rand()%12;
        prevBox = box;

        int col = box%4;
        int row = 0;
        if(box<4)
            row = 0;
        else if(box<8)
            row = 1;
        else
            row = 2;
        btn->x = col* boxWidth + int((boxWidth-btnSize)/2);
        btn->y = row* boxHeight + int((boxHeight-btnSize)/2);
        btn->posValid = true;
    }
    btn->resize(QSize(20,20));

    int resizeWidth = btnSize-btn->width();
    int resizeHeight = btnSize-btn->height();
    if(!smallButton)
    {
        btn->setEnabled(true);
        btn->setVisible(true);
        showAnimation->stop();
        showAnimation->setTargetObject(btn);
        showAnimation->setPropertyName("geometry");

        showAnimation->setDuration(600);
        showAnimation->setStartValue(QRect(btn->x+(resizeWidth/2),
                                   btn->y+(resizeHeight/2),
                                   btn->width(), btn->height()));
        showAnimation->setEndValue(QRect(btn->x, btn->y, btnSize, btnSize));

        showAnimation->setEasingCurve(QEasingCurve::OutElastic);

        showAnimation->start();
    }
    else
    {
        btn->setEnabled(false);
        btn->move(QPoint(btn->x+(resizeWidth/2),btn->y+(resizeHeight/2)));
        btn->resize(QSize(8,8));
        btn->show();
    }
}

void MainWindow::hideButton(HitButton *btn)
{
    btn->posValid = false;
    btn->setVisible(false);
}

void MainWindow::on_btnStart_clicked()
{
    ui->lblGameMessage->hide();
    ui->lblGameMessage->setText("");
    if(options.mode == 0 || reactionTest)
        ui->lcdTime->display(0.0);
    else if(options.mode == 1)
        ui->lcdTime->display(10.0);

    ui->lcdScore->display(0);
    if(!reactionTest)
    {
        timer.start();
        clock.start();
        showButton1();
    }
    else
    {
        reactionTimes.clear();
        prevBox = -1;
        showButton1React();
    }
}

void MainWindow::timerTimeOut()
{
    double val=((double)clock.elapsed())/1000;

    if(options.mode == 0)
    {
        ui->lcdTime->display(QString::number(val,'f',1));
    }
    else if(options.mode == 1)
    {
        val = 10.0-val;
        if(val<0.0)
        {
            timer.stop();
            ui->lcdTime->setStyleSheet( "" );
            ui->lcdTime->display(0.0);
            btnHit->setVisible(false);
            btnHit2->setVisible(false);
            checkHighScore();
        }
        else
        {
            if(val<2.0)
            {
                ui->lcdTime->setStyleSheet( "background-color: #990000" );
                ui->lcdTime->setAutoFillBackground(true);
            }
            ui->lcdTime->display(QString::number(val,'f',1));
        }
    }
}

void MainWindow::on_btnBackGame_clicked()
{
    timer.stop();
    ui->lcdTime->setStyleSheet( "" );
    ui->lcdTime->display(0.0);
    ui->lcdScore->display(0);
    btnHit->setVisible(false);
    btnHit->posValid = false;
    btnHit2->setVisible(false);
    btnHit2->posValid = false;
    ui->stack->setCurrentWidget(ui->pageMenu);
}

void MainWindow::on_btnPlayMenu_clicked()
{
    if(settingsChanged)
        saveOptions();

    reactionTest = (options.mode == 2);

    ui->lblGameMessage->setText("Click to Start!");
    ui->lblGameMessage->show();

    ui->stack->setCurrentWidget(ui->pageGame);
}

void MainWindow::on_btnExit_clicked()
{
    close();
}

void MainWindow::loadOptions()
{
    QSettings setup(settingsFile,QSettings::IniFormat);
    setup.beginGroup("Options");
    options.level = setup.value("level", 0).toInt();
    options.mode = setup.value("mode", 1).toInt();
    options.defName = setup.value("defName", "").toString();
    setup.endGroup();
}

void MainWindow::saveOptions()
{
    QSettings setup(settingsFile,QSettings::IniFormat);

    setup.beginGroup("Options");
    setup.setValue("level", options.level);
    setup.setValue("mode", options.mode);
    setup.setValue("defName", options.defName);
    setup.endGroup();

    settingsChanged = false;
}

void MainWindow::readOptions()
{
    if(ui->rdoEasy->isChecked())
        options.level = 0;
    else if(ui->rdoMedium->isChecked())
        options.level = 1;
    else if(ui->rdoHard->isChecked())
        options.level = 2;

    if(ui->rdo100hit->isChecked())
        options.mode = 0;
    else  if(ui->rdo10sec->isChecked())
        options.mode = 1;
    else  if(ui->rdoReact->isChecked())
        options.mode = 2;

    ui->gbDifficultyMain->setEnabled(!ui->rdoReact->isChecked());

    settingsChanged = true;
}

void MainWindow::updateSettings()
{
    if(options.level == 0)
        ui->rdoEasy->setChecked(true);
    else if(options.level == 1)
        ui->rdoMedium->setChecked(true);
    else if(options.level == 2)
        ui->rdoHard->setChecked(true);

    if(options.mode == 0)
        ui->rdo100hit->setChecked(true);
    else if(options.mode == 1)
        ui->rdo10sec->setChecked(true);
    else if(options.mode == 2)
        ui->rdoReact->setChecked(true);

    ui->gbDifficultyMain->setEnabled(!ui->rdoReact->isChecked());
}
QString MainWindow::askName()
{
    QString name = QInputDialog::getText(this, "High score",
                                         "Name to Top10 Table",QLineEdit::Normal,
                                         options.defName);
    name.truncate(15);
    if(name.isEmpty())
        name = "Unknown";
    else
    {
        options.defName = name;
        saveOptions();
    }
    return name;
}

void MainWindow::checkHighScore()
{
    QString msg;

    if(options.mode == 0)
    {
        double val=((double)clock.elapsed())/1000;
        ui->lcdTime->display(QString::number(val,'f',1));
        msg = "Time: "+QString::number(val,'f',2)
              +" s, " +QString::number(100/val,'f',2)
              +" hits/s";
        if(hsHits.isHighScore(options.level, val))
        {
            QString name = askName();
            hsHits.addToList(options.level,name,val);
            saveScore();
        }
    }
    if(options.mode == 1)
    {
        int val = ui->lcdScore->intValue();
        msg = QString::number(val)+" hits, "
              +QString::number(((double) val)/10.0,'f',2)
              +" hits/s.";

        if(hsSec.isHighScore(options.level, val))
        {
            QString name = askName();
            hsSec.addToList(options.level,name,val);
            saveScore();
        }
    }
    ui->lblGameMessage->setText(msg);
    ui->lblGameMessage->show();
}

void MainWindow::loadScore()
{
    hsReact.loadScores(scoreFile,"ReactMode");
    hsHits.loadScores(scoreFile,"HitMode");
    hsSec.loadScores(scoreFile,"TimeMode");
}

void MainWindow::saveScore()
{
    if(reactionTest)
        hsReact.saveScores(scoreFile,"ReactMode");
    else if(options.mode == 0)
        hsHits.saveScores(scoreFile,"HitMode");
    else if (options.mode == 1)
        hsSec.saveScores(scoreFile,"TimeMode");
}

void MainWindow::on_btnBackScore_clicked()
{
    ui->stack->setCurrentWidget(ui->pageMenu);
}

void MainWindow::on_btnScoreMenu_clicked()
{

    if(options.level == 0)
        ui->rdoEasyScore->setChecked(true);
    else if(options.level == 1)
        ui->rdoMediumScore->setChecked(true);
    else if(options.level == 2)
        ui->rdoHardScore->setChecked(true);

    if(options.mode == 0)
        ui->rdo100HitsScore->setChecked(true);
    else if(options.mode == 1)
        ui->rdo10SecScore->setChecked(true);
    else if(options.mode == 2)
        ui->rdoReactionScore->setChecked(true);

    updateScoreTable();
    ui->stack->setCurrentWidget(ui->pageScore);
}

void MainWindow::updateScoreTable()
{

    ui->gbDifficulty->setEnabled(!ui->rdoReactionScore->isChecked());

    ScoreList *list = 0;
    int mode = 0;

    if(ui->rdoReactionScore->isChecked())
    {
        mode = 2;
        list = &hsReact;
    }
    if(ui->rdo100HitsScore->isChecked())
    {
        mode = 0;
        list = &hsHits;
    }
    else if(ui->rdo10SecScore->isChecked())
    {
        mode = 1;
        list = &hsSec;
    }

    if(!list)
    {
        ui->tableScores->clear();
        return;
    }
    int level=0;
    if(ui->rdoEasyScore->isChecked())
        level = 0;
    else if(ui->rdoMediumScore->isChecked())
        level = 1;
    else if(ui->rdoHardScore->isChecked())
        level = 2;

    if(mode == 2) //reaction test
        level = 0;

    QStringList hHeaderList = (QStringList()<< tr("Name"));
    if(mode ==0)
        hHeaderList << tr("Time(s)");
    else if(mode == 1)
        hHeaderList << tr("Hits");
    else if(mode == 2)
        hHeaderList << tr("Time(ms)");

    hHeaderList<< tr("Hits/s");

    ui->tableScores->setHorizontalHeaderLabels(hHeaderList);

    QStringList vHeaderList;
    ui->tableScores->setRowCount(list->count(level));

    for(int i=0;i<list->count(level);++i)
    {
        vHeaderList<<QString::number(i+1);
        Player p = list->getAt(level,i);

        QTableWidgetItem* nameItem =  ui->tableScores->takeItem(i, 0);
        if(!nameItem)
            nameItem = new QTableWidgetItem();
        nameItem->setText(p.name);
        ui->tableScores->setItem(i,0,nameItem);

        QTableWidgetItem* valItem =  ui->tableScores->takeItem(i, 1);
        if(!valItem)
            valItem = new QTableWidgetItem();
        QString valText;
        if(mode ==0)
            valText = QString::number(p.value,'f',2);
        else if (mode ==1)
            valText = QString::number(p.value,'f',0);
        else if (mode ==2)
            valText = QString::number(p.value,'f',1);
        valItem->setText(valText);
        ui->tableScores->setItem(i,1,valItem);

        QTableWidgetItem* resItem =  ui->tableScores->takeItem(i, 2);
        if(!resItem)
            resItem = new QTableWidgetItem();
        double res;
        if(mode ==0)
            res =   100/p.value;
        else if (mode ==1)
            res = p.value/10;
        else if (mode ==2)
            res = 1/(p.value/1000);

        QString resText = QString::number(res,'f',2);;
        resItem->setText(resText);
        ui->tableScores->setItem(i,2,resItem);
    }
    ui->tableScores->setVerticalHeaderLabels(vHeaderList);

}

void MainWindow::reactionTestFailed()
{
    reactionTimer.stop();
    hideButton(btnHit);
    hideButton(btnHit2);
    ui->lblGameMessage->setText("Oops, you failed!");
    ui->lblGameMessage->show();

}

void MainWindow::reactionTestFinished()
{
    hideButton(btnHit);
    hideButton(btnHit2);
    reactionTimer.stop();
    qSort(reactionTimes);
    if(reactionTimes.size()<2)
        return;

    double result = (reactionTimes.at(0)+reactionTimes.at(1))/2;
    ui->lcdTime->display(QString::number(result,'f',3));
    QString msg = "Time: " +QString::number(result,'f',1)
                  +" ms, "+QString::number(1/(result/1000),'f',2)
                  +" hits/s";
    if(hsReact.isHighScore(0,result))
    {
        QString name = askName();
        hsReact.addToList(0,name,result);
        saveScore();

    }
    ui->lblGameMessage->setText(msg);
    ui->lblGameMessage->show();
}


void MainWindow::on_btnClearScore_clicked()
{
    int level=-1;
    if(ui->rdoEasyScore->isChecked())
        level = 0;
    else if(ui->rdoMediumScore->isChecked())
        level = 1;
    else if(ui->rdoHardScore->isChecked())
        level = 2;

    QVector<Player> *vec;

    if(ui->rdo100HitsScore->isChecked())
        vec = &hsHits.scores[level];
    else  if(ui->rdo10SecScore->isChecked())
        vec = &hsSec.scores[level];
    else  if(ui->rdoReactionScore->isChecked())
        vec = &hsReact.scores[level];

    vec->clear();

    saveScore();
    updateScoreTable();
}

void MainWindow::on_btnBackAbout_clicked()
{
    ui->stack->setCurrentWidget(ui->pageMenu);
}

void MainWindow::on_btnHelp_clicked()
{
    ui->stack->setCurrentWidget(ui->pageHelp);
}

void MainWindow::on_btnAboutMenu_clicked()
{
    ui->stack->setCurrentWidget(ui->pageAbout);
}

void MainWindow::on_btnBackHelp_clicked()
{
    ui->stack->setCurrentWidget(ui->pageMenu);
}
