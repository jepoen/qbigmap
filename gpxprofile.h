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
    GpxProfile(const GpxPointList& gpxPoints, Settings *settings):
        myGpxPoints(gpxPoints),
        mySettings(settings),
        myOffset(20)
    {}
    void paint(QPaintDevice *dev, int width, int height, int textwidth=0);
};

#endif // GPXPROFILE_H
