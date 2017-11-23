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
    hsReact.biggerBetter = false;
    hsMarathon.biggerBetter = true;
    QString buttonStyle = "QPushButton {border: 3px solid #8f8f91;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #660000, stop: 1 #ff0000);}QPushButton:pressed{border: 3px solid #8f8f8f;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #330000, stop: 1 #880000);}";
    ui->btnPlayMenu->setStyleSheet( buttonStyle);
    ui->textBrowser->setProperty("FingerScrollable", true);
    ui->tableScores->setProperty("FingerScrollable", true);

    btnHit = new HitButton(ui->frameArea);
    btnHit2 = new HitButton(ui->frameArea);
    btnHit3 = new HitButton(ui->frameArea);
    btnHit4 = new HitButton(ui->frameArea);

    btnList[0] = btnHit;
    btnList[1] = btnHit2;
    btnList[2] = btnHit3;
    btnList[3] = btnHit4;

    for(int i=0;i<4;++i)
    {
        showAnimation[i] = new QPropertyAnimation(this);
    }
    timer.setInterval(100);
    reactionTimer.setSingleShot(true);

    connect(btnHit, SIGNAL(pressed()), this, SLOT(buttonHitted()));
    connect(btnHit2, SIGNAL(pressed()), this, SLOT(button2Hitted()));
    connect(btnHit3, SIGNAL(pressed()), this, SLOT(button3Hitted()));
    connect(btnHit4, SIGNAL(pressed()), this, SLOT(button4Hitted()));

    connect(&timer,SIGNAL(timeout()),this, SLOT(timerTimeOut()));

    ui->tableScores->setColumnCount(3);
    ui->tableScores->horizontalHeader()->
            setResizeMode(QHeaderView::ResizeToContents);
    ui->tableScores->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableScores->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->frameArea->setAttribute(Qt::WA_AcceptTouchEvents, true);
    ui->frameArea->installEventFilter(this);

    charm = new FlickCharm(this);
    charm->activateOn(ui->tableScores);
    ui->tableScores->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->textBrowser);
    ui->textBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QButtonGroup* group1 = new QButtonGroup(this);
    group1->addButton(ui->rdoAtak);
    group1->addButton(ui->rdoMarathon);
    group1->addButton(ui->rdoReact);
    QButtonGroup* group2 = new QButtonGroup(this);
    group2->addButton(ui->rdoAtakScore);
    group2->addButton(ui->rdoMarathonScore);
    group2->addButton(ui->rdoReactionScore);
    QButtonGroup* group3 = new QButtonGroup(this);
    group3->addButton(ui->rdoEasy);
    group3->addButton(ui->rdoMedium);
    group3->addButton(ui->rdoHard);
    QButtonGroup* group4 = new QButtonGroup(this);
    group4->addButton(ui->rdoEasyScore);
    group4->addButton(ui->rdoMediumScore);
    group4->addButton(ui->rdoHardScore);

    connect(ui->rdoEasyScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoMediumScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoHardScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoAtakScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoMarathonScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));
    connect(ui->rdoReactionScore,SIGNAL(clicked()), this, SLOT(updateScoreTable()));

    connect(ui->rdoAtak,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoMarathon,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoReact,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoEasy,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoMedium,SIGNAL(clicked()), this, SLOT(readOptions()));
    connect(ui->rdoHard,SIGNAL(clicked()), this, SLOT(readOptions()));

    loadOptions();
    loadScore();

    updateSettings();
    srand(time(0));
    fillAlphapets();
}

MainWindow::~MainWindow()
{
    if(settingsChanged)
        saveOptions();
    delete ui;
}

