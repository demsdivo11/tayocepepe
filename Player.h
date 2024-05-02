#pragma once
#include <filesystem>
#include <regex>

struct Items {
	uint16_t id = 0, count = 0;
	bool transmuted;
};
struct Friends {
	string name = "";
	bool mute = false, block_trade = false;
	long long last_seen = 0;
	int uid = 0;
};
struct PlayMods {
	int id = 0;
	long long time = 0;
	string user = "";
};

enum AffinityType : uint8_t {
	NO_AFFINITY = 0,
	LOVER = 1, 
	BESTIE = 2
};

string affinity_to_string(AffinityType type) {
	return type == LOVER ? "Lover" : type == BESTIE ? "Bestie" : "None";
}

struct Player {
	bool blmNotip = true;
	bool justnowExit = false;
	bool isFrozen = false;

	struct {

		int mEffect = 0;
		std::int64_t mRange = 10;
		std::int64_t mCooldown = 0;

	} rayman;

	struct HideAndSeek {
		bool Seekers = false;
		bool Hiders = false;
		bool InGame = false;
		bool JoinGame = false;
		int playerBlock = 0;
	} hideseek;
	map<string, int64_t> cdCommands;
	bool knockback = false;
	string pending_affinity;
	double game_temp = 0;
	int pending_affinity_type;
	struct { int lastActionSent = 0, totalActionSent = 0; } packetSent;
	bool hide_logs = false, isBet = false;
	int Mine_Pos = -1;
	//minner
	int musik = 1;
	bool isMinner = true;
	float minnerData = 0.00;
	int levelMinner = 0;
	int flagged = 0, totalFlag = 0;
	bool unlocked_ci = true;
	bool unlocked_af = true;
	bool void_ray_1hit = false;
	int last_fg_mascot = 0;
	int last_bg_mascot = 0;
	int kill_ = 0;
	bool fg_only_mascot = false;
	bool got_tsukoyomi = false;
	bool unlocked_as = false;
	bool genjutsu2 = false;
	bool unlocked_at = true;
	bool unlocked_ab = true;
	bool unlocked_ft = true;
	bool unlocked_fd = true;
	bool unlocked_ss = true;
	bool unlocked_sj = true;
	bool unlocked_hp = false;
	bool unlocked_hc = true;
	bool unlocked_fp = true;
	bool unlocked_fs = true;
	bool Cheat_FastSpin = false;
	long long int lastFlagged = 0, lastRespawn = 0, lastDatabaseSave = 0;
	bool isFlagged = false;
	AffinityType affinity_type;
	int affinity_level;
	bool sharingan_control = false;
	int affinity_with = 0;
	bool big_cheat = false, small_cheat = false;
	bool udah_daftar = false;
	int udahan = 0;
	int raylvl = 1;
	int expray = 0;
	// Clashparkour
	long long Time_Remaining = 0, Time_Remaining_1 = 0;
	bool At_Clashparkour = false, Reach_Tier = false;
	// Wolf
	bool wolf_world = false, end_wolf = false;
	long long Wolf_Time = 0;
	// DNA Proccesor
	int DNAInput = 0;
	int IDDNA1 = 0;
	int IDDNA2 = 0;
	int IDDNA3 = 0;
	bool update = false;
	uint8_t sprite = 0, wild = 6, golem = 6;
	bool adventure_begins = false;
	uint8_t pure_shadow = 0, grow_air_ballon = 0;
	bool kamish = false;
	int dateMasterI_title = 0;
	bool dateMasterB_title = false;

