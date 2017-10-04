/* This file is part of Quts, Copyright 2017 Markus Mertama. Quts is distributed under the GNU General Public License (GNU GPL) version 3.  See License.txt  */
#ifndef QUTS_H
#define QUTS_H

#include <QObject>
#include <QScopedPointer>
#include <QHash>
#include <QVariant>


class QIODevice;
class QQmlApplicationEngine;

#ifdef QUTSLIB_EXPORT
# define QUTSLIB_EXPORT_API Q_DECL_EXPORT
#else
# define QUTSLIB_EXPORT_API Q_DECL_IMPORT
#endif

namespace Quts {

class QutsAPIPrivate;

class QUTSLIB_EXPORT_API QutsAPI : public QObject{
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(QVariantMap snapShot READ snapShot NOTIFY snapShotChanged)
    Q_PROPERTY(bool isPrintStd READ isPrintStd WRITE setIsPrintStd NOTIFY isPrintStdChanged)
public:
    enum  Flags{
        StdPrint = 0x1
    };
    /**
     * @brief QutsAPI  constructor
     * @param engine
     * @param flags
     * @param parent
     */
    QutsAPI(QQmlApplicationEngine* engine = nullptr, int flags = 0, QObject* parent = nullptr);

    ~QutsAPI();

    /**
      * @brief start
      * @param scriptname
      * @param memory
      * @param stack
      * @return
      */

     Q_INVOKABLE bool start(const QString& scriptname, const QVariantMap& memory = QVariantMap(), const QVariantList& stack = QVariantList());

    /**
     * @brief read,  add script sequence from file
     * @param filename
     * @return scriptName
     */
    Q_INVOKABLE QString read(const QString& filename);

    /**
     * @brief read,  add script sequence from url
     * @param filename
     * @return scriptName
     */
    Q_INVOKABLE QString read(const QUrl& url);
     /**
      * @brief execute
      * @param scriptName - just a name to identify
      * @param data
      * @return
      */
     Q_INVOKABLE bool execute(const QString& scriptName, const QStringList& data);

    /**
     * @brief pause
     */

    Q_INVOKABLE void pause();

    /**
     * @brief resume
     */
    Q_INVOKABLE bool resume();

    /**
     * @brief step
     */
    Q_INVOKABLE void step();

    /**
      * @brief clear
      */
    Q_INVOKABLE void clear();


    /**
     * @brief stop
     */
    Q_INVOKABLE void stop();


   /**
     * @brief doc
     * return list of keywords
     */
     Q_INVOKABLE QStringList help(const QString& system = "") const;

    /**
      * @brief doc, detailed information of keyword
      * @param keywords
      */
     Q_INVOKABLE QVariantMap functionHelp(const QString& system, const QString& keyword = "") const;

    /**
       * @brief setBreakpoint
       * @param lineNumber
       * @param set
       * @return success
       */

      Q_INVOKABLE bool setBreakpoint(const QString& scriptName, int lineNumber, bool set);


    /**
        * @brief removeAllBreakpoints
        */

    Q_INVOKABLE void removeAllBreakpoints();

    /**
      * @brief read add script from stream
      * @param scriptName
      * @param stream
      */
      bool read(QIODevice& stream, const QString& scriptName);

      /**
       * @brief active
       * @return true if running
       */
      bool active() const;

      /**
         * @brief snapShot
         * @return map of internals
         */

       QVariantMap snapShot() const;

        bool isPrintStd() const;
        void setIsPrintStd(const bool isPrint);

signals:
     /**
      * @brief qutsSignal
      * @param signalName
      * @param params
      */
     void qutsSignal(const QString& signalName, const QVariantList& params);
    /**
     * @brief end(const QString& scriptName); signalled when sequence ends
     */
    void end(const QString& scriptName);
    /**
     * @brief error(const QString& message, const QString& script, int line); signalled on error
     * @param message
     * @param script
     * @param line
     */
    void error(const QString& message, const QString& script, int line);

    /**
     * @brief printLine
     * @param line
     */
    void printLine(const QString& line);

    /**
     * @brief activeChanged
     */
    void activeChanged();

    /**
     * @brief breakAt
     * @param lineNumber
     */
    void breakAt(const QString& scriptName, int lineNo);

    /**
     * @brief lineRead
     * @param module
     * @param lineNo
     * @param line
     */
    void lineAdded(const QString& scriptName, int lineNo, const QString& line);

    /**
     * @brief snapShotChanged
     */
    void snapShotChanged();

    void isPrintStdChanged();

private:
    Q_DECLARE_PRIVATE(QutsAPI)
    QScopedPointer<QutsAPIPrivate> d_ptr;
};

} //namespace


#endif // QUTS_H
