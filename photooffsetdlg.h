#ifndef PHOTOOFFSETDLG_H
#define PHOTOOFFSETDLG_H

#include <QDialog>
#include <QDateTime>

class Model;
class QLabel;
class QSpinBox;

class PhotoOffsetDlg : public QDialog
{
    Q_OBJECT
private:
    static const int MAXOFFSET = 5*24*3600;
    Model *myModel;
    QDateTime myTimestamp;
    int myOffset;
    int myTimeDiff;

    QSpinBox *eOffset;
    QLabel *eTimeDiff;

    void showTrackPos();
public:

    explicit PhotoOffsetDlg(Model *model, const QDateTime& timestamp, int offset, QWidget *parent = 0);
    int offset() const { return myOffset; }
signals:
    
private slots:
    void setOffset(int val);
};

#endif // PHOTOOFFSETDLG_H
