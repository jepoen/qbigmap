#include <QtWidgets>
#include "gpxsplitdialog.h"

GpxSplitDialog::GpxSplitDialog(const QString &fileName, const GpxPoint &point):
    myFileName(fileName), myPoint(point)
{
    setWindowTitle(tr("Split GPX Track/Route"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGroupBox *buttonGroup = new QGroupBox();
    QVBoxLayout *layoutGroup = new QVBoxLayout();
    bPreserve1 = new QRadioButton(tr("Preserve first part"));
    bPreserve2 = new QRadioButton(tr("Preserve second part"));
    bPreserve1->setChecked(true);
    QString base = baseFileName();
    bSave1 = new QCheckBox(tr("Save first part %1-1.gpx").arg(base));
    bSave1->setChecked(true);
    bSave2 = new QCheckBox(tr("Save second part %1-2.gpx").arg(base));
    bSave2->setChecked(true);
    layoutGroup->addWidget(bPreserve1);
    layoutGroup->addWidget(bPreserve2);
    layoutGroup->addWidget(bSave1);
    layoutGroup->addWidget(bSave2);
    buttonGroup->setLayout(layoutGroup);
    mainLayout->addWidget(buttonGroup);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(box);
    setLayout(mainLayout);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

bool GpxSplitDialog::isPreservedFirst() const {
    return bPreserve1->isChecked();
}

bool GpxSplitDialog::isSavedFirst() const {
    return bSave1->isChecked();
}

bool GpxSplitDialog::isSavedSecond() const {
    return bSave2->isChecked();
}

QString GpxSplitDialog::baseFileName() const {
    return QFileInfo(myFileName).completeBaseName();
}

QString GpxSplitDialog::splitFileName(int part) const {
    QDir dir = QFileInfo(myFileName).dir();
    QString splitName;
    splitName = QString("%1-%2.gpx").arg(baseFileName()).arg(part==0?1:2);
    return dir.absoluteFilePath(splitName);
}
