#ifndef GPXLINKDLG_H
#define GPXLINKDLG_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QComboBox;
class QLineEdit;

class GpxLinkDlg : public QDialog
{
private:
    QLineEdit *eUrl;
    QLineEdit *eText;
    QComboBox *eMime;
    QStringList definedMimeTypes;
public:
    GpxLinkDlg(const QString& url = QString(), const QString& text = QString(), const QString& mime = QString(), QWidget *parent = 0);
    QString url() const;
    QString text() const;
    QString mime() const;
};

#endif // GPXLINKDLG_H
