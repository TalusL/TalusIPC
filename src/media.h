#pragma once

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

extern "C"{
#include <hal/types.h>
#include "error.h"
}



typedef struct HALConfig_ {
    bool mirror;
    bool flip;
    unsigned int audio_srate;
    bool audio_enable;
    int audio_gain;
    unsigned int video_framerate;
    unsigned int video_width;
    unsigned int video_height;
    char sensor_config[128];
    unsigned long long encoding_bitrate;
    hal_vidcodec encoding_codec;
    hal_vidmode encoding_mode;
    hal_vidprofile encoding_profile;
    unsigned int encoding_gop;
} HALConfig;

extern HALConfig app_config;

int start_sdk(void);
int stop_sdk(void);


void request_idr(void);
void set_grayscale(bool active);
int take_next_free_channel(bool mainLoop);

int create_channel(char index, short width, short height, char framerate, char jpeg);
int bind_channel(char index, char framerate, char jpeg);
int unbind_channel(char index, char jpeg);
int disable_video(char index, char jpeg);

void disable_audio(void);
int enable_audio(void);