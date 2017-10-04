/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include "audiosystem.h"
#include "constants.h"

#include "BeepPlayer/beepplayer.h"
#include "BeepPlayer/noteparser.h"
#include "utils.h"
#include "quts_doc.h"

using namespace Quts;

#define GUFFX(x) GUFF(AudioSubsystem, x)

SUBSYSTEM(AUDIO) {
    return new AudioSubsystem((QObject*)parent,  * (QutsEngine*) engine);
}

QString AudioSubsystem::sysName() const {
    return AUDIO_NAME;
}

AudioSubsystem::AudioSubsystem(QObject* parent, QutsEngine& engine) : Subsystem(parent, engine) {
}

void AudioSubsystem::inject() {
    GUFFX(beep);
    GUFFX(wait);
}

AudioSubsystem::~AudioSubsystem() {
}

void AudioSubsystem::create(const QStringList&) {
    if(m_beepPlayer == nullptr) {
        m_beepPlayer.reset(Beeper::BeepPlayer::open<16, 22050>(this)); //create only if Beep is needed.
        QObject::connect(m_beepPlayer.data(), &Beeper::BeepPlayer::endOfPlay, this, &AudioSubsystem::end);
    }
}

void AudioSubsystem::close() {
    if(m_beepPlayer) {
        m_beepPlayer->close();
    }
}

/**DOC
 * Play Notes
 * N BEEP
 * S AUDIO
 * B notes <duration> <octave>
 * P notes := notes note; note = [CDEFGAB][#b]?\d* and d 1, 2, 4, 8, or 16 (default 4)
 * P duration of each note in ms
 * P octave where notes are played
**/


/**DOC
 * Wait play to complete
 * N WAIT
 * S AUDIO
**/

bool AudioSubsystem::wait(const QStringList& params) {
    Q_UNUSED(params);
    if(isActive()) {
        m_engine.setPending();
    }
    return true;
}

void AudioSubsystem::clear() {
    if(isActive()) {
        m_beepPlayer->flush();
    }
    m_beepPlayer.reset();
}

bool AudioSubsystem::isActive() const {
    return m_beepPlayer != nullptr && m_beepPlayer->isActive();
}


bool AudioSubsystem::beep(const QStringList& values) {
    MIN_ARG(1)
    if(m_beepPlayer == nullptr) {
        m_engine.syntaxError(NOT_READY);
        return false;
    }
    const auto nv = m_engine.getValue(values[0]);

    if(!nv.canConvert(QVariant::String)) {
        m_engine.syntaxError(INVALID_ARGUMENT);
        return false;
    }

    const auto notes = nv.toString();

    int noteLength = 500;
    if(values.length() > 1) {
        if(!m_engine.numValue(values[1], noteLength)) {
            m_engine.syntaxError(INVALID_ARGUMENT, values[1]);
        }
    }
    int octave = 3;
    if(values.length() > 2) {
        if(!m_engine.numValue(values[2], octave) || octave < 0 || octave > Beeper::MAXOCTAVE) {
            m_engine.syntaxError(INVALID_ARGUMENT, values[2]);
        }
    }
    static qreal constexpr MAXVOL = 100;
    int vol = MAXVOL;
    if(values.length() > 3) {
        if(!m_engine.numValue(values[3], vol) || vol < 0 || vol > MAXVOL) {
            m_engine.syntaxError(INVALID_ARGUMENT, values[3]);
        }
    }
    Beeper::NoteParser noteparser(notes, octave);
    while(noteparser.isNext()) {
        const auto note = noteparser.next();
        if(noteparser.error() != 0) {
            m_engine.syntaxError(INVALID_ARGUMENT);
            return false;
        } else {
            if(!m_beepPlayer->play(note, noteLength, qreal(vol) / MAXVOL)) {
                m_engine.errorStr(AUDIO_ERROR);
                return false;
            }
        }
    }
    return true;
}
