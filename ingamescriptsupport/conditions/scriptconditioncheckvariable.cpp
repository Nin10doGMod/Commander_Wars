#include "ingamescriptsupport/conditions/scriptconditioncheckvariable.h"

#include "ingamescriptsupport/scripteditor.h"
#include "ingamescriptsupport/genericbox.h"

#include "resource_management/fontmanager.h"

#include "coreengine/gameconsole.h"

#include "objects/base/spinbox.h"
#include "objects/base/textbox.h"
#include "objects/base/dropdownmenu.h"
#include "objects/base/label.h"

ScriptConditionCheckVariable::ScriptConditionCheckVariable(GameMap* pMap)
    : ScriptCondition(pMap, ConditionType::checkVariable)
{

}

void ScriptConditionCheckVariable::readCondition(QTextStream& rStream, QString line)
{
    CONSOLE_PRINT("Reading ConditionCheckVariable", GameConsole::eDEBUG);
    line = line.simplified();
    line = line.replace("if (", "");
    if (line.startsWith(ScriptData::variables))
    {
        line = line.replace(QString(ScriptData::variables) + ".createVariable(\"", "");
        m_CampaignVariable = false;
    }
    else
    {
        line = line.replace(QString(ScriptData::campaignVariables) + ".createVariable(\"", "");
        m_CampaignVariable = true;
    }
    QStringList items = line.replace("\").readDataInt32() ", "@")
                        .replace(") { // ", "@")
                        .replace(" ", "@").split("@");
    if (items.size() >= 3)
    {
        m_Variable = items[0];
        m_Compare = items[1].replace(">", "&gt;").replace("<", "&lt;");
        m_value = items[2].toInt();
    }
    while (!rStream.atEnd())
    {
        if (readSubCondition(m_pMap, rStream, ConditionCheckVariable, line))
        {
            break;
        }
        spScriptEvent event = ScriptEvent::createReadEvent(m_pMap, rStream, line);
        if (event.get() != nullptr)
        {
            events.append(event);
        }
    }
}

void ScriptConditionCheckVariable::writePreCondition(QTextStream& rStream)
{
    CONSOLE_PRINT("Writing ConditionCheckVariable", GameConsole::eDEBUG);
    m_executed = ScriptData::getVariableName();
    rStream << "        var " << m_executed << " = " << ScriptData::variables << ".createVariable(\"" << m_executed << "\");\n";
    if (subCondition.get() != nullptr)
    {
        subCondition->writePreCondition(rStream);
    }
}

void ScriptConditionCheckVariable::writeCondition(QTextStream& rStream)
{
    QString variableName = ScriptData::variables;
    if (m_CampaignVariable)
    {
        variableName = ScriptData::campaignVariables;
    }
    QString compare = m_Compare;
    compare = compare.replace("&gt;", ">").replace("&lt;", "<");
    rStream << "        if (" + variableName + ".createVariable(\"" + m_Variable + "\").readDataInt32() " + compare + " " + QString::number(m_value) + ") { // "
            << QString::number(getVersion()) << " " << QString(ConditionCheckVariable) + "\n";
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    if (subCondition.get() != nullptr)
    {
        subCondition->writeCondition(rStream);
    }
    rStream << "        } // " + QString(ConditionCheckVariable) + " End\n";
}

void ScriptConditionCheckVariable::writePostCondition(QTextStream& rStream)
{
    ScriptCondition::writePostCondition(rStream);
    for (qint32 i = 0; i < events.size(); i++)
    {
        events[i]->writeEvent(rStream);
    }
    rStream << "            " << m_executed << ".writeDataBool(true);\n";
}

void ScriptConditionCheckVariable::showEditCondition(spScriptEditor pScriptEditor)
{
    spGenericBox pBox = spGenericBox::create();

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 width = 300;

    spLabel pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Variable: "));
    pText->setPosition(30, 30);
    pBox->addItem(pText);
    spTextbox textBox = spTextbox::create(500);
    textBox->setTooltipText(tr("Name of the Variable that should be checked. Try not to use names starting with \"variable\". This name is used by the system."));
    textBox->setPosition(width, 30);
    textBox->setCurrentText(m_Variable);
    connect(textBox.get(), &Textbox::sigTextChanged, this,
            [this](QString value)
    {
        m_Variable = value;
    });
    pBox->addItem(textBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Compare: "));
    pText->setPosition(30, 70);
    pBox->addItem(pText);
    QStringList items = {"===", "!==", "&gt;=", "&lt;="};
    spDropDownmenu dropDown = spDropDownmenu::create(150, items);
    dropDown->setTooltipText(tr("The way how the variable gets compared with the constant.\n=== equal\n!== unequal\n>= greater or equal\n<= smaller or equal"));
    dropDown->setPosition(width, 70);
    dropDown->setCurrentItemText(m_Compare);
    DropDownmenu* pDropDown = dropDown.get();
    connect(dropDown.get(), &DropDownmenu::sigItemChanged, this, [this, pDropDown](qint32)
    {
        m_Compare = pDropDown->getCurrentItemText();
    });
    pBox->addItem(dropDown);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Value: "));
    pText->setPosition(30, 110);
    pBox->addItem(pText);
    spSpinBox spinBox = spSpinBox::create(300, -999999, 999999);
    spinBox->setTooltipText(tr("The value that the variable gets checked against."));
    spinBox->setPosition(width, 110);
    spinBox->setCurrentValue(m_value);
    connect(spinBox.get(), &SpinBox::sigValueChanged, this,
            [this](qreal value)
    {
        m_value = value;
    });
    pBox->addItem(spinBox);

    pText = spLabel::create(width - 10);
    pText->setStyle(style);
    pText->setHtmlText(tr("Campaign Variable: "));
    pText->setPosition(30, 150);
    pBox->addItem(pText);
    spCheckbox checkBox = spCheckbox::create();
    checkBox->setTooltipText(tr("If checked the map needs to be played in a campaign and the variable is avaible during all campaign maps and in the campaign."));
    checkBox->setPosition(width, 150);
    checkBox->setChecked(m_CampaignVariable);
    connect(checkBox.get(), &Checkbox::checkChanged, this,
            [this](bool value)
    {
        m_CampaignVariable = value;
    });
    pBox->addItem(checkBox);

    pScriptEditor->addChild(pBox);
    connect(pBox.get(), &GenericBox::sigFinished, pScriptEditor.get(), &ScriptEditor::updateEvents, Qt::QueuedConnection);
}
