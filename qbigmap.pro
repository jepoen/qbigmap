# -------------------------------------------------
# Project created by QtCreator 2010-05-09T15:31:47
# -------------------------------------------------
QT += network \
    xml
TARGET = qbigmap
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    tile.cpp \
    httpget.cpp \
    model.cpp \
    mapscene.cpp \
    mapview.cpp \
    viewfunction.cpp \
    layersdialog.cpp \
    layersmodel.cpp \
    centerdialog.cpp \
    track.cpp \
    trackseldialog.cpp \
    settings.cpp \
    settingsdialog.cpp \
    outputseldlg.cpp \
    trackposdlg.cpp \
    route.cpp \
    profilescene.cpp \
    profileview.cpp \
    tracksimplifydlg.cpp \
    saveroutedlg.cpp \
    graph.cpp \
    photo.cpp \
    photodlg.cpp
HEADERS += mainwindow.h \
    tile.h \
    httpget.h \
    model.h \
    mapscene.h \
    mapview.h \
    viewfunction.h \
    layersdialog.h \
    layersmodel.h \
    centerdialog.h \
    track.h \
    trackseldialog.h \
    settings.h \
    settingsdialog.h \
    trackposdlg.h \
    route.h \
    profilescene.h \
    profileview.h \
    tracksimplifydlg.h \
    outputseldlg.h \
    saveroutedlg.h \
    graph.h \
    photo.h \
    photodlg.h
RESOURCES += qbigmap.qrc
TRANSLATIONS = qbigmap_de.ts
LIBS += -lexif
