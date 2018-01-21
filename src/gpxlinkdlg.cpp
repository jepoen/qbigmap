#include <QtWidgets>
#include "gpxlinkdlg.h"

GpxLinkDlg::GpxLinkDlg(const QString &url, const QString &text, const QString &mime, QWidget *parent):
    QDialog(parent)
{
    definedMimeTypes<<"image/jpeg"<<"image/png"<<"text/plain";
    setWindowTitle(tr("Edit GPX Link"));
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QFormLayout *controlLayout = new QFormLayout();
    eUrl = new QLineEdit(url);
    controlLayout->addRow(tr("&URL:"), eUrl);
    eText = new QLineEdit(text);
    controlLayout->addRow(tr("&Text:"), eText);
    eMime = new QComboBox();
    int index = definedMimeTypes.indexOf(mime);
    if (index < 0) {
        eMime->addItem(mime);
        index = 0;
    }
    eMime->addItem("image/jpeg");
    eMime->addItem("image/png");
    eMime->addItem("text/html");
    eMime->addItem("text/plain");
    eMime->setEditable(true);
    eMime->setCurrentIndex(index);
    controlLayout->addRow(tr("&MIME type:"), eMime);
    mainLayout->addLayout(controlLayout);
    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal);
    connect(box, SIGNAL(accepted()), this, SLOT(accept()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(box);
    setLayout(mainLayout);
}

QString GpxLinkDlg::text() const {
    return eText->text().trimmed();
}

QString GpxLinkDlg::url() const {
    return eUrl->text().trimmed();
}

QString GpxLinkDlg::mime() const {
    qDebug()<<"Mime: "<<eMime->currentText()<<" Index "<<eMime->currentIndex();
    return eMime->currentText().trimmed();
}
