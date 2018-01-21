#include <QtCore>
#include "mapicon.h"
#include "osmmap.h"

QString OsmMap::pixEntry(const QString& name, const QString& fileName, const QPixmap& pix) {
    return QString("%1: new OpenLayers.Icon('%2',\n"
                   "  new OpenLayers.Size(%3, %4),\n"
                   "  new OpenLayers.Pixel(%5, %6)),\n")
            .arg(name).arg(fileName).arg(pix.width()).arg(pix.height())
            .arg(-pix.width()/2).arg(-pix.height()/2);
}

bool OsmMap::writeTrackFile(const QString &osmFileName, const QString &trackFileName, const QString& title,
                            const MapIconList& mapIconList) {
    QFile templateFile(":/resources/osm-track.html");
    if (!templateFile.open(QIODevice::ReadOnly|QIODevice::Text)) return false;
    QString text = templateFile.readAll();
    text = text.replace(QString("${lon}"), QString("%1").arg(myCenter.x()));
    text = text.replace(QString("${lat}"), QString("%1").arg(myCenter.y()));
    text = text.replace(QString("${zoom}"), QString("%1").arg(myZoom));
    text = text.replace(QString("${track}"), QFileInfo(trackFileName).fileName());
    text = text.replace(QString("${title}"), title);
    QFile osmFile(osmFileName);
    if (!osmFile.open(QIODevice::WriteOnly|QIODevice::Text)) return false;
    osmFile.write(text.toUtf8());
    osmFile.close();
    QDir mapDir(QFileInfo(osmFileName).absoluteDir());
    QString pixmaps;
    foreach (const MapIcon& icon, mapIconList.icons()) {
        QFile f(icon.mapIcoFile());
        QFileInfo fi(f);
        QPixmap pix(icon.mapIcoFile());
        pixmaps.append(pixEntry(icon.name(), fi.fileName(), pix));
        if (mapDir.exists(fi.fileName())) mapDir.remove(fi.fileName());
        f.copy(mapDir.absoluteFilePath(fi.fileName()));
    }
    QFile f(":/resources/qbm-track-osm.js");
    if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) return false;
    text = f.readAll();
    f.close();
    text = text.replace(QString("${pix}"), pixmaps);
    QFileInfo fi(f);
    QFile fo(mapDir.absoluteFilePath(fi.fileName()));
    if (!fo.open(QIODevice::WriteOnly|QIODevice::Text)) return false;
    fo.write(text.toUtf8());
    fo.close();
    return true;
}
