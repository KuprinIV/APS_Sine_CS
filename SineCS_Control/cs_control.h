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
    bool registerHotplugCallback(ushort vid, ushort pid);

    // CS control commands
    bool powerControl(bool is_enabled);
    bool calibrationModeControl(bool is_enabled);
    bool saveCalibrationData(void);
    bool setRawOffset(ushort offset);
    bool setRawAmplitude(ushort ampl);
    bool setAmplitude(unsigned char ampl_0_1);

signals:
    void deviceWasConnected(void);
    void deviceWasDisconnected(void);

private:
    // members
    libusb_device_handle *cs_device = Q_NULLPTR;     //указатель на устройство
    libusb_hotplug_callback_handle hotplug_callback_handle;
    bool isLibusbInited = false;
    bool isHotplugCallbackRegistered = false;

    // functions
    bool controlTransfer(uint8_t bRequest, uint16_t wValue);
    static int hotplugEventCallback(struct libusb_context *ctx, struct libusb_device *dev,
                             libusb_hotplug_event event, void *user_data);
};

#endif // CS_CONTROL_H
