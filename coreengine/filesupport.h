#ifndef FILESUPPORT_H
#define FILESUPPORT_H

#include <QObject>
#include <QDataStream>

class Filesupport : public QObject
{
    Q_OBJECT
public:
    struct StringList
    {
        QString name;
        QStringList items;
    };
    virtual ~Filesupport() = default;
    /**
     * @brief getRuntimeHash
     * @return
     */
    static QByteArray getRuntimeHash(const QStringList & mods);
    /**
     * @brief getHash
     * @param filter
     * @param folders
     * @return
     */
    static QByteArray getHash(const QStringList & filter, const QStringList & folders);
    /**
     * @brief writeByteArray
     * @param stream
     * @param array
     */
    static void writeByteArray(QDataStream& stream, const QByteArray& array);
    /**
     * @brief writeBytes
     * @param stream
     * @param array
     */
    static void writeBytes(QDataStream& stream, const QByteArray& array);
    /**
     * @brief readByteArray
     * @param stream
     * @param array
     */
    static QByteArray readByteArray(QDataStream& stream);

    template<typename _TVectorList>
    static void writeVectorList(QDataStream& stream, const _TVectorList & array)
    {
        stream << static_cast<qint32>(array.size());
        for (qint32 i = 0; i < array.size(); i++)
        {
            stream << array[i];
        }
    }

    template<typename _TType, template<typename T> class _TVectorList>
    static _TVectorList<_TType> readVectorList(QDataStream& stream)
    {
        _TVectorList<_TType> array;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            _TType value = 0;
            stream >> value;
            array.append(value);
        }
        return array;
    }
    /**
     * @brief storeList
     * @param file
     * @param items
     * @param folder
     */
    static void storeList(QString file, const QStringList  &items, QString folder);
    /**
     * @brief readList
     * @param file
     * @return
     */
    static StringList readList(QString file);
    /**
     * @brief readList
     * @param file
     * @param folder
     * @return
     */
    static StringList readList(QString file, QString folder);
    /**
     *
     */
    template<typename _TMap>
    static void writeMap(QDataStream& stream, const _TMap & map)
    {
        stream << static_cast<qint32>(map.size());
        auto iter = map.constBegin();
        while (iter != map.constEnd())
        {
            stream << iter.key();
            stream << iter.value();
            ++iter;
        }
    }
    /**
     *
     */
    template<typename _TKey, typename _TType, template<typename _T1, typename _T2> class _TMap>
    static _TMap<_TKey, _TType> readMap(QDataStream& stream)
    {
        _TMap<_TKey, _TType> map;
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            _TKey key;
            _TType value;
            stream >> key;
            stream >> value;
            map.insert(key, value);
        }
        return map;
    }
signals:

private:
    explicit Filesupport() = default;
};

#endif // HASHING_H
