#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/Draggable.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"

namespace oxygine
{
    Draggable::~Draggable()
    {
        destroy();
    }

    void Draggable::destroy()
    {
        if (m_dragClient != nullptr)
        {
            if (oxygine::Stage::getStage().get() != nullptr)
            {
                oxygine::Stage::getStage()->removeEventListeners(this);
            }
            m_dragClient->removeEventListeners(this);
            m_dragClient->detach();
            m_dragClient = nullptr;
        }

        m_pressed = false;
    }

    void Draggable::init(Actor* actor)
    {
        destroy();
        m_dragClient = actor;
        actor->addEventListener(TouchEvent::TOUCH_DOWN, EventCallback(this, &Draggable::onEvent));
        actor->addEventListener(TouchEvent::TOUCH_UP, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::startDrag(const QPoint& localCenter)
    {
        m_startTm = Clock::getTimeMS();
        m_pressed = true;
        m_dragPos = localCenter;
        m_clientPos = m_dragClient->getPosition();
        oxygine::Stage::getStage()->addEventListener(TouchEvent::MOVE, EventCallback(this, &Draggable::onEvent));
    }

    void Draggable::onMove(const QPoint& position)
    {
        if (m_pressed && (m_dragEnabled || m_middleButton) &&
            m_dragClient != nullptr)
        {
            QPoint localPos = m_dragClient->stage2local(position);
            QPoint dragOffset = localPos - m_dragPos;
            QPoint converted = convertPosUp(m_dragClient, m_dragClient->getParent(), dragOffset, true);
            QPoint np;
            bool _clientIsParent = true;
            if (!_clientIsParent)
            {
                np = m_clientPos + dragOffset;
            }
            else
            {
                np = m_dragClient->getPosition() + converted;
            }
            auto startPos = m_dragClient->getPosition();
            m_dragClient->setPosition(np);
            snapClient2Bounds();
            if (startPos != m_dragClient->getPosition())
            {
                oxygine::Event pEvent(DragMoveEvent);
                m_dragClient->dispatchEvent(&pEvent);
            }
        }
    }

    bool Draggable::getNoLockForMiddleButton() const
    {
        return m_noLockForMiddleButton;
    }

    void Draggable::setNoLockForMiddleButton(bool newNoLockForMiddleButton)
    {
        m_noLockForMiddleButton = newNoLockForMiddleButton;
    }

    void Draggable::onEvent(Event* event)
    {
        TouchEvent* te = safeCast<TouchEvent*>(event);
        switch (te->type)
        {
            case TouchEvent::TOUCH_DOWN:
            {
                if (te->mouseButton == MouseButton_Middle && m_noLockForMiddleButton)
                {
                    m_middleButton = true;
                }
                startDrag(te->localPosition);
                break;
            }
            case TouchEvent::TOUCH_UP:
            {
                if (m_dragClient != nullptr)
                {
                    m_middleButton = false;
                    m_pressed = false;
                    oxygine::Stage::getStage()->removeEventListeners(this);
                    if (Clock::getTimeMS() - m_startTm < timeMS(2))
                    {
                        m_dragClient->setPosition(m_clientPos);
                    }
                }
                break;
            }

            case TouchEvent::MOVE:
            {
                onMove(te->localPosition);
                break;
            }
        }
    }

    void Draggable::setDragBounds(const QRect& r)
    {
        m_bounds = r;
    }

    bool isParent(Actor* child, Actor* isItParent)
    {
        while (child)
        {
            if (child == isItParent)
            {
                return true;
            }
            child = child->getParent();
        }
        return false;
    }

    Actor* Draggable::getClient()
    {
        return m_dragClient;
    }

    void Draggable::snapClient2Bounds()
    {
        if (m_dragClient != nullptr)
        {
            QPoint np = m_dragClient->getPosition();
            if (m_bounds.width() != -1 && m_bounds.height() != -1)
            {
                np.setX(std::max(np.x(), static_cast<qint32>(m_bounds.x())));
                np.setY(std::max(np.y(), static_cast<qint32>(m_bounds.y())));
                np.setX(std::min(np.x(), static_cast<qint32>(m_bounds.right())));
                np.setY(std::min(np.y(), static_cast<qint32>(m_bounds.bottom())));
            }
            m_dragClient->setPosition(np);
        }
    }

    QPoint Draggable::convertPosUp(Actor* src, Actor* dest, const QPoint& pos, bool direction)
    {
        QPoint locPos(pos);
#ifdef GRAPHICSUPPORT
        QTransform t;
        while (src != dest && src)
        {
            t = src->getTransform() * t;
            src = src->getParent();
        }
        if (direction)
        {
            t.setMatrix(t.m11(), t.m12(), t.m13(),
                        t.m21(), t.m22(), t.m23(),
                        0, 0, t.m33());
        }
        locPos = t.map(locPos);
#endif
        return locPos;
    }

    QPoint Draggable::convertPosDown(Actor* src, Actor* dest, const QPoint& pos, bool direction)
    {
        QPoint locPos(pos);
#ifdef GRAPHICSUPPORT
        QTransform t;
        t = src->getTransform();
        while (src != dest && src)
        {
            t =  t * src->getTransform();
            src = src->getParent();
        }
        if (direction)
        {
            t.setMatrix(t.m11(), t.m12(), t.m13(),
                        t.m21(), t.m22(), t.m23(),
                        0, 0, t.m33());
        }
        locPos = t.map(locPos);
#endif
        return locPos;
    }

}
