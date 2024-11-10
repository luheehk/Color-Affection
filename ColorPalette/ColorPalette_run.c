#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sndfile.h>

#define INPUT_FILE_NAME "allenFM.wav"
#define OUTPUT_FILE_NAME "HSV1.wav"
#define NUM_ECHO 100 // Number of Echo
#define MIX_RATIO 0.5 // Dry/Wet Mix
#define HUE_SCALING_FACTOR 0.0028f
#define SATURATION_SCALING_FACTOR 0.0028f
#define VALUE_SCALING_FACTOR 0.0028f

typedef struct {
  SNDFILE *file;
  SF_INFO info;
} SoundFile;

void rgbToHsv(float r, float g, float b, float *h, float *s, float *v);
void hueToDelayTime(float hue, float *delayTime);
void satToDecay(float saturation, float *decay);
void valToThreshold(float value, float *threshold);
int openInputFile(SoundFile *inFile);
int createOutputFile(SoundFile *inFile, SoundFile *outFile);
void processAudio(float *inputBuffer, float *outputBuffer, float *delayBuffer, sf_count_t bufferSize, float delayTime, float decay, float threshold, float gain);

float max(float a, float b, float c) {
  return ((a > b) ? (a > c ? a : c) : (b > c ? b : c));
}

float min(float a, float b, float c) {
  return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}

void rgbToHsv(float r, float g, float b, float *h, float *s, float *v) {
  r /= 255.0f;
  g /= 255.0f;
  b /= 255.0f;

  float cmax = max(r, g, b);
  float cmin = min(r, g, b);
  float diff = cmax - cmin;

  if (cmax == cmin)
    *h = 0;
  else if (cmax == r)
    *h = fmod((60 * ((g - b) / diff) + 360), 360.0);
  else if (cmax == g)
    *h = fmod((60 * ((b - r) / diff) + 120), 360.0);
  else
    *h = fmod((60 * ((r - g) / diff) + 240), 360.0);

  *s = (cmax == 0) ? 0 : (diff / cmax) * 100;
  *v = cmax * 100;

  printf("HSV=(%f, %f, %f)\n", *h, *s, *v);
}

void hueToDelayTime(float hue, float *delayTime) {
  *delayTime = hue * HUE_SCALING_FACTOR;
}

void satToDecay(float saturation, float *decay) {
  *decay = saturation * SATURATION_SCALING_FACTOR;
}

void valToThreshold(float value, float *threshold) {
  *threshold = value * VALUE_SCALING_FACTOR;
}

int openInputFile(SoundFile *sndFile) {
  memset(&sndFile->info, 0, sizeof(SF_INFO));
  sndFile->file = sf_open(INPUT_FILE_NAME, SFM_READ, &sndFile->info);
  
  if (!sndFile->file) {
    printf("Error: Could not open file: %s\n", INPUT_FILE_NAME);
    puts(sf_strerror(NULL));
    return 1;
  }

  if (!sf_format_check(&sndFile->info) || sndFile->info.channels > 1) {
    sf_close(sndFile->file);
    printf("Invalid file format or non-mono file\n");
    return 1;
  }

  printf("Sample rate: %d\nChannels: %d\nFrames: %lld\n",
         sndFile->info.samplerate, sndFile->info.channels, sndFile->info.frames);

  return 0;
}

int createOutputFile(SoundFile *inFile, SoundFile *outFile) {
  outFile->file = sf_open(OUTPUT_FILE_NAME, SFM_WRITE, &inFile->info);
  if (!outFile->file) {
    printf("Error: Could not open file: %s\n", OUTPUT_FILE_NAME);
    puts(sf_strerror(NULL));
    return 1;
  }
  return 0;
}

void processAudio(float *inputBuffer, float *outputBuffer, float *delayBuffer, sf_count_t bufferSize, float delayTime, float decay, float threshold, float gain) {
  sf_count_t sampleIndex;
  for (sf_count_t n = 0; n < bufferSize; n++) {
    delayBuffer[n] = inputBuffer[n];
    for (int i = 1; i < NUM_ECHO; i++) {
      sampleIndex = (sf_count_t)(n - (float)i * delayTime);
      if (sampleIndex >= 0) {
        delayBuffer[n] += MIX_RATIO * pow(decay, (float)i) * inputBuffer[sampleIndex];
      }
    }
  }

  for (sf_count_t n = 0; n < bufferSize; n++) {
    outputBuffer[n] = delayBuffer[n];
    if (outputBuffer[n] > threshold) {
      outputBuffer[n] = threshold + (outputBuffer[n] - threshold) * (1.0f / 10.0f);
    } else if (outputBuffer[n] < -threshold) {
      outputBuffer[n] = -threshold + (outputBuffer[n] + threshold) * (1.0f / 10.0f);
    }
    outputBuffer[n] *= gain;
  }
}

int main(void) {
  float h, s, v;
  int r, g, b;

  printf("Enter RGB values (0-255):\n");
  scanf("%d %d %d", &r, &g, &b);

  rgbToHsv(r, g, b, &h, &s, &v);

  float delayTime, decay, threshold, gain;
  hueToDelayTime(h, &delayTime);
  satToDecay(s, &decay);
  valToThreshold(v, &threshold);

  SoundFile inFile, outFile;
  if (openInputFile(&inFile)) return 1;

  sf_count_t bufferSize = inFile.info.frames;
  float *inputBuffer = (float *)malloc(bufferSize * sizeof(float));
  float *outputBuffer = (float *)calloc(bufferSize, sizeof(float));
  float *delayBuffer = (float *)calloc(bufferSize, sizeof(float));

  sf_read_float(inFile.file, inputBuffer, bufferSize);
  delayTime *= inFile.info.samplerate;
  gain = 1.0f / (threshold + (1.0f - threshold) * (1.0f / 10.0f));

  processAudio(inputBuffer, outputBuffer, delayBuffer, bufferSize, delayTime, decay, threshold, gain);

  if (createOutputFile(&inFile, &outFile)) return 1;
  sf_write_float(outFile.file, outputBuffer, bufferSize);

  sf_close(inFile.file);
  sf_close(outFile.file);
  free(inputBuffer);
  free(outputBuffer);
  free(delayBuffer);

  return 0;
}
