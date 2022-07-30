#ifndef HID_WRAPPER_H
#define HID_WRAPPER_H

#include <QObject>
#include "hidapi.h"
#include "readthread.h"

#define OUT_REPORT_LENGTH 4

class HID_Wrapper : public QObject
{
    Q_OBJECT

public:
    HID_Wrapper();
    ~HID_Wrapper();

    // common interface functions
    bool openDevice(ushort vid, ushort pid);
    void closeDevice();
    QList<QPair<ushort, ushort>>* hidEnumerate(void);
    bool checkDeviceConnected(ushort vid, ushort pid);
    QString getSerialNumberString(void);
    ReadThread* getInputThread();

    // CS control commands
    bool powerControl(bool is_enabled);
    bool calibrationModeControl(bool is_enabled);
    bool saveCalibrationData(void);
    bool setRawOffset(ushort offset);
    bool setRawAmplitude(ushort ampl);
    bool setAmplitude(unsigned char ampl_0_1);

private:
    bool sendReport(uint8_t* data, uint8_t length);

    hid_device *device_handle = Q_NULLPTR;
    ReadThread* inputDataThread;
    QList<QPair<ushort, ushort>> hidDevsList;
    QString serialNumber;
};

#endif // HID_WRAPPER_H
