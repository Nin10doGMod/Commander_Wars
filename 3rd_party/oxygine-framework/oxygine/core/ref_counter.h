#pragma once
#include <atomic>
#include <QMutex>
#include <vector>

namespace oxygine
{
    template <class T>
    class intrusive_ptr;

    /** RefCounter **/
    class ref_counter
    {
    public:
        explicit ref_counter() = default;
        virtual ~ref_counter() = default;

        inline const std::atomic<qint32> & getRefCounter() const
        {
            return m_ref_counter;
        }
        static void trackObject(ref_counter* pObj);
        static qint32 getAlloctedObjectCount()
        {
            return m_instanceCounter;
        }
    private:
        template <class T>
        friend class intrusive_ptr;
        inline void addRef()
        {
            ++m_ref_counter;
        }
        void releaseRef();
        void freeObject();
        ref_counter(const ref_counter&) = delete ;
        const ref_counter& operator=(const ref_counter&) = delete;
        ref_counter(const ref_counter&&) = delete;
        const ref_counter&& operator=(const ref_counter&&) = delete;
    private:
        std::atomic<qint32> m_ref_counter{0};

        static std::atomic<qint32> m_instanceCounter;
#ifdef MEMORYTRACING
        static QMutex m_lock;
        static std::vector<ref_counter*> m_objects;
#endif
    };


}
