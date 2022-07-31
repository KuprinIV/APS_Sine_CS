#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cs_control.h"
#include "calibrationdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_usbConnectBtn_clicked();
    void on_powerCtrlCB_clicked(bool checked);
    void on_peakRB_clicked(bool checked);
    void on_rmsRB_clicked(bool checked);
    void on_amplitudeSpinBox_valueChanged(double arg1);
    void on_calibrationBtn_clicked();
    void onDeviceConnected(void);
    void onDeviceDisconnected(void);

private:
    Ui::MainWindow *ui;
    CS_Control *cs_control = Q_NULLPTR;

    bool isUSBConnected = false;
    bool isRMS = false;
    double amplitude = 0;
    short USB_PID = 0x5753;
    short USB_VID = 0x0483;
};
#endif // MAINWINDOW_H
