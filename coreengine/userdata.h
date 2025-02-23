#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QMap>

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "coreengine/fileserializable.h"
#include "coreengine/scriptvariablefile.h"
#include "game/GameEnums.h"

class Userdata;
using spUserdata = oxygine::intrusive_ptr<Userdata>;

class Userdata final : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    /**
     * @brief The Achievement struct
     */
    struct Achievement
    {
        QString id;
        qint32 progress{0};
        qint32 targetValue{0};
        QString name;
        QString description;
        QString icon;
        QString group;
        bool hide{false};
        bool loaded{false};
    };

    static constexpr qint32 MAX_VICTORY_INFO_PER_MAP = 5;
    /**
     * @brief The MapVictoryInfo struct
     */
    struct MapVictoryInfo
    {
        QStringList co1;
        QStringList co2;
        QVector<qint32> score;
    };

    struct ShopItem
    {
        QString key;
        /**
         * @brief name in the shop
         */
        QString name;
        /**
         * @brief price in the shop
         */
        qint32 price;
        /**
         * @brief buyable unlocked for the shop
         */
        bool buyable;
        /**
         * @brief bought in the shop and available for the player
         */
        bool bought;
        /**
         * @brief itemType used for filterin and showing item icons
         */
        GameEnums::ShopItemType itemType;
    };
   virtual ~Userdata() = default;
    static Userdata* getInstance();
    void changeUser();
    void storeUser();
    void loadShopData();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 6;
    }
    /**
     * @brief addCOStyle
     * @param coid
     * @param file
     * @param colorTable
     * @param maskTable
     */
    void addCOStyle(QString coid, QString file, QImage colorTable, QImage maskTable, bool useColorBox);
    /**
     * @brief removeCOStyle
     * @param coid
     */
    void removeCOStyle(QString coid);
    /**
     * @brief getCOStyle
     * @param coid
     * @return
     */
    std::tuple<QString, QString, QImage, QImage, bool>* getCOStyle(QString coid);
    /**
     * @brief getAchievements
     * @return
     */
    const QVector<Achievement>* getAchievements();
    /**
     * @brief addVictoryForMap
     * @param mapPath
     * @param co1
     * @param co2
     * @param score
     */
    void addVictoryForMap(QString mapPath, QString co1, QString co2, qint32 score);
    /**
     * @brief getVictoryForMap
     * @param mapPath
     * @return
     */
    const MapVictoryInfo * getVictoryForMap(QString mapPath);
    /**
     * @brief getItems
     * @param type
     * @param buyable
     * @param bought
     * @return
     */
    QVector<ShopItem> getItems(GameEnums::ShopItemType type, bool buyable, bool bought);
    /**
     * @brief Userdata::getItems
     * @param type
     * @param bought
     * @return
     */
    QVector<Userdata::ShopItem> getItems(GameEnums::ShopItemType type, bool bought);
    /**
     * @brief release
     */
    void release();

    /**
     * @brief getCredtis
     * @return
     */
    Q_INVOKABLE qint32 getCredtis() const;
    /**
     * @brief setCredtis
     * @param credtis
     */
    Q_INVOKABLE void setCredtis(const qint32 credtis);
    /**
     * @brief addCredtis
     * @param credtis
     */
    Q_INVOKABLE void addCredtis(const qint32 credtis);
    /**
     * @brief addAchievement
     * @param id
     * @param value
     */
    Q_INVOKABLE void increaseAchievement(QString id, qint32 value);
    /**
     * @brief addAchievement
     * @param id
     * @param targetValue
     * @param name
     * @param description
     * @param icon
     * @param hide
     */
    Q_INVOKABLE void addAchievement(QString id, qint32 targetValue, QString name, QString description, QString icon, bool hide = false, QString group = "Unknown");
    /**
     * @brief deleteAchievement
     * @param id
     */
    Q_INVOKABLE void deleteAchievement(QString id);
    /**
     * @brief achieved
     * @param id
     * @return
     */
    Q_INVOKABLE bool achieved(QString id);
    /**
     * @brief getActiveCoStyle
     * @param coid
     */
    Q_INVOKABLE QString getActiveCoStyle(QString coid);
    /**
     * @brief addShopItem adds an item to the shop list
     * @param itemType what sort of item it's used to disable the item and for showing an icon in the shop
     * @param key identifier of the item disabled for a co/skin it's his coid. For a map/campaign it's the path relative to the exe seperated by /
     * @param name shown in the shop
     * @param price for the item to be bought
     * @param buyable if the item is buyable right from the start
     */
    Q_INVOKABLE void addShopItem(GameEnums::ShopItemType itemType, QString key, QString name, qint32 price, bool buyable = false);
    /**
     * @brief removeShopItem
     * @param itemType
     * @param key
     */
    Q_INVOKABLE void removeShopItem(GameEnums::ShopItemType itemType, QString key);
    /**
     * @brief setShopItemBuyable changes if an item can be bought in the shop or not
     * @param itemType
     * @param key
     * @param buyable
     */
    Q_INVOKABLE void setShopItemBuyable(GameEnums::ShopItemType itemType, QString key, bool buyable);
    /**
     * @brief setShopItemBought changes if the item has been bought already can be used to enabe items by progress in a campaign or map.
     * @param itemType
     * @param key
     * @param bought
     */
    Q_INVOKABLE void setShopItemBought(GameEnums::ShopItemType itemType, QString key, bool bought);
    /**
     * @brief unlockAllShopItems (little cheat for testing and for people who don't wanna grind)
     * @param bought if true the items get bought as well
     */
    void unlockAllShopItems(bool bought);
    /**
     * @brief Userdata::getItems
     * @param type
     * @param bought
     * @return
     */
    Q_INVOKABLE QStringList getShopItemsList(GameEnums::ShopItemType type, bool bought);
    /**
     * @brief getScriptVariableFile
     * @param filename
     * @return
     */
    Q_INVOKABLE ScriptVariableFile* getScriptVariableFile(const QString filename);
    /**
     * @brief getUniqueIdentifier
     * @return
     */
    Q_INVOKABLE QString getUniqueIdentifier() const;
    /**
     * @brief setUniqueIdentifier
     * @param newUniqueIdentifier
     */
    Q_INVOKABLE void setUniqueIdentifier(const QString newUniqueIdentifier);
private:
    void showAchieved();
    void reset();
private:
    friend class oxygine::intrusive_ptr<Userdata>;
    explicit Userdata();
private:
    static spUserdata m_pInstance;
    QVector<std::tuple<QString, QString, QImage, QImage, bool>> m_customCOStyles;
    QVector<Achievement> m_achievements;
    QMap<QString, MapVictoryInfo> m_mapVictoryInfo;
    QVector<ShopItem> m_shopItems;
    /**
     * @brief m_credtis money available to spend in the shop
     */
    qint32 m_credtis{0};
    QString m_uniqueIdentifier;
    QVector<spScriptVariableFile> m_scriptVariableFiles;
};

#endif // USERDATA_H
