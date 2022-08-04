#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include <QMessageBox>
#include <QPushButton>

CalibrationDialog::CalibrationDialog(CS_Control *csc) :
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    cs_control = csc;

    // init start values
    bool res1 = cs_control->setRawOffset(ui->rawOffsetSB->value());
    bool res2 = cs_control->setRawAmplitude(ui->rawAmplitudeSB->value());
    if(!res1 || !res2)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't set default values"));
        // disable controls
        ui->rawOffsetSB->setEnabled(false);
        ui->rawAmplitudeSB->setEnabled(false);
        ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    }
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
    if(res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Calibration data was saved successfully"));
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't save calibration data"));
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

