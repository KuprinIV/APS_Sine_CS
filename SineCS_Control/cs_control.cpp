#include "cs_control.h"
#include <QDebug>

// define bRequest values for current source control
#define CS_CONTROL_POWER_CTRL 				0x31
#define CS_CONTROL_CALIB_MODE_CTRL			0x32
#define CS_CONTROL_SAVE_CALIB_DATA			0x33
#define CS_CONTROL_SET_RAW_OFFSET			0x34
#define CS_CONTROL_SET_RAW_AMPL				0x35
#define CS_CONTROL_SET_AMPL					0x36

CS_Control::CS_Control()
{
    if(libusb_init(NULL) >= 0)
    {
       isLibusbInited = true;
       qDebug()<<"Libusb is inited";
    }
}

CS_Control::~CS_Control()
{
    if(isHotplugCallbackRegistered)
    {
        isHotplugCallbackRegistered = false;
        libusb_hotplug_deregister_callback(NULL, hotplug_callback_handle);
    }
    this->disconnect();
    libusb_exit(NULL);
}

bool CS_Control::openDevice(ushort vid, ushort pid)
{
    int res;
    if(isLibusbInited)
    {
        cs_device = libusb_open_device_with_vid_pid(NULL, vid, pid);
        if(cs_device == NULL)
        {
            qDebug()<<"Can't get device handle";
            return false;
        }
        res = libusb_claim_interface(cs_device, 0);
        if(res != LIBUSB_SUCCESS)
        {
            libusb_close(cs_device);
            qDebug()<<"Can't claim interface: "<<libusb_error_name(res);
            return false;
        }
    }
    else
    {
        qDebug()<<"Libusb isn't inited";
        return false;
    }
    return true;
}

bool CS_Control::closeDevice(void)
{
    if(isLibusbInited)
    {
        libusb_release_interface(cs_device, 0);
        libusb_close(cs_device);
        cs_device = NULL;
    }
    else
    {
        qDebug()<<"Libusb isn't inited";
        return false;
    }
    return true;
}

bool CS_Control::registerHotplugCallback(ushort vid, ushort pid)
{
    // register hotplug events callback
    int res = libusb_hotplug_register_callback(NULL, (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                            LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT), (libusb_hotplug_flag)0, vid, pid,
                                            LIBUSB_HOTPLUG_MATCH_ANY, &CS_Control::hotplugEventCallback, this,
                                            &hotplug_callback_handle);
    if(res != LIBUSB_SUCCESS)
    {
        qDebug()<<"Can't register hotplug callback: "<<libusb_error_name(res);
        return false;
    }
    isHotplugCallbackRegistered = true;
    return true;
}

bool CS_Control::powerControl(bool is_enabled)
{
    uint16_t wValue = 0;
    if(is_enabled)
    {
        wValue = 0x0001;
    }
    return controlTransfer(CS_CONTROL_POWER_CTRL, wValue);
}

bool CS_Control::calibrationModeControl(bool is_enabled)
{
    uint16_t wValue = 0;
    if(is_enabled)
    {
        wValue = 0x0001;
    }
    return controlTransfer(CS_CONTROL_CALIB_MODE_CTRL, wValue);
}

bool CS_Control::saveCalibrationData(void)
{
    return controlTransfer(CS_CONTROL_SAVE_CALIB_DATA, 0);
}

bool CS_Control::setRawOffset(ushort offset)
{
    return controlTransfer(CS_CONTROL_SET_RAW_OFFSET, offset);
}

bool CS_Control::setRawAmplitude(ushort ampl)
{
    return controlTransfer(CS_CONTROL_SET_RAW_AMPL, ampl);
}

bool CS_Control::setAmplitude(unsigned char ampl_0_1)
{
    return controlTransfer(CS_CONTROL_SET_AMPL, (uint16_t)ampl_0_1);
}

bool CS_Control::controlTransfer(uint8_t bRequest, uint16_t wValue)
{
   int res = libusb_control_transfer(cs_device, LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT, bRequest, wValue, 0, NULL, 0, 1000);
   if(res < 0)
   {
       qDebug()<<"Control transfer error: "<<libusb_error_name(res);
       return false;
   }
   return true;
}

int CS_Control::hotplugEventCallback(struct libusb_context *ctx, struct libusb_device *dev,
                         libusb_hotplug_event event, void *user_data)
{
    Q_UNUSED(ctx);
    Q_UNUSED(dev);

    CS_Control* cs_control_instance = reinterpret_cast<CS_Control*>(user_data);
    if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event)
    {
        emit cs_control_instance->deviceWasConnected();
    }
    else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event)
    {
        emit cs_control_instance->deviceWasDisconnected();
    }
    return 0;
}
