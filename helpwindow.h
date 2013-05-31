#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QString>

class QTextBrowser;
class QToolButton;

class HelpWindow : public QDialog
{
    Q_OBJECT
private:
    QString myUrl;
    QToolButton *bHome;
    QToolButton *bBack;
    QToolButton *bForward;
    QToolButton *bClose;
    QTextBrowser *myText;

    QToolButton *createToolButton(const QString& ico, const QString& toolTip);
public:
    explicit HelpWindow(QWidget *parent = 0);
    ~HelpWindow();
    void setUrl(const QString& url);
signals:
    
public slots:
    
};

#endif // HELPWINDOW_H
