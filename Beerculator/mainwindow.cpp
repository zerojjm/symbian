#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lblPermilleSignPermiller->setText(QChar(UNICODE_PER_MILLE_SIGN));
    ui->lblPermilleSignSettings->setText(QChar(UNICODE_PER_MILLE_SIGN));
    /*QString allBtnStyle = "QPushButton {padding:9px; border: 3px solid #660000;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #110000, stop: 1 #550000);} QPushButton:pressed{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #020000, stop: 1 #220000);}";

    QList<QPushButton *> allPButtons = ui->centralWidget->findChildren<QPushButton *>();
    foreach(QPushButton* btn,allPButtons)
    {
        //if (ui->btnPlayMenu==btn)
        //    continue;
        //btn->setMinimumHeight(35);
        btn->setStyleSheet(allBtnStyle);
    }*/
    //QString bgStyle = "QWidget{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #001100, stop: 1 #005500);}}";
    //ui->centralWidget->setStyleSheet(bgStyle);
    mainMenu();

    setWindowTitle("Beerculator");
    //fill comboboxes
    currencies<<QString(UNICODE_EURO_SIGN)
            <<QString(UNICODE_POUND_SIGN)
            <<QString(UNICODE_DOLLAR_SIGN)
            <<"AUD"
            <<"BRL"
            <<"CAD"
            <<"CHF"
            <<"CNY"
            <<"CZK"
            <<"DKK"
            <<"EEK"
            <<"INR"
            <<"ISK"
            <<"JPY"
            <<"NOK"
            <<"PLN"
            <<"RUB"
            <<"SEK"
            <<QString(UNICODE_CURRENCY_SIGN);
    ui->cmbCurrency->addItems(currencies);

    QStringList listVol;
    listVol<<"dl"
           <<"fl.oz."
           <<"fl.oz.(US)";
    ui->cmbVolUnit->addItems(listVol);

    installValidators();

    ui->lblPermilleZero->setText("0.0"+QString(UNICODE_PER_MILLE_SIGN)
                                 +" after");

    charm = new FlickCharm(this);
    charm->activateOn(ui->scrollAreaMain);
    ui->scrollAreaMain->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->scrollAreaBeer);
    ui->scrollAreaBeer->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->scrollAreaPermiller);
    ui->scrollAreaPermiller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->scrollAreaCompare);
    ui->scrollAreaCompare->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->scrollAreaSettings);
    ui->scrollAreaSettings->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->textCompare);
    ui->textCompare->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    charm->activateOn(ui->textBrowser_2);
    ui->textBrowser_2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    loadSettingsFromFile();
    updateUiFromSettings();

    backSoftKeyAction = new QAction(QString("Back"), this);
    backSoftKeyAction->setSoftKeyRole(QAction::NegativeSoftKey);
    QObject::connect(backSoftKeyAction, SIGNAL(triggered()),
    this, SLOT(mainMenu()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCalculateBeer_clicked()
{
    double amount = ui->edtAmountBeer->text().toDouble();

    //convert to dl
    if(settings.volume == VOL_OZ)
        amount = amount*0.3/1.055852392;
    else if(settings.volume == VOL_OZUS)
        amount = amount*0.3;

    double beers = Calculator::calcBeer(amount,
                                        ui->edtPercentBeer->text().toDouble());
    ui->edtBeers->setText(QString::number(beers,'f',2));
}

void MainWindow::on_btnCalculateCompare_clicked()
{
    double amount = ui->edtAmountCompare->text().toDouble();
    double percent = ui->edtPercentCompare->text().toDouble();
    double euros = ui->edtPriceCompare->text().toDouble();
    double dl = amount;
    //convert to dl
    if(settings.volume == VOL_OZ)
        dl = dl*0.3/1.055852392;
    else if(settings.volume == VOL_OZUS)
        dl = dl*0.3/1.014420681;

    double euroPerG = Calculator::calcEuroPerGram(dl,percent,euros);
    //print results
    QString text = ui->textCompare->toPlainText();
    QString sRes = QString::number(amount,'f',2)+sVol+"/"+
                   QString::number(percent,'f',2)+"%/"+
                   QString::number(euros,'f',2)+sCurr+" = "+
                   QString::number(euroPerG,'f',3)+sCurr+"/g";
    ui->textCompare->setPlainText(sRes+"\n"+text);
}

void MainWindow::on_btnCalculatePermiller_clicked()
{
    double beers = ui->edtAmountPermiller->text().toDouble();
    double beginTime = ui->edtStartedAgo->text().toDouble();
    double timeLeft, timeLeftLimit;

    double currentPermil = Calculator::calcPermille(beers, beginTime,
                                                    settings.weight,
                                   settings.male, timeLeft,
                                   settings.addLimit, timeLeftLimit);

    ui->leBacPermiller->setText(QString::number(currentPermil,'f', 2));
    ui->leZeroAfterPermiller->setText(QString::number(timeLeft,'f', 1));
    ui->leLimitAfter->setText(QString::number(timeLeftLimit,'f', 1));
}

void MainWindow::on_btnSaveSettings_clicked()
{
    settings.male = ui->rdoMale->isChecked();
    settings.weight = ui->edtWeight->text().toDouble();
    settings.addLimit = ui->edtAddLimit->text().toDouble();
    settings.currency = ui->cmbCurrency->currentIndex();
    settings.volume = ui->cmbVolUnit->currentIndex();
    settings.valid = true;
    saveSettingsToFile();
    updateUiFromSettings();
}

void MainWindow::updateUiFromSettings()
{
    ui->rdoMale->setChecked(settings.male);
    ui->rdoFemale->setChecked(!settings.male);
    ui->edtWeight->setText(QString::number(settings.weight,'f',1));
    ui->edtAddLimit->setText(QString::number(settings.addLimit,'f',2));
    if(settings.currency < ui->cmbCurrency->count() && settings.currency >= 0)
        ui->cmbCurrency->setCurrentIndex(settings.currency);
    if(settings.volume < ui->cmbVolUnit->count() && settings.volume >= 0)
        ui->cmbVolUnit->setCurrentIndex(settings.volume);

    ui->lblCheckSettings->setVisible(!settings.valid);

    if(settings.currency>=0 && settings.currency<currencies.size())
        sCurr = currencies.at(settings.currency);
    else
        sCurr = currencies.at(0);

    ui->lblCurrencySign->setText(sCurr);

    if(settings.volume == VOL_DL)
        sVol = "dl";
    else if(settings.volume == VOL_OZ)
        sVol = "fl.oz.";
    else if(settings.volume == VOL_OZUS)
        sVol = "fl.oz.";

    ui->lblUserLimitPermiller->setText(QString::number(settings.addLimit,'f',2)
                                       +QString(UNICODE_PER_MILLE_SIGN)
                                       +" after");
    ui->lblVolumeBeer->setText(sVol);
    ui->lblVolumeCompare->setText(sVol);
}

void MainWindow::loadSettingsFromFile()
{
    QString filename = QApplication::applicationDirPath() + "/settings.ini";
    QSettings setup(filename,QSettings::IniFormat);
    setup.beginGroup("Settings");
    settings.male = setup.value("male", true).toBool();
    settings.weight = setup.value("weight", 60.0).toDouble();
    settings.addLimit = setup.value("addLimit", 0.5).toDouble();
    settings.currency = setup.value("currency", 0).toInt();
    settings.volume = setup.value("volume", 0).toInt();
    settings.valid = setup.value("valid", false).toBool();
    setup.endGroup();
}
void MainWindow::saveSettingsToFile()
{
    QString filename = QApplication::applicationDirPath() + "/settings.ini";
    QSettings setup(filename,QSettings::IniFormat);

    setup.beginGroup("Settings");
    setup.setValue("male", settings.male);
    setup.setValue("weight", settings.weight);
    setup.setValue("addLimit", settings.addLimit);
    setup.setValue("currency", settings.currency);
    setup.setValue("volume", settings.volume);
    setup.setValue("valid", settings.valid);
    setup.endGroup();
}

void MainWindow::on_btnBeerculator_clicked()
{
    ui->mainStack->setCurrentWidget(ui->pageBeer);
    setWindowTitle("Beerculator");
    addBackButton();
}

void MainWindow::on_btnCompare_clicked()
{
    ui->mainStack->setCurrentWidget(ui->pageCompare);
    setWindowTitle("Compare");
    addBackButton();
}

void MainWindow::on_btnPermiller_clicked()
{
    ui->mainStack->setCurrentWidget(ui->pagePermiller);
    setWindowTitle("Permiller");
    addBackButton();
}

void MainWindow::on_btnSettings_clicked()
{
    ui->mainStack->setCurrentWidget(ui->pageSettings);
    setWindowTitle("Settings");
    addBackButton();
}

void MainWindow::on_btnInfo_clicked()
{
    ui->mainStack->setCurrentWidget(ui->pageInfo);
    setWindowTitle("About");
    addBackButton();
}

void MainWindow::on_btnExit_clicked()
{
    close();
}
void MainWindow::mainMenu()
{

    if(ui->mainStack->currentWidget()
        == ui->pageSettings)
        on_btnSaveSettings_clicked();
    setWindowTitle("Beerculator");
    ui->mainStack->setCurrentWidget(ui->pageMain);
    removeBackButton();
}

void MainWindow::addBackButton()
{
    addAction(backSoftKeyAction);
}

void MainWindow::removeBackButton()
{
    if(this->actions().contains(backSoftKeyAction))
        removeAction(backSoftKeyAction);
}
void MainWindow::installValidators()
{
    QDoubleValidator *valAmount = new QDoubleValidator(this);
    valAmount->setNotation(QDoubleValidator::StandardNotation);
    valAmount->setRange(0,10000,2);

    QDoubleValidator *valPercent = new QDoubleValidator(this);
    valPercent->setNotation(QDoubleValidator::StandardNotation);
    valPercent->setRange(0,100,2);

    QDoubleValidator *valHours = new QDoubleValidator(this);
    valHours->setNotation(QDoubleValidator::StandardNotation);
    valHours->setRange(0,100,2);

    QDoubleValidator *valPermille = new QDoubleValidator(this);
    valPermille->setNotation(QDoubleValidator::StandardNotation);
    valPermille->setRange(0,10,2);

    QDoubleValidator *valWeight = new QDoubleValidator(this);
    valWeight->setNotation(QDoubleValidator::StandardNotation);
    valWeight->setRange(0,600,2);

    ui->edtAddLimit->setValidator(valPermille);
    ui->edtAmountBeer->setValidator(valAmount);
    ui->edtAmountCompare->setValidator(valAmount);
    ui->edtAmountPermiller->setValidator(valAmount);
    ui->edtPercentBeer->setValidator(valPercent);
    ui->edtPercentCompare->setValidator(valPercent);
    ui->edtPriceCompare->setValidator(valAmount);
    ui->edtStartedAgo->setValidator(valHours);
    ui->edtWeight->setValidator(valWeight);

}
