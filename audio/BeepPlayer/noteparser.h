/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef NOTEPARSER
#define NOTEPARSER

#include <QRegularExpression>

namespace Beeper {

constexpr int MAXOCTAVE = 10;

class Note {
public:
    qreal   frequency;
    int     duration;
};

class NoteParser {
public:
    NoteParser(const QString& notes, int octave);
    const Note& next();
    bool isNext() const;
    int error() const;
private:
    const QStringList m_notes;
    const int m_octave;
    int m_pos = 0;
    int m_error = 0;
    Note m_current;
};

}

#endif // NOTEPARSER

