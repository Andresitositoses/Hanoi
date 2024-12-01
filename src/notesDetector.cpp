//
// Created by andre on 26/11/2024.
//

#include "include/MusicalController/notesDetector.h"

#define BASE_SENSITIVITY 0.7  // Valor base entre 0.0 y 1.0
                             // Más bajo = más sensible
                             // Más alto = menos sensible
                             // Bajado de 0.9 a 0.7

NotesDetector::NotesDetector() {
    initializePortAudio();
}

void NotesDetector::initializePortAudio() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        throw std::runtime_error("Error al inicializar PortAudio: " + std::string(Pa_GetErrorText(err)));
    }

    // Aumentamos el tamaño del buffer para mejor resolución frecuencial
    audioData.sampleRate = 44100;
    audioData.bufferSize = 4096;  // Aumentado de 2048 a 4096
    audioData.buffer.resize(audioData.bufferSize);
    audioData.currentIndex = 0;
    audioData.lastDetectedNote = NONE;
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
                                     2048,       // Aumentado de 1024 a 2048
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

Note NotesDetector::getDetectedNote() {
    return audioData.lastDetectedNote;
}

std::string NotesDetector::getDetectedNoteString() {
    std::string noteString = "";
    switch (audioData.lastDetectedNote) {
        case SI: noteString = "SI"; break;
        case LA: noteString = "LA"; break;
        case SOL: noteString = "SOL"; break;
        default: noteString = "NONE"; break;
    }
    return noteString;
}

Note NotesDetector::detectNote(float frequency) {
    // Si la frecuencia es 0 (señal muy débil) retornamos NONE
    if (frequency == 0.0f) {
        // std::cout << "No se detecta ninguna nota" << std::endl;  // Opcional: descomentar para debug
        return NONE;
    }

    for (const auto& nota : NOTAS) {
        if (std::abs(frequency - nota.second) < FREQUENCY_TOLERANCE) {
            std::cout << "Nota detectada: " << nota.first << " - Frecuencia: " << frequency << std::endl;
            return nota.first;
        }
    }
    return NONE;
}

float NotesDetector::findDominantFrequency(const fftw_complex* output, int N, double sampleRate) {
    // Calculamos los umbrales basados en la sensibilidad
    const double amplitudeThreshold = 0.5 + (BASE_SENSITIVITY * 0.3);  // Rango: 0.5 - 0.8 (antes 0.4 - 0.8)
    const double powerThreshold = 0.002 + (BASE_SENSITIVITY * 0.003);  // Rango: 0.002 - 0.005 (antes 0.001 - 0.005)
    
    // Primero calculamos la potencia absoluta de la señal
    double signalPower = 0.0;
    for (int i = 0; i < N/2; i++) {
        double real = output[i][0];
        double imag = output[i][1];
        signalPower += (real * real + imag * imag) / N;
    }
    
    // Si la potencia absoluta es muy baja, no hay sonido real
    if (signalPower < powerThreshold) {
        return 0.0f;
    }
    
    double maxAmplitude = 0.0;
    int dominantFreqIndex = 0;
    
    // Encontrar la amplitud máxima para normalizar
    double maxPossibleAmplitude = 0.0;
    for (int i = 3; i < N/2; i++) {  // Empezamos desde 3 para captar mejor los graves
        double amplitude = std::sqrt(output[i][0] * output[i][0] + output[i][1] * output[i][1]);
        maxPossibleAmplitude = std::max(maxPossibleAmplitude, amplitude);
    }
    
    // Si la amplitud máxima es muy baja, no hay señal significativa
    if (maxPossibleAmplitude < 1e-6) {
        return 0.0f;
    }
    
    // Buscar la frecuencia dominante
    for (int i = 3; i < N/2; i++) {
        double amplitude = std::sqrt(output[i][0] * output[i][0] + output[i][1] * output[i][1]);
        amplitude = amplitude / maxPossibleAmplitude;
        
        if (amplitude > amplitudeThreshold && amplitude > maxAmplitude) {
            maxAmplitude = amplitude;
            dominantFreqIndex = i;
        }
    }
    
    // Si no encontramos ninguna frecuencia que supere el umbral
    if (maxAmplitude < amplitudeThreshold) {
        return 0.0f;
    }
    
    // Solo hacer interpolación si realmente encontramos una frecuencia dominante
    if (dominantFreqIndex > 0 && dominantFreqIndex < N/2 - 1) {
        double alpha = std::sqrt(output[dominantFreqIndex-1][0] * output[dominantFreqIndex-1][0] + 
                               output[dominantFreqIndex-1][1] * output[dominantFreqIndex-1][1]) / maxPossibleAmplitude;
        double beta = std::sqrt(output[dominantFreqIndex][0] * output[dominantFreqIndex][0] + 
                              output[dominantFreqIndex][1] * output[dominantFreqIndex][1]) / maxPossibleAmplitude;
        double gamma = std::sqrt(output[dominantFreqIndex+1][0] * output[dominantFreqIndex+1][0] + 
                               output[dominantFreqIndex+1][1] * output[dominantFreqIndex+1][1]) / maxPossibleAmplitude;
        
        double p = 0.5 * (alpha - gamma) / (alpha - 2*beta + gamma);
        return (dominantFreqIndex + p) * sampleRate / N;
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
    fftw_plan plan = fftw_plan_dft_r2c_1d(data->bufferSize, input.data(), output, FFTW_MEASURE);

    // Aplicar ventana Blackman-Harris para mejor resolución espectral
    for (size_t i = 0; i < data->bufferSize; i++) {
        const double a0 = 0.35875;
        const double a1 = 0.48829;
        const double a2 = 0.14128;
        const double a3 = 0.01168;
        double blackmanHarris = a0 - a1 * cos(2*M_PI*i/(data->bufferSize-1)) 
                                  + a2 * cos(4*M_PI*i/(data->bufferSize-1)) 
                                  - a3 * cos(6*M_PI*i/(data->bufferSize-1));
        input[i] = data->buffer[i] * blackmanHarris;
    }

    fftw_execute(plan);
    
    float dominantFreq = findDominantFrequency(output, data->bufferSize, data->sampleRate);
    Note nota = detectNote(dominantFreq);
    
    // Actualizar la nota detectada siempre, sea NONE o no
    data->lastDetectedNote = nota;
    data->isNoteUpdated = true;

    fftw_destroy_plan(plan);
    fftw_free(output);
}