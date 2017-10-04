/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "quts_private.h"
#include "utils.h"
#include "variantutils.h"

#include <QFileInfo>
#include <QRegularExpression>

#include "comp_utils.h"

#include <QDebug>

using namespace Quts;

constexpr char TYPENAME[] = R"(([A-Z]?[a-z]+)$)";

static inline bool compare(const QVariant& v, const QVariant t) {
    return v == t;
}

static QString join(const QVariant& var, const QString& sep = " ") {
    QString s;
    if(var.type() == QVariant::List) {
        s += "[";
        for(const auto& c : var.toList()) {
            s += join(c, sep);
        }
        s += "]";
    } else if(var.type() == QVariant::Map) {
        const auto m = var.toMap();
        s += "(";
        for(auto c = m.cbegin(); c != m.cend(); c++) {
            s += c.key() + ":" + join(c.value(), sep);
        }
        s += ")";
    } else if(var.type() == QVariant::Hash) {
        const auto h = var.toHash();
        s += "{";
        for(auto c = h.cbegin(); c != h.cend(); c++) {
            s += c.key() + ":" + join(c.value(), sep);
        }
        s += "}";
    } else if(var.canConvert(QVariant::String)) {
        s += QString("\"%1\"%2").arg(var.toString()).arg(sep);
    } else {
        s += "N/A";
    }
    return s;
}

QString unquote(const QString& str) {
    if((str.at(0) == '\"' && str.at(str.length() - 1) == '\"') ||
            (str.at(0) == '\'' && str.at(str.length() - 1) == '\'')) {
        return str.mid(1, str.length() - 2);
    }
    return QString();
}

/**DOC
 * N USE
 * import subsystem
 * B name
 * P name of subsystem
 * S CORE
 **/
bool QutsAPIPrivate::use(const QStringList& values) {
    Q_Q(QutsAPI);
    MIN_ARG(1)
    const Keyword ss(values[0]);
    if(!isToken(ss)) {
        syntaxError(INVALID_ARGUMENT, quoted(ss));
        return false;
    }
    const auto name = values.count() > 1 ?
                      values[1] == "\"\"" || values[1] == "\'\'" ? Keyword("") : Keyword(values[1]) : ss;
    if(!static_cast<QString>(name).isEmpty() && !isToken(name)) {
        syntaxError(INVALID_ARGUMENT, quoted(name));
        return false;
    }


    if(!m_subSystems.contains(ss)) {
        QString filePath;
        const auto ptr = create(ss, q, &filePath);
        if(ptr != nullptr) {
            auto params = values;
            params.removeFirst();

            m_subSystems.insert(ss, {ptr, name, filePath, params});

            m_subSystems[ss]->inject();

            for(const auto& k : m_functions.keys()) {
                if(m_functions[k] == nullptr) {
                    return syntaxError(AMBIGUOUS_NAME, quoted(k));
                }
            }
        } else {
            errorStr(QString("%1: %2, is QUTS_PATH environment variable set properly?").arg(INVALID_SUBSYSTEM).arg(quoted(ss)));
            return false;
        }
    } else {
        if(!m_subSystems[ss].setName(Keyword(name))) {
            errorStr(QString("%1: %2").arg(AMBIGUOUS_NAME).arg(quoted(name)));
            return false;
        }
    }

    return true;
}

/**DOC
 * N LOAD
 * as import but not called before the call
 **/
bool QutsAPIPrivate::load(const QStringList& values) {
    MIN_ARG(1)

    const auto filename = getValue(values[0]).toString();

    if(filename.isNull()) {
        return false;
    }

    const auto name =  values.length() > 1 ? getValue(values[1]).toString() :
                       QFileInfo(filename).baseName();

    return doLoad(name, filename);
}

/**DOC
 * Load another script into this script so they share stack and registers.
 * Another script and its subroutines can then be called using CALL
 * N IMPORT
 * B filename <name>
 * P filename that can be used to load script.
 * P name that is used to refer, if not given, a basename of filename
 **/
