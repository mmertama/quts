/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <QRegularExpression>

#include "qutsparser.h"
#include <QSet>

#include "constants.h"
#include "subsystem.h"

using namespace Quts;

constexpr char COMMAND_LINE[] = "^\\s*(.*?)\\s+(.*)";
constexpr char EOL[] = "[\\n\\r]$";
constexpr char COMMENT_[] = "#";
constexpr char SUBROUTINE_[] = "fun";
constexpr char LAMBDA_[] = "lambda";
constexpr char RETURN_VALUE = '=';
constexpr char TOKEN[] = "[A-Za-z_][A-Za-z0-9_]*";
constexpr char PRINT_CMD[] = "!";
constexpr char STRING_MATCH[] = R"((?:[a-zA-Z_]\w*\:)?([\"'])(?:[^\1\\]|\\.)*?\1|\=?[\w\.]+(?:(?:\.|\:)\w+)?|\=?\$\w*|-?(?:\d+\.?\d*|-?\d*\.?\d+))";


constexpr char FUN[] = "FUN";

#define COMPILETIME_COMMANDS {"USE", "IMPORT", "ALIAS"}

constexpr char ERR_END_OF_SUB[] = "End of scope";
constexpr char ERR_BAD_SUBNAME[] = "Bad subname";
constexpr char ERR_INVALID_NAME[] = "Invalid name";

static QStringList getReturnValues(QStringList& values) {
    QStringList returnValues;
    for(auto i = values.length() - 1 ; i >= 0; i--) {
        if(values.at(i).at(0) == RETURN_VALUE) {
            returnValues.append(values.takeAt(i).mid(1));
        }
    }
    return returnValues;
}

static QStringList split(const QString& value, int* err) {
    *err = -1;
    static const QRegularExpression re(STRING_MATCH);
    auto it = re.globalMatch(value);
    QStringList list;
    int pos = 0;
    while(it.hasNext()) {
        const auto m = it.next();
        const auto gap = value.mid(pos, m.capturedStart() - pos);
        if(gap.simplified().length() > 0) {
            *err = pos;
            break;
        }
        pos = m.capturedEnd();
        list.append(m.captured());
    }
    const auto gap = value.mid(pos);
    if(gap.simplified().length() > 0) {
        *err = pos;
    }
    return list;
}

static int scanChar(const QString& s, QChar c) {
    int quotes = 0;
    for(int i = 0; i < s.length(); i++) {
        if(s.at(i) == '\"') {
            ++quotes;
        } else if((quotes & 0x1) == 0 && s.at(i) == c) {
            return i;
        }
    }
    return -1;
}


QutsParser::QutsParser(QutsParserClient& client) : m_client(client) {
}


Command QutsParser::makePrint(const QString& cmd, int lineNo) {
#ifdef BROKEN_COMPILER
    const auto value = c_conv<QStringList, QString>(cmd);
    const Command cmd = {key, value, lineNo};
    m_commands.append(cmd);
#else
    auto value = QStringList({cmd});
    //const auto returnValues = getReturnValues(value);
    //print has no return values
    return {Keyword("_print"), value, {}, lineNo};
#endif
}

QStringList QutsParser::makeSubroutine(const QString& call) {
    static const QRegularExpression re(TOKEN);
    auto m = re.globalMatch(call);
    QStringList subname;
    while(m.hasNext()) {
        const QString token = m.next().captured();
        subname.append(token);
    }
    return subname;
}

QList<Command> QutsParser::parseError(int lineNo) {
    errorStr(SUBROUTINE_ERROR, QString("line %1").arg(lineNo));
    return QList<Command>();
}

Command QutsParser::makeCmd(const Keyword& key, const QString& params, int lineNo) {
    int errPos;
    const auto simplifiedParams = params.simplified();
    auto value = split(simplifiedParams, &errPos);
    const auto returnValue = getReturnValues(value);
    if(errPos >= 0) {
        m_error = QString("%1 at line %2, close %3").arg(INVALID_ARGUMENT).arg(lineNo).arg(simplifiedParams.mid(errPos));
        return Command();
    }
    return {key, value, returnValue, lineNo};
}

QList<Command> QutsParser::appendLine(const QString& line, int lineNo) {
    static const QRegularExpression re(COMMAND_LINE);
    const auto match = re.match(line);
    const QList<Command> nothing;
    if(!match.isValid()) {
        m_error = ERR_INVALID_NAME;
        return nothing;
    }

    const auto key = match.captured(1);

    if(key == COMMENT_ || key.isEmpty()) {
        return nothing;
    }

    if(SCOPE_WORDS.contains(Keyword(key))) {
        m_scopeKeys.append(key);
    } else if(key == END_OF_SCOPE) {
        if(m_scopeKeys.isEmpty()) {
            m_error = ERR_END_OF_SUB;
            return nothing;
        }
        const auto thisScope = m_scopeKeys.pop();
        if(thisScope == FUN || thisScope == LAMBDA_) {
            m_currentFunction.removeLast(); // real end of subroutine
            return nothing;
        }
    }

    if(key == PRINT_CMD) {
        static const QRegularExpression eol(EOL);
        return {makePrint(match.captured(2).remove(eol), lineNo)};
    }

    if(key == LAMBDA_) {
        const auto name  = LAMBDA_ + m_currentFunction.last();
        m_currentFunction.append(makeSubroutine(asSysKey(name, qHash(lineNo), m_scopeKeys.size())));
        return {makeCmd(Keyword(LAMBDA_), m_currentFunction.join(GLOBAL_SEP) + " " + match.captured(2).trimmed(), lineNo)};
    }

    if(key == SUBROUTINE_) {
        if(m_currentFunction.length() != 1) {
            m_error = ERR_BAD_SUBNAME;
        } else {
            const auto subname = makeSubroutine(match.captured(2).trimmed());
            if(subname.isEmpty()) {
                m_error = ERR_INVALID_NAME;
            }
            if(subname.first() == m_currentFunction[0]){
                m_error = ERR_BAD_SUBNAME;
            }
            m_currentFunction.append(subname.first());
        }
        return nothing;
    }

    const Keyword asKey(key);
    const auto params = match.captured(2);
    const auto sepIndex = scanChar(params, ';');
    if(sepIndex < 0) {
        return {makeCmd(asKey, params, lineNo)};

    } else {
        const auto p1 = params.left(sepIndex);
        QList<Command> list = {makeCmd(asKey, p1, lineNo)};
        list.append(appendLine(params.mid(sepIndex + 1).simplified(), lineNo));
        return list;
    }
}


