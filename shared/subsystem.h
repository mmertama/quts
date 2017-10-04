/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <functional>
#include <QObject>
#include <QVariant>
#include <QStack>

#include "constants.h"
#include "qutsengine.h"

#ifdef QUTSLIB_EXPORT
# define QUTSLIB_EXPORT_API Q_DECL_EXPORT
#else
# define QUTSLIB_EXPORT_API Q_DECL_IMPORT
#endif


#ifdef Q_OS_WIN
#define QUTS_EXPORT_FUNCTION Q_DECL_EXPORT
#else
#define QUTS_EXPORT_FUNCTION
#endif

namespace Quts {

#define SUBSYSTEM(name) \
    void name ## dummy1() { } \
    constexpr char name ## _NAME[] = #name; \
    extern "C" void* QUTS_EXPORT_FUNCTION name(void* parent, void* engine)


class QUTSLIB_EXPORT_API Subsystem : public QObject {
    Q_OBJECT
public:
    Subsystem(QObject* parent, QutsEngine& engine);
    virtual void inject() = 0;
    virtual QString sysName() const = 0;
    virtual bool isValid() const {return true;} //if this return true, create is not called!
    virtual void create(const QStringList& creationParams);
    virtual void init(const QStringList& creationParams);
    virtual bool isActive() const;
    virtual void clear();
    virtual void tick();
    virtual void close();
    virtual void requestResume(); //request pending command sending true
protected:
    bool errorStr(const QString& message) {return m_engine.errorStr(message);}
    bool num(const QVariant& value, int& out) {return m_engine.num(value, out);}
    QVariant getValue(const QString& value, bool* ok = nullptr, int regName = QutsEngine::NORMALPARAM) {return m_engine.getValue(value, ok, regName);}
    bool syntaxError(const QString& msg, const QString& value = "") {return m_engine.syntaxError(msg, value);}
    QTextStream& stream() {return m_engine.stream();};
    const QVariant& operator[](const QString& key) const {return m_engine[key];}
    bool contains(const QString& key) const {return m_engine.contains(key);}
signals:
    void end();
protected:
    QutsEngine& m_engine;

};

template <class T>
class LoopingSubsystem : public Subsystem {
protected:
    LoopingSubsystem(QObject* parent, QutsEngine& engine) : Subsystem(parent, engine){}
protected:
    struct IInfo {
        Index ipos;
        int index;
        Closure closure;
        std::function<bool (IInfo&) > lambda;
        T data;
    };
    void clear();
    bool loop(const QStringList& values, std::function<bool (IInfo&)> looper);
    bool run();
protected:
    QStack<IInfo> m_iterators;
};

template <class T>
class IterableSubsystem : public LoopingSubsystem<T> {
protected:
    IterableSubsystem(QObject* parent, QutsEngine& engine) : LoopingSubsystem<T>(parent, engine){}
protected:
    bool iterate(const QStringList& values);
};

#include "subsystem_template.h"

}

#endif // SUBSYSTEM_H
