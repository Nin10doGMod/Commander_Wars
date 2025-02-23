#ifndef GAMERECORDER_H
#define GAMERECORDER_H

#include <QObject>
#include <QVector3D>

#include "game/gamerecording/daytodayrecord.h"

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAnim.h"

#include "coreengine/LUPDATE_MACROS.h"

class GameMap;

class GameRecorder;
using spGameRecorder = oxygine::intrusive_ptr<GameRecorder>;

class AttackReport;
using spAttackReport = oxygine::intrusive_ptr<AttackReport>;

/**
 * @brief The AttackReport struct
 */
class AttackReport final : public oxygine::ref_counter
{
public:
    qint32 day{0};
    /**
     * @brief defenderKilled
     */
    bool attackerKilled{false};
    /**
     * @brief attackerDamage
     */
    qint32 attackerDamage{0};
    /**
     * @brief attackerX
     */
    qint32  attackerX{-1};
    /**
     * @brief attackerY
     */
    qint32  attackerY{-1};
    /**
     * @brief attackerID
     */
    QString attackerID;
    /**
     * @brief pAttackerOwner
     */
    qint32 attackerOwnerID{-1};
    /**
     * @brief defenderX
     */
    qint32  defenderX{-1};
    /**
     * @brief defenderY
     */
    qint32  defenderY{-1};
    /**
     * @brief defenderDamage
     */
    qint32 defenderDamage{0};
    /**
     * @brief defenderID
     */
    QString defenderID;
    /**
     * @brief defenderOwnerID
     */
    qint32 defenderOwnerID{-1};
    /**
     * @brief defenderKilled
     */
    bool defenderKilled{false};

    bool defenderSeesAttacker{true};
};

Q_DECLARE_INTERFACE(AttackReport, "AttackReport");

class GameRecorder final : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT    
public:
    struct UnitData
    {
        QString unitId;
        qint32 player{-1};
        qint32 count{0};
    };

    struct PlayerData
    {
        QVector<UnitData> producedUnits;
        QVector<UnitData> lostUnits;
        QVector<UnitData> killedUnits;
    };
    ENUM_CLASS Rang
    {
        S,
        A,
        B,
        C,
        D
    };

    explicit GameRecorder(GameMap* pMap);
   virtual ~GameRecorder() = default;
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
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 8;
    }
    /**
     * @brief calculateRang
     * @param player
     * @param scorePoints
     * @return
     */
    Rang calculateRang(qint32 player, QVector3D& scorePoints);
    /**
     * @brief getRank
     * @param score
     * @return
     */
    static Rang getRank(qint32 score);
    /**
     * @brief getRankAnim
     * @param rang
     * @return
     */
    static oxygine::ResAnim* getRankAnim(GameRecorder::Rang rang);

    /**
     * @brief getPlayerDataRecords
     * @return
     */
    const QVector<PlayerData> & getPlayerDataRecords() const;
    /**
     * @brief getAttackLog
     * @param player
     * @return
     */
    QVector<spAttackReport> getAttackLog(qint32 player);
    /**
     * @brief getDayRecord
     * @param index
     * @return
     */
    Q_INVOKABLE DayToDayRecord* getDayRecord(qint32 index)
    {
        if (index >= 0 && index < m_Record.size())
        {
            return m_Record[index].get();
        }
        return nullptr;
    }
    /**
     * @brief getScoreValues
     * @param player
     * @return a vector containing the score values for the player. X = speed Y = power width = technique height = rang
     */
    Q_INVOKABLE QRect getScoreValues(qint32 player);
    /**
     * @brief newDay
     */
    Q_INVOKABLE void newDay();
    /**
     * @brief updatePlayerData
     * @param player
     */
    Q_INVOKABLE void updatePlayerData(qint32 player);
    /**
     * @brief addSpecialEvent
     * @param player
     * @param event
     */
    Q_INVOKABLE void addSpecialEvent(qint32 player, GameEnums::GameRecord_SpecialEvents event);
    /**
     * @brief lostUnit
     * @param player
     * @param unitId is optional to make it upwards compatible
     */
    Q_INVOKABLE void lostUnit(qint32 player, QString unitId = "", qint32 owner = -1);
    /**
     * @brief getLostUnits
     * @param player
     * @return
     */
    Q_INVOKABLE quint32 getLostUnits(qint32 player);
    /**
     * @brief destroyedUnit
     * @param player
     * @param unitId is optional to make it upwards compatible
     */
    Q_INVOKABLE void destroyedUnit(qint32 player, QString unitId = "", qint32 owner = -1);
    /**
     * @brief getDestroyedUnits
     * @param player
     * @return
     */
    Q_INVOKABLE quint32 getDestroyedUnits(qint32 player);
    /**
     * @brief buildUnit
     * @param player
     * @param unitId is optional to make it upwards compatible
     */
    Q_INVOKABLE void buildUnit(qint32 player, const QString unitId = "", qint32 owner = -1);
    /**
     * @brief getBuildedUnits
     * @param player
     * @return
     */
    Q_INVOKABLE quint32 getBuildedUnits(qint32 player);
    /**
     * @brief attacked
     * @param damage
     */
    Q_INVOKABLE void attacked(qint32 player, qreal damage);
    /**
     * @brief getMapTime
     * @return
     */
    Q_INVOKABLE qint32 getMapTime() const;
    /**
     * @brief setMapTime
     * @param mapTime
     */
    Q_INVOKABLE void setMapTime(const qint32 mapTime);
    /**
     * @brief getDeployLimit
     * @return
     */
    Q_INVOKABLE quint32 getDeployLimit() const;
    /**
     * @brief setDeployLimit
     * @param deployLimit
     */
    Q_INVOKABLE void setDeployLimit(const quint32 deployLimit);
    /**
     * @brief logAttack
     * @param day
     * @param attackerDamage
     * @param attackerX
     * @param attackerY
     * @param attackerID
     * @param attackerOwnerID
     * @param defenderX
     * @param defenderY
     * @param defenderDamage
     * @param defenderID
     * @param defenderOwnerID
     */
    Q_INVOKABLE void logAttack(qint32 day,
                   qint32 attackerDamage, qint32 attackerX, qint32 attackerY, const QString attackerID, qint32 attackerOwnerID, bool attackerKilled,
                   qint32 defenderDamage, qint32 defenderX, qint32 defenderY, const QString defenderID, qint32 defenderOwnerID, bool defenderKilled, bool defenderSeesAttacker = true);
private:
    void saveUnitVector(QDataStream& pStream, const QVector<UnitData> & data) const;
    void loadUnitVector(QDataStream& pStream, QVector<UnitData> & data);
    void loadUnitVectorFromMap(QVector<UnitData> & data, QMap<QString, qint32> & info);
    void increaseItem(QVector<UnitData> & data, qint32 player, QString unitId);

private:
    QVector<spDayToDayRecord> m_Record;
    QVector<spAttackReport> m_Attackreports;
    QVector<quint32> m_destroyedUnits;
    QVector<quint32> m_lostUnits;
    QVector<qreal> m_damageDealt;
    QVector<quint32> m_attackNumbers;
    QVector<quint32> m_deployedUnits;
    qint32 m_mapTime{0};
    quint32 m_deployLimit{0};
    QVector<PlayerData> m_playerDataRecords;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(GameRecorder, "GameRecorder");

#endif // GAMERECORDER_H
