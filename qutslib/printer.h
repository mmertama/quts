/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef PRINTER_H
#define PRINTER_H

#include <QTextStream>

namespace Quts {

class QutsAPI;

class Printer : public QTextStream {
public:
    Printer() : QTextStream(&m_buffer) {
    }
    void flush(QutsAPI* quts, bool toConsole);
    bool hasData() const {return !m_buffer.isEmpty();}
private:
    QString m_buffer;
};
}

#endif // PRINTER_H
