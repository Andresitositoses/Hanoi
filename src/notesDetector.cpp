//
// Created by andre on 26/11/2024.
//

#include "include/MusicalController/notesDetector.h"

NotesDetector::NotesDetector() {
    initializePortAudio();
}

void NotesDetector::initializePortAudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw std::runtime_error("Error al inicializar PortAudio: " + std::string(Pa_GetErrorText(err)));
    }

    // Configurar datos de audio
    audioData.sampleRate = 44100;
    audioData.bufferSize = 2048;
    audioData.buffer.resize(audioData.bufferSize);
    audioData.currentIndex = 0;
    audioData.lastDetectedNote = "---";
    audioData.isNoteUpdated = false;

    setupInputStream();
}

void NotesDetector::setupInputStream() {
    int defaultInputDevice = Pa_GetDefaultInputDevice();
    const PaDeviceInfo* defaultDeviceInfo = Pa_GetDeviceInfo(defaultInputDevice);
    std::cout << "Usando micrófono: " << defaultDeviceInfo->name << "\n";

    PaError err = Pa_OpenDefaultStream(&stream,
                                     1,          // mono input
                                     0,          // sin output
                                     paFloat32,  // formato de muestra
                                     audioData.sampleRate,
                                     1024,       // frames por buffer
                                     audioCallback,
                                     &audioData);

    if (err != paNoError) {
        throw std::runtime_error("Error al abrir stream: " + std::string(Pa_GetErrorText(err)));
    }
}

NotesDetector::~NotesDetector() {
    if (stream) {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
    }
    Pa_Terminate();
}

void NotesDetector::start() {
    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        throw std::runtime_error("Error al iniciar stream: " + std::string(Pa_GetErrorText(err)));
    }
    std::cout << "Captura de audio iniciada. Detectando notas..." << std::endl;
}

void NotesDetector::stop() {
    if (stream && Pa_IsStreamActive(stream) == 1) {
        Pa_StopStream(stream);
    }
}

bool NotesDetector::getDetectedNote(std::string& note) {
    if (audioData.isNoteUpdated) {
        note = audioData.lastDetectedNote;
        audioData.isNoteUpdated = false;
        return true;
    }
    return false;
}

std::string NotesDetector::detectNote(float frequency) {
    for (const auto& nota : NOTAS) {
        if (std::abs(frequency - nota.second) < FREQUENCY_TOLERANCE) {
            return nota.first;
        }
    }
    return "---";
}

float NotesDetector::findDominantFrequency(const fftw_complex* output, int N, double sampleRate) {
    double maxAmplitude = 0.0;
    int dominantFreqIndex = 0;
    
    for (int i = 0; i < N/2; i++) {
        double amplitude = std::sqrt(output[i][0] * output[i][0] + output[i][1] * output[i][1]);
        if (amplitude > maxAmplitude) {
            maxAmplitude = amplitude;
            dominantFreqIndex = i;
        }
    }
    
    return dominantFreqIndex * sampleRate / N;
}

int NotesDetector::audioCallback(const void* inputBuffer, void* outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void* userData) {
    AudioData* data = static_cast<AudioData*>(userData);
    const float* in = static_cast<const float*>(inputBuffer);
    
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        data->buffer[data->currentIndex] = in[i];
        data->currentIndex = (data->currentIndex + 1) % data->bufferSize;
    }

    if (data->currentIndex % framesPerBuffer == 0) {
        processAudioBuffer(data);
    }

    return paContinue;
}

void NotesDetector::processAudioBuffer(AudioData* data) {
    std::vector<double> input(data->bufferSize);
    fftw_complex* output = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * data->bufferSize));
    fftw_plan plan = fftw_plan_dft_r2c_1d(data->bufferSize, input.data(), output, FFTW_ESTIMATE);

    // Aplicar ventana Hanning
    for (size_t i = 0; i < data->bufferSize; i++) {
        double hann = 0.5 * (1 - cos(2 * M_PI * i / (data->bufferSize - 1)));
        input[i] = data->buffer[i] * hann;
    }

    fftw_execute(plan);
    
    float dominantFreq = findDominantFrequency(output, data->bufferSize, data->sampleRate);
    std::string nota = detectNote(dominantFreq);
    
    // Actualizar la nota detectada
    data->lastDetectedNote = nota;
    data->isNoteUpdated = true;

    fftw_destroy_plan(plan);
    fftw_free(output);
}