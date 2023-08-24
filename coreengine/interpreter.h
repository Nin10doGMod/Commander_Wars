#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QObject>
#include <QQmlEngine>
#include <QVector>
#include <QCoreApplication>

class Interpreter;
using spInterpreter = std::shared_ptr<Interpreter>;
/**
 * @brief The Interpreter class java-script interpreter with easy access functions
 */
class Interpreter final : public QQmlEngine
{
    Q_OBJECT

public:
    static Interpreter* getInstance()
    {
        return m_pInstance.get();
    }
    static Interpreter* createInstance();
    ~Interpreter();
    static void release();

    static void setCppOwnerShip(QObject* object);
    /**
     * @brief getRuntimeData
     * @return
     */
    QString getRuntimeData();
    /**
     * @brief reloadInterpreter
     * @param runtime
     */
    static bool reloadInterpreter(QString runtime);

    bool getInJsCall() const;
    void trackJsObject(std::shared_ptr<QObject> pObj);

    template<typename _TType, template<typename T> class _TVectorList>
    QJSValue arraytoJSValue(const _TVectorList<_TType> & array)
    {
        QJSValue jsArray = newArray(array.size());
        for (qint32 i = 0; i < array.size(); i++)
        {
            if constexpr (std::is_same<_TType, QPoint>::value)
            {
                jsArray.setProperty(i, toScriptValue(array[i]));
            }
            else
            {
                jsArray.setProperty(i, array[i]);
            }
        }
        return jsArray;
    }

    void threadProcessEvents()
    {
        Q_ASSERT(m_inJsCall == 0);
        QCoreApplication::processEvents();
    }

signals:
    void sigNetworkGameFinished(qint32 value, QString id);
public slots:
    bool openScript(const QString & script, bool setup);
    bool loadScript(const QString & content, const QString & script);
    inline QJSValue doFunction(const QString & func, const QJSValueList& args = QJSValueList())
    {
        clearJsStack();
        QJSValue ret;
        {
            QJSValue funcPointer = globalObject().property(func);
            ++m_inJsCall;
            ret = funcPointer.call(args);
        }
        exitJsCall();
        if (ret.isError())
        {
            QString error = ret.toString() + " in File: " +
                            ret.property("fileName").toString() + " at Line: " +
                            ret.property("lineNumber").toString();
            printError(error);
        }
        return ret;
    }
    inline QJSValue doFunction(const QString & obj, const QString & func, const QJSValueList& args = QJSValueList())
    {
        clearJsStack();
        QJSValue ret;
        {
            QJSValue objPointer = globalObject().property(obj);
            QJSValue funcPointer = objPointer.property(func);
            ++m_inJsCall;
            ret = funcPointer.call(args);
        }
        exitJsCall();
        if (ret.isError())
        {
            QString error = ret.toString() + " in File: " +
                            ret.property("fileName").toString() + " at Line: " +
                            ret.property("lineNumber").toString();
            printError(error);
        }
        return ret;
    }

    inline void triggerCollectGarbage()
    {
        ++m_jsCallCount;
        constexpr qint32 REFRESH_COUNT = 400;
        if (m_jsCallCount >= REFRESH_COUNT)
        {
            collectGarbage();
            m_jsCallCount = 0;
        }
    }
    /**
     * @brief doString immediately interprates the string with the javascript-interpreter
     * @param task string parsed to the interpreter
     */
    QJSValue doString(const QString & task);
    /**
     * @brief pushInt
     * @param value
     */
    void pushInt(const QString & name, qint32 value);
    void pushDouble(const QString & name, double value);
    void pushString(const QString & name, const QString & value);
    void pushObject(const QString & name, QObject* object);
    inline QJSValue newQObject(QObject* object)
    {
        return QJSEngine::newQObject(object);
    }
    void deleteObject(const QString & name);

    qint32 getGlobalInt(const QString & var);
    bool getGlobalBool(const QString & var);
    double getGlobalDouble(const QString & var);
    QString getGlobalString(const QString & var);
    QJSValue getGlobal(const QString & var);
    void setGlobal(const QString & var, const QJSValue & obj);
    /**
     * @brief exists checks if the js object exists
     * @param object
     * @return
     */
    bool exists(const QString & object)
    {
        QJSValue objPointer = globalObject().property(object);
        if (objPointer.isObject())
        {
            return true;
        }
        return false;
    }
    /**
     * @brief exists checks if the js object and function exists
     * @param object
     * @return
     */
    bool exists(const QString & object, const QString & function)
    {
        QJSValue objPointer = globalObject().property(object);
        if (objPointer.isObject())
        {
            QJSValue funcPointer = objPointer.property(function);
            if (funcPointer.isCallable())
            {
                return true;
            }
        }
        return false;
    }

private slots:
    void networkGameFinished(qint32 value, QString id);
private:
    friend class MemoryManagement;
    explicit Interpreter();    
    /**
     * @brief init
     */
    void init();
    void exitJsCall()
    {        
        triggerCollectGarbage();
        --m_inJsCall;
        Q_ASSERT(m_inJsCall >= 0);
    }
    void clearJsStack()
    {
        if (m_inJsCall == 0)
        {
            m_jsObjects.clear();
        }
    }
    void printError(const QString & msg);
private:
    static spInterpreter m_pInstance;
    static QString m_runtimeData;
    qint32 m_inJsCall{0};
    std::vector<std::shared_ptr<QObject>> m_jsObjects;
    qint32 m_jsCallCount{0};
};

#endif // INTERPRETER_H
