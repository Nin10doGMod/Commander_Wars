#ifndef MAINAPP_H
#define MAINAPP_H

#include <atomic>
#include <QTimer>
#include <QTranslator>
#include <QThread>
#include <QCoreApplication>
#include <QRandomGenerator>
#include "QMutex"

#include "coreengine/interpreter.h"
#include "coreengine/audiothread.h"
#include "coreengine/workerthread.h"
#include "coreengine/settings.h"

#include "coreengine/qmlvector.h"

#include "network/tcpserver.h"

class Mainapp : public QCoreApplication
{
    Q_OBJECT
public:
    /**
     * @brief The ZOrder enum for z-order of actors directly attached to the game map or the menu
     */
    enum class ZOrder
    {
        Background = -32000,
        Terrain = -30000,
        CORange = 30000,
        FogFields,
        MarkedFields,
        Weather,
        Cursor,
        Animation,
        Objects = 31998,
        FocusedObjects = 31999,
        Dialogs = 32000,
        Console
    };

    explicit Mainapp(int argc, char* argv[]);
    virtual ~Mainapp();

    static Mainapp* getInstance();

    inline QTranslator* getTranslator()
    {
        return &m_Translator;
    }

    inline Interpreter* getInterpreter()
    {
        return m_Worker.getInterpreter();
    }

    inline AudioThread* getAudioThread()
    {
        return &m_Audiothread;
    }

    inline Settings* getSettings()
    {
        return &m_Settings;
    }

    void setup();
    static bool getUseSeed();
    static void setUseSeed(bool useSeed);

    inline static QThread* getWorkerthread()
    {
        return &m_Workerthread;
    }
    inline TCPServer* getGameServer()
    {
        return m_pGameServer.get();
    }
    inline void stopGameServer()
    {
        m_pGameServer = nullptr;
    }
    inline static QThread* getNetworkThread()
    {
        return &m_Networkthread;
    }
    inline static QThread* getAudioWorker()
    {
        return &m_AudioWorker;
    }

    void suspendThread();
    void continueThread();
    void start();
signals:
    void sigText(SDL_Event event);
    void sigKeyDown(SDL_Event event);
    void sigKeyUp(SDL_Event event);

    void sigConsoleText(SDL_Event event);
    void sigConsoleKeyDown(SDL_Event event);
    void sigConsoleKeyUp(SDL_Event event);

public slots:
    void update();
    static void seed(quint32 seed);
    static qint32 randInt(qint32 low, qint32 high);
    /**
     * @brief roundUp rounds all numbers up. 9.1 -> 10
     * @param value
     * @return the rounded up integer value
     */
    static qint32 roundUp(float value);
    static qint32 roundDown(float value);
    static bool isEven(qint32 value);
    /**
     * @brief getCircle
     * @param min radius
     * @param max radius
     * @return vector containing all fields in the given radius. Note: the Object needs to be deleted by the reciever
     */
    static QmlVectorPoint* getCircle(qint32 min, qint32 max);
    /**
     * @brief getShotFields
     * @param min
     * @param max
     * @param xDirection
     * @param yDirection
     * @return
     */
    static QmlVectorPoint* getShotFields(qint32 min, qint32 max, qint32 xDirection = 0, qint32 yDirection = 0);
    /**
     * @brief getEmptyPointArray
     * @return
     */
    static QmlVectorPoint* getEmptyPointArray();
    /**
     * @brief quitGame quits this game
     */
    void quitGame();


protected:
    void onEvent(oxygine::Event* ev);
private:
    QTranslator m_Translator;
    QTimer m_Timer;
    static Mainapp* m_pMainapp;
    static QRandomGenerator randGenerator;
    static bool m_useSeed;

    static QThread m_Workerthread;
    static QThread m_AudioWorker;
    static QThread m_Networkthread;
    spTCPServer m_pGameServer{nullptr};
    AudioThread m_Audiothread;
    WorkerThread m_Worker;


    Settings m_Settings;
    bool m_quit{false};

    QMutex m_Mutex{QMutex::RecursionMode::Recursive};
};

#endif // MAINAPP_H