	bool MKW = false, MKP = false; // Minokawa Save
	// Warning System
	int Warning = 0;
	vector<string> Warning_Message{}, Account_Notes{}, Alt_Accounts{};
	vector<string> WM{};
	// Halloween
	int Darking_Sacrifice = 0, Task_Darking = 0, Task_Dark_Ticket = 0, Task_Gift_Growganoth = 0, Task_Mountain = 0;
	int Tomb_X51 = 8000, Tomb_X42_1 = 8000, Tomb_X27 = 8000, Tomb_X71 = 8000, Tomb_X72 = 8000, Tomb_X42_2 = 8000;
	long long Tomb_Time = 0, Tomb_Finish = 0, Race_Time = 0;
	bool TombofGrowganoth = false, OnRace = false;
	//
	int step_punch = 0;
	int eq_a = 0, eq_a_1 = 0;
	bool eq_a_update = false;
	bool CanDB = false;
	bool xenonite_effect = false;
	// Ancestral
	int AncesID = 0, Upgradeto = 0, HowmuchSoulStone = 0, IDCeles = 0, JumlahCeles = 0, IDCrystalized = 0, JumlahCrystalized = 0;
	int DailyRiddles = 0;
	// World Time
	long long World_Timed = 0;
	bool WorldTimed = false;
	// Eye Hair Color
	uint32_t hair_color = 0xFFFFFFFF, eye_drop = 0xFFFFFFFF, eye_lenses = 0xFFFFFFFF;
	// Rift Cape
	bool fixrw = false;
	bool Time_Change = true;
	int Cycle_Time = 30;
	// Cape 1
	int Cape_R_0 = 147, Cape_G_0 = 56, Cape_B_0 = 143, Collar_R_0 = 147, Collar_G_0 = 56, Collar_B_0 = 143;
	bool Cape_Collar_0 = true, Closed_Cape_0 = false, Open_On_Move_0 = true, Aura_0 = true, Aura_1st_0 = false, Aura_2nd_0 = false, Aura_3rd_0 = true;
	// Cape 2
	int Cape_R_1 = 137, Cape_G_1 = 30, Cape_B_1 = 43, Collar_R_1 = 34, Collar_G_1 = 35, Collar_B_1 = 63;
	bool Cape_Collar_1 = true, Closed_Cape_1 = true, Open_On_Move_1 = false, Aura_1 = true, Aura_1st_1 = false, Aura_2nd_1 = true, Aura_3rd_1 = false;
	// Total
	int Cape_Value = 1952541691, Cape_Collor_0_Value = 2402849791, Cape_Collar_0_Value = 2402849791, Cape_Collor_1_Value = 723421695, Cape_Collar_1_Value = 1059267327;
	// End
	// Rift Wings
	bool Wing_Time_Change = true;
	int Wing_Cycle_Time = 30;
	// Wings 1
	int Wing_R_0 = 93, Wing_G_0 = 22, Wing_B_0 = 200, Wing_Metal_R_0 = 220, Wing_Metal_G_0 = 72, Wing_Metal_B_0 = 255;
	bool Open_Wing_0 = false, Closed_Wing_0 = false, Trail_On_0 = true, Stamp_Particle_0 = true, Trail_1st_0 = true, Trail_2nd_0 = false, Trail_3rd_0 = false, Material_1st_0 = true, Material_2nd_0 = false, Material_3rd_0 = false;
	// Wings 2
	int Wing_R_1 = 137, Wing_G_1 = 30, Wing_B_1 = 43, Wing_Metal_R_1 = 34, Wing_Metal_G_1 = 35, Wing_Metal_B_1 = 65;
	bool Open_Wing_1 = false, Closed_Wing_1 = false, Trail_On_1 = true, Stamp_Particle_1 = false, Trail_1st_1 = false, Trail_2nd_1 = true, Trail_3rd_1 = false, Material_1st_1 = false, Material_2nd_1 = true, Material_3rd_1 = false;
	// Total
	int Wing_Value = 104912, Wing_Color_0_Value = 3356909055, Wing_Metal_0_Value = 4282965247, Wing_Color_1_Value = 723421695, Wing_Metal_1_Value = 1059267327;
	// End
	// Infinity Crown
	bool Crown_Time_Change = true;
	int Crown_Cycle_Time = 15;
	// Crown 1
	int Base_R_0 = 255, Base_G_0 = 200, Base_B_0 = 37;
	int Gem_R_0 = 255, Gem_G_0 = 0, Gem_B_0 = 64;
	int Crystal_R_0 = 26, Crystal_G_0 = 45, Crystal_B_0 = 140;
	bool Crown_Floating_Effect_0 = false, Crown_Laser_Beam_0 = true, Crown_Crystals_0 = true, Crown_Rays_0 = true;
	// Crown 2
	int Base_R_1 = 255, Base_G_1 = 255, Base_B_1 = 255;
	int Gem_R_1 = 255, Gem_G_1 = 0, Gem_B_1 = 255;
	int Crystal_R_1 = 0, Crystal_G_1 = 45, Crystal_B_1 = 140;
	bool Crown_Floating_Effect_1 = true, Crown_Laser_Beam_1 = true, Crown_Crystals_1 = true, Crown_Rays_1 = true;
	// Total
	int Crown_Value = 1768716607;
	long long int Crown_Value_0_0 = 4294967295, Crown_Value_0_1 = 4278255615, Crown_Value_0_2 = 4190961919;
	long long int Crown_Value_1_0 = 633929727, Crown_Value_1_1 = 1073807359, Crown_Value_1_2 = 2351766271;
	// End
	// Crown of Season
	int Aura_Season = 2, Trail_Season = 2;
	// End
	// Banner Bandolier
	int Banner_Item = 0, CBanner_Item = 0, Banner_Flag = 0, CBanner_Flag = 0;
	// Magic Magnet
	int Magnet_Item = 0;
	// End
	int Healt = 100;
	int PunchHowmuch = 0;
	bool DrDes = false;
	int lastshirt = 0;
	int lastpants = 0;
	int lastfeet = 0;
	int lastface = 0;
	int lasthand = 0;
	int lastback = 0;
	int lastmask = 0;
	int lastnecklace = 0;
	int lastances = 0;
	int lasthair = 0;

	bool Cursed = false, In_Betting = false, Already_Bet = false;
	long long Cursed_Time = 0;
	int Offer = 0, Gems = 0, MySpin = 0, Bet_What = 0; // 0 = gems 1 = locks

	int carnival_credit = 0;

	// Profile
	bool Show_Achievements = false, Show_OwnedWorlds = false, Show_AccountCreated = false, Show_HomeWorld = false;

