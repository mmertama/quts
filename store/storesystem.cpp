/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "storesystem.h"
#include "quts_doc.h"
#include <QSettings>

using namespace Quts;

SUBSYSTEM(STORE) {
    return new StoreSystem((QObject*)parent,  * (QutsEngine*) engine);
}

StoreSystem::StoreSystem(QObject* parent, QutsEngine& engine) :  Subsystem(parent, engine) {
}

void StoreSystem::inject() {
    GUFF(StoreSystem, write);
    GUFF(StoreSystem, read);
    GUFF(StoreSystem, contains);
    GUFF(StoreSystem, container);
}

StoreSystem::~StoreSystem() {
}

QString StoreSystem::sysName() const {
    return STORE_NAME;
}

void StoreSystem::create(const QStringList& creationParams) {
    Q_UNUSED(creationParams);
    if(m_settings == nullptr) {
        m_settings.reset(new  QSettings(QSettings::IniFormat,
                                        QSettings::UserScope,
                                        "Quts"));
    }
}

void StoreSystem::close() {
    Q_ASSERT(m_settings != nullptr);
    m_settings->sync();
}

/**DOC
 * N container
 * S Store
 **/
bool StoreSystem::container(const QStringList& values) {
    Q_UNUSED(values)
    m_engine.pushc(m_settings->fileName());
    return true;
}


/**DOC
 * N contains
 * S Store
 **/
bool StoreSystem::contains(const QStringList& values) {
    MIN_ARG(1)
    bool ok;
    const auto keyVal = m_engine.parameter<QString>(values, 0, &ok, QutsEngine::BAREPARAM);
    if(!ok) { return false; }
    const auto key = keyVal.contains('/') ? keyVal : m_engine.scriptName() + '/' + keyVal;
    const auto v = m_settings->contains(key);
    m_engine.pushc(v);
    return true;
}

/**DOC
 * N write
 * S Store
 **/
bool StoreSystem::write(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const auto keyVal = m_engine.parameter<QString>(values, 0, &ok, QutsEngine::BAREPARAM);
    if(!ok) { return false; }
    const auto val = (values.length() == 1) ? m_engine.popc(&ok) :
                     getValue(values.last(), &ok);
    if(!ok) { return false; }
    const auto key = keyVal.contains('/') ? keyVal : m_engine.scriptName() + '/' + keyVal;
    m_settings->setValue(key, val);
    return true;
}

/**DOC
 * N read
 * S Store
 **/
bool StoreSystem::read(const QStringList& values) {
    MIN_ARG(1);
    bool ok;
    const auto keyVal = m_engine.parameter<QString>(values, 0, &ok, QutsEngine::BAREPARAM);
    if(!ok) { return false; }
    const auto key = keyVal.contains('/') ? keyVal : m_engine.scriptName() + '/' + keyVal;
    const auto val = m_settings->value(key);
    if(!val.isValid()) {
        return syntaxError(INVALID_OBJECT, quoted(key));
    }
    m_engine.pushc(val);
    return true;
}
