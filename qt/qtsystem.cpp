/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */


#include <QTest>

#include <QRegularExpression>
#include <QScopedPointer>

#include <QQuickItem>
#include <QQuickWindow>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQmlContext>

#include "qtsystem.h"
#include "signalreceiver.h"
#include "constants.h"
#include "../quts.h"
#include "../objectfinder/objectfinder.h"
#include "guisignal.h"
#include "quts_doc.h"

using namespace Quts;

SUBSYSTEM(QT) {
    return new QtSystem((QObject*)parent,  * (QutsEngine*) engine);
}



#define GUFFX(x) GUFF(QtSystem, x)

constexpr char SIGNAL_FUNCTION_MATCH[] = "\\(\\s*(int|string|)(?:\\s+.*)*\\)";
//same as QLOCATION, but it is not always defined
#define LOCATION "\0" __FILE__ ":" QT_STRINGIFY(__LINE__)

QtSystem::QtSystem(QObject* parent, QutsEngine& engine) : Subsystem(parent, engine) {
}

QString QtSystem::sysName() const {
    return QT_NAME;
}

void QtSystem::inject() {
    GUFFX(child);
    GUFFX(parent);
    GUFFX(wait);
    GUFFX(click);
    GUFFX(write);
    GUFFX(slot);
    GUFFX(find);
    GUFFX(set);
    GUFFX(get);
    GUFFX(signal);
    GUFFX(item);
    GUFFX(fid);
    GUFFX(qutsSignal);
}

QtSystem::~QtSystem() {
}


bool QtSystem::wait(const QStringList& value) {
    if(m_signals.count() <= 0) {
        errorStr(NO_SLOTS_SET);
        return false;
    }
    if(m_waitTicks < 0) {
        bool success = false;
        for(auto it = m_signals.begin(); it != m_signals.end() ; it++) {
            auto sig = *it;
            if(!sig->pendingLine.isValid()) {
                if(sig->function(sig)) {//then connect
                    success = true;
                    sig->pendingLine = m_engine.lineId();
                } else {
                    errorStr(SLOT_ERROR);
                    return false;
                }
            }
        }
        if(!success) {
            if(!m_waitPenging) {
                m_waitPenging = true;
                m_engine.setPending();
            }
            return true; //nothing to wait
        }
    }

    if(m_skipWait > 0) {
        --m_skipWait;
        if(m_skipWait > 0) {
            return true;
        }
    }

    const auto cont = setWait(!value.isEmpty() ? getValue(value[0]) : QVariant()); // set wait;
    if(!cont) {
        m_engine.setPending();
    }
    return true;
}

bool QtSystem::setWait(const QVariant& value) {
    if(m_waitTicks < 0) {
        int val;
        if(value.isValid() && num(value, val)) {
            m_waitTicks = m_engine.interval() > 0 ? val / m_engine.interval() : val;
        } else {
            m_waitTicks = std::numeric_limits<int>::max();
        }

    }
    if(m_waitTicks == 0) {
        if(m_engine[DEBUG_] == TRUE_) {
            PRINT << "Wait timeout";
        }
        //clean pendign signals
        auto it = m_signals.begin();
        while(it != m_signals.end()) {
            auto sig = *it;
            if(sig->pendingLine == m_engine.lineId()) {
                if(m_engine[DEBUG_] == TRUE_) {
                    PRINT << "Release signal (" << sig << ") name:" << sig->name <<  " pending:" << sig->pendingLine;
                    QObject::disconnect(sig->connection);
                    sig->deleteLater();
                    it = m_signals.erase(it); //ready or not this signal is removed.
                } else {
                    ++it;
                }
            }
        }
        m_signals.squeeze();
        m_engine.pushc(FALSE_);
    }
    return (m_waitTicks <= 0);
}

/**DOC
 * N click
 * S QT
 * B <x-offset y-offset>
 * Set mouse event to middle of focused UI item, offset to that point are optional.
 **/
bool QtSystem::click(const QStringList& value) {
    const auto item = qobject_cast<QQuickItem*>(m_focus);
    if(item != nullptr) {
        const auto point = item->mapToScene(QPoint(0, 0));
        int x = 0;
        int y = 0;
        if(!value.isEmpty()) {
            num(getValue(value[0]), x);
            num(value.count() > 1 ? getValue(value[1]) : "", y);
        }
        //  auto quickWindow = (qobject_cast<QQuickWindow*>(m_engine.rootObjects().at(0)));
        QTest::mouseClick(item->window(), Qt::LeftButton, Qt::NoModifier,
                          QPoint(x + point.x() + item->width() / 2, y + point.y() + item->height() / 2));
    } else {
        syntaxError(OBJECT_NOT_FOUND);
        return false;
    }
    return true;
}

