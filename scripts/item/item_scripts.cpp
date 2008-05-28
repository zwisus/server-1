/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Item_Scripts
SD%Complete: 100
SDComment: Items for a range of different items. See content below (in script)
SDCategory: Items
EndScriptData */

/*#####
# item_area_52_special(i28132)       Prevents abuse of this item
# item_draenei_fishing_net(i23654)   Correctly implements chance to spawn item or creature
# item_nether_wraith_beacon(i31742)  Summons creatures for quest Becoming a Spellfire Tailor (q10832)
# item_tame_beast_rods(many)         Prevent cast on any other creature than the intended (for all tame beast quests)
# item_vorenthals_presence(i30259)   Prevents abuse of this item
# item_yehkinyas_bramble(i10699)     Allow cast spell on vale screecher only and remove corpse if cast sucessful (q3520)
# item_zezzak_shard(i31463)          Quest The eyes of Grillok (q10813). Prevents abuse
# item_Sparrowhawk_Net(i32321)       Quest To Catch A Sparrowhawk (q10987). Prevents abuse
# item_Blackwhelp_Net(i31129)        Quest Whelps of the Wyrmcult (q10747). Prevents abuse
# 
#####*/

#include "sc_creature.h"
#include "sc_gossip.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "WorldPacket.h"

/*#####
# item_area_52_special
#####*/

bool ItemUse_item_area_52_special(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if ( player->GetAreaId() == 3803 )
    {
        return false;
    }
    else
    {
        player->SendEquipError(EQUIP_ERR_OUT_OF_RANGE,_Item,NULL);
        return true;
    }
}

/*#####
# item_draenei_fishing_net
#####*/

bool ItemUse_item_draenei_fishing_net(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    Item* item = NULL;
    ItemPosCountVec dest;

    if (player->GetQuestStatus(9452) == QUEST_STATUS_INCOMPLETE)
    {
        if (rand()%100 < 35)
        {
            Creature *Murloc;
            Murloc = player->SummonCreature(17102,player->GetPositionX() ,player->GetPositionY()+20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
            if (Murloc)
                ((CreatureAI*)Murloc->AI())->AttackStart(player);
        }    
        else
        {    
            uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 23614, 1);
            if(msg == EQUIP_ERR_OK)
            {
                item = player->StoreNewItem(dest, 23614, true);
                player->SendNewItem(item, 1, true, false);    

            }

        }
    }
    return false;
}

/*#####
# item_nether_wraith_beacon
#####*/

bool ItemUse_item_nether_wraith_beacon(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if (player->GetQuestStatus(10832) == QUEST_STATUS_INCOMPLETE)
    {
        Creature *Nether;
        Nether = player->SummonCreature(22408,player->GetPositionX() ,player->GetPositionY()+20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
        Nether = player->SummonCreature(22408,player->GetPositionX() ,player->GetPositionY()-20, player->GetPositionZ(), 0,TEMPSUMMON_TIMED_DESPAWN,180000);
        if (Nether)
            ((CreatureAI*)Nether->AI())->AttackStart(player);
    }
    return false;
}

/*#####
# item_tame_beast_rods
#####*/

bool ItemUse_item_tame_beast_rods(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    uint32 itemSpell = _Item->GetProto()->Spells[0].SpellId;
    uint32 cEntry = 0;

    if(itemSpell)
    {
        switch(itemSpell)
        {
            case 19548: cEntry =  1196; break;              //Ice Claw Bear
            case 19674: cEntry =  1126; break;              //Large Crag Boar
            case 19687: cEntry =  1201; break;              //Snow Leopard
            case 19688: cEntry =  2956; break;              //Adult Plainstrider
            case 19689: cEntry =  2959; break;              //Prairie Stalker
            case 19692: cEntry =  2970; break;              //Swoop
            case 19693: cEntry =  1998; break;              //Webwood Lurker
            case 19694: cEntry =  3099; break;              //Dire Mottled Boar
            case 19696: cEntry =  3107; break;              //Surf Crawler
            case 19697: cEntry =  3126; break;              //Armored Scorpid
            case 19699: cEntry =  2043; break;              //Nightsaber Stalker
            case 19700: cEntry =  1996; break;              //Strigid Screecher
            case 30646: cEntry = 17217; break;              //Barbed Crawler
            case 30653: cEntry = 17374; break;              //Greater Timberstrider
            case 30654: cEntry = 17203; break;              //Nightstalker
            case 30099: cEntry = 15650; break;              //Crazed Dragonhawk
            case 30102: cEntry = 15652; break;              //Elder Springpaw
            case 30105: cEntry = 16353; break;              //Mistbat
        }
        if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
            targets.getUnitTarget()->GetEntry() == cEntry )
            return false;
    }

    WorldPacket data(SMSG_CAST_FAILED, (4+2));              // prepare packet error message
    data << uint32(_Item->GetEntry());                      // itemId
    data << uint8(SPELL_FAILED_BAD_TARGETS);                // reason
    player->GetSession()->SendPacket(&data);                // send message: Invalid target

    player->SendEquipError(EQUIP_ERR_NONE,_Item,NULL);      // break spell
    return true;
}

