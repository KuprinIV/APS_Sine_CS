#include "readthread.h"

ReadThread::ReadThread(hid_device *handle)
{
    dev_handle = handle;
}

ReadThread::~ReadThread()
{
    disconnect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    //delete device;
    hid_close(dev_handle);
    hid_exit();
}

void ReadThread::run()
{
   unsigned char inData[3] = {0};
   /* ... here is the expensive or blocking operation ... */
   hid_set_nonblocking(dev_handle, 0);
   while(isRunning)
   {
       int res = hid_read(dev_handle, inData, sizeof(inData));
       if(res > 0)
       {
           inputData.clear();
           inputData.append((const char*)inData, res);
           emit dataReceived(&inputData);

           memset(inData, 0, sizeof(inData));
       }
   }
   quit();
}

void ReadThread::stopThread()
{
    isRunning = false;
}

void ReadThread::startThread()
{
    isRunning = true;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    start();
}
