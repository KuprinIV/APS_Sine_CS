#include "cs_control.h"
#include <QDebug>

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

bool CS_Control::checkDeviceConnected(ushort vid, ushort pid)
{
    libusb_device **devs;
    libusb_device *cur_dev;
    int devs_num = 0;
    bool result = false;

    devs_num = (int)libusb_get_device_list(NULL, &devs);

    if(devs_num > 0)
    {
        for(int i = 0; i < devs_num; i++)
        {
            cur_dev = devs[i];

            struct libusb_device_descriptor desc;
            int res = libusb_get_device_descriptor(cur_dev, &desc);

            if(res < 0)
            {
                continue;
            }
            else
            {
                if(desc.idProduct == pid && desc.idVendor == vid)
                {
                    result = true;
                    break;
                }
            }
        }
    }
    libusb_free_device_list(devs, 1);
    return result;
}

bool CS_Control::powerControl(bool is_enabled)
{
    uint16_t wValue = 0;
    if(is_enabled)
    {
        wValue = 0x0001;
    }
    else
    {
        wValue = 0x0010;
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
   int res = libusb_control_transfer(cs_device, LIBUSB_REQUEST_TYPE_VENDOR|LIBUSB_ENDPOINT_OUT, bRequest, wValue, 0, NULL, 0, 50);
   if(res < 0)
   {
       qDebug()<<"Control transfer error: "<<libusb_error_name(res);
       return false;
   }
   return true;
}
