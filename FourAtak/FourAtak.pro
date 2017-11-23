#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T17:12:18
#
#-------------------------------------------------

QT       += core gui

TARGET = FourAtak
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hitbutton.cpp \
    highscores.cpp \
    flickcharm.cpp

HEADERS  += mainwindow.h \
    hitbutton.h \
    highscores.h \
    flickcharm.h

FORMS    += mainwindow.ui

#CONFIG += mobility
#MOBILITY =

symbian {
    LIBS += -lcone -leikcore -lavkon
    TARGET.UID3 = 0x20035CE6
    symbian:DEPLOYMENT.installer_header = 0x2002CCCF
    #TARGET.UID3 = 0xe4821439
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
    ICON = iconNew.svg
    packageheader = "$${LITERAL_HASH}{\"Four Atak\"}, (0x20035CE6), 1, 1, 0, TYPE=SA"
    vendorinfo = "%{\"T.Makalainen\"}" ":\"T.Makalainen\""
    my_deployment.pkg_prerules = packageheader vendorinfo
    DEPLOYMENT += my_deployment
}


