#ifndef SCRIPTEVENTCHANGEBUILDLIST_H
#define SCRIPTEVENTCHANGEBUILDLIST_H

#include "game/player.h"
#include "ingamescriptsupport/events/scriptevent.h"

class ScriptEventChangeBuildlist;
using spScriptEventChangeBuildlist = oxygine::intrusive_ptr<ScriptEventChangeBuildlist>;

class ScriptEventChangeBuildlist final : public ScriptEvent
{
    Q_OBJECT
public:
    explicit ScriptEventChangeBuildlist(GameMap* pMap);
    /**
     * @brief readEvent
     * @param rStream
     */
    virtual void readEvent(QTextStream& rStream, QString line) override;
    /**
     * @brief writeEvent
     * @param rStream
     */
    virtual void writeEvent(QTextStream& rStream) override;
    /**
     * @brief getPlayer
     * @return
     */
    qint32 getPlayer() const;
    /**
     * @brief setPlayer
     * @param value
     */
    void setPlayer(const qint32 &value);
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Change Buildlist: ") + QString::number(player + 1);
    }
    /**
     * @brief showEditEvent
     */
    virtual void showEditEvent(spScriptEditor pScriptEditor) override;
    /**
     * @brief getUnitID
     * @return
     */
    QString getUnitID() const;
    /**
     * @brief setUnitID
     * @param value
     */
    void setUnitID(const QString &value);
    /**
     * @brief getRemove
     * @return
     */
    bool getRemove() const;
    /**
     * @brief setRemove
     * @param value
     */
    void setRemove(bool value);
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    GameMap* m_pMap{nullptr};
    qint32 player{0};
    QString unitID{"INFANTRY"};
    bool remove{false};
    spPlayer m_dropDownPlayer;
};

#endif // SCRIPTEVENTCHANGEBUILDLIST_H
