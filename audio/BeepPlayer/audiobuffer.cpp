/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */

#include "audiobuffer.h"

using namespace Beeper;


void AudioBuffer::clear() {
    m_buffer.clear();
}

qint64 AudioBuffer::readData(char* data, qint64 maxSize) {
    const auto begin = m_buffer.begin();
    const auto end = begin + qMin(maxSize, size());
    std::copy(begin, end, data);
    m_buffer.erase(begin, end);
    return std::distance(begin, end);
}

qint64 AudioBuffer::writeData(const char* data, qint64 maxSize) {
    if(m_buffer.end() == m_buffer.end()){
        m_buffer.insert(m_buffer.begin(), data, data + maxSize);
        return m_buffer.size();
    } else {
        const auto pos = m_buffer.end();
        m_buffer.insert(pos, data, data + maxSize);
        return std::distance(pos, m_buffer.end());
    }
}

