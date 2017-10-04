/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <limits>
#include <QRegularExpression>
#include <QFile>
#include <QTimer>
#include <QQmlProperty>
#include <QSet>
#include <QFileInfo>

#include "quts_private.h"
#include "constants.h"
#include "variantutils.h"

#include "comp_utils.h"
#include "objectfinder/objectfinder.h"

#include "quts_doc.h"

#include <QDebug>


using namespace Quts;

constexpr int INTERVAL = 5;
constexpr int MINSPEED = 1;

constexpr char TOKEN[] = "[A-Za-z_][A-Za-z0-9_]*";
constexpr char PRINT_PARAM[] = "\\(\\$(.*?)\\)";
constexpr char COMMAND_LINE[] = "^\\s*(.*?)\\s+(.*)";
constexpr char EOL[] = "[\\n\\r]$";
constexpr char LINE_SCRIPT[] = "??line_script??";
constexpr char GLOBAL_NAME[] = "([A-Za-z_][A-Za-z0-9_]*)\\.([A-Za-z_][A-Za-z0-9_]*)";
constexpr char GLOBAL_PREFIX = '_';
constexpr char NAMED_PARAM[] = R"(([A-Za-z_][A-Za-z0-9_]*)\:(.*))";
constexpr char RETURN_VALUE = '=';

constexpr char SUBROUTINE_[] = "fun";
constexpr char COMMENT_[] = "#";

static const QSet<QString> RESERVED_WORDS({TRUE_, FALSE_, MAXINT_, MININT_, NOT_});

enum {
    CALL_STACKLEN
};

QString Quts::quoted(const QString& v) {
    return QString("\"%1\"").arg(v);
}

QString Quts::makeGlobalName(const QString& local, const QString& current)  {
    if(current.isEmpty() || local == current) { //this is script name, they are always global
        return local;
    }
    const QRegularExpression grex(GLOBAL_NAME);
    const auto localMatch = grex.match(local);
    if(localMatch.hasMatch()) {
        return local;    //already global
    }
    const auto currentMatch = grex.match(current);
    if(currentMatch.hasMatch()) {
        const auto globalPart = currentMatch.captured(1);
        return globalPart + GLOBAL_SEP + local; //strip script name
    } else {
        return current + GLOBAL_SEP + local;    //current is script name
    }
}


#define GUFFP(x) inject(nullptr, Keyword(#x), std::bind(&QutsAPIPrivate::x, this, std::placeholders::_1))
#define GUFFP_ALIAS(x, f) makeAlias(nullptr, Keyword(#x), Keyword(#f))

QString QutsAPIPrivate::makeGlobalName(const QString& local, const Subsystem* sub) const {
    if(sub == nullptr) {
        return Quts::makeGlobalName(local, QString());
    } else {
        const auto pk = local.lastIndexOf(GLOBAL_SEP);
        if(pk >= 0) {
            const auto name = local.left(pk);
            const auto functionName = local.mid(pk + 1);
            return  Quts::makeGlobalName(functionName, sub->sysName());
        } else {
            return Quts::makeGlobalName(local, sub->sysName());
        }
        // return QString("%1:%2").arg(sub->context()).arg(globalName);
    }
}

QString QutsAPIPrivate::getGlobalName(const QString& local, const Subsystem* sub) const {
    const auto globalName = makeGlobalName(local, sub);
    Q_ASSERT(m_functions.contains(globalName)); //should be there
    return globalName;
}


QTextStream& QutsAPIPrivate::stream() {
    Q_Q(QutsAPI);
    if(m_printer.hasData()) {
        m_printer.flush(q, (m_flags & QutsAPI::StdPrint) != 0);
    }
    return m_printer;
}


QString QutsAPI::read(const QUrl& url) {
    const auto fileName = url.toLocalFile();
    return read(fileName);
}

QString QutsAPI::read(const QString& filename) {
    QFile file(filename);
    if(!file.exists()) {
        return QString();
    }
    const auto scriptName = QFileInfo(filename).baseName();
    file.open(QIODevice::ReadOnly);
    auto success = read(file, scriptName);
    file.close();
    return success ? scriptName : QString();
}

bool QutsAPIPrivate::free(const QString& scriptName) {
    if(scriptName.isEmpty()) {
        // m_scripts.clear();
        m_commands.clear();
        clear(true);
        return true;
    }
    auto found = false;
    for(const auto& name : m_commands.keys()) {
        QString key;
        if(name == scriptName) {
            key = name;
        } else {
            const QRegularExpression rxp(GLOBAL_NAME);
            const auto m = rxp.match(name);
            if(m.hasMatch() && (m.captured(1) == scriptName) && m.captured() == name) {
                key = name;
            }
        }
        if(!key.isEmpty()) {
            // const auto script = m_scripts[key];
            // m_scripts.remove(key);
            m_commands.remove(key);
            found = true;
        }
    }
    return found;
}


void QutsAPIPrivate::inject(const Subsystem* sub, const QString& name, const std::function< bool (const QStringList& value) >& func) {
    const auto globalName = makeGlobalName(name, sub);
    Q_ASSERT(!m_functions.contains(globalName));

    m_functions.insert(globalName, func);
    Q_ASSERT(m_functions.keys() == m_functions.uniqueKeys());
    if(sub) {
        m_subSystems[sub->sysName()].append(name);
    }
}

