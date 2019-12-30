#include "objects/dialogattacklog.h"

#include "coreengine/mainapp.h"

#include "game/gamerecording/gamerecorder.h"
#include "game/player.h"
#include "game/gamemap.h"

#include "objects/panel.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogAttackLog::DialogAttackLog(Player* pPlayer)
    : QObject()
{
    GameMap* pMap = GameMap::getInstance();
    m_Log = pMap->getGameRecorder()->getAttackLog(pPlayer->getPlayerID());

    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = QColor(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;


    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(pApp->getSettings()->getWidth() / 2 - pOkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        detach();
        emit sigFinished();
    });

    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);

    pSpriteBox->addChild(pPanel);

    qint32 currentDay = -1;
    qint32 y = 10;
    for (qint32 i = m_Log.size() - 1; i >= 0; i--)
    {
        const spAttackReport& log = m_Log[i];
        if (log->day != currentDay)
        {
            currentDay = log->day;
            oxygine::spTextField pText = new oxygine::TextField();
            pText->setHtmlText(tr("Day ") + QString::number(currentDay));
            pText->setStyle(style);
            pText->setPosition(10, y);
            pPanel->addItem(pText);
            y += 40;
        }

        oxygine::spTextField pText = new oxygine::TextField();
        pText->setHtmlText(tr("Attacker: "));
        pText->setStyle(style);
        pText->setPosition(10, y);
        pPanel->addItem(pText);

        Terrain* pTerrain = pMap->getTerrain(log->attackerX, log->attackerY);
        spTerrain pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pActor->loadSprites();
        pActor->addChild(new Unit(log->attackerID, pMap->getPlayer(log->attackerOwnerID), false));
        pActor->setPosition(140, y + 8);
        pPanel->addItem(pActor);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" at X: ") + QString::number(log->attackerX));
        pText->setStyle(style);
        pText->setPosition(200, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" Y: ") + QString::number(log->attackerY));
        pText->setStyle(style);
        pText->setPosition(300, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" dealt "));
        pText->setStyle(style);
        pText->setPosition(370, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(QString::number(log->attackerDamage) + tr("Hp"));
        pText->setStyle(style);
        pText->setPosition(440, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr("to Defender "));
        pText->setStyle(style);
        pText->setPosition(500, y);
        pPanel->addItem(pText);

        pTerrain = pMap->getTerrain(log->defenderX, log->defenderY);
        pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pActor->loadSprites();
        pActor->addChild(new Unit(log->defenderID, pMap->getPlayer(log->defenderOwnerID), false));
        pActor->setPosition(650, y + 8);
        pPanel->addItem(pActor);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" at X: ") + QString::number(log->defenderX));
        pText->setStyle(style);
        pText->setPosition(700, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" Y: ") + QString::number(log->defenderY));
        pText->setStyle(style);
        pText->setPosition(800, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(tr(" and recieved "));
        pText->setStyle(style);
        pText->setPosition(870, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        pText->setHtmlText(QString::number(log->defenderDamage) + tr("Hp"));
        pText->setStyle(style);
        pText->setPosition(1000, y);
        pPanel->addItem(pText);

        y += 40;
    }
    pPanel->setContentWidth(1100);
    pPanel->setContentHeigth(y + 40);
}
