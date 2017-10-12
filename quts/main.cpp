/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include <QCoreApplication>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTimer>
#include "../quts.h"

#include <QDebug>
#include <QProcessEnvironment>

using namespace Quts;

constexpr char TOKEN[] = R"(([a-zA-Z][\w]*)\:)";

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    QScopedPointer<QutsAPI> quts(new Quts::QutsAPI(nullptr, Quts::QutsAPI::StdPrint));

    const auto envs = QProcessEnvironment::systemEnvironment();
    if(envs.contains("QUTS_PATH")){
        const auto p = envs.value("QUTS_PATH").split(';');
        for(const auto& s : p)
            QCoreApplication::addLibraryPath(s);
    }

    const auto path = QCoreApplication::applicationDirPath();
    auto pe = QProcessEnvironment();
    auto p = pe.value("QUTS_PATH");
    if(p.isEmpty()){
        p.append(path);
        p.append(QString(";%1/libs").arg(path));
        pe.insert("QUTS_PATH", p);
    }

    if(argc < 2) {
        qWarning() << QFileInfo(argv[0]).baseName() << "quts-file" << "<parameters>";
        qWarning() << "parameters with name:value pairs are stored as variables, plain values are pushed into stack.";
    }

    QVariantMap data;
    QVariantList stack;
    for(auto i = 2; i < argc; i++) {
        const QRegularExpression tok(TOKEN);
        const QString s(argv[i]);
        const auto m = tok.match(s);
        if(m.hasMatch()) {
            const auto k = m.captured(1);
            const auto v = s.mid(m.captured(0).length());
            data[k] = v;
        } else {
            stack.append(s);
        }
    }

    const QString filename(argv[1]);


    QTimer timer;
    timer.setSingleShot(true);
    timer.start(0);
    QObject::connect(&timer, &QTimer::timeout, [&quts, &data, &stack, &filename]() {
        const auto name = quts->read(filename);
        if(!name.isEmpty()) {
            quts->start(name, data, stack);
        }
    });


    QObject::connect(quts.data(), &QutsAPI::end, [](const QString&) {
        QCoreApplication::quit();
    });

    QObject::connect(quts.data(), &QutsAPI::error, [](const QString & message, const QString & script, int line) {
        qWarning() << "Error:" << message << "in" << script << "at" << line;
        QCoreApplication::quit();
    });

    return a.exec();
}
