#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QSpinBox>
#include <QSettings>
#include <QStackedWidget>
#include <QDoubleValidator>
#include "calculator.h"
#include "flickcharm.h"

#define UNICODE_PER_MILLE_SIGN 0x2030
#define UNICODE_EURO_SIGN 0x20AC
#define UNICODE_POUND_SIGN 0x00A3
#define UNICODE_DOLLAR_SIGN 0x0024
#define UNICODE_CURRENCY_SIGN 0x00A4

#define CURR_EURO 0
#define CURR_POUND 1
#define CURR_DOLLAR 2
#define CURR_CURR 3

#define VOL_DL 0
#define VOL_OZ 1
#define VOL_OZUS 2

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    struct Settings{
        bool male;
        double weight;
        double addLimit;

        int currency;
        int volume;

        bool valid;

    };

    Ui::MainWindow *ui;
    void updateUiFromSettings();
    void loadSettingsFromFile();
    void saveSettingsToFile();
    void installValidators();
    Settings settings;

    QString sCurr;
    QString sVol;

    QStringList currencies;

    QAction *backSoftKeyAction;

    FlickCharm *charm;

private slots:
    void on_btnExit_clicked();
    void on_btnInfo_clicked();
    void on_btnSettings_clicked();
    void on_btnPermiller_clicked();
    void on_btnCompare_clicked();
    void on_btnBeerculator_clicked();
    void on_btnSaveSettings_clicked();
    void on_btnCalculatePermiller_clicked();
    void on_btnCalculateCompare_clicked();
    void on_btnCalculateBeer_clicked();

    void addBackButton();
    void removeBackButton();
    void mainMenu();
};

#endif // MAINWINDOW_H
