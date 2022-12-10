CO_KANBEI.getCostModifier = function(co, id, baseCost, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        return baseCost * CO_KANBEI.costIncrease / 100;
    }
    return 0;
};
CO_KANBEI.coZoneBonus = 0;
CO_KANBEI.globalBonus = 30;
CO_KANBEI.costIncrease = 20;
CO_KANBEI.powerAtkBonus = 50;
CO_KANBEI.superPowerAtkBonus = 50;
CO_KANBEI.superPowerCounterBonus = 100;
CO_KANBEI.powerDefBonus = 40;
CO_KANBEI.superPowerDefBonus = 60;
CO_KANBEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender)
            {
                return CO_KANBEI.superPowerCounterBonus;
            }
            return CO_KANBEI.powerBonus;
        case GameEnums.PowerMode_Power:
            return CO_KANBEI.superPowerBonus;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
CO_KANBEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_KANBEI.superPowerDefBonus;
        case GameEnums.PowerMode_Power:
            return CO_KANBEI.powerDefBonus;
        default:
            return CO_KANBEI.globalBonus;
        }
    }
    return 0;
};