void QutsAPIPrivate::makeAlias(const Subsystem* sub, const QString& alias, const QString& name) {
    const auto globalAliasName = makeGlobalName(alias, sub);
    const auto globalName = getGlobalName(name, sub);
    if(globalName != globalAliasName) {
        Q_ASSERT(!m_functions.contains(globalAliasName));
        //if(m_functions.contains(globalAliasName)){
        //     m_functions[globalAliasName] = nullptr;
        //     return;
        //}
        m_functions.insert(globalAliasName, m_functions[globalName]);
        Q_ASSERT(m_functions.keys() == m_functions.uniqueKeys());
        if(sub) {
            m_subSystems[sub->sysName()].append(alias);
        }
    }
}

void QutsAPIPrivate::setPending() {
    m_running = false;
}

QVariant QutsAPIPrivate::getValue(const QVariant& vvalue, bool* ok, int valueType) {
    bool isNumber;
    if(ok) { *ok = true; }
    vvalue.toDouble(&isNumber);
    if(isNumber) {
        return vvalue;
    }
    static const QRegularExpression reToken(TOKEN);
    if(vvalue.canConvert(QVariant::String)) {
        const auto value = vvalue.toString();
        if(!value.isEmpty()) {
            if(value[0] == '"' && value[value.length() - 1] == '"') {
                return value.mid(1, value.length() - 2);
            } else if(value[0] == '$' && value.length() == 1) {
                return popc(ok);
            } else {
                if((valueType & BAREPARAM) == BAREPARAM && !RESERVED_WORDS.contains(value)) {
                    if(reToken.match(value).captured() == value) {
                        if((valueType & CALLPARAM) == CALLPARAM) {
                            bool thisOk;
                            const auto callName = getVar(value, &thisOk);
                            if(thisOk) {
                                if(ok) { *ok = true; }
                                return callName;
                            }
                        }
                        return value;
                    } else if(value[0] == '$') {
                        const auto key = value.mid(1);
                        return getVar(key, ok);
                    }
                } else {
                    bool okVal;
                    const auto var =  getVar(value, &okVal);
                    if(okVal) {
                        if(ok) { *ok = true; }
                        return var;
                    }
                }
            }
            if(RESERVED_WORDS.contains(value)) {
                if(value == MAXINT_) {
                    return std::numeric_limits<int>::max();
                }
                if(value == MININT_) {
                    return std::numeric_limits<int>::min();
                }
                if(value == TRUE_) {
                    return QVariant::fromValue<bool>(true);
                }
                if(value == FALSE_) {
                    return QVariant::fromValue<bool>(false);
                }
                return vvalue;
            }
            if((valueType & CALLPARAM) == CALLPARAM) {
                auto isCallName = true;
                for(const auto& token : value.split('.')) {
                    if(!(reToken.match(token).hasMatch())) {
                        isCallName = false;
                        break;
                    }
                }
                if(isCallName) {
                    return vvalue;
                }
            } else {
                if(ok) { *ok = false; }
                return QVariant(); //invalid value
            }
        }
    }
    return vvalue;
}


bool QutsAPIPrivate::runSubroutine(const QString& nextscript,
                                   const QVariantHash& paramValues,
                                   const QVariantList& params,
                                   const QStringList& rvalues, Cleanup extraClean, bool iterator) {
    if(!m_commands.contains(nextscript)) {
        return syntaxError(INVALID_ARGUMENT, quoted(nextscript));
    }
    //We wanna jump next only if there is next (and asked)
    const auto nextIndex = m_index + (iterator ? 0 : 1);
    const auto stackDepth = m_stack.length();
    const auto script = m_index(nextscript);

    if(script.isEmpty()) { // nothing to run, empty
        return true;
    }

    Q_ASSERT(script.isValid());
    m_scopes.push(Scope(Scope::CALL, {script, script.end()}, nextIndex, QHash<int, QVariant>(),
    [this, rvalues, stackDepth, extraClean]() {
        //call cleanup
        return
            (extraClean == nullptr || extraClean())
            && endSubroutine(rvalues, stackDepth);
    }));

    m_index = script;
    Q_ASSERT(m_index.isValid());
    m_running = false;

    //uups almost forgot to push params into stack :-)
    for(const auto& p : params) {
        pushc(p);
    }

    //copy param values to scope
    for(const auto& k : paramValues.keys()) {
        setVar(m_scopes.top(), k, paramValues[k], true);
    }

    return true;
}

