/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#ifndef INDEX_H
#define INDEX_H

#include <QString>
#include <QDebug>
#include "command.h"

namespace Quts {

#define BAD_INDEX Index()


#define INTERNAL_FORMAT_INDEX static_cast<QString>(i.m_snippet) << "->"<< i.m_index

class Index {
public:
    Index() {}
    Index(const CommandList* cmds) : m_cmds(cmds) {}
    Index(const Index& other) = default;
    Index(Index&& other) = default;
    void init(const QString& snippet) {m_snippet = snippet; m_index = 0; Q_ASSERT(m_cmds);}
    Index operator()(const QString& snippet, int index = 0) const {Q_ASSERT(m_cmds); auto s(*this); s.m_snippet = snippet; s.m_index = index; return s;}
    void invalidate() {m_index = -1; m_snippet = QString();}
    bool isValid() const {return m_index >= 0 && m_snippet.isValid() && m_cmds && m_cmds->contains(m_snippet) && m_index < (*m_cmds)[m_snippet].length();}
    bool isEmpty() const {Q_ASSERT(m_index >= 0 && m_snippet.isValid() && m_cmds && m_cmds->contains(m_snippet)); return (*m_cmds)[m_snippet].isEmpty();}
    Index end() const {return isValid() ? (*this)((*m_cmds)[m_snippet].length() - 1) : BAD_INDEX;}
    Index begin() const {return isValid() && !(*m_cmds)[m_snippet].isEmpty() ? (*this)(0) : BAD_INDEX;}
    void clone(const Index& other) {m_cmds = other.m_cmds; *this = other; }
    Index& operator = (const Index& other) {Q_ASSERT(m_cmds || (!m_cmds && other.m_cmds)); if(!m_cmds) m_cmds = other.m_cmds;  m_index = other.m_index; m_snippet = other.m_snippet; return *this;}
    Index& operator = (int val) {m_index = val; return *this;}
    Index& operator = (const QString& snippet) {m_index = 0; m_snippet = snippet; return *this;}
    Index& operator ++ () {++m_index; return *this;}
    Index& operator += (int value) {m_index += value; return *this;}
    const Command& operator * () const {Q_ASSERT(isValid()); return (*m_cmds)[m_snippet][m_index];}
    QString current() const {return m_snippet;}
    Index operator()(int value) const {auto ii = *this; ii.m_index = value; return ii;}
    bool operator==(const Index& other) const {return m_index == other.m_index && m_snippet == other.m_snippet;}
    bool operator!=(const Index& other) const {return ! operator ==(other);}
    friend QDebug operator << (QDebug d, const Index& i)  {d << INTERNAL_FORMAT_INDEX; return d;}
    friend QTextStream& operator << (QTextStream& d, const Index& i)  {d << INTERNAL_FORMAT_INDEX; return d;}
    friend Index operator + (const Index& index, int val) { auto ii = index; ii.m_index += val; return ii;}
    friend Index operator - (const Index& index, int val) { auto ii = index; ii.m_index -= val; return ii;}
    friend bool operator < (const Index& a, const Index& b) {return a.m_snippet == b.m_snippet && a.m_index < b.m_index;}
    friend bool operator > (const Index& a, const Index& b) {return a.m_snippet == b.m_snippet && a.m_index > b.m_index;}
    friend bool operator <= (const Index& a, const Index& b) {return  a == b || a < b;}
    friend bool operator >= (const Index& a, const Index& b) {return  a == b || a > b;}
    friend uint qHash(const Index& key) { return qHash(static_cast<QString>(key.m_snippet) + key.m_index);}
private:
    const CommandList* m_cmds = nullptr;
    int m_index = -1;
    Keyword m_snippet;
};


}


#endif // INDEX_H
