#ifndef CCAudio_H
#define CCAudio_H
#pragma once
#include "portaudio.h"
#include "vector"
#include "string"
#include "minimp3.h"
#include "bass.h"




struct AudioDeviceInfo {
    std::string name;
    unsigned int index;
    unsigned int Channels;
    PaDeviceInfo * deviceInfo = nullptr;
};

enum AudioType
{
    Capture,
    Render
};

typedef PaStream AudioStream;

namespace CCMultimedia
{
    bool Init();
    void Release();
    class CCAudio
    {
    public:
        PaStream *Stream; // 录音流和播放流
        PaStreamParameters Parameters;
        HSTREAM Bass_stream;

    public:
        CCAudio() = default;
        explicit CCAudio(AudioType type);
        //----------------------------------------------------------------
        bool OpenStream(PaStreamCallback *streamCallback,
                             double sampleRate = 48000,
                             unsigned long framesPerBuffer = 256,
                             PaStreamFlags streamFlags = paNoFlag,
                             void *userData = nullptr);
        bool BassInit(int device = -1, DWORD freq = 48000, DWORD flags=0, void *win=0, const void *dsguid = nullptr);
        bool Start();
        bool Start(PaStream *stream);
        bool Stop();
        bool Stop(PaStream *stream);
        bool CloseStream();
        bool CloseStream(PaStream *stream);
        bool ReadStream(void *buffer,unsigned long frames);
        bool WriteStream(void *buffer,unsigned long frames);
        bool BassReadStreamFile(const void *file, BOOL mem = FALSE, QWORD offset = 0, QWORD length = 0,
                            DWORD flags = BASS_SAMPLE_LOOP | BASS_SAMPLE_FLOAT);
        bool BassPlayStream(BOOL restart = FALSE);
        bool BassPausedStream();
        bool BassClogStream();
        void BassStop();
        void SetMode(AudioType Type);
        void SetSleep(long ms = 50);
        void SetStreamParameters(PaDeviceIndex device,PaTime suggestedLatency,int channelCount = 1,
                                 PaSampleFormat sampleFormat = paFloat32,
                                 void *hostApiSpecificStreamInfo = nullptr);
        void SetStream(HSTREAM hstream);
        bool SetSchedule(uint32_t s,DWORD mode = BASS_DATA_AVAILABLE);
        unsigned int GetDeviceCount();
        int GetDefaultDevice(AudioType Type);
        std::vector<AudioDeviceInfo> GetDevices(AudioType type);
        std::vector<BASS_DEVICEINFO> GetBassDevices(AudioType type);
        //----------------------------------------------------------------
        double GetSchedule(DWORD mode = BASS_DATA_AVAILABLE);
        double GetBassSecondslen(DWORD mode = BASS_DATA_AVAILABLE);
        //----------------------------------------------------------------
        HSTREAM GetStream();
        template<class name>
        std::vector<name> GetBassData(DWORD length,void * buff = nullptr);

    private:
        AudioType Type;
        bool BassStart = false;
        DWORD HZ = 44100;



    };
}



#endif
