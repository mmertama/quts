/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include "noteparser.h"
#include <QDebug>

using namespace Beeper;

//static const char notes[] =  "CDEFGAB";
//this is fun gets offset to ASCII value,
//      'C' - 'A' -> 2 -> 0
//      'D' - 'A' -> 3 -> 2
//      'E' - 'A' -> 4 -> 4
//      'F' - 'A' -> 5 -> 5
//      'G' - 'A' -> 6 -> 7
//      'A' - 'A' -> 0 -> 9
//      'B' - 'A' -> 1 -> 11
static const int notesOff[] = {9, 11, 0, 2, 4, 5, 7};
#define NOTE(c) (notesOff[c - 'A'])
static const qreal freqences[] = {
    16.351, //C 0
    17.324, //  1
    18.354, //D 2
    19.445, //  3
    20.601, //E 4
    21.827, //F 5
    23.124, //  6
    24.499, //G 7
    25.956, //  8
    27.5,   //A 9
    29.135, //  10
    30.868, //B 11

    32.703,
    34.648,
    36.708,
    38.891,
    41.203,
    43.654,
    46.249,
    48.999,
    51.913,
    55,
    58.27,
    61.735,

    65.406,
    69.296,
    73.416,
    77.782,
    82.407,
    87.307,
    92.499,
    97.999,
    103.826,
    110,
    116.541,
    123.471,

    130.813,
    138.591,
    146.832,
    155.563,
    164.814,
    174.614,
    184.997,
    195.998,
    207.652,
    220,
    233.082,
    246.942,

    261.626,
    277.183,
    293.665,
    311.127,
    329.628,
    349.228,
    369.994,
    391.995,
    415.305,
    440,
    466.164,
    493.883,

    523.251,
    554.365,
    587.33,
    622.254,
    659.255,
    698.456,
    739.989,
    783.991,
    830.609,
    880,
    932.328,
    987.767,

    1046.502,
    1108.731,
    1174.659,
    1244.508,
    1318.51,
    1396.913,
    1479.978,
    1567.982,
    1661.219,
    1760,
    1864.655,
    1975.533,

    2093.005,
    2217.461,
    2349.318,
    2489.016,
    2637.021,
    2793.826,
    2959.955,
    3135.964,
    3322.438,
    3520,
    3729.31,
    3951.066,

    4186.009,
    4434.922,
    4698.636,
    4978.032,
    5274.042,
    5587.652,
    5919.91,
    6271.928,
    6644.876,
    7040,
    7458.62,
    7902.132,

    8372.018,
    8869.844,
    9397.272,
    9956.064,
    10548.084,
    11175.304,
    11839.82,
    12543.856,
    13289.752,
    14080,
    14917.24,
    15804.264
};

#define LEN(a) (int(sizeof(a) / sizeof(a[0])))

static const QRegularExpression Token
("([CDEFGAB])([#b]?)(\\d)*");

qreal freq(int octave, int note, int next) {
    const int octaveSize = 12;
    const int start = octave * octaveSize;
    Q_ASSERT(start <= LEN(freqences));
    Q_ASSERT(note >= 'A' && note <= 'G');
    Q_ASSERT(next == '#' || next == 'b' || next == ' ');
    const int noteIndex = NOTE(note);
    int asc = 0;
    if(next == '#') {
        asc = 1;
        Q_ASSERT(noteIndex < octaveSize - 2);
    } else if(next == 'b') {
        asc = -1;
        Q_ASSERT(noteIndex > 0);
    }
    return freqences[start + noteIndex + asc];
}

NoteParser::NoteParser(const QString& notes, int octave) :
    m_notes(notes.split(' ')),
    m_octave(octave) {
}

const Note& NoteParser::next() {
    if(m_pos < m_notes.length()) {
        const auto match = Token.match(m_notes[m_pos]);
        if(match.hasMatch()) {
            const int note = match.captured(1)[0].toLatin1();
            const auto asc = match.captured(2);
            const int ascencion = !asc.isEmpty() ? asc[0].toLatin1() : ' ';
            bool ok = false;
            const auto dur = match.captured(3);
            if(!dur.isEmpty()) {
                m_current.duration = dur.toInt(&ok);
            }
            if(!ok) {
                m_current.duration = 4;
            }
            m_current.frequency = freq(m_octave, note, ascencion);
        } else {
            m_error = -1;
        }
        if(m_current.duration != 1 &&
                m_current.duration != 2 &&
                m_current.duration != 4 &&
                m_current.duration != 8 &&
                m_current.duration != 16 &&
                m_current.duration != 32) {
            m_error = -2;
        }
        ++m_pos;
    }
    return m_current;
}

bool NoteParser::isNext() const {
    return m_error == 0 && m_pos < m_notes.length();
}

int NoteParser::error() const {
    return m_error;
}


