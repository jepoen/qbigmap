#ifndef PROFILEVIEW_H
#define PROFILEVIEW_H

#include <QGraphicsView>
#include "profilescene.h"

class ProfileView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ProfileView(QGraphicsScene *scene, QWidget *parent = 0);

signals:

public slots:

};

#endif // PROFILEVIEW_H
