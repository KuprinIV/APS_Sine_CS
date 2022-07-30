#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //register device attach/detach events  {88BAE032-5A81-49f0-BC3D-A4FF138216D6}
    GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };//HID class GUID
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    NotificationFilter.dbcc_classguid = InterfaceClassGuid;

    register_handle = RegisterDeviceNotification((void*)(this->winId()),&NotificationFilter,0);
    hid_wrapper = new HID_Wrapper();

    qApp->installEventFilter(this);

    amplitude = ui->amplitudeSpinBox->value();
}

MainWindow::~MainWindow()
{
    UnregisterDeviceNotification(register_handle);

    isUSBConnected = false;

    this->disconnect();

    hid_wrapper->closeDevice();

    if(hid_wrapper != Q_NULLPTR)
        delete hid_wrapper;

    delete ui;
}

void MainWindow::on_usbConnectBtn_clicked()
{
    if(!isUSBConnected)
    {
        isUSBConnected = hid_wrapper->openDevice(USB_VID, USB_PID);
        if(isUSBConnected)
        {
            connect(hid_wrapper->getInputThread(), &ReadThread::dataReceived, this, &MainWindow::onInputDataReceived, Qt::DirectConnection);
            // set initial amplitude value
            on_amplitudeSpinBox_valueChanged(amplitude);
            // update UI
            ui->usbConnectBtn->setText(tr("Disconnect"));
            ui->csControlGB->setEnabled(true);
        }
    }
    else
    {
        isUSBConnected = false;
        disconnect(hid_wrapper->getInputThread(), &ReadThread::dataReceived, this, &MainWindow::onInputDataReceived);
        hid_wrapper->closeDevice();
        // update UI
        ui->usbConnectBtn->setText(tr("Connect"));
        ui->amplitudeGB->setEnabled(false);
        ui->csControlGB->setEnabled(false);
    }
}


void MainWindow::on_powerCtrlCB_clicked(bool checked)
{
    bool res = hid_wrapper->powerControl(checked);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
    // update UI
    ui->amplitudeGB->setEnabled(checked);
    ui->calibrationBtn->setEnabled(checked);
}

void MainWindow::on_peakRB_clicked(bool checked)
{
    if(checked)
    {
        isRMS = false;
        ui->rmsRB->setChecked(false);
        ui->amplitudeSpinBox->setMaximum(7);
        on_amplitudeSpinBox_valueChanged(amplitude);
    }
}

void MainWindow::on_rmsRB_clicked(bool checked)
{
    if(checked)
    {
        isRMS = true;
        ui->peakRB->setChecked(false);
        ui->amplitudeSpinBox->setMaximum(5);
        on_amplitudeSpinBox_valueChanged(amplitude);
    }
}

void MainWindow::on_amplitudeSpinBox_valueChanged(double arg1)
{
    double amplitude0A1 = 10*arg1;
    bool res;

    amplitude = arg1;

    if(isRMS)
    {
        amplitude0A1 *= 1.4142;
    }

    res = hid_wrapper->setAmplitude((uint8_t)amplitude0A1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}

void MainWindow::on_calibrationBtn_clicked()
{
    hid_wrapper->calibrationModeControl(true);
    CalibrationDialog* cd = new CalibrationDialog(hid_wrapper);
    cd->setAttribute(Qt::WA_DeleteOnClose);
    cd->exec();
}

void MainWindow::onInputDataReceived(QByteArray* input_data)
{
    // clear report data array
    input_data->clear();
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    Q_UNUSED(result);
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    bool usbHidState;

    if(msg->message == WM_DEVICECHANGE)
    {
        if(msg->wParam == 0x8000 || msg->wParam == 0x8004) //if USB device attached of detached
        {
            if(hid_wrapper != Q_NULLPTR)
            {
                // check USB and BT keyboard connection status
                usbHidState = hid_wrapper->checkDeviceConnected(USB_VID, USB_PID);

                // if USB connection status changed
                if(isUSBConnected != usbHidState)
                {
                    on_usbConnectBtn_clicked();
                }
            }
        }
    }
    return false;
}
