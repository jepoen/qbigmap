#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QAbstractTableModel>
#include <QDialog>
#include <QItemDelegate>
#include "settings.h"

class QCheckBox;
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
    QCheckBox *eUseSrtm;
    QLineEdit *eSrtmDir;
    QPushButton *bSrtmDir;
    QLineEdit *eExportDir;
    QPushButton *bExportDir;
    QLineEdit *eCacheDir;
    QPushButton *bCacheDir;
public:
    DirTab(Settings *settings, QWidget *parent = 0);
    const QString trackDir() const;
    bool useSrtm() const;
    const QString srtmDir() const;
    const QString exportDir() const;
    const QString cacheDir() const;

private slots:
    void selectTrackDir();
    void selectSrtmDir();
    void selectExportDir();
    void selectCacheDir();
};

class PrintTab: public QWidget
{
    Q_OBJECT
private:
    Settings *mySettings;
    QSpinBox *bTileSize;
    QSpinBox *bPrintBorder;
    QSpinBox *bTrackWidth;
    QPushButton *bTrackColor;
    QSpinBox *bRouteWidth;
    QPushButton *bRouteColor;
    QColor myTrackColor;
    QColor myRouteColor;

public:
    PrintTab(Settings *settings, QWidget *parent = 0);
    int tileSize() const;
    int printBorder() const;
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
    bool setData(const QModelIndex &index, const QVariant &, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    MapIcon iconAt(int idx) const { return myIcons.at(idx); }
    void setIcon(int idx, const MapIcon& ico) { myIcons.setIco(idx, ico); }
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    MapIconList mapIconList() const { return myIcons; }
    int iconListSize() const { return myIcons.size(); }
};

class IconTab: public QWidget {
    Q_OBJECT
private:
    Settings *mySettings;
    QPushButton *myNewIco;
    QTableView *tab;
    IconTableModel myModel;
public:
    IconTab(Settings *settings, QWidget *parent = 0);
    MapIconList mapIconList() const { return myModel.mapIconList(); }
private slots:
    void add();
    void edit(const QModelIndex& index);
    void resizeTab();
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
