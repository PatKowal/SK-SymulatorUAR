QT += core gui widgets charts network

CONFIG += c++17

# Włącz API, które jest zgodne z wersją 5.x lub nowszą
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Pliki źródłowe
SOURCES += \
    ModelARX.cpp \
    PetlaSprzezeniaZwrotnego.cpp \
    ProstyUAR.cpp \
    RegulatorPID.cpp \
    Testy_ARX.cpp \
    Testy_PID.cpp \
    Testy_UAR.cpp \
    arxokno.cpp \
    main.cpp \
    mainwindow.cpp

# Pliki nagłówkowe
HEADERS += \
    ModelARX.h \
    NetMan.h \
    ProstyUAR.h \
    RegulatorPID.h \
    arxokno.h \
    mainwindow.h

# Pliki UI
FORMS += \
    arxokno.ui \
    mainwindow.ui

# Domyślne reguły instalacji
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
