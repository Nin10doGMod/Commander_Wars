CO_ROBOANDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                         defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return co.getPowerFilled() * 4;
        }
    }
    return 0;
};

CO_ROBOANDY.getBonusMisfortune = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return co.getPowerFilled() * 2;
        }
    }
    return 0;
};


CO_ROBOANDY.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (gotAttacked)
            {
                if (defender.getHp() <= 0)
                {
                    attacker.setHp(attacker.getHp() - 3);
                }
            }
            else
            {
                attacker.setHp(attacker.getHp() + 3);
            }
            break;
        case GameEnums.PowerMode_Power:
            if (gotAttacked)
            {
                if (defender.getHp() <= 0)
                {
                    attacker.setHp(attacker.getHp() - 1);
                }
            }
            else
            {
                attacker.setHp(attacker.getHp() + 1);
            }
            break;
        default:
            break;
        }
    }
};