bool QutsAPIPrivate::endSubroutine(const QStringList& rvalues, int stackDepth) {
    //save return values
    QList<QPair<QString, QVariant>> returnValues;

    if(rvalues.length() > m_stack.length()) {
        errorStr(STACK_UNDERFLOW);
        return false;
    }

    for(const auto& r : rvalues) {
        bool ok;
        returnValues.append({r, popc(&ok)});
        if(!ok) {
            return false;
        }
    }
    //cleaning the stack
    const auto count = m_stack.count() - stackDepth;
    for(auto i = 0; i < count; i++) {
        m_stack.pop();
    }

    if(count < 0) {
        syntaxError(STACK_UNDERFLOW);
        return false;
    }

    m_scopes.pop();

    for(const auto& r : returnValues) {
        if(r.first == "$") {
            pushc(r.second);
        } else {
            const QRegularExpression token(TOKEN);
            const auto m = token.match(r.first);
            if(m.captured() == r.first) {
                m_scopes.top()[r.first] = r.second;
            } else {
                syntaxError(INVALID_ARGUMENT, r.first);
                return false;
            }
        }
    }

    return true;
}



bool QutsAPIPrivate::onError(const QStringList& list) {
    Q_Q(QutsAPI);
    const auto err = list.join(' ');

    if(m_index.isValid()) {
        err + QString(" near line %1").arg((*m_index).lineNo);
    }
    PRINT << "Error: " << err;
    emit q->error(err, current(), m_index.isValid() ? (*m_index).lineNo : 0); // hmm >= 0 does not print console...
   // if(m_envs[ONERRORQUIT_] == TRUE_)
    if(m_running){
        quit();
    } else {
        if(!m_scopes.isEmpty()) {
            qWarning() << "Warning: scopes should be empty";
        }
        m_scopes.clear();
        emit q->activeChanged();
    }
    m_printer.flush(q, (m_flags & QutsAPI::StdPrint) != 0);
    return false;
}

QVariant QutsAPIPrivate::popc(bool* ok) {
    if(!m_stack.isEmpty()) {
        if(ok) { *ok = true; }
        return m_stack.pop();
    } else {
        syntaxError(STACK_UNDERFLOW);
        if(ok) { *ok = false; }
        return QVariant();
    }
}

int QutsAPIPrivate::interval() const {
    return m_timer->interval();
}

bool QutsAPIPrivate::finishWaiting(const QString& routineName, const QStringList& values, const QString& waitCmd) {
    if(!m_index.isValid() || !static_cast<QString>((*m_index).cmd).endsWith(waitCmd)) {
        syntaxError(STRAY_SIGNAL);
        return false;
    }
    // m_sleepTicks = -1;
    if(!routineName.isEmpty()) {
        const auto subname = Quts::makeGlobalName(routineName, current());
        if(!m_commands.contains(subname)) {
            syntaxError(SUBROUTINE_NAME);
            return false;
        }

        QVariantHash paramValues;
        QStringList rvalues;
        QVariantList params;
        if(!fromCommandLine(values, 2, &paramValues, &params, &rvalues)) {
            return false;
        }
        runSubroutine(subname, paramValues, params, rvalues, nullptr);
    } else {
        ++m_index;
    }

    return true;
}



bool QutsAPIPrivate::_print(const QStringList& val) {
    QString value = val.join(' ');
    if(!value.isEmpty()) {
        QRegularExpression re(PRINT_PARAM);
        auto it = re.globalMatch(value);
        if(it.isValid()) {
            QString out;
            int pos = 0;
            while(it.hasNext()) {
                auto m = it.next();
                out.append(value.mid(pos, m.capturedStart() - pos));
                auto key = m.captured(1);
                if(key.isEmpty()) {
                    bool ok;
                    const auto p = popc(&ok);
                    if(ok) {
                        out.append(p.toString());
                    }
                } else {
                    bool ok = false;
                    const auto v = getVar(key, &ok);
                    if(ok) {
                        if(v.canConvert(QVariant::String)) {
                            out.append(v.toString());
                        } else {
                            ok = false;
                        }
                    }
                    if(!ok) {
                        //can I return?
                        syntaxError(INVALID_ARGUMENT, key);
                    }
                }
                pos = m.capturedStart() + m.capturedLength();
            }
            out += value.mid(pos);
            PRINT << out;
        } else {
            PRINT << value;
        }
    }
    return true;
}

