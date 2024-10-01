#include <iostream>
#include "../include/CCAudio.h"



bool CCMultimedia::Init()
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

void CCMultimedia::Release()
{
    Pa_Terminate();
    BASS_Free();
}

unsigned int CCMultimedia::CCAudio::GetDeviceCount()
{
    return Pa_GetDeviceCount();
}

std::vector<AudioDeviceInfo> CCMultimedia::CCAudio::GetDevices(AudioType type)
{
    std::vector<AudioDeviceInfo> inputDevices; //用于存储输入设备的数组
    auto deviceCount = GetDeviceCount();
    for (int i = 0; i < deviceCount; ++i)
    {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo)
        {
            if(type == AudioType::Capture)
            {
                if (deviceInfo->maxInputChannels > 0)
                { // 检查设备是否支持输入
                    AudioDeviceInfo device;
                    device.name = deviceInfo->name;
                    device.index = i;
                    device.Channels = deviceInfo->maxInputChannels;
                    device.deviceInfo = (PaDeviceInfo *)deviceInfo;
                    inputDevices.push_back(device); // 将设备信息添加到数组中
                    //std::cout << "Found input device: " << device.name << std::endl;
                }
            }
            else if (type == AudioType::Render)
            {
                if (deviceInfo->maxOutputChannels > 0)
                { // 检查设备是否支持输入
                    AudioDeviceInfo device;
                    device.name = deviceInfo->name;
                    device.index = i;
                    device.Channels = deviceInfo->maxOutputChannels;
                    device.deviceInfo = (PaDeviceInfo *)deviceInfo;
                    inputDevices.push_back(device); // 将设备信息添加到数组中
                    //std::cout << "Found input device: " << device.name << std::endl;
                }
            }
        }

    }
    return inputDevices;
}

