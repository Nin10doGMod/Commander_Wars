var Constructor = function()
{
    this.init = function(co)
    {
        co.setPowerStars(2);
        co.setSuperpowerStars(3);
    };

    this.activatePower = function(co)
    {

        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(false);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_ADDER.loadCOMusic(co);
        audio.playRandom();
    };

    this.activateSuperpower = function(co)
    {
        var dialogAnimation = co.createPowerSentence();
        var powerNameAnimation = co.createPowerScreen(true);
        dialogAnimation.queueAnimation(powerNameAnimation);

        var units = co.getPlayer().getUnits();
        var animations = [];
        var counter = 0;
        units.randomize();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
            if (animations.length < 5)
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5, globals.randInt(0, 400));
                powerNameAnimation.queueAnimation(animation);
                animations.push(animation);
            }
            else
            {
                animation.addSprite("power12", -map.getImageSize() * 2, -map.getImageSize() * 2, 0, 1.5);
                animations[counter].queueAnimation(animation);
                animations[counter] = animation;
                counter++;
                if (counter >= animations.length)
                {
                    counter = 0;
                }
            }
        }
        units.remove();

        audio.clearPlayList();
        CO_ADDER.loadCOMusic(co);
        audio.playRandom();
    };

    this.loadCOMusic = function(co)
    {
        // put the co music in here.
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Power:
                audio.addMusic("resources/music/cos/bh_power.mp3");
                break;
            case GameEnums.PowerMode_Superpower:
                audio.addMusic("resources/music/cos/bh_superpower.mp3");
                break;
            default:
                audio.addMusic("resources/music/cos/adder.mp3")
                break;
        }
    };

    this.getCOUnitRange = function(co)
    {
        return 3;
    };
    this.getCOArmy = function()
    {
        return "BH";
    };
    this.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
    {
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Superpower:
                return 10;
            case GameEnums.PowerMode_Power:
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY)))
                {
                    return 10;
                }
                break;
        }
        return 0;
    };
    this.getMovementPointModifier = function(co, unit)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower)
        {
            return 2;
        }
		else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
        return 0;
    };
	
	this.gainPowerstar = function(co, fondsDamage, x, y)
    {
		// adder gains more power than other co's
        co.setPowerFilled(co.getPowerFilled() + fondsDamage / 10000 * 1.2)
    };

    // CO - Intel
    this.getBio = function()
    {
        return qsTr("A self-absorbed CO who believes his skills are matchless. Second to Hawke in rank.");
    };
    this.getHits = function()
    {
        return qsTr("His own Face");
    };
    this.getMiss = function()
    {
        return qsTr("Dirty Things");
    };
    this.getCODescription = function()
    {
        return qsTr("Adept at making quick command decisions, he stores up energy for his CO Power more rapidly than other CO's.");
    };
    this.getPowerDescription = function()
    {
        return qsTr("Movement range for all units is increased by one space.");
    };
    this.getPowerName = function()
    {
        return qsTr("Sideslip");
    };
    this.getSuperPowerDescription = function()
    {
        return qsTr("Movement range for all units is increased by two spaces.");
    };
    this.getSuperPowerName = function()
    {
        return qsTr("Sidewinder");
    };
    this.getPowerSentences = function()
    {
        return [qsTr("The look of terror on your face... It's absolutely delicious!"),
                qsTr("Heh heh heh... I'm going to enjoy breaking you!"),
                qsTr("Heh heh heh... Can't a guy have a little fun!?"),
                qsTr("Heh heh heh... It only hurts for a moment."),
                qsTr("A battle with me is a greater honor than you deserve!"),
                qsTr("Heh heh heh... Kneel before Adder!"),
                qsTr("Hee hee. Your skills are laughable!"),
                qsTr("It's already too late for you!"),
                qsTr("Let me hear your last words!"),
                qsTr("Check out my new and improved powers!"),
                qsTr("Wriggle worm... wriggle!"),
                qsTr("My venom courses through your veins!")];
    };
    this.getVictorySentences = function()
    {
        return [qsTr("Heh heh heh... What did you expect?"),
				qsTr("My apologies. Should I have gone easier on you?"),
				qsTr("Don't forget me now. That would be a shame.")];
    };
    this.getDefeatSentences = function()
    {
        return [qsTr("Hssss! Today was... unlucky. A bad day. Nothing more."),
                qsTr("It's the blasted weather! That was the problem! Hssss!")];
    };
    this.getName = function()
    {
        return qsTr("Adder");
    };
}

Constructor.prototype = CO;
var CO_ADDER = new Constructor();