bool QtSystem::write(const QStringList& values) {
    MIN_ARG(1)
    const auto item = qobject_cast<QQuickItem*>(m_focus);
    if(item != nullptr) {
        const auto sv = getValue(values[0]);

        if(!sv.canConvert(QVariant::String)) {
            syntaxError(INVALID_ARGUMENT);
            return false;
        }

        const auto sequence = sv.toString();

        for(int i = 0; i < sequence.length(); i++) {
            QTest::keyClick(item->window(), sequence.at(i).toLatin1());
        }
    }
    return true;
}

QMetaObject::Connection QtSystem::connect(GuiSignal* sig) {
    static const QRegularExpression funcTypeParser(SIGNAL_FUNCTION_MATCH);
    const auto m = funcTypeParser.match(sig->name);
    const char* const slotFunctions[] = {SLOT(slotFunction(QString)), SLOT(slotFunction(int)), SLOT(slotFunction())};
    QMetaObject::Connection connection;
    const char* slotf = nullptr;
    if(m.isValid()) {
        const auto type = m.captured(1);
        if(type == "") {
            slotf = slotFunctions[2];
        } else if(type == "string") {
            slotf = slotFunctions[0];
            sig->replace("string", "QString");
        } else if(type == "int") {
            slotf = slotFunctions[1];
        }
    }
    if(slotf == nullptr) {
        syntaxError(INVALID_SIGNAL, sig->name);
    } else {
        const auto ba = sig->name.toUtf8();
        const char* signalExpression = ba.constData();
        connection = QObject::connect(m_focus, qFlagLocation(signalExpression), sig, slotf);
    }
    return connection;
}

void QtSystem::expired(GuiSignal* item) {
    if(m_engine[DEBUG_] == TRUE_) {
        PRINT << "signalled (" << item << ") name:" << item->name << " sub:" << item->routineName << " pending:" << item->pendingLine;
    }
    const Keyword waitCmd("wait");


    if(m_engine.finishWaiting(item->routineName, item->values, waitCmd)) {

        if(m_skipWait > 0) {
            --m_skipWait;
        }
        m_waitTicks = -1;

    } else {
        //TODO: Here should come skip wait...
        ++m_skipWait;
    }
    m_signals.remove(item); //ready or not this signal is removed.
    QObject::disconnect(item->connection);
    item->deleteLater();
    m_engine.pushc(TRUE_);
}

/**DOC
 * N SLOT
 **/
bool QtSystem::slot(const QStringList& values) {
    MIN_ARG(1)
    if(m_focus != nullptr) {
        const auto connector = [this](GuiSignal * sig)->bool{
            auto connection = connect(sig);
            if(connection) {
                sig->connection = connection;
                return true;
            } else {
                syntaxError(SIGNAL_NOT_FOUND);
                return false;
            }
        };
        const auto sigName = getValue(values[0]).toString();

        if(sigName.isEmpty()) {
            syntaxError(INVALID_ARGUMENT);
            return false;
        }

        const QString mangled(QString("2" + sigName + " " + LOCATION).trimmed());
        QScopedPointer<GuiSignal> s(new GuiSignal(m_engine.api(), *this, mangled, connector));
        if(values.count() > 1) {
            bool ok;
            const auto label = getValue(values[1], &ok, QutsEngine::CALLPARAM).toString();
            if(!ok || label.isEmpty()) {
                syntaxError(INVALID_ARGUMENT, quoted(values[1]));
                return false;
            }
            s->routineName = label;
            s->values = values;
        }
        m_signals.insert(s.take());
    } else {
        syntaxError(OBJECT_NOT_FOUND);
        return false;
    }
    return true;
}

void QtSystem::setFocus(const QList<QObject*>& objects, const QStringList& values) {
    auto offset = 0;
    auto count = objects.count();
    auto objs = objects;
    if(values.count() > 0) {
        bool ok;
        offset = getValue(values[0]).toInt(&ok);
        //if ok, then p count must be odd
        const auto odd = values.count() & 0x1;
        if((ok && !odd) || (!ok && odd)) {
            syntaxError(INVALID_ARGUMENT);
            count = -1;
        } else {
            //then filters...
            if(values.length() > (ok ? 1 : 0)) {
                QList<QObject*> filtered;
                for(auto p = ok ? 1 : 0; p < values.length(); p += 2) {
                    const auto property = getValue(values[p]);
                    const auto rexp = getValue(values[p + 1]).toString();
                    const QRegularExpression rex(rexp);
                    if(!rex.isValid()) {
                        syntaxError(INVALID_ARGUMENT, quoted(rexp));
                        count = -1;
                        break;
                    } else {
                        for(const auto& obj : objects) {
                            const auto var = QQmlProperty::read(obj, property.toString());
                            if(rex.match(var.toString()).hasMatch()) {
                                filtered.append(obj);
                            }
                        }
                    }
                }
                count = filtered.length();
                objs = filtered;
            }
        }
    }

    if(offset < count) {
        pushc(count);
        m_focus = objs[offset];
    } else {
        pushc(FALSE_); //false
    }
}

