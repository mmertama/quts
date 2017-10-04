/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef BeepPlayer_TEMPLATE_H
#define BeepPlayer_TEMPLATE_H

#include <functional>
#include <QtMultimedia/QAudioOutput>
#include <QBuffer>
#include <qmath.h>
#include <QtEndian>

#include "BeepPlayer/beepplayer.h"

/***
http://jigantic.blogspot.de/2010/12/generating-16bit-pcm-sine-wave-in.html
 generate 1 sec. worth of data (i.e., 44100 samples)
 (this sample can later be looped...see below)

 make the PCM-A440 sine wave:
 1 sec. of A440 = 440 cycles / periods
 take 44100 samples
         / 2 * PI     \
 y = sin| -------- * t |
         \ PERIOD     /

 where PERIOD = 1 / FREQUENCY and t = [0, 1] (seconds)
***/

namespace Beeper {

template<class T>
inline constexpr T pow(const T base, unsigned const exponent) {
    // (parentheses not required in next line)
    return (exponent == 0) ? 1 : (base * pow(base, exponent - 1));
}



template <int t_sampleSize, int t_sampleRate>
BeepPlayerImplementation<t_sampleSize, t_sampleRate>::BeepPlayerImplementation(QObject* parent) : BeepPlayer(parent) {
}

template <int t_sampleSize, int t_sampleRate>
QByteArray BeepPlayerImplementation<t_sampleSize, t_sampleRate>::generateBytes(qreal vol, qreal freq, int mslen) {
    switch(t_sampleSize) {
    case 8: return generateWav<quint8>(vol, freq, mslen);
    case 16: return generateWav<quint16>(vol, freq, mslen);
    case 32: return generateWav<quint32>(vol, freq, mslen);
    default:
        return QByteArray();
    }
}

template <int t_sampleSize, int t_sampleRate>
template <typename I> QByteArray BeepPlayerImplementation<t_sampleSize, t_sampleRate>::generateWav(qreal vol, qreal freq, int mslen) {
    const int samples = (mslen * t_sampleRate) / 1000;
    const qreal period = 1.0 / freq;
    const qreal timeStep = 1.0 / qreal(samples);
    const qreal bits = pow<int>(2, t_sampleSize - 1);

    qreal damp = 0.0;
    const qreal dampLimit = 20.0 * (t_sampleRate / 1000.0);
    const qreal dampStep =  1.0 / dampLimit;

    QByteArray bytes(sizeof(I) * samples, 0);
    I* array = reinterpret_cast<I*>(bytes.data());
    qreal time = 0.0;
    for(int i = 0; i < samples; ++i) {
        // expand out the equation above in steps
        const qreal angle = (2.0 * M_PI / period) * time;
        qreal factor = 0.5 * (qSin(angle) + 1.0); // convert range that sin returns from [-1, 1] to [0, 1]
        // factor is in the range [0, 1]
        // set the current sample to 2^(16-1) * factor
        // (since we'r e dealing with 16-bit PCM)
        // for a quieter wave, change 32768 to some
        // other maximum amplitude.
        //

        if(i < dampLimit && damp < 1.0) {
            damp += dampStep;
        }

        if(i > samples - dampLimit && damp > 0) {
            damp -= dampStep;
        }

        const I x = I(bits * factor * vol * damp);
        qToLittleEndian<I>(x, reinterpret_cast<uchar*>(&(array[i])));
        time += timeStep;
    }
    return bytes;
}

template <int t_sampleSize, int t_sampleRate>
BeepPlayer* BeepPlayer::open(QObject* parent) {
    BeepPlayer* player = NULL;
    if(t_sampleSize == 8 || t_sampleSize == 16 || t_sampleSize == 32) {
        player = new BeepPlayerImplementation<t_sampleSize, t_sampleRate>(parent);
        if(!player->createAudioDevice(t_sampleSize, t_sampleRate)) {
            delete player;
            player = NULL;
        }
    }
    return player;
}

}

#endif // BeepPlayer_TEMPLATE_H

