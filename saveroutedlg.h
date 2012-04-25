#ifndef SAVEROUTEDLG_H
#define SAVEROUTEDLG_H

#include <QDialog>
class QCheckBox;
class QLineEdit;
class QLabel;
class QToolButton;

class SaveRouteDlg : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *eName;
    QLabel *eFileName;
    QToolButton *bFileName;
    QCheckBox *eWaypoints;
    QCheckBox *eUpload;
public:
    explicit SaveRouteDlg(const QString& fileName, const QString& name, QWidget *parent = 0);
    const QString name() const;
    const QString fileName() const;
    bool isWaywaypoints() const;
    void setWaypoints(bool val);
    bool isUpload() const;
signals:

private slots:
    void selectFile();
    void check();
};

#endif // SAVEROUTEDLG_H
