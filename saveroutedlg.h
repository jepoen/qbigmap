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
    QCheckBox *eUpload;
public:
    explicit SaveRouteDlg(const QString& name, QWidget *parent = 0);
    const QString name() const;
    const QString fileName() const;
    bool isUpload() const;
signals:

private slots:
    void selectFile();
    void check();
};

#endif // SAVEROUTEDLG_H
