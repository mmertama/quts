/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <QtMultimedia/QAudioOutput>
#include <QFile>
#include <QDebug>

#include "audiobuffer.h"
#include "beepplayer.h"
#include "noteparser.h"

using namespace Beeper;

void dump(const QByteArray& data, const QString& fileName) {
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(data);
    file.close();
}

#define DUMP(d) dump(d, "debug.pcm")


using namespace Beeper;

BeepPlayer::BeepPlayer(QObject* parent) : QObject(parent),
    m_buffer(new AudioBuffer(this)), m_audio(nullptr) {
    QObject::connect(&m_start, &QTimer::timeout, [this]() {
        m_audio->start(m_buffer);
    });
    m_start.setSingleShot(true);
}

BeepPlayer::~BeepPlayer() {
}

bool BeepPlayer::createAudioDevice(int sampleSize, int sampleRate) {
    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(1);
    format.setSampleSize(sampleSize);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(sampleSize == 8 ? QAudioFormat::UnSignedInt : QAudioFormat::SignedInt);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(! info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return false;
    }

    m_audio = new QAudioOutput(format, this);

    connect(m_audio, &QAudioOutput::stateChanged, this, &BeepPlayer::stateChanged);

    return true;
}

void  BeepPlayer::stateChanged(QAudio::State state) {
    switch(state) {
    case QAudio::IdleState:
        if(
            m_state == PLAYING &&
            m_state != PENDING &&
            m_state != WAITING) {
            m_state = WAITING;
            m_audio->stop();
            m_buffer->clear();
            m_buffer->close();
            emit endOfPlay();
        }
        break;
    case QAudio::ActiveState:
        m_state = PLAYING;
        break;
    case QAudio::SuspendedState:
        break;
    case QAudio::StoppedState:
        if(m_state == WAITING) {
            m_state = WAITING;
            m_buffer->clear();
            m_buffer->close();
            emit endOfPlay();
        }
        break;
    }
}

bool BeepPlayer::play(const Note& note, int mslen, qreal vol) {
    return play(note.frequency, mslen / note.duration, vol);
}

bool BeepPlayer::play(qreal freq, int mslen, qreal vol) {
    if(m_state == WAITING) {
        m_state = PENDING;
    }

    const auto array = generateBytes(vol, freq, mslen);
    if(array.length() <= 0) {
        qWarning() << "PCM generation failed";
        return false;
    }


    if(m_audio->state() != QAudio::ActiveState) {
        m_buffer->open(QIODevice::ReadWrite);
        Q_ASSERT(m_buffer->isOpen());
        m_buffer->write(array);
        if(!m_start.isActive()) {
            m_start.start(0);
        }

    } else {
        m_buffer->write(array);
    }




    return true;
}


void BeepPlayer::close() {
    m_audio->stop();
    m_audio->reset();
    m_buffer->close();
}

void BeepPlayer::setVolume(qreal vol) {
    m_audio->setVolume(vol);
}

bool BeepPlayer::flush() {
    if(m_audio->state() == QAudio::ActiveState) {
        m_audio->stop();
        m_buffer->reset();
        m_audio->start();
    }
    return true;
}