/**DOC
 * change active 'item', if there are multiple items, use index, defaults to 0.
 * There may be list of property names and regexp values given used for filtering the results.
 * Return amount of hits on top of stack. Does not change UI focus (use CLICK for that).
 * see CHILD, PARENT, SIGNAL and CLICK.
 * N FIND
 * B name <index> <list>, where list := <list item>, item := property regexp
 * P index of item in case of multiple matches. Defaults to the first.
 * P property name of item
 * P regexp that match to to value of property.
 * R number of items in match.
**/
bool QtSystem::find(const QStringList& values) {
    MIN_ARG(1)
    const auto objName = getValue(values[0]).toString();
    if(objName.isEmpty()) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }
    const auto objs = m_engine.objectFinder().findByName(objName);
    auto params = values;
    params.removeFirst();
    setFocus(objs, params);
    return true;
}

bool QtSystem::set(const QStringList& values) {
    MIN_ARG(1)
    if(m_focus != nullptr) {
        bool ok = true;
        const auto val = values.length() > 1 ? getValue(values[1], &ok, QutsEngine::NORMALPARAM) : "";
        if(!ok) {
            syntaxError(INVALID_ARGUMENT, values[1]);
            return false;
        }
        const auto propertyName = getValue(values[0], &ok, QutsEngine::NORMALPARAM).toString();
        if(!ok || !QQmlProperty::write(m_focus, propertyName, val)) {
            syntaxError(INVALID_ARGUMENT, propertyName);
            return false;
        }
    } else {
        errorStr(OBJECT_NOT_FOUND);
        return false;
    }
    return true;
}

bool QtSystem::get(const QStringList& values) {
    MIN_ARG(1)
    if(m_focus != nullptr) {
        const auto property = getValue(values[0]).toString();
        const auto var = QQmlProperty::read(m_focus, property);
        if(var.isValid()) {
            if(values.length() > 1) {
                auto pos = getValue(values[1]).toString();
                if(pos.isEmpty()) {
                    return errorStr(INVALID_ARGUMENT);
                } else {
                    m_engine[pos] = var;
                }
            } else {
                m_engine.pushc(var);
            }
        } else { return errorStr(INVALID_OBJECT); }
    } else { return errorStr(OBJECT_NOT_FOUND); }
    return true;
}

/**DOC
 * Set Focus to current item parent.
 * N PARENT
 * R Boolean due found.
**/
bool QtSystem::parent(const QStringList& value) {
    Q_UNUSED(value);
    const auto item = qobject_cast<QQuickItem*>(m_focus);
    if(item != nullptr) {
        m_focus = item->parentItem();
        pushc(TRUE_);
    } else {
        pushc(FALSE_);
    }
    return true;
}


template <typename T>
void objectsFormVariant(QList<T*>& list, const QVariant& var) {
    switch(var.type()) {
    case QVariant::Hash: {
        const auto hash = var.toHash();
        QHashIterator<QString, QVariant> iter(hash);
        while(iter.hasNext()) {
            objectsFormVariant(list, iter.value());
        }
    }
    break;
    case QVariant::Map: {
        const auto map = var.toMap();
        QMapIterator<QString, QVariant> iter(map);
        while(iter.hasNext()) {
            objectsFormVariant(list, iter.value());
        }
    }
    break;
    case QVariant::List: {
        const auto vlist = var.toList();
        for(const auto& v : vlist) {
            objectsFormVariant(list, v);
        }
    }
    break;
    default: {
        const auto item = qvariant_cast<T*>(var);
        if(item != nullptr) {
            list.append(item);
        }
    }
    }
}

/**DOC
 * Set Focus to current item child or item its property refers to
 * See FOCUS and PARENT
 * N CHILD
 * B <index> <list>, where list := <list item>, item := property regexp
 * P index of item in case of multiple matches. Defaults to the first.
 * P property name of item
 * P regexp that match to to value of property.
 * R number of items in match.
**/
bool QtSystem::child(const QStringList& values) {
    const auto item = qobject_cast<QQuickItem*>(m_focus);
    if(item == nullptr) {
        syntaxError(OBJECT_NOT_FOUND);
        pushc(FALSE_);
        return false;
    }
    const auto childs = map<QObject*, QQuickItem*>(item->childItems(), [](QQuickItem * p) {return p;});
    setFocus(childs, values);
    return true;
}

