#ifndef __DEFINES_H__
#define __DEFINES_H__

#define AUDIO_SAMPLE_RATE   44100 // Hz
#define AUDIO_BUFFER_SIZE   1024  // frames
#define AUDIO_CHANNELS_IN   0
#define AUDIO_CHANNELS_OUT  2

#define GPIO_TIMER_INTERVAL 1000 // ms

#define I2C_BUS_NUMBER      1 // 1 = Hardware I2C, 3 = Software I2C
#define I2C_SLAVE_ADDRESS   0x68
#define I2C_BUFFER_SIZE     6

#define MIDI_TIMER_INTERVAL 1   // ms
#define MIDI_IN_BUFFER_SIZE 100 // messages

#define NUMBER_OF_TRACKS    1

#define LEFT                0
#define RIGHT               1

typedef unsigned long long frame_t;
typedef unsigned char byte;

#endif // __DEFINES_H__
