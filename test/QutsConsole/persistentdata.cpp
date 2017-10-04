/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#include "persistentdata.h"

#include <QSettings>
#include <QDebug>



PersistentData::PersistentData(const QString& appname, const QString& companyname) :
    m_settingComName(companyname),
    m_settingAppName(appname) {
}

QVariant PersistentData::getSettingVariant(QString id, const QVariant& def) {
    QSettings settings(m_settingComName,
                       m_settingAppName);
    if(settings.status() != QSettings::NoError) {
        qWarning() << "get setting" << settings.status();
    }
    return settings.value("data/" + id, def);
}

QString PersistentData::getSettingString(QString id, QString  def) {
    return getSettingVariant(id, def).toString();
}

void PersistentData::setSetting(QString id, const QVariant& value) {
    QSettings settings(m_settingComName,
                       m_settingAppName);
    if(settings.status() != QSettings::NoError) {
        qWarning() << "set setting" << settings.status();
    }
    settings.beginGroup("data");
    settings.setValue(id, value);
    settings.endGroup();
}