bool QutsAPIPrivate::import(const QStringList& values) {
    MIN_ARG(1)

    const auto filename = unquote(values[0]);

    if(filename.isEmpty()) {
        syntaxError(INVALID_ARGUMENT, quoted(filename));
        return false;
    }

    const auto name =  values.length() > 1 ? unquote(values[1]) :
                       QFileInfo(filename).baseName();

    if(name.isEmpty() || !isToken(name)) {
        syntaxError(INVALID_ARGUMENT, quoted(name));
        return false;
    }

    return doLoad(name, filename);
}

/**DOC
 * Call subroutine, subroutines are sections starting with &
 * N CALL
 * B name
 **/
bool QutsAPIPrivate::call(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const auto param = getValue(values[0], &ok,  CALLPARAM);
    if(!ok) {
        return false;
    }

    QString routineName;
    Closure closure;

    if(param.type() == QVariant::String) {

        const auto nextscript = param.toString();
        if(!ok) {
            syntaxError(INVALID_ARGUMENT, quoted(values[0]));
            return false;
        }

        routineName = m_commands.contains(nextscript) ?
                      nextscript : Quts::makeGlobalName(nextscript, current());
        if(nextscript.isEmpty() || routineName.isEmpty()) {
            return syntaxError(INVALID_ARGUMENT);
        }

    } else if(param.canConvert<Closure>()) {
        closure = param.value<Closure>();
        routineName = closure.first;
    } else {
        return syntaxError(INVALID_ARGUMENT, values[0]);
    }


    const auto next = m_index + 1;
    Q_ASSERT(next.isValid() && (*next).cmd == "NOP");
    const auto rvalues = (*next).returnValues;
    QVariantHash paramValues;
    QVariantList params;
    if(!fromCommandLine(values, 1, &paramValues, &params, nullptr)) {
        return false;
    }
    const auto success =  runSubroutine(routineName, paramValues, params, rvalues, nullptr);
    if(success && routineName == closure.first) {
        auto& topScope = m_scopes.top();
        for(const auto& k : closure.second.keys()) {
            topScope[k] = closure.second[k];
        }
    }
    return success;
}



/**DOC
 * print out internals
 * N PUKE
 **/
bool QutsAPIPrivate::puke(const QStringList& values) {
    Q_UNUSED(values);
    PRINT << "line:" << (*m_index).lineNo;
    auto scopeCount = 0;
    for(const auto& scope : m_scopes) {
        PRINT << "scope: " << (++scopeCount) << " "
              << (*scope.begin()).lineNo << "-"
              << (*scope.end()).lineNo << " type:" << scope.type();
        PRINT << "memory" << join(scope.dumpMem(), ";");
        PRINT << "data" << join(scope.dumpData(), ";");
    }
    QString str;
    for(const auto& s : m_stack) {
        str.append(join(s, ";"));
    }
    PRINT << "stack: " << str;
    PRINT << "commands" << dumpCommands();
    return true;
}

/**DOC
 * Write value to variables
 * N LET
 * B variableName <value>
 * P variable name
 * P value, if not set a variable is destroyed
 **/
bool QutsAPIPrivate::let(const QStringList& values) {
    MIN_ARG(1)
    return (values.length() > 1) ? setVar(*this, values[0], getValue(values[1]), true)
           : removeVar(*this, values[0]);
}

/**DOC
 * N PUSH
 * Copy values on top of stack
 * B <values>
 * P if empty current value (if exists) is copied on stack.
 * R Pushed values so that first value is on top of stack.
 **/
bool QutsAPIPrivate::push(const QStringList& val) {
    if(val.isEmpty()) { //dup
        if(!m_stack.empty()) {
            m_stack.push(m_stack.top());
        } else {
            return syntaxError(STACK_UNDERFLOW);
        }
    } else {
        for(auto r = val.crbegin() ; r != val.crend(); r++) {
            bool ok;
            const auto value = getValue(*r, &ok);
            if(!ok) {
                syntaxError(INVALID_ARGUMENT, quoted(*r));
                return false;
            }
            m_stack.push(value);
        }
    }
    return true;
}

/**DOC
 * N POP
 **/
