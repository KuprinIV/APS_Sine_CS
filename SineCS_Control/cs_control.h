#ifndef CS_CONTROL_H
#define CS_CONTROL_H

#include <QObject>
#include "libusb.h"

class CS_Control : public QObject
{
    Q_OBJECT
public:
    CS_Control();
    ~CS_Control();

    bool openDevice(ushort vid, ushort pid);
    bool closeDevice(void);
    bool checkDeviceConnected(ushort vid, ushort pid);

    // CS control commands
    bool powerControl(bool is_enabled);
    bool calibrationModeControl(bool is_enabled);
    bool saveCalibrationData(void);
    bool setRawOffset(ushort offset);
    bool setRawAmplitude(ushort ampl);
    bool setAmplitude(unsigned char ampl_0_1);

private:
    // members
    libusb_device_handle *cs_device = Q_NULLPTR;     //указатель на устройство
    bool isLibusbInited = false;

    // functions
    bool controlTransfer(uint8_t bRequest, uint16_t wValue);
};

#endif // CS_CONTROL_H
