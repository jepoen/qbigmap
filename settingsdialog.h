#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QAbstractTableModel>
#include <QDialog>
#include <QItemDelegate>
#include "settings.h"

class QLineEdit;
class QSpinBox;
class QTableView;
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

class SelectFileDelegate : public QItemDelegate {
    Q_OBJECT
public:
    SelectFileDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class DirTab : public QWidget
{
    Q_OBJECT
private:
    Settings *mySettings;
    QLineEdit *eTrackDir;
    QPushButton *bTrackDir;
    QLineEdit *eSrtmDir;
    QPushButton *bSrtmDir;
    QLineEdit *eExportDir;
    QPushButton *bExportDir;
public:
    DirTab(Settings *settings, QWidget *parent = 0);
    const QString trackDir() const;
    const QString srtmDir() const;
    const QString exportDir() const;

private slots:
    void selectTrackDir();
    void selectSrtmDir();
    void selectExportDir();
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

class IconTableModel: public QAbstractTableModel {
    Q_OBJECT
private:
    MapIconList myIcons;
public:
    IconTableModel(const MapIconList &icons, QObject *parent = 0);
    ~IconTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

class IconTab: public QWidget {
    Q_OBJECT
private:
    Settings *mySettings;
    QTableView *tab;
    IconTableModel myModel;
public:
    IconTab(Settings *settings, QWidget *parent = 0);
private slots:
    void edit(const QModelIndex& index);
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
private:
    Settings mySettings;
    QTabWidget *tabWidget;
    GpsTab *gpsTab;
    DirTab *trackTab;
    PrintTab *printTab;
    IconTab *iconTab;
public:
    SettingsDialog(const Settings& settings, QWidget *parent = 0);
    const Settings& settings() const { return mySettings; }
    static QPixmap colorIcon(const QColor& col);
private slots:
    void accept();
};

#endif // SETTINGSDIALOG_H
