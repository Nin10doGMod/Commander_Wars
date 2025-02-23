var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Cannon");
    };
    this.damageTable = [// ships
                        ["GUNBOAT", 120],
                        ["CANNONBOAT", 120],
                        ["BLACK_BOAT", 120],
                        ["TORPEDOBOAT", 120],
                        ["LANDER", 120],
                        ["FRIGATE", 120],
                        ["DESTROYER", 120],
                        ["CRUISER", 120],
                        ["SUBMARINE", 25],
                        ["BATTLECRUISER", 55],
                        ["BATTLESHIP", 65],
                        ["AIRCRAFTCARRIER", 75],
                        // destroyer can attack ground units
                        ["INFANTRY", 125],
                        ["MECH", 75],
                        ["MOTORBIKE", 75],
                        ["SNIPER", 75],
                        // supporter
                        ["APC", 75],
                        ["FLARE", 75],
                        ["RECON", 75],
                        // tanks
                        ["FLAK", 65],
                        ["HOVERFLAK", 65],
                        ["LIGHT_TANK", 65],
                        ["HOVERCRAFT", 65],
                        // heavy tanks
                        ["HEAVY_HOVERCRAFT", 50],
                        ["HEAVY_TANK", 40],
                        ["NEOTANK", 50],
                        // very heavy tanks
                        ["MEGATANK", 30],
                        // ranged land units
                        ["ARTILLERY", 70],
                        ["ARTILLERYCRAFT", 70],
                        ["ANTITANKCANNON", 55],
                        ["MISSILE", 75],
                        ["ROCKETTHROWER", 75],
                        ["PIPERUNNER", 80],
                        ["HOELLIUM", 20]];

    this.getBaseDamage = function(unit)
    {
        return WEAPON.getDamageFromTable(unit, WEAPON_ANTI_SHIP_CANNON.damageTable, "WEAPON_ANTI_SHIP_CANNON");
    };
};

Constructor.prototype = WEAPON;
var WEAPON_ANTI_SHIP_CANNON = new Constructor();
