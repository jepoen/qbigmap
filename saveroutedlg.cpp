#include <QtGui>
#include "saveroutedlg.h"

SaveRouteDlg::SaveRouteDlg(int typ, const QString& fileName, const QString &name, QWidget *parent) :
    QDialog(parent), myTyp(typ)
{
    QString titles[] = { tr("Save route"), tr("Save track") };
    QString names[]  = { tr("&Route name:"), tr("&Track name") };
    setWindowTitle(titles[typ]);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QGridLayout *controlLayout = new QGridLayout();
    QLabel *lName = new QLabel(names[typ]);
    controlLayout->addWidget(lName, 0, 0);
    eName = new QLineEdit(name);
    lName->setBuddy(eName);
    controlLayout->addWidget(eName, 0, 1);
    QLabel *lFileName = new QLabel(tr("&File name:"));
    controlLayout->addWidget(lFileName, 1, 0);
    QString fn = fileName;
    if (fileName.isEmpty()) fn = QDir::currentPath().append("/route.gpx");
    eFileName = new QLabel(fn);
    eFileName->setMinimumWidth(300);
    eFileName->setAutoFillBackground(true);
    eFileName->setPalette(Qt::yellow);
    eFileName->setMargin(2);
    controlLayout->addWidget(eFileName, 1, 1);
    bFileName = new QToolButton();
    lFileName->setBuddy(bFileName);
    QAction *selFileAction = new QAction(QIcon(":/icons/disk.png"), tr("Select File"), this);
    bFileName->setDefaultAction(selFileAction);
    controlLayout->addWidget(bFileName, 1, 2);
    eWaypoints = new QCheckBox(tr("Add &Waypoints"));
    controlLayout->addWidget(eWaypoints, 2, 0, 1, 2);
    eUpload = new QCheckBox(tr("&Upload to GPS device"));
    controlLayout->addWidget(eUpload, 3, 0, 1, 2);

    mainLayout->addLayout(controlLayout);
    QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    mainLayout->addWidget(bbox);
    setLayout(mainLayout);

    connect(selFileAction, SIGNAL(triggered()), this, SLOT(selectFile()));
    connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(bbox, SIGNAL(accepted()), this, SLOT(check()));
}

void SaveRouteDlg::selectFile() {
    QString fn = QFileDialog::getSaveFileName(this, tr("Select Route File"), QDir(fileName()).absolutePath(),
                                                    tr("GPX file (*.gpx)"));
    if (!fn.isEmpty()) {
        if (QFileInfo(fn).suffix() != "gpx") fn += ".gpx";
        eFileName->setText(fn);
    }
}

void SaveRouteDlg::check() {
    QString fn = fileName();
    int pos = fn.lastIndexOf('/');
    QDir path(fn.left(pos));
    fn = fn.mid(pos+1);
    if (fn.size() == 0) return;
    if (path.exists()) {
        qDebug()<<"ok";
        emit accept();
    }
}

const QString SaveRouteDlg::name() const {
    return eName->text();
}

const QString SaveRouteDlg::fileName() const {
    QString fn = eFileName->text();
    int pos = fn.lastIndexOf('.');
    if (pos < 0) {
        fn += ".gpx";
    }
    return fn;
}

void SaveRouteDlg::setWaypoints(bool val) {
    eWaypoints->setChecked(val);
}

bool SaveRouteDlg::isWaywaypoints() const {
    return eWaypoints->isChecked();
}

bool SaveRouteDlg::isUpload() const {
    return eUpload->isChecked();
}
