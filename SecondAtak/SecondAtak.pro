#-------------------------------------------------
#
# Project created by QtCreator 2010-10-25T17:12:18
#
#-------------------------------------------------

QT       += core gui

TARGET = SecondAtak
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hitbutton.cpp \
    highscores.cpp

HEADERS  += mainwindow.h \
    hitbutton.h \
    highscores.h

FORMS    += mainwindow.ui

#CONFIG += mobility
#MOBILITY =

symbian {
    LIBS += -lcone -leikcore -lavkon
    TARGET.UID3 = 0x20035CE5
    #TARGET.UID3 = 0xe4821439
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
    ICON = icon.svg
    packageheader = "$${LITERAL_HASH}{\"Second Atak\"}, (0x20035CE5), 1, 1, 0, TYPE=SA"
    vendorinfo = \
         "%{\"T.Makalainen\"}" \
         ":\"T.Makalainen\""
    my_deployment.pkg_prerules = packageheader vendorinfo
    DEPLOYMENT += my_deployment
}
