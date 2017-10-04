/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include <QSet>
#include <QPair>
#include <QQmlApplicationEngine>
#include <QMutableSetIterator>

#include "objectfinder.h"
#include "qhooks_p.h"
#include "qqmlcontext_p.h"

#include "objectfinder_private.h"

#include <QDebug>

using namespace Quts;



ObjectFinderPrivate::ObjectFinderPrivate() {
    m_hooks[0] = qtHookData[QHooks::AddQObject];
    m_hooks[1] = qtHookData[QHooks::RemoveQObject];

    static ObjectFinderPrivate* gThis = this;

    const auto b = [](QObject * object) {
        gThis->append(object);
        if(gThis->m_hooks[0] != 0)
            reinterpret_cast<QHooks::AddQObjectCallback>
            (gThis->m_hooks[0])(object);

    };
    const auto r = [](QObject * object) {
        gThis->remove(object);
        if(gThis->m_hooks[1] != 0)
            reinterpret_cast<QHooks::RemoveQObjectCallback>
            (gThis->m_hooks[1])(object);
    };
    qtHookData[QHooks::AddQObject] = reinterpret_cast<quintptr>(
                                         (QHooks::AddQObjectCallback)b);
    qtHookData[QHooks::RemoveQObject] = reinterpret_cast<quintptr>(
                                            (QHooks::RemoveQObjectCallback)r);

}


void ObjectFinderPrivate::append(QObject* object) {
    QMutexLocker(&this->m_mutex);
    m_objects.insert(object);
}


void ObjectFinderPrivate::remove(QObject* object) {
    QMutexLocker(&this->m_mutex);
    m_objects.remove(object);
}

QList<QObject*> ObjectFinderPrivate::findByName(const QString& name) const {
    QMutexLocker(&this->m_mutex);
    QList<QObject*> list;
    for(const auto obj : m_objects)
        if(obj->objectName() == name) {
            list.append(obj);
        }
    return list;
}

static bool equals(QQmlContextData* context, const QString& contextName) {
    if(context) {
        if(context->urlString().contains(contextName, Qt::CaseSensitive)) {
            return true;
        } else {
            return equals(context->parent, contextName);
        }
    }
    return false;
}



QList<QObject*> ObjectFinderPrivate::findById(const QString& id, const QString& contextName) const {
    QMutexLocker(&this->m_mutex);
    QList<QObject*> list;
    for(const auto& obj : m_objects) {
        const auto priv = QObjectPrivate::get(obj);
        if(priv && priv->declarativeData) {
            const auto ddata = static_cast<QQmlData*>(priv->declarativeData);
            const auto c = ddata->context;
            if(c != nullptr && (contextName.isEmpty() || equals(c, contextName))) {
                const auto objId = c->findObjectId(obj);
                if(objId == id) {
                    list.append(obj);
                    break;

                }
            }
        }
    }
    return list;
}


ObjectFinderPrivate::~ObjectFinderPrivate() {

    qtHookData[QHooks::AddQObject] = m_hooks[0] ;
    qtHookData[QHooks::RemoveQObject] = m_hooks[1];
}


ObjectFinder::ObjectFinder() : m_p(new ObjectFinderPrivate()) {
}

ObjectFinder::~ObjectFinder() {
}

QList <QObject*> ObjectFinder::findByName(const QString& name) const {
    return m_p->findByName(name);
}

QList<QObject*> ObjectFinder::findById(const QString& id, const QString& context) const {
    return m_p->findById(id, context);
}
