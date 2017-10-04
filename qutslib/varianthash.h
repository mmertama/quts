/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef VARIANTHASH_H
#define VARIANTHASH_H

#include <QVariant>

namespace Quts {

class VariantHash {
public:
    virtual QVariant operator[](const QString& key) const = 0;
    virtual QVariant& operator[](const QString& key) = 0;
    virtual bool contains(const QString& key) const = 0;
    virtual void remove(const QString& key) = 0;
    VariantHash() = default;
    VariantHash(const VariantHash&) = default;
    VariantHash(VariantHash&&) = default;
    VariantHash& operator=(const VariantHash&)& = default;
    VariantHash& operator=(VariantHash&&)& = default;
    virtual ~VariantHash() = 0;
};
}

#endif // VARIANTHASH_H