bool QutsAPIPrivate::pop(const QStringList& val) {
    bool ok = true;
    if(!val.isEmpty()) {
        const auto key = getValue(val[0], &ok, BAREPARAM).toString();
        if(!ok || key.isEmpty()) {
            return syntaxError(INVALID_ARGUMENT, quoted(val[0]));
        }
        const auto p = popc(&ok);
        if(ok) {
            setVar(m_scopes.top(), key, p, false);
        }
    } else {
        popc(&ok);
    }
    return ok;
}

/**DOC
 * N SWAP
 * Bring nth value on top stack
 * <num>
 * P nth value, if not given swap second on top
 **/
bool QutsAPIPrivate::swap(const QStringList& val) {
    int index = 1;
    if(!val.isEmpty()) {
        if(!num(getValue(val[0]), index)) {
            index = -1;
        }
    }
    if(index >= 0) {
        if(m_stack.count() >= qMax(2, index + 1)) {
            const auto v = m_stack.takeAt(m_stack.count() - (index + 1));
            m_stack.push(v);
        } else { return syntaxError(STACK_UNDERFLOW); }
    } else { return syntaxError(INVALID_ARGUMENT); }
    return true;
}

/**DOC
 * N IF
 * Createas a conditional scope
 * B condition
 **/
bool QutsAPIPrivate::_if(const QStringList& values) {
    MIN_ARG(1)
    const auto blockC = blockParse();
    if(!blockC.isValid()) {
        syntaxError(UNBALANCED_SCOPE);
        return false;
    }

    const auto blockE = blockParse("ELSE", false, blockC);

    bool ok = true;
    const auto val0 = getValue(values[0], &ok);
    if(!ok) {
        syntaxError(INVALID_ARGUMENT, values[0]);
        return false;
    }

    const bool isNot = val0.toString() == NOT_;

    if(isNot) {
        MIN_ARG(2);
    }

    const auto val1 = isNot ? getValue(values[1], &ok) : val0;
    if(!ok) { return false; }
    const auto val2Pos = isNot ? 2 : 1;
    const auto val2 = (values.length() > val2Pos) ? getValue(values[val2Pos], &ok)
                      : popc(&ok);
    if(!ok) { return false; }

    const auto result = (val1 == val2);

    if(result != isNot) {
        if(blockE.isValid()) {
            m_scopes.push({Scope::IF, {m_index + 1, blockE}, blockC});
        } else {
            m_scopes.push({Scope::IF, {m_index + 1, blockC}, blockC});
        }
    } else {
        if(blockE.isValid()) {
            m_index = blockE;
            Q_ASSERT(m_index.isValid());
            m_scopes.push({Scope::IF, { blockE + 1, blockC}, blockC});
        } else {
            //skip this
            m_index = blockC;
            Q_ASSERT(m_index.isValid());
        }
    }
    return true;
}

/**DOC
 * N return
 **/
bool QutsAPIPrivate::_return(const QStringList& value) {
    Q_UNUSED(value);
    while(m_scopes.length() > 1 && !(m_scopes.top().type() == Scope::CALL)) {
        m_scopes.pop();
    }
    Q_ASSERT(m_scopes.top().type() == Scope::CALL);
    Q_ASSERT(m_scopes.length() > 0);
    //Global scope will be cleaned later

    m_index = m_scopes.count() == 1 ? m_scopes.top().outIndex() : m_scopes.pop().outIndex();
    return m_index.isValid();
}


/**DOC
 * N else
 **/
bool QutsAPIPrivate::_else(const QStringList& value) {
    Q_UNUSED(value)
    const auto scope = m_scopes.pop();
    Q_ASSERT(!m_scopes.isEmpty());
    m_index = scope.outIndex();
    Q_ASSERT(m_index.isValid());
    return true;
}