void QutsParser::appendCommand(QList<Command>& list, const Command& command) const {
    list.append(command);
    //Safe command to jump
    const auto justAdded = command.cmd;
    if(justAdded == "CALL" || justAdded == "EVAL") {
        list.append({Keyword("NOP"), {},  std::move(command.returnValues), command.lineNo});
    }
}

CommandList QutsParser::read(const QString& scriptName, std::function<QString(int line)> reader) {

    int lineNo = 0;
    m_error = QString();

    const auto scopeDepth = m_scopeKeys.length();

    CommandList routines;
    routines.insert(scriptName, QList<Command>());
    QList<Command>* commands = &routines[scriptName];
    QList<Command> preCommands;

    //function in real name format
    auto currentRoutine = scriptName;
    m_currentFunction = QStringList({scriptName});
    QString subCall;


    //handle subsystem multiple friendly name definitios
    QHash<QString, QString> aliasNames;
    for(auto& ss : m_client.subSystems()) {
        aliasNames.insert(ss->sysName(), ss.name());
        ss.setName(INVALID_NAME);
    }

    QVector<QPair<QString, int>> unresolvedCommands;



    for(;;) {
        const auto line = reader(lineNo);
        if(line.isNull()) {
            break;
        }

        ++lineNo;


        const auto parsed = appendLine(line, lineNo);

        if(!m_error.isEmpty()) {
            errorStr(SUBROUTINE_ERROR, QString("%1 at %2").arg(m_error).arg(lineNo));
            return {};
        }

        if(!parsed.isEmpty()) {
            //some commands are executed before runtime
            static const QSet<QString> pre({COMPILETIME_COMMANDS});
            for(const auto& cmd : parsed) {
                if(pre.contains(cmd.cmd)) {
                    preCommands.append(cmd);
                } else {
                    const auto lastKey = m_client.resolveName(cmd.cmd);
                    if(lastKey.isEmpty()) {
                        Q_ASSERT(&(routines[currentRoutine]) == commands); //just check commands points to currentRoutine
                        unresolvedCommands.append({currentRoutine, commands->length()}); //store index after the command
                        appendCommand(*commands, cmd);
                    } else if(lastKey == cmd.cmd) {
                        appendCommand(*commands, cmd);
                    } else {
                        auto resolved = cmd;
                        resolved.cmd = lastKey;
                        appendCommand(*commands, resolved);
                    }
                }
            }
        }


        if(m_currentFunction.length() > 1) {
            // subroutine
            if(m_currentFunction.last() != subCall) {
                const auto realName = m_currentFunction.join(GLOBAL_SEP);

                if(!routines.contains(realName)) { //otherwise a nested function and already exists
                    routines.insert(realName, QList<Command>());
                }

                //routines are expexted not to be non-empty
                if(commands->isEmpty()) {
                    commands->append({Keyword("NOP"), {},  {}, lineNo});
                }

                currentRoutine = realName;
                commands = &routines[realName];
                subCall = m_currentFunction.last();
            }
        } else if(!subCall.isEmpty()) {
            currentRoutine = scriptName;
            subCall.clear();
            commands = &routines[scriptName];
        }
    }

    //execute precommands
    for(const auto& preCommand : preCommands) {
        if(!m_client.execute(preCommand))
            return {};
    }

    if(commands->isEmpty()) {
        commands->append({Keyword("NOP"), {},  {}, lineNo});
    }

    //after precommands we may resolve all commands again
    //we do it backwards no to mess indexing
    for(auto it = unresolvedCommands.rbegin(); it != unresolvedCommands.rend(); it++) {
        const auto unresolved = *it;
        auto& unresolvedFunction = routines[unresolved.first];
        auto cmd = &(unresolvedFunction[unresolved.second]);
        const auto resolvedKey = m_client.resolveName(cmd->cmd);
        if(!resolvedKey.isEmpty()) {
            cmd->cmd = resolvedKey;
        } else {
            cmd->params.prepend(cmd->cmd);
            cmd->cmd = "CALL";
            routines[unresolved.first].insert(
                unresolved.second + 1,
            {Keyword("NOP"), {},  std::move(cmd->returnValues), cmd->lineNo});
        }

    }

    auto& s = m_client.subSystems();
    for(const auto& key : aliasNames.keys()) {
        s[key].setName(INVALID_NAME);
        s[key].setName(aliasNames[key]);
    }
    if(m_scopeKeys.length() != scopeDepth) {
        errorStr(UNBALANCED_SCOPE, QString(" at %1").arg(lineNo));
        return {};
    }
    return routines;
}

void QutsParser::clear(){
    m_error = QString();
    m_scopeKeys.clear();
    m_currentFunction.clear();
}
