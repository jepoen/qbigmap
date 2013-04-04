#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QString>

class QTextBrowser;

class HelpWindow : public QDialog
{
    Q_OBJECT
private:
    QString myUrl;
    QTextBrowser *myText;

public:
    explicit HelpWindow(QWidget *parent = 0);
    ~HelpWindow();
    void setUrl(const QString& url);
signals:
    
public slots:
    
};

#endif // HELPWINDOW_H