QutsAPIPrivate::QutsAPIPrivate(int flags, QutsAPI* parent) : QutsEngine(), q_ptr(parent), m_timer(new QTimer(parent)),
    m_entry(&m_commands), m_index(&m_commands), m_lastIndex(&m_commands), m_flags(flags), m_parser(new QutsParser(*this)), m_objectFinder(new ObjectFinder) {
    GUFFP(error);
    GUFFP(import);
    GUFFP(let);
    GUFFP(pop);
    GUFFP(push);
    GUFFP(_print);
    GUFFP(swap);
    GUFFP(eval);
    GUFFP(use);
    GUFFP(assert);
    GUFFP(_for);
    GUFFP(help);
    GUFFP(load);
    GUFFP(call);
    GUFFP(_void);
    GUFFP(test);
    GUFFP(_not);
    GUFFP(puke);
    GUFFP(nop);
    GUFFP(_break);
    GUFFP(_if);
    GUFFP(_return);
    GUFFP(_else);
    GUFFP(_typename);
    GUFFP(var);
    GUFFP(inc);
    GUFFP(lambda);
    GUFFP(_endOfScope);
    GUFFP_ALIAS(!, _print);
    GUFFP_ALIAS(for, _for);
    GUFFP_ALIAS(void, _void);
    GUFFP_ALIAS(not, _not);
    GUFFP_ALIAS(break, _break);
    GUFFP_ALIAS(if, _if);
    GUFFP_ALIAS(else, _else);
    GUFFP_ALIAS(return, _return);
    GUFFP_ALIAS(typename, _typename);
    GUFFP_ALIAS(;;, _endOfScope);

    QObject::connect(m_timer, &QTimer::timeout, [this]() {
        Q_Q(QutsAPI);
        if(!execute()) {
            m_running = false;
            m_timer->stop();
            while(!m_scopes.isEmpty()) {
                m_scopes.pop().cleanup();
            }
            m_index = -1;
            for(auto&& ss : m_subSystems) {
                ss->close();
            }
            clear(false);
            emit q->activeChanged();
        }
        if(m_running) {
            int val;
            if(!num((*this)[SPEED_], val)) {
                (*this)[SPEED_] = INTERVAL;
            }
            if(val == m_timer->interval()) {
                return;
            }
            if(val <= 0) {
                if(m_timer->interval() == MINSPEED) {
                    return;
                }
                val = MINSPEED;
            }
            m_timer->setInterval(val);
        }
        m_printer.flush(q, (m_flags & QutsAPI::StdPrint) != 0);
    });
    m_timer->setInterval(INTERVAL);
}

bool QutsAPIPrivate::isActive() const {
    return !m_scopes.isEmpty();
}

QutsAPIPrivate::~QutsAPIPrivate() {
}


bool QutsAPIPrivate::execute() {
    Q_Q(QutsAPI);
    Q_ASSERT(m_scopes.length() > 0);
    const auto scope = m_scopes.top();
    if(!m_index.isValid() || m_index > scope.end()) {
        const auto nextIndex = scope.outIndex();
        Q_ASSERT(scope.type() == Scope::CALL);
        if(!nextIndex.isValid()) {
            if(m_scopes.length() == 1) { //exit
                m_scopes.pop();
                Q_ASSERT(m_scopes.isEmpty());
                return false;
            } else if(scope.cleanup()) {
                Q_ASSERT(m_scopes.length() > 0);
                m_scopes.pop();
                Q_ASSERT(m_scopes.isEmpty());
                return false;
            } else {
                return true;
            }
        } else {
            if(!scope.cleanup()) {
                return false;
            }
            m_index = nextIndex;
            Q_ASSERT(m_index.isValid());
        }
    }

    for(auto& ss : m_subSystems) {
        ss->tick();
    }


#ifdef PRINT_A_LOT
    if(m_lastIndex != m_index)

        qDebug() << "execution" << current() << m_index << m_lastIndex <<
                 (m_lastIndex.isValid() ? (QString)(*m_lastIndex).cmd  : "N/A") << (m_index.isValid() ? (QString)(*m_index).cmd : "N/A");
#endif
    const auto command = (*m_index);


    if(m_index != m_lastIndex && contains(DEBUG_) && (*this)[DEBUG_] == TRUE_) {
        PRINT << command.cmd << " " <<  command.params.join(" ");
    }

    if(!m_skipBreakpoint && contains(BREAKPOINTS_) && m_breakPoints.contains(m_index)) {

        if(m_lastIndex != m_index) {
            emit q->breakAt(m_index.current(), command.lineNo);
        }

        m_lastIndex = m_index;
        return true;
    }

    m_skipBreakpoint = false;
    m_lastIndex.clone(m_index);

    m_running = true;

    if(m_functions.contains(command.cmd)) {
        if(!m_functions[command.cmd](command.params)) {
            return false;
        }

        //These return values are not available yet
        if(command.cmd != "NOP"
                && command.cmd != "CALL"
                && command.cmd != "EVAL") {
            for(const auto& rV : command.returnValues) {
                bool ok;
                const auto v = popc(&ok);
                if(!ok) { return false; }
                setVar(m_scopes.top(), rV, v, false);
            }
        }

    } else {
        syntaxError(UNKNOWN_COMMAND, command.cmd);
        return false;
    }
    m_printer.flush(q, (m_flags & QutsAPI::StdPrint) != 0);

    if(m_running) {
        ++m_index;
    }


    return true;
}


bool QutsAPIPrivate::nop(const QStringList& value) {
    Q_UNUSED(value);
    return true;
}






void QutsAPIPrivate::quit() {
    m_running = false;
    while(m_scopes.length() > 1) {
        m_scopes.pop();
    }
    Q_ASSERT(!m_scopes.isEmpty());
    _return(QStringList());
}


#define SYNTAX_ERROR_OUT(current, lineNo, msg, val, file) PRINT << "Syntax error: " << current << ", line " << lineNo << " " << file << "; " << msg << " " << value;

bool QutsAPIPrivate::syntaxError(const QString& msg, const QString& value)  {
    Q_Q(QutsAPI);
    if(!m_index.isValid()) {
        if(!m_running) {
            SYNTAX_ERROR_OUT(value, -1, msg, value, current());
            emit q->error(msg, value, -1);
        }
    } else {
        SYNTAX_ERROR_OUT(current(), ((*m_index).lineNo), msg, value, QutsAPIPrivate::current());
        emit q->error(msg, current(), (*m_index).lineNo);
        quit();

    }
    m_printer.flush(q, (m_flags & QutsAPI::StdPrint) != 0);
    return false;
}

