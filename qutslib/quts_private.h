/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef QUTS_PRIVATE_H
#define QUTS_PRIVATE_H

#include <functional>
#include <limits>
#include <QVector>
#include <QPair>
#include <QMap>
#include <QTimer>
#include <QSet>
#include <QStack>
#include <QTextStream>

class QQmlApplicationEngine;
class QIODevice;
class QQuickWindow;

#include "quts.h"
#include "subsystem.h"
#include "utils.h"
#include "qutsengine.h"
#include "varianthash.h"
#include "subroutine.h"
#include "printer.h"
#include "subsystemdata.h"
#include "scope.h"
#include "qutsparser.h"
#include "index.h"

namespace Quts {

class GuiSignal;

enum {
    FOR_LOOPINCREMENTOR,
    FOR_LOOPINCREMENT,
    FOR_LOOPSIZE,
    FOR_LOOPDATAKEY,
    LAMBDA_CLOSURE
};


class QutsAPIPrivate : public QTextStream, QutsEngine, VariantHash,
    public QutsParserClient {
    using QutsEngine::errorStr;
public:
    QutsAPIPrivate(int flags, QutsAPI* parent);
    ~QutsAPIPrivate();
    bool start(const QString& name, const QVariantMap& memory, const QVariantList& stack);
    bool read(const QString& testName, QIODevice& stream);
    bool free(const QString& testName);
    bool append(const QString& name, const QStringList& cmds);
    void pause();
    bool resume();
    void step();
    void clear(bool eraseCommands);
    int flags() const {return m_flags;}
    void setFlags(int flags) {m_flags = flags;}
    QStringList commandList() const;
    bool isActive() const;
    bool setCommandPosition(const QString& scriptName);
    bool startExecute();
    bool setBreakpoint(const QString& scriptName, int lineNumber, bool set);
    void removeAllBreakpoints();
    QVariantMap snapShot() const;
    bool initValues(std::function<void()> cleanup);
    QStringList helpDocument(const QString& name) const;
    QVariantMap helpDocument(const QString& name, const QString& keyword) const;
private:
    QString current() const {return m_index.current();}
    void setPending();
    bool setVar(VariantHash& vars, const QString& name, const QVariant& value, bool forceScope);
    bool removeVar(VariantHash& vars, const QString& name);
    QVariant getVar(const QString& name, bool* ok);
    bool execute();
    bool syntaxError(const QString& msg, const QString& value);
    bool syntaxError(const QString& msg) {return QutsEngine::syntaxError(msg);}
    QVariant popc(bool* ok = nullptr);
    void inject(const Subsystem* sub, const QString& name, const std::function< bool (const QStringList& value) >& func);
    void makeAlias(const Subsystem* sub, const QString& alias, const QString& name);

    QVariant getValue(const QVariant& value, bool* ok = nullptr, int valueType = NORMALPARAM);
    void quit();
    bool onError(const QStringList& list);
    void pushc(const QVariant& item);
    QTextStream& stream();
    QVariant& operator[](const QString& key) {Q_ASSERT(m_scopes.length() > 0); return m_scopes.first()[key];}
    QVariant operator[](const QString& key) const {Q_ASSERT(m_scopes.length() > 0); return m_scopes.first()[key];}
    void remove(const QString& key) {m_scopes.first().remove(key);}
    bool contains(const QString& key) const {return m_scopes.length() > 0 && m_scopes.first().contains(key);}
    bool finishWaiting(const QString& name, const QStringList& values, const QString& waitCmd);
    int interval() const;
    bool runSubroutine(const QString& name, const QVariantHash& paramValues, const QVariantList& params, const QStringList& rvalues, Cleanup extraClean, bool iterator = false);
    bool endSubroutine(const QStringList& rvalues, int stackCount);
    Index blockParse(const QString& end = QString(), bool up = false, const Index& max = BAD_INDEX) const;
    QStringList reservedWords() const;
    int stackExpectation(const QStringList& values, int pos) ;

    template <typename T> T parameter(const QStringList& values, int index);
    bool doLoad(const QString& name, const QString& fileName);
    bool fromCommandLine(const QStringList& values, int offset, QVariantHash* paramValues, QVariantList* params, QStringList* rvalues);
    bool isToken(const QString& string) const;
    Index lineId() const;
    QString makeGlobalName(const QString& local, const Subsystem* sub) const;
    QString getGlobalName(const QString& local, const Subsystem* sub) const;

    QString resolveName(const QString& key) const;

    QutsAPI* api();
    const ObjectFinder& objectFinder() const;
    QString scriptName() const;
    bool endOfIf();
    bool endOfFor();
    bool execute(const Quts::Command& cmd);
    QHash<Keyword, SubsystemData>& subSystems();
    void errorStr(const std::initializer_list<QString>& p);
    bool callLambda(const Closure& closure, const QVariantList& params, bool iterator, std::function<void (bool iterator)> callback);

    QString dumpCommands() const;

    GUFU(_endOfScope)

    GUFU(error)
    GUFU(import)
    GUFU(load)
    GUFU(let)
    GUFU(pop)
    GUFU(push)
    GUFU(_print)
    GUFU(swap)
    GUFU(eval)
    GUFU(use)
    GUFU(assert)
    GUFU(_for)
    GUFU(help)
    GUFU(call)
    GUFU(_void)
    GUFU(test)
    GUFU(_not)
    GUFU(puke)
    GUFU(nop)
    GUFU(_break)
    GUFU(_if)
    GUFU(_else)
    GUFU(_return)
    GUFU(_typename)
    GUFU(var)
    GUFU(inc)
    GUFU(lambda)

private:
    Q_DECLARE_PUBLIC(QutsAPI)
    QutsAPI* q_ptr;
    CommandList m_commands;
    QStack<QVariant> m_stack;
    QTimer* m_timer;
    Index m_entry;
    Index m_index;
    Index m_lastIndex;
    bool m_running = false;
    QHash<Keyword, std::function< bool (const QStringList& value) >> m_functions;
    Printer m_printer;
    int m_flags = 0;
    QHash<Keyword, SubsystemData> m_subSystems;
    QStack<Scope> m_scopes;
    QSet<Index> m_breakPoints;
    bool m_skipBreakpoint = false;
    QScopedPointer<QutsParser> m_parser;
    QScopedPointer<ObjectFinder> m_objectFinder;
};




}

#endif
