#include "objects/base/v_scrollbar.h"

#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

V_Scrollbar::V_Scrollbar(qint32 width, qint32 contentWidth)
    : m_Width(width),
      m_ContentWidth(contentWidth)
{
#ifdef GRAPHICSUPPORT
    setObjectName("V_Scrollbar");
#endif
    Interpreter::setCppOwnerShip(this);

    m_ScrollTimer.start();
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setSize(width, 33);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("scrollbar");
    m_pBox = oxygine::spBox9Sprite::create();
    m_pBox->setResAnim(pAnim);
    m_pBox->setSize(width, 33);
    addChild(m_pBox);

    m_pArrowRigth = oxygine::spButton::create();
    m_pArrowRigth->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+right"));
    m_pArrowRigth->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    oxygine::Actor* pPtrRight = m_pArrowRigth.get();
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::OVER, [this, pPtrRight](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrRight->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::OUTX, [this, pPtrRight](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrRight->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            m_scroll = 1;
            m_currentScrollspeed = m_Scrollspeed;
            m_speedCounter = 0;
            emit sigStartEditValue();
        }
    });
    m_pArrowRigth->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            m_scroll = 0;
            emit sigEndEditValue(m_Scrollvalue);
        }
    });
    m_pBox->addChild(m_pArrowRigth);
    m_pArrowRigth->setPosition(width - m_pArrowRigth->getScaledWidth() - 8, 9);

    m_pArrowLeft = oxygine::spButton::create();
    // pButton->setPosition(200, 200);
    m_pArrowLeft->setResAnim(ObjectManager::getInstance()->getResAnim("small_arrow+right"));
    m_pArrowLeft->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pArrowLeft->setFlippedX(true);
    oxygine::Actor* pPtrLeft = m_pArrowLeft.get();
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::OVER, [this, pPtrLeft](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrLeft->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::OUTX, [this, pPtrLeft](oxygine::Event*)
    {
        if (m_enabled)
        {
            pPtrLeft->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            m_scroll = -1;
            m_currentScrollspeed = m_Scrollspeed;
            m_speedCounter = 0;
            emit sigStartEditValue();
        }
    });
    m_pArrowLeft->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            m_scroll = 0;
            emit sigEndEditValue(m_Scrollvalue);
        }
    });
    m_pBox->addChild(m_pArrowLeft);
    m_pArrowLeft->setPosition(9, 8);

    m_slider = oxygine::spBox9Sprite::create();
    pAnim = pObjectManager->getResAnim("v_scrollbar");
    m_slider->setResAnim(pAnim);

    qint32 sliderWidth = 50;
    sliderWidth = ((width - m_slider->getScaledWidth() - 20 - 20) * width) / contentWidth;
    if (sliderWidth < 11)
    {
        sliderWidth = 11;
    }
    else if (sliderWidth > (width - m_slider->getScaledWidth() - 20 - 20))
    {
        sliderWidth = (width - m_slider->getScaledWidth() - 20 - 20);
    }

    m_slider->setSize(sliderWidth, 18);
    m_slider->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_slider->setPosition(20, 9);
    m_pBox->addChild(m_slider);
    oxygine::Sprite* pSlider = m_slider.get();
    m_slider->addEventListener(oxygine::TouchEvent::OVER, [this, pObjectManager, pSlider](oxygine::Event*)
    {
        if (m_enabled)
        {
            oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("v_scrollbar_active");
            pSlider->setResAnim(pAnimState);
            pSlider->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
        }
    });
    addEventListener(oxygine::TouchEvent::OUTX, [this, pSlider](oxygine::Event*)
    {
        if (m_enabled)
        {
            setSliding(getSliding());
            pSlider->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this](oxygine::Event* event)
    {
        if (m_enabled)
        {
            setSliding(true);
            emit sigStartEditValue();
            emit sigFocused();
        }
    });
    m_slider->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this](oxygine::Event* event)
    {
        if (m_enabled)
        {
            if (getSliding())
            {
                setSliding(false);
                emit sigEndEditValue(m_Scrollvalue);
            }
            emit sigFocusedLost();
        }
    });
    m_pBox->addEventListener(oxygine::TouchEvent::MOVE, [this](oxygine::Event* pEvent)
    {
        if (m_enabled)
        {
            scroll(pEvent);
        }
    });

    connect(this, &V_Scrollbar::sigChangeScrollValue, this, &V_Scrollbar::changeScrollValue, Qt::QueuedConnection);
}

void V_Scrollbar::focusedLost()
{
    bool emitSignal = getSliding();
    setSliding(false);
    if (emitSignal)
    {
        emit sigEndEditValue(m_Scrollvalue);
    }
}

