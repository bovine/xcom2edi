// []--------------------------------------------------------[]
// |  Structure of GAME_??\CRAFT.DAT file (submarine editor)  |
// []--------------------------------------------------------[]
struct craft
{
    BYTE shiptype;
    BYTE leftammotype, leftammocount;
    BYTE UNK1[2], rightammotype;
    BYTE inventory[55];
    BYTE rightammocount;
    WORD UNK2[9], fuelremaining;
    WORD UNK3[14];
};


// []-------------------------------------------------------------------[]
// |  structure of the XCOM I GAME_??\SOLDIER.DAT file (soldier import)  |
// []-------------------------------------------------------------------[]
struct old_soldier_type
{
    WORD rank;
    WORD location;
    WORD craft;             // if soldier ARE on craft
    WORD previous_craft;    // if soldier WAS on craft (wounded ones)
    WORD missions, kills;
    WORD recovery;          // recovery time in days
    WORD UNK1;
    BYTE name[26];
    BYTE time, health, stamina, reactions, strength, firing;
    BYTE throwing, UNK2, psionicstr, psionicsk, bravery;
    
    BYTE inctime, inchealth, incstamina, increactions, incstrength;
    BYTE incfiring, incthrowing, UNK3, incbravery;
    
    BYTE armour;
    BYTE incpsionicsk;
    BYTE in_training;
    BYTE new_rank;        // true if rank has just changed
    BYTE sex;             // 0=male, 1=female
    BYTE race;            // 0=European, 1=Caucaso, 2=Asiatic, 3=African
};


// []-----------------------------------------------------------------[]
// |  Structure of GAME_??\ASTORE.DAT file (alien containment editor)  |
// []-----------------------------------------------------------------[]
struct astoredat
{
    BYTE alientype, location, alienrank;
    BYTE unk[9];
};


// []--------------------------------------------------------------------[]
// |  Structure of GAME_??\BASE.DAT file (inventory and building editor)  |
// []--------------------------------------------------------------------[]
struct basedat
{
    BYTE name[16];
    WORD inventory[101];        /* probably shorter */
    BYTE buildings[6][6], completion[6][6];
    BYTE technicians;      /* or vise  */
    BYTE scientists;       /*    versa */
    BYTE unk_status;
    BYTE unk[3];
};


// []----------------------------------------------------------[]
// |  Structure of GAME_??\FACIL.DAT file (facilities details)  |
// []----------------------------------------------------------[]
struct facildat
{
    BYTE unk1[2];
    WORD short construction_cost;
    BYTE maintenance_cost;
    BYTE construction_time;
    BYTE unk2[2];
    WORD short defense_value;
    BYTE hit_ratio;
    BYTE unk3[5];
};


// []---------------------------------------------------------------------[]
// |  Structure of GAME_??\LOC.DAT file (determines base_id of each base)  |
// []---------------------------------------------------------------------[]
struct locdat
{
    BYTE activeif3, basedatidx;
    BYTE unk0[18];
};


// []------------------------------------------------------[]
// |  Structure of GEODATA\OBDATA.DAT file (object editor)  |
// []------------------------------------------------------[]
struct obdata
{
    BYTE name[20];
    BYTE icons[2];
    BYTE damage;               // usually 0 for contact weapons
    BYTE unk2;
    BYTE width, height;        // both 1-3, maybe 0
    BYTE ammotype[3];
    BYTE unk3[2];
    BYTE damagetype;           // 0 - 6 or 7 ? -1 invalid ?
    BYTE accuracy[3];
    BYTE timeunits[3];
    BYTE capacity;             // 0 - 255 (note this is the damage done
                // by contact weapons) (for grenades is the round*2 to detonate on)
    BYTE unk4[3], health;      // 0 - 255 this is how much instantaneous damage
                // it takes to destroy the item (ie make particle grenades easier to destroy ... )
    BYTE weight;               // 0 - 255  (careful with BIG numbers)
    BYTE is_grenade;           // 0 - 3 if set object IS a grenade ONLY
    BYTE unk5;
    BYTE uses_ammo;
    BYTE contact_weapon_only;
    BYTE is_gun;
    BYTE is_ammo;
    BYTE two_handed;
    BYTE is_disruptor;         // 0 - 1 , add waypoints and eat your entire clip of ammo !
    BYTE is_alien;             // 0 - 1 , either is alien or you need to research to use it ?
    BYTE unk6;
};


