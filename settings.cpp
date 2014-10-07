#include <QDir>
#include <QMainWindow>
#include <QSettings>
#include <QStringList>
#include <QtDebug>
#include "settings.h"

const QString LAYER_NAME = "Mapnik";
const QString LAYER_URL = "http://tile.openstreetmap.org/$z/$x/$y.png";
QString defaultIcons[][3] = {
    {"flag", ":/icons/flag-s.png", ":/icons/flag.png"},
    {"star", ":/icons/star-s.png", ":/icons/star.png"},
    {"square", ":/icons/square.png", ":/icons/square.png"},
    {"place", ":/icons/place-s.png", ":/icons/place.png"},
    {"church", ":/icons/church-s.png", ":/icons/church.png"},
    {"hostel", ":/icons/hostel-s.png", ":/icons/hostel.png"},
    {"shell", ":/icons/shell-s.png", ":/icons/shell.png"},
    {"pilgrim", ":/icons/pilgrim-hostel-s.png", ":/icons/pilgrim-hostel.png"},
    {0, 0, 0}
};

Layer::Layer(const QString &name, const QString &url) :
        myName(name), myUrl(url)
{}

Settings::Settings()
{}

int Settings::iconIndex(const QString icons[][3], const QString &key) {
    for (int i = 0; icons[i][0] != 0; i++) {
        if (icons[i][0] == key) return i;
    }
    return -1;
}

void Settings::load(QMainWindow *win) {
    QSettings *settings = new QSettings("osm", "QBigMap");
    //qDebug()<<"settings size"<<settings->allKeys().size();
    if (settings->allKeys().size() == 0) {
        //delete settings;
        //settings = new QSettings(":/resources/QBigMap.conf", QSettings::IniFormat);
        //qDebug()<<"settings size"<<settings->allKeys().size();
    }
    myCenter = settings->value("center", QPointF(12.9, 50.8)).toPointF();
    myXext = settings->value("xext", "5").toInt();
    myYext = settings->value("yext", "4").toInt();
    myZoom = settings->value("zoom", 6).toInt();
    myTileSize = settings->value("tilesize", 45).toInt();
    myPrintBorder = settings->value("printborder", 5).toInt();
    myGpsbabel = settings->value("gpsbabel", "gpsbabel").toString();
    myGpsDevice = settings->value("gpsDevice", "garmin").toString();
    myGpsInterface = settings->value("gpsInterface", "usb:").toString();
    myTrackDir = settings->value("trackdir", ".").toString();
    myUseSrtm = settings->value("usesrtm", "false").toBool();
    mySrtmDir = settings->value("srtmdir", ".").toString();
    myCacheDir = settings->value("cachedir", "/tmp").toString();
    myExportDir = settings->value("exportdir", ".").toString();
    myOutTrackColor = QColor::fromRgba(settings->value("outTrackColor", QString("%1").arg(0xff7f007f)).toInt());
    myOutTrackWidth = settings->value("outTrackWidth", "2").toInt();
    myOutRouteColor = QColor::fromRgba(settings->value("outRouteColor", QString("%1").arg(0xff00007f)).toInt());
    myOutRouteWidth = settings->value("outRouteWidth", "4").toInt();
    int size = settings->beginReadArray("baseLayers");
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        Layer l(settings->value("name").toString(), settings->value("url").toString());
        myBaseLayers.append(l);
    }
    settings->endArray();
    if (size == 0) {
        myBaseLayers.append(Layer(LAYER_NAME, LAYER_URL));
    }
    size = settings->beginReadArray("overlays");
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        Layer l(settings->value("name").toString(), settings->value("url").toString());
        myOverlays.append(l);
    }
    settings->endArray();
    QList<MapIcon> icons;
    for (int i = 0; defaultIcons[i][0] != 0; i++) {
        icons.append(MapIcon(defaultIcons[i][0], defaultIcons[i][1], defaultIcons[i][2], defaultIcons[i][2]));
    }
    size = settings->beginReadArray("mapicons");
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        int idx = iconIndex(defaultIcons, settings->value("name").toString());
        if (idx >= 0) {
            icons[idx] = MapIcon(settings->value("name").toString(), defaultIcons[idx][1],
                         defaultIcons[idx][2], settings->value("mapicofile").toString());
        } else {
            // added icons
            icons.push_back(MapIcon(settings->value("name").toString(), settings->value("icofile").toString(),
                                    settings->value("mapicofile").toString(), settings->value("mapicofile").toString()));
        }
    }
    settings->endArray();
    myTrackDir = replaceHome(myTrackDir);
    mySrtmDir = replaceHome(mySrtmDir);
    myExportDir = replaceHome(myExportDir);
    myMapIcons.setIcons(icons);
    win->move(settings->value("mainPos", QPoint(50, 50)).toPoint());
    win->resize(settings->value("mainSize", QSize(800, 600)).toSize());
}

QString Settings::replaceHome(const QString &dir) const {
    if (dir[0] == '~') {
        return QDir::homePath()+dir.mid(1);
    } else {
        return dir;
    }
}

void Settings::save(QMainWindow *win) {
    QSettings settings("osm", "QBigMap");
    settings.beginWriteArray("baseLayers");
    for (int i = 0; i < myBaseLayers.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("name", myBaseLayers[i].name());
        settings.setValue(("url"), myBaseLayers[i].url());
    }
    settings.endArray();
    settings.beginWriteArray("overlays");
    for (int i = 0; i < myOverlays.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("name", myOverlays[i].name());
        settings.setValue(("url"), myOverlays[i].url());
    }
    settings.endArray();
    settings.beginWriteArray("mapicons");
    const QList<MapIcon>& icons = myMapIcons.icons();
    for (int i = 0; i < icons.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("name", icons[i].name());
        settings.setValue("icofile", icons[i].icoFile());
        settings.setValue("mapicofile", icons[i].mapIcoFile());
    }
    settings.endArray();
    settings.setValue("center", myCenter);
    settings.setValue("zoom", myZoom);
    settings.setValue("xext", myXext);
    settings.setValue("yext", myYext);
    settings.setValue("tilesize", myTileSize);
    settings.setValue("printborder", myPrintBorder);
    settings.setValue("gpsbabel", myGpsbabel);
    settings.setValue("gpsDevice", myGpsDevice);
    settings.setValue("gpsInterface", myGpsInterface);
    settings.setValue("trackdir", myTrackDir);
    settings.setValue("usesrtm", myUseSrtm);
    settings.setValue("srtmdir", mySrtmDir);
    settings.setValue("cachedir", myCacheDir);
    settings.setValue("exportdir", myExportDir);
    settings.setValue("outTrackColor", myOutTrackColor.rgba());
    settings.setValue("outTrackWidth", myOutTrackWidth);
    settings.setValue("outRouteColor", myOutRouteColor.rgba());
    settings.setValue("outRouteWidth", myOutRouteWidth);
    settings.setValue("mainPos", win->pos());
    settings.setValue("mainSize", win->size());
}
