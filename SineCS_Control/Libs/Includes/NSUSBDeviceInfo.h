#ifndef NSUSBDEVICEINFO_H
#define NSUSBDEVICEINFO_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "libusb.h"
#include "NSUSB_Global.h"

class NSUSBDeviceInfo: public QObject
{
    Q_OBJECT

public:
    NSUSBDeviceInfo(libusb_device* device);
    ~NSUSBDeviceInfo();
    int VID;
    int PID;
    QString SerialNumber;
    QString Vendor;
    QString Product;
    QString FirmwareVersion;
    libusb_device* Device;
};

#endif // NSUSBDEVICEINFO_H