	int staged = 0, p_status = 0, trader = 0;
	int s4tb = 0;
	int lvlKit = 0, xpKit = 0;
	bool startedKit = false, kit1 = false, kit2 = false, kit3 = false, kit4 = false, kit5 = false, kit6 = false, kit7 = false, kit8 = false, kit9 = false, kit10 = false;
	/*
	level 5 = mystery boxes x3
level 10 = supporter (if already unlocked = 100k gems)
level 15 = rift cape or rift wings
level 20 = x10 Wooden Booty Chest
level 30 = random gacha items? SSS/SGBC/GBC/SFW
level 40 = exclusive skin (if already unlock = 100k gems)
level 50 = exclusive role titles (if already unlock = 100k gems)
	*/
	// Cheat
	long long int Cheat_AF_Time = 0, Cheat_AF_Since = 0;
	bool Cheat_HideOther = false, Cheat_HideChat = false, isFacingLeft = false, isRemote = false, Cheat_AF = false, Cheat_AF_isRunning = false, Cheat_AB = false, Cheat_Spam = false, Cheat_AutoCollect = false, Cheat_FastPull = false, Cheat_FastDrop = false, Cheat_FastTrash = false, Cheat_Speed = false, Cheat_Jump = false;
	int Cheat_AF_Hit = 0, Cheat_AF_PunchX = 0, Cheat_AF_PunchY = 0, Cheat_AF_PlantingTree = 0, Cheat_Spam_Delay = 3, Cheat_Last_Spam = 0;
	string Cheat_Spam_Text = "`cTayoPs `wBest Server", ChoosingName = "";
	// Lquest
	int quest_step = 1;
	int quest_progress = 0;
	bool quest_active = false;
	string lastquest = "";
	string choosing_quest = "";
	// Cquest
	bool C_QuestActive = false, CQ = false;
	int C_QuestStep = 1, C_QuestProgress = 0, C_ProgressNeeded = 0, C_QuestKind = 0, C_DeliverID = 0;
	//
	bool While_Respawning = false;
	string ip = "", lo = "", last_ip = ""; //real ip
	long long int last_option = 0;
	int option_open = 0;
	vector<string> last_visited_worlds{}, worlds_owned{}, trade_history;
	vector<map<int, int>> trade_items{};
	string name_color = "`0";
	int id = 0, netID = 0, state = 0, trading_with = -1;
	bool trade_accept = false, accept_the_offer = false;
	int x = -1, y = -1, characterState = 0;
	int lastx = -1, lasty = -1;
	int gems = 0;
	bool ttBadge = false, ccBadge = false;
	int usedlocke = 0;
	unsigned long long int xp = 0, last_online = 0;
	bool usedmegaphone = 0;
	map<int, int> ac_{};
	bool hit1 = false, hit2 = false;
	bool shuffle = false, loopsong = false;
	int songend = 0, songselected = 0;
	int skin = 0x8295C3FF;
	int expected_count = 0;
	int punched = 0;
	bool b_logs = false;
	int enter_game = 0;
	int lock = 0;
	int flagmay = 256;
	int crole = 0;
	int punch_sword_time = 0;
	int lockeitem = 0;
	bool ghost = false, invis = false;
	int n = 0; //new player passed save
	string note = "";
	int totaltopup = 0;
	int totalWls = 0, totalNetWorth = 0;
	bool isFastTrash = false, isFastDrop = false;
	bool autoFarming = false;
	int autoFarmItem = 0;
	int autoFarmRange = 0;
	bool autoCollectGem = false;
	bool doubleJump = false;
	int supp = 0, hs = 0;
	int mod = 0, dev = 0, m_h = 0;
	int wls = 0;
	int csn = -1;
	long long int s_t = 0;
	string s_r = "";
	// new role
	int mvp = 0, umod = 0, staff = 0, owner = 0;
	long long int b_t = 0, b_s = 0; // ban seconds
	//int last_infected = 0;
	string b_r = "", b_b = ""; // ban reason & banned by
	string m_r = "", m_b = "";
	int hair = 0, shirt = 0, pants = 0, feet = 0, face = 0, hand = 0, back = 0, mask = 0, necklace = 0, ances = 0; /*clothes*/
	string tankIDName = "", tankIDPass = "", modName = "", requestedName = "", world = "", email = "", discordTag = "", discord = "", country = "", home_world = "", last_wrenched = "", roundedPlaytime = "", lastReqName = "";
	string d_name = "";
	vector<Items> inv{};
	vector<Friends> friends{};
	vector<int> last_friends_selection{};
	bool show_uid = false;
	uint64_t lockInBank = 0;
	int page_number = 26;
	string page_item = "";
	vector<int> pending_friends{};
	vector<string> bans{}, logs{};
	string last_edit = "";
	string growmoji = "(wl)|ā|0&(oops)|ą|0&(sleep)|Ċ|0&(punch)|ċ|0&(bheart)|Ĕ|0&(grow)|Ė|0&(gems)|ė|0&(gtoken)|ę|0&(cry)|ĝ|0&(vend)|Ğ|0&(bunny)|ě|0&(cactus)|ğ|0&(pine)|Ĥ|0&(peace)|ģ|0&(terror)|ġ|0&(troll)|Ġ|0&(evil)|Ģ|0&(fireworks)|Ħ|0&(football)|ĥ|0&(alien)|ħ|0&(party)|Ĩ|0&(pizza)|ĩ|0&(clap)|Ī|0&(song)|ī|0&(ghost)|Ĭ|0&(nuke)|ĭ|0&(halo)|Į|0&(turkey)|į|0&(gift)|İ|0&(cake)|ı|0&(heartarrow)|Ĳ|0&(lucky)|ĳ|0&(shamrock)|Ĵ|0&(grin)|ĵ|0&(ill)|Ķ|0&(eyes)|ķ|0&(weary)|ĸ|0&(moyai)|ļ|0&(plead)|Ľ|0&";
	int m_volume = 100;
	string m_note = "C-F-G#G-F-B#A#G-F-G#G-D#G-C-";
	int i_11818_1 = 0, i_11818_2 = 0;
	int8_t random_fossil = rand() % 3 + 4;
	bool Double_Jump = false, High_Jump = false, Fireproof = false, Punch_Power = false, Punch_Range = false, Speedy = false, Build_Range = false, Speedy_In_Water = false, Punch_Damage = false;
	int opc = 0;
	bool reme_spin = false, qeme_spin = false;
	int cc = 0;
	bool bb = false;
	int vip = 0, vvip = 0, glory = 0, supermod = 0, admin = 0, rb = 0, founder = 0, guardian = 0;
	int gp = 0, gd = 0, glo = 0;
	int w_w = 0, w_d = 0;
	int mds = 0;
	int offergems = 0;
	int confirm_reset = 0;
	bool show_location_ = true, show_friend_notifications_ = true, isVerifying = false, canEnter = false, account2FA = false, isVerified = false;
	int level = 1, firesputout = 0, carnivalgameswon = 0;
	long long playtime = 0;
	long long int account_created = 0, seconds = 0;
	string rid = "", mac = "", meta = "", vid = "", platformid = "", wk = "", gameVersion = "", proto = "", verifiedIP = "", verifiedRID = "", verifiedMAC = "";
	vector<string> accountLogs{};
	map<string, int> achievements{};
	int aAchieved = 0, code2FA = 0, expiredAt = 0;
	string lastmsg = "", lastmsgworld = "";
	int gtwl = 0;
	int c_x = 0, c_y = 0;
	int lavaeffect = 0;
	int fires = 0;
	//cooldowns
	long long int i240 = 0, i756 = 0, i758 = 0;
	bool tmod = 0, unlockRoleSkin = false, titleDoctor = false, titleFisher = false, titleChef = false, titleStartopia = false;
	//billboard
	int b_i = 0, b_a = 0, b_w = 0, b_p = 0;
	//
	bool wide_punch = false;
	long long int last_summon = 0;
	bool wide_place = false;
	int same_input = 0, not_same = 0;
	string last_input_text = "";
	string uuid = "";
	long long int last_inf = 0, last_text_input = 0, last_spam_detection = 0, last_consumable = 0, last_world_enter = 0, last_kickall = 0, last_fish_catch = 0, respawn_time = 0, hand_torch = 0, punch_time = 0, lava_time = 0, world_time = 0, geiger_time = 0, valentine_time = 0, remind_time = 0, warp_time = 0, name_time = 0, address_change = 0;
	int dds = 0;
	int hack_ = 0;
	bool cancel_enter = false;
	int lastwrenchx = 0, lastwrenchy = 0, lastwrenchb = 0, lastchoosenitem = 0, lastchoosennr = 0;
	bool invalid_data = false;
	int pps = 0;
	unsigned int onlineID = 0;
	long long lastres = 0;
	int rescheat = 0;
	long long lpps = 0;
	long long int punch_count = 0;
	int received = 0;
	int geiger_ = 0;
	int fishing_used = 0, f_x = 0, f_y = 0, move_warning = 0, f_xy = 0, punch_warning = 0, fish_seconds = 0;
	vector<int> glo_p, lvl_p;
	int geiger_x = -1, geiger_y = -1;
	int t_xp = 0, bb_xp = 0, g_xp = 0, p_xp = 0, t_lvl = 0, bb_lvl = 0, g_lvl = 0, p_lvl = 0, ff_xp = 0, ff_lvl = 0, s_lvl = 0, s_xp = 0;
	vector<int> t_p, bb_p, p_p, g_p, ff_p, surg_p;
	bool saved_on_close = false;
	int booty_broken = 0;
	int dd = 0;
	bool AlreadyDailyQ = false;
	bool isRespawning = false;
	bool isStateUpdated = false;
	vector<PlayMods> playmods{};
	int b_l = 1;
	vector<pair<int, int>> bp;
	int choosenitem = 0;
	int promo = 0, flagset = 0, warn = 0;
	int radio = 0;
	int tk = 0;
	int b_ra = 0, b_lvl = 1;
	int magnetron_id = 0;
	int magnetron_x = 0;
	int magnetron_y = 0;
	vector<string> gr;
	long long int save_time = 0;
	vector<vector<long long>> completed_blarneys{
		{1, 0},
		{2, 0},
		{3, 0},
		{4, 0},
		{5, 0},
		{6, 0},
		{7, 0},
		{8, 0},
		{9, 0},
	};
	vector<string> transmute{};
	vector<string> temptransmute{};
	bool updtrans = false;
	bool block_trade = false;
	int p_x = 0;
	int p_y = 0;
	int guild_id = 0;
	int uid = 0;
	uint32_t pending_guild = 0;
	int is_legend = false;
	int is_datemaster = false;
	int legend = false;
	int pps2 = 0;
	long long lpps2 = 0;
	int pps23 = 0;
	long long lpps23 = 0;
	int exitwarn = 0;
	int last_exit = 0;
	int roleIcon = 6;
	int roleSkin = 6;

