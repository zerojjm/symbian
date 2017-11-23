#-------------------------------------------------
#
# Project created by QtCreator 2010-10-14T16:25:57
#
#-------------------------------------------------

QT       += core gui

TARGET = Beerculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calculator.cpp \
    flickcharm.cpp

HEADERS  += mainwindow.h \
    calculator.h \
    flickcharm.h

FORMS    += mainwindow.ui

#CONFIG += mobility
#MOBILITY =

symbian {
    TARGET.UID3 = 0x20035CE4
    DEPLOYMENT.installer_header = 0x2002CCCF
    #TARGET.UID3 = 0xe2601979

    # TARGET.CAPABILITY +=
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
    ICON = iconNew4Tiny.svg
    packageheader = "$${LITERAL_HASH}{\"Beerculator\"}, (0x20035CE4), 1, 1, 0, TYPE=SA"
    vendorinfo = "%{\"T.Makalainen\"}" ":\"T.Makalainen\""
    my_deployment.pkg_prerules = packageheader vendorinfo
    DEPLOYMENT += my_deployment

}
