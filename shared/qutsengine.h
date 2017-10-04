/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef QUTSENGINE_H
#define QUTSENGINE_H

#include <QVariant>
#include "constants.h"
#include "index.h"


class QTextStream;

namespace Quts {

class Subsystem;
class ObjectFinder;
class QutsAPI;

typedef QHash<QString, QMap<QString, QVariant>> HelpData;
typedef void* (*FactoryFunction)(void* parent, void* engine);

typedef QPair<QString, QVariantHash> Closure;

class QUTSLIB_EXPORT_API QutsEngine {
public:
    enum {
        NORMALPARAM = 0,
        BAREPARAM = 1,
        CALLPARAM = 3 //also has REGPARAMTYPE
    };
    QutsEngine();
    virtual ~QutsEngine();
    virtual QVariant popc(bool* ok = nullptr) = 0;
    virtual void pushc(const QVariant& value) = 0;
    virtual QVariant getValue(const QVariant& value, bool* ok = nullptr, int valueType = NORMALPARAM) = 0;
    virtual bool syntaxError(const QString& msg, const QString& value) = 0;
    virtual void inject(const Subsystem* sub, const QString& name, const std::function< bool (const QStringList& value) >& func) = 0;
    virtual void makeAlias(const Subsystem* sub, const QString& alias, const QString& name) = 0;
    virtual QTextStream& stream() = 0;
    virtual bool contains(const QString& key) const = 0;
    virtual QVariant& operator[](const QString& key) = 0;
    virtual QVariant operator[](const QString& key) const = 0;
    virtual bool finishWaiting(const QString& label, const QStringList& values, const QString& waitCmd) = 0;
    virtual int interval() const = 0;
    virtual void setPending() = 0;
    virtual Index lineId() const = 0;
    virtual QutsAPI* api() = 0;
    virtual const ObjectFinder& objectFinder() const = 0;
    virtual QString scriptName() const = 0;
    virtual QString current() const = 0;
    virtual bool callLambda(const Closure& closure, const QVariantList& params, bool iterator, std::function<void (bool iterator)> callback) = 0;
public:
    template<typename...Args>
    bool errorStr(Args...args) {
        QStringList lst = {args...};
        return onError(lst);
    }
    bool numValue(const QVariant& value, int& out);
    bool num(const QVariant& value, int& out);
    //NOTE: Change to this!
    template <typename T>
    T parameter(const QStringList& values, int index, bool* ok, int valueType = NORMALPARAM) {
        const auto cmd = values.length() > index ?
                         getValue(values[index], ok, valueType) : popc(ok);
        if(!cmd.canConvert<T>()) {
            syntaxError(INVALID_ARGUMENT, values.length() > index ? values[index] : QString());
            if(ok) {
                *ok = false;
            }
            return T();
        }
        if(ok) {
            *ok = true;
        }
        return cmd.value<T>();
    }

    bool syntaxError(const QString& msg) {return syntaxError(msg, "");}
protected:
    HelpData getHelp(const QString& name) const;
    QStringList scanPath() const;
    QString findPath(const QString& name) const;
    Subsystem* create(const QString& subsystem, QObject* parent, QString* fileName);
    virtual bool onError(const QStringList& list) = 0;
private:
    FactoryFunction findPath(const QString& name, QString* libPath) const;
private:
    QStringList m_libraryPath;
};

}

Q_DECLARE_METATYPE(Quts::Closure)


#endif // QUTSENGINE_H
