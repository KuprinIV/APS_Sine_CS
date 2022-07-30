#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include "cs_control.h"

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    CalibrationDialog(CS_Control* csc);
    ~CalibrationDialog();

private slots:
    void on_rawOffsetSB_valueChanged(int arg1);
    void on_rawAmplitudeSB_valueChanged(int arg1);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::CalibrationDialog *ui;
    CS_Control *cs_control  = Q_NULLPTR;
};

#endif // CALIBRATIONDIALOG_H
