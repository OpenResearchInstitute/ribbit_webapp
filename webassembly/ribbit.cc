/*
WebAssembly Example 3

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/
#include <emscripten.h>
#include <stdio.h>
#include <algorithm>
#include "example.hh"
#include "dsp/complex.hh"
#include "dsp/window.hh"
#include "dsp/filter.hh"
#include "dsp/coeffs.hh"
#include "dsp/decibel.hh"
#include "dsp/fft.hh"
#include "dsp/encoder.hh"
#include "dsp/decoder.hh"

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif
static Encoder *encoder;
static Decoder *decoder;
EXTERN EMSCRIPTEN_KEEPALIVE void createEncoder()
{
    encoder = new Encoder();
    if (encoder)
    {
        printf("Encoder created!\n");
        EM_ASM({ encoderCreated($0); }, (int)encoder);
    }
}
EXTERN EMSCRIPTEN_KEEPALIVE void createDecoder()
{
    decoder = new Decoder();
    if (decoder)
    {
        printf("Decoder created!\n");
        EM_ASM({ decoderCreated($0); }, (int)decoder);
    }
}
EXTERN EMSCRIPTEN_KEEPALIVE int main()
{
    printf("wasmModule loaded!\n");
    createEncoder();
    createDecoder();
    EM_ASM({ mainCalled($0); }, 1);
    return 0;
}

static const int FEED_LENGTH = 2048; // the buffer size for the audio input
float feed[FEED_LENGTH];
EXTERN EMSCRIPTEN_KEEPALIVE float *feed_pointer() { return feed; }
EXTERN EMSCRIPTEN_KEEPALIVE int feed_length() { return FEED_LENGTH; }
static const int CHUNK_LENGTH = 160; // the buffer size for the audio input
float chunk[CHUNK_LENGTH];
float overflow[CHUNK_LENGTH];
int over = 0;
// payload
static const int PAYLOAD_LENGTH = 256;
uint8_t payload[PAYLOAD_LENGTH];
// message
static const int MESSAGE_LENGTH = 256;
uint8_t message[MESSAGE_LENGTH];
EXTERN EMSCRIPTEN_KEEPALIVE void *message_pointer()
{
    return message;
}
EXTERN EMSCRIPTEN_KEEPALIVE int message_length()
{
    return MESSAGE_LENGTH;
}
static const int SIGNAL_LENGTH = 4096 * 4;
float signal[SIGNAL_LENGTH];
EXTERN EMSCRIPTEN_KEEPALIVE float *signal_pointer()
{
    return signal;
}
EXTERN EMSCRIPTEN_KEEPALIVE int signal_length()
{
    return SIGNAL_LENGTH;
}
EXTERN EMSCRIPTEN_KEEPALIVE void *payload_pointer()
{
    return payload;
}
EXTERN EMSCRIPTEN_KEEPALIVE int payload_length()
{
    return PAYLOAD_LENGTH;
}
EXTERN EMSCRIPTEN_KEEPALIVE void feedDecoder()
{
    if (decoder == nullptr)
    {
        return;
    }
    bool getpayload = false;
    getpayload = decoder->feed(chunk, 160);
    if (getpayload)
    {
        printf("Message Incoming!\n");
        int outputresult = -1;
        outputresult = decoder->fetch(payload);
        EM_ASM({ fetchDecoded($0); }, outputresult);
    }
}
EXTERN EMSCRIPTEN_KEEPALIVE void digestFeed()
{
    int totalLength = over + FEED_LENGTH;
    // printf("totalLength: %d\n", totalLength);
    // new array for overflow with feed concatenated
    float localArray[totalLength];
    int index = 0;
    for (int i = 0; i < over; i++)
    {
        // fill in from overflow
        localArray[index++] = overflow[i];
    }
    for (int i = 0; i < FEED_LENGTH; i++)
    {
        // concatenate from feed
        localArray[index++] = feed[i];
    }

    // fill the chunk for the decoder and request
    // the decoder to be fed
    int fedTo = 0;
    for (int i = 0; i < totalLength; i += CHUNK_LENGTH)
    {
        if (i + CHUNK_LENGTH < totalLength)
        {
            // printf("chunk[%d-%d]", i, i + CHUNK_LENGTH);
            // fill chunk before feeding to decoder
            for (int j = 0; j < CHUNK_LENGTH; j++)
            {
                chunk[j] = localArray[i + j];
            }
            feedDecoder();
        }
        else
        {
            // what has not been fed to the decoder
            // will be stored in the overflow
            fedTo = i;
        }
    }

    // store the overflow
    if (fedTo >= 0)
    {
        over = totalLength - fedTo;
        for (int i = 0; i < over; i++)
        {
            overflow[i] = localArray[fedTo + i];
        }
        fedTo = 0;
    }
}
EXTERN EMSCRIPTEN_KEEPALIVE void initEncoder()
{
    encoder->init(message);
}
EXTERN EMSCRIPTEN_KEEPALIVE void readEncoder()
{
    encoder->read(signal, SIGNAL_LENGTH);
}
