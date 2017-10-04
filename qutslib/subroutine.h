/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */


#ifndef SUBROUTINE_H
#define SUBROUTINE_H

#include <QStringList>
#include <QPair>

namespace Quts {

class Subroutine {
public:
    Subroutine(const QString& name, const QStringList& args) : m_name(name), m_args(args) {}
    Subroutine() {}
private:
    const QString m_name;
    QStringList m_args;
};

}

#endif // SUBROUTINE_H
