CO_GRIT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var tankUnits = CO_GRIT.getDirectUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 70;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 50;
            }
            break;
        default:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 20;
            }
            break;
        }
        if (tankUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -20;
        }
    }
    return 0;
};
CO_GRIT.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (unit.getBaseMaxRange() > 1)
            {
                return 3;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 2;
            }
            break;
        default:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
        }
    }
    return 0;
};