bool CCMultimedia::CCAudio::OpenStream(PaStreamCallback *streamCallback,
                              double sampleRate,
                              unsigned long framesPerBuffer,
                              PaStreamFlags streamFlags,
                              void *userData){
    PaError err;
    if(Type == AudioType::Capture)
    {
        err = Pa_OpenStream(&Stream,&Parameters, nullptr,sampleRate,framesPerBuffer,
                                 streamFlags,streamCallback,userData);
    }
    else
    {
        err = Pa_OpenStream(&Stream, nullptr, &Parameters,sampleRate,framesPerBuffer,
                            streamFlags,streamCallback,userData);
    }
    if (err != paNoError)
    {
        std::cerr << "Failed to open record stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}


void CCMultimedia::CCAudio::SetStreamParameters(PaDeviceIndex device,PaTime suggestedLatency,int channelCount,PaSampleFormat sampleFormat,
                                       void *hostApiSpecificStreamInfo)
{
    Parameters.device = device;
    Parameters.channelCount = channelCount; // 录音单通道
    Parameters.sampleFormat = sampleFormat; // 32位浮点数样本
    Parameters.suggestedLatency = suggestedLatency;
    Parameters.hostApiSpecificStreamInfo = hostApiSpecificStreamInfo;
}

bool CCMultimedia::CCAudio::Start()
{
    auto err = Pa_StartStream(Stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::Stop()
{
    auto err = Pa_StartStream(Stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::CloseStream()
{
    auto err = Pa_StartStream(Stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

int CCMultimedia::CCAudio::GetDefaultDevice(AudioType Type)
{
    if(Type == AudioType::Capture)
    {
        return Pa_GetDefaultInputDevice();
    }
    else if(Type == AudioType::Render)
    {
        return Pa_GetDefaultOutputDevice();
    }
    return -1;
}

bool CCMultimedia::CCAudio::Start(PaStream *stream)
{
    auto err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::Stop(PaStream *stream)
{
    auto err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::CloseStream(PaStream *stream)
{
    auto err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::ReadStream(void *buffer, unsigned long frames)
{
    auto err = Pa_ReadStream(Stream,buffer,frames);
    if(err == paNoError)
        return true;
    return false;
}

bool CCMultimedia::CCAudio::WriteStream(void *buffer, unsigned long frames)
{
    auto err = Pa_WriteStream(Stream,buffer,frames);
    if(err == paNoError)
        return true;
    return false;
}

void CCMultimedia::CCAudio::SetSleep(long ms)
{
    Pa_Sleep(ms);
}

void CCMultimedia::CCAudio::SetMode(AudioType type)
{
    Type = type;
}

CCMultimedia::CCAudio::CCAudio(AudioType type)
{
    Type = type;
}

bool CCMultimedia::CCAudio::BassInit(int device, DWORD freq, DWORD flags, void *win, const void *dsguid)
{
    HZ = freq;
#ifdef _WIN32
    return BASS_Init(device, freq, flags, (HWND)win, dsguid);
#elif __linux__
    return BASS_Init(device, freq, flags, win, dsguid);
#endif
}

bool CCMultimedia::CCAudio::BassReadStreamFile(const void *file, BOOL mem, QWORD offset, QWORD length, DWORD flags)
{
    Bass_stream = BASS_StreamCreateFile(mem, file, offset, length, flags);
    if (Bass_stream == 0)
    {
        std::cerr << "BASS_StreamCreateFile failed: " << BASS_ErrorGetCode() << std::endl;
        return false;
    }
    return true;
}

bool CCMultimedia::CCAudio::BassPlayStream(BOOL restart)
{
    BassStart = BASS_ChannelPlay(Bass_stream, restart);
    if (!BassStart)
    {
        return false;
    }
    return true;
}

void CCMultimedia::CCAudio::BassStop()
{
    BassStart = false;
    BASS_ChannelStop(Bass_stream);
    BASS_StreamFree(Bass_stream);

}

HSTREAM CCMultimedia::CCAudio::GetStream()
{
    return Bass_stream;
}

double CCMultimedia::CCAudio::GetBassSecondslen(DWORD mode)
{
    QWORD lengthInBytes = BASS_ChannelGetLength( Bass_stream, mode);
    if (lengthInBytes == 0) {
        // 长度未知，可能是一个流或者无法确定长度
        std::cerr << "Unable to determine audio length." << std::endl;
        return 0.0;
    }

    // 转换为秒数
    double lengthInSeconds = BASS_ChannelBytes2Seconds(Bass_stream, lengthInBytes);
    return lengthInSeconds;
}

template<class name>
std::vector<name> CCMultimedia::CCAudio::GetBassData(DWORD length, void *buff)
{
    name* buf = new name[length * sizeof(name)];
    std::vector<name> data;
    if(buff == nullptr)
    {
        BASS_ChannelGetData(Bass_stream,buf,length);
    }
    else
    {
        BASS_ChannelGetData(Bass_stream,buff,length);
    }
    for (int i = 0; i < length; ++i) {
        data.push_back(buf[i]);
    }
    return data;
}

double CCMultimedia::CCAudio::GetSchedule(DWORD mode)
{
    if(Bass_stream != 0)
    {
        QWORD position = BASS_ChannelGetPosition(Bass_stream, mode); // 获取字节位置
        auto seconds = BASS_ChannelBytes2Seconds(Bass_stream, position); // 转换为秒
        return seconds;
    }
    return 0;
}

bool CCMultimedia::CCAudio::BassPausedStream()
{
    if(BASS_ChannelPause(Bass_stream))
    {
        return true;
    }
    return false;
}

bool CCMultimedia::CCAudio::BassClogStream()
{
    while (BASS_ChannelIsActive(Bass_stream))
    {

    }
    return true;
}

void CCMultimedia::CCAudio::SetStream(HSTREAM hstream)
{
    Bass_stream = hstream;
}

bool CCMultimedia::CCAudio::SetSchedule(uint32_t s,DWORD mode)
{
    // 设置音频的播放进度到第30秒
    QWORD newPosition = s * 10 * HZ; // 假设音频的采样率是44100Hz（即BASS_FREQ的默认值）
    if (!BASS_ChannelSetPosition(Bass_stream, newPosition, mode)) {
        return false;
    }
    return true;
}

std::vector<BASS_DEVICEINFO> CCMultimedia::CCAudio::GetBassDevices(AudioType type)
{
    std::vector<BASS_DEVICEINFO> devices;
    if(type == AudioType::Render)
    {
        for (DWORD i = 0; ;i++)
        {
            BASS_DEVICEINFO info;
            if(!BASS_GetDeviceInfo(i, &info))
            {
                break;
            }
            devices.push_back(info);
        }
    }
    else if(type == AudioType::Capture)
    {
        for (int i = 0; ; i++)
        {
            BASS_DEVICEINFO rdi;
            if (!BASS_RecordGetDeviceInfo(i, &rdi))
            {
                break;
            }
           devices.push_back(rdi);
        }
    }
    return devices;
}

