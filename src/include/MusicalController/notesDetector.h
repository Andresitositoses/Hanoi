#ifndef NOTES_DETECTOR_H
#define NOTES_DETECTOR_H

#include <PortAudio/portaudio.h>
#include <FFTW/fftw3.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <vector>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float FREQUENCY_TOLERANCE = 30.0;

enum Note {
    NONE,
    SI, LA, SOL, FA, MI, RE, DO
};

// Frecuencias de las notas (en Hz)
const std::pair<Note, float> NOTAS[] = {
    {SI, 987.77}, {LA, 880.00}, {SOL, 783.99},
    {FA, 698.46}, {MI, 659.26}, {RE, 587.33},
    {DO, 523.25}
};

typedef struct {
    double sampleRate;
    std::vector<float> buffer;
    size_t bufferSize;
    size_t currentIndex;
    Note lastDetectedNote;
    bool isNoteUpdated;
} AudioData;

class NotesDetector {
    private:
        AudioData audioData;
        PaStream* stream;
    public:
        NotesDetector();
        ~NotesDetector();
        void start();
        void stop();
        Note getDetectedNote();
        std::string getDetectedNoteString();
    private:
        void initializePortAudio();
        void setupInputStream();
        static void processAudioBuffer(AudioData* data);
        static int audioCallback(const void* inputBuffer, void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData);
        static float findDominantFrequency(const fftw_complex* output, int N, double sampleRate);
        static Note detectNote(float frequency);
};


#endif