QString QutsAPIPrivate::resolveName(const QString& key) const {
    if(key == END_OF_SCOPE) {
        return END_OF_SCOPE;
    }
    if(m_functions.contains(key)) {
        return key;
    }
    for(const auto& ss : m_subSystems) {
        const auto functionName = ss.functionName(key);
        if(!functionName.isEmpty()) {
            return makeGlobalName(key, ss.data());
        }
    }
    return QString();
}

bool QutsAPIPrivate::append(const QString& name, const QStringList& cmds) {
    const auto parserResult =  m_parser->read(name, [&cmds](int line)->QString{
        return line < cmds.length() ? cmds.at(line) : QString();
    });
    if(parserResult.isEmpty()) {
        return false;
    }
    for(const auto name : parserResult.keys()) {
        if(m_commands.contains(name)) { //names shall not conflict
            syntaxError(AMBIGUOUS_NAME, name);
            return false;
        }
        m_commands.insert(name, parserResult[name]);
    }
    return true;
}


QString QutsAPIPrivate::dumpCommands() const {
    QString out;
    for(const auto& k : m_commands.keys()) {
        const auto l = m_commands[k];
        out.append(k + '\n');
        for(const auto& c : l) {
            out.append("\t" + c.cmd + "<" + c.params.join(',') + ">" + "[" + c.returnValues.join(',') + "]\n");
        }
    }
    return out;
}

bool QutsAPIPrivate::read(const QString& scriptName, QIODevice& stream) {
    Q_Q(QutsAPI);
    const auto parseResult = m_parser->read(scriptName, [&stream, q, &scriptName](int no)->QString{
        const auto s = stream.readLine();
        emit q->lineAdded(scriptName, no, s.endsWith('\n') ?
                          s.mid(0, s.length() - 1) : s);
        return !s.isEmpty() ? s : QString();
    });
    if(parseResult.isEmpty()) {
        return false;
    }
    for(const auto name : parseResult.keys()) {
        if(m_commands.contains(name)) { //names shall not conflict
            syntaxError(AMBIGUOUS_NAME, name);
            return false;
        }
        m_commands.insert(name, parseResult[name]);
    }
#ifdef PRINT_A_LOT
    qDebug().noquote() << dumpCommands();
#endif
    return true;
}


void QutsAPIPrivate::pause() {
    if(m_timer->isActive()) {
        m_timer->stop();
    }
}

bool QutsAPIPrivate::resume() {
    if(m_index == m_lastIndex && contains(BREAKPOINTS_) && m_breakPoints.contains(m_index)) {
        m_skipBreakpoint = true;
    }
    if(m_index.isValid() && !m_timer->isActive()) {
        startExecute();
        return true;
    }
    return false;
}

bool QutsAPIPrivate::startExecute() {
    Q_Q(QutsAPI);
    m_running = true;
    int val;
    const auto ok = num((*this)[SPEED_], val);
    emit q->activeChanged();
    Q_ASSERT(m_index.isValid());
    m_timer->start(ok ? (val > 0 ? val : MINSPEED) : INTERVAL);
    return true;
}

void QutsAPIPrivate::step() {
    pause();
    QTimer::singleShot(1, [this]() {
        execute();
    });
}


bool QutsAPIPrivate::start(const QString& scriptName, const QVariantMap& memory, const QVariantList& stack) {
    if(!m_commands.contains(scriptName)) {
        return false;
    }


    m_index = m_index(scriptName);
    m_entry = m_index;

    if(!initValues(nullptr)) {
        m_index.invalidate();
        return false;
    }
    startExecute();
    for(const auto& key : memory.keys()) {
        setVar(*this, key, memory[key], true);
    }
    for(const auto& value : stack) {
        pushc(value);
    }
    return true;
}


bool variantAppend(QList<QVariant>& vlist, const QVariant& v) {
    for(const auto& vv : vlist) {
        if(vv == v) {
            return false;
        }
    }
    vlist.append(v);
    return true;
}

void findObjects(QObject* obj, const QString& name, QSet<QObject*>& objects, QSet<QObject*>& traversed);

void findObjectsFromVariant(const QVariant& value, const QString& name, QSet<QObject*>& objects, QSet<QObject*>& traversed, QList<QVariant>& vlist) {
    if(value.canConvert<QList<QVariant>>()) {
        const auto lst = value.toList();
        for(const auto& v : lst) {
            findObjectsFromVariant(v, name, objects, traversed, vlist);
        }
    } else if(value.canConvert<QHash<QString, QVariant>>()) {
        const auto hash = value.toHash();
        for(const auto& v : hash) {
            findObjectsFromVariant(v, name, objects, traversed, vlist);
        }

    } else if(value.canConvert<QObject*>()) {
        auto obj = value.value<QObject*>();
        if(obj) {
            findObjects(obj, name, objects, traversed);
        }
    }

}

