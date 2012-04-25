#ifndef SRTM_H
#define SRTM_H

#include <QString>

class SrtmEntry
{
private:
    int myLon;
    int myLat;
    int *myData;

    void readFile(const QString& srtmDir);
public:
    SrtmEntry(int lon, int lat, const QString& srtmDir):
        myLon(lon), myLat(lat), myData(0)
    {
        readFile(srtmDir);
    }
    ~SrtmEntry() { delete [] myData; }
    QString getFileName(const QString& srtmEntry) const;
    int lon0() const { return myLon; }
    int lat0() const { return myLat; }
    /**
      * Get elevation relative to lon0, lat0.
      */
    int offEle(double offLon, double offLat) const;
    int ele(double lon, double lat) const { return offEle(lon-myLon, lat-myLat); }
};

#endif // SRTM_H
