// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2008 Jason Rogers (dovoto)
// Copyright (C) 2008 Dave Murphy (WinterMute)

// Sound Functions

#include <string.h>

#include <nds/arm9/cache.h>
#include <nds/arm9/sound.h>
#include <nds/fifocommon.h>
#include <nds/fifomessages.h>

void soundEnable(void)
{
    fifoSendValue32(FIFO_SOUND, SOUND_MASTER_ENABLE);
}
void soundDisable(void)
{
    fifoSendValue32(FIFO_SOUND, SOUND_MASTER_DISABLE);
}
int soundPlayPSG(DutyCycle cycle, u16 freq, u8 volume, u8 pan)
{
    FifoMessage msg;

    msg.type = SOUND_PSG_MESSAGE;
    msg.SoundPsg.dutyCycle = cycle;
    msg.SoundPsg.freq = freq;
    msg.SoundPsg.volume = volume;
    msg.SoundPsg.pan = pan;

    fifoMutexAcquire(FIFO_SOUND);

    fifoSendDatamsg(FIFO_SOUND, sizeof(msg), (u8 *)&msg);
    fifoWaitValue32Async(FIFO_SOUND);
    int result = fifoGetValue32(FIFO_SOUND);

    fifoMutexRelease(FIFO_SOUND);

    return result;
}

int soundPlayNoise(u16 freq, u8 volume, u8 pan)
{
    FifoMessage msg;

    msg.type = SOUND_NOISE_MESSAGE;
    msg.SoundPsg.freq = freq;
    msg.SoundPsg.volume = volume;
    msg.SoundPsg.pan = pan;

    fifoMutexAcquire(FIFO_SOUND);

    fifoSendDatamsg(FIFO_SOUND, sizeof(msg), (u8 *)&msg);
    fifoWaitValue32Async(FIFO_SOUND);
    int result = fifoGetValue32(FIFO_SOUND);

    fifoMutexRelease(FIFO_SOUND);

    return result;
}

int soundPlaySample(const void *data, SoundFormat format, u32 dataSize, u16 freq,
                    u8 volume, u8 pan, bool loop, u16 loopPoint)
{
    FifoMessage msg;

    msg.type = SOUND_PLAY_MESSAGE;
    msg.SoundPlay.data = data;
    msg.SoundPlay.freq = freq;
    msg.SoundPlay.volume = volume;
    msg.SoundPlay.pan = pan;
    msg.SoundPlay.loop = loop;
    msg.SoundPlay.format = format;
    msg.SoundPlay.loopPoint = loopPoint;
    msg.SoundPlay.dataSize = dataSize >> 2;

    fifoMutexAcquire(FIFO_SOUND);

    fifoSendDatamsg(FIFO_SOUND, sizeof(msg), (u8 *)&msg);
    fifoWaitValue32Async(FIFO_SOUND);
    int result = fifoGetValue32(FIFO_SOUND);

    fifoMutexRelease(FIFO_SOUND);

    return result;
}

void soundPause(int soundId)
{
    fifoSendValue32(FIFO_SOUND, SOUND_PAUSE | (soundId << 16));
}

void soundKill(int soundId)
{
    fifoSendValue32(FIFO_SOUND, SOUND_KILL | (soundId << 16));
}

void soundResume(int soundId)
{
    fifoSendValue32(FIFO_SOUND, SOUND_RESUME | (soundId << 16));
}

void soundSetVolume(int soundId, u8 volume)
{
    fifoSendValue32(FIFO_SOUND, SOUND_SET_VOLUME | (soundId << 16) | volume);
}

void soundSetPan(int soundId, u8 pan)
{
    fifoSendValue32(FIFO_SOUND, SOUND_SET_PAN | (soundId << 16) | pan);
}

void soundSetFreq(int soundId, u16 freq)
{
    fifoSendValue32(FIFO_SOUND, SOUND_SET_FREQ | (soundId << 16) | freq);
}

void soundSetWaveDuty(int soundId, DutyCycle cycle)
{
    fifoSendValue32(FIFO_SOUND, SOUND_SET_WAVEDUTY | (soundId << 16) | cycle);
}

MicCallback micCallback = 0;

void micBufferHandler(int bytes, void *user_data)
{
    (void)user_data;

    FifoMessage msg;

    fifoGetDatamsg(FIFO_SOUND, bytes, (u8 *)&msg);

    if (msg.type == MIC_BUFFER_FULL_MESSAGE)
    {
        if (micCallback)
            micCallback(msg.MicBufferFull.buffer, msg.MicBufferFull.length);
    }
}

int soundMicRecord(void *buffer, u32 bufferLength, MicFormat format, int freq,
                   MicCallback callback)
{
    FifoMessage msg;

    msg.type = MIC_RECORD_MESSAGE;
    msg.MicRecord.format = format;
    msg.MicRecord.buffer = buffer;
    msg.MicRecord.freq = freq;
    msg.MicRecord.bufferLength = bufferLength;

    micCallback = callback;

    fifoSetDatamsgHandler(FIFO_SOUND, micBufferHandler, 0);

    fifoMutexAcquire(FIFO_SOUND);

    fifoSendDatamsg(FIFO_SOUND, sizeof(msg), (u8 *)&msg);
    fifoWaitValue32Async(FIFO_SOUND);
    int result = fifoGetValue32(FIFO_SOUND);

    fifoMutexRelease(FIFO_SOUND);

    return result;
}

void soundMicOff(void)
{
    fifoSendValue32(FIFO_SOUND, MIC_STOP);
}
