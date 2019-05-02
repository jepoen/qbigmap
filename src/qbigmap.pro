# -------------------------------------------------
# Project created by QtCreator 2010-05-09T15:31:47
# -------------------------------------------------
QT += network \
    printsupport \
    xml \
    widgets
TARGET = qbigmap
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
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
    #photodlg.cpp \
    mapicon.cpp \
    geom.cpp \
    gpx.cpp \
    srtm.cpp \
    gpxpointdlg.cpp \
    gpxlistmodel.cpp \
    deltrkpartdlg.cpp \
    osmmap.cpp \
    trackexportdlg.cpp \
    photooffsetdlg.cpp \
    gpxprofile.cpp \
    gpxprofiledlg.cpp \
    helpwindow.cpp \
    boolspinbox.cpp \
    placedialog.cpp \
    mapprintdlg.cpp \
    mapiconeditdlg.cpp \
    photodlg.cpp \
    tile.cpp \
    wptmodel.cpp \
    managewaypointdlg.cpp \
    gpxlinkdlg.cpp \
    gpxsplitdialog.cpp \
    routeappenddlg.cpp
HEADERS += mainwindow.h \
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
    #photodlg.h \
    mapicon.h \
    geom.h \
    gpx.h \
    srtm.h \
    gpxpointdlg.h \
    gpxlistmodel.h \
    deltrkpartdlg.h \
    osmmap.h \
    trackexportdlg.h \
    photooffsetdlg.h \
    gpxprofile.h \
    gpxprofiledlg.h \
    helpwindow.h \
    boolspinbox.h \
    placedialog.h \
    mapprintdlg.h \
    mapiconeditdlg.h \
    photodlg.h \
    savetrackdlg.h \
    tile.h \
    wptmodel.h \
    managewaypointdlg.h \
    gpxlinkdlg.h \
    gpxsplitdialog.h \
    routeappenddlg.h
RESOURCES += \
    qbigmap.qrc
TRANSLATIONS = qbigmap_de.ts
LIBS += -l exiv2

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    resources/QBigMap.conf \
    icons/zoom_out.png \
    icons/zoom_in.png \
    icons/shell-s.png \
    icons/shell.png \
    icons/resultset_previous.png \
    icons/resultset_next.png \
    icons/resultset_last.png \
    icons/resultset_first.png \
    icons/printer.png \
    icons/information.png \
    icons/grid.xpm \
    icons/grid.png \
    icons/flag.png \
    icons/dprev.png \
    icons/dnext.png \
    icons/disk.png \
    icons/delete.png \
    icons/church-s.png \
    icons/church.png \
    icons/arrow_up.png \
    icons/arrow_right.png \
    icons/arrow_refresh.png \
    icons/arrow_left.png \
    icons/arrow_down.png \
    icons/add.png \
    icons/star.png \
    icons/hostel-s.png \
    icons/hostel.png \
    icons/star-s.png \
    icons/pilgrim-hostel-s.png \
    icons/pilgrim-hostel.png \
    icons/square.png \
    icons/world.png \
    help/index.html \
    help/map.html \
    help/qbigmap.css \
    icons/place-s.png \
    icons/place.png \
    help/track.html \
    icons/house.png \
    help/de/track.html \
    help/de/map.html \
    help/de/index.html \
    help/en/index.html \
    icons/peak.png \
    icons/viewpoint_s.png

DISTFILES += \
    mapicons/church.xpm




