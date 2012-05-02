#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QObject>
#include <QList>
#include <QPoint>
#include "mapicon.h"

class Layer {
private:
    QString myName;
    QString myUrl;
public:
    Layer(const QString& name, const QString& url);
    const QString& name() const { return myName; }
    void setName(const QString& name) { myName = name; }
    const QString& url() const { return myUrl; }
    void setUrl(const QString& url) { myUrl = url; }
};

/*
class Overlay
{
private:
    QString myKey;
    int myZ;
public:
    Overlay(const QString& key, int z);
    const QString& key() { return myKey; }
    int z() { return myZ; }
};
*/
class Settings
{
private:
    QString myGpsbabel;
    QString myGpsDevice;
    QString myGpsInterface;
    int myZoom;
    QPointF myCenter;
    int myXext;
    int myYext;
    int myTileSize;
    QString myTrackDir;
    QString mySrtmDir;
    QList<Layer> myBaseLayers;
    QList<Layer> myOverlays;
    QColor myOutTrackColor;
    int myOutTrackWidth;
    QColor myOutRouteColor;
    int myOutRouteWidth;
    MapIconList myMapIcons;
public:
    Settings();
    int zoom() const { return myZoom; }
    void setZoom(int zoom) { myZoom = zoom; }
    const QPointF& center() const { return myCenter; }
    void setCenter(const QPointF& center) { myCenter = center; }
    int xExt() const { return myXext; }
    void setXExt(int xExt) { myXext = xExt; }
    int yExt() const { return myYext; }
    void setYExt(int yExt) { myYext = yExt; }
    int tileSize() const { return myTileSize; }
    void setTileSize(int size) { myTileSize = size; }
    const QList<Layer>& baseLayers() const { return myBaseLayers; }
    void setBaseLayers(const QList<Layer>& layers) { myBaseLayers = layers; }
    const QList<Layer>& overlays() const { return myOverlays; }
    void setOverlays(const QList<Layer>& layers) { myOverlays = layers; }
    const QString& gpsbabel() const { return myGpsbabel; }
    void setGpsbabel(const QString& gpsbabel) { myGpsbabel = gpsbabel; }
    const QString& gpsDevice() const { return myGpsDevice; }
    void setGpsDevice(const QString& device) { myGpsDevice = device; }
    const QString& gpsInterface() const { return myGpsInterface; }
    void setGpsInterface(const QString& interface) { myGpsInterface = interface; }
    const QString& trackDir() const { return myTrackDir; }
    void setTrackDir(const QString& dir) { myTrackDir = dir; }
    const QString& srtmDir() const { return mySrtmDir; }
    void setSrtmDir(const QString& dir) { mySrtmDir = dir; }
    const QColor& outTrackColor() const { return myOutTrackColor; }
    void setOutTrackColor(const QColor& co) { myOutTrackColor = co; }
    int outTrackWidth() const { return myOutTrackWidth; }
    void setOutTrackWidth(int val) { myOutTrackWidth = val; }
    const QColor& outRouteColor() const { return myOutRouteColor; }
    void setOutRouteColor(const QColor& co) { myOutRouteColor = co; }
    int outRouteWidth() const { return myOutRouteWidth; }
    void setOutRouteWidth(int val) { myOutRouteWidth = val; }
    const MapIconList& mapIconList() const { return myMapIcons; }
    void setMapIcons(const QList<MapIcon>& mapIcons) { myMapIcons = mapIcons; }
    void setMapIcon(int idx, const QString& fileName) { myMapIcons.setMapIco(idx, fileName); }
    void load();
    void save();
};

#endif // SETTINGS_H