	struct GhostEvents {
		int level = 0;
	} ghosts;

	int snackLevel = 0;

	//Max Invis
	std::string superInvisName;
	int superInvisFace = -1;
	bool isSuperInvis = false;
	//
	struct PlayerEvents {

		std::uint64_t chocolates = 0;

	} valentine;

	vector<int> available_surg_items;
	// Stats variables
	bool sounded = false;
	bool labworked = false;
	bool fixed = false;
	bool fixable = false;
	bool flu = false;
	int pulse = 40;
	int site = 0;
	int sleep = 0;
	int dirt = 0;
	int broken = 0;
	int shattered = 0;
	int incisions = 0;
	int bleeding = 0;
	int incneeded = 0;
	int heart = 0;
	double temp = 98.6;
	double fever = 0.0;
	string pretext = "";
	string fixtext = "";
	string postext = "";
	string scantext = "";
	string tooltext = "The patient is prepped for surgery.";
	string endtext = "";
	int failchance = 0;
	bool s = true;
	bool antibs = false;

	int spongUsed = 0;
	int scalpUsed = 0;
	int stitcUsed = 0;
	int antibUsed = 0;
	int antisUsed = 0;
	int ultraUsed = 0;
	int labkiUsed = 0;
	int anestUsed = 0;
	int defibUsed = 0;
	int splinUsed = 0;
	int pinsUsed = 0;
	int clampUsed = 0;
	int transUsed = 0;

