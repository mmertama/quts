/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <QLibrary>
#include <QProcessEnvironment>
#include <QDir>


#include "qutsengine.h"
#include "subsystem.h"
#include "utils.h"

#include <QDebug>

using namespace Quts;

constexpr char QUTS_PATH[] = "QUTS_PATH";
constexpr char HELP_FUNCTION[] = "HELP_FUNCTION";

#define ASSTRING_F(x) #x
#define ASSTRING(x) ASSTRING_F(x)

QutsEngine::QutsEngine() {
    const auto envs = QProcessEnvironment::systemEnvironment();
    if(envs.contains(QUTS_PATH)) {
        //remove newlines....
        m_libraryPath = map<QString, QString>(envs.value(QUTS_PATH).split(';'), [](const QString & p) {return p.trimmed();});
    }
    m_libraryPath += ASSTRING(QUTS_DEFAULT_PATH);
    m_libraryPath += "libs";
    m_libraryPath += ".";
}

QutsEngine::~QutsEngine() {
}

bool QutsEngine::numValue(const QVariant& value, int& out) {
    return num(getValue(value), out);
}


bool QutsEngine::num(const QVariant& value, int& out) {
    bool ok;
    out = value.toInt(&ok);
    if(!ok) {
        syntaxError(NUMBER_NOT_FOUND);
    }
    return ok;
}


HelpData QutsEngine::getHelp(const QString& path) const {
    if(!path.isEmpty() && QLibrary::isLibrary(path)) {
        const auto f = reinterpret_cast<void* (*)() > (QLibrary::resolve(path, HELP_FUNCTION));
        if(!f) {
            qWarning() << "HELP_FUNCTION not defined in" << path;
            return HelpData();
        }
        auto helpPtr = reinterpret_cast<HelpData*>(f());
        if(!helpPtr) {
            qWarning() << "HELP_FUNCTION return nullptr" << path;
            return HelpData();
        }
        HelpData data(*helpPtr);
        delete helpPtr;
        return data;
    }
    return HelpData();
}

QString QutsEngine::findPath(const QString& ss) const {
    QString path;
    findPath(ss, &path);
    return path;
}

FactoryFunction QutsEngine::findPath(const QString& ss, QString* libPath) const {
    for(const auto p : m_libraryPath) {
        const QDir path(p);
        if(path.exists()) {
            const auto entries = path.entryInfoList();
            for(const auto e : entries) {
                if(QLibrary::isLibrary(e.absoluteFilePath())) {
                    const auto f = reinterpret_cast<FactoryFunction>(QLibrary::resolve(e.absoluteFilePath(), ss.toStdString().c_str()));
                    if(f) {
                        *libPath = e.absoluteFilePath();
                        return f;
                    }
                }
            }
        }
    }
    return nullptr;
}

Subsystem* QutsEngine::create(const QString& ss, QObject* parent, QString* filePath) {
    const auto f = findPath(ss, filePath);
    return f ? static_cast<Subsystem*>(f(parent, this)) : nullptr;
}

QString getSubname(const QString& name) {
    const auto pos = name.indexOf('/');
    return name.left(pos);
}

#define PRINT_ALOT

QStringList QutsEngine::scanPath() const {
    QSet<QString> subSystems;
    for(const auto& p : m_libraryPath){
        const QDir path(p);
        if(path.exists()) {
            const auto entries = path.entryInfoList();
            for(const auto& e : entries){
                if(QLibrary::isLibrary(e.absoluteFilePath())){
                    const auto f = reinterpret_cast<void* (*) () > (QLibrary::resolve(e.absoluteFilePath(), HELP_FUNCTION));
#ifdef PRINT_A_LOT
                    qDebug() << "LIB" << e.absoluteFilePath() << f;
#endif
                    if(f) {
                        auto helpPtr = reinterpret_cast<HelpData*>(f());
                        if(!helpPtr) {
                            qWarning() << "HELP_FUNCTION return nullptr" << path;
                        } else {
                            for(const auto& k : helpPtr->keys()) {
                                const auto name = getSubname(k);
                                subSystems.insert(Keyword(name));
                            }
                        }
                    }
                }
            }
        } else {
          //  qWarning() << "Warning: when scanning libraries, a path" << p << "does not exists.";
        }
    }
    if(subSystems.isEmpty()){
        qWarning() << "QUTS_PATH enviroment should point to folders where libs are located, e.g. /Users/<yourpath to developement>/quts/lib/osx/Debug...";
        qWarning() << "Default path is" << m_libraryPath.join(';');
    }
    return subSystems.toList();
}
