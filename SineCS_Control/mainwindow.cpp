#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cs_control = new CS_Control();
    // if hotplug supports, add USB connect/disconnect events handling
    if(cs_control->registerHotplugCallback(USB_VID, USB_PID))
    {
        connect(cs_control, &CS_Control::deviceWasConnected, this, &MainWindow::onDeviceConnected);
        connect(cs_control, &CS_Control::deviceWasDisconnected, this, &MainWindow::onDeviceDisconnected);
    }

    amplitude = ui->amplitudeSpinBox->value();
}

MainWindow::~MainWindow()
{
    this->disconnect();

    if(cs_control != Q_NULLPTR)
    {
        if(isUSBConnected)
        {
            isUSBConnected = false;
            cs_control->closeDevice();
        }
        delete cs_control;
    }

    delete ui;
}

void MainWindow::on_usbConnectBtn_clicked()
{
    if(!isUSBConnected)
    {
        isUSBConnected = cs_control->openDevice(USB_VID, USB_PID);
        if(isUSBConnected)
        {
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
        cs_control->closeDevice();
        // update UI
        ui->usbConnectBtn->setText(tr("Connect"));
        ui->amplitudeGB->setEnabled(false);
        ui->csControlGB->setEnabled(false);
    }
}


void MainWindow::on_powerCtrlCB_clicked(bool checked)
{
    bool res = cs_control->powerControl(checked);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
        return;
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

    res = cs_control->setAmplitude((uint8_t)amplitude0A1);
    if(!res)
    {
        QMessageBox::warning(this, tr("Error"), tr("Can't send data"));
    }
}

void MainWindow::on_calibrationBtn_clicked()
{
    cs_control->calibrationModeControl(true);
    CalibrationDialog* cd = new CalibrationDialog(cs_control);
    cd->setAttribute(Qt::WA_DeleteOnClose);
    cd->exec();
}

void MainWindow::onDeviceConnected(void)
{
    isUSBConnected = cs_control->openDevice(USB_VID, USB_PID);
    if(isUSBConnected)
    {
        // set initial amplitude value
        on_amplitudeSpinBox_valueChanged(amplitude);
        // update UI
        ui->usbConnectBtn->setText(tr("Disconnect"));
        ui->csControlGB->setEnabled(true);
    }
}

void MainWindow::onDeviceDisconnected(void)
{
    isUSBConnected = false;
    cs_control->closeDevice();
    // update UI
    ui->usbConnectBtn->setText(tr("Connect"));
    ui->amplitudeGB->setEnabled(false);
    ui->csControlGB->setEnabled(false);
}