void findObjects(QObject* obj, const QString& name, QSet<QObject*>& objects, QSet<QObject*>& traversed) {
    if(traversed.contains(obj)) {
        return;
    }
    traversed.insert(obj);

    if(obj->objectName() == name) {
        objects.insert(obj);
    }


    for(const auto& chld : obj->children()) {
        findObjects(chld, name, objects, traversed);
    }

    QList<QVariant> vlist;
    const auto meta = obj->metaObject();
    for(auto i = 0; i < meta->propertyCount(); i++) {
        const auto prop = meta->property(i);
        if(prop.isValid()) {
            const auto propName = prop.name();
            auto value = QQmlProperty::read(obj, propName);
            findObjectsFromVariant(value, name, objects, traversed, vlist);
        }
    }
}



Index QutsAPIPrivate::blockParse(const QString& eobStr, bool up, const Index& max) const {
    const auto eob = eobStr.isEmpty() ? END_OF_SCOPE : eobStr;
    const auto end = max.isValid() ? max : up ? m_index(0) : m_index.end();
    const auto inc = up ? -1 : 1;
    auto fors = 1;
    auto index = m_index;
    //const auto low = m_scripts[name].first;
    //const auto high = m_scripts[name].second;
    for(;;) {
        index += inc;

        const auto cc = Keyword((*index).cmd);
        if(SCOPE_WORDS.contains(cc)) {
            fors += inc;
        }
        if(cc == eob) {
            fors -= inc;
        }
        if(fors == 0) {
            return index;
        }
        if(index == end) {
            break;
        }
    }
    return BAD_INDEX;
}




void QutsAPIPrivate::pushc(const QVariant& item) {
    m_stack.push_back(item);
}

void QutsAPIPrivate::clear(bool eraseCommands) {
    Q_Q(QutsAPI);
    m_timer->stop();
    m_index = 0;
//   m_currentScript.clear();
//   m_nextStack.clear();
//   m_scope.clear();
    m_scopes.clear();
    for(auto& s : m_subSystems) {
        s->clear();
    }

    if(m_parser)
        m_parser->clear();

    //m_scripts.clear();
    m_stack.clear();
    m_skipBreakpoint = false;
    if(eraseCommands) {
        m_commands.clear();
    }
    emit q->activeChanged();
}

bool QutsAPIPrivate::initValues(std::function<void()> cleanup) {
    Q_Q(QutsAPI);
    Q_ASSERT(m_scopes.isEmpty());
    const auto currentScript = current();
    m_scopes.push(Scope(Scope::CALL, {m_index.begin(), m_index.end()}, BAD_INDEX, QHash<int, QVariant>(),
    [this, currentScript, q, cleanup]()->bool{
        bool hasActives = false;
        m_running = false;
        for(auto ss : m_subSystems) {
            if(ss->isActive()) {
                hasActives = true;
                QObject::connect(ss.data(), &Subsystem::end, [this, currentScript, q, cleanup]() {
                    for(auto ss : m_subSystems) {
                        if(ss->isActive()) {
                            return false;
                        }
                    }
                    if(cleanup != nullptr) {
                        cleanup();
                    }
                    emit q->end(currentScript);
                    return true;
                });
            }
        }
        if(!hasActives)
            emit q->end(currentScript);
        ++m_index;
        return true;
    }));

    (*this)[SPEED_] = (INTERVAL);

    for(const auto& ss : m_subSystems.keys())
        if(!m_subSystems[ss].create()) {
            return syntaxError(NOT_READY, quoted(ss));
        }
//   m_envs[ONERRORQUIT_] = TRUE_;
//   m_envs[ONSYNTAXERRORQUIT_] = TRUE_;
    return true;
}

QStringList QutsAPIPrivate::commandList() const {
    return map<QString, Keyword>(m_functions.keys(), [](const Keyword & k)->QString{return k;});
}


bool QutsAPIPrivate::doLoad(const QString& name, const QString& filename) {

    if(name.isEmpty()) {
        return syntaxError(INVALID_ARGUMENT);
    }

    if(m_commands.contains(name)) {
        return true;    //already loaded
    }

    QFile file(filename);
    if(!file.exists()) {
        return syntaxError(INVALID_ARGUMENT, quoted(filename));
    }

    file.open(QIODevice::ReadOnly);
    const auto success = read(name, file);
    file.close();

    for(const auto& ss : m_subSystems.keys())
        if(!m_subSystems[ss].create()) {
            return syntaxError(NOT_READY, quoted(ss));
        }

    return success;
}

bool QutsAPIPrivate::fromCommandLine(const QStringList& values,
                                     int offset,
                                     QVariantHash* paramValues,
                                     QVariantList* params,
                                     QStringList* rvalues) {
    for(auto i = offset;  i < values.length(); i++) {
        const auto& p = values[i];
        if(p.at(0) == RETURN_VALUE) {
            rvalues->append(p.mid(1));
        } else {
            bool ok = true;
            auto getVal = [this, &ok](const QString & p)->QVariant{
                const auto v = getValue(p, &ok);
                if(!ok) {
                    syntaxError(INVALID_ARGUMENT, p);
                }
                return v;
            };
            QRegularExpression reg(NAMED_PARAM);
            const auto m = reg.match(p);
            if(m.hasMatch()) {
                const auto name = m.captured(1);
                const auto valueName = m.captured(2);
                const auto value = getVal(valueName);
                if(!ok) { return false; }
                paramValues->insert(name, value);
            } else {
                const auto value = getVal(p);
                if(!ok) { return false; }
                params->prepend(value);
            }
        }
    }
    return true;
}

