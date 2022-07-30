#ifndef READTHREAD_H
#define READTHREAD_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <string.h>
#include "hidapi.h"

class ReadThread: public QThread
{
    Q_OBJECT

public:
    ReadThread(hid_device* handle);
    ~ReadThread();

    void stopThread();
    void startThread();
    void run() Q_DECL_OVERRIDE;

signals:
    void dataReceived(QByteArray* input_data);

private:
    hid_device* dev_handle;
    QByteArray inputData;
    bool isRunning;
};

#endif // READTHREAD_H
