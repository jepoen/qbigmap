#ifndef TILE_H
#define TILE_H
#include <QString>

const QString MAPYCZ = "http://mapserver.mapy.cz";

class Tile
{
private:
    QString myMaptype;
    int myZoom;
    int myX;
    int myY;
public:
    Tile(const QString& maptype, int zoom, int x, int y);
    void download();
};

#endif // TILE_H
