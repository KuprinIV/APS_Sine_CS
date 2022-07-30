#ifndef ASYNCOPERATION_H
#define ASYNCOPERATION_H

#include <QObject>
#include <qthread.h>
#include <libusb.h>
#include "NSUSB_Global.h"

#ifdef Q_OS_LINUX
typedef unsigned char byte;
#endif

#ifdef Q_OS_MAC
typedef unsigned char byte;
#endif

enum ControlTransferDirection
{
    In,     // device -- >host
    Out     // host --> device
};

class NSUSB;
class AsyncOperationThread;

enum AsyncOperationType
{
    Control,
    Bulk,
    Interrupt,
    Isochronous
};

class AsyncOperation: public QObject
{
    Q_OBJECT

protected:
    NSUSB* usb = NULL;
    byte endPoint = 0;
    AsyncOperationType type = Control;
    byte* data = NULL;
    int length = 0;
    int actualLength = 0;
    bool repeat = false;
    bool submitted = false;
    bool cancelled = false;
    int completed = 0;
    int errorCode = 0;

    AsyncOperationThread *thread = nullptr;
    libusb_device_handle *handle = nullptr;
    libusb_transfer *transfer = nullptr;

    virtual void ProcessData();
    void SubmitTransfer();
    void WaitForCompleted();

    void OperationComplete();
    int TransferStatusToErrorCode(libusb_transfer_status status);
    void Finished();

#ifdef Q_OS_WIN
    static void __stdcall AsyncCallback(libusb_transfer* transfer);
#else
    static void AsyncCallback(libusb_transfer* transfer);
#endif

public:

    AsyncOperation(NSUSB* usb, byte endPoint, AsyncOperationType type, byte* data, int length, bool repeat = false);
    virtual ~AsyncOperation();

    virtual void Start();
    virtual void Cancel();

    byte EndPoint() { return endPoint; }
    AsyncOperationType Type() { return type; }
    byte* Data() { return data; }
    int Length() { return length; }
    int ActualLength() { return actualLength; }
    int ErrorCode() { return errorCode; }
    bool isRepeat() { return repeat; }
    bool isCancelled() { return cancelled; }

signals:
    void onFinished(AsyncOperation* operation);

    friend class AsyncOperationThread;
    friend class MultiTransferAsyncReadingOperation;
};

class AsyncOperationThread : public QThread
{
    Q_OBJECT

protected:
    AsyncOperation* operation;
    void run() Q_DECL_OVERRIDE;

public:
    AsyncOperationThread(AsyncOperation* operation);
};

class ControlAsyncOperation: public AsyncOperation
{
protected:
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    byte* controlData = nullptr;
    void CopyInData();

public:
    ControlTransferDirection Direction;
    ControlAsyncOperation(NSUSB* usb, ControlTransferDirection direction, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, byte* data = NULL, uint16_t length = 0, bool repeat = false);
    virtual ~ControlAsyncOperation();

    friend class AsyncOperation;
};

class BulkAsyncOperation: public AsyncOperation
{
public:
    BulkAsyncOperation(NSUSB* usb, byte endPoint, byte* data, int length, bool repeat = false);
};

class InterruptAsyncOperation: public AsyncOperation
{
public:
    InterruptAsyncOperation(NSUSB* usb, byte endPoint, byte* data, int length, bool repeat = false);
};

class IsochronousAsyncOperation: public AsyncOperation
{
public:
    IsochronousAsyncOperation(NSUSB* usb, byte endPoint, byte* data, int length, int packetsCount, bool repeat = false);
};

class MultiTransferAsyncReadingOperation: public AsyncOperation
{
protected:
    QList<byte*> buffers;
    QList<AsyncOperation*> operations;
    virtual void ProcessData();
    bool HasSubmittedTransfers();

public:
    virtual void Start();
    virtual void Cancel();

    MultiTransferAsyncReadingOperation(NSUSB* usb, byte endPoint, AsyncOperationType type, int length, int transferCount, int packetCount = 0);
    virtual ~MultiTransferAsyncReadingOperation();

    byte* getBuffer(AsyncOperation *op) const;

};

#endif // ASYNCOPERATION_H
