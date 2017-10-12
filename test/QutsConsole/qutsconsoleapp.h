/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef QUTSCONSOLEAPP_H
#define QUTSCONSOLEAPP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "persistentdata.h"

class QutsConsoleApp : public QObject {
    Q_OBJECT
public:
    QutsConsoleApp(QQmlApplicationEngine& engine) {
        engine.rootContext()->setContextProperty("QutsApp", this);
    }
    Q_INVOKABLE void setSetting(const QString& key, const QVariant& value) const {
        PersistentData("quts", "markus").setSetting(key, value);
    }
    Q_INVOKABLE QVariant getSetting(const QString& key, const QVariant& defaultValue) const {
        return PersistentData("quts", "markus").getSetting(key, defaultValue);
    }
    Q_INVOKABLE QString toFile(const QUrl& url) const {
        return url.toLocalFile();
    }
    Q_INVOKABLE bool copyTo(const QString& from, const QString& to) const;

};

#endif // QUTSCONSOLEAPP_H
