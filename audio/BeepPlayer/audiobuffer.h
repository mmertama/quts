#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <deque>
#include <QIODevice>


namespace Beeper {


class AudioBuffer : public QIODevice {
    Q_OBJECT
public:
    AudioBuffer(QObject* parent) : QIODevice(parent) {}
    bool open(OpenMode mode) {setOpenMode(mode); return isOpen();}
    void clear();
    bool isSequential() const {return false;}
    qint64 size() const {return m_buffer.size();}
    qint64 bytesAvailable() const {return m_buffer.size() + QIODevice::bytesAvailable();}
protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64  writeData(const char* data, qint64 maxSize);
private:
    std::deque<char> m_buffer;
};

}

#endif // AUDIOBUFFER_H
