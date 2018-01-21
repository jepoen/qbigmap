#ifndef GPXPROFILE_H
#define GPXPROFILE_H

#include "gpx.h"

class Settings;
class QPaintDevice;

class GpxProfile
{
private:
    GpxPointList myGpxPoints;
    Settings *mySettings;
    int myOffset;
public:
    enum {ELE, SRTM};
    GpxProfile(const GpxPointList& gpxPoints, Settings *settings):
        myGpxPoints(gpxPoints),
        mySettings(settings),
        myOffset(20)
    {}
    void paint(QPaintDevice *dev, int variant, int width, int height, int top=0, int textwidth=0);
};

#endif // GPXPROFILE_H
