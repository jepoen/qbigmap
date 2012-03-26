#include <QSettings>
#include <QStringList>
#include "settings.h"

Layer::Layer(const QString &name, const QString &url) :
        myName(name), myUrl(url)
{}

Settings::Settings()
{
}

void Settings::load() {
    QSettings *settings = new QSettings("knipling", "QBigMap");
    if (settings->allKeys().size() == 0) {
        delete settings;
        settings = new QSettings(":/resources/QBigMap.conf", QSettings::NativeFormat);
    }
    myCenter = settings->value("center", QPointF(12.9, 50.8)).toPointF();
    myZoom = settings->value("zoom", 6).toInt();
    myTileSize = settings->value("tilesize", 45).toInt();
    myGpsbabel = settings->value("gpsbabel", "gpsbabel").toString();
    myGpsDevice = settings->value("gpsDevice", "garmin").toString();
    myGpsInterface = settings->value("gpsInterface", "usb:").toString();
    myTrackDir = settings->value("trackdir", ".").toString();
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
}

void Settings::save() {
    QSettings settings("knipling", "QBigMap");
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
    settings.setValue("outTrackColor", myOutTrackColor.rgba());
    settings.setValue("outTrackWidth", myOutTrackWidth);
    settings.setValue("outRouteColor", myOutTrackColor.rgba());
    settings.setValue("outRouteWidth", myOutTrackWidth);
}
