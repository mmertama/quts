/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef PERSISTENTDATA_H
#define PERSISTENTDATA_H

#include <QVariant>


class PersistentData {
public:
    PersistentData(const QString& appname, const QString& companyname);
    QVariant getSettingVariant(QString id, const QVariant& def);
    QString getSettingString(QString id, QString  def = "");
    void setSetting(QString id, const QVariant& value);

    template <class T>
    void setSetting(QString id, const T& value) {
        setSetting(id, QVariant::fromValue<T>(value));
    }

    template <class T>
    T getSetting(QString id, const T& defaultValue) {
        return getSettingVariant(id, QVariant::fromValue<T>(defaultValue)).template value<T>();
    }
private:
    const QString m_settingComName;
    const QString m_settingAppName;
};



#endif // PERSISTENTDATA_H