	int surgery_skill = 0;
	int surgery_done = 0;
	int su_8552_1 = 0;
	int su_8552_2 = 0;
	bool surgery_started = false;
	int started_type = 0;
	string surgery_world = "";
	string surged_person = "";
	string surged_display = "";
	int surgery_type = -1;
	string surgery_name = "";
	bool hospital_bed = false;
	int egg = 0;

	bool mercy = false;
	bool drtitle = false;
	bool drt = false;
	bool lvl125 = false;
	int donor = false;
	bool spotlight = false;
	int master = false;
	int have_master = false;
	int have_donor = false;
	bool hide_reseller = false;
	/*
	int grow4good_day = 0;
	int grow4good_rarity = 0;
	int grow4good_total_rarity = 0;
	int grow4good_wl = 0;
	int grow4good_total_wl = 0;
	bool grow4good_donate_gems = false;
	int grow4good_gems = 0;
	bool grow4good_purchase_waving = false;
	int grow4good_surgery = 0;
	int grow4good_fish = 0;
	int grow4good_points = 0;
	int grow4good_claim_prize = 0;
	int grow4good_claimed_prize = 0;
	int grow4good_break = 0;
	int grow4good_place = 0;
	int grow4good_trade = 0;
	int grow4good_sb = 0;

	int donate_count = 0;
	*/
	int hp = 100;
	string old_name = "";
	int choose_fg = 0;
	int choose_bg = 0;
	int last_infected = 0;
	int last_punched = 0;
	int pinata_day = 0;
	bool pinata_prize = false;
	bool pinata_claimed = false;
	std::string chatColor;
	bool honors = false;
	string game_version = "";
	double gver = 0.0;
	bool pc_player = false;
	bool bypass = false, update_player = false, bypass2 = false;
	int new_packets = 0;
	int ausd = 0;
	long long last_open_packet = 0;
	int packet_to_much = 0;
	bool in_enter_game = false;
	bool isIn = false;
	bool isUpdating = false;
	bool kill_mode = false;
	bool genjutsu = false;
	string uidtoken = "";
	string gid = "";
	//long long fishing_begin = 0;
	bool izanagi = false;
	int hack2_ = 0;
	bool ignore_cheats2 = false, ignore_cheatss = false, ignore_cheats = false;
	int ignore_cheatss2 = 0;
	int fishing_score = 0;
	long long fishing_until = 0;
	//int fishing_until = 0;
	string fish_water = "";
	int foundg = 0, minesleft = 0;
	bool lostt = false;
	bool endedit = true;
	vector<int> mines{};
	vector<int> clickm{};
	int bM = 0;

