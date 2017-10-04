/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <QRegularExpression>
#include <QDebug>
#include <QThread>

#include "printer.h"
#include "quts.h"

using namespace Quts;

constexpr char NEWLINE[] = "(?:^|[^\\\\])\\\\n";

void Printer::flush(QutsAPI* quts, bool toConsole) {
    if(!m_buffer.isEmpty()) {
        static const QRegularExpression newline(NEWLINE);
        const auto split = m_buffer.split(newline);
        for(const auto& line : split) {
            if(toConsole) {
                qDebug().noquote().nospace() << line;
            }
            quts->printLine(line);
            QThread::yieldCurrentThread(); //OSX have IPC issues, in my experience yield sometimes helps :-)
        }
        m_buffer.clear();
    }
}