/**DOC
 * Executes parameters as commads
 * N EVAL
 * B <value> <param and return value list>
 * P value is how many lines from stack are consumed as command lines, a list or a string
**/
bool QutsAPIPrivate::eval(const QStringList& values) {
    MIN_ARG(1)
    QStringList lines;
    const auto data = getValue(values[0]);
    bool isInt;
    const auto count = data.toInt(&isInt);
    // Get DATA
    if(isInt) {
        for(auto i = 0; i < count; i++) {
            bool ok;
            const auto p = popc(&ok);
            if(ok && !p.canConvert(QVariant::String)) {
                syntaxError(INVALID_ARGUMENT);
                return false;
            } else {
                lines.append(p.toString());
            }
        }
    } else if(data.canConvert(QVariant::List)) {
        for(const auto& v : data.toList()) {
            if(!v.canConvert((QVariant::String))) {
                syntaxError(INVALID_ARGUMENT);
                return false;
            } else {
                lines.append(v.toString());
            }
        }
    } else if(data.canConvert(QVariant::String)) {
        lines.append(data.toString());
    } else {
        syntaxError(INVALID_ARGUMENT);
    }

    //Generate commands
    //const auto pos = m_commands.length();

    int lineNo = 0;
    QList<Command> commands;
    for(auto line : lines) {
        line.replace(QRegularExpression(R"(\\(.))"), R"(\1)"); //unescape
        commands.append(m_parser->command(line, ++lineNo));
        if(commands.isEmpty()) {
            syntaxError(INVALID_ARGUMENT, line);
            return false;
        }
    }

    //resolve
    for(auto& command : commands) {
        const auto cmd = command.cmd;
        const auto resolvedKey = resolveName(cmd);
        if(resolvedKey.isEmpty()) {
            errorStr(UNKNOWN_COMMAND, QString("%1, line %2").arg(cmd).arg(command.lineNo));
            return false;
        }
        if(resolvedKey != cmd) {
            command.cmd = resolvedKey;
        }
    }

    //Generate unique name
    const auto scriptName = asSysKey("EVAL", qHash(m_index), m_scopes.length());

    if(!m_commands.isEmpty()) {
        const auto likeGlobal = Quts::makeGlobalName(scriptName, current());
        m_commands.insert(likeGlobal, commands);
        Q_ASSERT(m_commands.keys() == m_commands.uniqueKeys());

        const auto cleanup = [this, likeGlobal]()  {
            Q_ASSERT(!likeGlobal.isEmpty());
            if(!free(likeGlobal)) {
                return syntaxError(SUBROUTINE_NAME);
            }
            return true;
        };

        const auto next = m_index + 1;
        Q_ASSERT(next.isValid() && (*next).cmd == "NOP");
        const auto rvalues = (*next).returnValues;

        QVariantHash paramValues;
        QVariantList params;
        if(!fromCommandLine(values, 1, &paramValues, &params, nullptr)) {
            return false;
        }
        return runSubroutine(likeGlobal, paramValues, params, rvalues, cleanup);
    }
    return true;
}

bool QutsAPIPrivate::_break(const QStringList& values) {
    Q_UNUSED(values);
    while(m_scopes.top().type() != Scope::FOR
            && m_scopes.top().type() != Scope::CALL) {
        m_scopes.pop();
    }
    const auto scope = m_scopes.pop();
    m_index = scope.outIndex() - 1;
    Q_ASSERT(m_index.isValid());
    return true;
}

/**DOC
 * N not
 **/
bool QutsAPIPrivate::_not(const QStringList& values) {
    Q_UNUSED(values);
    bool ok;
    const auto v = popc(&ok);
    if(ok) {
        if(compare(v, FALSE_)) {
            pushc(TRUE_);
        } else {
            pushc(FALSE_);
        }
    }
    return true;
}

/**DOC
 * N TEST
 * Compare values
 * B <valueList>
 * P value list, for each value one value is popped from stack and compared, if no values are given it if stack is empty
 * R True if all compares match, otherwise false
 **/
bool QutsAPIPrivate::test(const QStringList& values) {
    if(values.isEmpty()) {
        pushc(m_stack.isEmpty() ? FALSE_ : TRUE_);
    } else {
        auto result = true;
        for(const auto& val : values) {
            const auto item = getValue(val);
            bool ok;
            const auto stackItem = popc(&ok);
            if(!ok || item != stackItem) {
                result = false;
            }
        }
        pushc(result ? TRUE_ : FALSE_);
    }
    return true;
}


