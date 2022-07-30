#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <dbt.h>

#include "hid_wrapper.h"
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
    void onInputDataReceived(QByteArray* input_data);

private:
    Ui::MainWindow *ui;
    HID_Wrapper *hid_wrapper  = Q_NULLPTR;
    HDEVNOTIFY register_handle;

    bool isUSBConnected = false;
    bool isRMS = false;
    double amplitude = 0;
    short USB_PID = 0x5752;
    short USB_VID = 0x0483;

protected:
   virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
};
#endif // MAINWINDOW_H
