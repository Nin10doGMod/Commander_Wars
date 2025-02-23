#pragma once
#include <QScopedPointer>
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"

namespace oxygine
{
    const qint32 MAX_TOUCHES = 17;

    class Input final
    {
    public:
        static Input & getInstance()
        {
            if (m_instance.isNull())
            {
                m_instance.reset(new Input());
            }
            return *m_instance.get();
        };

       virtual ~Input() = default;
        void cleanup();
        void multiTouchEnabled(bool en);
        /**index should be in range [1, MAX_TOUCHES]*/
        PointerState* getTouchByIndex(pointer_index index);
        qint32 touchID2index(qint64 id);
        PointerState* getTouchByID(qint64 id);
        PointerState* getPointerMouse();
        void sendPointerButtonEvent(spStage & stage, MouseButton button, qint32 x, qint32 y, float pressure, qint32 type, PointerState* ps);
        void sendPointerMotionEvent(spStage & stage, qint32 x, qint32 y, float pressure, PointerState* ps);
        void sendPointerWheelEvent(spStage & stage, const QPoint& dir, PointerState* ps);
    private:
        explicit Input();
    private:
        static QScopedPointer<Input> m_instance;
        PointerState m_pointers[MAX_TOUCHES];
        PointerState m_pointerMouse;
        qint64 m_ids[MAX_TOUCHES + 1];
        bool m_multiTouch;
    };
}
