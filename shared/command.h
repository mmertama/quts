/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef COMMAND_H
#define COMMAND_H

#include "utils.h"

namespace Quts {


class Command {
public:
    Keyword cmd;
    QStringList params;
    QStringList returnValues;
    int lineNo;
    bool isValid() const {return cmd.isValid();}
};

typedef QHash<Keyword, QList<Command>> CommandList;

}

#endif // COMMAND_H
