/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef QUTSPARSER_H
#define QUTSPARSER_H

#include <functional>
#include <QStringList>
#include <QStack>

#include "command.h"
#include "subsystemdata.h"

namespace Quts {


class QutsParserClient {
public:
    virtual QString resolveName(const QString& key) const = 0;
    virtual bool execute(const Quts::Command& cmd) = 0;
    virtual QHash<Keyword, SubsystemData>& subSystems() = 0;
    virtual void errorStr(const std::initializer_list<QString>& p) = 0;
};

class QutsParser {
public:
    QutsParser(QutsParserClient& client);
    CommandList read(const QString& scriptName, std::function<QString(int line)> reader);
    QList<Command> command(const QString& line, int lineNo) {return appendLine(line, lineNo);}
    void clear();
private:
    Command parseLine(const QString& line, int lineNo, QStringList* subname);
    Command makePrint(const QString& cmd, int lineNo);
    QStringList makeSubroutine(const QString& cmd);
    Command makeCmd(const Keyword& key, const QString& params, int lineNo);
    QList<Command> parseError(int lineNo);
    QList<Command> appendLine(const QString& line, int lineNo);
    void appendCommand(QList<Command>& list, const Command& command) const;
    template <typename ...Args>
    void errorStr(const Args& ... s) {
        m_client.errorStr({s...});
    }
private:
    QutsParserClient& m_client;
    QString m_error;
    QStack<Keyword> m_scopeKeys;
    QStringList m_currentFunction;
};
}

#endif // QUTSPARSER_H