/**DOC
 * Set Focus to item that is currently selected item property.
 * N ITEM
 * B property <index> <list>, where list := <list item>, item := property regexp
 * P property name
 * P index if property is a list, hash or map
 * P list of property regep pairs, see FOCUS and CHILD
 * R number of matches
**/
bool QtSystem::item(const QStringList& values) {
    MIN_ARG(1)
    const auto item = qobject_cast<QQuickItem*>(m_focus);
    if(item == nullptr) {
        syntaxError(OBJECT_NOT_FOUND);
        pushc(FALSE_);
        return false;
    }
    const auto property = getValue(values[0]).toString();
    if(property.isEmpty()) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }
    const auto var = QQmlProperty::read(m_focus, property);
    QList<QObject*> list;
    objectsFormVariant(list, var);
    auto params = values;
    params.removeFirst();
    setFocus(list, params);
    return true;
}

/**DOC
 * Emits a QutsAPI signal "qutsSignal"
 * N qutsSignal
 * B signal_name <parameter_list>
 * P sign_name is string to idententify this signal in the slot, followed by parameters
 **/
bool QtSystem::qutsSignal(const QStringList& values) {
    MIN_ARG(1)
    const auto signalName = getValue(values[0]).toString();
    if(signalName.isEmpty()) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }
    QVariantList params;
    for(int i = 1; i < values.count() ; i++) {
        params.append(getValue(values.at(i)));
    }
    m_waitPenging = false;
    emit m_engine.api()->qutsSignal(signalName, params);
    return true;
}


/**DOC
 * Emit a signal to focused object
 * N signal
 * B signal_name <parameter_list>
 * P signal_name (without parameters or brackets)
 **/
bool QtSystem::signal(const QStringList& values) {
    MIN_ARG(1)
    if(!m_focus) {
        syntaxError(OBJECT_NOT_FOUND);
        return false;
    }
    const auto val = getValue(values[0]).toString();
    if(val.isEmpty()) {
        syntaxError(INVALID_ARGUMENT);
        return false;
    }
    const auto signalName = qPrintable(val);
    bool conversionOk = true;

    constexpr auto maxParamCount = 10;
    auto getArg = [&values, &conversionOk, this](int index)->QGenericArgument{
        const auto typeIndex = (index * 2) + 1;
        const auto valueIndex = (index * 2) + 2;
        const auto typeName = (valueIndex < values.length()) ?
        values[typeIndex] : "";

        if(valueIndex < values.length()) {
            const auto dataStr = values[typeIndex];
            if(typeName == "string") {
                return Q_ARG(QVariant, qPrintable(dataStr));
            } else if(typeName == "int" || typeName == "short") {
                return Q_ARG(QVariant, dataStr.toInt(&conversionOk));
            } else if(typeName == "float" || typeName == "real" || typeName == "double") {
                return Q_ARG(QVariant, dataStr.toFloat(&conversionOk));
            }
        }
        return QGenericArgument();
    };
    QGenericArgument args[maxParamCount];
    for(auto i = 0; i < maxParamCount; i++) {
        args[i] = getArg(i);
        if(!conversionOk) {
            syntaxError(INVALID_ARGUMENT, QString("argument at %1").arg(i));
            return false;
        }
    }

    const auto success = QMetaObject::invokeMethod(m_focus, signalName,
                         args[0],
                         args[1],
                         args[2],
                         args[3],
                         args[4],
                         args[5],
                         args[6],
                         args[7],
                         args[8],
                         args[9]
                                                  );


    if(!success) {
        syntaxError(INVALID_SIGNAL, val);
        return false;
    }
    m_waitPenging = false;
    return true;
}

bool QtSystem::isActive() const {
    if(m_signals.count() > 0) {
        const_cast<QtSystem*>(this)->errorStr(STRAY_SIGNAL);
    }
    return false;
}

void QtSystem::clear() {
    auto it = m_signals.begin();
    while(it != m_signals.end()) {
        auto item = *it;
        QObject::disconnect(item->connection);
        item->deleteLater();
        it = m_signals.erase(it); //ready or not this signal is removed.
    }
    m_skipWait = 0;
    m_waitTicks = -1;
    m_focus = nullptr;
    m_signals.clear();
}

void QtSystem::tick() {
    if(m_waitTicks >= 0) {
        --m_waitTicks;
    }
}

void QtSystem::requestResume() {
    m_waitTicks = -1;
}

bool QtSystem::fid(const QStringList& values) {
    MIN_ARG(1)
    const auto id = getValue(values[0]).toString();
    if(id.isEmpty()) {
        syntaxError(INVALID_ARGUMENT); //no return?
        return false;
    }
    QString scope = "";
    if(values.length() > 1) {
        scope = getValue(values[1]).toString();
        if(scope.isEmpty()) {
            syntaxError(INVALID_ARGUMENT);
            return false;
        }
    }

    const auto objs = m_engine.objectFinder().findById(id, scope);
    auto params = values;
    params.removeFirst();
    if(!scope.isEmpty()) {
        params.removeFirst();
    }
    setFocus(objs, params);
    return true;
}


