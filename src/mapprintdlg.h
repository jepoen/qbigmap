#ifndef MAPPRINTDLG_H
#define MAPPRINTDLG_H

#include <QDialog>
#include <QPointF>
#include <QSize>

class QLabel;
class QSpinBox;
class QPrinter;
class Settings;

class MapPrintDlg : public QDialog
{
    Q_OBJECT
    QSpinBox *eTileSize;
    QSpinBox *ePrintBorder;
    QLabel *oPages;
    QLabel *oTiles;
    QLabel *oMargins;

    QPrinter *myDevice;
    Settings *mySettings;
    QSize myImgSize;
    QSize myTilesPerPage;
    QSize myPages;
    double myMargW; // map margin in mm
    double myMargH;
    QPointF myDevBorder;  // print border in device coords
    QPointF myDevMargins; // map margin in device coords
public:
    explicit MapPrintDlg(QPrinter *printer, Settings *settings, QSize imgSize, QWidget *parent = 0);
    QSize tilesPerPage() const { return myTilesPerPage; }
    QSize pages() const { return myPages; }
    QPointF devBorder() const { return myDevBorder; }
    QPointF devMargin() const { return myDevMargins; }
signals:
    
private slots:
    void computeLayout();
    void saveSettings();
public slots:
    
};

#endif // MAPPRINTDLG_H