// []-------------------------------------[]
// |  Structure of GAME_??\OBPOS.DAT file  |
// []-------------------------------------[]
struct obpos
{
    BYTE type,row,col,lvl,unit,loc;    /* loc=body_location */
    WORD load;
    BYTE a1,a2,a3,arm,unk[4];          /* a=ammo, arm=armed time */
};


// []--------------------------------------------------------[]
// |  structure of GAME_??\SOLDIER.DAT file (soldier editor)  |
// []--------------------------------------------------------[]
struct soldier_type
{
    BYTE UNK0, inctime, implantation, UNK1, inchealth, incstrength;
    WORD location, craft, rank, UNK2, kills;
    WORD recovery, missions;
    BYTE UNK3, UNK4, UNK5, incstamina, increactions, incfiring;
    BYTE time, health, stamina, throwing, armour;
    BYTE UNK6, UNK7, firing, strength;
    BYTE name[26];
    BYTE UNK8, incbravery, incthrowing, bravery, mcskill;
    BYTE UNK9, mcstrength, reactions, UNKA;
};


// []----------------------------------------------------[]
// |  Structure of GAME_??\UNITPOS.DAT file (reveal map)  |
// []----------------------------------------------------[]
struct unitpos
{
    BYTE row,col,lvl;
        // map sizes are as follows based on MAP.DAT file size
        //  57600 = 60X x 60Y x 4Z x (4 bytes per tile)
        //  40000 = 50  x 50  x 4  x 4
        //  33600 = 30  x 70  x 4  x 4
        //  28800 = 60  x 60  x 2  x 4
    BYTE unk0[5];
    BYTE sighted,loyalty,visible;
    BYTE unk1[3];
};


// []---------------------------------------------------------[]
// |  Structure of GAME_??\UNITREF.DAT file (tactical editor)  |
// []---------------------------------------------------------[]
struct unitref
{
    BYTE type, rank, unk0[9], heading;
    BYTE time, health, shok, energy, reactions;
    BYTE strength, armour[5], firing, throwing, timemax;
    BYTE healthmax, energymax, reactionsmax, armourmax[5];
    BYTE unk1[3], mcskill, corpse_icon, unk2, unitnum, unk3;
    BYTE alienrank, unk4[10], lefthand, righthand;
    BYTE unk5[2], mcstrength, morale, bravery, unk6[3], fatal[6];
    BYTE unk7[17], name[26], unk8[20];
};


// []---------------------------------------------------------[]
// |  Structure of GAME_??\DIPLOM.DAT file (diplomacy status)  |
// []---------------------------------------------------------[]
struct diplom
{
    WORD zindex;
    WORD status;                /* 0x0003 == Good */
    WORD funds_next_factor;     /* fuzzy: increase = increased funds */
    WORD fund_history[10];      /* funding history */
    WORD funding, incfunding;   /* fuzzy, but seems to be true */
    WORD traitor;               /* 1 == just changed */
    WORD unk[2];
};


#define SHIP 33600
#define SHIP_MAPX 30
#define SHIP_MAPY 70
#define SHIP_MAP0 2
#define SHIP_MAPINC 6

#define MAP50 40000
#define MAP50_MAPX 50
#define MAP50_MAPY 50
#define MAP50_MAP0 5
#define MAP50_MAPINC 10

#define MAP60 57600
#define MAP60_MAPX 60
#define MAP60_MAPY 60
#define MAP60_MAP0 5
#define MAP60_MAPINC 10

#define MAXLEARN 100
#define MAXTILE 1024

