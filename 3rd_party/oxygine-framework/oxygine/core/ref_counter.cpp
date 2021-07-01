#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

#include "gameinput/mapmover.h"

#include <qobject.h>
#include <qthread.h>

namespace oxygine
{
    void ref_counter::releaseRef()
    {
        if (0 == --m_ref_counter)
        {
            QObject* pObj = dynamic_cast<QObject*>(this);
            if (pObj == nullptr)
            {
                delete this;
            }
            else if (pObj->thread() == QThread::currentThread())
            {
                delete this;
            }
            else
            {
                oxygine::ResAnim* pObj1 = dynamic_cast<oxygine::ResAnim*>(this);
                MapMover* pObj2 = dynamic_cast<MapMover*>(this);
                if (pObj1 == nullptr &&
                    pObj2 == nullptr)
                {
                    handleErrorPolicy(oxygine::ep_show_error, "deleting object from different thread");
                }
                pObj->deleteLater();
            }
        }
    }
}
