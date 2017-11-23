#include <QtGui/QApplication>
#include "mainwindow.h"

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Symbian specific code
#ifdef Q_OS_SYMBIAN
    CAknAppUi* appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    TRAPD(error,
    if (appUi) {
        // Lock application orientation into landscape
        appUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
        //appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
    }
    );
#endif
    w.showFullScreen();

/*#if defined(Q_WS_S60)
    w.showFullScreen();
#else
    w.showFullScreen();
#endif*/
#ifdef Q_OS_SYMBIAN
    if (appUi) {
        // Lock application orientation into landscape
        appUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
    }
#endif


    return a.exec();
}