void MainWindow::fillAlphapets()
{
    alphapets.clear();
    alphapets = (QStringList()
                 <<"A"<<"B"<<"C"<<"D"<<"E"<<"F"<<"G"<<"H"
                 <<"I"<<"J"<<"K"<<"L"<<"M"<<"N"<<"O"<<"P"
                 <<"Q"<<"R"<<"S"<<"T"<<"U"<<"V"<<"X"<<"Y"<<"Z");
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

void MainWindow::randNumbers(int end, int count, QList<int> &vec)
{
    vec.clear();
    QList<int> list;
    for(int i=0;i<=end;++i)
           list.push_back(i);

    int tmpRange = end+1;
    for(int j=0;j<count;++j)
    {
           int rval = rand()%tmpRange;
           vec.push_back(list.takeAt(rval));
           --tmpRange;
    }

}

void MainWindow::showAtakButtons()
{
    QList<int> btnTextVals;
    if(options.level==1)
    {
        randNumbers(8, 4, btnTextVals);
        qSort(btnTextVals.begin(), btnTextVals.end());
    }
    else if(options.level==2)
    {
        randNumbers(alphapets.size()-1, 4, btnTextVals);
        qSort(btnTextVals.begin(), btnTextVals.end());
    }
    for(int i = 0;i<4;++i)
    {
        if(options.level == 0)
        {
            btnList[i]->setText(QString::number(i+1));
        }
        else if(options.level == 1)
        {
            btnList[i]->setText(QString::number(btnTextVals.at(i)+1));
        }
        else if(options.level == 2)
        {
            btnList[i]->setText(alphapets.at(btnTextVals.at(i)));
        }
    }
    QList<int> randVals;
    randNumbers(11, 4, randVals);
    atakCount++;

    atakBoxHeight =  int (ui->frameArea->height()/3 -1);
    atakBoxWidth = int (ui->frameArea->width()/4 -1);

    for(int i=0;i<4;++i)
    {
        int row = 0;
        int col = 0;
        if(randVals.size()>i)
        {
            col = randVals.at(i)%4;
            if(randVals.at(i)<4)
                row = 0;
            else if(randVals.at(i)<8)
                row = 1;
            else
                row = 2;
        }
        atakBtnHitted[i] = false;
        btnList[i]->setRed();
        btnList[i]->x = col* atakBoxWidth + int((atakBoxWidth-atakBtnSize)/2);
        btnList[i]->y = row* atakBoxHeight + int((atakBoxHeight-atakBtnSize)/2);
        btnList[i]->resize(QSize(20,20));

        int resizeWidth = atakBtnSize-btnList[i]->width();
        int resizeHeight = atakBtnSize-btnList[i]->height();

        btnList[i]->setEnabled(true);
        btnList[i]->setVisible(true);
        showAnimation[i]->stop();
        showAnimation[i]->setTargetObject(btnList[i]);
        showAnimation[i]->setPropertyName("geometry");

        showAnimation[i]->setDuration(600);
        showAnimation[i]->setStartValue(QRect(btnList[i]->x+(resizeWidth/2),
                                   btnList[i]->y+(resizeHeight/2),
                                   btnList[i]->width(), btnList[i]->height()));
        showAnimation[i]->setEndValue(QRect(btnList[i]->x, btnList[i]->y, atakBtnSize, atakBtnSize));

        showAnimation[i]->setEasingCurve(QEasingCurve::OutElastic);
    }

    for(int i=0;i<4;++i)
        showAnimation[i]->start();
}

//only in reaction test
void MainWindow::showButton1()
{
    hideButton(btnHit2);
    showButton(btnHit,false);
}
//only in reaction test
void MainWindow::showButton2()
{
    hideButton(btnHit);
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
    if(reactionTest)
    {
        addScore();
        double time = clock.elapsed();
        reactionTimes.push_back(time);
        ui->lcdTime->display(QString::number(time,'f',1));
        if(reactionTimes.size()>=3)
            reactionTestFinished();
        else
            showButton2React();
        return;
    }
    atakButtonHitted(btnHit);

}
void MainWindow::button2Hitted()
{
    if(reactionTest)
    {
        addScore();
        double time = clock.elapsed();
        reactionTimes.push_back(time);
        ui->lcdTime->display(QString::number(time,'f',1));
        if(reactionTimes.size()>=3)
            reactionTestFinished();
        else
            showButton1React();
        return;
    }
    atakButtonHitted(btnHit2);
}
void MainWindow::button3Hitted()
{
    atakButtonHitted(btnHit3);
}
void MainWindow::button4Hitted()
{
    atakButtonHitted(btnHit4);
}

void MainWindow::atakButtonHitted(HitButton* btn)
{
    bool finished = false;
    for(int i=0;i<4;++i)
    {
        if(atakBtnHitted[i] == false)
        {
            if(btn == btnList[i])
            {
                atakBtnHitted[i] = true;
                if(options.mode == 1)
                    ui->lcdScore->display(ui->lcdScore->value()+1);
                btnList[i]->setGreen();
                if(i==3)
                    finished = true;
            }
            else
            {
                failCount++;
                if(options.mode == 0)
                    ui->lcdScore->display(failCount);
            }
            break;
        }
    }
    if(options.mode ==1)
    {
        if(finished)
            showAtakButtons();
        return;
    }
    if(finished && atakCount >=4 )
    {
        //finish
        timer.stop();
        for(int i=0;i<4;++i)
            btnList[i]->setVisible(false);
        checkHighScore();
    }
    else if(finished)
    {
        showAtakButtons();
    }
    else if (failCount>3)
    {
        timer.stop();
        for(int i=0;i<4;++i)
        {
            hideButton(btnList[i]);
        }
        ui->lblGameMessage->setText("Oops, you failed!");
        ui->lblGameMessage->show();
    }
    //else return and continue clicking
}

void MainWindow::addScore()
{
   ui->lcdScore->display(ui->lcdScore->intValue()+1);
}

void MainWindow::showButton(HitButton *btn, bool smallButton)
{
    int btnSize = 95;
    if(options.level<1)
        btnSize = 125;
    if(options.level==1)
        btnSize = 110;

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
    //not called, showAtakButtons used instead
    /*if(!btn->posValid)
    {
        int mainWidth = ui->frameArea->width();
        int mainHeight = ui->frameArea->height();
        int widthInterval = (mainWidth-btnSize);
        int heightInterval = (mainHeight-btnSize);
        btn->x = rand()%widthInterval;
        btn->y = rand()%heightInterval;
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
    }*/
}

void MainWindow::hideButton(HitButton *btn)
{
    btn->posValid = false;
    btn->setVisible(false);
}

void MainWindow::on_btnStart_clicked()
{
    atakBoxHeight =  int (ui->frameArea->height()/3 -1);
    atakBoxWidth = int (ui->frameArea->width()/4 -1);

    if(atakBoxHeight < atakBoxWidth)
        atakBtnSize = atakBoxHeight;
    else
        atakBtnSize = atakBoxWidth;

    if(atakBtnSize>100)
        atakBtnSize = 100;

    ui->lblGameMessage->hide();
    ui->lblGameMessage->setText("");

    ui->lcdTime->display(0.0);
    ui->lcdScore->display(0);

    if(!reactionTest)
    {
        atakCount = 0;
        failCount = 0;
        timer.start();
        clock.start();
        showAtakButtons();
    }
    else
    {
        reactionTimes.clear();
        showButton1React();
    }
}

void MainWindow::timerTimeOut()
{
    double val=((double)clock.elapsed())/1000;

    if(options.mode == 0 || options.mode == 1)
    {
        ui->lcdTime->display(QString::number(val,'f',1));
    }
    if(options.mode == 1 && val >=60)
    {
        //finish
        timer.stop();
        ui->lcdTime->display(QString::number(60.0,'f',1));
        for(int i=0;i<4;++i)
            btnList[i]->setVisible(false);
        checkHighScore();
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
    btnHit3->setVisible(false);
    btnHit3->posValid = false;
    btnHit4->setVisible(false);
    btnHit4->posValid = false;
    ui->stack->setCurrentWidget(ui->pageMenu);
}

void MainWindow::on_btnPlayMenu_clicked()
{
    if(settingsChanged)
        saveOptions();

    reactionTest = (options.mode == 2);

    //ui->lcdScore->setVisible(reactionTest);
    if(reactionTest || options.mode == 1)
    {
        ui->labelGameHits->setText("Hits");
        for(int i=0;i<4;++i)
            btnList[i]->setRed();
    }
    else
        ui->labelGameHits->setText("Fails");

    ui->lblGameMessage->setText("Click to Start!");
    ui->lblGameMessage->show();

    for(int i=0;i<4;++i)
    {
        btnList[i]->setText("");
    }
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
    options.mode = setup.value("mode", 0).toInt();
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

    if(ui->rdoAtak->isChecked())
        options.mode = 0;
    else  if(ui->rdoMarathon->isChecked())
        options.mode = 1;
    else  if(ui->rdoReact->isChecked())
        options.mode = 2;

    ui->rdoEasy->setEnabled(!ui->rdoReact->isChecked());
    ui->rdoMedium->setEnabled(!ui->rdoReact->isChecked());
    ui->rdoHard->setEnabled(!ui->rdoReact->isChecked());
    ui->lblDifficulty->setEnabled(!ui->rdoReact->isChecked());

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
        ui->rdoAtak->setChecked(true);
    else if(options.mode == 1)
        ui->rdoMarathon->setChecked(true);
    else if(options.mode == 2)
        ui->rdoReact->setChecked(true);

    ui->rdoEasy->setEnabled(!ui->rdoReact->isChecked());
    ui->rdoMedium->setEnabled(!ui->rdoReact->isChecked());
    ui->rdoHard->setEnabled(!ui->rdoReact->isChecked());
    ui->lblDifficulty->setEnabled(!ui->rdoReact->isChecked());

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
        double hitsPerSec = 16.0/val;
        ui->lcdTime->display(QString::number(val,'f',1));
        msg = "Time: "+QString::number(val,'f',2)
              +" s, " +QString::number(hitsPerSec,'f',2)
              +" hits/s";
        if(hsHits.isHighScore(options.level, val))
        {
            QString name = askName();
            hsHits.addToList(options.level,name,val);
            saveScore();

        }
    }
    else if(options.mode == 1)
    {
        double val=ui->lcdScore->value();
        double hitsPerSec = val/60;
        ui->lcdTime->display(QString::number(60.0,'f',1));
        msg = "Hits: "+QString::number(val,'f',0)
              +", " +QString::number(hitsPerSec,'f',2)
              +" hits/s";
        if(hsMarathon.isHighScore(options.level, val))
        {
            QString name = askName();
            hsMarathon.addToList(options.level,name,val);
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
    hsMarathon.loadScores(scoreFile,"MarathonMode");
}

void MainWindow::saveScore()
{
    if(reactionTest)
        hsReact.saveScores(scoreFile,"ReactMode");
    else if(options.mode == 0)
        hsHits.saveScores(scoreFile,"HitMode");
    else if(options.mode == 1)
        hsMarathon.saveScores(scoreFile,"MarathonMode");
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
        ui->rdoAtakScore->setChecked(true);
    else if(options.mode == 1)
        ui->rdoMarathonScore->setChecked(true);
    else if(options.mode == 2)
        ui->rdoReactionScore->setChecked(true);

    updateScoreTable();
    ui->stack->setCurrentWidget(ui->pageScore);
}

void MainWindow::updateScoreTable()
{

    ui->rdoEasyScore->setEnabled(!ui->rdoReactionScore->isChecked());
    ui->rdoMediumScore->setEnabled(!ui->rdoReactionScore->isChecked());
    ui->rdoHardScore->setEnabled(!ui->rdoReactionScore->isChecked());
    ui->lblDifficultyScore->setEnabled(!ui->rdoReactionScore->isChecked());

    ScoreList *list = 0;
    int mode = 0;

    if(ui->rdoReactionScore->isChecked())
    {
        mode = 2;
        list = &hsReact;
    }
    if(ui->rdoMarathonScore->isChecked())
    {
        mode = 1;
        list = &hsMarathon;
    }
    if(ui->rdoAtakScore->isChecked())
    {
        mode = 0;
        list = &hsHits;
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
            res =   16.0/p.value;
        else if (mode ==1)
            res = p.value/60;
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

    if(ui->rdoAtakScore->isChecked())
        vec = &hsHits.scores[level];
    else  if(ui->rdoMarathonScore->isChecked())
        vec = &hsMarathon.scores[level];
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
