#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

CalibrationDialog::CalibrationDialog(HID_Wrapper *hw) :
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    hid_wrapper = hw;
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_rawOffsetSB_valueChanged(int arg1)
{
    bool res = hid_wrapper->setRawOffset(arg1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_rawAmplitudeSB_valueChanged(int arg1)
{
    bool res = hid_wrapper->setRawAmplitude(arg1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_buttonBox_accepted()
{
    bool res = hid_wrapper->saveCalibrationData();
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }

    res = hid_wrapper->calibrationModeControl(false);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_buttonBox_rejected()
{
    bool res = hid_wrapper->calibrationModeControl(false);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}

