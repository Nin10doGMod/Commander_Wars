#include "resource_management/unitspritemanager.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"
#include "coreengine/audiomanager.h"

#include "game/player.h"
#include "game/gameanimation/gameanimationwalk.h"
#include "game/gamemap.h"
#include "game/unit.h"
#include "game/co.h"

GameAnimationWalk::GameAnimationWalk(Unit* pUnit, const QVector<QPoint> & movePath, GameMap* pMap)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime), pMap),
      m_pUnit(pUnit),
      m_movePath(movePath)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameAnimationWalk");
#endif
    Interpreter::setCppOwnerShip(this);
    m_pUnit->setUnitVisible(false, nullptr);
    m_frameTime = static_cast<quint32>(GameMap::frameTime / Settings::getInstance()->getWalkAnimationSpeed());
}

void GameAnimationWalk::start()
{
    if (!m_started)
    {
        m_started = true;
        Player* pPlayer = m_pMap->getCurrentViewPlayer();
        if (m_pUnit->isStealthed(pPlayer))
        {
            setVisible(false);
        }
        else
        {
            setVisible(true);
        }
        m_previousAnimation.free();
        doPreAnimationCall();
        AudioManager* pAudioThread = Mainapp::getInstance()->getAudioManager();
        for (auto & data : m_SoundData)
        {
            pAudioThread->playSound(data.soundFile, data.loops, data.delayMs / Settings::getInstance()->getWalkAnimationSpeed(), data.volume);
        }
    }
}

bool GameAnimationWalk::onFinished(bool skipping)
{    
    
    Player* pPlayer = m_pMap->getCurrentViewPlayer();
    Mainapp::getInstance()->getAudioManager()->stopAllSounds();
    if (!m_pUnit->isStealthed(pPlayer))
    {
        m_pUnit->setUnitVisible(true, pPlayer);
    }
    if (m_movePath.size() > 0)
    {
        QPoint pos = m_movePath[0];
        if (m_pMap->getTerrain(pos.x(), pos.y())->getUnit() == nullptr)
        {
            m_pMap->getTerrain(pos.x(), pos.y())->setUnit(m_pUnit);
        }
    }
    bool ret = GameAnimation::onFinished(skipping);
    return ret;
}

GameEnums::Directions GameAnimationWalk::getMovementDirection(qint32 x, qint32 y, qint32 x2, qint32 y2)
{
    if (x == x2)
    {
        if (y < y2)
        {
            return GameEnums::Directions_South;
        }
        else if (y > y2)
        {
            return GameEnums::Directions_North;
        }
    }
    else if (y == y2)
    {
        if (x < x2)
        {
            return GameEnums::Directions_East;
        }
        else if (x > x2)
        {
            return GameEnums::Directions_West;
        }
    }
    return GameEnums::Directions_East;
}

void GameAnimationWalk::loadSprite(const QString spriteID, bool addPlayerColor, float scaling)
{
    if (addPlayerColor)
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_Mask, scaling);
    }
    else
    {
        loadSpriteV2(spriteID, GameEnums::Recoloring_None, scaling);
    }
}

void GameAnimationWalk::loadSpriteV2(const QString spriteID, GameEnums::Recoloring mode, float scaling)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    oxygine::ResAnim* pAnim = pUnitSpriteManager->getResAnim(spriteID);
    if (pAnim != nullptr)
    {
        Player* pPlayer = m_pMap->getCurrentViewPlayer();
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        oxygine::spTweenQueue queueAnimating = oxygine::spTweenQueue::create();
        oxygine::spTweenQueue queueMoving = oxygine::spTweenQueue::create();
        pSprite->setPosition(m_pUnit->Unit::getX() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::getImageSize()) / 2.0f),
                             m_pUnit->Unit::getY() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::getImageSize()) / 2.0f));
        setSize(pAnim->getSize());
        if (m_pUnit->isStealthed(pPlayer))
        {
            setVisible(false);
        }
        pSprite->setScale(scaling);
        if (m_movePath.size() > 1)
        {
            for (qint32 i = m_movePath.size() - 2; i >= 0; i--)
            {
                qint32 x = 0;
                qint32 y = 0;
                GameEnums::Directions direction = GameEnums::Directions_None;
                if (i == m_movePath.size() - 1)
                {
                    direction = getMovementDirection(m_pUnit->Unit::getX(), m_pUnit->Unit::getY(), m_movePath[i].x(), m_movePath[i].y());
                }
                else
                {
                    direction = getMovementDirection(m_movePath[i + 1].x(), m_movePath[i + 1].y(), m_movePath[i].x(), m_movePath[i].y());
                }
                x = m_movePath[i].x() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getWidth() * scaling - GameMap::getImageSize()) / 2.0f);
                y = m_movePath[i].y() * GameMap::getImageSize() - static_cast<qint32>((pAnim->getHeight() * scaling - GameMap::getImageSize()) / 2.0f);

                oxygine::spTween tween1 = oxygine::createTween(oxygine::Actor::TweenPosition(QPoint(x, y)), oxygine::timeMS(m_frameTime * pAnim->getRows()), 1);
                // toggle visibility of the unit
                if (i - 1 >= 0)
                {
                    bool isVisible = true;
                    if (m_pUnit->isStealthed(pPlayer, false, m_movePath[i - 1].x(), m_movePath[i - 1].y()))
                    {
                        isVisible = false;
                    }
                    tween1->addDoneCallback([this, isVisible](oxygine::Event *)
                    {
                        setVisible(isVisible);
                    });
                }

                queueMoving->add(tween1);

                qint32 row = 0;
                if (pAnim->getRows() > 1)
                {
                    switch (direction)
                    {
                        case GameEnums::Directions_North:
                        {
                            row = 1;
                            break;
                        }
                        case GameEnums::Directions_South:
                        {
                            row = 0;
                            break;
                        }
                        case GameEnums::Directions_East:
                        {
                            row = 2;
                            break;
                        }
                        case GameEnums::Directions_West:
                        {
                            row = 3;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                }
                oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, row), oxygine::timeMS(m_frameTime * pAnim->getRows()), 1);
                queueAnimating->add(tween);
                if (i == 0)
                {
                    if(!m_finishQueued)
                    {
                        m_finishQueued = true;
                        queueMoving->addDoneCallback([this](oxygine::Event *)->void
                        {
                            emitFinished();
                        });
                    }
                }
            }
        }
        //
        if (m_movePath.size() <= 1)
        {
            if(!m_finishQueued)
            {
                m_finishQueued = true;
                queueMoving->addDoneCallback([this](oxygine::Event *)->void
                {
                    emitFinished();
                });
            }
        }
        pSprite->addTween(queueMoving);
        pSprite->addTween(queueAnimating);

        // repaint the unit?
        if (mode == GameEnums::Recoloring_Mask)
        {
            QColor color = m_pUnit->getOwner()->getColor();
            pSprite->setColor(color);
        }
        else if (mode == GameEnums::Recoloring_Table ||
                 mode == GameEnums::Recoloring_Matrix)
        {
            bool matrixMode = mode == GameEnums::Recoloring_Matrix;
            pSprite->setColorTable(m_pUnit->getOwner()->getColorTableAnim(), matrixMode);
        }
        else
        {
            pSprite->setPriority(1);
        }

        addChild(pSprite);
    }
    else
    {
        CONSOLE_PRINT_MODULE("Unable to load unit walk sprite: " + spriteID, GameConsole::eDEBUG, GameConsole::eResources);
        emitFinished();
    }
}
