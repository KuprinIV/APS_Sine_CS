#ifndef NSUSB_H
#define NSUSB_H

#define NEUROSOFT_VID 0xACCA

#include <stdio.h>
#include <QString>
#include <QList>
#include <QDebug>

#include "libusb.h"

#include "NSUSB_Global.h"
#include "NSUSBDeviceInfo.h"
#include "AsyncOperation.h"

#ifdef Q_OS_MAC
typedef unsigned char byte;
#elif defined Q_OS_LINUX
typedef unsigned char byte;
#endif

class NSUSB: public QObject
{
    Q_OBJECT

protected:
    static const byte Vendor = 0xb0;
    static bool InitLibUsb();
    static void ExitLibUsb();
    static QString getStringDescriptor(libusb_device_handle* hDevice, uint8_t index);

    libusb_device* device;
    libusb_device_handle* hDevice = nullptr;
    int pid = 0;
    QString serialNumber = "";
    int errorCode = 0;
    QList<AsyncOperation*> asyncOperations;

    bool Open(libusb_device* device);

    void AddAsyncOperation(AsyncOperation* operation);
    void RemoveAsyncOperation(AsyncOperation* operation);

    void OnAsyncOperationComplete(AsyncOperation* operation);

public:
    static libusb_context* LibUsbContext;

    libusb_device* getDevice() { return device; }
    libusb_device_handle* getDeviceHandle() { return hDevice; }

    static bool SetDebugLevel(int level);

    NSUSB(int pid = 0, QString serialNumber = "");
    ~NSUSB();

    static QList<NSUSBDeviceInfo*>* Enumerate(int pid, QString serialNumber);

    bool Open();
    bool Open(NSUSBDeviceInfo* deviceInfo);
    bool isOpened();
    void Close();
    int getLastErrorCode() { return errorCode; }

    bool setInterface(int interface);
    bool ReleaseInterface(int interface);
    bool setAltSettings(int interface, int altSetting);
    bool setConfiguration(int configuration);
    bool getConfiguration(int* configuration);
    bool Unconfigure();
    QString getSerialNumber() { return serialNumber; }

    bool getBusNumber(int* num);

    bool ControlTransfer(ControlTransferDirection direction, uint16_t wValue, uint16_t wIndex, byte* data = nullptr, uint16_t count = 0, int timeout = 1000);
    bool ControlTransfer(ControlTransferDirection direction, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, byte* data = nullptr, uint16_t count = 0, int timeout = 1000);
    bool BulkTransfer(byte endPoint, byte* data, int count, int timeout = 1000);
    bool InterruptTransfer(byte endPoint, byte* data, int count, int timeout = 1000);

    ControlAsyncOperation* ControlTransferAsync(ControlTransferDirection direction, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, byte* data = nullptr, uint16_t count = 0, bool repeat = false);
    BulkAsyncOperation* BulkTransferAsync(byte endPoint, byte* data, int count, bool repeat = false);
    InterruptAsyncOperation* InterruptTransferAsync(byte endPoint, byte* data, int count, bool repeat = false);
    IsochronousAsyncOperation* IsochronousTransferAsync(byte endPoint, byte* data, int count, int packetsCount, bool repeat = false);

    MultiTransferAsyncReadingOperation* BulkMultiTransferAsyncReading(byte endPoint, int bufferLength, int bufferCount = 3);
    MultiTransferAsyncReadingOperation* InterruptMultiTransferAsyncReading(byte endPoint, int bufferLength, int bufferCount = 3);
    MultiTransferAsyncReadingOperation* IsochronousMultiTransferAsyncReading(byte endPoint, int bufferLength, int bufferCount = 3, int packetCount = 0);

    bool CancelAsyncOperation(AsyncOperation* operation);
    void CancelAllAsyncOperations();

private slots:
    void AsyncOperationFinished(AsyncOperation* operation);

signals:
    void OnControlTransferComplete(ControlAsyncOperation* operation);
    void OnBulkTransferComplete(BulkAsyncOperation* operation);
    void OnInterruptTransferComplete(InterruptAsyncOperation* operation);
    void OnIsochronousTransferComplete(IsochronousAsyncOperation* operation);

friend class AsyncOperation;
friend class MultiTransferAsyncReadingOperation;

};

#endif // NSUSB_H
