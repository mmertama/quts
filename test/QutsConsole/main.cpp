/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDirIterator>
#include <QRegularExpression>
#include <QTimer>
#include <QProcessEnvironment>

#include <QDebug>

#include "qutsconsoleapp.h"
#include "quts.h"
#include "persistentdata.h"

QStringList resources() {
    QStringList list;
    const QRegularExpression re(R"(/\:\/qt-project\.org/)");
    QDirIterator it(":", QDirIterator::Subdirectories);
    while(it.hasNext()) {
        const auto r = it.next();
        if(!re.match(r).hasMatch()) {
            list.append(r);
        }
    }
    return list;
}


int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QutsConsoleApp singleton(engine);
    QScopedPointer<Quts::QutsAPI> quts(new Quts::QutsAPI(&engine, Quts::QutsAPI::StdPrint));

    engine.rootContext()->setContextProperty("resourceList", QVariant::fromValue(resources()));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QTimer timer;
    if(argc > 1) {
        const QString filename(argv[1]);
        timer.setSingleShot(true);
        QObject::connect(&timer, &QTimer::timeout, [&quts, filename]() {
            const auto name = quts->read(filename);
            if(!name.isEmpty()) {
                quts->start(name);
            }
        });
        timer.start(5000);
    }

    const auto path = QCoreApplication::applicationDirPath();
    auto pe = QProcessEnvironment();
    auto p = pe.value("QUTS_PATH");
    if(p.isEmpty()){
        p.append(path);
        p.append(QString(";%1/libs").arg(path));
        pe.insert("QUTS_PATH", p);
    }
    return app.exec();
}


