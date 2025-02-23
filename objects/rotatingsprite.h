#ifndef ROTATINGSPRITE_H
#define ROTATINGSPRITE_H

#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"

class RotatingSprite;
using spRotatingSprite = oxygine::intrusive_ptr<RotatingSprite>;

class RotatingSprite final : public oxygine::Actor
{
public:
    explicit RotatingSprite();
   virtual ~RotatingSprite() = default;
    void setSprite(oxygine::spActor firstSprite, oxygine::spActor secondSprite);
    qint32 getSpeed() const;
    void setSpeed(const qint32 &value);
    virtual void sizeChanged(const QSize& size) override;
    qint32 getDirection() const;
    void setDirection(const qint32 &value);

protected:
    virtual void doUpdate(const oxygine::UpdateState& us) override;
private:
    oxygine::spClipRectActor m_FirstRect;
    oxygine::spClipRectActor m_SecondRect;
    oxygine::spActor m_FirstSprite;
    oxygine::spActor m_SecondSprite;
    QElapsedTimer m_speedTimer;
    qint32 m_speed{10};
    qint32 m_direction{1};
};

#endif // ROTATINGSPRITE_H
