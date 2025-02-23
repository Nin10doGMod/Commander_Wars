#ifndef H_SCROLLBAR_H
#define H_SCROLLBAR_H

#include <QElapsedTimer>

#include "objects/base/tooltip.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

class H_Scrollbar;
using spH_Scrollbar = oxygine::intrusive_ptr<H_Scrollbar>;

class H_Scrollbar final : public Tooltip
{
    Q_OBJECT
public:
    /**
     * @brief H_Scrollbar
     * @param heigth heigth of the complete scrollbar. Slider + Both Arrows -> Note: No Checks for a valid value is done here.
     * @param contentHeigth heigth of the area you can scroll with this scrollbar. Needed for the arrows to calculate the add value. :)
     */
    explicit H_Scrollbar(qint32 heigth, qint32 contentHeigth);
   virtual ~H_Scrollbar() = default;
    /**
     * @brief setContentHeigth
     * @param heigth
     */
    void setContentHeigth(qint32 heigth);
    qint32 getContentHeigth() const;

    virtual void update(const oxygine::UpdateState&) override;
    virtual void setHeight(qint32 h) override;
    Q_INVOKABLE void setScrollvalue(float Scrollvalue);
    Q_INVOKABLE bool getSliding() const;
    Q_INVOKABLE void setSliding(bool sliding);
signals:
    /**
     * @brief sigScrollValueChanged emitted when the scroll value changes between 0.0f and 1.0f
     * @param value
     */
    void sigScrollValueChanged(float value);
    /**
     * @brief sigChangeScrollValue
     * @param value
     */
    void sigChangeScrollValue(float value);
public slots:
    void changeScrollValue(float value);
private:
    void scroll(oxygine::Event* pEvent);
private:
    float m_Scrollvalue{0.0f};
    float m_Scrollspeed{1.0f};
    qint32 m_speedCounter{0};
    qint32 m_Heigth;
    qint32 m_ContentHeigth;
    qint32 m_scroll{0};
    bool m_sliding{false};
    oxygine::spBox9Sprite  m_slider;
    QElapsedTimer m_ScrollTimer;

    oxygine::spBox9Sprite m_pBox;
    oxygine::spButton m_pArrowDown;
};

Q_DECLARE_INTERFACE(H_Scrollbar, "H_Scrollbar");

#endif // H_SCROLLBAR_H