/**DOC
 * Empty items from stack
 * N VOID
 * B <value>
 * P value if not available is read from stack set amount of values read from stack.
**/
bool QutsAPIPrivate::_void(const QStringList& values) {

    bool ok = true;
    const auto s  = values.isEmpty() ?
                    popc(&ok) :  getValue(values[0]);

    if(!ok) {
        return false;
    }

    int val;
    if(num(s, val) && val < m_stack.length()) {
        for(auto i = 0; i < val; i++) {
            m_stack.pop();
        }
    } else {
        syntaxError(INVALID_ARGUMENT, values.length() > 0 ? values[0] : "");
        return false;
    }
    return true;
}


/**DOC
 * N ERROR
 **/
bool QutsAPIPrivate::error(const QStringList& values) {
    QString err;
    for(const auto& s : values) {
        bool ok;
        const auto v = getValue(s, &ok);
        if(err.length() > 0) {
            err.append(' ');
        }
        if(ok && v.canConvert(QVariant::String)) {
            err.append(v.toString());
        } else {
            err.append(INVALID_ERROR);
        }

    }
    errorStr(err);
    return true;
}

/**DOC
 * N ASSERT
 **/
bool QutsAPIPrivate::assert(const QStringList& value) {
    const auto assertError = [this]() {
        Q_Q(QutsAPI);
        if(m_index.isValid()) {
            PRINT << ASSERTED << ":" << current() << ", line " << (*m_index).lineNo;
            emit q->error(ASSERTED, current(), (*m_index).lineNo);
            //    if(m_envs[ONSYNTAXERRORQUIT_] == TRUE_)
            quit();
        }
    };
    if(value.isEmpty()) {
        bool ok;
        const auto test = popc(&ok);
        if(!ok || !toBool(test)) {
            assertError();
            return false;
        }
    } else if(value.length() == 1) {
        bool ok;
        const auto test1 = popc(&ok);
        const auto test2 = getValue(value[0]);
        if(!ok || !compare(test1, test2)) {
            assertError();
            return false;
        }
    } else {
        if(!compare(getValue(value[0]), getValue(value[1]))) {
            assertError();
            return false;
        }
    }
    return true;
}

/**DOC
 * N FOR
 * B <count> <index>
 **/
bool QutsAPIPrivate::_for(const QStringList& values) {
    const auto block = blockParse();
    if(!block.isValid()) {
        syntaxError(UNBALANCED_SCOPE);
        return false;
    }

    QHash<int, QVariant> forLoopData;

    //initial value
    bool ok = true;
    const auto count = values.count() > 0 ? getValue(values[0]).toInt(&ok) : std::numeric_limits<int>::max();
    if(!ok) {
        syntaxError(INVALID_ARGUMENT, quoted(values[0]));
        return false;
    }

    forLoopData.insert(FOR_LOOPSIZE, count);

    QString key;

    if(values.count() > 1) {
        key = getValue(values[1], &ok, BAREPARAM).toString();
        if(!ok || key.isEmpty()) {
            syntaxError(INVALID_ARGUMENT);
            return false;
        }
        forLoopData.insert(FOR_LOOPDATAKEY, key);
    }

    auto inc = 1;
    if(values.length() > 2) {
        //decrementor
        bool hasIncrementor;
        inc = getValue(values[2]).toInt(&hasIncrementor);
        if(!hasIncrementor) {
            return syntaxError(INVALID_ARGUMENT, values[2]);
        }
    }

    forLoopData.insert(FOR_LOOPINCREMENTOR, inc);
    forLoopData.insert(FOR_LOOPINCREMENT, 0);

    if(!key.isEmpty()) {
        //ok now we have a scope where we cancreate a variant in scope
        if(!setVar(m_scopes.top(), key, 0, false)) {
            return false;
        }
    }

    //special case count is zero
    if(count == 0) {
        m_index = block;
        Q_ASSERT(m_index.isValid());
        return true;
    }

    m_scopes.push({Scope::FOR, {m_index + 1, block}, block + 1, forLoopData});
    return true;
}