/*#####
# item_vorenthals_presence
#####*/

bool ItemUse_item_vorenthals_presence(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // allow use
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 20132 )
        return false;

    // error
    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

/*#####
# item_yehkinyas_bramble
#####*/

bool ItemUse_item_yehkinyas_bramble(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if (player->GetQuestStatus(3520) == QUEST_STATUS_INCOMPLETE)
    {
        Unit * unit_target = targets.getUnitTarget();
        if( unit_target && 
            unit_target->GetTypeId()==TYPEID_UNIT &&
            unit_target->isDead() &&
            (unit_target->GetEntry()==5307 || unit_target->GetEntry()==5308) )// cast only on corpse 5307 or 5308
        {
            ((Creature*)unit_target)->RemoveCorpse();       // remove corpse for cancelling second use
            return false;                                   // all ok
        }
    }
    WorldPacket data(SMSG_CAST_FAILED, (4+2));              // prepare packet error message
    data << uint32(10699);                                  // itemId
    data << uint8(SPELL_FAILED_BAD_TARGETS);                // reason
    player->GetSession()->SendPacket(&data);                // send message: Bad target
    player->SendEquipError(EQUIP_ERR_NONE,_Item,NULL);      // break spell
    return true;
}

/*#####
# item_zezzak_shard
#####*/

bool ItemUse_item_zezzak_shard(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 19440 )
        return false;

    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

/*#####
# item_Sparrowhawk_Net
#####*/

bool ItemUse_item_Sparrowhawk_Net(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 22979 )
        return false;

    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

/*#####
# item_Blackwhelp_Net
#####*/

bool ItemUse_item_Blackwhelp_Net(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    if( targets.getUnitTarget() && targets.getUnitTarget()->GetTypeId()==TYPEID_UNIT && 
        targets.getUnitTarget()->GetEntry() == 21387 )
        return false;

    player->SendEquipError(EQUIP_ERR_YOU_CAN_NEVER_USE_THAT_ITEM,_Item,NULL);
    return true;
}

void AddSC_item_scripts()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_area_52_special";
    newscript->pItemUse = ItemUse_item_area_52_special;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_draenei_fishing_net";
    newscript->pItemUse = ItemUse_item_draenei_fishing_net;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_nether_wraith_beacon";
    newscript->pItemUse = ItemUse_item_nether_wraith_beacon;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_tame_beast_rods";
    newscript->pItemUse = ItemUse_item_tame_beast_rods;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_vorenthals_presence";
    newscript->pItemUse = ItemUse_item_vorenthals_presence;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_yehkinyas_bramble";
    newscript->pItemUse = ItemUse_item_yehkinyas_bramble;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_zezzaks_shard";
    newscript->pItemUse = ItemUse_item_zezzak_shard;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_Sparrowhawk_Net";
    newscript->pItemUse = ItemUse_item_Sparrowhawk_Net;
    m_scripts[nrscripts++] = newscript;

    newscript = new Script;
    newscript->Name="item_Blackwhelp_Net";
    newscript->pItemUse = ItemUse_item_Blackwhelp_Net;
    m_scripts[nrscripts++] = newscript;
}
