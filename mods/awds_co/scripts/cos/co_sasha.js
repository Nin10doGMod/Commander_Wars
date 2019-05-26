CO_SASHA.getBonusIncome = function(co, building, income)
{
    if (co.getIsCO0() === true)
    {
        return income * 0.1;
    }
    return 0;
};

CO_SASHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    return 0;
};

CO_SASHA.postBattleActions = function(co, attacker, atkDamage, defender, gotAttacked)
{
    if (co.getIsCO0() === true)
    {
        if (gotAttacked === false)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                // damage can be negativ if we can't do a counter attack the damge is -1
                // avoid loosing money cause of our super power
                if (atkDamage > 0)
                {
                    co.getOwner().addFonds(atkDamage / 10.0 * defender.getUnitCosts() * 0.5);
                }
                break;
            case GameEnums.PowerMode_Power:
                break;
            default:
                break;
            }
        }
    }
};