/**DOC
 * Print information, without a parameter prints prints available topics.
 * N HELP
 * B <name>
 * P name of topic spesific information is requested.
**/
bool QutsAPIPrivate::help(const QStringList& values) {
    Q_Q(QutsAPI);
    bool ok = true;
    if(values.length() == 0) {
#ifdef BROKEN_COMPILER
        _print(c_conv<QStringList, QString>({q->help().join(" ")}));
#else
        _print(QStringList({q->help().join(" ")}));
#endif
        return true;
    }


    const Keyword name = values.isEmpty() ? "" : getValue(values[0], &ok, BAREPARAM).toString();
    if(!ok) { return false; }
    const auto keywords = q->help(name);
    if(keywords.isEmpty()) {
        return syntaxError(INVALID_ARGUMENT, quoted(name));
    }

    if(values.length() == 1) {
        _print(QStringList({keywords.join(" ")}));
        return true;
    }

    const auto keyword = getValue(values[1], &ok, BAREPARAM).toString();
    if(!ok) { return false; }
    const auto doc = q->functionHelp(name, keyword);
    if(doc.isEmpty()) {
        return syntaxError(INVALID_ARGUMENT, quoted(keyword));
    }

    auto pos = 2;
    QStringList lines({name,
                       doc["definition"].toString(),
                       "return value:" + doc["returnValue"].toString(),
                       doc["comment"].toString()
                      });
    for(const auto& p : doc["parameters"].toStringList()) {
        lines.insert(pos++, "parameter" + p);
    }
    for(const auto& line : lines)
        _print({QStringList(line)});
    return true;
}

/**DOC
 * N
 **/
bool QutsAPIPrivate::_typename(const QStringList& values) {
    bool ok = true;
    const auto p = values.length() > 0 ? getValue(values[0]) : popc(&ok);
    if(!ok) {
        return false;
    }
    const QRegularExpression reg(TYPENAME);
    const auto m = reg.match(QString(p.typeName()));
    Q_ASSERT(m.hasMatch());
    pushc(m.captured());
    return true;
}

/**DOC
 * N var
 **/
bool QutsAPIPrivate::var(const QStringList& values) {
    MIN_ARG(1)
    return values.length() > 1 ? setVar(m_scopes.top(), values[0], getValue(values[1]), true)
           : removeVar(m_scopes.top(), values[0]);
}


/**DOC
 * N INC
 * B <values>
 * increment by values
 * R result
 **/
bool QutsAPIPrivate::inc(const QStringList& values) {
    bool ok = false;
    const auto sv = popc(&ok);
    if(!ok){
        return false;
    }

    bool numerical;
    auto v0 = sv.toInt(&numerical);
    if(!numerical) {
        const auto s = sv.toString();
        v0 = s.toFloat(&numerical);
        if(!numerical) {
            return syntaxError(INVALID_ARGUMENT);
        }
    }

    if(values.length() == 0) {
        pushc(v0 + 1);
    } else {
        auto vn = 0;
        for(const auto& vString : values) {
            auto vv = getValue(vString);
            auto v = vv.toFloat(&numerical);
            if(!numerical) {
                const auto s = vv.toString();
                v = s.toFloat(&numerical);
                if(!numerical){
                    syntaxError(INVALID_ARGUMENT);
                    return false;
                }
            }
            vn += v;
        }
        pushc(v0 + vn);
    }

    return true;
}

//The implementation of closure is lazy, it just copy
//requested elementts and keeps them with lambda.
//the better way could be keeping scopes as shared resources

bool QutsAPIPrivate::lambda(const QStringList& value) {
    Q_ASSERT(value.length() > 0);
    auto it = value.constBegin();
    const auto lambdaName = Keyword(*it++);
    Q_ASSERT(m_commands.contains((lambdaName)));
    const auto function = m_commands[lambdaName];
    Q_ASSERT(!function.isEmpty()); //there shall be such command list

    Closure closure;
    for(; it != value.end(); it++) {
        const auto var = getValue(*it);
        closure.second[*it] =  var;
    }
    closure.first = lambdaName;

    pushc(QVariant::fromValue(closure));

    return true;
}