bool QutsAPIPrivate::isToken(const QString& string) const {
    const QRegularExpression token(TOKEN);
    return token.match(string).captured() == string;
}

bool QutsAPIPrivate::removeVar(VariantHash& vars, const QString& name) {
    bool ok = true;
    const auto key = (name[0] == GLOBAL_PREFIX) ? name.mid(1)
                     : getValue(name, &ok, BAREPARAM).toString();

    if(!ok || key.isEmpty()) {
        return syntaxError(INVALID_ARGUMENT);
    }

    if(vars.contains(key)) {
        vars.remove(key);
    }

    return true;
}

bool QutsAPIPrivate::setVar(VariantHash& vars, const QString& name, const QVariant& value, bool forceScope) {
    bool ok = true;
    const auto key = (name[0] == GLOBAL_PREFIX) ? name.mid(1)
                     : getValue(name, &ok, BAREPARAM).toString();

    if(!ok || key.isEmpty()) {
        return syntaxError(INVALID_ARGUMENT);
    }

    if(!value.isValid()) {
        return syntaxError(INVALID_ARGUMENT, quoted(value.toString().isEmpty() ?
                           name : value.toString()));
    }

    if(!forceScope) {
        auto scopeIndex = m_scopes.length() - 1;
        while(scopeIndex >= 0) {
            if(m_scopes[scopeIndex].contains(key)) {
                m_scopes[scopeIndex][key] = value;
                return true;
            }
            --scopeIndex;
        }
    }

    vars[key] = value;

    return true;
}

QVariant QutsAPIPrivate::getVar(const QString& name, bool* ok) {
    if(name.isEmpty()) {
        if(ok != nullptr) { *ok = false; }
        return QVariant();
    }
    const auto getGlobal = [this](const QString & key, bool * ok)->QVariant{
        if(contains(key)) {
            if(ok != nullptr) { *ok = true; }
            return (*this)[key];
        }
        if(ok != nullptr) { *ok = false; }
        return QVariant();
    };

    if(name[0] == GLOBAL_PREFIX) {
        return getGlobal(name.mid(1), ok);

    }

    for(auto ii = m_scopes.length() - 1; ii >= 0; ii--) {
        if(m_scopes[ii].contains(name)) {
            if(ok != nullptr) { *ok = true; }
            return m_scopes[ii][name];
        }
    }
    return getGlobal(name, ok);
}

Index QutsAPIPrivate::lineId() const {
    Q_ASSERT(m_index.isValid());
    return m_index;
}

bool QutsAPIPrivate::setCommandPosition(const QString& scriptName) {
    if(m_commands.contains(scriptName)) {
        m_index.init(scriptName);
        return true;
    }
    return false;
}

bool QutsAPIPrivate::setBreakpoint(const QString& scriptName, int lineNumber, bool set) {
    if(!m_commands.contains(scriptName)) {
        qWarning() << scriptName << "not found when setting breakpoint";
        return false;
    }


    int index = -1;
    QString name;

    //We reverse sort names as that as child functions are matched first
    auto keys = map<QString, Keyword>(m_commands.keys(), [](const Keyword & k)->QString{return k;});
    keys.sort();
    std::reverse(keys.begin(), keys.end());

    for(const auto& sn : keys) {
        const QString sname = sn;
        if(sname.compare(scriptName, Qt::CaseInsensitive) == 0
                || sname.startsWith(scriptName + ".", Qt::CaseInsensitive)) {

            const auto script = m_commands[sn];

            if(script.length() > 0
                    && script.first().lineNo <= lineNumber
                    && script.last().lineNo >= lineNumber) {
                const auto p = std::lower_bound(script.begin(), script.end(),
                Command({QString(), {}, {}, lineNumber}), [](const Command & a, const Command & b) {
                    return a.lineNo < b.lineNo;
                });
                if(p != script.end()) {
                    index = static_cast<int>(std::distance(script.begin(), p));
                    name = sname;
                    break;
                }
            }
        }
    }


    if(index < 0) {
        return false;
    }

    if(!m_index.isValid()) { //actual issue is BAD_INDEX after syntaxError
        m_index = Index(&m_commands);
    }

    const auto bkPos = m_index(name, index);
    if(set) {
        m_breakPoints.insert(bkPos);
    } else {
        m_breakPoints.remove(bkPos);
    }
    return true;
}

VariantHash::~VariantHash() {
}

