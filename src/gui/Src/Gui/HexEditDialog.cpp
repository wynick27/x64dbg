#include "HexEditDialog.h"
#include "ui_HexEditDialog.h"
#include "Configuration.h"
#include "Bridge.h"

HexEditDialog::HexEditDialog(QWidget* parent) : QDialog(parent), ui(new Ui::HexEditDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(this->size()); //fixed size
    setModal(true); //modal window

    //setup text fields
    ui->lineEditAscii->setEncoding(HexLineEdit::Encoding::Ascii);
    ui->lineEditUnicode->setEncoding(HexLineEdit::Encoding::Unicode);

    ui->chkEntireBlock->hide();

    mDataInitialized = false;

    //setup hex editor
    mHexEdit = new QHexEdit(this);
    mHexEdit->setEditFont(ConfigFont("HexEdit"));
    mHexEdit->setHorizontalSpacing(6);
    mHexEdit->setOverwriteMode(true);
    ui->scrollArea->setWidget(mHexEdit);
    mHexEdit->widget()->setFocus();
    mHexEdit->setTabOrder(ui->btnUnicode2Hex, mHexEdit->widget());
    connect(mHexEdit, SIGNAL(dataChanged()), this, SLOT(dataChangedSlot()));
    connect(mHexEdit, SIGNAL(dataEdited()), this, SLOT(dataEditedSlot()));

    connect(Config(), SIGNAL(colorsUpdated()), this, SLOT(updateStyle()));
    connect(Bridge::getBridge(), SIGNAL(repaintGui()), this, SLOT(updateStyle()));

    updateStyle();
}

HexEditDialog::~HexEditDialog()
{
    delete ui;
}

void HexEditDialog::showEntireBlock(bool show)
{
    if(show)
        ui->chkEntireBlock->show();
    else
        ui->chkEntireBlock->hide();
}

bool HexEditDialog::entireBlock()
{
    return ui->chkEntireBlock->isChecked();
}

void HexEditDialog::updateStyle()
{
    QString style = QString("QLineEdit { border-style: outset; border-width: 1px; border-color: %1; color: %1; background-color: %2 }").arg(ConfigColor("HexEditTextColor").name(), ConfigColor("HexEditBackgroundColor").name());
    ui->lineEditAscii->setStyleSheet(style);
    ui->lineEditUnicode->setStyleSheet(style);

    mHexEdit->setTextColor(ConfigColor("HexEditTextColor"));
    mHexEdit->setWildcardColor(ConfigColor("HexEditWildcardColor"));
    mHexEdit->setBackgroundColor(ConfigColor("HexEditBackgroundColor"));
    mHexEdit->setSelectionColor(ConfigColor("HexEditSelectionColor"));
}

void HexEditDialog::on_btnAscii2Hex_clicked()
{
    QByteArray data = ui->lineEditAscii->data();
    data = resizeData(data);
    ui->lineEditUnicode->setData(data);
    mHexEdit->setData(data);
}

void HexEditDialog::on_btnUnicode2Hex_clicked()
{
    QByteArray data = ui->lineEditUnicode->data();
    data = resizeData(data);
    ui->lineEditAscii->setData(data);
    mHexEdit->setData(data);
}

void HexEditDialog::on_chkKeepSize_toggled(bool checked)
{
    mHexEdit->setKeepSize(checked);
    ui->lineEditAscii->setKeepSize(checked);
    ui->lineEditUnicode->setKeepSize(checked);
}

void HexEditDialog::dataChangedSlot()
{
    // Allows initialization of the data by calling setData() on mHexEdit.
    if(!mDataInitialized)
    {
        QByteArray data = mHexEdit->data();
        ui->lineEditAscii->setData(data);
        ui->lineEditUnicode->setData(data);
        mDataInitialized = true;
    }
}

void HexEditDialog::dataEditedSlot()
{
    QByteArray data = mHexEdit->data();
    ui->lineEditAscii->setData(data);
    ui->lineEditUnicode->setData(data);
}

void HexEditDialog::on_lineEditAscii_dataEdited()
{
    on_btnAscii2Hex_clicked();
}

void HexEditDialog::on_lineEditUnicode_dataEdited()
{
    on_btnUnicode2Hex_clicked();
}

QByteArray HexEditDialog::resizeData(QByteArray & data)
{
    // truncate or pad the data
    if(mHexEdit->keepSize())
    {
        int dataSize = mHexEdit->data().size();
        if(dataSize < data.size())
            data.resize(dataSize);
        else if(dataSize > data.size())
            data.append(QByteArray(dataSize - data.size(), 0));
    }

    return data;
}
