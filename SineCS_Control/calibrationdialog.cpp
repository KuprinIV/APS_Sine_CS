#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>

CalibrationDialog::CalibrationDialog(CS_Control *csc) :
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    cs_control = csc;
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_rawOffsetSB_valueChanged(int arg1)
{
    bool res = cs_control->setRawOffset(arg1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_rawAmplitudeSB_valueChanged(int arg1)
{
    bool res = cs_control->setRawAmplitude(arg1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_buttonBox_accepted()
{
    bool res = cs_control->saveCalibrationData();
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }

    res = cs_control->calibrationModeControl(false);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}


void CalibrationDialog::on_buttonBox_rejected()
{
    bool res = cs_control->calibrationModeControl(false);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}