	// PVP
	string PVP_Enemy = "";
	bool PVP_Ongoing = false, PVP_Deactive = false;
	int PVP_Bet = 0, PVP_netID = -1, PVP_Point = 0, PVP_Hit = 10;
	long long int PVP_Time = 0;
};
#define pInfo(peer) ((Player*)(peer->data))
struct PlayerMoving {
	int32_t netID, effect_flags_check;
	int packetType, characterState, plantingTree, punchX, punchY, secondnetID;
	float x, y, XSpeed, YSpeed;
};
struct FishMoving {
	int8_t packetType = 31, stopped_fishing;
	int32_t netID, x, y;
};
namespace pData::Algorithm {
	Friends* get_friend(ENetPeer* peer, string const& name) {
		if (auto it = std::ranges::find_if(pInfo(peer)->friends, [name](Friends const& fr) { return fr.name == name; }); it != pInfo(peer)->friends.end())
			return &(*it);
		return nullptr;
	}
	vector<PlayMods>::iterator get_playmod(ENetPeer* peer, int const& id) {
		return std::ranges::find_if(pInfo(peer)->playmods, [id](PlayMods const& mod) { return mod.id == id; });
	}
	vector<Items>::iterator get_inventory_it(ENetPeer* peer, int const& id) {
		return std::ranges::find_if(pInfo(peer)->inv, [id](Items const& item) { return item.id == id; });
	}
	int inventory_empty_slot(ENetPeer* peer, int const& itemId) {
		int slot = 0;
		for (Items const& item : pInfo(peer)->inv) {
			if ((item.id == itemId || item.id == 0) && item.count < 200) slot++;
		}
		return slot;
	}
	bool inventory_contains(ENetPeer* peer, int const& itemId, int const& quantity) {
		auto it = get_inventory_it(peer, itemId);
		if (it == pInfo(peer)->inv.end()) return false;

		if (it->count < quantity) return false;

		return true;
	}
	ENetPeer* get_player(string const& name) {
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
			if (to_lower(pInfo(currentPeer)->tankIDName) == name) return currentPeer;
		}
		return nullptr;
	}
	optional<json> get_inventory_database(string const& player) {
		ifstream fstream("players/" + player + "_.json");
		if (fstream.fail()) return nullopt;

		json j;
		fstream >> j;

		return j["inv"];
	}
	template<typename Func>
	void loop_players(Func function) {
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; currentPeer++) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
			function(currentPeer);
		}
	}
	pair<int, int> count_players(string const& world, bool includeHidden = true) {
		pair<int, int> total = { 0, 0 };
		loop_players([world, &total, includeHidden](ENetPeer* currentPeer) {
			total.second++;
			if (pInfo(currentPeer)->world == world) {
				if (pInfo(currentPeer)->invis && !includeHidden) return;
				total.first++;
			}
			});
		return total;
	}

	void modify_inventory_database(string const& player, int const& itemId, int quantity, bool const& set = false) {
		ifstream fstream("database/players/" + player + "_.json");
		if (fstream.fail()) return;

		json j;
		fstream >> j;

		for (size_t i = 0; json & jObject : j["inv"]) {
			if (jObject["i"].get<int>() == itemId) {
				int dataQuantity = jObject["c"].get<int>();
				if (dataQuantity + quantity <= 0) {
					j["inv"].erase(i);
					break;
				}

				jObject["c"] = quantity + (set ? 0 : dataQuantity);
			}
			i++;
		}

		ofstream ostream("database/players/" + player + "_.json");
		ostream << j;
	}
	string get_display_name(ENetPeer* peer) {
		return pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color + pInfo(peer)->tankIDName : pInfo(peer)->d_name;
	}
	template<typename T>
	T get_json_object(string const& player, string const& object) {
		ifstream istream("database/players/" + player + "_.json");
		json j;
		istream >> j;

		return j[object].get<T>();
	}
	optional<Items> get_inventory(ENetPeer* peer, int const& itemId) {
		if (auto it = std::ranges::find_if(pInfo(peer)->inv, [itemId](Items const& item) { return item.id == itemId; }); it != pInfo(peer)->inv.end())
			return *it;
		return nullopt;
	}
}
namespace player::dataserver {
	std::string get_display_name(ENetPeer* peer) {
		if (peer == nullptr || peer->data == nullptr) return "";

		try {
			return pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color + pInfo(peer)->tankIDName : pInfo(peer)->d_name;
		}
		catch (...) { return ""; }
	}
	template<typename T>
	T get_json_object(std::string const& player, std::string const& object) {
		std::ifstream istream("database/players/" + player + "_.json");

		if (!istream.is_open()) {
			return T{};
		}

		nlohmann::json j;
		istream >> j;
		istream.close();
		return j[object].get<T>();
	}
}
void save_promoters() {
	json j;
	ofstream save_("database/promoters.json");
	j["promoter"] = promoter;
	save_ << j;
	save_.close();
}
void save_reseller() {
	json j;
	ofstream save_("database/resellers.json");
	j["reseller"] = reseller;
	save_ << j;
	save_.close();
}
bool is_reseller(ENetPeer* peer) {
	return find(reseller.begin(), reseller.end(), pInfo(peer)->uid) != reseller.end();
}
bool is_reseller(int uid) {
	return find(reseller.begin(), reseller.end(), uid) != reseller.end();
}
BYTE* packPlayerMoving(PlayerMoving* dataStruct, int size_ = 56) {
	BYTE* data = new BYTE[size_];
	memset(data, 0, size_);
	memcpy(data + 0, &dataStruct->packetType, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 12, &dataStruct->characterState, 4);
	memcpy(data + 20, &dataStruct->plantingTree, 4);
	memcpy(data + 24, &dataStruct->x, 4);
	memcpy(data + 28, &dataStruct->y, 4);
	memcpy(data + 32, &dataStruct->XSpeed, 4);
	memcpy(data + 36, &dataStruct->YSpeed, 4);
	memcpy(data + 44, &dataStruct->punchX, 4);
	memcpy(data + 48, &dataStruct->punchY, 4);
	return data;
}
BYTE* packFishMoving(FishMoving* dataStruct, int size_ = 56) {
	BYTE* data = new BYTE[size_];
	memset(data, 0, size_);
	memcpy(data + 0, &dataStruct->packetType, 4);
	memcpy(data + 3, &dataStruct->stopped_fishing, 4);
	memcpy(data + 4, &dataStruct->netID, 4);
	memcpy(data + 44, &dataStruct->x, 4);
	memcpy(data + 48, &dataStruct->y, 4);
	return data;
}
void send_raw(ENetPeer* peer, int a1, void* packetData, int packetDataSize, int packetFlag) {
	ENetPacket* p;
	if (peer) {
		if (a1 == 4 && *((BYTE*)packetData + 12) & 8) {
			p = enet_packet_create(0, packetDataSize + *((DWORD*)packetData + 13) + 5, packetFlag);
			int four = 4;
			memcpy(p->data, &four, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
			memcpy((char*)p->data + packetDataSize + 4, 0, *((DWORD*)packetData + 13));
		}
		else {
			p = enet_packet_create(0, packetDataSize + 5, packetFlag);
			memcpy(p->data, &a1, 4);
			memcpy((char*)p->data + 4, packetData, packetDataSize);
		}
		enet_peer_send(peer, 0, p);
	}
}
inline string currentTime()
{
	auto now = time(nullptr);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%R", &tstruct);
	return buf;
}
inline string uptimeText(int n) {
	std::string x = "";
	auto day = n / (24 * 3600);
	if (day != 0) 
		x.append(std::format("{} day{}", day, day <= 1 ? "s" : ""));
	
	n = n % (24 * 3600);
	auto hour = n / 3600;

	if (hour != 0)
		if (x.length() > 1) 
			x.append(", ");
	if (hour != 0) 
		x.append(std::format("{} hour{}", hour, hour <= 1 ? "s" : ""));
	
	n %= 3600;
	auto minutes = n / 60;
	if (minutes != 0) 
		if (x.length() > 1)
			x.append(", ");

	if (minutes != 0) 
		x.append(std::format("{} min{}", minutes, minutes <= 1 ? "s" : ""));

	n %= 60;
	auto seconds = n;
	if (seconds != 0) 
		if (x.length() > 1)
			x.append(", ");
	if (seconds != 0)
		x.append(std::format("{} sec{}", seconds, seconds <= 1 ? "s" : ""));

	return x;
}
void send_inventory(ENetPeer* peer) {
	//if (pInfo(peer)->inventory_sent == false) pInfo(peer)->inventory_sent = true;
	__int16 inv_size = pInfo(peer)->inv.size(), packetLen = 66 + (inv_size * 4) + 4;
	BYTE* d_ = new BYTE[packetLen];
	memset(d_, 0, packetLen);
	*(__int8*)(d_) = 4;
	*(__int8*)(d_ + 4) = 9;
	*(__int8*)(d_ + 8) = -1;
	*(__int8*)(d_ + 16) = 8;
	*(int*)(d_ + 56) = 6 + (inv_size * 4) + 4;
	*(__int8*)(d_ + 60) = 1;
	*(int*)(d_ + 61) = inv_size;
	*(int*)(d_ + 65) = inv_size;
	vector<pair<uint16_t, uint8_t>> send_later;
	int offset = 67;
	for (__int16 i_ = 0; i_ < inv_size; ++i_) {
		if (pInfo(peer)->inv[i_].id == 9148 || pInfo(peer)->inv[i_].id == 9636 || pInfo(peer)->inv[i_].id == 9500 || pInfo(peer)->inv[i_].id == 9584 || pInfo(peer)->inv[i_].id == 9586 || pInfo(peer)->inv[i_].id == 9588 || pInfo(peer)->inv[i_].id == 9590 || pInfo(peer)->inv[i_].id == 9550 || pInfo(peer)->inv[i_].id == 9582 || pInfo(peer)->inv[i_].id == 9580 || pInfo(peer)->inv[i_].id == 9578 || pInfo(peer)->inv[i_].id == 9572 || pInfo(peer)->inv[i_].id == 9570 || pInfo(peer)->inv[i_].id == 9568 || pInfo(peer)->inv[i_].id == 9566 || pInfo(peer)->inv[i_].id == 9564 || pInfo(peer)->inv[i_].id == 9562 || pInfo(peer)->inv[i_].id == 9560 || pInfo(peer)->inv[i_].id == 9558 || pInfo(peer)->inv[i_].id == 9490 || pInfo(peer)->inv[i_].id == 9492 || pInfo(peer)->inv[i_].id == 9490 || pInfo(peer)->inv[i_].id == 9492 || pInfo(peer)->inv[i_].id == 9516 || pInfo(peer)->inv[i_].id == 9540) {
			send_later.push_back(make_pair(pInfo(peer)->inv[i_].id, pInfo(peer)->inv[i_].count));
		}
		else {
			*(__int16*)(d_ + offset) = pInfo(peer)->inv[i_].id; offset += 2;
			*(__int8*)(d_ + offset) = pInfo(peer)->inv[i_].count; offset += 2;
		}
	}
	ENetPacket* const p_ = enet_packet_create(d_, packetLen, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, p_);
	delete[] d_;
	if (send_later.size() > 0) {
		PlayerMoving data_{};
		data_.packetType = 13;
		for (int i = 0; i < send_later.size(); i++) {
			data_.plantingTree = send_later[i].first;
			BYTE* raw = packPlayerMoving(&data_);
			raw[3] = send_later[i].second;
			send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
			delete[]raw;
		}
	}
}
bool HasRole(Player* p) {
	return p->vip + p->vvip + p->mod + p->supermod + p->glory + p->admin + p->dev >= 1;
}

string aff_lvl_mod(Player* p_) {
	if (p_->affinity_level <= 0 || p_->affinity_level > 5 || p_->affinity_with <= 0 || p_->affinity_type == NO_AFFINITY) return "";
	string buff = "";
	switch (p_->affinity_level) {
	case 1: buff = "Reduce Provider Growth"; break;
	case 2: buff = "Reduce Tree Growth"; break;
	case 3: buff = "Extra XP"; break;
	case 4: buff = "Extra Block"; break;
	case 5: buff = "Extra Gems"; break;
	default: buff = "None"; break;
	}
	return format("{} Level {}: {}", p_->affinity_type == BESTIE ? "Power of Friendship" : "Power of Love", p_->affinity_level, buff);
}

string aff_lvl_mod_console(Player* p_) {
	if (p_->affinity_level <= 0 || p_->affinity_level > 5 || p_->affinity_with <= 0 || p_->affinity_type == NO_AFFINITY) return "";
	string buff = "";
	switch (p_->affinity_level) {
	case 1: buff = "Reduce Provider Growth"; break;
	case 2: buff = "Reduce Tree Growth"; break;
	case 3: buff = "Extra XP"; break;
	case 4: buff = "Extra Block"; break;
	case 5: buff = "Extra Gems"; break;
	default: buff = "None"; break;
	}
	return format("{} (Level {}: {} mod added)", p_->affinity_type == BESTIE ? "Power of Friendship" : "Power of Love", p_->affinity_level, buff);
}
int get_role_level(ENetPeer* peer) {
	if (to_lower(pInfo(peer)->tankIDName) == "iDems") return 1337;
	else if (pInfo(peer)->founder) return 13;
	else if (pInfo(peer)->owner) return 12;
	else if (pInfo(peer)->staff) return 11;
	else if (pInfo(peer)->dev) return 10;
	else if (pInfo(peer)->admin || is_reseller(pInfo(peer)->uid)) return 9;
	else if (pInfo(peer)->umod) return 8;
	else if (pInfo(peer)->supermod) return 7;
	else if (pInfo(peer)->guardian) return 6;
	else if (pInfo(peer)->mod) return 5;
	else if (pInfo(peer)->glory) return 4;
	else if (pInfo(peer)->mvp) return 3;
	else if (pInfo(peer)->vvip) return 2;
	else if (pInfo(peer)->vip) return 1;
	else return 0;
	return 0;
}
string get_default_role_name(ENetPeer* peer) {
	int level = get_role_level(peer);
	switch (level) {
	case 13: return "founder";

	case 12: return "owner";

	case 11: return "staff";
	case 10: return "dev";

	case 9: return "admin";
	case 8: return "umod";

	case 7: return "smod";
	case 6: return "guardian";

	case 5: return "mod";
	case 4: return "glory";
	case 3: return "mvp";

	case 2: return "vvip";
	case 1: return "vip";
	default: return "default";
	}
	return "default";
}

string get_role_name(ENetPeer* peer) {
	int level = get_role_level(peer);
	switch (level) {
	case 13: return "founder";
	case 12: return "owner";
	case 11: return "staff";
	case 10: return "dev";
	case 9: return "admin";
	case 8: return "umod";
	case 7: return "smod";
	case 6: return "guardian";
	case 5: return "mod";
	case 4: return "glory";
	case 3: return "mvp";

	case 2: return "vvip";
	case 1: return "vip";

	default: return "default";
	}
	return "default";
}
enum RoleShop : int32_t {
	DEFAULT,
	VIP,
	VVIP,
	GLORY,
	MODERATOR,
	SUPERMOD
};
int get_role_price(int level) {
	switch (level) {
	case SUPERMOD: return 3500;
	case MODERATOR: return 1500;
	case GLORY: return 500;
	case VVIP: return 200;
	case VIP: return 0;
	default: return 0;
	}
	return 0;
}
int get_role_price(ENetPeer* peer) {
	int level = get_role_level(peer);
	switch (level) {
	case 5: return 3500;
	case 4: return 1500;
	case 3: return 500;
	case 2: return 200;
	case 1: return 0;
	default: return 0;
	}
	return 0;
}

void InsertUIDData(Player* player) {
	ofstream write(format("database/uids/{}.json", player->uid)); {
		json j;
		j["name"] = player->tankIDName;
		write << j;
		write.close();
	}
}

string GetNameByUid(int uid) {
	string name = "deleted_user#" + to_string(uid);
	ifstream get(format("database/uids/{}.json", uid)); {
		if (get.is_open()) {
			json j;
			get >> j;
			name = j["name"].get<string>();
			get.close();
		}
	}
	return name;
}

int GetUidByName(string name) {
	int uid = 0;
	ifstream get(format("database/players/{}_.json", name)); {
		if (get.is_open()) {
			json j;
			get >> j;
			uid = j["userID"].get<int>();
			get.close();
		}
	}
	return uid;
}