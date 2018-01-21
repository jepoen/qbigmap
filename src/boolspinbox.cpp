#include "boolspinbox.h"
#include <QtGui>

BoolSpinBox::BoolSpinBox(QWidget *parent) :
    QComboBox(parent)
{
    addItem(QIcon(":/icons/delete.png"), QString(), false);
    addItem(QIcon(":/icons/add.png"), QString(), true);
}
