#ifndef IMGUI_CCSERIALPORT_H
#define IMGUI_CCSERIALPORT_H
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#define imsleep(microsecond) Sleep(microsecond) // ms
#else
#include <unistd.h>
#define imsleep(microsecond) usleep(1000 * microsecond) // ms
#endif

#include <vector>
#include <functional>
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"

using namespace itas109;

class CCSerialPort:public CSerialPortListener
{
public:
    CCSerialPort();
    explicit CCSerialPort(CSerialPort *sp):COM(sp)
    {
        PortsList = CSerialPortInfo::availablePortInfos();

    };
    //----------------------------------------------------------------
    unsigned int GetPortSize();
    SerialPortInfo * GetProtInfoAll();
    void Initialize(const char *portName,
                    int baudRate /*= itas109::BaudRate::BaudRate9600*/,
                    itas109::Parity parity = itas109::Parity::ParityNone,
                    itas109::DataBits dataBits = itas109::DataBits::DataBits8,
                    itas109::StopBits stopbits = itas109::StopBits::StopOne,
                    itas109::FlowControl flowControl = itas109::FlowControl::FlowNone,
                    unsigned int readBufferSize = 4096);
    void SetReadIntervalTime(unsigned int ms);
    bool Open();
    bool IsOpen();
    int GetLastError();
    const char * GetLastErrorMsg();
    void onReadEvent(const char *portName, unsigned int readBufferLen) override;
    void Write(const void *data, int size);
    void Write(std::string data);
    char* GetReadData();
    void Close();
    //----------------------------------------------------------------
    template <typename Func, typename... Args>
    bool SetReadClick(Func&& fun, Args&&... args);


private:
    CSerialPort *COM;
    std::vector<SerialPortInfo> PortsList;
    SerialPortInfo* serialPortInfo;
    std::function<void()> Function;
    char* Date;

};

template<typename Func, typename... Args>
inline bool CCSerialPort::SetReadClick(Func &&fun, Args &&... args)
{
    Function = std::bind(std::forward<Func>(fun), std::forward<Args>(args)...);
    return true;
}

#endif
