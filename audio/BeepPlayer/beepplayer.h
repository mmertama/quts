/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef BEEPPLAYER
#define BEEPPLAYER

#include <QByteArray>
#include <QAudio>
#include <QTimer>
#

class QAudioOutput;


namespace Beeper {

class AudioBuffer;
class Note;

class BeepPlayer : public QObject {
    Q_OBJECT
public:
    inline bool isActive() const {return m_state != WAITING;}
    virtual bool play(qreal freq, int mslen, qreal vol = 1.0);
    bool play(const Note& note, int mslen, qreal vol = 1.0);
    virtual bool flush();
    template <int t_sampleSize, int t_sampleRate>
    static BeepPlayer* open(QObject* parent = NULL);
    void setVolume(qreal vol);
    ~BeepPlayer();
    void close();
protected:
    BeepPlayer(QObject* parent);
    bool createAudioDevice(int sampleSize, int sampleRate);
protected:
    virtual QByteArray generateBytes(qreal vol, qreal freq, int mslen) = 0;
signals:
    void endOfPlay();
private slots:
    void stateChanged(QAudio::State);
protected:
private:
    enum State {WAITING, PENDING, PLAYING};
    AudioBuffer* m_buffer;
    QAudioOutput* m_audio;
    State m_state = WAITING;
    QTimer m_start;
};

template <int t_sampleSize, int t_sampleRate>
class BeepPlayerImplementation : public BeepPlayer {
public:
    BeepPlayerImplementation(QObject* parent);
private:
    template <typename I>
    static QByteArray generateWav(qreal vol, qreal freq, int mslen);
    QByteArray generateBytes(qreal vol, qreal freq, int mslen);

};

}

#include "beepplayer_template.h"



#endif // BEEPPLAYER

