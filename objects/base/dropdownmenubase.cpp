#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/base/dropdownmenubase.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

DropDownmenuBase::DropDownmenuBase(qint32 width, qint32 itemcount)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DropDownmenuBase");
#endif
    Interpreter::setCppOwnerShip(this);

    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setWidth(width);
    setHeight(40);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("dropdownmenu");
    m_Box = oxygine::spBox9Sprite::create();
    m_Box->setResAnim(pAnim);
    m_Box->setSize(width, 40);
    m_pClipActor = oxygine::spClipRectActor::create();
    m_Box->addChild(m_pClipActor);
    m_pClipActor->setSize(m_Box->getScaledWidth() - 20 - 45, m_Box->getScaledHeight());
    m_pClipActor->setX(10);
    addChild(m_Box);
    qint32 maxItemCount = 2;
    qint32 changedCount = oxygine::Stage::getStage()->getHeight() / 40 / 4;
    if (changedCount > maxItemCount)
    {
        maxItemCount = changedCount;
    }
    if (oxygine::Stage::getStage()->getHeight() / 4 < maxItemCount * 40)
    {
        maxItemCount = oxygine::Stage::getStage()->getHeight() / 4 / 40;
    }
    qint32 scrollHeigth = maxItemCount * 40;
    if (itemcount < maxItemCount)
    {
        scrollHeigth = (itemcount + 1) * 40;
    }
    m_Panel = spPanel::create(false, QSize(width, scrollHeigth), QSize(width, itemcount * 40));
    m_Panel->setSubComponent(true);
    m_Panel->setVisible(false);
    addChild(m_Panel);
    m_pArrowDown = oxygine::spButton::create();
    m_Box->addChild(m_pArrowDown);
    m_pArrowDown->setPosition(m_Box->getScaledWidth() - 45, 10);
    m_pArrowDown->setResAnim(ObjectManager::getInstance()->getResAnim("arrow+down"));
    oxygine::Actor* pPtrDown = m_pArrowDown.get();
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OVER, [pPtrDown](oxygine::Event*)
    {
        pPtrDown->addTween(oxygine::Sprite::TweenAddColor(QColor(16, 16, 16, 0)), oxygine::timeMS(300));
    });
    m_pArrowDown->addEventListener(oxygine::TouchEvent::OUTX, [pPtrDown](oxygine::Event*)
    {
        pPtrDown->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    m_Box->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        if (m_Panel->getVisible())
        {
            emit sigHideDropDown();
        }
        else
        {
            emit sigFocused();
            emit sigShowDropDown();
        }
    });
    connect(this, &DropDownmenuBase::sigItemChangedInternal, this, &DropDownmenuBase::itemChanged, Qt::QueuedConnection);
    connect(this, &DropDownmenuBase::sigShowDropDown, this, &DropDownmenuBase::showDropDown, Qt::QueuedConnection);
    connect(this, &DropDownmenuBase::sigHideDropDown, this, &DropDownmenuBase::hideDropDown, Qt::QueuedConnection);
}

void DropDownmenuBase::changeItemCount(qint32 itemcount)
{
    m_Panel->setContentHeigth(itemcount * 40);
}

void DropDownmenuBase::focusedLost()
{
    emit sigHideDropDown();
}

void DropDownmenuBase::showDropDown()
{
#ifdef GRAPHICSUPPORT
    setPriority(static_cast<qint32>(Mainapp::ZOrder::DropDownList));
    m_Panel->setVisible(true);
    auto transform = computeGlobalTransform();
    if (transform.m32() > oxygine::Stage::getStage()->getHeight() / 2)
    {
        if (m_Panel->getH_Scrollbar()->getVisible())
        {
            m_Panel->setY(-m_Panel->getScaledHeight());
        }
        else
        {
            m_Panel->setY(-m_Panel->getScaledHeight() + m_Panel->getV_Scrollbar()->getScaledHeight() + 7);
        }
    }
    else
    {
        m_Panel->setY(m_Box->getScaledHeight());
    }
#endif
}

void DropDownmenuBase::hideDropDown()
{
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    for (auto & item : m_Items)
    {
        item->setAddColor(QColor(0, 0, 0, 0));
    }
    m_Panel->setVisible(false);
    hideTooltip();
    stopTooltiptimer();
}

void DropDownmenuBase::setEnabled(bool value)
{    
    oxygine::Actor::setEnabled(value);
    m_pArrowDown->setEnabled(value);
    m_Box->setEnabled(value);
    m_Panel->setVisible(false);    
}

qint32 DropDownmenuBase::getCurrentItem() const
{
    return m_currentItem;
}

const QSize& DropDownmenuBase::addDropDownItem(oxygine::spActor item, qint32 id)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("topbar+dropdown");
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    pBox->setResAnim(pAnim);
    pBox->addChild(item);
    pBox->setSize(getWidth() - 33, 40);
    pBox->setPosition(0, 40 * m_Items.size());

    m_Items.append(pBox);
    m_Panel->addItem(pBox);
    // add some event handling :)
    oxygine::Actor* pPtrBox = pBox.get();
    pBox->addEventListener(oxygine::TouchEvent::OVER, [pPtrBox](oxygine::Event*)
    {
        pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(32, 200, 32, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::OUTX, [pPtrBox](oxygine::Event*)
    {
        pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(300));
    });
    pBox->addEventListener(oxygine::TouchEvent::CLICK, [this, id, pPtrBox](oxygine::Event*)
    {
        m_currentItem = id;
        pPtrBox->addTween(oxygine::Sprite::TweenAddColor(QColor(0, 0, 0, 0)), oxygine::timeMS(1));
        emit sigItemChangedInternal(m_currentItem);
        emit sigHideDropDown();
    });
    return pBox->getSize();
}
