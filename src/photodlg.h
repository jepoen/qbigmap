#ifndef PHOTODLG_H
#define PHOTODLG_H

#include <QDialog>
#include <QLabel>
#include <QPixmap>
class PhotoDlg : public QDialog
{
    Q_OBJECT
private:
    QString myFileName;
    QPixmap myPixmap;
    QLabel *photoLabel;
public:
    explicit PhotoDlg(const QString& fileName, QWidget *parent = 0);

signals:

public slots:

};

#endif // PHOTODLG_H
