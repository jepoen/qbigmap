#ifndef ICONEDITDLG_H
#define ICONEDITDLG_H

#include <QDialog>
#include <QString>
#include "mapicon.h"

class QButtonGroup;
class QLineEdit;
class QComboBox;
class QLabel;
class QRadioButton;
class QToolButton;

class MapIconEditDlg : public QDialog {
    Q_OBJECT
private:
    QLineEdit *myName;
    QButtonGroup *gMenuIcon;
    QRadioButton *bMenuIntern;
    QRadioButton *bMenuFile;
    QToolButton *bSelMenuFile;
    QComboBox *cInternalMenuIcons;
    QLabel *lMenuIcon;
    QLabel *lMenuPath;
    QString myMenuIconPath;
    QButtonGroup *gMapIcon;
    QRadioButton *bMapIntern;
    QRadioButton *bMapFile;
    QToolButton *bSelMapFile;
    QComboBox *cInternalMapIcons;
    QLabel *lMapIcon;
    QLabel *lMapPath;
    QString myMapIconPath;
    void showInternalIcons();
    enum {SEL_INTERN, SEL_FILE};
public:
    MapIconEditDlg(QWidget *parent=0);
    void setIcon(const MapIcon& icon);
    MapIcon getIcon() const;

signals:

public slots:
private slots:
    void selInternalMenuIcon(int idx);
    void selInternalMenuFile();
    void selMenuIcon(int idx);
    void selInternalMapIcon(int idx);
    void selInternalMapFile();
    void selMapIcon(int idx);
    void checkAccept();
};

#endif // ICONEDITDLG_H
