#include <QSettings>
#include <QStringList>
#include <QtDebug>
#include "settings.h"

const QString LAYER_NAME = "Mapnik";
const QString LAYER_URL = "http://tile.openstreetmap.org/$z/$x/$y.png";
QString defaultIcons[][3] = {
    {"flag", ":/icons/flag.png", ":/icons/flag.png"},
    {"church", ":/icons/church-s.png", ":/icons/church.png"},
    {"shell", ":/icons/shell-s.png", ":/icons/shell-png"},
    {0, 0, 0}
};

Layer::Layer(const QString &name, const QString &url) :
        myName(name), myUrl(url)
{}

Settings::Settings()
{
}

int Settings::mapIconIdx(const QString &key) const {
    for (int i = 0; i < myMapIcons.size(); i++) {
        if (myMapIcons[i].name() == key) return i;
    }
    return -1;
}

void Settings::load() {
    QSettings *settings = new QSettings("osm", "QBigMap");
    //qDebug()<<"settings size"<<settings->allKeys().size();
    if (settings->allKeys().size() == 0) {
        delete settings;
        settings = new QSettings(":/resources/QBigMap.conf", QSettings::IniFormat);
        //qDebug()<<"settings size"<<settings->allKeys().size();
    }
    myCenter = settings->value("center", QPointF(12.9, 50.8)).toPointF();
    myZoom = settings->value("zoom", 6).toInt();
    myTileSize = settings->value("tilesize", 45).toInt();
    myGpsbabel = settings->value("gpsbabel", "gpsbabel").toString();
    myGpsDevice = settings->value("gpsDevice", "garmin").toString();
    myGpsInterface = settings->value("gpsInterface", "usb:").toString();
    myTrackDir = settings->value("trackdir", ".").toString();
    mySrtmDir = settings->value("srtmdir", ".").toString();
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
    size = settings->beginReadArray("mapicons");
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        myMapIcons.append(MapIcon(settings->value("name").toString(), settings->value("icofile").toString(),
                                  settings->value("mapicofile").toString()));
    }
    settings->endArray();
    if (size == 0) {
        for (int i = 0; defaultIcons[i][0] != 0; i++) {
            myMapIcons.append(MapIcon(defaultIcons[i][0], defaultIcons[i][1], defaultIcons[i][2]));
        }
    }
}

void Settings::save() {
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
    for (int i = 0; i < myMapIcons.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("name", myMapIcons[i].name());
        settings.setValue("icofile", myMapIcons[i].icoFile());
        settings.setValue("mapicofile", myMapIcons[i].mapIcoFile());
    }
    settings.endArray();
    settings.setValue("center", myCenter);
    settings.setValue("zoom", myZoom);
    settings.setValue("tilesize", myTileSize);
    settings.setValue("gpsbabel", myGpsbabel);
    settings.setValue("gpsDevice", myGpsDevice);
    settings.setValue("gpsInterface", myGpsInterface);
    settings.setValue("trackdir", myTrackDir);
    settings.setValue("srtmdir", mySrtmDir);
    settings.setValue("outTrackColor", myOutTrackColor.rgba());
    settings.setValue("outTrackWidth", myOutTrackWidth);
    settings.setValue("outRouteColor", myOutTrackColor.rgba());
    settings.setValue("outRouteWidth", myOutTrackWidth);
}