void V_Scrollbar::scroll(oxygine::Event* pEvent)
{
    if (m_sliding)
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            Tooltip::hideTooltip();
            pTouchEvent->stopPropagation();
            qint32 x = pTouchEvent->localPosition.x() - m_slider->getScaledWidth() / 2;
            if (x < 20)
            {
                x = 20;
            }
            else if (x > m_Width - m_slider->getScaledWidth() - 20)
            {
                x = m_Width - m_slider->getScaledWidth() - 20;
            }
            m_slider->setX(x);
            // calc scroll value :)
            if (m_Width - m_slider->getScaledWidth() - 20 - 20 > 0)
            {
                m_Scrollvalue = static_cast<float>(x - 20) / static_cast<float>(m_Width - m_slider->getScaledWidth() - 20 - 20);
            }
            else
            {
                m_Scrollvalue = 0;
            }
            emit sigScrollValueChanged(m_Scrollvalue);
        }
    }
}

bool V_Scrollbar::getSliding() const
{
    return m_sliding;
}

void V_Scrollbar::setSliding(bool sliding)
{
    
    m_sliding = sliding;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (sliding)
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("v_scrollbar_pressed");
        m_slider->setResAnim(pAnimState);
    }
    else
    {
        oxygine::ResAnim* pAnimState = pObjectManager->getResAnim("v_scrollbar");
        m_slider->setResAnim(pAnimState);
    }
    
}

float V_Scrollbar::getScrollspeed() const
{
    return m_Scrollspeed;
}

void V_Scrollbar::setScrollspeed(float Scrollspeed)
{
    m_Scrollspeed = Scrollspeed;
}

void V_Scrollbar::setContentWidth(qint32 width)
{
    if (m_ContentWidth > 0)
    {
        m_ContentWidth = width;
        qint32 sliderWidth = 50;
        if (m_ContentWidth > 0)
        {
            sliderWidth = ((m_Width - 20 - 20) * m_Width) / m_ContentWidth;
        }
        if (sliderWidth < 11)
        {
            sliderWidth = 11;
        }
        else if (sliderWidth > (m_Width - 20 - 20))
        {
            sliderWidth = (m_Width - 20 - 20);
        }
        m_Scrollvalue = 0;
        m_slider->setSize(sliderWidth, 18);
        setScrollvalue(m_Scrollvalue);
    }
}

void V_Scrollbar::update(const oxygine::UpdateState& us)
{
    // no need to calculate more than we need if we're invisible
    if(m_scroll != 0)
    {
        if (m_ScrollTimer.elapsed() > 250)
        {
            if (m_ContentWidth > m_Width)
            {
                m_speedCounter++;
                if (m_speedCounter % 8 == 0)
                {
                    m_currentScrollspeed *= 2;
                }
                emit sigChangeScrollValue(m_scroll * m_currentScrollspeed * 10.0f / static_cast<float>(m_ContentWidth));
                m_ScrollTimer.start();
            }
        }
    }
    oxygine::Actor::update(us);
}

void V_Scrollbar::changeScrollValue(float value)
{
    if (value != 0.0f)
    {
        m_Scrollvalue += value;
        if (m_Scrollvalue < 0)
        {
            m_Scrollvalue = 0;
        }
        else if (m_Scrollvalue > 1.0f)
        {
            m_Scrollvalue = 1.0f;
        }
        else
        {
            // all fine do nothing
        }
        setScrollvalue(m_Scrollvalue);
        emit sigScrollValueChanged(m_Scrollvalue);
    }
}

float V_Scrollbar::getScrollvalue() const
{
    return m_Scrollvalue;
}

void V_Scrollbar::setScrollvalue(float Scrollvalue)
{    
    if (m_Scrollvalue != Scrollvalue)
    {
        m_Scrollvalue = Scrollvalue;
        if (m_Scrollvalue < 0)
        {
            m_Scrollvalue = 0;
        }
        else if (m_Scrollvalue > 1.0f)
        {
            m_Scrollvalue = 1.0f;
        }
        else
        {
            // all fine do nothing
        }
        m_slider->setX(20 + m_Scrollvalue * (m_Width - m_slider->getScaledWidth() - 20 - 20));
    }
}

void V_Scrollbar::setWidth(qint32 w)
{    
    oxygine::Actor::setWidth(w);
    m_Width = w;
    m_pBox->setWidth(w);
    m_pArrowRigth->setPosition(m_Width - m_pArrowRigth->getScaledWidth() - 8, 9);
    setContentWidth(m_ContentWidth);
}

void V_Scrollbar::setEnabled(bool value)
{
    oxygine::Actor::setEnabled(value);
    m_slider->setEnabled(value);
    m_pBox->setEnabled(value);
    m_pArrowRigth->setEnabled(value);
    m_pArrowLeft->setEnabled(value);
}
