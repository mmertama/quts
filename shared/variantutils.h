/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef VARIANTUTILS_H
#define VARIANTUTILS_H

#include <QVariant>
#include <QBitArray>
#include <QDate>
#include <QUrl>
#include <QLocale>
#include <QRect>
#include <QLine>
#include <QRegularExpression>
#include <QUuid>
#include <QModelIndex>


#include "constants.h"

namespace Quts {

inline bool toBool(const QVariant& var) {
    switch(var.type()) {
    case QVariant::Invalid: return false;
    case QVariant::Bool: return var.toBool();
    case QVariant::Int: return var.toInt();
    case QVariant::UInt: return var.toUInt();
    case QVariant::LongLong: return var.toLongLong();
    case QVariant::ULongLong: return var.toULongLong();
    case QVariant::Double: return static_cast<bool>(var.toDouble());
    case QVariant::Char: return var.toChar() != QChar() && var.toChar() != QChar('0') && var.toChar() != QChar('\0');
    case QVariant::Map: return !var.toMap().isEmpty();
    case QVariant::List: return !var.toList().isEmpty();
    case QVariant::String: return !var.toString().isEmpty() && var.toString() != FALSE_ && var.toString() != "0";
    case QVariant::StringList: return var.toStringList().isEmpty();
    case QVariant::ByteArray: return !var.toByteArray().isEmpty();
    case QVariant::BitArray: return !var.toBitArray().isEmpty();
    case QVariant::Date: return var.toDate().isValid();
    case QVariant::Time: return var.toTime().isValid();
    case QVariant::DateTime: return var.toDateTime().isValid();
    case QVariant::Url: return var.toUrl().isValid();
    case QVariant::Locale: return !var.toLocale().name().isEmpty();
    case QVariant::Rect: return !var.toRect().isEmpty();
    case QVariant::RectF: return !var.toRectF().isEmpty();
    case QVariant::Size: return !var.toSize().isEmpty();
    case QVariant::SizeF: return !var.toSizeF().isEmpty();
    case QVariant::Line: return !var.toLine().isNull();
    case QVariant::LineF: return !var.toLineF().isNull();
    case QVariant::Point: return !var.toPoint().isNull();
    case QVariant::PointF: return var.toPointF().isNull();
    case QVariant::RegExp: return var.toRegExp().isValid();
    case QVariant::RegularExpression: return var.toRegularExpression().isValid();
    case QVariant::Hash: return !var.toHash().isEmpty();
    case QVariant:: Uuid: return !var.toUuid().isNull();
    default:
        return var.isValid() && var.data() != nullptr;
    }
}

inline int toLen(const QVariant& var) {
    switch(var.type()) {
    case QVariant::Map: return var.toMap().size();
    case QVariant::List: return var.toList().length();
    case QVariant::String: return var.toString().length();
    case QVariant::StringList: return var.toStringList().length();
    case QVariant::ByteArray: return var.toByteArray().length();
    case QVariant::BitArray: return var.toBitArray().count();
    case QVariant::Hash: return var.toHash().size();
    default:
        if(var.canConvert(QVariant::String)) {
            return toLen(var.toString());
        }
    }
    return -1;
}
}

#endif // VARIANTUTILS_H
