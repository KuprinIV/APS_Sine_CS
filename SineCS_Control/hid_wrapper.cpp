#include "hid_wrapper.h"

HID_Wrapper::HID_Wrapper()
{

}

HID_Wrapper::~HID_Wrapper()
{
    hidDevsList.clear();
    closeDevice();
    hid_exit();
}

bool HID_Wrapper::openDevice(ushort vid, ushort pid)
{
    device_handle = hid_open(vid, pid, NULL);
    if(device_handle != NULL)
    {
        struct hid_device_info *device_data = hid_enumerate(vid, pid);
        serialNumber = QString("%1").arg(device_data->serial_number);
        hid_free_enumeration(device_data); // release enumeration resources

        inputDataThread = new ReadThread(device_handle);
        inputDataThread->startThread();
        return true;
    }
    return false;
}

void HID_Wrapper::closeDevice()
{
    if(device_handle != Q_NULLPTR)
    {
        inputDataThread->stopThread();
        device_handle = Q_NULLPTR;
    }
}

QList<QPair<ushort, ushort>>* HID_Wrapper::hidEnumerate(void)
{
    struct hid_device_info *devices = NULL;
    struct hid_device_info *cur_dev;
    hidDevsList.clear(); // clear previous devices list

    devices = hid_enumerate(0x0, 0x0);
    cur_dev = devices;
    while (cur_dev)
    {
      hidDevsList.append(QPair<ushort, ushort>(cur_dev->vendor_id,cur_dev->product_id));
      cur_dev = cur_dev->next;
    }

    hid_free_enumeration(devices); // release enumeration resources
    return &hidDevsList;
}

bool HID_Wrapper::checkDeviceConnected(ushort vid, ushort pid)
{
    bool result = false;

    hidEnumerate();
    for(int i = 0; i < hidDevsList.length(); i++)
    {
        if(hidDevsList.at(i).first == vid && hidDevsList.at(i).second == pid)
        {
            result = true;
            break;
        }
    }
    return result;
}

QString HID_Wrapper::getSerialNumberString(void)
{
    if(serialNumber.isEmpty()) return "";
    return serialNumber;
}

ReadThread* HID_Wrapper::getInputThread()
{
    return inputDataThread;
}

bool HID_Wrapper::sendReport(uint8_t* data, uint8_t length)
{
    int res = -1;

    // limit out report length
    if(length > OUT_REPORT_LENGTH)
    {
        length = OUT_REPORT_LENGTH;
    }

    if(device_handle != NULL && device_handle != Q_NULLPTR)
    {
        res = hid_write(device_handle, data, length);
        if(res <= 0)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool HID_Wrapper::powerControl(bool is_enabled)
{
    uint8_t cmd_report[4] = {0x01, 0x00, 0x00, 0x00};

    if(is_enabled)
    {
        cmd_report[1] = 0x01;
    }
    else
    {
        cmd_report[1] = 0x10;
    }

    return sendReport(cmd_report, 4);
}

bool HID_Wrapper::calibrationModeControl(bool is_enabled)
{
    uint8_t cmd_report[4] = {0x01, 0x00, 0x00, 0x00};

    if(is_enabled)
    {
        cmd_report[2] = 0x01;
    }

    return sendReport(cmd_report, 4);
}

bool HID_Wrapper::saveCalibrationData(void)
{
    uint8_t cmd_report[4] = {0x01, 0x00, 0x01, 0x01};
    return sendReport(cmd_report, 4);
}

bool HID_Wrapper::setRawOffset(ushort offset)
{
    uint8_t offset_report[3] = {0x02, 0x00, 0x00};

    offset_report[1] = (uint8_t)(offset>>8);
    offset_report[2] = (uint8_t)(offset & 0xFF);

    return sendReport(offset_report, 3);
}

bool HID_Wrapper::setRawAmplitude(ushort ampl)
{
    uint8_t raw_ampl_report[3] = {0x03, 0x00, 0x00};

    raw_ampl_report[1] = (uint8_t)(ampl>>8);
    raw_ampl_report[2] = (uint8_t)(ampl & 0xFF);

    return sendReport(raw_ampl_report, 3);
}

bool HID_Wrapper::setAmplitude(unsigned char ampl_0_1)
{
    uint8_t ampl_report[3] = {0x04, ampl_0_1};
    return sendReport(ampl_report, 2);
}
