#ifndef SCRIPTCONDITIONCHECKVARIABLE_H
#define SCRIPTCONDITIONCHECKVARIABLE_H

#include "ingamescriptsupport/conditions/scriptcondition.h"

class ScriptConditionCheckVariable;
using spScriptConditionCheckVariable = oxygine::intrusive_ptr<ScriptConditionCheckVariable>;

class ScriptConditionCheckVariable final : public ScriptCondition
{
    Q_OBJECT
public:
    explicit ScriptConditionCheckVariable(GameMap* pMap);

    qint32 getTeam() const;
    void setTeam(const qint32 &value);
    /**
     * @brief readCondition
     * @param rStream
     */
    virtual void readCondition(QTextStream& rStream, QString line) override;
    /**
     * @brief writeCondition
     * @param rStream
     */
    virtual void writeCondition(QTextStream& rStream) override;
    /**
     * @brief getDescription
     * @return
     */
    virtual QString getDescription() override
    {
        return tr("Check Variable: ") + m_Variable;
    }
    /**
     * @brief writePreCondition
     * @param rStream
     */
    virtual void writePreCondition(QTextStream& rStream) override;
    /**
     * @brief writePostCondition
     * @param rStream
     */
    virtual void writePostCondition(QTextStream& rStream) override;
    /**
     * @brief showEditConditin
     */
    virtual void showEditCondition(spScriptEditor pScriptEditor) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 0;
    }
private:
    QString m_Variable{"myName"};
    QString m_Compare = "===";
    qint32 m_value{0};
    bool m_CampaignVariable{false};
    QString m_executed;
};

#endif // SCRIPTCONDITIONVICTORY_H
