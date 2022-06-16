#ifndef LOBBYMENU_H
#define LOBBYMENU_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/panel.h"
#include "objects/base/tableview.h"

#include "network/tcpclient.h"
#include "network/networkgamedata.h"

#include "multiplayer/networkcommands.h"
#include "multiplayer/password.h"

#include "menue/basemenu.h"

class LobbyMenu;
using spLobbyMenu = oxygine::intrusive_ptr<LobbyMenu>;

class LobbyMenu : public Basemenu
{
    Q_OBJECT
public:
    explicit LobbyMenu();
    virtual ~LobbyMenu() = default;

signals:    
    void sigExitMenue();
    void sigHostServer();
    void sigHostLocal();
    void sigJoinGame();
    void sigJoinAdress();
    void sigObserveGame();
    void sigObserveAdress();
    void sigUpdateGamesView();
public slots:
    void exitMenue();    
    void hostLocal();
    void hostServer();
    void joinGame();
    void joinGamePassword(QString password);
    void join(QString adress, QString password);
    void joinAdress();
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    void updateGamesView();
    void selectGame();
    void observe(QString adress, QString password);
    void observeAdress();
    void observeGame();
    void observeGamePassword(QString password);
    void connected(quint64 socket);
    void onLogin();
    bool isValidEmailAdress(const QString & emailAdress);
    bool isValidPassword(const QString & password);
    void leaveServer();
    void createServerAccount(const QString & password, const QString & emailAdress);
    void loginToServerAccount(const QString & password);
    void resetPasswordOnServerAccount(const QString & emailAdress);
protected slots:
    virtual void onEnter() override;
private:
    void updateGameData(const QJsonObject & objData);
    void joinSlaveGame(const QJsonObject & objData);
    void onPublicKeyCreateAccount(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
    void onPublicKeyLoginAccount(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
    void onPublicKeyResetAccount(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
private:
    spPanel m_pGamesPanel;
    spNetworkInterface m_pTCPClient{nullptr};
    QVector<spNetworkGameData> m_games;
    spNetworkGameData m_currentGame;
    oxygine::spButton m_pButtonHostOnServer;
    spTableView m_Gamesview;
    QString m_password;
    bool m_loggedIn{false};

    Password m_serverPassword;
    QString m_serverEmailAdress;
};

#endif // LOBBYMENU_H
