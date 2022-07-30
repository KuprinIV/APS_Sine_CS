#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "hid_wrapper.h"

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    CalibrationDialog(HID_Wrapper* hw);
    ~CalibrationDialog();

private slots:
    void on_rawOffsetSB_valueChanged(int arg1);
    void on_rawAmplitudeSB_valueChanged(int arg1);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::CalibrationDialog *ui;
    HID_Wrapper *hid_wrapper  = Q_NULLPTR;
};

#endif // CALIBRATIONDIALOG_H
