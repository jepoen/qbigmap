#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "settings.h"

class QLineEdit;
class QSpinBox;
class QToolButton;
class QTabWidget;

class GpsTab : public QWidget
{
    Q_OBJECT
private:
    Settings *mySettings;
    QLineEdit *eGpsBabel;
    QPushButton *bGpsBabel;
    QLineEdit *eGpsDevice;
    QLineEdit *eGpsInterface;
public:
    GpsTab(Settings *settings, QWidget *parent = 0);
    const QString gpsbabel() const;
    const QString gpsDevice() const;
    const QString gpsInterface() const;
private slots:
    void selectGpsBabel();
};

class TrackTab : public QWidget
{
    Q_OBJECT
private:
    Settings *mySettings;
    QLineEdit *eTrackDir;
    QPushButton *bTrackDir;
public:
    TrackTab(Settings *settings, QWidget *parent = 0);
    const QString trackDir() const;
private slots:
    void selectTrackDir();
};

class PrintTab: public QWidget
{
    Q_OBJECT
private:
    Settings *mySettings;
    QSpinBox *bTileSize;
    QSpinBox *bTrackWidth;
    QPushButton *bTrackColor;
    QSpinBox *bRouteWidth;
    QPushButton *bRouteColor;
    QColor myTrackColor;
    QColor myRouteColor;

public:
    PrintTab(Settings *settings, QWidget *parent = 0);
    int tileSize() const;
    int trackWidth() const;
    QColor trackColor() const;
    int routeWidth() const;
    QColor routeColor() const;
private slots:
    void selTrackColor();
    void selRouteColor();

};


class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    Settings mySettings;
    QTabWidget *tabWidget;
    GpsTab *gpsTab;
    TrackTab *trackTab;
    PrintTab *printTab;
public:
    SettingsDialog(const Settings& settings, QWidget *parent = 0);
    const Settings& settings() const { return mySettings; }
    static QPixmap colorIcon(const QColor& col);
private slots:
    void accept();
};

#endif // SETTINGSDIALOG_H