QVariantMap QutsAPIPrivate::snapShot() const {
    QVariantMap snap;
    snap.insert("line", m_index.isValid() ?
                (*m_index).lineNo : -1);
    QVariantList scopes;
    for(const auto& scope : m_scopes) {
        scopes.append(QVariantMap({
            {"begin", (*scope.begin()).lineNo},
            {"end", (*scope.end()).lineNo},
            {"type", scope.type()},
            {"memory", scope.dumpMem()},
            {"data", scope.dumpData()}
        }));
    }
    snap.insert("scopes", scopes);
    auto lst = m_stack.toList();
    for(auto it = lst.begin(); it != lst.end(); it++) {
        if(!it->canConvert(QVariant::String)) {
            *it = QString("<%1>").arg(it->typeName());
        }
    }
    std::reverse(lst.begin(), lst.end());
    snap.insert("stack", lst);
    return snap;
}

QutsAPI* QutsAPIPrivate::api() {
    Q_Q(QutsAPI);
    return q;
}

const ObjectFinder& QutsAPIPrivate::objectFinder() const {
    return *m_objectFinder;
}

static QStringList toNameList(const QString& name, const QStringList& longList) {
    QStringList list;
    for(const auto& s : longList) {
        const auto p = s.indexOf('/');
        if(s.left(p) == name) {
            list.append(s.mid(p + 1));
        }
    }
    return list;
}


void QutsAPIPrivate::removeAllBreakpoints() {
    m_skipBreakpoint = false;
    m_breakPoints.clear();
}

QStringList QutsAPIPrivate::helpDocument(const QString& name) const {
    if(name == "CORE") {
        return toNameList("CORE", qutsDoc.keys());
    }

    if(name.isEmpty()) {
        auto list = scanPath();
        list.append("CORE");
        return list;
    }
    return toNameList(name, m_subSystems.contains(name) ?
                      getHelp(m_subSystems[name].libraryName()).keys() :
                      getHelp(findPath(name)).keys());
}

QVariantMap QutsAPIPrivate::helpDocument(const QString& name, const QString& keyword) const {
    if((name.isEmpty() || name == "CORE") && qutsDoc.contains(keyword)) {
        return qutsDoc[keyword];
    }
    if(m_subSystems.contains(name)) {
        const auto helpData = getHelp(m_subSystems[name].libraryName());
        if(helpData.contains(keyword)) {
            return helpData[keyword];
        }
    } else {
        const auto helpData = getHelp(findPath(name));
        const auto key = Keyword(name + '/' + keyword);
        if(helpData.contains(key)) {
            return helpData[key];
        }
    }
    return QVariantMap();
}

QString QutsAPIPrivate::scriptName() const {
    return m_entry.current();
}

bool QutsAPIPrivate::endOfFor() {
    auto scope = m_scopes.pop();
    const auto inc = scope[FOR_LOOPINCREMENTOR].toInt();
    const auto count = scope[FOR_LOOPINCREMENT].toInt();
    const auto size = scope[FOR_LOOPSIZE].toInt();

    const auto result = count + inc;

    scope[FOR_LOOPINCREMENT] = result;

    if(scope.contains(FOR_LOOPDATAKEY)) {
        setVar(scope, scope[FOR_LOOPDATAKEY].toString(), result, false);
    }

    if(result == size) {
        m_index = scope.outIndex() - 1;
        Q_ASSERT(m_index.isValid());
        Q_ASSERT(!m_scopes.isEmpty());
    } else {
        m_scopes.push(scope); //put it back
        m_index = scope.begin() - 1;
        Q_ASSERT(m_index.isValid()); //hmm this was m_index >= -1, so it could be -1 meaning, after inc its zero? maybe this broke it
    }

    return true;
}

bool QutsAPIPrivate::endOfIf() {
    m_scopes.pop();
    Q_ASSERT(!m_scopes.isEmpty());
    return true;
}

bool QutsAPIPrivate::_endOfScope(const QStringList& v) {
    Q_UNUSED(v);
    const auto scope = m_scopes.top();
    switch(scope.type()) {
    case Scope::FOR:
        endOfFor();
        return true;
    case Scope::IF:
        endOfIf();
        return true;
    default:
        syntaxError(UNBALANCED_SCOPE);
        return false;
    }
}

bool QutsAPIPrivate::execute(const Quts::Command& cmd) {
    if(m_functions.contains(cmd.cmd)) {
        if(!m_functions[cmd.cmd](cmd.params)) {
            return false;
        }

    } else {
        errorStr(QString("%1: %2 at %3").arg(UNKNOWN_COMMAND).arg(cmd.cmd).arg(cmd.lineNo));
        return false;
    }
    return true;
}

QHash<Keyword, SubsystemData>& QutsAPIPrivate::subSystems() {
    return m_subSystems;
}

void QutsAPIPrivate::errorStr(const std::initializer_list<QString>& p) {
    QutsEngine::errorStr(p);
}

bool QutsAPIPrivate::callLambda(const Closure& closure, const QVariantList& params, bool iterator, std::function<void (bool iterator)> callback) {
    QVariantHash paramValues;
    QStringList rvalues;
    const auto success = runSubroutine(closure.first, paramValues, params, rvalues, [iterator, callback]() {
        if(callback)
            callback(iterator);
        return true;
    }, iterator);
    if(success) {
        auto& topScope = m_scopes.top();
        for(const auto& k : closure.second.keys()) {
            topScope[k] = closure.second[k];
        }
    }
    return success;
}



