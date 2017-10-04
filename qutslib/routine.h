/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */


#ifndef ROUTINE_H
#define ROUTINE_H

#include <QVector>
#include "command.h"


namespace Quts {

class Routine {
public:
    Routine();
    const Command& operator[](int index) const {return m_commands[index];}
private:
    QVector<Command> m_commands;
};
}

#endif // ROUTINE_H
