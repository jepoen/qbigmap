#include <QtGui>
#include <QtNetwork>
#include "tile.h"

Tile::Tile(const QString &maptype, int zoom, int x, int y) :
        myMaptype(maptype), myZoom(zoom), myX(x), myY(y)
{
}

void Tile::download() {

}
