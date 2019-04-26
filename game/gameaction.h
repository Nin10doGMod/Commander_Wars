#ifndef GAMEACTION_H
#define GAMEACTION_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QBuffer>
#include <QDataStream>
#include "game/smartpointers.h"
#include "gameinput/menudata.h"
#include "gameinput/markedfielddata.h"
#include "gameinput/cursordata.h"

class GameAction : public QObject
{
    Q_OBJECT

public:
    explicit GameAction();
    explicit GameAction(QString actionID);
    virtual ~GameAction() = default;
    /**
     * @brief setTarget sets the target for the current action
     * @param point
     */
    void setTarget(QPoint point);

    /**
     * @brief getActionText the displayed text for this action
     * @param actionID
     * @return
     */
    static QString getActionText(QString actionID);
    /**
     * @brief getActionIcon the icon shown for this menue item
     * @param actionID
     * @return
     */
    static QString getActionIcon(QString actionID);



signals:

public slots:
    /**
     * @brief getSeed
     * @return
     */
    quint32 getSeed() const;
    /**
     * @brief setActionID sets the action id for this action
     * @param actionID
     */
    void setActionID(QString actionID);
    /**
     * @brief getActionID
     * @return
     */
    QString getActionID();
    /**
     * @brief getInputStep step the ai or player performs for adding info to this action
     * @return
     */
    qint32 getInputStep() const;
    /**
     * @brief setInputStep updates the input step
     * @param value
     */
    void setInputStep(const qint32 &value);
    /**
     * @brief perform executes this action
     */
    void perform();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param x
     * @param y
     * @return
     */
    bool canBePerformed();
    /**
     * @brief canBePerformed checks if this action can be performed
     * @param actionID id of the action we want to check
     * @return
     */
    bool canBePerformed(QString actionID, bool emptyField = false);
    /**
     * @brief isFinalStep
     * @return true if we have all data to perform this action
     */
    bool isFinalStep();
    /**
     * @brief isFinalStep
     * @param actionID id of the action we want to perform
     * @return true if we have all data to perform this action
     */
    bool isFinalStep(QString actionID);
    /**
     * @brief getTargetUnit the unit that will perform the action
     * @return
     */
    Unit* getTargetUnit();
    /**
     * @brief getTargetBuilding the building that will perform the action
     * @return
     */
    Building* getTargetBuilding();
    /**
     * @brief setMovepath sets the movepath for this unit
     * @param points
     */
    void setMovepath(QVector<QPoint> points);
    /**
     * @brief getActionTarget the target action
     * @return
     */
    QPoint getActionTarget();
    /**
     * @brief getTarget the field or unit with which we want to perform the action
     * @return
     */
    QPoint getTarget();
    /**
     * @brief getMovePath
     * @return the move path
     */
    QVector<QPoint> getMovePath();
    /**
     * @brief getMovementTarget the unit on the final movement field
     * @return
     */
    Unit* getMovementTarget();
    /**
     * @brief getMovementBuilding the building on the final movement field
     * @return
     */
    Building* getMovementBuilding();
    /**
     * @brief getCosts gets the costs of this actions
     * @return
     */
    qint32 getCosts() const;
    /**
     * @brief setCosts sets the costs of this actions
     * @param value
     */
    void setCosts(const qint32 &value);
    /**
     * @brief getStepInputType
     * @return the input type used during this step
     */
    QString getStepInputType();
    /**
     * @brief getStepCursor
     * @return the cursor we want to show during this step. Needs to be deleted by the reciever
     */
    CursorData* getStepCursor();
    /**
     * @brief getMenuStepData
     * @return the data needed to create an input menu. the data needs to be deleted by the caller
     */
    MenuData* getMenuStepData();
    /**
     * @brief getMarkedFieldStepData
     * @return
     */
    MarkedFieldData* getMarkedFieldStepData();
    /************** reading and writing data to the action buffer *****************/
    /**
     * @brief writeDataString adds a string to the action data
     * @param data
     */
    void writeDataString(QString data)
    {
        buffer.seek(buffer.size());
        actionData << data;
    }
    /**
     * @brief readDataString
     * @return reads a string from the action data
     */
    QString readDataString()
    {
        QString data;
        actionData >> data;
        return data;
    }
    /**
     * @brief writeDataInt32 adds a int32 to the action data
     * @param data
     */
    void writeDataInt32(qint32 data)
    {
        buffer.seek(buffer.size());
        actionData << data;
    }
    /**
     * @brief readDataInt32
     * @return reads a int32 from the action data
     */
    qint32 readDataInt32()
    {
        qint32 data;
        actionData >> data;
        return data;
    }
    /**
     * @brief writeDataFloat adds a float to the action data
     * @param data
     */
    void writeDataFloat(float data)
    {
        buffer.seek(buffer.size());
        actionData << data;
    }
    /**
     * @brief readDataFloat
     * @return reads a float from the action data
     */
    float readDataFloat()
    {
        float data;
        actionData >> data;
        return data;
    }
    /**
     * @brief startReading starts the reading of the buffer
     */
    void startReading()
    {
        // go to start again
        buffer.seek(0);
    }
    /**
     * @brief deleteAction
     */
    void deleteAction();
private:
    QString m_actionID;
    /**
     * @brief m_target unit, building on which we perfom the action
     */
    QPoint m_target;
    /**
     * @brief m_Movepath the path this unit will go
     */
    QVector<QPoint> m_Movepath;
    /**
      * @brief current input step for tracking when all data is gathered to perform the action
      */
    qint32 inputStep{0};
    /**
      * @brief costs needed to be paid to perform this action
      */
    qint32 costs{0};
    /**
     * @brief actionData data needed to perform this action
     */
    QBuffer buffer;
    QDataStream actionData{&buffer};

    quint32 seed;
};

#endif // GAMEACTION_H
