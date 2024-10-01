#include "../include/CCSerialPort.h"

CCSerialPort::CCSerialPort()
{
    this->COM = new CSerialPort();
    PortsList = CSerialPortInfo::availablePortInfos();
}

unsigned int CCSerialPort::GetPortSize()
{
    return (int)PortsList.size();
}

SerialPortInfo * CCSerialPort::GetProtInfoAll()
{
    int d = this->GetPortSize();
    serialPortInfo = new SerialPortInfo[d];
    for (int i = 0; i < this->GetPortSize(); ++i)
    {
        serialPortInfo[i] = PortsList[i];
        printf("%d - %s\n", i, serialPortInfo[i].portName);
    }
    return serialPortInfo;
}

void CCSerialPort::Initialize(const char *portName, int baudRate, itas109::Parity parity, itas109::DataBits dataBits,
                              itas109::StopBits stopbits, itas109::FlowControl flowControl,
                              unsigned int readBufferSize)
{
    this->COM->init(portName, baudRate, parity, dataBits, stopbits,flowControl,readBufferSize);
}

void CCSerialPort::SetReadIntervalTime(unsigned int ms)
{
    this->COM->setReadIntervalTimeout(ms);
}

bool CCSerialPort::Open()
{
    auto f = this->COM->open();
    this->COM->connectReadEvent(this);
    return f;
}

bool CCSerialPort::IsOpen()
{
    return this->COM->isOpen();
}

int CCSerialPort::GetLastError()
{
    return this->COM->getLastError();
}

const char *CCSerialPort::GetLastErrorMsg()
{
    return this->COM->getLastErrorMsg();
}

void CCSerialPort::onReadEvent(const char *portName, unsigned int readBufferLen)
{
    if (readBufferLen > 0)
    {
        char *data = new char[readBufferLen + 1];
        if(data)
        {
            int recLen = COM->readData(data, readBufferLen);
            if (recLen > 0)
            {
                data[recLen] = '\0';
                Date = data;
                if(Function)
                {
                    Function();
                }

            }

            delete[] data;
            data = NULL;
        }

    }

}

void CCSerialPort::Write(const void *data, int size)
{
    this->COM->writeData(data, size);
}

void CCSerialPort::Write(std::string data)
{
    this->COM->writeData(data.c_str(), data.length());
}

char *CCSerialPort::GetReadData()
{
    return Date;
}

void CCSerialPort::Close()
{
    this->COM->close();
}


