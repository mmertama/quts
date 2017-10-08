/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "quts_private.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDebug>

using namespace Quts;

QutsAPI::QutsAPI(QQmlApplicationEngine* engine, int flags, QObject* parent) :
    QObject(parent),
    d_ptr(new QutsAPIPrivate(flags, this)) {
    if(engine) {
        engine->rootContext()->setContextProperty("QutsAPI", this);
    }
    QObject::connect(this, &QutsAPI::breakAt, this, [this](const QString&, int) {
        emit snapShotChanged();
    });
    QObject::connect(this, &QutsAPI::error, this, [this](QString, QString, int) {
        emit snapShotChanged();
    });
}


QutsAPI::~QutsAPI() {

}

bool QutsAPI::start(const QString& name, const QVariantMap& memory, const QVariantList& stack) {
    Q_D(QutsAPI);
    return d->start(name, memory, stack);
}

bool QutsAPI::read(QIODevice& stream, const QString& scriptName) {
    Q_D(QutsAPI);
    d->free("");
    return d->read(scriptName, stream);
}

bool QutsAPI::execute(const QString& scriptName, const QStringList& data) {
    Q_D(QutsAPI);
    auto plist = data;
    for(auto& p : plist) {
        p.append(" ");
    }
    if(d->append(scriptName, plist)) {
        if(d->setCommandPosition(scriptName) && d->initValues([d, scriptName]() {
            d->free(scriptName);
        })) {
            d->startExecute();
            return true;
        }
    }
    return false;
}

void QutsAPI::pause() {
    Q_D(QutsAPI);
    d->pause();
}

bool QutsAPI::resume() {
    Q_D(QutsAPI);
    return d->resume();
}

void QutsAPI::step() {
    Q_D(QutsAPI);
    d->resume();
}

void QutsAPI::clear() {
    Q_D(QutsAPI);
    d->clear(true);
}

QVariantMap QutsAPI::functionHelp(const QString& name, const QString& sub) const {
    Q_D(const QutsAPI);
    return d->helpDocument(name, sub);
}

QStringList QutsAPI::help(const QString& system) const {
    Q_D(const QutsAPI);
    auto list = d->helpDocument(system);
    list.sort();
    return list;
}


bool QutsAPI::active() const {
    Q_D(const QutsAPI);
    return d->isActive();
}

bool QutsAPI::setBreakpoint(const QString& scriptName, int lineNumber, bool set) {
    Q_D(QutsAPI);
    return d->setBreakpoint(scriptName, lineNumber, set);
}

QVariantMap QutsAPI::snapShot() const {
    Q_D(const QutsAPI);
    return d->snapShot();
}

void QutsAPI::removeAllBreakpoints() {
    Q_D(QutsAPI);
    d->removeAllBreakpoints();
}

void QutsAPI::stop(){
    Q_D(QutsAPI);
    d->clear(false);
}


bool QutsAPI::isPrintStd() const {
    Q_D(const QutsAPI);
    return d->flags() & StdPrint;
}

void QutsAPI::setIsPrintStd(const bool isPrint) {
    Q_D(QutsAPI);
    if(isPrint != isPrintStd()) {
        d->setFlags(isPrint ? d->flags() | StdPrint : d->flags() & ~StdPrint);
        emit isPrintStdChanged();
    }
}
