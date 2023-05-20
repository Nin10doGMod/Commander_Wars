var Constructor = function()
{
    
    this.canBePerformed = function(action, map)
    {
        if (!map.getGameRules().getParallelCos())
        {
            let player = map.getCurrentPlayer();
            let co0 = player.getCO(0);
            let co1 = player.getCO(1);
            if (co0 !== null && co1 !== null &&
                co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
                co1.getPowerMode() === GameEnums.PowerMode_Off)
            {
                return false;
            }
        }
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Next player");
    };
    this.getIcon = function(map)
    {
        return "next_player";
    };
    this.perform = function(action, map)
    {
        map.nextTurn();
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getName = function()
    {
        return qsTr("Next player");
    };
    this.getDescription = function()
    {
        return qsTr("Ends your turn and gives control to the next player.");
    };
}


Constructor.prototype = ACTION;
var ACTION_NEXT_PLAYER = new Constructor();
