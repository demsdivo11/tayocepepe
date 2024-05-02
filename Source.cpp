#define _CRTDBG_MAP_ALLOC_
#include <atomic>
#include <chrono>
#include <cstring>
#include <limits>
#include <memory>
#include <thread>
#include <string>
#include <iostream>
#include <exception>
#include <filesystem>
#include <cstdlib>
#include <crtdbg.h>
#include <signal.h>
#include <fstream>
#include <vector>
#include "enet/enet.h"
#include <nlohmann/json.hpp>
#include "Item.h"
#include "Base.h"
#include "Player.h"
#include "Packet.h"
#include "GameUpdatePacket.h"
#include "Dialog.h"
#include "Algorithm.h"
#include "Guilds.h"
#include "World.h"
#include "SaveSystem.h"
#pragma comment(lib, "Ws2_32.lib")

//mutex as_;
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType) {
	save_all();
	return FALSE;
}
void sendItemDataToPeer(ENetPeer* peer)
{
	ENetPacket* pkt = enet_packet_create(item_data, static_cast<size_t>(item_data_size) + 60, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, pkt);
}
vector<string> split_string_by_newline(const string& str)
{
	auto result = vector<string>{};
	auto ss = stringstream{ str };

	for (string line; getline(ss, line, '\n');)
		result.push_back(line);

	return result;
}
BOOL WINAPI ConsoleHandler(DWORD dwType)
{
	switch (dwType) {
	case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
	{
		down_save();
		return TRUE;
	}
	default:
	{
		break;
	}
	}
	return FALSE;
}
namespace global {
	std::int64_t lastAutoFarm = 0;
}
long long last_auto = 0;
long long lastUpdateData = 0, last_time = 0, last_guild_save = time(NULL) + 60, last_time_ = 0, last_time2_ = 0, last_hm_time = 0, last_growganoth_time = 0, autofarm_delay = 0, autospam_delay = 0, last_checkip = 0;
/*void loop_cheat() {
	long long ms_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	try {
		if (autofarm_delay - ms_time <= 0) {
			autofarm_delay = ms_time + 250;
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()) continue;
				if (not has_playmod(pInfo(currentPeer), "Cheat Active: Auto Farm")) continue;
				bool isRemote = pInfo(currentPeer)->isRemote;
				// Auto Farm
				string name_ = pInfo(currentPeer)->world;
				vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
				if (p != worlds.end()) {
					World* world_ = &worlds[p - worlds.begin()];
					if (pInfo(currentPeer)->Cheat_AF_PlantingTree != 0 and pInfo(currentPeer)->Cheat_AF_isRunning) {
						string name_ = pInfo(currentPeer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							WorldBlock* block_ = &world_->blocks[pInfo(currentPeer)->Cheat_AF_PunchX + (pInfo(currentPeer)->Cheat_AF_PunchY * 100)];
							if (block_->fg == 0 and block_->bg == 0) {
								if (items[pInfo(currentPeer)->Cheat_AF_PlantingTree].blockType == BlockTypes::CLOTHING) {
									pInfo(currentPeer)->Cheat_AF_PlantingTree = 0;
									break;
								}
								int adaBrp = 0;
								modify_inventory(currentPeer, pInfo(currentPeer)->Cheat_AF_PlantingTree, adaBrp);
								if (adaBrp == 0) break;
								if (!isRemote and has_playmod(pInfo(currentPeer), "rayman's fist") and adaBrp < 3) break;
								if (!isRemote and pInfo(currentPeer)->hand == 5196 or pInfo(currentPeer)->hand == 7962 and adaBrp < 5) break;
								if (!isRemote and pInfo(currentPeer)->hand == 9158 and adaBrp < 4) break;
								if (!isRemote and pInfo(currentPeer)->hand == 9158 and adaBrp < 7) break;
								if (!isRemote and pInfo(currentPeer)->hand == 8430 and adaBrp < 10) break;
								if (!isRemote and pInfo(currentPeer)->hand == 8530 and adaBrp < 6) break;
								if (!isRemote and pInfo(currentPeer)->hand == 8532 and adaBrp < 15) break;
								if (!isRemote and pInfo(currentPeer)->hand == 13823 and adaBrp < 20) break;
								if (has_playmod(pInfo(currentPeer), "rayman's fist")) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								if (pInfo(currentPeer)->hand == 5196 or pInfo(currentPeer)->hand == 7962) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 9156) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 9158) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 8532) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 7, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 8, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 9, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 10, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 11, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 10, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 11, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 13828) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 7, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 8, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 9, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 10, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 11, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 12, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 13, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 14, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 15, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 16, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 17, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 18, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 19, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 20, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 21, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 10, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 11, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 8530) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else if (pInfo(currentPeer)->hand == 8430 || pInfo(currentPeer)->hand == 9908) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
								else {
									if (!isRemote and adaBrp < 2) break;
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
									else {
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
										autoPlace_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
									}
								}
							}
							else {
								if (has_playmod(pInfo(currentPeer), "rayman's fist")) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								if (pInfo(currentPeer)->hand == 5196 or pInfo(currentPeer)->hand == 7962) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else if (pInfo(currentPeer)->hand == 9156) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else if (pInfo(currentPeer)->hand == 9158) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else if (pInfo(currentPeer)->hand == 8532) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 9, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 10, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 11, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 10, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 11, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else if (pInfo(currentPeer)->hand == 13828) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 9, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 10, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 11, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 12, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 13, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 14, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 15, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 16, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 17, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 18, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 19, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 20, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 21, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 10, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 11, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 12, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 13, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 14, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 15, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 16, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 17, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 18, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 19, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 20, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 21, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else if (pInfo(currentPeer)->hand == 8530) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										{
											autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY);
											autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY);
											autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY);
											autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY);
										}
									}
								}
								else if (pInfo(currentPeer)->hand == 8430 || pInfo(currentPeer)->hand == 9908) {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 9, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 2, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 3, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 4, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 5, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 6, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 7, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 8, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 9, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
								else {
									if (pInfo(currentPeer)->isFacingLeft) {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX - 1, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
									else {
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
										autoPunch_tile(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + 1, pInfo(currentPeer)->Cheat_AF_PunchY);
									}
								}
							}
						}
					}
				}
			}
		}
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()) continue;
			if (not has_playmod(pInfo(currentPeer), "Cheat Active: Auto Spam")) continue;
			string name_ = pInfo(currentPeer)->world;
			vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
			if (p != worlds.end()) {
				if (has_playmod(pInfo(currentPeer), "Cheat Active: Auto Spam")) {
					if (duration_cast<seconds>(system_clock::now().time_since_epoch()).count() > pInfo(currentPeer)->Cheat_Last_Spam) {
						pInfo(currentPeer)->Cheat_Last_Spam = duration_cast<seconds>(system_clock::now().time_since_epoch()).count() + pInfo(currentPeer)->Cheat_Spam_Delay;
						string msg = pInfo(currentPeer)->Cheat_Spam_Text;
						if (pInfo(currentPeer)->tankIDName == "") break;
						if (msg.length() <= 0 || msg.length() > 120 || msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c) {return std::isspace(c); })) continue;
						for (char c : msg) if (c < 0x20 || c > 0x7A) continue;
						space_(msg);
						if (msg[0] == '/') SendCmd(currentPeer, msg);
						else {
							if (msg[0] == '`' and msg.size() <= 2) break;
							if (pInfo(currentPeer)->world == "") break;
							if (has_playmod(pInfo(currentPeer), "duct tape") || has_playmod(pInfo(currentPeer), "Iron MMMFF")) {
								string msg2 = "";
								for (int i = 0; i < msg.length(); i++) {
									if (isspace(msg[i])) msg2 += " ";
									else {
										if (isupper(msg[i])) msg2 += i % 2 == 0 ? "M" : "F";
										else msg2 += i % 2 == 0 ? "m" : "f";
									}
								}
								msg = msg2;
							}
							string check_ = msg;
							transform(check_.begin(), check_.end(), check_.begin(), ::tolower);
							{
								string name_ = pInfo(currentPeer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (world_->silence and pInfo(currentPeer)->dev != 1 and world_->owner_name != pInfo(currentPeer)->tankIDName) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("`1(Peasants must not speak)"), p.Insert(1), p.CreatePacket(currentPeer);
										break;
									}
								}
							}
							replaceAll(msg, "`%", "");
							string chat_color = "`$";
							if (pInfo(currentPeer)->d_name.empty()) {
								chat_color = pInfo(currentPeer)->dev ? "`^" : pInfo(currentPeer)->tmod ? "`^" : "`$";
							}
							if (has_playmod(pInfo(currentPeer), "Infected!")) {
								chat_color = "`2";
								if (rand() % 4 < 1) chat_color += "Brraaiinnss...";
							}
							if (pInfo(currentPeer)->face == 1170)  chat_color = "`4";

							gamepacket_t p, p2;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:_PL:0_OID:_CT:[W]_ `6<`w" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + (pInfo(currentPeer)->is_legend ? " of Legend" : "") + "`6> " + chat_color + msg);
							p2.Insert("OnTalkBubble");
							p2.Insert(pInfo(currentPeer)->netID);
							if (check_ != ":/" and check_ != ":p" and check_ != ":*" and check_ != ";)" and check_ != ":d" and check_ != ":o" and check_ != ":'(" and check_ != ":(") {
								p2.Insert("CP:_PL:0_OID:_player_chat=" + (chat_color == "`$" ? "" : chat_color) + msg);
							}
							else p2.Insert(msg);
							for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
								if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL) continue;
								if (pInfo(currentPeer2)->world == pInfo(currentPeer)->world) {
									bool muted_ = false;
									for (int c_ = 0; c_ < pInfo(currentPeer2)->friends.size(); c_++) {
										if (pInfo(currentPeer2)->friends[c_].name == pInfo(currentPeer)->tankIDName) {
											if (pInfo(currentPeer2)->friends[c_].mute) {
												muted_ = true;
												break;
											}
										}
									}
									if (not muted_) {
										p.CreatePacket(currentPeer2);
										p2.CreatePacket(currentPeer2);
									}
								}
							}
							for (int i = 0; i < illegal_words.size(); i++) {
								if (to_lower(msg).find(illegal_words[i]) != string::npos and pInfo(currentPeer)->last_spam_detection + 1000 < duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) {
									add_pFlag(currentPeer, 2);
									pInfo(currentPeer)->lastFlagged = Algorithm::get_epoch_ms() + 5000, pInfo(currentPeer)->totalFlag++;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	catch (out_of_range& sui) {
		tayo_warn(sui.what());
	}
}*/
void loop_cheat() {
	long long ms_time = Algorithm::get_epoch_ms();
	try {
		if (last_auto - ms_time <= 0) {
			last_auto = ms_time + 200;
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or
					pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()
					or pInfo(currentPeer)->Cheat_AF_PlantingTree == 0
					//or pInfo(currentPeer)->Cheat_AF_PunchX == -1 or pInfo(currentPeer)->Cheat_AF_PunchY == -1
					) continue;
				if (not pInfo(currentPeer)->Cheat_AF and pInfo(currentPeer)->Cheat_AF_isRunning) continue;
				//cout << "check 1 passed\n"; - called
				if (pInfo(currentPeer)->Cheat_AF_PlantingTree != 0 && pInfo(currentPeer)->Cheat_AF_isRunning)
				{
					World* world = GetCurrentWorld(pInfo(currentPeer)->world);
					WorldBlock* block = GetTile(pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, world);
					if (block->fg == 0 && block->bg == 0) {
						if (items[pInfo(currentPeer)->Cheat_AF_PlantingTree].blockType == CLOTHING) {
							pInfo(currentPeer)->Cheat_AF_PlantingTree = 0;
							break;
						}
						//cout << "check 2 passed\n"; -- called
						int item_count = get_item_count(pInfo(currentPeer), pInfo(currentPeer)->Cheat_AF_PlantingTree);
						if (item_count == 0) break;
						int place_ = items[pInfo(currentPeer)->hand].place_far;
						bool isRemote = (pInfo(currentPeer)->Cheat_AF_PlantingTree == 5640);
						auto magplant = GetMagplant(currentPeer);
						if (isRemote && magplant == nullptr) break;
						if (place_ > 1) {
							if (!isRemote and item_count < place_)
								place_ = item_count;
							if (isRemote and magplant->pr > 0 and place_ > magplant->pr)
								place_ = magplant->pr;
							for (int i = 0; i < place_; i++)
								AutoPlace(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + (pInfo(currentPeer)->isFacingLeft ? (-i) : i), pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
						}
						else if (pInfo(currentPeer)->hand == 5172 || pInfo(currentPeer)->hand == 5174) {
							if (!isRemote and item_count < 1) break;
							if (isRemote and magplant->pr < 1) break;
							AutoPlace(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
						}
						else if (items[pInfo(currentPeer)->Cheat_AF_PlantingTree].blockType == CONSUMABLE) {
							if (!isRemote and item_count < 1) break;
							if (isRemote and magplant->pr < 1) break;
							AutoPlace(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
						}
						else {
							if (!isRemote and item_count < 2) break;
							if (isRemote and magplant->pr < 2) break;
							AutoPlace(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
							AutoPlace(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + (pInfo(currentPeer)->isFacingLeft ? -1 : 1), pInfo(currentPeer)->Cheat_AF_PunchY, pInfo(currentPeer)->Cheat_AF_PlantingTree);
						}
					}
					else {
						//cout << "check 3 passed\n"; -- not called
						int punch_ = items[pInfo(currentPeer)->hand].punch_far;
						if (punch_ > 1) {
							for (int i = 0; i < punch_; i++)
								AutoPunch(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + (pInfo(currentPeer)->isFacingLeft ? -i : i), pInfo(currentPeer)->Cheat_AF_PunchY);
						}
						else if (pInfo(currentPeer)->hand == 5172 || pInfo(currentPeer)->hand == 5174) {
							AutoPunch(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
						}
						else {
							AutoPunch(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX, pInfo(currentPeer)->Cheat_AF_PunchY);
							AutoPunch(currentPeer, pInfo(currentPeer)->Cheat_AF_PunchX + (pInfo(currentPeer)->isFacingLeft ? -1 : 1), pInfo(currentPeer)->Cheat_AF_PunchY);
						}
					}
				}
			}
		}
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()) continue;
			if (not has_playmod(pInfo(currentPeer), "Cheat Active: Auto Spam")) continue;
			string name_ = pInfo(currentPeer)->world;
			vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
			if (p != worlds.end()) {
				if (has_playmod(pInfo(currentPeer), "Cheat Active: Auto Spam")) {
					if (duration_cast<seconds>(system_clock::now().time_since_epoch()).count() > pInfo(currentPeer)->Cheat_Last_Spam) {
						pInfo(currentPeer)->Cheat_Last_Spam = duration_cast<seconds>(system_clock::now().time_since_epoch()).count() + pInfo(currentPeer)->Cheat_Spam_Delay;
						string msg = pInfo(currentPeer)->Cheat_Spam_Text;
						if (pInfo(currentPeer)->tankIDName == "") break;
						if (msg.length() <= 0 || msg.length() > 120 || msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c) {return std::isspace(c); })) continue;
						for (char c : msg) if (c < 0x20 || c > 0x7A) continue;
						space_(msg);
						if (msg[0] == '/') SendCmd(currentPeer, msg);
						else {
							if (msg[0] == '`' and msg.size() <= 2) break;
							if (pInfo(currentPeer)->world == "") break;
							if (has_playmod(pInfo(currentPeer), "duct tape") || has_playmod(pInfo(currentPeer), "Iron MMMFF")) {
								string msg2 = "";
								for (int i = 0; i < msg.length(); i++) {
									if (isspace(msg[i])) msg2 += " ";
									else {
										if (isupper(msg[i])) msg2 += i % 2 == 0 ? "M" : "F";
										else msg2 += i % 2 == 0 ? "m" : "f";
									}
								}
								msg = msg2;
							}
							string check_ = msg;
							transform(check_.begin(), check_.end(), check_.begin(), ::tolower);
							{
								string name_ = pInfo(currentPeer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (world_->silence and pInfo(currentPeer)->dev != 1 and world_->owner_name != pInfo(currentPeer)->tankIDName) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("`1(Peasants must not speak)"), p.Insert(1), p.CreatePacket(currentPeer);
										break;
									}
								}
							}
							replaceAll(msg, "`%", "");
							string chat_color = "`$";
							if (pInfo(currentPeer)->d_name.empty()) {
								chat_color = pInfo(currentPeer)->dev ? "`^" : pInfo(currentPeer)->tmod ? "`^" : "`$";
							}
							if (has_playmod(pInfo(currentPeer), "Infected!")) {
								chat_color = "`2";
								if (rand() % 4 < 1) chat_color += "Brraaiinnss...";
							}
							if (pInfo(currentPeer)->face == 1170)  chat_color = "`4";

							gamepacket_t p, p2;
							p.Insert("OnConsoleMessage");
							p.Insert("CP:_PL:0_OID:_CT:[W]_ `6<`w" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + (pInfo(currentPeer)->is_datemaster ? " `4Datemaster" : "") + (pInfo(currentPeer)->is_legend ? " of Legend" : "") + "`6> " + chat_color + msg);
							p2.Insert("OnTalkBubble");
							p2.Insert(pInfo(currentPeer)->netID);
							if (check_ != ":/" and check_ != ":p" and check_ != ":*" and check_ != ";)" and check_ != ":d" and check_ != ":o" and check_ != ":'(" and check_ != ":(") {
								p2.Insert("CP:_PL:0_OID:_player_chat=" + (chat_color == "`$" ? "" : chat_color) + msg);
							}
							else p2.Insert(msg);
							for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
								if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL) continue;
								if (pInfo(currentPeer2)->world == pInfo(currentPeer)->world) {
									bool muted_ = false;
									for (int c_ = 0; c_ < pInfo(currentPeer2)->friends.size(); c_++) {
										if (pInfo(currentPeer2)->friends[c_].name == pInfo(currentPeer)->tankIDName) {
											if (pInfo(currentPeer2)->friends[c_].mute) {
												muted_ = true;
												break;
											}
										}
									}
									if (not muted_) {
										p.CreatePacket(currentPeer2);
										p2.CreatePacket(currentPeer2);
									}
								}
							}
							for (int i = 0; i < illegal_words.size(); i++) {
								if (to_lower(msg).find(illegal_words[i]) != string::npos and pInfo(currentPeer)->last_spam_detection + 1000 < duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) {
									add_pFlag(currentPeer, 2);
									pInfo(currentPeer)->lastFlagged = Algorithm::get_epoch_ms() + 5000, pInfo(currentPeer)->totalFlag++;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	catch (out_of_range& bad_warn) {
		tayo_warn(bad_warn.what());
		return;
	}
}
inline thread cheat_thread() {
	//lock_guard g_(autofarm); -- same with calling the lock and unlock function
	//autofarm.lock();
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		loop_cheat();
	}
	return thread();
	//autofarm.unlock();
}
long long last_carnival_system = 0, last_carnival_entity = 0, last_tournament_system = 0;
void loop_worlds() {
	long long ms_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	if (lastUpdateData - ms_time <= 0) {
		serverTimeOn = serverTimeOn + 10;
		ofstream write_new_timeon("uptime.txt");
		write_new_timeon << (uptimeText(serverTimeOn));
		write_new_timeon.close();
		lastUpdateData = ms_time + 10000;
	}
	if (updates::is_cooldown("valentine") == false && global::events::valentine) {
		global::events::valentineUpdate = date_time::get_epoch_time() + 3600;

		pData::Algorithm::loop_players([&](ENetPeer* currentPeer) {
			if (pInfo(currentPeer)->valentine.chocolates >= 1) {
				auto it = std::find_if(global::events::leaderboards.begin(), global::events::leaderboards.end(), [&](const auto& leaderboard) {
					return leaderboard.first == pInfo(currentPeer)->tankIDName;
				});

				if (it != global::events::leaderboards.end()) {
					it->second = pInfo(currentPeer)->valentine.chocolates;
				} else global::events::leaderboards.emplace_back(pInfo(currentPeer)->tankIDName, pInfo(currentPeer)->valentine.chocolates);

				std::sort(global::events::leaderboards.begin(), global::events::leaderboards.end(), [](const auto& a, const auto& b) {
					return a.second > b.second;
				});

				if (global::events::leaderboards.size() > 10) {
					global::events::leaderboards.resize(10);
					global::events::leaderboards.erase(global::events::leaderboards.begin() + 10, global::events::leaderboards.end());
				}
			}
		});
	}
	if (last_hm_time - ms_time <= 0) {
		last_hm_time = ms_time + 60000;
		for (int i = 0; i < monitors.size(); i++) {
			string name_ = monitors[i].world_name;
			vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
			if (p != worlds.end()) {
				World* world_ = &worlds[p - worlds.begin()];
				WorldBlock* monitor = &world_->blocks[monitors[i].x + (monitors[i].y * world_->width)];
				if (!items[monitor->fg].heart_monitor) {
					monitors.erase(monitors.begin() + i);
					i--;
					continue;
				}
				monitors[i].active = 0;
				string find_mon = monitor->heart_monitor;
				if (find_mon.size() >= 2) find_mon.resize(find_mon.size() - 2); // remove `` is galo
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
					if (pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName == find_mon) {
						monitors[i].active = 1;
						break;
					}
				}
				PlayerMoving data_{};
				data_.packetType = 5, data_.punchX = monitors[i].x, data_.punchY = monitors[i].y, data_.characterState = 0x8;
				BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, monitor));
				BYTE* blc = raw + 56;
				form_visual(blc, *monitor, *world_, NULL, false, false, monitors[i].x, monitors[i].y);
				for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
					if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
					if (pInfo(currentPeer)->world == world_->name) {
						send_raw(currentPeer, 4, raw, 112 + alloc_(world_, monitor), ENET_PACKET_FLAG_RELIABLE);
					}
				}
				delete[] raw, blc;
			}
		}
	}
	if (global::algorithm::hideseek.Timers <= Algorithm::get_epoch_time<std::chrono::seconds>() && global::algorithm::hideseek.GameStarted == true) {
		pData::Algorithm::loop_players([&](ENetPeer* currentPeer) {
			if (currentPeer != nullptr && !pInfo(currentPeer)->tankIDName.empty()) {

				std::string message = global::algorithm::hideseek.HidersWon ? "The `2Hiders `ojust won the games." : "The `4Seekers `ojust won the games.";

				enet_peer_send(currentPeer, 0, Variant{ "OnConsoleMessage" }.push("`1HIDE-SEEK: `oThe `5Hide and Seek Games `ohas been ended. Use `5/joingame `oto join the games! " + message).pack());

				if (pInfo(currentPeer)->hideseek.InGame) {
					player::algorithm::OnCountdownEnd(currentPeer);

					pInfo(currentPeer)->hideseek.InGame = false;
					pInfo(currentPeer)->hideseek.Hiders = false;
					pInfo(currentPeer)->hideseek.Seekers = false;

					exit_(currentPeer);
				}
			}
			});

		global::algorithm::hideseek.Timers = 0;
		global::algorithm::hideseek.GameStarted = false;

		global::algorithm::hideseek.Players = 0;
		global::algorithm::hideseek.Seekers = 0;
		global::algorithm::hideseek.Hiders = 0;

		global::algorithm::hideseek.SeekersWon = false;
		global::algorithm::hideseek.HidersWon = false;
	}
	if (global::algorithm::hideseek.RestTimers <= Algorithm::get_epoch_time<std::chrono::seconds>() && global::algorithm::hideseek.Seekers >= 1) {
		if (!global::algorithm::hideseek.GameStarted) {
			pData::Algorithm::loop_players([&](ENetPeer* currentPeer) {
				if (currentPeer != nullptr && !pInfo(currentPeer)->tankIDName.empty()) {
					if (pInfo(currentPeer)->hideseek.InGame) {
						player::algorithm::OnCountdownEnd(currentPeer);
					}
				}
				});
			player::algorithm::StartGameSeek();
			global::algorithm::hideseek.LobbyTimers = 0;
			global::algorithm::hideseek.Timers = Algorithm::get_epoch_time<std::chrono::seconds>() + 300;
			global::algorithm::hideseek.GameStarted = true;
		}
	}
	if (last_time2_ - ms_time <= 0 && restart_server_status) {
		gamepacket_t p;
		p.Insert("OnConsoleMessage"), p.Insert("`4Global System Message``: Restarting server for update in `4" + to_string(restart_server_time) + "`` minutes");
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
			if (restart_server_time == 1) packet_(currentPeer, "action|play_sfx\nfile|audio/ogg/suspended.ogg\ndelayMS|700");
			p.CreatePacket(currentPeer);
		}
		restart_server_time -= 1;
		if (restart_server_time == 0) {
			last_time2_ = ms_time + 10000, restart_server_status_seconds = true, restart_server_status = false;
			restart_server_time = 50;
		}
		else last_time2_ = ms_time + 60000;
	}
	if (restart_server_status_seconds && last_time2_ - ms_time <= 0) {
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
			gamepacket_t p;
			p.Insert("OnConsoleMessage"), p.Insert("`4Global System Message``: Restarting server for update in `4" + (restart_server_time > 0 ? to_string(restart_server_time) : "ZERO") + "`` seconds" + (restart_server_time > 0 ? "" : "! Should be back up in a minute or so. BYE!") + "");
			p.CreatePacket(currentPeer);
		}
		last_time2_ = ms_time + 10000;
		if (restart_server_time == 0) {
			restart_server_status_seconds = false;
			for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
				if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
				if (not pInfo(currentPeer)->tankIDName.empty()) {
					save_player(pInfo(currentPeer), false);
				}
				if (pInfo(currentPeer)->trading_with != -1) {
					cancel_trade(currentPeer, false);
				}
			}
			write_maintenance(true);
			down_save();
		}
		restart_server_time -= 10;
	}
	if (last_time - ms_time <= 0) {
		last_time = ms_time + 1000;
		epoch_cycle(); // fix this if something went wrong epoch cycle crash
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world.empty() or pInfo(currentPeer)->tankIDName.empty()) continue;
			if (pInfo(currentPeer)->last_fish_catch + pInfo(currentPeer)->fish_seconds < ms_time && pInfo(currentPeer)->fishing_used != 0 && rand() % 100 < (pInfo(currentPeer)->hand == 3010 ? 11 : 8)) {
				PlayerMoving data_{};
				data_.packetType = 17, data_.netID = 34, data_.YSpeed = 34, data_.x = pInfo(currentPeer)->f_x * 32 + 16, data_.y = pInfo(currentPeer)->f_y * 32 + 16;
				pInfo(currentPeer)->last_fish_catch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				BYTE* raw = packPlayerMoving(&data_);
				gamepacket_t p3(0, pInfo(currentPeer)->netID);
				p3.Insert("OnPlayPositioned"), p3.Insert("audio/splash.wav");
				for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
					if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != pInfo(currentPeer)->world) continue;
					send_raw(currentPeer_event, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE), p3.CreatePacket(currentPeer_event);
				}
				delete[] raw;
			}
			if (pInfo(currentPeer)->save_time + 600000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
				pInfo(currentPeer)->save_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
			}

			string name_ = pInfo(currentPeer)->world;
			vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
			if (p != worlds.end()) {
				World* world_ = &worlds[p - worlds.begin()];
				
				if (world_->name == "CARNIVAL" and carnival_open)
				{
					if (last_carnival_system - ms_time <= 0) { //carnival
						last_carnival_system = ms_time + 2000;
						for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
							if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->tankIDName.empty()) continue;
							wipe_carnival(currentPeer), carnival_end(currentPeer);
						}
					}
					if (last_carnival_entity - ms_time <= 0) {
						last_carnival_entity = ms_time + 1000;
						if (carnival[1].player_ != "" or carnival[2].player_ != "") {
							if (carnival[1].player_ != "") {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->tankIDName.empty()) continue;
									if (pInfo(currentPeer)->tankIDName == carnival[1].player_) {
										if (carnival[1].punch_c != 0) return;
										string name = pInfo(currentPeer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
										if (p != worlds.end()) {
											World* world = &worlds[p - worlds.begin()];
											vector<vector<int>> wht = { {67, 52}, {69, 52}, {71, 52}, {73, 52}, {68, 50}, {70, 50}, {72, 50}, {69, 48}, {71, 48}, {70, 46} };
											int loop = 3;
											for (int i = 0; i < loop; i++) {
												int s_ = rand() % wht.size();
												if (world->blocks[wht[s_][0] + (wht[s_][1] * world_->width)].flags & 0x00400000) loop++;
												bullseye_update(currentPeer, wht[s_][0], wht[s_][1]);
											}
											carnival[1].punch_c = 3;
										}
									}
								}
							}
							if (carnival[2].player_ != "") {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->tankIDName.empty()) continue;
									if (pInfo(currentPeer)->tankIDName == carnival[2].player_) {
										if (carnival[2].punch_c != 0) return;
										string name = pInfo(currentPeer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
										if (p != worlds.end()) {
											World* world = &worlds[p - worlds.begin()];
											vector<vector<int>> wht = { {67, 52}, {69, 52}, {71, 52}, {73, 52}, {68, 50}, {70, 50}, {72, 50}, {69, 48}, {71, 48}, {70, 46} };
											int loop = 3;
											for (int i = 0; i < loop; i++) {
												int s_ = rand() % wht.size();
												if (world->blocks[(wht[s_][0] + 36) + (wht[s_][1] * world_->width)].flags & 0x00400000) loop++;
												bullseye_update(currentPeer, (wht[s_][0] + 36), wht[s_][1]);
											}
											carnival[2].punch_c = 3;
										}
									}
								}
							}
						}
					}
				}
				if (last_growganoth_time - ms_time <= 0 && world_->name == "GROWGANOTH" && Halloween) {
					last_growganoth_time = ms_time + 15000;
					for (int i_ = 0; i_ < growganoth_platform.size(); i_++) update_tile(currentPeer, growganoth_platform[i_] % world_->width, growganoth_platform[i_] / world_->width, 0, false, false);
					growganoth_platform.clear();
					for (int i_ = 0; i_ < 50; i_++) update_tile(currentPeer, rand() % 60 + 20, rand() % 32 + 20, (i_ < 40 ? 1222 : 7048), true, false);
				}
				if (global::algorithm::hideseek.Players >= 1 && global::algorithm::hideseek.GameStarted == false) {
					if (global::algorithm::hideseek.LobbyTimers + 30LL < (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())).count()) {
						if (pInfo(currentPeer)->hideseek.InGame && !global::algorithm::hideseek.Hiders >= 1 && !global::algorithm::hideseek.Seekers >= 1) {
							if (global::algorithm::hideseek.Players < 3) {
								ENetPacket* packets = Variant{ "OnConsoleMessage" }.push("`1HIDE-SEEK: `oThe game can't be start. Must be atleast `w5 players `oin the games.").pack();

								enet_peer_send(currentPeer, 0, packets);

								global::algorithm::hideseek.LobbyTimers = (duration_cast<seconds>(system_clock::now().time_since_epoch())).count();
							}
							else {
								player::algorithm::StartHideSeek();

								global::algorithm::hideseek.LobbyTimers = 0;
							}
						}
					}
				}
				/*
					if (world_->special_event && world_->last_special_event + 30000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						gamepacket_t p, p2;
						p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world_->special_event_item].event_name + ":`` " + (items[world_->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world_->special_event_item_taken) + " of " + to_string(items[world_->special_event_item].event_total) + " items found.``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
						p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` " + (items[world_->special_event_item].event_total == 1 ? "`oTime's up! Nobody found it!``" : "`oTime's up! " + to_string(world_->special_event_item_taken) + " of " + to_string(items[world_->special_event_item].event_total) + " items found.``") + "");
						for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
							if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != name_) continue;
							p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
							PlayerMoving data_{};
							data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = 0;
							for (int i_ = 0; i_ < world_->drop.size(); i_++) {
								if (world_->drop[i_].special == true) {
									data_.plantingTree = world_->drop[i_].uid;
									BYTE* raw = packPlayerMoving(&data_);
									send_raw(currentPeer_event, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
									world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
								}
							}
						}
						world_->last_special_event = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(), world_->special_event_item = 0, world_->special_event_item_taken = 0, world_->special_event = false;
					}
				}
			if (pInfo(currentPeer)->hand == 3578 && pInfo(currentPeer)->hand_torch + 60000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
				if (pInfo(currentPeer)->hand_torch != 0) {
					int got = 0;
					modify_inventory(currentPeer, 3578, got);
					if (got - 1 >= 1) {
						gamepacket_t p;
						p.Insert("OnTalkBubble"), p.Insert(pInfo(currentPeer)->netID), p.Insert("`4My torch went out, but I have " + to_string(got - 1) + " more!``"), p.CreatePacket(currentPeer);
					}
					modify_inventory(currentPeer, 3578, got = -1);
				}
				pInfo(currentPeer)->hand_torch = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
			}
			/*
			if (pInfo(currentPeer)->rb == 1 && pInfo(currentPeer)->name_time + 250 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
				pInfo(currentPeer)->name_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
				string msg2 = pInfo(currentPeer)->name_color;
				if (pInfo(currentPeer)->is_legend) msg2 = "`" + to_string(rand() % 9 + 1) + pInfo(currentPeer)->tankIDName;
				else for (int i = 0; i < pInfo(currentPeer)->tankIDName.length(); i++) msg2 += "`" + to_string(rand() % 9 + 1) + "" + pInfo(currentPeer)->tankIDName[i];
				ChangeName(currentPeer, msg2);
			}*/
				/*
				if (not pInfo(currentPeer)->world.empty() and pInfo(currentPeer)->Cheat_AF and pInfo(currentPeer)->Cheat_AF_isRunning and pInfo(currentPeer)->Cheat_AF_PlantingTree != 0) {
					AutoFarm(currentPeer); 
				}
				*/
				if (pInfo(currentPeer)->hand == 2204 and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1) {
					if (not has_playmod(pInfo(currentPeer), "Irradiated")) {
						if (pInfo(currentPeer)->geiger_x == -1 and pInfo(currentPeer)->geiger_y == -1) pInfo(currentPeer)->geiger_x = (rand() % 100) * 32, pInfo(currentPeer)->geiger_y = (rand() % 54) * 32;
						int a_ = pInfo(currentPeer)->geiger_x + ((pInfo(currentPeer)->geiger_y * world_->width) / 32), b_ = pInfo(currentPeer)->x + ((pInfo(currentPeer)->y * world_->width) / 32), diff = abs(a_ - b_) / 32;
						if (diff > 30) { // nieko

						}
						else if (diff >= 30) { // raudona
							if (pInfo(currentPeer)->geiger_time + 1500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								PlayerMoving data_{};
								data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8, data_.XSpeed = 0, data_.YSpeed = 114;
								BYTE* raw = packPlayerMoving(&data_, 56);
								raw[3] = 114;
								double rotation = -4.13;
								memcpy(raw + 40, &rotation, 4);
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
							}
						}
						else if (diff >= 15) { // geltona
							if (pInfo(currentPeer)->geiger_time + 1500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								PlayerMoving data_{};
								data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8, data_.XSpeed = 1, data_.YSpeed = 114;
								BYTE* raw = packPlayerMoving(&data_, 56);
								raw[3] = 114;
								double rotation = -4.13;
								memcpy(raw + 40, &rotation, 4);
								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
								delete[] raw;
							}
						}
						else { // zalia
							if (diff <= 1) { // surado
								{
									if (pInfo(currentPeer)->geiger_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										pInfo(currentPeer)->geiger_x = -1, pInfo(currentPeer)->geiger_y = -1;
										{
											int c_ = -1;
											modify_inventory(currentPeer, 2204, c_);
											int c_2 = 1;
											if (modify_inventory(currentPeer, 2286, c_2) != 0) {
												string name_ = pInfo(currentPeer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													WorldDrop drop_block_{};
													drop_block_.id = 2286, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
													dropas_(world_, drop_block_);
												}
											}
											PlayMods give_playmod{};
											give_playmod.id = 10;
											give_playmod.time = time(nullptr) + (thedaytoday == 3 ? 60 : 120);
											pInfo(currentPeer)->playmods.push_back(give_playmod);
											pInfo(currentPeer)->hand = 2286;
											update_clothes(currentPeer);
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert(a + "You are aglow with radiation! (`$Irradiated`` mod added, `$" + (thedaytoday == 3 ? "1" : "2") + " mins`` left)");
											p.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
										}
										int chanced = 0;
										if (thedaytoday == 3) chanced = 3;
										add_role_xp(currentPeer, 1, "geiger");
										vector<int> geiger_items = { 1796,6416,3196,2206,1500,1498,2806,2804,8270,8272,8274,2244,2246,2242,3792,3306,4676,4678,4680,4682,4652,4650,4648,4646,11186,10086,242 };
										vector<int> rare_cr = { 1796,1796,2248,2250,3792,10084,242,1796 };
										vector<int> rarest = { 7188,4654 , 9380 , 11562, 1486,242,1796,1796 };
										vector<int> extra_craft = { 9166, 9490, 7188 };
										vector<int> geiger_reward = { 242, 1796, 1796 };
										int item_ = geiger_items[rand() % geiger_items.size()];
										if (rand() % 50 < 15 + chanced) item_ = extra_craft[rand() % extra_craft.size()];
										if (rand() % 50 < 10 + chanced) item_ = rare_cr[rand() % rare_cr.size()];
										if (rand() % 50 < 2 + chanced) item_ = rarest[rand() % rarest.size()];
										if (rand() % 50 < 1 + chanced) item_ = geiger_reward[rand() % geiger_reward.size()];
										int c_ = 1;
										if (modify_inventory(currentPeer, item_, c_) != 0) {
											string name_ = pInfo(currentPeer)->world;
											vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
											if (p != worlds.end()) {
												World* world_ = &worlds[p - worlds.begin()];
												WorldDrop drop_block_{};
												drop_block_.id = item_, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(currentPeer)->x + rand() % 17, drop_block_.y = pInfo(currentPeer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										{
											if (rand() % 8 < 4 && global::events::valentine) {
												int randomGets = rand() % 100 + 50;

												Algorithm::send_console(currentPeer, std::format("`o>> You've received a `w{} Valentine's Chocolate `ofrom fishing.", randomGets));

												pInfo(currentPeer)->valentine.chocolates += randomGets;
											}
										}
										gamepacket_t p;
										p.Insert("OnParticleEffect");
										p.Insert(48);
										p.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
										p.CreatePacket(currentPeer);
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(currentPeer)->netID);
											p.Insert("I found `21 " + items[item_].name + "``!");
											p.Insert(0);
											p.CreatePacket(currentPeer);
											gamepacket_t p2;
											p2.Insert("OnConsoleMessage");
											p2.Insert(pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "`` found `21 " + items[item_].name + "``!");
											PlayerMoving data_{};
											data_.packetType = 19, data_.plantingTree = 0, data_.netID = 0;
											data_.punchX = item_;
											data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16;
											int32_t to_netid = pInfo(currentPeer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
												if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL) continue;
												if (pInfo(currentPeer2)->world == pInfo(currentPeer)->world) {
													send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													p2.CreatePacket(currentPeer2);
												}
											}
											delete[]raw;
										}
									}
								}
							}
							else {
								int t_ = 0;
								if (diff >= 6) t_ = 1350;
								else t_ = 1000;
								if (pInfo(currentPeer)->geiger_time + t_ < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
									pInfo(currentPeer)->geiger_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
									PlayerMoving data_{};
									data_.packetType = 17, data_.x = pInfo(currentPeer)->x + 10, data_.y = pInfo(currentPeer)->y + 16, data_.characterState = 0x8;
									data_.XSpeed = 2, data_.YSpeed = 114;
									BYTE* raw = packPlayerMoving(&data_, 56);
									raw[3] = 114;
									double rotation = -4.13;
									memcpy(raw + 40, &rotation, 4);
									send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
								}
							}
						}
					}
				}
				long long time_ = time(nullptr);
				for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) {
					if (pInfo(currentPeer)->playmods[i_].id == 12) {
						if (pInfo(currentPeer)->valentine_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(currentPeer)->valentine_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							for (ENetPeer* valentine = server->peers; valentine < &server->peers[server->peerCount]; ++valentine) {
								if (valentine->state != ENET_PEER_STATE_CONNECTED or valentine->data == NULL) continue;
								if (pInfo(valentine)->world == pInfo(currentPeer)->world and pInfo(valentine)->tankIDName == pInfo(currentPeer)->playmods[i_].user) {
									if (not pInfo(valentine)->invis and not pInfo(currentPeer)->invis and pInfo(currentPeer)->x != -1 and pInfo(currentPeer)->y != -1 and pInfo(valentine)->x != -1 and pInfo(valentine)->y != -1) {
										gamepacket_t p;
										p.Insert("OnParticleEffect");
										p.Insert(13);
										p.Insert((float)pInfo(valentine)->x + 10, (float)pInfo(valentine)->y + 16);
										p.Insert((float)0), p.Insert((float)pInfo(currentPeer)->netID);
										bool double_send = false;
										for (int i_2 = 0; i_2 < pInfo(valentine)->playmods.size(); i_2++) {
											if (pInfo(valentine)->playmods[i_2].id == 12 and pInfo(valentine)->playmods[i_2].user == pInfo(currentPeer)->tankIDName) {
												double_send = true;
												break;
											}
										}
										gamepacket_t p2;
										p2.Insert("OnParticleEffect");
										p2.Insert(13);
										p2.Insert((float)pInfo(currentPeer)->x + 10, (float)pInfo(currentPeer)->y + 16);
										p2.Insert((float)0), p2.Insert((float)pInfo(valentine)->netID);
										for (ENetPeer* valentine_bc = server->peers; valentine_bc < &server->peers[server->peerCount]; ++valentine_bc) {
											if (valentine_bc->state != ENET_PEER_STATE_CONNECTED or valentine_bc->data == NULL) continue;
											if (pInfo(valentine_bc)->world == pInfo(currentPeer)->world) {
												p.CreatePacket(valentine_bc);
												if (double_send) p2.CreatePacket(valentine_bc);
											}
										}
									}
									break;
								}
							}
						}
					}
					if (pInfo(currentPeer)->playmods[i_].time - time_ < 0) {
						// issue the playmods turns to 0
						uint32_t playmod_id = pInfo(currentPeer)->playmods[i_].id;
						//cout << playmod_id << endl; // the value is not expected, huh?
						Playmod playmod = info_about_playmods[playmod_id];
						string playmod_name = playmod.name;
						string playmod_on_remove = playmod.onRemoved;
						//cout << "loop_world playmod data: \n";
						//print_info_pmod(playmod); 
						if (pInfo(currentPeer)->playmods[i_].id == 126 and pInfo(currentPeer)->playmods[i_].time - time_ < 0)
							end_player_tournament(currentPeer);
						pInfo(currentPeer)->playmods.erase(pInfo(currentPeer)->playmods.begin() + i_);
						packet_(currentPeer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
						gamepacket_t p;
						p.Insert("OnConsoleMessage");
						p.Insert(playmod_on_remove + " (`$" + playmod_name + "`` mod removed)");
						p.CreatePacket(currentPeer);
						update_clothes(currentPeer);
					}
				}
			}
		}
	}
}
bool isASCII(const string& s)
{
	return !any_of(s.begin(), s.end(), [](char c) {
		return static_cast<unsigned char>(c) > 127;
		});
}
inline void SaveSystem() {
	//lock_guard<mutex> guard(as_); 
	while (true) {
		this_thread::sleep_for(minutes(1));
		Server_Saving(), Server_Reset();
	}
}

int main(int argc, char* argv[]) {
	// init local-time
	//int* ptr = reinterpret_cast<int*>(0x000000FDCDEC4740);
	//cout << &ptr; 
	try {
		struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	today_day = newtime.tm_mday;
	algorithm::log_text("SERVER_PACKET.txt", "\n[Server started]\n");
	//
	system("COLOR B");
	system("TITLE TayoPs");
	tayo_info("Setting up server...");
	tayo_info("Program By iDems Aka Demsm.kom");

	//
	if (atexit(save_all)) {
		gnettoken_db_logs("Saving all data...");
	}
	srand(unsigned int(time(nullptr)));
	signal(SIGINT, save_w), signal(SIGABRT, save_w);
	SetConsoleCtrlHandler(HandlerRoutine, true);
	gnettoken_db_logs("Serializing guild & server data...");
	for (const auto& entry : fs::directory_iterator("database/guilds")) {
		if (!fs::is_directory(entry.path()) && !fs::is_empty(entry.path())) {
			string guild_id = explode(".", entry.path().filename().string())[0];
			json guild_read;
			ifstream read_guild(entry.path(), ifstream::binary);
			read_guild >> guild_read;
			read_guild.close();
			Guild new_guild{};
			new_guild.guild_id = atoi(guild_id.c_str());
			new_guild.guild_name = guild_read["guild_name"].get<string>();
			new_guild.guild_description = guild_read["guild_description"].get<string>();
			new_guild.guild_mascot = guild_read["guild_mascot"].get<vector<uint16_t>>();
			new_guild.guild_level = guild_read["guild_level"].get<uint16_t>();
			new_guild.guild_xp = guild_read["guild_xp"].get<uint32_t>();
			new_guild.coleader_access = guild_read["coleader_access"].get<bool>();
			new_guild.coleader_elder_access = guild_read["coleader_elder_access"].get<bool>();
			new_guild.all_access = guild_read["all_access"].get<bool>();
			new_guild.guild_world = guild_read["guild_world"].get<string>();
			json a_ = guild_read["guild_members"].get<json>();
			for (int i_ = 0; i_ < a_.size(); i_++) {
				GuildMember new_member{};
				new_member.member_name = a_[i_]["member_name"].get<string>();
				new_member.role_id = a_[i_]["role_id"].get<int>();
				new_member.public_location = a_[i_]["public_location"].get<bool>();
				new_member.show_notifications = a_[i_]["show_notifications"].get<bool>();
				new_member.last_online = a_[i_]["last_online"].get<long long>();
				new_guild.guild_members.push_back(new_member);
			}
			json b_ = guild_read["guild_logs"].get<json>();
			for (int i_ = 0; i_ < b_.size(); i_++) {
				GuildLog new_log{};
				new_log.info = b_[i_]["info"].get<string>();
				new_log.display_id = b_[i_]["display_id"].get<uint16_t>();
				new_log.date = b_[i_]["date"].get<long long>();
				new_guild.guild_logs.push_back(new_log);
			}
			guilds.push_back(new_guild);
		}
	}
	{
		ifstream ifs("database/promoters.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			promoter = j["promoter"].get<vector<int>>();
			ifs.close();
			tayo_info(format("Added {} promoters to the list.", setGems(promoter.size())));
		}
	}
	{
		ifstream ifs("database/resellers.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			reseller = j["reseller"].get<vector<int>>();
			ifs.close();
			tayo_info(format("Added {} resellers to the list.", setGems(reseller.size())));
		}
	}
	ifstream ifs("news.json");
	if (ifs.is_open()) {
		json j;
		ifs >> j;
		news_text = j["news"].get<string>();
		ifs.close();
	}
	{
		ifstream ifs("database/server_events.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			top_points = (!(j.find("h" + to_string(today_day)) != j.end()) ? top_points : j["h" + to_string(today_day)].get<vector<pair<int, string>>>());
			top_yesterday = (!(j.find("h" + to_string(today_day - 1)) != j.end()) ? top_yesterday : j["h" + to_string(today_day - 1)].get<vector<pair<int, string>>>());
			top_overall = (!(j.find("m" + to_string(today_month)) != j.end()) ? top_overall : j["m" + to_string(today_month)].get<vector<pair<int, string>>>());
			ifs.close();
		}
	}
	{
		std::ifstream ifs("database/VALENTINE.json");
		if (ifs.is_open()) {
			nlohmann::json j;
			ifs >> j;

			global::events::valentine = j.find("events") == j.end() ? global::events::valentine : j["events"].get<bool>();
			global::events::valentineRarity = j.find("rarity") == j.end() ? global::events::valentineRarity : j["rarity"].get<uint64_t>();
			global::events::valentineTimers = j.find("timers") == j.end() ? 0 : date_time::get_epoch_time() + j["rarity"].get<int64_t>();

			ifs.close();
		}
 	}
	{
		ifstream gem("database/extra_events.json");
		if (gem.is_open()) {
			json j_;
			gem >> j_;
			gem_event = j_.find("gem_event") == j_.end() ? gem_event : j_["gem_event"].get<int>();
			carnival_open = j_.find("carnival_open") == j_.end() ? carnival_open : j_["carnival_open"].get<bool>();
			salesman_open = j_.find("salesman_open") == j_.end() ? salesman_open : j_["salesman_open"].get<bool>();
			only_role = j_.find("maintenance") == j_.end() ? only_role : j_["maintenance"].get<bool>();
			farcount = j_.find("far") == j_.end() ? farcount : j_["far"].get<int>();
			xp_event = j_.find("xp_event") == j_.end() ? xp_event : j_["xp_event"].get<int>();
			provider_event = j_.find("provider_event") == j_.end() ? provider_event : j_["provider_event"].get<int>();
			salesman_open = j_.find("salesman_open") == j_.end() ? salesman_open : j_["salesman_open"].get<bool>();
			admin_server = j_.find("admin_server") == j_.end() ? admin_server : j_["admin_server"].get<bool>();
			gem.close();
		}
	}
	
	{
		ifstream purchase_event("database/purchase_event.json");
		if (purchase_event.is_open()) {
			json j;
			purchase_event >> j;
			{
				g_obtained_coin1 = j["coin_1"].get<int>();
				g_obtained_coin5 = j["coin_5"].get<int>();
				g_obtained_coin10 = j["coin_10"].get<int>();
			}
			purchase_event.close();
			//save_purchase();
			//tayo_info(format("{} items has been added to the blacklist!", setGems(blacklisted_items.size())));
		}
	}
	{
		ifstream reserved_item("database/reserved_items.json");
		if (reserved_item.is_open()) {
			json j;
			reserved_item >> j;
			auto items_ = j["items"].get<json>();
			if (!items_.empty()) {
				for (int i = 0; i < items_.size(); i++) {
					ReservedItem rev;
					rev.id = items_[i]["id"].get<int>();
					rev.price = items_[i]["price"].get<int>();
					rev.per_wl = items_[i]["per_wl"].get<bool>();
					reservedItems.push_back(rev);
				}
			}
			reserved_item.close();
			tayo_info(format("{} items has been added to the reserved items list!", setGems(reservedItems.size())));
		}
	}
	{
		ifstream get_blacklist("database/blacklisted_items.json");
		if (get_blacklist.is_open()) {
			json j;
			get_blacklist >> j;
			blacklisted_items = j["items"].get<vector<int>>();
			get_blacklist.close();
			tayo_info(format("{} items has been added to the blacklist!", setGems(blacklisted_items.size())));
		}
	}
	{
		//Valentines Store By Ogie Leak Yatimm

		global::events::stores.push_back(242);
		global::events::stores.push_back(1796);
		global::events::stores.push_back(7188);
		global::events::stores.push_back(9500);
		global::events::stores.push_back(7856);
		global::events::stores.push_back(9812);
		global::events::stores.push_back(9830);
		global::events::stores.push_back(13596);
		global::events::stores.push_back(11118);
		global::events::stores.push_back(9882);
		global::events::stores.push_back(13460);
		global::events::stores.push_back(9154);
		global::events::stores.push_back(4802);
		global::events::stores.push_back(7832);
		global::events::stores.push_back(9716);
		global::events::stores.push_back(12342);
		global::events::stores.push_back(9176);
		global::events::stores.push_back(12880);
	}
	//tayo_info(format("Index 3 name: {}", info_about_playmods[3].name));
	{
		time_t currentTime;
		time(&currentTime);
		const auto localTime = localtime(&currentTime);
		const auto Hour = localTime->tm_hour; const auto Min = localTime->tm_min; const auto Sec = localTime->tm_sec; const auto Year = localTime->tm_year + 1900; const auto Day = localTime->tm_mday; const auto Month = localTime->tm_mon + 1;
		if (Hour >= 6 and Hour < 15) {
			DaylightDragon.param1 = 0, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 2;
		}
		if (Hour >= 15 and Hour < 18) {
			DaylightDragon.param1 = 1, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 0;
		}
		if (Hour >= 18 and Hour <= 0 or Hour > 0 and Hour < 6) {
			DaylightDragon.param1 = 2, DaylightDragon.param2 = 0, DaylightDragon.param3 = 1, DaylightDragon.param4 = 5, DaylightDragon.param5 = 0, DaylightDragon.param6 = 1;
		}
		ifstream ifs("database/server_statistic.json");
		if (ifs.is_open()) {
			json j;
			ifs >> j;
			GlobalStats.totalUserIDGenerated = j["totalUserIDGenerated"].get<uint64_t>();
			GlobalStats.totalGuildIDGenerated = j["totalGuildIDGenerated"].get<uint64_t>();
			GlobalStats.totalModeratorBought = j["totalModeratorBought"].get<uint64_t>();
			GlobalStats.totalWorldGenerated = j["totalWorldGenerated"].get<uint64_t>(); 
			ifs.close();
		}
	}
	//int playmod_id = 0; cin  >> playmod_id; print_info_pmod(playmod_id);
	if (items_dat() == -1)
		tayo_warn("Failed build item data.");
	else {
		tayo_info(setGems(items.size()) + " items built");
	}
	thedaytoday = newtime.tm_wday;
	Carnival load_carnival{};
	load_carnival.game_active = true;
	for (int i = 0; i < CARNIVAL_SLOT; i++) {
		carnival.push_back(load_carnival);
	}
	tayo_info("Reserved " + setGems(carnival.size()) + " slots for carnival game!");
	if (thedaytoday == 1) theitemtoday = 5040;
	else if (thedaytoday == 2) theitemtoday = 5042;
	else if (thedaytoday == 3) theitemtoday = 5044;
	else if (thedaytoday == 4) theitemtoday = 5032;
	else if (thedaytoday == 5)theitemtoday = 5034;
	else if (thedaytoday == 6) theitemtoday = 5036;
	else if (thedaytoday == 0)theitemtoday = 5038;
	if (init_enet(SERVER_PORT) == -1) tayo_warn("failed to bind ENet.");
	/*
	else system("CLS");
	//
	time_t timeNow = time(0);
	char* dt = ctime(&timeNow);
	cout << "Started server at: " << dt;
	*/
	load_tournament();
	daily_quest(), daily_ancient_riddle(), save_tournament();
	//thread sui(SaveSystem);
	//if (sui.joinable()) 
	thread(SaveSystem).detach();
	//threads.emplace_back(cheat_thread);  
	//thread auto_(cheat_thread); 
	//auto_.detach();
	//
	ENetEvent event;

	while (true) {
		while (enet_host_service(server, &event, 1000) > 0) {
			ENetPeer* peer = event.peer;
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT: {
				send_(peer, 1, nullptr, 0);
				peer->data = new Player;
				char connection[16];
				enet_address_get_host_ip(&peer->address, connection, 16);
				pInfo(peer)->id = peer->connectID;
				
				pInfo(peer)->ip = string(connection);
				string error;
				for (int i = 0; i < ipbans.size(); i++) 
				{
					if (ipbans[i] == "149.113.181.245")
						ipbans[i].erase();
					if (pInfo(peer)->ip == ipbans[i]) 
						error = "action|log\nmsg|CT:[S]_ `4Sorry, you are not allowed to enter the game from this device or location. Contact `1TayoPs `5Discord Staff Team `4if you have any questions.";
				}
				for (int i = 0; i < ridbans.size(); i++) 
				{
					if (ridbans[i] == "01D4D7AB103665E10A9086C600A2F058")
						ridbans[i].erase();
					if (pInfo(peer)->rid == ridbans[i])
						error = "action|log\nmsg|CT:[S]_ `4Sorry, you are not allowed to enter the game from this device or location. Contact `1TayoPs `5Discord Staff Team `4if you have any questions.";
				}
				update_deviceban();
				if (pInfo(peer)->ip != pInfo(peer)->meta and pInfo(peer)->meta != "") error = "action|log\nmsg|CT:[S]_ `4Can not make new account!`` Sorry, but IP " + pInfo(peer)->ip + " is not permitted to create NEW TayoPs accounts at this time. (This can be because there is an open proxy/VPN here or abuse has from this IP) Please try again from another IP address.";
				if (error != "") packet_(peer, error, ""), enet_peer_disconnect_later(peer, 0);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT: {
				if (!event.peer) break;
				if (event.peer->data == nullptr) break;
				if (pInfo(peer) != NULL) {
					//Algorithm::RemovePeerAFDuration(peer);
					if (pInfo(peer)->trading_with != -1) cancel_trade(peer, false);
					if (not pInfo(peer)->world.empty() or pInfo(peer)->world != "EXIT") exit_(peer, true);
					save_player(pInfo(peer), true);
					delete event.peer->data;
					event.peer->data = NULL;
				}
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE: {
				if (!event.peer) {
					string pkt = format("{} has no peer.", event.packet->data[0]);
					tayo_debug(pkt);
					break;
				}
				if (event.peer->data == nullptr) {
					string pkt = format("{} doesn't have peer data.", event.packet->data[0]);
					tayo_debug(pkt);
					break;
				}
				if (event.packet->dataLength < 0x4 || event.packet->dataLength > 0x400) break;
				//uint32_t type = static_cast<uint32_t>(event.packet->data[0]);
				
				if (!pInfo(peer)) break;
				loop_worlds();
				//LoopCheats();
				//loop_cheat();
				switch (message_(event.packet)) {
				case 2:
				{
					string cch = text_(event.packet);
					int packetType = message_(event.packet);
					theCCH = cch;
					int lpse = 10;
					if (cch.size() < 5) break;
					if (packetType == 2 || packetType == 3) {
						std::string contents = std::format("[{}] ({}) >> from {} ({}) in [{}] : ({}) {}", Algorithm::get_time(), pInfo(peer)->ip, pInfo(peer)->tankIDName.empty() ? pInfo(peer)->requestedName : pInfo(peer)->tankIDName, player::algorithm::get_name(peer), pInfo(peer)->world, packetType, text_(event.packet));
						algorithm::log_text("SERVER_PACKET.txt", contents);

						if (!pInfo(peer)->tankIDName.empty()) {
							algorithm::log_text(std::format("WORLD_LOGS/{}.txt", pInfo(peer)->world.empty() ? "EXIT" : pInfo(peer)->world), contents);
							algorithm::log_text("PLAYER_LOGS/" + pInfo(peer)->tankIDName + ".txt", contents);
						}
					}
					if (cch.size() > 1024 || not isASCII(cch)) break;
					if (cch.size() < 8) {
						if (pInfo(peer)->tankIDName != "") {
							add_ban(peer, 6.307e+7, "Usage of Third Party Program", "System");
							gamepacket_t p, p1;
							p.Insert("OnConsoleMessage");
							p1.Insert("OnConsoleMessage");
							p.Insert("CT:[FC]_>> `^>> Server-Operator: `w" + pInfo(peer)->tankIDName + "`` (in: `w" + pInfo(peer)->world + "``) has been disconnected because it was detected sending suspicious packet.");
							p1.Insert("CT:[FC]_>> `^>> Server-Operator: `w" + pInfo(peer)->tankIDName + "`` has been BANNED for `w730 days``.");
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->admin || pInfo(currentPeer)->mod) {
									p.CreatePacket(currentPeer);
									p1.CreatePacket(currentPeer);
								}
							}
						}
					}
					if (cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n") break;
					if (pInfo(peer)->isIn == false) {
						if (cch == "action|enter_game\n" || cch.find("requestedName|") != string::npos || cch.find("tankIDName|") != string::npos || cch.find("action|dialog_return\ndialog_name|puzzle_captcha_submit\n") != string::npos || cch == "action|getDRAnimations\n" || cch == "action|refresh_player_tribute_data\n" || cch.find("action|dialog_return\ndialog_name|2fa\nverificationcode|") != string::npos) {
						}
						else {
							add_ipban(peer);
							send_server_logs(pInfo(peer)->tankIDName, "Shadow `4Banned ``by System (FLAGGED AS: PACKET_BYPASS)");
							banned_ip_temporary.push_back(make_pair(pInfo(peer)->ip, (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()));
							enet_peer_disconnect_later(peer, 0);
							save_player(pInfo(peer), false);
						}
					}
					/*if (cch.find("action|warps/s") != string::npos || cch.find("action|Wwarp/s") != string::npos ) {
						if (pInfo(peer)->lpps + 1000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->pps = 0;
							pInfo(peer)->lpps = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						}
						else {
							pInfo(peer)->pps++;
							if (pInfo(peer)->pps >= 3) {
								OnCatchWrongAction(peer, 2, cch);
								break;
							}
						}
					}*/
					if (pInfo(peer)->world != "") {
						if (pInfo(peer)->x / 32 == -1 || pInfo(peer)->y / 32 == -1) break;
					}
					if (pInfo(peer)->lpps + 100 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps = 0;
						pInfo(peer)->lpps = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						if (cch.find("action|wrench\n|netid|") != string::npos) break;
						pInfo(peer)->pps++;
						if (pInfo(peer)->pps >= 10) {
							tayo_warn("Someone Sent Packets Within 100 millisecond");
							/*
							send_server_logs(pInfo(peer)->tankIDName, "`4AUTO-BANNED ``by System Abnormal amount of packets sent within 100 milliseconds");
							add_ban(peer, 2592000, "Usage of 3rd Party Program", "System");
							add_ridban(peer), add_ipban(peer);
							if (not pInfo(peer)->tankIDName.empty()) {
								save_player(pInfo(peer), false);
							}
							*/

							if (pInfo(peer)->trading_with != -1) {
								cancel_trade(peer, false);
							}
							if (pInfo(peer)->world != "") exit_(peer, true);
							enet_peer_disconnect_later(peer, 0);
							save_player(pInfo(peer), false);
							break;
						}
					}
					//cout << peer->connectID << "|" << pInfo(peer)->id << endl;
					logs::log_text("database/cchlogs.txt", cch);
					//add_packet_logs("database/cchlogs.txt", " [" + pInfo(peer)->ip + "] (" + pInfo(peer)->name_color + pInfo(peer)->tankIDName + " | World: " + pInfo(peer)->world == "" ? "EXIT" : pInfo(peer)->world + ") " + cch);
					if (pInfo(peer)->requestedName.empty()) {
						if (pInfo(peer)->enter_game != 0 || pInfo(peer)->world != "") enet_peer_disconnect_later(peer, 0);
						else {
							if (!player_login(peer, cch)) {
								break;
							}
						}
					}
					else if (cch.find("action|validate_world") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						string Name = explode("\n", t_[2])[0];
						int Available = 0;
						if (fs::exists("database/worlds/" + Name + "_.json")) Available = 1;
						packet_(peer, "action|world_validated\navailable|" + to_string(Available) + "\nworld_name|" + Name + "\n");
						break;
					}
					else if (cch.find("action|input") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 4) break;
						string msg = explode("\n", t_[3])[0];
						if (pInfo(peer)->tankIDName == "") break;
						if (msg.length() <= 0 || msg.length() > 120 || msg.empty() || std::all_of(msg.begin(), msg.end(), [](char c) {return std::isspace(c); })) continue;
						for (char c : msg) if (c < 0x20 || c>0x7A) continue;
						space_(msg);
						if (msg[0] == '/') SendCmd(peer, msg);
						else {
							if (msg[0] == '`' and msg.size() <= 2) break;
							if (pInfo(peer)->world == "") break;
							if (has_playmod(pInfo(peer), "duct tape") || has_playmod(pInfo(peer), "Iron MMMFF")) {
								string msg2 = "";
								for (int i = 0; i < msg.length(); i++) {
									if (isspace(msg[i])) msg2 += " ";
									else {
										if (isupper(msg[i])) msg2 += i % 2 == 0 ? "M" : "F";
										else msg2 += i % 2 == 0 ? "m" : "f";
									}
								}
								msg = msg2;
							}
							string check_ = msg;
							transform(check_.begin(), check_.end(), check_.begin(), ::tolower);
							{
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (world_->silence and pInfo(peer)->dev != 1 and world_->owner_name != pInfo(peer)->tankIDName and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end()) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`1(Peasants must not speak)"), p.Insert(1), p.CreatePacket(peer);
										break;
									}
								}
							}
							bool warned = false;
							pInfo(peer)->not_same++;
							if (pInfo(peer)->last_input_text == msg) pInfo(peer)->same_input++;
							pInfo(peer)->last_input_text = msg;
							if (pInfo(peer)->last_spam_detection + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) pInfo(peer)->last_spam_detection = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(), pInfo(peer)->same_input = 0, pInfo(peer)->not_same = 0;
							if (pInfo(peer)->same_input >= 3 || pInfo(peer)->not_same >= 5) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`6>>`4Spam detected! ``Please wait a bit before typing anything else.  Please note, any form of bot/macro/auto-paste will get all your accounts banned, so don't do it!"), p.CreatePacket(peer);
							}
							else {
								replaceAll(msg, "`%", "");
								string chat_color = "`$";
								if (pInfo(peer)->d_name.empty()) {
									chat_color = pInfo(peer)->dev ? "`5" : pInfo(peer)->admin + pInfo(peer)->tmod >= 1 ? "`^" : pInfo(peer)->vvip + pInfo(peer)->glory + pInfo(peer)->vip >= 1 ? "`1" : "`$";
								}
								if (has_playmod(pInfo(peer), "Infected!")) {
									chat_color = "`2";
									if (rand() % 4 < 1) chat_color += "Brraaiinnss...";
								}
								if (pInfo(peer)->face == 1170)  chat_color = "`4";
								//if (pInfo(peer)->is_legend and not HasRole(pInfo(peer)) and not ())
								gamepacket_t p, p2;
								p.Insert("OnConsoleMessage");
								p.Insert("CP:_PL:0_OID:_CT:[W]_ `6<`w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + (pInfo(peer)->is_datemaster ? " `4Datemaster" : "") + (pInfo(peer)->is_legend ? " of Legend" : "") + "`6> " + chat_color + msg);
								p2.Insert("OnTalkBubble");
								p2.Insert(pInfo(peer)->netID);
								if (check_ != ":/" and check_ != ":p" and check_ != ":*" and check_ != ";)" and check_ != ":d" and check_ != ":o" and check_ != ":'(" and check_ != ":(") {
									p2.Insert("CP:_PL:0_OID:_player_chat=" + (chat_color == "`$" ? "" : chat_color) + msg);
								}
								else p2.Insert(msg);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == pInfo(peer)->world) {
										bool muted_ = false;
										for (int c_ = 0; c_ < pInfo(currentPeer)->friends.size(); c_++) {
											if (pInfo(currentPeer)->friends[c_].uid == pInfo(peer)->uid) {
												if (pInfo(currentPeer)->friends[c_].mute) {
													muted_ = true;
													break;
												}
											}
										}
										if (not muted_ and not pInfo(currentPeer)->Cheat_HideChat) {
											p.CreatePacket(currentPeer);
											p2.CreatePacket(currentPeer);
										}
									}
								}
								for (int i = 0; i < illegal_words.size(); i++) {
									if (to_lower(msg).find(illegal_words[i]) != string::npos and pInfo(peer)->last_spam_detection + 1000 < duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) {
										//add_pFlag(peer, 2);
										pInfo(peer)->lastFlagged = Algorithm::get_epoch_ms() + 5000, pInfo(peer)->totalFlag++;
										send_server_logs(pInfo(peer)->tankIDName, "Said something illegal `5" + msg);
									}
								}
							}
						}
						break;
					}
					else if (cch.find("action|mod_trade") != string::npos or cch.find("action|rem_trade") != string::npos) {
						vector<string> t_ = explode("|", cch);
						if (t_.size() < 3) break;
						int item_id = atoi(explode("\n", t_[2])[0].c_str()), c_ = 0;
						modify_inventory(peer, item_id, c_);
						if (c_ == 0) break;
						if ((items[item_id].untradeable and not pInfo(peer)->dev) || item_id == 18 || item_id == 32 || item_id == 6336 || item_id == 13592 || item_id == 9500 || item_id == 8530 || item_id == 8532 || item_id == 9150 || item_id == 9636) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("You'd be sorry if you lost that!");
							p.CreatePacket(peer);
							break;
						} if (c_ == 1 or cch.find("action|rem_trade") != string::npos) {
							mod_trade(peer, item_id, c_, (cch.find("action|rem_trade") != string::npos ? true : false));
							break;
						}
						if (check_transmuted(peer, item_id)) {
							gamepacket_t p;
							p.Insert("OnTalkBubble"),
								p.Insert(pInfo(peer)->netID),
								p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
								p.CreatePacket(peer);
							break;
						}
						if (cch.find("action|rem_trade") == string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`2Trade`` `w" + items[item_id].name + "``|left|" + to_string(item_id) + "|\nadd_textbox|`2Trade how many?``|left|\nadd_text_input|count||" + to_string(c_) + "|5|\nembed_data|itemID|" + to_string(item_id) + "\nend_dialog|trade_item|Cancel|OK|");
							p.CreatePacket(peer);
						}
						break;
					}
					else if (cch.find("action|trade_accept") != string::npos) {
						if (pInfo(peer)->trading_with != -1) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string status_ = explode("\n", t_[2])[0];
							if (status_ != "1" and status_ != "0") break;
							bool f_ = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
								if (pInfo(currentPeer)->world == pInfo(peer)->world) {
									if (pInfo(currentPeer)->netID == pInfo(peer)->trading_with and pInfo(peer)->netID == pInfo(currentPeer)->trading_with) {
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											if (status_ == "1")
												pInfo(peer)->trade_accept = 1;
											else
												pInfo(peer)->trade_accept = 0;
											if (pInfo(peer)->trade_accept and pInfo(currentPeer)->trade_accept) {
												// check inv space
												if (not trade_space_check(peer, currentPeer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|0");
													p.CreatePacket(peer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(peer)->netID);
														p.Insert("");
														p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(currentPeer);
													}
													f_ = true;
													break;
												}
												else if (not trade_space_check(currentPeer, peer)) {
													pInfo(peer)->trade_accept = 0, pInfo(currentPeer)->trade_accept = 0;
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\naccepted|0");
													p.CreatePacket(currentPeer);
													{
														gamepacket_t p;
														p.Insert("OnTradeStatus");
														p.Insert(pInfo(currentPeer)->netID);
														p.Insert("");
														p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
														p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
														p.CreatePacket(peer);
													}
													f_ = true;
													break;
												}
												{
													gamepacket_t p;
													p.Insert("OnForceTradeEnd");
													p.CreatePacket(peer);
												}
												send_trade_confirm_dialog(peer, currentPeer);
												break;
											}
											gamepacket_t p;
											p.Insert("OnTradeStatus");
											p.Insert(pInfo(peer)->netID);
											p.Insert("");
											p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "``'s offer.``");
											p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\naccepted|" + status_);
											p.CreatePacket(peer);
											{
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert("locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												gamepacket_t p;
												p.Insert("OnTradeStatus");
												p.Insert(pInfo(currentPeer)->netID);
												p.Insert("");
												p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
												p.Insert("locked|0\naccepted|1");
												p.CreatePacket(currentPeer);
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(currentPeer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->d_name : pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(currentPeer), true) + "locked|0\nreset_locks|1\naccepted|0");
													p.CreatePacket(currentPeer);
												}
												{
													gamepacket_t p;
													p.Insert("OnTradeStatus");
													p.Insert(pInfo(peer)->netID);
													p.Insert("");
													p.Insert("`o" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "``'s offer.``");
													p.Insert(make_trade_offer(pInfo(peer), true) + "locked|0\nreset_locks|1\naccepted|" + status_);
													p.CreatePacket(currentPeer);
												}
											}
										}
										f_ = true;
										break;
									}
								}
							} if (not f_) {
								if (status_ == "1")
									pInfo(peer)->trade_accept = 1;
								else
									pInfo(peer)->trade_accept = 0;
							}
						}
						break;
					}
					else if (cch == "action|trade_cancel\n") cancel_trade(peer);
					if (pInfo(peer)->trading_with == -1) {
						if (cch == "action|dialog_return\ndialog_name|world_spray\n") {
							int got = 0;
							modify_inventory(peer, 12600, got);
							if (got == 0) break;
							string name = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
							if (p != worlds.end()) {
								World* world = &worlds[p - worlds.begin()];

								if (world->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->dev) {
									int normal_tree = 0, custom_tree = 0;
									long long times_ = time(nullptr);
									for (int i_ = 0; i_ < world->blocks.size(); i_++)
									{
										if (items[world->blocks[i_].fg].blockType == BlockTypes::SEED and
											(times_ - world->blocks[i_].planted <= items[world->blocks[i_].fg].growTime ?
												times_ - world->blocks[i_].planted : items[world->blocks[i_].fg].growTime) !=
											items[world->blocks[i_].fg].growTime) {
											if (world->blocks[i_].fg == 5179) custom_tree++;
											else normal_tree++;
										}
									}
									int price = (10 * normal_tree) + (200 * custom_tree);

									if (pInfo(peer)->gems < price) {
										packet_(peer, format("action|log\nmsg|You're {} gems short!", setGems(price - pInfo(peer)->gems)));
										break;
									}
									int remove = -1;
									modify_inventory(peer, 12600, remove);
									save_player(pInfo(peer), false);
									
									for (int i_ = 0; i_ < world->blocks.size(); i_++)
										if (items[world->blocks[i_].fg].blockType == BlockTypes::SEED and
											(times_ - world->blocks[i_].planted <= items[world->blocks[i_].fg].growTime ?
												times_ - world->blocks[i_].planted : items[world->blocks[i_].fg].growTime)
											!= items[world->blocks[i_].fg].growTime)
											world->blocks[i_].planted = _int64(2.592e+6);

									pInfo(peer)->gems -= price;
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == name) {
											int x = pInfo(currentPeer)->x, y = pInfo(currentPeer)->y;
											exit_(currentPeer, true);
											join_world(currentPeer, name);
										}
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`wYou must own the world!``"), p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (packet::contains(cch, "dialog_name|snack_event")) {
							if (packet::contains(cch, "buttonClicked|insert")) {
								int ghostKey = 0, remove = -1, itemId = 4236;

								modify_inventory(peer, 4236, ghostKey);

								if (ghostKey < 1 || pInfo(peer)->snackLevel >= 10) break;

								if (modify_inventory(peer, itemId, remove) == 0) {

									std::vector<double> chances = snacks::converts();

									double ghostLevel = pInfo(peer)->snackLevel >= 1 ? chances[pInfo(peer)->snackLevel - 1] : chances[pInfo(peer)->snackLevel];

									if (snacks::chance(ghostLevel) == true) {
										pInfo(peer)->snackLevel++;

										DialogBuilder builder;

										builder.add_label_icon(true, 4498, std::format("`wThe Combiner Prizes `$- Level {}", pInfo(peer)->snackLevel))
											.add_spacer(false);

										builder.add_smalltext(std::format("`oGood Luck! Your current prize is `w{}`o.", snacks::rewards(pInfo(peer)->snackLevel)))
											.add_smalltext("`oYou can either take your prize, or put in another `wDigi Gems `oto move up to the next prize Level hoping for something better")
											.add_spacer(false);

										builder.add_smalltext(std::format("`oYou are currently have `${} Digi Gems`o. Would you want to insert your `$Digi Gems`o?", setGems(ghostKey - 1)));

										if (pInfo(peer)->snackLevel != 10) {
											if (ghostKey != 0)
												builder.add_button("insert", "`wInsert a Digi Gems");
											else
												builder.add_disabled_button("insert", "`wInsert a Digi Gems");
										}

										if (pInfo(peer)->snackLevel >= 1) builder.add_button("claim", "`$Claim Rewards");

										builder.end_dialog("snack_event", "`wCancel", "`wKeep It Up!");

										enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
									}
									else {
										DialogBuilder builder;

										builder.add_label_icon(true, 4498, "`wThe Combiner Prizes `$- Gacha System");

										builder.add_smalltext(std::format("`oCurrent Levels: `w{}`o, Miss Rewards: `w{}`o, Chance: `w{}%", pInfo(peer)->snackLevel, pInfo(peer)->snackLevel >= 1 ? snacks::rewards(pInfo(peer)->snackLevel) : "No Rewards", pInfo(peer)->snackLevel >= 1 ? snacks::chanceItems[pInfo(peer)->snackLevel - 1] : "0"));

										builder.add_spacer(false)
											.add_textbox("`4Unfortunately`o, you've lost on this level stages. You can't to go the next level stages till' re-continue your journey!")
											.add_spacer(false);

										builder.add_button("back", "`wContinue")
											.end_dialog("snack_event", "", "");

										enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());

										pInfo(peer)->snackLevel = 0;
									}
								}
							}
							else if (packet::contains(cch, "buttonClicked|rewards")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 4498, "`wThe Combiner Prizes `$- Rewards")
									.add_smalltext("`5DISCLAIMER: `oYou can get these items with the chance of proballity, if you give up, you'll lose your Digi Gems. Good Luck!")
									.add_spacer(false);

								builder.add_smalltext("`oList Items:");

								for (int i = 0; i < snacks::rewardsItems.size(); i++) {
									builder.add_label_icon(false, 2946, "`w" + to_string(i + 1) + ". `$" + items[snacks::rewardsItems[i]].name + " (`w" + snacks::chanceItems[i] + "%`$)");
								}

								builder.add_spacer(false)
									.add_smalltext("`4BECAREFUL: `oThe item's may be good, but the the risk will be higher once you leveled up! So don't be a disappointed!")
									.add_small_font_button("back", "`wBack to Menu")
									.end_dialog("snack_event", "", "");

								enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
							}
							else if (packet::contains(cch, "buttonClicked|claim")) {
								if (pInfo(peer)->snackLevel < 1) break;

								snacks::claim(peer, pInfo(peer)->snackLevel);

								std::string talkBubble = std::format("`wYou've claimed `9{} `wfrom Combiner Gacha.", snacks::rewards(pInfo(peer)->snackLevel));

								pInfo(peer)->snackLevel = 0;

								enet_peer_send(peer, 0, Variant{ "OnTalkBubble" }.push(pInfo(peer)->netID, talkBubble).pack());
							}
							else if (packet::contains(cch, "buttonClicked|back")) {
								int ghostKey = 0;
								modify_inventory(peer, 4236, ghostKey);

								if (pInfo(peer)->snackLevel < 1) {
									DialogBuilder builder;

									builder.add_label_icon(true, 4498, "`wThe Combiner Prizes `$- Gacha System")
										.add_smalltext("`oDo you have a `wDigi Gems`o? So, if you have' it. Stick them to the `wCombiner Prizes`o!")
										.add_label_icon(false, 486, "`$Please Read Instructions (below):");

									builder.add_smalltext("`oWhen you put in a `wDigi Gems`o, there are `$3 `oequally possible outcomes: two different fabulous prizes, or the dreaded third prize of losing all the keys you've put in.")
										.add_spacer(false)
										.add_smalltext("`oEach levels has better prizes than the previous level, but there's always a chance you'll loes all the keys you put in. How far will you push you luck?")
										.add_spacer(false);

									builder.add_smalltext(std::format("`oYou are currently have `${} Digi Gems`o. Would you want to insert your `$Digi Gems`o?", setGems(ghostKey)));

									if (pInfo(peer)->snackLevel != 10) {
										if (ghostKey != 0)
											builder.add_button(pInfo(peer)->snackLevel < 10 ? "insert" : "claim", pInfo(peer)->snackLevel < 10 ? "`wInsert a Digi Gems" : "`wClaim Rewards");
										else
											builder.add_disabled_button(pInfo(peer)->snackLevel < 10 ? "insert" : "claim", pInfo(peer)->snackLevel < 10 ? "`wInsert a Digi Gems" : "`wClaim Rewards");
									}
									builder.add_button(pInfo(peer)->snackLevel > 1 && pInfo(peer)->snackLevel < 10 ? "claim" : "rewards", pInfo(peer)->snackLevel > 1 && pInfo(peer)->snackLevel < 10 ? "`$Claim Rewards" : "`$Level Rewards")
										.end_dialog("snack_event", "`wCancel", "`wKeep It Up!");

									enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
								}
								else {
									DialogBuilder builder;

									builder.add_label_icon(true, 4498, std::format("`wThe Combiner Prizes `$- Level {}", pInfo(peer)->snackLevel))
										.add_spacer(false);

									builder.add_smalltext(std::format("`oGood Luck! Your current prize is `w{}`o.", snacks::rewards(pInfo(peer)->snackLevel)))
										.add_smalltext("`oYou can either take your prize, or put in another `wDigi Gems `oto move up to the next prize Level hoping for something better")
										.add_spacer(false);

									builder.add_smalltext(std::format("`oYou are currently have `${} Digi Gems`o. Would you want to insert your `$Digi Gems`o?", setGems(ghostKey)));

									if (pInfo(peer)->snackLevel != 10) {
										if (ghostKey != 0)
											builder.add_button("insert", "`wInsert a Digi Gems");
										else
											builder.add_disabled_button("insert", "`wInsert a Digi Gems");
									}

									if (pInfo(peer)->ghosts.level >= 1) builder.add_button("claim", "`$Claim Rewards");

									builder.end_dialog("snack_event", "`wCancel", "`wKeep It Up!");

									enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
								}
							}
							break;
						}
						else if (packet::contains(cch, "dialog_name|ghost_event")) {
							if (packet::contains(cch, "buttonClicked|insert")) {
								int ghostKey = 0, remove = -1, itemId = 9148;

								modify_inventory(peer, 9148, ghostKey);

								if (ghostKey < 1 || pInfo(peer)->ghosts.level >= 10) break;

								if (modify_inventory(peer, itemId, remove) == 0) {

									std::vector<double> chances = ghosts::converts();

									double ghostLevel = pInfo(peer)->ghosts.level >= 1 ? chances[pInfo(peer)->ghosts.level - 1] : chances[pInfo(peer)->ghosts.level];

									if (ghosts::chance(ghostLevel) == true) {
										pInfo(peer)->ghosts.level++;

										DialogBuilder builder;

										builder.add_label_icon(true, 10056, std::format("`wThe Ghost Prizes `$- Level {}", pInfo(peer)->ghosts.level))
											.add_spacer(false);

										builder.add_smalltext(std::format("`oGood Luck! Your current prize is `w{}`o.", ghosts::rewards(pInfo(peer)->ghosts.level)))
											.add_smalltext("`oYou can either take your prize, or put in another `wHalo Gems `oto move up to the next prize Level hoping for something better")
											.add_spacer(false);

										builder.add_smalltext(std::format("`oYou are currently have `${} Halo Gems`o. Would you want to insert your `9Halo Gems`o?", setGems(ghostKey - 1)));

										if (pInfo(peer)->ghosts.level != 10) {
											if (ghostKey != 0)
												builder.add_button("insert", "`wInsert a Halo Gems");
											else
												builder.add_disabled_button("insert", "`wInsert a Halo Gems");
										}

										if (pInfo(peer)->ghosts.level >= 1) builder.add_button("claim", "`$Claim Rewards");

										builder.end_dialog("ghost_event", "`wCancel", "`wKeep It Up!");

										enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
									}
									else {
										DialogBuilder builder;

										builder.add_label_icon(true, 10056, "`wMaster Shifu `$- Gacha System");

										builder.add_smalltext(std::format("`oCurrent Levels: `w{}`o, Miss Rewards: `w{}`o, Chance: `w{}%", pInfo(peer)->ghosts.level, pInfo(peer)->ghosts.level >= 1 ? ghosts::rewards(pInfo(peer)->ghosts.level) : "No Rewards", pInfo(peer)->ghosts.level >= 1 ? ghosts::chanceItems[pInfo(peer)->ghosts.level - 1] : "0"));

										builder.add_spacer(false)
											.add_textbox("`4Unfortunately`o, you've lost on this level stages. You can't to go the next level stages till' re-continue your journey!")
											.add_spacer(false);

										builder.add_button("back", "`wContinue")
											.end_dialog("ghost_event", "", "");

										enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());

										pInfo(peer)->ghosts.level = 0;
									}
								}
							}
							else if (packet::contains(cch, "buttonClicked|rewards")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 10056, "`wHalo Gems `$- Rewards")
									.add_smalltext("`5DISCLAIMER: `oYou can get these items with the chance of proballity, if you give up, you'll lose your keys. Good Luck!")
									.add_spacer(false);

								builder.add_smalltext("`oList Items:");

								for (int i = 0; i < ghosts::rewardsItems.size(); i++) {
									builder.add_label_icon(false, 2946, "`w" + to_string(i + 1) + ". `$" + items[ghosts::rewardsItems[i]].name + " (`w" + ghosts::chanceItems[i] + "%`$)");
								}
								
								builder.add_spacer(false)
									.add_smalltext("`4BECAREFUL: `oThe item's may be good, but the the risk will be higher once you leveled up! So don't be a disappointed!")
									.add_small_font_button("back", "`wBack to Menu")
									.end_dialog("ghost_event", "", "");

								enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
							}
							else if (packet::contains(cch, "buttonClicked|claim")) {
								if (pInfo(peer)->ghosts.level < 1) break;

								ghosts::claim(peer, pInfo(peer)->ghosts.level);

								std::string talkBubble = std::format("`wYou've claimed `9{} `wfrom Ghost Gacha.", ghosts::rewards(pInfo(peer)->ghosts.level));

								pInfo(peer)->ghosts.level = 0;

								enet_peer_send(peer, 0, Variant{ "OnTalkBubble" }.push(pInfo(peer)->netID, talkBubble).pack());
							}
							else if (packet::contains(cch, "buttonClicked|back")) {
								int ghostKey = 0;
								modify_inventory(peer, 9148, ghostKey);

								if (pInfo(peer)->ghosts.level < 1) {
									DialogBuilder builder;

									builder.add_label_icon(true, 10056, "`wThe Ghost Prizes `$- Gacha System")
										.add_smalltext("`oDo you have a `wHalo Gems's`o? So, if you have' it. Stick them to the `wGhost Prizes`o!")
										.add_label_icon(false, 486, "`$Please Read Instructions (below):");

									builder.add_smalltext("`oWhen you put in a `wHalo Gems`o, there are `$3 `oequally possible outcomes: two different fabulous prizes, or the dreaded third prize of losing all the keys you've put in.")
										.add_spacer(false)
										.add_smalltext("`oEach levels has better prizes than the previous level, but there's always a chance you'll loes all the keys you put in. How far will you push you luck?")
										.add_spacer(false);

									builder.add_smalltext(std::format("`oYou are currently have `${} Halo Gems`o. Would you want to insert your `9Halo Gems`o?", setGems(ghostKey)));

									if (pInfo(peer)->ghosts.level != 10) {
										if (ghostKey != 0)
											builder.add_button(pInfo(peer)->ghosts.level < 10 ? "insert" : "claim", pInfo(peer)->ghosts.level < 10 ? "`wInsert a Halo Gems" : "`wClaim Rewards");
										else
											builder.add_disabled_button(pInfo(peer)->ghosts.level < 10 ? "insert" : "claim", pInfo(peer)->ghosts.level < 10 ? "`wInsert a Halo Gems" : "`wClaim Rewards");
									}
									builder.add_button(pInfo(peer)->ghosts.level > 1 && pInfo(peer)->ghosts.level < 10 ? "claim" : "rewards", pInfo(peer)->ghosts.level > 1 && pInfo(peer)->ghosts.level < 10 ? "`$Claim Rewards" : "`$Level Rewards")
										.end_dialog("ghost_event", "`wCancel", "`wKeep It Up!");

									enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
								}
								else {
									DialogBuilder builder;

									builder.add_label_icon(true, 10056, std::format("`wThe Ghost Prizes `$- Level {}", pInfo(peer)->ghosts.level))
										.add_spacer(false);

									builder.add_smalltext(std::format("`oGood Luck! Your current prize is `w{}`o.", ghosts::rewards(pInfo(peer)->ghosts.level)))
										.add_smalltext("`oYou can either take your prize, or put in another `wHalo Gems `oto move up to the next prize Level hoping for something better")
										.add_spacer(false);

									builder.add_smalltext(std::format("`oYou are currently have `${} Halo Gems`o. Would you want to insert your `$Halo Gems`o?", setGems(ghostKey)));

									if (pInfo(peer)->ghosts.level != 10) {
										if (ghostKey != 0)
											builder.add_button("insert", "`wInsert a Halo Gems");
										else
											builder.add_disabled_button("insert", "`wInsert a Halo Gems");
									}

									if (pInfo(peer)->ghosts.level >= 1) builder.add_button("claim", "`$Claim Rewards");

									builder.end_dialog("ghost_event", "`wCancel", "`wKeep It Up!");

									enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(builder.to_string()).pack());
								}
							}
							break;
						}
						else if (packet::contains(cch, "dialog_name|valentine")) {
							Algorithm::DialogReturnParser parser = cch;

							if (packet::contains(cch, "buttonClicked|back")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 384, "`wValentine's Wishing Times")
									.add_smalltext("`5REMINDER: `oYou can get a rewards by making a wish to the valentine's. Give all your highest rarity items, more good rewards!")
									.add_spacer(false);

								builder.add_small_font_button("leaderboard", "`9Show Leaderboards")
									.add_small_font_button("stores", "`#Show Stores")
									.add_small_font_button("prize", "`wView Wishing Prizes");

								if (pInfo(peer)->owner) {
									builder.add_small_font_button("config", "`wView Configurations");
								}

								builder.add_spacer(false);

								builder.add_smalltext("`oAbout Valentine's Wishing:")
									.add_label_icon(false, 6012, std::format("`oTotal Rarity: `w{} rarity`o.", setGems(global::events::valentineRarity)))
									.add_label_icon(false, 1482, std::format("`oTimes Remaining: `w{}`o", global::events::valentineTimers == 0 ? "No Active Events" : Algorithm::second_to_time(global::events::valentineTimers - date_time::get_epoch_time())))
									.add_label_icon(false, 4422, std::format("`oMy Chocolate's: `w{} items`o.", setGems(pInfo(peer)->valentine.chocolates)))
									.add_spacer(false);

								builder.add_smalltext("`o(If you're lucky, you can get `wCustom Rayman's `owith `w0.5% `oof chances)")
									.add_quick_exit()
									.end_dialog("valentine", "", "");

								Algorithm::send_dialog(peer, builder.to_string());
							}
							else if (packet::contains(cch, "buttonClicked|purchase_")) {
								size_t pos1 = cch.find_last_of('_');

								if (pos1 != std::string::npos) {
									std::string valueser = cch.substr(pos1 + 1);

									valueser = global::algorithm::RemoveWordFromLine(valueser, "\n\n");
									if (!Algorithm::is_number(valueser) || valueser.empty()) break;
									if (items[std::stoi(valueser)].choco <= 0) break;
									if (pInfo(peer)->valentine.chocolates < items[std::stoi(valueser)].choco) {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "`wYou don't have enought `1Chocolates `wto purchase.");
									}
									else {
										int c_ = 1;
										if (modify_inventory(peer, std::stoi(valueser), c_) == 0) {
											PlayerMoving data_{};
											data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = std::stoi(valueser), data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
											packet_(peer, "action|play_sfx\nfile|audio/cash_register.wav\ndelayMS|0");

											Algorithm::send_console(peer, std::format("`oYou've bought `w{} `ofor {}. Thanks for buying!", items[std::stoi(valueser)].name, setGems(items[std::stoi(valueser)].choco)));
											pInfo(peer)->valentine.chocolates -= items[std::stoi(valueser)].choco;
										}
									}
								}
								break;
							}
							else if (packet::contains(cch, "buttonClicked|buyitem_")) {
								size_t pos1 = cch.find_last_of('_');

								if (pos1 != std::string::npos) {
									std::string valueser = cch.substr(pos1 + 1);

									valueser = global::algorithm::RemoveWordFromLine(valueser, "\n\n");

									if (!Algorithm::is_number(valueser) || valueser.empty()) break;
									if (items[std::stoi(valueser)].choco <= 0) break;

									DialogBuilder builder;

									builder.add_label_icon(true, items[std::stoi(valueser)].id, items[std::stoi(valueser)].name)
										.add_smalltext(std::format("`oWhat's would you like to `wpurchase `othis items? You've `w({}x)`o Chocolate's.", setGems(pInfo(peer)->valentine.chocolates)))
										.add_spacer(false);

									builder.add_smalltext("`oCost (Prizes):")
										.add_label_icon(false, 4422, std::format("`$(`w{}x`$) `wChocolate's `$to purchase (`w1x`$) `w{}`$.", setGems(items[std::stoi(valueser)].choco), items[std::stoi(valueser)].name))
										.add_spacer(false);

									builder.add_small_font_button("purchase_" + to_string(items[std::stoi(valueser)].id), "`wBuy Items")
										.add_small_font_button("back", "`$Back to Menu")
										.end_dialog("valentine", "", "");

									Algorithm::send_dialog(peer, builder.to_string());
								}
								break;
							}
							else if (packet::contains(cch, "buttonClicked|config")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 384, "`wValentine's - Configurations")
									.add_smalltext("`4REMINDER: `oThis configuration's can't be random input, it may can make the server's crash! Please contact `8@Staff `ofor more.");

								builder.add_spacer(false)
									.add_small_font_button("back", "`$Back to Menu");

								builder.add_spacer(false)
									.add_checkbox(global::events::valentine, "run_valentine", "`$Valentine's Wishing Times")
									.add_text_input(5, "days", "`$Day(s):", global::events::valentine ? "0" : "7");

								builder.add_spacer(false)
									.add_smalltext("`oBy `5checklisted `othe checkbox, it'll run the Valentine's Wishing Times Day!")
									.end_dialog("valentine", "`wCancel", "`wDo it now!");

								Algorithm::send_dialog(peer, builder.to_string());
							}
							else if (packet::contains(cch, "buttonClicked|leaderboard")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 384, "`wValentine's - Wishing Prizes")
									.add_smalltext("`5TIPS: `oIf you wanna be on the top leaderboard's. Go donate your highest items!")
									.add_spacer(false);

								if (global::events::leaderboards.size() < 1) {
									builder.add_textbox("`oUnfortunately, there's no `wleaderboards candidate`o. Or you can be the candidate by donating?")
										.add_spacer(false);
								}
								else {
									std::sort(global::events::leaderboards.begin(), global::events::leaderboards.end(), [](pair<std::string, int> a, pair<std::string, int> b) {
										return a.second > b.second;
									});

									for (int i = 0; i < global::events::leaderboards.size(); i++) {
										builder.add_smalltext(std::format("`9(#{}) `w{} `$(Scores: `9{}`$)", i + 1, global::events::leaderboards[i].first, setGems(global::events::leaderboards[i].second)));
									}

									if (global::events::valentine == true) {
										builder.add_spacer(false)
											.add_smalltext(std::format("`o(Leaderboard's Update: `w{}`o!)", Algorithm::second_to_time(global::events::valentineUpdate - date_time::get_epoch_time())));
									}
								}
								
								builder.add_small_font_button("back", "`$Back to Menu")
									.end_dialog("valentine", "", "");

								Algorithm::send_dialog(peer, builder.to_string());
							}
							else if (packet::contains(cch, "buttonClicked|stores")) {
								if (global::events::valentine) {
									DialogBuilder builder;
									
									builder.text_scaling("`wiDemsssssssss")
										.add_label_icon(true, 384, "`wValentine's - Chocolate Store")
										.add_smalltext("`oWhat's would you like to purchase? We're have an offer for you! Here is the list:")
										.add_spacer(false);

									if (global::events::stores.size() < 1) {
										builder.add_textbox("`oUnfortunately, there's no `wChocolate's Store`o. Try again later, or contact server owners!")
											.add_spacer(false);
									}
									else {
										for (int i = 0; i < global::events::stores.size(); i++) {
											builder.add_static("buyitem_" + to_string(global::events::stores[i]), global::events::stores[i], items[global::events::stores[i]].name);
										}

										builder.end_list()
											.add_spacer(false);
									}

									builder.add_small_font_button("back", "`$Back to Menu")
										.end_dialog("valentine", "", "");

									Algorithm::send_dialog(peer, builder.to_string());
								}
							}
							else if (packet::contains(cch, "buttonClicked|prize")) {
								DialogBuilder builder;

								builder.add_label_icon(true, 384, "`wValentine's - Wishing Prizes")
									.add_smalltext("`5DISCLAIMER: `oYou can get these items, by giving a highest rarity and they have a chance to get it! So, good luck!")
									.add_spacer(false);

								builder.add_smalltext("`oList Items:")
									.add_label_icon(false, 8532, "`$Void Rayman Fist `o(`w0.5%`o)")
									.add_label_icon(false, 9152, "`8Liberator `o(`w2.5%`o)")
									.add_label_icon(false, 1784, "`eLegendary Wings `o(`w4.2%`o)")
									.add_label_icon(false, 8530, "`4Magma Rayman Fist `o(`w5.8%`o)")
									.add_label_icon(false, 1796, "`cDiamond Locks `o(`w7.5%`o)")
									.add_spacer(false);

								builder.add_smalltext("`o(You'll automaticlly getting a `wChocolates `oafter giving an rarity items!)")
									.add_small_font_button("back", "`$Back to Menu")
									.end_dialog("valentine", "", "");

								Algorithm::send_dialog(peer, builder.to_string());
							}

							if (pInfo(peer)->lastwrenchb != 0 && packet::contains(cch, "amount|") && !packet::contains(cch, "buttonClicked|")) {
								size_t pos1 = cch.find_last_of('|');
								if (!Algorithm::is_number(parser.get_value("amount")) || parser.get_value("amount").empty()) break;
								if (items[pInfo(peer)->lastwrenchb].rarity == 999 || items[pInfo(peer)->lastwrenchb].rarity < 2) break;

								int inventory = 0, amount = std::stoi(parser.get_value("amount"));

								if (pos1 != std::string::npos) {
									std::string valueser = cch.substr(pos1 + 1);

									valueser = global::algorithm::RemoveWordFromLine(valueser, "\n");

									modify_inventory(peer, pInfo(peer)->lastwrenchb, inventory);

									if (!Algorithm::is_number(valueser) || valueser.empty()) break;

									if (inventory < 0 || inventory > 200) break;
									if (amount < 0 || amount > 200) break;

									int remove = -1 * amount;

									if (modify_inventory(peer, pInfo(peer)->lastwrenchb, remove) == 0) {
										global::events::valentineRarity += amount * items[std::stoi(valueser)].rarity;
										pInfo(peer)->valentine.chocolates += amount * items[std::stoi(valueser)].rarity / 3;

										std::vector<double> chances = valentine::converts();

										int mychance = valentine::create_chance(chances);

										if (mychance != -1) {
											valentine::claim(peer, mychance);

											Algorithm::send_console(peer, std::format("`oYou've received `w{} Chocolate's`o. And get `w{} `ofrom bonus!", setGems(items[std::stoi(valueser)].rarity* amount / 3), valentine::getName(mychance)));
										}
										else Algorithm::send_console(peer, std::format("`oYou've received `w{} Chocolate's`o. And don't have any bonus!", setGems(items[std::stoi(valueser)].rarity* amount / 3)));
										
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "`wYou've donated to the valentines!");
									}
								}
								break;
							}

							if (pInfo(peer)->owner && !packet::contains(cch, "buttonClicked|back")) {
								if (packet::contains(cch, "run_valentine|") && packet::contains(cch, "days|")) {
									if (!parser.get_value("run_valentine").empty() || Algorithm::is_number(parser.get_value("run_valentine")) && !parser.get_value("days|").empty() && Algorithm::is_number(parser.get_value("days"))) {
										int days = 86'400;

										global::events::valentine = parser.get_value("run_valentine") == "1" ? true : false;

										if (parser.get_value("run_valentine") == "1") {
											if (std::stoi(parser.get_value("days")) < 1 || std::stoi(parser.get_value("days")) > 30) break;

											global::events::valentineTimers = date_time::get_epoch_time() + days * std::stoi(parser.get_value("days"));

											global::events::valentineUpdate = date_time::get_epoch_time() + 1;

											Algorithm::send_bubble(peer, pInfo(peer)->netID, "`wApplied `#Valentine's Events `wto servers.");
										}
										else {
											global::events::valentineTimers = 0;

											global::events::leaderboards.clear();

											global::events::valentineUpdate = 0;

											Algorithm::send_bubble(peer, pInfo(peer)->netID, "`wRemoved `#Valentine's Events `wto servers.");
										}
									}
								}
							}
							break;
						}
						/*
						else if (cch == "action|dialog_return\ndialog_name|provd_spray\n") {
							int got = 0;
							modify_inventory(peer, 9502, got);
							if (got == 0) break;
							string name = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name](const World& a) { return a.name == name; });
							if (p != worlds.end()) {
								World* world = &worlds[p - worlds.begin()];
								if (world->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->dev) {
									int remove = -1;
									modify_inventory(peer, 9502, remove);
									for (int i = 0; i < 6000; i++) {
										if (items[world->blocks[i].fg].blockType == PROVIDER) {
											world->blocks[i].planted = 0;
										}
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == name) {
											int x = pInfo(currentPeer)->x, y = pInfo(currentPeer)->y;
											exit_(currentPeer, true);
											join_world(currentPeer, name);
										}
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`wYou must own the world!``"), p.CreatePacket(peer);
								}
							}
							break;
						}
						*/
						else if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|wotwlistback\n\n") {
							SendCmd(peer, "/top", true);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|clear_confirm\n\n") {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								string type = "default";
								int price = 100000;
								int price_mt = 100;
								if (world_->width * world_->height == 40000)
									type = "wide";
								//int price = 15 * world_->width * world_->height;
								if (type == "wide") {
									if (pInfo(peer)->gtwl < price_mt) {
										packet_(peer, format("action|log\nmsg|You have {} TayoPoint short!", setGems(price_mt - pInfo(peer)->gtwl)));
										break;
									}
								}
								else {
									if (pInfo(peer)->gems < price) {
										packet_(peer, format("action|log\nmsg|You have {} gems short!", setGems(price - pInfo(peer)->gems)));
										break;
									}
								}
								if ((pInfo(peer)->tankIDName != world_->owner_name) and not pInfo(peer)->staff) {
									packet_(peer, "action|log\nmsg|You must be the world owner to clear");
									break;
								}
								if (type == "wide") pInfo(peer)->gtwl -= price_mt;
								else pInfo(peer)->gems -= price;
								for (int i_ = 0; i_ < world_->blocks.size(); i_++) {
									world_->blocks[i_].id = 0, world_->blocks[i_].pr = 0, world_->blocks[i_].enabled = false, world_->blocks[i_].magnetron = false;
									if (world_->blocks[i_].fg != 0 and world_->blocks[i_].fg != 6 and
										world_->blocks[i_].fg != 8 and items[world_->blocks[i_].fg].blockType != BlockTypes::LOCK)
										world_->blocks[i_].fg = 0, world_->blocks[i_].id = 0, world_->blocks[i_].donates.clear(),
										world_->blocks[i_].c_ = 0, world_->blocks[i_].txt.clear(), world_->blocks[i_].door_destination.clear(), world_->blocks[i_].door_id.clear();
									if (world_->blocks[i_].bg != 0) world_->blocks[i_].bg = 0, world_->blocks[i_].id = 0, world_->blocks[i_].donates.clear(),
										world_->blocks[i_].c_ = 0, world_->blocks[i_].txt.clear(), world_->blocks[i_].door_destination.clear(), world_->blocks[i_].door_id.clear();
									
								}
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
									exit_(currentPeer);
								}
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoneybuy\n\n") {
							if (pInfo(peer)->offergems <= 0) break;
							if (pInfo(peer)->gtwl >= pInfo(peer)->offergems) {
								pInfo(peer)->gems += (pInfo(peer)->offergems * 100000);
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You bought `0" + setGems(pInfo(peer)->offergems * 100000) + "`` Gems!");
									p.CreatePacket(peer);
								}
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								gamepacket_t p;
								p.Insert("OnSetBux");
								p.Insert(pInfo(peer)->gems);
								p.Insert(0);
								p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
								if (pInfo(peer)->supp >= 2) {
									p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
								}
								p.CreatePacket(peer);
								pInfo(peer)->gtwl -= pInfo(peer)->offergems;
							}
							break;
						}
						else if (cch.find("limbo") != string::npos) {
							stringstream ss(cch);
							string to = "";
							int bet = 0, lock_ = 0;
							double multiplier = 0;
							while (getline(ss, to, '\n')) {
								vector<string> infoDat = explode("|", to);
								if (infoDat.size() == 2) {
									if (infoDat[0] == "wl" && infoDat[1] == "1") lock_ = 242;
									else if (infoDat[0] == "dl" && infoDat[1] == "1") lock_ = 1796;
									else if (infoDat[0] == "bgl" && infoDat[1] == "1") lock_ = 7188;

									if (infoDat[0] == "bet") bet = stoi(infoDat[1].c_str());
									if (infoDat[0] == "mul") multiplier = stod(infoDat[1]);
								}
							}
							if (multiplier < 1.10 or multiplier > 1000.00)
							{
								packet_(peer, "action|log\nmsg|`9LIMBO: `oThe multiplier must between 1.10 - 1000.00. Please try again``", "");
								break;
							}
							if ((bet < 10 && lock_ == 242) or (bet > 5 && lock_ == 7188))
							{
								packet_(peer, "action|log\nmsg|`9LIMBO: `oThe minimum bet is `$10 World Locks ``and the maximum bet is `$5 Blue Gem Locks``.", "");
								break;
							}

							{
								int chck = 0, take = 0;
								modify_inventory(peer, lock_, chck);
								if (chck < bet)
								{
									packet_(peer, "action|log\nmsg|Your lock is not sufficient with the bet you placed, if your lock is sufficient, please convert it. Because the system reads your lock according to the type (bgl/wl/dl).", "");
									break;
								}
								else modify_inventory(peer, lock_, take -= bet);
							}

							auto CreateMult = [&](double m_min, double m_max)
								{
									static bool se = 0;
									if (!se) {
										srand(static_cast<unsigned int>(time(nullptr)));
										se = 1;
									}
									double randF = static_cast<double>(rand()) / RAND_MAX;
									double result = m_min + randF * (m_max - m_min);
									return result;
								};
							auto m_muls = [&](double& m_mull, int lockr, int bets)
								{
									uint8_t m_lucky = rand() % 100 + 1;
									if (lockr == 7188 or (lockr == 1796 && bets >= 100)) {
										if (m_lucky == 2)						  m_mull = CreateMult(1000.00f, 1000.00f);
										else if (m_lucky >= 1 and m_lucky <= 3)	  m_mull = CreateMult(900.00f, 1000.00f);
										else if (m_lucky >= 4 and m_lucky <= 5)	  m_mull = CreateMult(500.00f, 800.00f);
										else if (m_lucky >= 6 and m_lucky <= 7)	  m_mull = CreateMult(400.00f, 500.00f);
										else if (m_lucky >= 8 and m_lucky <= 9)   m_mull = CreateMult(250.00f, 400.00f);
										else if (m_lucky >= 10 and m_lucky <= 11) m_mull = CreateMult(100.00f, 250.00f);
										else if (m_lucky >= 12 and m_lucky <= 13) m_mull = CreateMult(50.00f, 250.00f);
										else if (m_lucky >= 14 and m_lucky <= 15) m_mull = CreateMult(20.00f, 50.00f);
										else if (m_lucky >= 16 and m_lucky <= 20) m_mull = CreateMult(10.00f, 20.00f);
										else if (m_lucky >= 21 and m_lucky <= 34) m_mull = CreateMult(5.00f, 10.00f);
										else if (m_lucky >= 35 and m_lucky <= 40) m_mull = CreateMult(3.00f, 5.00f);
										else if (m_lucky > 40 and m_lucky < 92)   m_mull = CreateMult(1.10f, 1.90f);
										else if (m_lucky > 92 and m_lucky <= 95)  m_mull = CreateMult(1.90f, 3.00f);
										else if (m_lucky > 95)					  m_mull = CreateMult(0.00f, 1.10f);
									}
									else if (lockr == 1796 && bets < 100) {
										if (m_lucky == 2)						  m_mull = CreateMult(1000.00f, 1000.00f);
										else if (m_lucky >= 1 and m_lucky <= 3)	  m_mull = CreateMult(900.00f, 1000.00f);
										else if (m_lucky >= 4 and m_lucky <= 5)	  m_mull = CreateMult(500.00f, 800.00f);
										else if (m_lucky >= 6 and m_lucky <= 7)	  m_mull = CreateMult(400.00f, 500.00f);
										else if (m_lucky >= 8 and m_lucky <= 9)   m_mull = CreateMult(250.00f, 400.00f);
										else if (m_lucky >= 10 and m_lucky <= 11) m_mull = CreateMult(100.00f, 250.00f);
										else if (m_lucky >= 12 and m_lucky <= 13) m_mull = CreateMult(50.00f, 250.00f);
										else if (m_lucky >= 14 and m_lucky <= 15) m_mull = CreateMult(20.00f, 50.00f);
										else if (m_lucky >= 16 and m_lucky <= 20) m_mull = CreateMult(10.00f, 20.00f);
										else if (m_lucky >= 21 and m_lucky <= 30) m_mull = CreateMult(5.00f, 10.00f);
										else if (m_lucky >= 30 and m_lucky <= 45) m_mull = CreateMult(3.00f, 5.00f);
										else if (m_lucky > 45 and m_lucky < 90)   m_mull = CreateMult(1.10f, 1.90f);
										else if (m_lucky > 90 and m_lucky <= 95)  m_mull = CreateMult(1.90f, 3.00f);
										else if (m_lucky > 95)					  m_mull = CreateMult(0.00f, 1.10f);
									}
									else if (lockr == 242) {
										if (m_lucky == 2)						  m_mull = CreateMult(1000.00f, 1000.00f);
										else if (m_lucky >= 1 and m_lucky <= 3)	  m_mull = CreateMult(900.00f, 1000.00f);
										else if (m_lucky >= 4 and m_lucky <= 5)	  m_mull = CreateMult(500.00f, 800.00f);
										else if (m_lucky >= 6 and m_lucky <= 7)	  m_mull = CreateMult(400.00f, 500.00f);
										else if (m_lucky >= 8 and m_lucky <= 9)   m_mull = CreateMult(250.00f, 400.00f);
										else if (m_lucky >= 10 and m_lucky <= 11) m_mull = CreateMult(100.00f, 250.00f);
										else if (m_lucky >= 12 and m_lucky <= 13) m_mull = CreateMult(50.00f, 250.00f);
										else if (m_lucky >= 14 and m_lucky <= 15) m_mull = CreateMult(20.00f, 50.00f);
										else if (m_lucky >= 16 and m_lucky <= 20) m_mull = CreateMult(10.00f, 20.00f);
										else if (m_lucky >= 21 and m_lucky <= 30) m_mull = CreateMult(5.00f, 10.00f);
										else if (m_lucky >= 30 and m_lucky <= 50) m_mull = CreateMult(3.00f, 5.00f);
										else if (m_lucky > 50 and m_lucky < 85)   m_mull = CreateMult(1.10f, 1.90f);
										else if (m_lucky > 85 and m_lucky <= 95)  m_mull = CreateMult(1.90f, 3.00f);
										else if (m_lucky > 95)					  m_mull = CreateMult(0.00f, 1.10f);
									}
								};
							auto dropL = [&](int id_, int count_) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldDrop dropeerblock_{};
									dropeerblock_.x = pInfo(peer)->x, dropeerblock_.y = pInfo(peer)->y, dropeerblock_.id = id_, dropeerblock_.count = count_, dropeerblock_.uid = uint16_t(world_->drop.size()) + 1;
									dropas_(world_, dropeerblock_);
								}
								};

							double m_mul = 00.00f;

							m_muls(m_mul, lock_, bet);
							bool win_ = (multiplier <= m_mul ? true : false);
							double mamul = m_mul / 10;

							gamepacket_t p1; ostringstream _r;
							_r << fixed << setprecision(2) << m_mul;

							for (double i = 00.00f; i < m_mul; i += mamul)
							{
								gamepacket_t p; ostringstream r_;
								r_ << fixed << setprecision(2) << i;
								p.Insert("OnTextOverlay");
								p.Insert((win_ == 1 ? "`2x" : "`4x") + r_.str());
								p.CreatePacket(peer);
							}
							gamepacket_t ph;
							ph.Insert("OnTextOverlay");
							ph.Insert((win_ == 1 ? "`2x" : "`4x") + _r.str());
							ph.CreatePacket(peer);

							int give = 0;
							int profit = bet * multiplier;

							cout << profit << endl;

							gamepacket_t p3;
							p3.Insert("OnTalkBubble");
							p3.Insert(pInfo(peer)->netID);
							p3.Insert(win_ == true ? "`2You Won The Game!" : "`4You Lose The Game.");
							p3.Insert(0), p3.Insert(0);
							p3.CreatePacket(peer);

							string locks = (lock_ == 7188 ? "Blue Gem Locks" : (lock_ == 1796 ? "Diamond Locks" : (lock_ == 242 ? "World Locks" : "Locks")));
							string c_msg = (win_ == true ?
								format("Congratulations! You won the game. \n`$Profit: `2+{} `9World Locks````.\n`$House Multiplier: `2x{}\n`$Your Multiplier: `2x{}", profit - bet, _r.str(), multiplier) :
								format("`4Bad Luck``. You lose the game, better luck next time!\n`$Profit: `4-{} `9World Locks````.\n`$House Multiplier: `4{}\n`$Your Multiplier: `4x{}", bet, _r.str(), multiplier));
							gamepacket_t p4;
							p4.Insert("OnConsoleMessage");
							p4.Insert(c_msg);
							p4.CreatePacket(peer);

							int chck = 0;
							if (win_ == 1) {
								{
									modify_inventory(peer, lock_, chck);
									if (chck + profit > 200) {
										while (profit >= 200) {
											dropL(lock_, 200);
											profit -= 200;
										}
										if (profit > 0 && profit < 200) dropL(lock_, profit);
									}
									else modify_inventory(peer, lock_, give = profit);
									packet_(peer, ("action|log\nmsg|`9LIMBO: `oGiven `$" + to_string(profit) + " ``" + locks), "");
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|find_hand") != string::npos) {
							TextScanner parser(cch);
							std::string error_message;
							string find_target = parser.get("search_items", 1);
							if (find_target.empty()) {
								error_message += "Item name must be more than 2 letters!";
								break;
							}

							vector<pair<int, string>> found_items;
							for (int i_ = 0; i_ < items.size(); i_++) {
								uint32_t item_id = items[i_].id;
								if (items[item_id].blockType == SEED) continue;
								if (items[item_id].blockType != CLOTHING) continue;
								if (to_lower(items[i_].ori_name).find(find_target) != string::npos) {
									found_items.push_back(make_pair(items[i_].id, items[i_].ori_name));
								}
							}
							if (found_items.empty()) {
								error_message += "`4Oops: `oThere is no item starting with `w" + find_target + "`o.";
								goto err_find;
								break;
							}
						err_find:
							DialogBuilder db;
							db.add_label_icon(true, 5956, format("`7Found (`1{}``) Item, Search name: \"`2{}``\"", found_items.size(), find_target));
							db.add_smalltext(format("{}", error_message));
							db.add_spacer(false)
								.add_button("find_again", "`5Find Again")
								.add_spacer(false)
								.raw("\ntext_scaling_string|XnxxHentaiiiiiiiiiii|");
							for (int i = 0; i < found_items.size(); i++) {
								auto& item = found_items.at(i);
								db.raw("\ntext_scaling_string||");
								db.add_static_icon_button(std::format("itemid_{}", item.first), item.first, std::format("`1{}``({})", item.second, item.first));

								if (i == found_items.size() - 1)
									db.raw("\nadd_button_with_icon||END_LIST|noflags|0||");
							}
							db.add_spacer(true);
							db.add_label_icon(false, 5956, "`7Click to `4delete`` from, List Items Far-Hand``");
							std::ifstream infile("./database/config/items.json");
							json jsonData;
							infile >> jsonData;
							infile.close();
							if (jsonData.is_array()) {
								for (const auto& obj : jsonData) {
									if (obj.is_object()) {
										if (obj.contains("ItemId") && obj.contains("Fars")) {
											int item_id = obj["ItemId"];
											db.add_static_icon_button(format("delete_{}", item_id), item_id, format("`4Delete``: `7{}``", items[item_id].ori_name));
										}
									}
								}
							}
							db.end_list();
							db.end_dialog("setting_hand", "Cancel", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(db.to_string());
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|quest_dialog") != string::npos) {
							if (cch.find("buttonClicked|start_quest_p") != string::npos) {
								if (pInfo(peer)->step_punch == 0) {

	

								pInfo(peer)->step_punch = 1;
							}
								Algorithm::send_console(peer, "`oDoing your job!");
								quest_p1(peer);
								if (pInfo(peer)->step_punch == 1) {
									quest_p1(peer);
								}
								
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|setting_hand\nbuttonClicked|itemid_") != string::npos) {
							std::uint16_t itemId = atoi(cch.substr(67, cch.length() - 67).c_str());
							if (itemId < 0 or itemId >= items.size()) break;
							DialogBuilder db;
							db.raw("`o\nadd_label_with_ele_icon|big|`bEDITING:`9 " + items[itemId].ori_name + "``|left|" + to_string(itemId) + "|" + to_string(items[itemId].chi) + "|")
								.add_spacer(false);
							db.add_smalltext("!!!!")
								.add_smalltext("Dont Edit This Column!!!!!")
								.add_text_input(5, "itemid_a", "Id :", "" + to_string(itemId))
								.add_spacer(false)
								.add_smalltext("Enter Count The far")
								.add_text_input(3, "fars_a", "Far Punch:", "")
								//.add_text_input(3, "place_amount", "Far Place:", "")
								.add_spacer(false)
								.add_smalltext("Enter The Gems Bonus")
								.add_text_input(3, "gems_a", "Gems (x) :", "")
								.add_spacer(false)
								.add_smalltext("Enter a breakhit count")
								.add_text_input(3, "breakhit_a", "Hit Count :", "")
								.add_spacer(false)
								.add_smalltext("Enter a xpbonus count")
								.add_text_input(3, "exbonus_a", "Exp Count :", "")
								.add_spacer(false);
							//db.embed_data(false, "itemID", format("{}", itemId));
							db.add_spacer(false);
							db.add_button("confirm_added", "Added Edit");
							db.add_spacer(false);
							db.add_textbox("`#** [`7About Item's``]**")
								.add_smalltext(format("`o<CR>[>].Name: `9{}``<CR>[>].ItemID: `7{}``<CR>[>].Rarity: `7{}``<CR>[>].Far Hand: `b{}``X`b{}``<CR><CR>Description: `7{}``", items[itemId].ori_name, items[itemId].id, items[itemId].rarity, items[itemId].place_far, items[itemId].punch_far, items[itemId].description))
								.add_textbox("");
							db.end_dialog("setting_hand", "Close", "");
							enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(db.to_string()).pack());
							//break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|setting_hand\nbuttonClicked|confirm_added") != string::npos) {
							int item_id = atoi(explode("\n", explode("itemid_a|", cch)[1])[0].c_str());
							//std::uint16_t itemId = atoi(cch.substr(67, cch.length() - 67).c_str());
							int maxgem = atoi(explode("\n", explode("gems_a|", cch)[1])[0].c_str());
							int brekhit = atoi(explode("\n", explode("breakhit_a|", cch)[1])[0].c_str());
							int fars = atoi(explode("\n", explode("fars_a|", cch)[1])[0].c_str());
							int exps = atoi(explode("\n", explode("exbonus_a|", cch)[1])[0].c_str());
							std::ifstream inputFile("./database/config/items.json");
							json jsonData;
							if (inputFile.is_open()) {
								inputFile >> jsonData;
								inputFile.close();
							}
							json newItem = {
							   {"ItemId", item_id},
							   {"Gems", maxgem},
							   {"xpbonus", exps},
							   {"Fars", fars},
							   {"BreakHit", brekhit}
							};
							add_items_newjson(peer, newItem, jsonData);
						}
						else if (cch.find("action|dialog_return\ndialog_name|setting_hand\nbuttonClicked|delete_") != string::npos) {
							std::uint16_t item_id = atoi(cch.substr(67, cch.length() - 67).c_str());
							std::ifstream infile("./database/config/items.json");
							json jsonData;
							infile >> jsonData;
							infile.close();

							items.at(item_id).punch_far = 1;
							items.at(item_id).place_far = 1;
							bool found = false;
							for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
								if ((*it)["ItemId"] == item_id) {
									it = jsonData.erase(it);
									found = true;
									break;
								}
							}
							if (found) {
								std::ofstream outfile("./database/config/items.json");
								outfile << std::setw(4) << jsonData << std::endl;
								outfile.close();
								Algorithm::send_overlay(peer, "SUCCESS DELETED FROM CONFIG!");
								break;
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|setting_hand\nbuttonClicked|find_again") != string::npos) {
							SendCmd(peer, "/setitem");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|battle_pvp") != string::npos) {
								try {
									int bet = 0, netID = 0, Opponent = 0;
									bool Found = false;
									if (cch.find("bet|") != string::npos) bet = atoi(explode("\n", explode("bet|", cch)[1])[0].c_str());
									if (cch.find("netID|") != string::npos) netID = atoi(explode("\n", explode("netID|", cch)[1])[0].c_str());
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world or pInfo(currentPeer)->PVP_Enemy != pInfo(peer)->tankIDName) continue;
										Found = true;
										Opponent = pInfo(currentPeer)->PVP_Bet;
										break;
									}
									gamepacket_t p;
									p.Insert("OnTalkBubble"); p.Insert(pInfo(peer)->netID);
									if (bet < 100 or bet > 5000) p.Insert("You can only bet between 100 - 5000 gems!");
									else if (bet > pInfo(peer)->gems) p.Insert("You doesn't have enough gems!");
									else if (Found and bet != Opponent) p.Insert("Your opponent bet is " + setGems(Opponent) + ", you can only bet at the same amount!");
									else if (pInfo(peer)->PVP_netID != -1) p.Insert("You already request a PVP battle with someone!");
									else {
										{
											pInfo(peer)->gems -= bet;
											gamepacket_t p;
											p.Insert("OnSetBux"); p.Insert(pInfo(peer)->gems); p.Insert(0); p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											p.CreatePacket(peer);
										}
										p.Insert("Succesfully asking for battle!");
										pInfo(peer)->PVP_netID = netID; pInfo(peer)->PVP_Bet = bet;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world or pInfo(currentPeer)->netID != pInfo(peer)->PVP_netID) continue;
											pInfo(peer)->PVP_Enemy = pInfo(currentPeer)->tankIDName;
											if (pInfo(peer)->PVP_Enemy == pInfo(currentPeer)->tankIDName and Found) Start_PVP(peer, currentPeer);
											else {
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`2BATTLE PVP``: " + pInfo(peer)->name_color + pInfo(peer)->tankIDName + "`` asking you for a PVP battle for " + setGems(bet) + " gems! Wrench him/her to accept the battle.");
												p.CreatePacket(currentPeer);
												packet_(currentPeer, "action|play_sfx\nfile|audio/siren.wav\ndelayMS|0");
											}
										}
									}
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
								}
								catch (...) {
									break;
								}
								break;
								}
						else if (cch.find("action|dialog_return\ndialog_name|fish_tour") != string::npos) {
							if (pInfo(peer)->lastwrenchb != 9498) break;
							if (cch.find("buttonClicked|register_tour") != string::npos) {
								if (find(tournament_players.begin(), tournament_players.end(),
									(pInfo(peer)->uid)) != tournament_players.end() || has_playmod(pInfo(peer), "tournament mode")) {
									Algorithm::send_console(peer, "You're already a player in tournament.");
									break;
								}
								if (pInfo(peer)->gems < 10000) {
									Algorithm::send_console(peer, "You will need 10,000 gems to join the tournament.");
									break;
								}

								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9498, "Registration confirmation: ")
									.add_textbox("Confirm to register. (The registration costs 10,000 gems and you only have 5 minutes to finish it.)")
									.add_button("confirm_reg", "`wConfirm Registration")
									.end_dialog("fish_tour", "`wCancel``", "").add_quick_exit();

								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|confirm_reg") != string::npos) {
								if (find(tournament_players.begin(), tournament_players.end(),
									(pInfo(peer)->uid)) != tournament_players.end() || has_playmod(pInfo(peer), "tournament mode")) {
									Algorithm::send_console(peer, "You're already a player in tournament.");
									break;
								}
								if (pInfo(peer)->gems < 10000) {
									Algorithm::send_console(peer, "You will need 10,000 gems to join the tournament.");
									break;
								}

								PlayMods new_mod;
								new_mod.id = 126;
								new_mod.time = time(nullptr) + 300;
								new_mod.user = pInfo(peer)->tankIDName;
								pInfo(peer)->playmods.push_back(new_mod);

								Algorithm::send_console(peer, "Your tournament has been started! (Tournament Mode mod added, 5 mins left)");

								packet_(peer, "action|play_sfx\nfile|audio/race_start.wav\ndelayMS|0");
								packet_(peer, "action|play_sfx\nfile|audio/mp3/battle_theme_loop.mp3\ndelayMS|3");

								start_tournament(peer);

								Algorithm::send_console(peer, ("You have 5 minutes to fish."));
								Algorithm::send_console(peer, "`4WARNING: ``Leaving this world will cause you lost your progress instantly.");
								Algorithm::send_console(peer, "`2INFO: ``There might be an extra time for you.");
								pInfo(peer)->gems -= 10000;
								Algorithm::set_bux(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mooncake_reward_dialog") != string::npos) { 
							World* world = GetCurrentWorld(pInfo(peer)->world); 
							if (!world) break; 
							WorldBlock* tile = GetTile({ pInfo(peer)->lastwrenchx, pInfo(peer)->lastwrenchy }, world); 
							if (!tile) break; 
							if (tile->fg != 12598) break; 
							TextScanner parser(cch); 
							string button = ""; 
							if (!parser.try_get("buttonClicked", button)) break; 
							if (button == "reroll") {
								offering_table(peer, tile, "reroll"); 
								break;
							}
							if (button == "take_reward") {
								if (tile->shelf_1 != 0) {
									int c_ = 1; 
									modify_inventory(peer, tile->shelf_1, c_);

									Algorithm::send_console(peer, format("You have received {} {}!", c_, items[tile->shelf_1].name));

									tile->bunny_egg_progress = 0; 
									tile->shelf_1 = 0;  
								}
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mooncake_altar_dialog") != string::npos) {
							World* world = GetCurrentWorld(pInfo(peer)->world);
							if (!world) break; 
							WorldBlock* tile = GetTile({ pInfo(peer)->lastwrenchx, pInfo(peer)->lastwrenchy }, world); 
							if (!tile) break;
							if (tile->fg != 12598) break;
							TextScanner parser(cch);
							string button = "";
							if (!parser.try_get("buttonClicked", button)) break;
							if (button == "reward_list_btn") {
								offering_table(peer, tile, "reward");
								break;
							}
							if (button.find("select_") != string::npos) {
								auto data = explode("_", button);
								if (data.size() != 3) break;
								int item = atoi(data[1].c_str()), index = atoi(data[2].c_str());
								if (item >= items.size() || !items[item].mooncake || index >= 10 || index < 0) break;
								if (get_item_count(pInfo(peer), item) <= 0) break;
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, item, format("`w{}``", items[item].name))
									.embed_data(false, "itemID", to_string(item))
									.embed_data(false, "index", to_string(index))
									.add_textbox("`2How many do you want to offer?``")
									.add_text_input(5, "itemCount", "")
									.add_spacer(false)
									.add_button("cancel", "Cancel")
									.add_button("confirm_select", "OK")
									.end_dialog("mooncake_altar_dialog", "", "");
								
								Algorithm::send_dialog(peer, db.to_string());
								break;
							}
							if (button == "cancel") {
								offering_table(peer, tile, "");
								break;
							}
							if (button == "confirm_select") {
								int stored_ = 0, returned_ = 0, count_ = 0;
								string count_str;
								bool does_exist = false;
								if (!parser.try_get("itemCount", count_str)) break;
								if (!isdigit(count_str[0])) break;
								count_ = atoi(count_str.c_str());
								string item_str = get_embed(cch, "itemID"), index_str = get_embed(cch, "index"); 
								if (item_str.empty() || index_str.empty()) break; 
								int item = atoi(item_str.c_str()), index = atoi(index_str.c_str()); 
								if (item >= items.size() || !items[item].mooncake || index >= 10 || index < 0) break;
								if (get_item_count(pInfo(peer), item) < count_) break; 
								for (int i = 0; i < tile->donates.size(); i++) {
									if (i == index && tile->donates[i].item != 0) {
										stored_ = tile->donates[i].item;
										returned_ = tile->donates[i].count;

										if (stored_ != 0 && returned_ != 0) {
											if (get_item_count(pInfo(peer), stored_) + returned_ > 200) { 
											 	Algorithm::send_console(peer, "Unable to replace the mooncake in the table due to not enough space in your backpack.");
												break;
											}
											modify_inventory(peer, stored_, returned_);
											does_exist = true;
										}

										tile->donates[i].item = item;
										tile->donates[i].count = count_;
										break;
									}
								}

								offering_table(peer, tile, "", does_exist ? 0 : item, does_exist ? 0 : count_); 
								break;
							}
							if (button.find("slot_btn_") != string::npos) { // not called
								//if (button.length() < 10) break; 
								auto data = explode("slot_btn_", button); 
								if (data.size() < 2) break;
								int index = atoi(data[1].c_str()); 
								if (index >= 10 || index < 0) break;
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 12598, "`wSelect a mooncake``");
								for (int i = 0; i < pInfo(peer)->inv.size(); i++) {
									if (items[pInfo(peer)->inv[i].id].mooncake && pInfo(peer)->inv[i].count >= 1) {
										db.add_label_icon_button(false, items[pInfo(peer)->inv[i].id].name, pInfo(peer)->inv[i].id, format("select_{}_{}", pInfo(peer)->inv[i].id, index));
									}
								}
								db.end_list().add_quick_exit().end_dialog("mooncake_altar_dialog", "`wCancel``", "");
								//cout << db.to_string() << endl; 
								Algorithm::send_dialog(peer, db.to_string(), 200);
								break;
							}
							if (button == "offer_btn") {
								offering_table(peer, tile, "offer");
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|growpedia") != string::npos) {
							int got = 0;
							modify_inventory(peer, 6336, got);
							if (got <= 0) break;
							int DailR;
							ifstream IDRiddle("database/Ancestral/IDDailyRiddles.txt");
							IDRiddle >> DailR;
							IDRiddle.close();
							if (cch.find("buttonClicked|rces") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 5084, "`0Red Ancestral")
									.add_spacer(false)
									.add_textbox("`wLevel 1-2``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 2 Celestial Kaleidoscope``")
									.add_textbox("`w--> 1 Crystallized Wealth``")
									.add_spacer(false)
									.add_textbox("`wLevel 2-3``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 2 Plasma Globe``")
									.add_textbox("`w--> 1 Crystallized Wealth``")
									.add_spacer(false)
									.add_textbox("`wLevel 3-4``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 2 Harmonic Chimes``")
									.add_textbox("`w--> 1 Crystallized Wealth``")
									.add_spacer(false)
									.add_textbox("`wLevel 4-5``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 3 Celestial Kaleidoscope``")
									.add_textbox("`w--> 2 Crystallized Wealth``")
									.add_spacer(false)
									.add_textbox("`wLevel 5-6``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 3 Plasma Globe``")
									.add_textbox("`w--> 2 Crystallized Wealth``")
									.add_spacer(false)
									.add_textbox(format("`wDaily Riddles: {}``", items[DailR].name))
									.add_spacer(false)
									.end_dialog("rces", "Close", "");

								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|bces") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 5080, "`0Blue Ancestral")
									.add_spacer(false)
									.add_textbox("`wLevel 1-2``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Plasma Globe``")
									.add_textbox("`w--> 1 Crystallized Reality``")
									.add_spacer(false)
									.add_textbox("`wLevel 2-3``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Celestial Kaleidoscope``")
									.add_textbox("`w--> 1 Crystallized Reality``")
									.add_spacer(false)
									.add_textbox("`wLevel 3-4``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 1 Harmonic Chimes``")
									.add_textbox("`w--> 1 Crystallized Reality``")
									.add_spacer(false)
									.add_textbox("`wLevel 4-5``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 2 Celestial Kaleidoscope``")
									.add_textbox("`w--> 2 Crystallized Reality``")
									.add_spacer(false)
									.add_textbox("`wLevel 5-6``")
									.add_textbox("`w--> 3 Soul Stone``")
									.add_textbox("`w--> 3 Plasma Globe``")
									.add_textbox("`w--> 2 Crystallized Reality``")
									.add_spacer(false)
									.add_textbox(format("`wDaily Riddles: {}``", items[DailR].name))
									.add_spacer(false)
									.end_dialog("bces", "Close", "");
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|gces") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 5162, "`0Green Ancestral")
									.add_spacer(false)
									.add_textbox("`wLevel 1-2``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Harmonic Chimes``")
									.add_textbox("`w--> 1 Crystallized Nature``")
									.add_spacer(false)
									.add_textbox("`wLevel 2-3``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Plasma Globe``")
									.add_textbox("`w--> 1 Crystallized Nature``")
									.add_spacer(false)
									.add_textbox("`wLevel 3-4``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 1 Celestial Kaleidoscope``")
									.add_textbox("`w--> 1 Crystallized Nature``")
									.add_spacer(false)
									.add_textbox("`wLevel 4-5``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 2 Harmonic Chimes``")
									.add_textbox("`w--> 2 Crystallized Nature``")
									.add_spacer(false)
									.add_textbox("`wLevel 5-6``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 3 Celestial Kaleidoscope``")
									.add_textbox("`w--> 2 Crystallized Nature``")
									.add_spacer(false)
									.add_textbox(format("`wDaily Riddles: {}``", items[DailR].name))
									.add_spacer(false)
									.end_dialog("gces", "Close", "");
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|yces") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 5078, "`0Yellow Ancestral")
									.add_spacer(false)
									.add_textbox("`wLevel 1-2``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Celestial Kaleidoscope``")
									.add_textbox("`w--> 1 Crystallized Brilliance``")
									.add_spacer(false)
									.add_textbox("`wLevel 2-3``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Harmonic Chimes``")
									.add_textbox("`w--> 1 Crystallized Brilliance``")
									.add_spacer(false)
									.add_textbox("`wLevel 3-4``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 1 Plasma Globe``")
									.add_textbox("`w--> 1 Crystallized Brilliance``")
									.add_spacer(false)
									.add_textbox("`wLevel 4-5``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 2 Celestial Kaleidoscope``")
									.add_textbox("`w--> 2 Crystallized Brilliance``")
									.add_spacer(false)
									.add_textbox("`wLevel 5-6``")
									.add_textbox("`w--> 3 Soul Stone``")
									.add_textbox("`w--> 3 Harmonic Chimes``")
									.add_textbox("`w--> 2 Crystallized Brilliance``")
									.add_spacer(false)
									.add_textbox(format("`wDaily Riddles: {}``", items[DailR].name))
									.add_spacer(false)
									.end_dialog("yces", "Close", "");
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|pces") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 7166, "`0Purple Ancestral")
									.add_spacer(false)
									.add_textbox("`wLevel 1-2``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 2 Harmonic Chimes``")
									.add_textbox("`w--> 1 Crystallized Time``")
									.add_spacer(false)
									.add_textbox("`wLevel 2-3``")
									.add_textbox("`w--> 1 Soul Stone``")
									.add_textbox("`w--> 1 Celestial Kaleidoscope``")
									.add_textbox("`w--> 1 Crystallized Time``")
									.add_spacer(false)
									.add_textbox("`wLevel 3-4``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 1 Plasma Globe``")
									.add_textbox("`w--> 1 Crystallized Time``")
									.add_spacer(false)
									.add_textbox("`wLevel 4-5``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 2 Harmonic Chimes``")
									.add_textbox("`w--> 2 Crystallized Time``")
									.add_spacer(false)
									.add_textbox("`wLevel 5-6``")
									.add_textbox("`w--> 2 Soul Stone``")
									.add_textbox("`w--> 3 Celestial Kaleidoscope``")
									.add_textbox("`w--> 2 Crystallized Time``")
									.add_spacer(false)
									.add_textbox(format("`wDaily Riddles: {}``", items[DailR].name))
									.add_spacer(false)
									.end_dialog("pces", "Close", "");
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							else if (cch.find("buttonClicked|lnegros") != string::npos) {
								DialogBuilder db("`o");
								db.add_label_icon(true, 9158, "`Obtain Ingredient")
									.add_spacer(false)
									.add_textbox("`wChance From Break :``")
									.add_textbox("`w1.Old Wheel``")
									.add_textbox("`w2.Bottle Shard``")
									.add_spacer(false)
									.add_textbox("`wChance Drop From Geiger :``")
									.add_textbox("`w1.Dirty Boots``")
									.add_textbox("`w2.Lost Scuba Glasses``")
									.add_spacer(false)
									.add_textbox("`wChance Drop From Fishing :``")
									.add_spacer(false)
									.add_textbox("`wFrom Uranium Water``")
									.add_textbox("`w--> Anchor``")
									.add_spacer(false)
									.add_textbox("`wFrom Ice Water``")
									.add_textbox("`w--> Empty Can``")
									.add_spacer(false)
									.add_textbox("`wFrom All Water``")
									.add_textbox("`w--> Wasted Turbine``")
									.add_textbox("`w--> Trash Bag``")
									.add_spacer(false)
									.add_textbox("`wFrom Normal Water``")
									.add_textbox("`w--> Starfish``")
									.add_textbox("`w--> Diamond Ring")
									.add_spacer(false)
									.end_dialog("lnegros", "Close", "");

								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							}
							break;
						}
						/*
						else if (cch.find("action|dialog_return\ndialog_name|socialportal\nbuttonClicked|guess_game\n") != string::npos) {
							DialogBuilder db{ "`o" };
							db.add_label_icon(true, 9154, "`0TayoPs Gamble Game")
								.add_textbox("Tired of losing against real players in your gamble game? "
									"This game might be the best choice for you, it offers great wealth with easy rules.")
								.add_spacer(false)
								.add_textbox("Read these rules before play: ")
								.add_textbox("1. Enter the amount of the gems you'd like to bet. (Payment currently only with gems)")
								.add_textbox("2. Enter the number that's lucky for you. (Only 1-10)")
								.add_textbox("3. If the number equals to generated number, you win x10 of your bet. Otherwise, you lose your bet.")
								.add_textbox("4. Finally, click the 'Start' Button.")
								.add_spacer(false)
								.add_textbox(format("You currently have {} gems.", setGems(pInfo(peer)->gems)))
								.add_text_input(3, "lucky_number", "Your Lucky Number: ")
								.add_text_input(120, "guess_bet", "Your Bet: ")
								.end_dialog("guess_game_confirm", "`wNevermind``", "`wStart``")
								;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(db.to_string());
							p.CreatePacket(peer);
						}
						*/
						else if (cch.find("action|dialog_return\ndialog_name|guide_book") != string::npos) {
							if (cch.find("buttonClicked|splicing") != string::npos) {
								show_recipe(peer, splicing);
								pInfo(peer)->page_item = "", pInfo(peer)->page_number = 26;
								break;
							}
							else if (cch.find("buttonClicked|combining") != string::npos) {
								show_recipe(peer, combining);
								pInfo(peer)->page_item = "", pInfo(peer)->page_number = 26;
								break;
							}
							else if (cch.find("buttonClicked|combusting") != string::npos) {
								show_recipe(peer, combusting);
								pInfo(peer)->page_item = "", pInfo(peer)->page_number = 26;
								break;
							}
							else if (cch.find("buttonClicked|crystals") != string::npos) {
								show_recipe(peer, crystals);
								pInfo(peer)->page_item = "", pInfo(peer)->page_number = 26;
								break;
							}
							else if (cch.find("buttonClicked|search") != string::npos) {
								//pInfo(peer)->page_item = explode("\n", explode("query|", cch)[1])[0];
								TextScanner parser(cch);
								string dialog = "", page = ""; 
								if (!parser.try_get("query", page)) break;
								replace(page, "\n", "");
								pInfo(peer)->page_item = page;
								if (!parser.try_get("dialog_name", dialog)) break;
								switch (Algorithm::quick_hash(dialog)) {
								case "guide_book_s"_qh: show_recipe(peer, splicing); break; 
								case "guide_book_c"_qh: show_recipe(peer, combining); break;
								case "guide_book_f"_qh: show_recipe(peer, combusting); break;
								case "guide_book_r"_qh: show_recipe(peer, crystals); break;
								default: break;
								}
								//if (explode("\n", explode("dialog_name|", cch)[1])[0] == "guide_book_s") show_recipe(peer, splicing);
								//else if (explode("\n", explode("dialog_name|", cch)[1])[0] == "guide_book_c") show_recipe(peer, combining);
							//else if (explode("\n", explode("dialog_name|", cch)[1])[0] == "guide_book_f") show_recipe(peer, combusting);
								//else if (explode("\n", explode("dialog_name|", cch)[1])[0] == "guide_book_r") show_recipe(peer, crystals);
								break;
							}
							else if (cch.find("buttonClicked|next_pg") != string::npos) {
								pInfo(peer)->page_number += 26;
								TextScanner parser(cch);
								string dialog = "", page = "";
								if (!parser.try_get("query", page)) break;
								replace(page, "\n", "");
								pInfo(peer)->page_item = page;
								if (!parser.try_get("dialog_name", dialog)) break;
								switch (Algorithm::quick_hash(dialog)) {
								case "guide_book_s"_qh: show_recipe(peer, splicing); break;
								case "guide_book_c"_qh: show_recipe(peer, combining); break;
								case "guide_book_f"_qh: show_recipe(peer, combusting); break;
								case "guide_book_r"_qh: show_recipe(peer, crystals); break;
								default: break;
								}
								break;
							}
							else if (cch.find("buttonClicked|last_pg") != string::npos) {
								pInfo(peer)->page_number -= 26;
								TextScanner parser(cch);
								string dialog = "", page = "";
								if (!parser.try_get("query", page)) break;
								replace(page, "\n", "");
								pInfo(peer)->page_item = page;
								if (!parser.try_get("dialog_name", dialog)) break;
								switch (Algorithm::quick_hash(dialog)) {
								case "guide_book_s"_qh: show_recipe(peer, splicing); break;
								case "guide_book_c"_qh: show_recipe(peer, combining); break;
								case "guide_book_f"_qh: show_recipe(peer, combusting); break;
								case "guide_book_r"_qh: show_recipe(peer, crystals); break;
								default: break;
								}
								break;
							}
							else if (cch.find("buttonClicked|rules") != string::npos) {
								SendCmd(peer, "/rules", false);
								break;
							}
							else if (cch.find("buttonClicked|news") != string::npos) {
								SendCmd(peer, "/news", false);
								break;
							}
							pInfo(peer)->page_item = "", pInfo(peer)->page_number = 26;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_quick_exit\n"
								"add_label_with_icon|big|`wGuidebook|left|6336|\nadd_spacer|small|\n"
								"set_custom_spacing|x:5;y:10|\n"
								"add_custom_button|splicing|image:interface/large/gui_splicing.rttex;image_size:495,170;width:0.4;|\n"
								"add_custom_button|combining|image:interface/large/gui_combining.rttex;image_size:495,170;width:0.4;|\n"
								"add_custom_break|\n"
								"add_custom_button|crystals|image:interface/large/gui_crystals.rttex;image_size:495,170;width:0.4;|\n"
								"set_custom_spacing|x:0;y:0|\n"
								"add_custom_break|\n"
								"add_spacer|small|\n"
								"set_custom_spacing|x:5;y:10|\n"
								"add_custom_button|news|image:interface/large/gui_news.rttex;image_size:495,170;width:0.4;|\n"
								"add_custom_button|rules|image:interface/large/gui_rules.rttex;image_size:495,170;width:0.4;|\n"
								"set_custom_spacing|x:0;y:0|\n"
								"add_custom_break|\n"
								"end_dialog|guide_book|Close||");
							p.CreatePacket(peer);
							break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|sell_fish_") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|back") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|price_list") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|sell_all") != string::npos) {
								//TextScanner parser(cch);
								if (pInfo(peer)->lastwrenchb != 9156) break;
								if (cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|sell_fish_") != string::npos)
								{
									auto data = atoi(explode("sell_fish_", cch)[1].c_str());
									if (data == NULL || data > items.size() || data <= 0 || items[data].fish_max_lb == 0 ||
										items[data].mlpr == 0 || items[data].ppl == 0 || items[data].blockType != FISH) break;

									int got = 0;
									modify_inventory(peer, data, got);
									if (got <= 0) break;

									int c_ = got * -1;

									if (got >= items[data].fish_max_lb) {
										if (modify_inventory(peer, data, c_) == 0) {
											pInfo(peer)->gems += items[data].mlpr;
											Algorithm::send_console(peer, format("Successfully sold {} lbs of {} for {} gems!", got, items[data].name, setGems(items[data].mlpr)));
										}
									}
									else {
										if (modify_inventory(peer, data, c_) == 0) {
											pInfo(peer)->gems += (items[data].ppl * got);
											Algorithm::send_console(peer, format("Successfully sold {} lbs of {} for {} gems!", got, items[data].name, setGems(items[data].ppl * got)));
										}
									}

									gamepacket_t p2;
									p2.Insert("OnSetBux"), p2.Insert(pInfo(peer)->gems), p2.Insert(0), p2.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) p2.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									p2.CreatePacket(peer);
								}
								else if (cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|price_list") != string::npos) {
									DialogBuilder db{ "`o" };
									db.add_label_icon(true, 9156, "`wPrice List: ");
									for (int i = 0; i < fishprices.size(); i++)
										db.add_label_icon(false, fishprices[i][0], format("{}'s price: (1-{} lb): {} per lb, {} lb = {} gems.", items[fishprices[i][0]].name, fishprices[i][1] - 1, fishprices[i][2], fishprices[i][1], setGems(fishprices[i][3])));

									db.add_spacer(false)
										.add_button("back", "`wBack``")
										.end_dialog("fish_barrel", "`wClose``", "");
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert(db.to_string());
									p.CreatePacket(peer);
								}
								else if (cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|sell_all") != string::npos) {
									int total_amount = 0;
									int size = 0;
									vector<int> lists;
									for (int i = 0; i < pInfo(peer)->inv.size(); i++) {
										if (items[pInfo(peer)->inv[i].id].blockType == FISH and items[pInfo(peer)->inv[i].id].mlpr != 0
											and items[pInfo(peer)->inv[i].id].ppl != 0) {
											lists.push_back(pInfo(peer)->inv[i].id);
											//cout << "pushed " << pInfo(peer)->inv[i].id << endl;
										}
									}
									int got = 0;
									for (int i = 0; i < lists.size(); i++) {
										modify_inventory(peer, lists[i], got);
										if (got <= 0) break;

										int c_ = got * -1;
										int price = got >= items[lists[i]].fish_max_lb ? items[lists[i]].mlpr : (items[lists[i]].ppl * got);

										//cout << "data: " << lists[i] << "|" << price << endl;

										if (modify_inventory(peer, lists[i], c_) == 0) {
											total_amount += price;
											size++;
										}
										got = 0;
									}
									pInfo(peer)->gems += total_amount;

									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert(format("You've sold {} fishes and earned {} gems!", setGems(size), setGems(total_amount)));
									p.CreatePacket(peer);

									gamepacket_t p2;
									p2.Insert("OnSetBux"), p2.Insert(pInfo(peer)->gems), p2.Insert(0), p2.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) p2.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									p2.CreatePacket(peer);
								}

								//Growland gacha /machine
								else if (cch.find("action|dialog_return\ndialog_name|gemsgrowlandgacha1") != string::npos) {
									int x_ = 0;
									int y_ = 0;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (paa != worlds.end()) {
										World* world_ = &worlds[paa - worlds.begin()];
										int gems = 0;
										bool luck = true;
										int c_ = 1;
										vector<int>rand_prize, rand_prize2, rand_prize3;
										if (rand() % 1000 < 99) rand_prize = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464 }, luck = true;
										else if (rand() % 300 < 35) rand_prize2 = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 946411308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 };
										else rand_prize3 = {
					1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464,11308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 }, c_ = 1;
										WorldDrop drop_block_{};
										drop_block_.id = 611, drop_block_.count = rand() % 20 + 20, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
										dropas_(world_, drop_block_);

										if (gems == 100000) {
											rand() % 100 < 25;
											gems = 100000;
											gems = drop_block_.id = rand_prize[rand() % rand_prize.size()], drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
											dropas_(world_, drop_block_);
										}
									}
								}
								else if (cch.find("action|dialog_return\ndialog_name|gemsgrowlandgacha2") != string::npos) {
									int x_ = 0;
									int y_ = 0;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (paa != worlds.end()) {
										World* world_ = &worlds[paa - worlds.begin()];
										int gems = 0;
										bool luck = true;
										int c_ = 1;
										vector<int>rand_prize, rand_prize2, rand_prize3;
										if (rand() % 1000 < 99) rand_prize = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464 }, luck = true;
										else if (rand() % 300 < 35) rand_prize2 = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 946411308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 };
										else rand_prize3 = {
					1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464,11308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 }, c_ = 1;
										WorldDrop drop_block_{};
										drop_block_.id = 611, drop_block_.count = rand() % 20 + 20, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
										dropas_(world_, drop_block_);

										if (gems == 350000) {
											rand() % 100 < 50;
											gems = 350000;
											gems = drop_block_.id = rand_prize2[rand() % rand_prize2.size()], drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
											dropas_(world_, drop_block_);
										}
									}
								}
								else if (cch.find("action|dialog_return\ndialog_name|gemsgrowlandgacha3") != string::npos) {
									int x_ = 0;
									int y_ = 0;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (paa != worlds.end()) {
										World* world_ = &worlds[paa - worlds.begin()];
										int gems = 0;
										bool luck = true;
										int c_ = 1;
										vector<int>rand_prize, rand_prize2, rand_prize3;
										if (rand() % 1000 < 99) rand_prize = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464 }, luck = true;
										else if (rand() % 300 < 35) rand_prize2 = { 1796, 1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 946411308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 };
										else rand_prize3 = {
					1738, 10184, 10338, 10424, 10772, 10806, 11016, 11290, 13198, 1342, 13576, 5738, 7084, 2982, 362, 2256, 2257, 2260, 2262, 2264, 2390, 818, 678, 3862, 1672, 1824, 3986, 4534, 3512, 6818, 6842, 7104, 7150, 7196, 7502, 6694, 6334, 7412, 7582, 10440, 11172, 11226, 11228, 11174, 11176, 11178, 11222, 11224, 11340, 11488, 11576, 11570, 11726, 11736, 12062, 12070, 12088, 12112, 12124, 12296, 12260,12492, 12494, 12496, 12498, 12500, 12502, 12504, 12506, 12508, 12510, 2718, 2872, 2972, 4182, 4816, 4182, 4818, 4954, 5266, 5482, 6006, 6008, 6814, 6816,  7092, 7152, 7514, 8480, 8814, 9262, 10082, 10084, 10086, 9334, 9462, 9464,11308, 11442, 11508, 11552, 11662, 11706, 11752, 12644, 12836, 12874, 12876, 13460, 13718, 12434, 12346, 12248, 13260, 13024, 13108, 13260 }, c_ = 1;
										WorldDrop drop_block_{};
										drop_block_.id = 611, drop_block_.count = rand() % 20 + 20, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
										dropas_(world_, drop_block_);

										if (gems == 850000) {
											rand() % 100 < 90;
											gems = 850000;
											gems = drop_block_.id = rand_prize3[rand() % rand_prize3.size()], drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (x_ * 32) + rand() % 17, drop_block_.y = (y_ * 32) + rand() % 17;
											dropas_(world_, drop_block_);
										}
									}
								}
							else if (cch.find("action|dialog_return\ndialog_name|buttongrowlandgacha1") != string::npos) {
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9154, "`wWelcome To Gacha Level 1 :)")
								.add_smalltext("Give Me 100k Your Gems");

								db.add_text_input(1796, "gemsgrowlandgacha1", "")
								.add_button("gemsgrowlandgacha1", "Confirm");
								
								db.add_spacer(false);

								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(db.to_string());
								p.CreatePacket(peer);
							}
							else if (cch.find("action|dialog_return\ndialog_name|buttongrowlandgacha2") != string::npos) {
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9154, "`wWelcome To Gacha Level 2 :) ")
								 .add_smalltext("Give Me 350k Your Gems");

								db.add_text_input(1796, "gemsgrowlandgacha2", "")
								.add_button("gemsgrowlandgacha2", "Confirm");
								db.add_spacer(false);

								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(db.to_string());
								p.CreatePacket(peer);
							}
							else if (cch.find("action|dialog_return\ndialog_name|buttongrowlandgacha3") != string::npos) {
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9154, "`wWelcome To Gacha Level 3 :) ")
								.add_smalltext("Give Me 850k Your Gems");

								db.add_text_input(1796, "gemsgrowlandgacha3", "")
								.add_button("gemsgrowlandgacha3", "Confirm");

								db.add_spacer(false);

								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(db.to_string());
								p.CreatePacket(peer);
							}
							
							else if (cch.find("action|dialog_return\ndialog_name|fish_barrel\nbuttonClicked|back") != string::npos) {
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9156, format("`w{}``", items[9156].name)); {
									db.add_textbox(format("Hello, `w{}``, this is the new way of profit, but before you start, you should follow these rules:", pInfo(peer)->tankIDName))
										.add_textbox("1. Normal fishes can be sold for 1-3 gems / lb. But for perfect one it'll be on different price.")
										.add_textbox("2. Each fishes have different prices (even for perfect and price per lb.)")
										.add_textbox("3. Nothing more i guess, you can just trade your fish. :D")
										.add_textbox("You may see the price list first before trading: ")
										.add_button("price_list", "Fish Prices List")
										.add_textbox("Let's see what do you have: ")
										.add_spacer(false);

									for (int i = 0; i < pInfo(peer)->inv.size(); i++) {
										if (items[pInfo(peer)->inv[i].id].blockType == FISH and items[pInfo(peer)->inv[i].id].mlpr != 0
											and items[pInfo(peer)->inv[i].id].ppl != 0) {
											db.add_button(format("sell_fish_{}", pInfo(peer)->inv[i].id), format("Sell {} ({} lb) for {} gems.", items[pInfo(peer)->inv[i].id].name, pInfo(peer)->inv[i].count, setGems(pInfo(peer)->inv[i].count >= items[pInfo(peer)->inv[i].id].fish_max_lb ? items[pInfo(peer)->inv[i].id].mlpr : pInfo(peer)->inv[i].count * items[pInfo(peer)->inv[i].id].ppl)));
										}
									}

									db.end_dialog("fish_barrel", "`wClose``", "");
								}
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(db.to_string());
								p.CreatePacket(peer);
							}
                        }
						/*
						else if (cch.find("action|dialog_return\ndialog_name|guess_game_confirm") != string::npos) {
						TextScanner parser(cch);
						int num = atoi(parser.get("lucky_number", 1).c_str());
						int bet = atoi(parser.get("guess_bet", 1).c_str());

						if (num > 10 || bet > INT_MAX || num <= 0) {
							packet_(peer, "action|log\nmsg|Limit of number: 1-10, limit of bet: 1-2147483647.");
							break;
						}

						if (bet > pInfo(peer)->gems) {
							packet_(peer, "action|log\nmsg|You don't have enough gems to do it.");
							break;
						}

						int server_number = Algorithm::rand(1, 10);

						DialogBuilder db{ "`o" };
						db.add_label_icon(true, 9154, "`0TayoPs Gamble Game")
							.add_textbox("Gamble Result: ")
							.add_spacer(false);
					      	 if (num == server_number) 
							 {
								 int fixWin = (bet * 10);
								 pInfo(peer)->gems += fixWin;
								 db.add_textbox("Status: `2WIN!``")
									 .add_textbox(format("Server generated number: {}.", server_number))
									 .add_textbox(format("Your lucky number: {}.", num))
									 .add_textbox(format("You won: {} gems.", setGems(fixWin)));
							 }
							 else 
							 {
								 pInfo(peer)->gems -= bet;
								 db.add_textbox("Status: `4LOSE!``")
									 .add_textbox(format("Server generated number: {}.", server_number))
									 .add_textbox(format("Your lucky number: {}.", num))
									 .add_textbox(format("You lost: {} gems.", setGems(bet)));
							 }
							 db.add_spacer(false)
								 .add_textbox(format("You currently have {} gems.", setGems(pInfo(peer)->gems)))
								 .add_button("guess_game", "`wStart a new game``")
							     .end_dialog("socialportal", "`wClose``", "")
							;
							 gamepacket_t p2;
						p2.Insert("OnSetBux"), p2.Insert(pInfo(peer)->gems), p2.Insert(0), p2.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
					    if (pInfo(peer)->supp >= 2) p2.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
						p2.CreatePacket(peer);
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert(db.to_string());
						p.CreatePacket(peer);
						}
						*/
						else if (cch == "action|redeem_open\n") {
							DialogBuilder db;
							db.add_label_icon(true, 1050, "Redeem a code").add_text_input(120, "r_code", "Input your code:", "MTPS-")
								.end_dialog("redeem_dialog", "`wClose``", "`wRedeem``").add_quick_exit();

							gamepacket_t p(500);
							p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|casino_gacha\n") {
							//cout << "I'm called.\n";
							// nothing wrongs at called state since its called
							DialogBuilder db{ "`o" };
							db.add_label_icon(true, 758, "`wCasino Gacha``")
								.add_spacer(false)
								.add_textbox("Coin statistic (Bought/Obtained): ")
								.add_label_icon(false, 8470, format("{}: {}/10,000", items[8470].name, setGems(g_obtained_coin1)))
								.add_label_icon(false, 7188, format("{}: {}/5,000", items[7188].name, g_obtained_coin5))
								.add_label_icon(false, 1796, format("{}: {}/2,000", items[1796].name, g_obtained_coin10))
								.add_spacer(false)
								.add_textbox("You'll get one of these items: ")
								.add_center_button("", "10,000", 8470)
								.add_center_button("", "5,000", 7188)
								.add_center_button("", "2,000", 1796)
								.add_center_button("", "Nothing", 1398)
								.end_list();

								if (g_obtained_coin1 >= 2000 && g_obtained_coin5 >= 500 && g_obtained_coin10 >= 500) {
									db.add_disabled_button("", "Buy a Gacha for 1 Blue Gem Lock");
								}
								else
								{
									db.add_friend_image_label_button("gas_gacha", "Buy a Gacha for 1 Blue Gem Lock", "game/tiles_page12.rttex", 1.6, { 8, 4 });
								}
							db.add_friend_image_label_button("buy_coin", "Buy a coin with TayoPoint", "game/mtokenn.rttex", 1.6, { 0, 0 })
								.add_quick_exit()
								.end_dialog("get_gacha", "`wClose``", "");
							//cout << db.to_string() << endl;
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert(db.to_string());
							p.CreatePacket(peer);
							//break;
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|gas_gacha") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|return_back") != string::npos || 
							cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|buy_coin") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|coin_") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|confirm_coin_") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|return_back") != string::npos)
							{
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 7188, "`wCasino Gacha``")
									.add_spacer(false)
									.add_textbox("Coin statistic (Bought/Obtained): ")
									.add_label_icon(false, 8470, format("{}: {}/10,000", items[8470].name, setGems(g_obtained_coin1)))
									.add_label_icon(false, 7188, format("{}: {}/5,000", items[7188].name, g_obtained_coin5))
									.add_label_icon(false, 1796, format("{}: {}/2,000", items[1796].name, g_obtained_coin10))
								    .add_spacer(false)
									.add_textbox("You'll get one of these items: ")
									.add_center_button("", "10,000", 8470)
									.add_center_button("", "5,000", 7188)
									.add_center_button("", "2,000", 1796)
									.add_center_button("", "Nothing", 1398)
									.end_list();

									//..add_button("gas_gacha", "Buy a Gacha for 1 Blue Gem Lock")
								if (g_obtained_coin1 >= 2000 && g_obtained_coin5 >= 500 && g_obtained_coin10 >= 500) {
									db.add_disabled_button("", "Buy a Gacha for 1 Blue Gem Lock");
								}
								else
								{
									db.add_friend_image_label_button("gas_gacha", "Buy a Gacha for 1 Blue Gem Lock", "game/tiles_page12.rttex", 1.6, { 8, 4 });
								}
								db.add_friend_image_label_button("buy_coin", "Buy a coin with TayoPoint", "game/mtokenn.rttex", 1.6, { 0, 0 })
								    .add_quick_exit()
									.end_dialog("get_gacha", "`wClose``", "");
								//cout << db.to_string() << endl;
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(db.to_string());
								p.CreatePacket(peer);
								break;
							}
							if (cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|coin_") != string::npos) {
								int item_ = atoi(explode("coin_", cch)[1].c_str());
								if (item_ != 5190 && item_ != 5192 && item_ != 5194) break;
								if (item_ == 5190 && g_obtained_coin1 >= 2000) break;
								if (item_ == 5192 && g_obtained_coin5 >= 500) break;
								if (item_ == 5194 && g_obtained_coin10 >= 500) break;

								int price = 0;
								string stock_bought = "0/2,000";

								if (item_ == 5190) price = 10, stock_bought = format("{}/2,000", setGems(g_obtained_coin1));
								if (item_ == 5192) price = 50, stock_bought = format("{}/500", setGems(g_obtained_coin5));
								if (item_ == 5194) price = 100, stock_bought = format("{}/500", setGems(g_obtained_coin10));

								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item_].name + "``|left|" + to_string(items[item_].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + setGems(price) + " TayoPoint``|left|\nadd_smalltext|Duration: `7Until the limit exceeded.``|left|\nadd_smalltext|Stock: `7" + stock_bought + "``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|None.|left|\nadd_spacer|\nadd_button|confirm_coin_" + to_string(item_) + "|`0Purchase for `1" + setGems(price) + " TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|return_back|Close|noflags|0|0|\nnend_dialog|||OK|");
								p.CreatePacket(peer);
							}
							if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|confirm_coin_") != string::npos) {
								int item_ = atoi(explode("confirm_coin_", cch)[1].c_str());
								if (item_ != 5190 && item_ != 5192 && item_ != 5194) break;
								if (item_ == 5190 && g_obtained_coin1 >= 2000) break;
								if (item_ == 5192 && g_obtained_coin5 >= 500) break;
								if (item_ == 5194 && g_obtained_coin10 >= 500) break;

								int price = 0, c_ = 1;

								if (item_ == 5190) price = 10;
								if (item_ == 5192) price = 50;
								if (item_ == 5194) price = 100;

								if (pInfo(peer)->gtwl < price) {
									Algorithm::send_console(peer, format("You have {} TayoPoint short.", setGems(price - pInfo(peer)->gtwl)));
									break;
								}

								if (get_free_slots(pInfo(peer)) >= 1) {
									if (get_item_count(pInfo(peer), item_) < 200) {
										if (item_ == 5190) g_obtained_coin1++;
										if (item_ == 5192) g_obtained_coin5++;
										if (item_ == 5194) g_obtained_coin10++;
										modify_inventory(peer, item_, c_);
										pInfo(peer)->gtwl -= price;
										Algorithm::send_console(peer, format("You have purchased {} for {} TayoPoint. You have {} left.", items[item_].name, price, setGems(pInfo(peer)->gtwl)));
										save_purchase();
									}
									else {
										Algorithm::send_console(peer, "You don't have any space in your backpack.");
										break;
									}
								}
								else {
									Algorithm::send_console(peer, "You don't have any space in your backpack.");
									break;
								}
								break;
							}
							if (cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|buy_coin") != string::npos) {
								DialogBuilder db{ "`o" };
								db.add_label_icon(true, 9784, "Purchase Coins")
									.add_spacer(false)
									.add_textbox("Coin statistic (Bought/Obtained): ")
									.add_label_icon(false, 5190, format("{}: {}/2,000", items[5190].name, setGems(g_obtained_coin1)))
									.add_label_icon(false, 5192, format("{}: {}/500", items[5192].name, g_obtained_coin5))
									.add_label_icon(false, 5194, format("{}: {}/500", items[5194].name, g_obtained_coin10))
									.add_textbox("Available items: ");

									if (g_obtained_coin1 >= 2000) {
										db.add_disabled_button("", "10", 5190);
									}
									else {
										db.add_center_button("coin_5190", "10", 5190);
									}
									if (g_obtained_coin5 >= 500) {
										db.add_disabled_button("", "50", 5192);
									}
									else {
										db.add_center_button("coin_5192", "50", 5192);
									}
									if (g_obtained_coin10 >= 2000) {
										db.add_disabled_button("", "100", 5194);
									}
									else {
										db.add_center_button("coin_5194", "100", 5194);
									}
									db.end_list().add_button("return_back", "`wBack``").add_quick_exit()
										.end_dialog("get_gacha", "", "");

									gamepacket_t p;
									p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
								break;
							}
							if (cch.find("action|dialog_return\ndialog_name|get_gacha\nbuttonClicked|gas_gacha") != string::npos) {
								if (g_obtained_coin1 >= 2000 && g_obtained_coin5 >= 500 && g_obtained_coin10 >= 500) break;
								int c_ = 1, c_bgl = 0, remove = -1, item_ = 0;
								vector<int> random_items{0};
								modify_inventory(peer, 7188, c_bgl);

								if (rand() % 100 <= 45) random_items = { 5190, 5192, 5194 };
								item_ = random_items.at(rand() % random_items.size());

								if (item_ == 5190 && g_obtained_coin1 >= 2000 &&
									item_ == 5192 && g_obtained_coin5 >= 500 &&
									item_ == 5194 && g_obtained_coin10 >= 500)
									item_ = 0;

								if (item_ == 5190 && g_obtained_coin1 >= 2000)
									item_ = random_items.at(rand() % random_items.size());
							    if (item_ == 5192 && g_obtained_coin5 >= 500)
									item_ = random_items.at(rand() % random_items.size());
								if (item_ == 5194 && g_obtained_coin10 >= 500)
									item_ = random_items.at(rand() % random_items.size());

								string name = item_ == 0 ? "nothing" : items[item_].name;

								if (c_bgl > 0) {
									string message = "";
									if (get_free_slots(pInfo(peer)) >= 1) {
										if (get_item_count(pInfo(peer), item_) < 200) {
											if (item_ == 5190) g_obtained_coin1++;
											if (item_ == 5192) g_obtained_coin5++;
											if (item_ == 5194) g_obtained_coin10++;
											modify_inventory(peer, item_, c_);
											modify_inventory(peer, 7188, remove);
											message = format("You spent a Blue Gem Lock and got {}{}.", item_ != 0 ? "a " : "", name);
											save_purchase();
										}
										else {
											message = "You don't have enough space in your backpack.";
										}
										
										DialogBuilder db{ "`o" };
										db.add_label_icon(true, 7188, "`wCasino Coin Gacha``")
											.add_spacer(false)
											.add_textbox(message)
											.add_button("return_back", "`wReturn``")
											.end_dialog("get_gacha", "`wClose``", "");

										gamepacket_t p;
										p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
									}
									else {
										DialogBuilder db{ "`o" };
										db.add_label_icon(true, 7188, "`wCasino Coin Gacha``")
											.add_spacer(false)
											.add_textbox("You don't have a space on your room to do this.")
											.add_button("return_back", "`wReturn``")
											.end_dialog("get_gacha", "`wClose``", "");

										gamepacket_t p;
										p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
										break;
									}
								}
								else {
									DialogBuilder db{ "`o" };
									db.add_label_icon(true, 7188, "`wCasino Coin Gacha``")
										.add_spacer(false)
										.add_textbox("You don't have a Blue Gem Lock to do this.")
										.add_button("return_back", "`wReturn``")
										.end_dialog("get_gacha", "`wClose``", "");

									gamepacket_t p;
									p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
									break;
								}
								break;
							}
							break;
						}
						else if (cch == "action|claimprogressbar\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n" || cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n" || cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n" || 
						cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n" || cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n") {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprank\n\n")
							{
								shoprank_show(peer);
							}
							
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopitems\n\n")
							{
								//gamepacket_t py, pz;
								//py.Insert("HidePearlCurrency"), py.CreatePacket(peer);
								//pz.Insert("OnSetPearl"), pz.Insert(0), pz.Insert(0), pz.CreatePacket(peer); // undefined param
								int Common = 0, Unique = 0, Special = 0;
								OldDialogBuilder dialog;
								dialog.set_default_color("o")
									->add_icon_label("`9Premium Items``", 752, LABEL_BIG)
									->add_textbox("Welcome to `9Premium Items Shop``! Do you have any Premium WLS? You can buy items here with them.")
									->add_smalltext("`#You can earn a Premium WL by depositing wl in real growtopia world that's listed on /shop.``")
									->add_spacer(SPACER_SMALL)
									->add_textbox(format("You have {} Premium WlS(s).", setGems(pInfo(peer)->gtwl)))
									->add_spacer(SPACER_SMALL)
									->add_textbox("`1Common Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : shopitems) {
									if (credit_it.type == COMMON_ITEM)
									{
										dialog.add_button_with_icon_frame(format("shop_price_{}", credit_it.id), credit_it.id, credit_it.price, "staticBlueFrame");
										if (Common > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Common = 0;
										}
										else Common++;
									}
								}
								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_textbox("`#Unique Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : shopitems) {
									if (credit_it.type == UNIQUE_ITEM)
									{
										dialog.add_button_with_icon_frame(format("shop_price_{}", credit_it.id), credit_it.id, credit_it.price, "staticPurpleFrame");
										if (Unique > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Unique = 0;
										}
										else Unique++;
									}
								}

								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_textbox("`9Special Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : shopitems) {
									if (credit_it.type == SPECIAL_ITEM)
									{
										dialog.add_button_with_icon_frame("shop_price_" + to_string(credit_it.id), credit_it.id, credit_it.price, "staticYellowFrame");
										if (Special > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Special = 0;
										}
										else Special++;
									}
								}

								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_button("socialportal", "Back")
									->add_button("", "Close")
									->add_quick_exit()
									->end_dialog("", "", "");

								gamepacket_t p2;
								p2.Insert("OnDialogRequest");
								p2.Insert(dialog.get());
								p2.CreatePacket(peer);
								/*
								DialogBuilder sb{"`o"};
								sb.add_label_icon(true, 9784, "gnettoken Store (Items)")
									.add_image_button("", "center_banner")
									.add_spacer(false)
									.add_label_icon(false, 9784, format("You currently have {} gnettokens.", setGems(pInfo(peer)->gtwl)))
									.add_textbox("Available items to purchase: ")
									.add_spacer(false);

								for (auto& shop : shopitems) {
									sb.add_custom_button(format("shop_price_{}", shop.id), format("image:interface/large/{}.rttex;image_size_x:400;image_size_y:260;width:0.19;", shop.texture));
								}

								sb.end_list().add_quick_exit();
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(sb.to_string()), p.CreatePacket(peer);
								*/
								break;
							}
							//if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopmoney\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Buy Gems``|left|9162|\n\nadd_spacer|small|\nadd_textbox|`11 gnettoken `wis `9100,000 `wgems!``|\nadd_textbox|`wYou have `1" + to_string(pInfo(peer)->gtwl) + " gnettoken`w, how much gnettoken you want to spend for gems? `7(Enter gnettoken amount)``|\nadd_text_input|gemspurchase|`1gnettoken``||30|\nend_dialog|shopgemsconfirm|Cancel|Checkout|\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|toplist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`8Top Worlds Of Today``|left|394|\nadd_spacer|" + top_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|yesterdaylist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`5Top Worlds Of Yesterday``|left|394|\nadd_spacer|" + top_yesterday_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|top\nbuttonClicked|overalllist\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Top Worlds Of All Time``|left|394|\nadd_spacer|" + top_overall_list + "\nadd_button|wotwlistback|`oBack`|NOFLAGS|0|0|\nend_dialog|top|Close||\n");
							if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopgrowtoken\n\n")p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Growtoken``|left|1486|\nadd_spacer|small|\n\nadd_textbox|`7Please choose packet you want to purchase:``|left|\n\nadd_spacer|small|\nadd_button_with_icon|gtoken_packet_1|`7Buy Packet 1|staticYellowFrame|1486|5|\nadd_button_with_icon|gtoken_packet_2|`7Buy Packet 2|staticYellowFrame|1486|10|\nadd_button_with_icon|gtoken_packet_3|`7Buy Packet 3|staticYellowFrame|1486|50|\nadd_button_with_icon|gtoken_packet_4|`7Buy Packet 4|staticYellowFrame|6802||\nadd_button_with_icon||END_LIST|noflags|0|0|\nadd_button|shop|`0Back``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							if (cch == "action|dialog_return\ndialog_name|socialportal\nbuttonClicked|onlinepointhub\n\n")
							{
								int Common = 0, Unique = 0, Special = 0;
								OldDialogBuilder dialog;
								dialog.set_default_color("o")
									->add_icon_label("`9Credit Exchange Center``", 752, LABEL_BIG)
									->add_textbox("Welcome to `9Credit Exchange Center``! Do you have any Credit Points? You can buy items here with them.")
									->add_smalltext("`#You can earn 1 Credit Point by breaking blocks besides Dirt and Cave Background.``")
									->add_spacer(SPACER_SMALL)
									->add_textbox(format("You have {} Credit Point(s).", setGems(pInfo(peer)->opc)))
									->add_spacer(SPACER_SMALL)
									->add_textbox("`1Common Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : credit_items) {
									if (credit_it.item_type == COMMON_ITEM)
									{
										dialog.add_button_with_icon_frame(format("opop_price_{}", credit_it.id), credit_it.id, credit_it.price, "staticBlueFrame");
										if (Common > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Common = 0;
										}
										else Common++;
									}
								}
								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_textbox("`#Unique Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : credit_items) {
									if (credit_it.item_type == UNIQUE_ITEM)
									{
										dialog.add_button_with_icon_frame(format("opop_price_{}", credit_it.id), credit_it.id, credit_it.price, "staticPurpleFrame");
										if (Unique > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Unique = 0;
										}
										else Unique++;
									}
								}

								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_textbox("`9Special Items:")
									->add_spacer(SPACER_SMALL);

								for (auto& credit_it : credit_items) {
									if (credit_it.item_type == SPECIAL_ITEM)
									{
										dialog.add_button_with_icon_frame("opop_price_" + to_string(credit_it.id), credit_it.id, credit_it.price, "staticYellowFrame");
										if (Special > 5) {
											dialog.add_icon_button("", 0, "END_LIST");
											Special = 0;
										}
										else Special++;
									}
								}

								dialog.add_icon_button("", 0, "END_LIST")
									->add_spacer(SPACER_SMALL)
									->add_button("socialportal", "Back")
									->add_button("", "Close")
									->add_quick_exit()
									->end_dialog("", "", "");

								gamepacket_t p2;
								p2.Insert("OnDialogRequest");
								p2.Insert(dialog.get());
								p2.CreatePacket(peer);
								break;
							}
							if (cch == "action|claimprogressbar\n")p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAbout Valentine's Event``|left|384|\nadd_spacer|small|\nadd_textbox|During Valentine's Week you will gain points for opening Golden Booty Chests. Claim enough points to earn bonus rewards.|left|\nadd_spacer|small|\nadd_textbox|Current Progress: " + to_string(pInfo(peer)->booty_broken) + "/100|left|\nadd_spacer|small|\nadd_textbox|Reward:|left|\nadd_label_with_icon|small|Super Golden Booty Chest|left|9350|\nadd_smalltext|             - 4x chance of getting a Golden Heart Crystal when opening!|left|\nend_dialog|valentines_quest||OK|\n");
							p.CreatePacket(peer);
							break;
						}
						
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankmod\n\n" || cch == "action|shoprankmod\n\n" || cch == "action|shoprankmod\n" || cch == "action|shoprankmod") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->mod) {
								shoprank_show(peer);
								break;
							}
							int modPrice = get_role_price(MODERATOR) - get_role_price(peer);
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Moderator``|left|278|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + fixint(modPrice) + " TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Receive daily moderator items everyday + you can submit your custom moderator item, only moderator commands available in `2/help`7, and Moderator role on `5TayoPs Discord Server`w.<CR><CR><CR>`4Warning`w: you might got demoted if you abuse your power!``|left|\nadd_spacer|\nadd_button|shoprankmodbuy|`0Purchase for `1" + fixint(modPrice) + " TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						/*
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankgrowpass\n\n" || cch == "action|shoprankgrowpass\n\n" || cch == "action|shoprankgrowpass\n" || cch == "action|shoprankgrowpass") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Grow Pass``|left|9222|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `150 gnettoken``|left|\nadd_smalltext|Duration: `7[```531 Days```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Unlock quests which only can be done by Growpass users with exclusive prize.``|left|\nadd_spacer|\nadd_button|shoprankgrowpassbuy|`0Purchase for `150 gnettoken``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						*/
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankglory\n\n" || cch == "action|shoprankglory\n\n" || cch == "action|shoprankglory\n" || cch == "action|shoprankglory") {
							if (pInfo(peer)->glory) {
								shoprank_show(peer);
								break;
							}
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Road to Glory``|left|9436|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1500 TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Receive 100k gems instantly, get up to 4M gems by leveling up, and Unlock Account Security feature.``|left|\nadd_spacer|\nadd_button|shoprankglorybuy|`0Purchase for `1500 TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankvvip\n\n" || cch == "action|shoprankvvip\n\n" || cch == "action|shoprankvvip\n" || cch == "action|shoprankvvip") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->vvip) {
								shoprank_show(peer);
								break;
							}
							int modPrice = get_role_price(VVIP) - get_role_price(peer);
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase VVIP``|left|278|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + fixint(modPrice) + " TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Having Yellow nickname, only VVIP commands available in `2/help`7, and more.`w.<CR><CR><CR>`4Warning`w: you might got demoted if you abuse your power!``|left|\nadd_spacer|\nadd_button|shoprankvvipbuy|`0Purchase for `1" + fixint(modPrice) + " TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopranksmod\n\n" || cch == "action|shopranksmod\n\n" || cch == "action|shopranksmod\n" || cch == "action|shopranksmod") {
						    if (pInfo(peer)->supermod) {
							    shoprank_show(peer);
							    break;
						    }

						    int modPrice = get_role_price(SUPERMOD) - get_role_price(peer);
						
						    gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Super Moderator Rank``|left|9436|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + fixint(modPrice) + " TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7You will get Super Moderator role in `5TayoPs Discord``, having some super-moderator only commands, black nickname, and you can sponsor your own super-moderator only items!``|left|\nadd_spacer|\nadd_button|shopranksmodbuy|`0Purchase for `1" + fixint(modPrice) + " TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankglorys\n\n" || cch == "action|shoprankglorys\n\n" || cch == "action|shoprankglorys\n" || cch == "action|shoprankglorys") {
						    if (pInfo(peer)->glory) {
						    	shoprank_show(peer);
						    	break;
					     	}

						    int modPrice = get_role_price(GLORY) - get_role_price(peer);
						
						    gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase Glory Rank``|left|9436|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + fixint(modPrice) + " TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\nadd_smalltext|Extra: `7Cyan display name, unlocked all glory only commands.``|left|\nadd_spacer|\nadd_button|shoprankglorybuys|`0Purchase for `1" + fixint(modPrice) + " TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankglorybuys\n\n" || cch == "action|shoprankglorybuys\n\n" || cch == "action|shoprankglorybuys\n" || cch == "action|shoprankglorybuys") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->glory) {
								shoprank_show(peer);
								break;
							}

							int modPrice = get_role_price(GLORY) - get_role_price(peer);
							if (pInfo(peer)->gtwl >= modPrice) {
								pInfo(peer)->gtwl -= modPrice;
								//GlobalStats.totalModeratorBought++;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								pInfo(peer)->glory = 1;
								gamepacket_t p2, p3;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`#**`` `$The Ancient Ones`` have made`` " + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "`` as `8GLORY`` (`2/shop`` to purchase GLORY!)");
								p3.Insert("OnConsoleMessage");
								p3.Insert("`o>> You purchased GLORY Role!");
								p3.CreatePacket(peer);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									p2.CreatePacket(currentPeer);
								}
								save_player(pInfo(peer), false);
								enet_peer_disconnect_later(peer, 0);
							}
							shoprank_show(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankvvipbuy\n\n" || cch == "action|shoprankvvipbuy\n\n" || cch == "action|shoprankvvipbuy\n" || cch == "action|shoprankvvipbuy") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->vvip) {
								shoprank_show(peer);
								break;
							}

							int modPrice = get_role_price(VVIP) - get_role_price(peer);
							if (pInfo(peer)->gtwl >= modPrice) {
								pInfo(peer)->gtwl -= modPrice;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								pInfo(peer)->vvip = 1;
								gamepacket_t p2, p3;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`#**`` `$The Ancient Ones`` have made`` " + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "`` as `9VVIP`` (`2/shop`` to purchase VVIP!)");
								p3.Insert("OnConsoleMessage");
								p3.Insert("`o>> You purchased VVIP!``");
								p3.CreatePacket(peer);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									p2.CreatePacket(currentPeer);
								}
								save_player(pInfo(peer), false);
								enet_peer_disconnect_later(peer, 0);
							}

							shoprank_show(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankmodbuy\n\n" || cch == "action|shoprankmodbuy\n\n" || cch == "action|shoprankmodbuy\n" || cch == "action|shoprankmodbuy") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->mod) {
								shoprank_show(peer);
								break;
							}

							int modPrice = get_role_price(MODERATOR) - get_role_price(peer);
							if (pInfo(peer)->gtwl >= modPrice) {
								pInfo(peer)->gtwl -= modPrice;
								//GlobalStats.totalModeratorBought++;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								pInfo(peer)->mod = 1;
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`#**`` `$The Ancient Ones`` have made`` " + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "`` as `#Moderator`` (`2/shop`` to purchase moderator!)");
								
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									p2.CreatePacket(currentPeer);
								}
								save_player(pInfo(peer), false);
								enet_peer_disconnect_later(peer, 0);
							}
							shoprank_show(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shopranksmodbuy\n\n" || cch == "action|shopranksmodbuy\n\n" || cch == "action|shopranksmodbuy\n" || cch == "action|shopranksmodbuy") {
							//int modPurchased = GlobalStats.totalModeratorBought;
							if (pInfo(peer)->supermod) {
								shoprank_show(peer);
								break;
							}

							int modPrice = get_role_price(SUPERMOD) - get_role_price(peer);
							if (pInfo(peer)->gtwl >= modPrice) {
								pInfo(peer)->gtwl -= modPrice;
								//GlobalStats.totalModeratorBought++;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								pInfo(peer)->supermod = 1;
								gamepacket_t p2;
								p2.Insert("OnConsoleMessage");
								p2.Insert("`#**`` `$The Ancient Ones`` have made`` " + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "`` as `bSuper-Moderator`` (`2/shop`` to purchase Super-Moderator!)");
								
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									p2.CreatePacket(currentPeer);
								}
								save_player(pInfo(peer), false);
								enet_peer_disconnect_later(peer, 0);
							}
							shoprank_show(peer);
							break;
						}
						/*
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankgrowpassbuy\n\n" || cch == "action|shoprankgrowpassbuy\n\n" || cch == "action|shoprankgrowpassbuy\n" || cch == "action|shoprankgrowpassbuy") {
							if (pInfo(peer)->gtwl >= 50) {
								int c_ = 1;
								if (modify_inventory(peer, 11304, c_) == 0) {
									pInfo(peer)->gtwl -= 50;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased Royal Grow Pass Token, check your inventory and consume to get the benefits!");
									p.CreatePacket(peer);
									save_player(pInfo(peer), false);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1gnettoken`` to buy this.");
								p.CreatePacket(peer);
							}
							enet_peer_disconnect_later(peer, 0);
							break;
						}
						*/
						else if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|shoprankglorybuy\n\n" || cch == "action|shoprankglorybuy\n\n" || cch == "action|shoprankglorybuy\n" || cch == "action|shoprankglorybuy") {
							if (pInfo(peer)->gtwl >= 500) {
								pInfo(peer)->gems += 100000;
								gamepacket_t p;
								p.Insert("OnSetBux");
								p.Insert(pInfo(peer)->gems);
								p.Insert(0);
								p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
								if (pInfo(peer)->supp >= 2) {
									p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
								}
								p.CreatePacket(peer);
								pInfo(peer)->gtwl -= 100;
								packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
								pInfo(peer)->glo = 1;
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased Road to Glory! Wrench yourself and press on Road to Glory button!``");
									p.CreatePacket(peer);
								}
								enet_peer_disconnect_later(peer, 0);
							}
							break;
						}
						else if (cch == "action|storenavigate\nitem|main\nselection|deposit\n" || cch == "action|storenavigate\nitem|locks\nselection|upgrade_backpack\n" || cch == "action|storenavigate\nitem|main\nselection|bonanza\n" || cch == "action|storenavigate\nitem|main\nselection|calendar\n" || cch == "action|store\nlocation|bottommenu\n" || cch == "action|store\nlocation|gem\n" || cch == "action|store\nlocation|pausemenu\n" || cch == "action|storenavigate\nitem|main\nselection|gems_rain\n") {
							if (cch == "action|store\nlocation|bottommenu\n" || cch == "action|store\nlocation|gem\n" || cch == "action|store\nlocation|pausemenu\n") shop_tab(peer, "tab1");
							if (cch == "action|storenavigate\nitem|main\nselection|gems_rain\n") shop_tab(peer, "tab1_1");
							if (cch == "action|storenavigate\nitem|main\nselection|calendar\n") shop_tab(peer, "tab1_2");
							if (cch == "action|storenavigate\nitem|main\nselection|bonanza\n")	shop_tab(peer, "tab1_3");
							if (cch == "action|storenavigate\nitem|locks\nselection|upgrade_backpack\n") shop_tab(peer, "tab2_1");
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|claim_reward\n\n") {
							if (pInfo(peer)->gp == 1) {
								if (today_day != pInfo(peer)->gd) {
									// IMPROVE GROW PASS REWARDS
									vector<int> list2{ 1796, 10396, 11476, 542, 10386, 722, 10826 };
									int receive = 1, item = list2[rand() % list2.size()], received = 1;
									if (item == 10386 || item == 722) receive = 20, received = 20;
									if (item == 542) receive = 100, received = 100;
									if (item == 10826) receive = 5, received = 5;
									if (modify_inventory(peer, item, receive) == 0) {
										pInfo(peer)->gd = today_day;
										save_player(pInfo(peer), false);
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`9 >> You claimed your Grow Pass reward:");
											p.CreatePacket(peer);
										}
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Given `0" + to_string(received) + " " + items[item].name + "``.");
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17;
										data_.netID = 48;
										data_.YSpeed = 48;
										data_.x = pInfo(peer)->x + 16;
										data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world) {
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
										}
										delete[] raw;
									}
									else {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Clean your inventory and try again!");
											p.CreatePacket(peer);
										}
									}
								}
								else {
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You already claimed your reward today!");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|redeem_dialog\nr_code|") != string::npos) {
						string code = explode("r_code|", cch)[1];
						if (code.empty()) break;
						//cout << code << endl;
						// code is correct alrd but it doesnt even respond the real ans
						replace(code, "\n", "");
						ifstream r_c("database/redeem/-" + code + ".json");
						if (r_c.is_open()) {
							json j;
							r_c >> j;

							switch (j["reward_type"].get<int>()) {
							case 1: {
								vector<string> users_ = j["users"].get<vector<string>>();

								if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
									Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
									break;
								}

								if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
									Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
									break;
								}

								int item_ = j["id"].get<int>();
								int c_ = j["count"].get<int>();
								int stored_ = c_;
								int used_ = j["time_used"].get<int>();
								int max_ = j["max_time"].get<int>();

								if (c_ == 0) break;
								if (get_free_slots(pInfo(peer)) < 1) {
									Algorithm::send_console(peer, "Failed to redeem, you don't have enough space in your backpack.");
									break;
								}
								if (modify_inventory(peer, item_, c_) == 0) {
									users_.push_back(pInfo(peer)->tankIDName);
									Algorithm::send_console(peer, format("Successfully redeemed {} {}.", stored_, items[item_].name));
									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 1;
									j_["id"] = item_;
									j_["count"] = stored_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/anni_plu.rttex");
									p.Insert(format("Successfully redeemed {} {}.", stored_, items[item_].name));
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("** `4Global System Message: ``{} has redeemed {} {}!", pInfo(peer)->tankIDName, stored_, items[item_].name));
										p.CreatePacket(currentPeer);
									}
									break;
								}
								break;
							}
							case 2: {
								string role_ = j["role"].get<string>();
								int used_ = j["time_used"].get<int>();
								int max_ = j["max_time"].get<int>();

								if (role_ == "dev") {
									vector<string> users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->dev) {
										Algorithm::send_console(peer, "You already have a Developer Role.");
										break;
									}
									pInfo(peer)->dev = 1;
									Algorithm::send_console(peer, "Successfully redeemed a Developer role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to Developer.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `6@Developer``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "admin") {
									vector<string> users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->admin) {
										Algorithm::send_console(peer, "You already have an Admin Role.");
										break;
									}
									pInfo(peer)->admin = 1;
									Algorithm::send_console(peer, "Successfully redeemed an Admin role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to Admin.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `8@Admin``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "supermod") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->supermod) {
										Algorithm::send_console(peer, "You already have a Super Moderator Role.");
										break;
									}
									pInfo(peer)->supermod = 1;
									Algorithm::send_console(peer, "Successfully redeemed a Super Moderator role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to Super Moderator.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `b@Super Moderator``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "mod") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->mod) {
										Algorithm::send_console(peer, "You already have a Moderator Role.");
										break;
									}
									pInfo(peer)->mod = 1;
									Algorithm::send_console(peer, "Successfully redeemed a Moderator role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to Moderator.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `#@Moderator``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "glory") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->glory) {
										Algorithm::send_console(peer, "You already have a Glory Role.");
										break;
									}
									pInfo(peer)->glory = 1;
									Algorithm::send_console(peer, "Successfully redeemed a Glory role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to GLORY.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `cGLORY``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "vvip") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->vvip) {
										Algorithm::send_console(peer, "You already have a VVIP Role.");
										break;
									}
									pInfo(peer)->vvip = 1;
									Algorithm::send_console(peer, "Successfully redeemed a VVIP role!");
									save_player(pInfo(peer), false);
									//enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to VVIP.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `9VVIP``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else if (role_ == "vip") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->vip) {
										Algorithm::send_console(peer, "You already have a VIP Role.");
										break;
									}
									pInfo(peer)->vip = 1;
									Algorithm::send_console(peer, "Successfully redeemed a VIP role!");
									save_player(pInfo(peer), false);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 2;
									j_["role"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have been promoted to VIP.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert(format("`#**`` The Ancient Ones have `2promoted `w{}`` to `1VIP``! (Via /redeem MTPS-CODE)", pInfo(peer)->tankIDName));
										p.CreatePacket(currentPeer);
									}
									enet_peer_disconnect_later(peer, 0);
								}
								else {
									Algorithm::send_console(peer, "Something is wrong! Please contact the developer to tell about this issue.");
									break;
								}
								break;
							}
							case 3: {
								vector<string> users_ = j["users"].get<vector<string>>();

								if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
									Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
									break;
								}

								if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
									Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
									break;
								}

								int c_ = j["count"].get<int>();
								int used_ = j["time_used"].get<int>();
								int max_ = j["max_time"].get<int>();

								if (c_ == 0) break;
								if (get_free_slots(pInfo(peer)) < 1) {
									Algorithm::send_console(peer, "Failed to redeem, you don't have enough space in your backpack.");
									break;
								}
								pInfo(peer)->gtwl += c_;
								save_player(pInfo(peer), false);
								{
									users_.push_back(pInfo(peer)->tankIDName);
									Algorithm::send_console(peer, format("Successfully redeemed {} TayoPoint.", c_));
									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 3;
									j_["count"] = c_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/anni_plu.rttex");
									p.Insert(format("Successfully redeemed {} TayoPoint.", c_));
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									break;
								}
								break;
							}
							case 4: {
								string role_ = j["cheat"].get<string>();
								int used_ = j["time_used"].get<int>();
								int max_ = j["max_time"].get<int>();

								if (role_ == "autofarm") {
									vector<string> users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_af) {
										Algorithm::send_console(peer, "You already have Autofarm Cheat.");
										break;
									}
									pInfo(peer)->unlocked_af = true;
									Algorithm::send_console(peer, "Successfully redeemed Autofarm Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Autofarm.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "fastspin") {
									vector<string> users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_fs) {
										Algorithm::send_console(peer, "You already have a Fast Spin Cheat.");
										break;
									}
									pInfo(peer)->unlocked_fs = true;
									Algorithm::send_console(peer, "Successfully redeemed Fast Spin Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Fast Spin Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "hidechat") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_hc) {
										Algorithm::send_console(peer, "You already have Hide Chat Cheat.");
										break;
									}
									pInfo(peer)->unlocked_hc = true;
									Algorithm::send_console(peer, "Successfully redeemed Hide Chat Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Hide Chat Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "autocollect") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_at) {
										Algorithm::send_console(peer, "You already have Auto Collect Cheat.");
										break;
									}
									pInfo(peer)->unlocked_at = true;
									Algorithm::send_console(peer, "Successfully redeemed Auto Collect Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Auto Collect Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "fastdrop") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_fd) {
										Algorithm::send_console(peer, "You already have Fast Drop Cheat.");
										break;
									}
									pInfo(peer)->unlocked_fd = true;
									Algorithm::send_console(peer, "Successfully redeemed Fast Drop Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Fast Drop Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "fastpull") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_fp) {
										Algorithm::send_console(peer, "You already have Fast Pull Cheat.");
										break;
									}
									pInfo(peer)->unlocked_fp = true;
									Algorithm::send_console(peer, "Successfully redeemed Fast Pull Cheat!");
									save_player(pInfo(peer), false);
									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Fast Pull Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);
									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else if (role_ == "fasttrash") {
									vector<string> users_;
									users_ = j["users"].get<vector<string>>();

									if (j["time_used"].get<int>() >= j["max_time"].get<int>()) {
										Algorithm::send_console(peer, "The code can't longer be used because it reached the limit.");
										break;
									}

									if (find(users_.begin(), users_.end(), pInfo(peer)->tankIDName) != users_.end()) {
										Algorithm::send_console(peer, "You've already redeemed this code, don't be greedy.");
										break;
									}
									users_.push_back(pInfo(peer)->tankIDName);
									if (pInfo(peer)->unlocked_ft) {
										Algorithm::send_console(peer, "You already have Fast Trash Cheat.");
										break;
									}
									pInfo(peer)->unlocked_ft = true;
									Algorithm::send_console(peer, "Successfully redeemed Fast Trash Cheat!");
									save_player(pInfo(peer), false);

									gamepacket_t p;
									p.Insert("OnAddNotification");
									p.Insert("interface/large/gd_prom.rttex");
									p.Insert("You have unlocked Fast Trash Cheat.");
									p.Insert("audio/cumbia_horns.wav");
									p.Insert(0);
									p.CreatePacket(peer);

									enet_peer_disconnect_later(peer, 0);

									ofstream wr_("database/redeem/-" + code + ".json");
									json j_;
									j_["reward_type"] = 4;
									j_["cheat"] = role_;
									j_["time_used"] = used_ + 1;
									j_["max_time"] = max_;
									j_["users"] = users_;
									wr_ << j_ << endl;
									wr_.close();
								}
								else {
									Algorithm::send_console(peer, "Something is wrong! Please contact the developer to tell about this issue.");
									break;
								}
								break;
							}
							default: {
								Algorithm::send_console(peer, "Something is wrong! Please contact the developer to tell about this issue.");
								break;
							}
							}
							r_c.close();
							break;
						}
						else {
							Algorithm::send_console(peer, "That code doesn't exist.");
							break;
						}
						break;
						}
						else if (cch == "action|dialog_return\ndialog_name|surge\n") {
							if (pInfo(peer)->lastwrenchb == 4296 || pInfo(peer)->lastwrenchb == 8558) {
								setstats(peer, rand() % 30, "", items[pInfo(peer)->lastwrenchb].name);
								pInfo(peer)->lastwrenchb = 0;
							}
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|builder_reward\n\n" || cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|fishing_reward\n\n" || cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|geiger_reward\n\n" || cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|farmer_reward\n\n" || cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|provider_reward\n\n") {
							if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|farmer_reward\n\n") farmer_reward_show(peer);
							if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|provider_reward\n\n")provider_reward_show(peer);
							if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|geiger_reward\n\n") geiger_reward_show(peer);
							if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|fishing_reward\n\n") 	fishing_reward_show(peer);
							if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|builder_reward\n\n")	builder_reward_show(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|backpack_upgrade\n\n") {
							if (pInfo(peer)->gems >= (1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2) {
								if (pInfo(peer)->b_l * 10 > 250) {
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have reached max slots!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
								else {
									pInfo(peer)->gems -= (1000 * ((pInfo(peer)->b_l * pInfo(peer)->b_l) + 25)) * 2;
									pInfo(peer)->b_l++;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									}
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have upgraded your Backpack!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									backpack_show(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("You don't have enough gems!");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							break;
						}
						
						else if (cch == "action|battlepasspopup\n") {
							gamepacket_t p(550);
							p.Insert("OnDialogRequest");
							int growpassid = 6124;
							if (today_day == pInfo(peer)->gd) growpassid = 6292;
							if (pInfo(peer)->gp == 1) p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_smalltext|`9You can claim your daily reward everyday here.``|left|\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|claim_reward||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|worlds_list||Back|\nadd_quick_exit|\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|Grow Pass Rewards|left|9222|\nadd_button|deposit|`2Purchase``|noflags|0|0|\nadd_smalltext|`4You must purchase the Grow Pass role to claim your prize!``|left|\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon|||staticBlueFrame|" + to_string(growpassid) + "||\nadd_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|||Back|\nadd_quick_exit|\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|AccountSecurity\nlocation|pausemenu\n") {
							gamepacket_t p(500);
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wAdvanced Account Protection ``|left|3732|\nadd_textbox|`1You are about to enable the Advanced Account Protection.``|left|\nadd_textbox|`1After that, every time you try to log in from a new device and IP you will receive an dialog with a PIN confirmation button.``|left|\nadd_textbox|`1This will significantly increase your account security.``|left|\nend_dialog|secureaccount|Cancel|Ok|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|surgery_reward\n\n") {
							surgery_reward_show(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|2646\nbuttonClicked|off\n\n") {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or block_->spotlight != pInfo(currentPeer)->tankIDName) continue;
									pInfo(currentPeer)->spotlight = false, update_clothes(currentPeer);
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("Back to anonymity. (`$In the Spotlight`` mod removed)"), p.CreatePacket(currentPeer);
								}
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Lights out!"), p.CreatePacket(peer);
								block_->spotlight = "";
							}
							break;
						}
						else if (cch == "action|refresh_item_data\n") {
							if (pInfo(peer)->isIn) {
								packet_(peer, "action|log\nmsg|Hy! Welcome To Our Server!, Enjoy With Our Server");
								packet_(peer, "action|log\nmsg|One moment, updating item data...");
								sendItemDataToPeer(peer);
								//enet_host_flush(server);
							}
							break;
						}
						if (cch == "action|enter_game\n" && pInfo(peer)->isIn && !pInfo(peer)->in_enter_game) {
							pInfo(peer)->in_enter_game = true;
							if (pInfo(peer)->isIn) {
								pInfo(peer)->enter_game++;
								if (pInfo(peer)->world == "" && pInfo(peer)->enter_game == 1) {
									if (pInfo(peer)->tankIDName.empty()) {
										gamepacket_t p(250);
										p.Insert("OnDialogRequest"), p.Insert(Algorithm::r_dialog("")), p.CreatePacket(peer);
									}
									else {
										pInfo(peer)->tmod = pInfo(peer)->mod;
										pInfo(peer)->name_color = (pInfo(peer)->dev == 1 ? "`6@" : pInfo(peer)->admin == 1 ? "`8@" : pInfo(peer)->supermod == 1 ? "`b@" : (pInfo(peer)->tmod == 1) ? "`#@" : 
											pInfo(peer)->glory == 1 ? "`c[GLORY] " : pInfo(peer)->vvip == 1 ? "`9[VVIP] " : pInfo(peer)->vip == 1 ? "`1[VIP] " : "`0");
										pInfo(peer)->modName = pInfo(peer)->name_color + pInfo(peer)->tankIDName;
										if (is_reseller(peer) and not pInfo(peer)->hide_reseller) pInfo(peer)->modName += " `b[Shadow]``";
										int on_ = 0, t_ = 0;
										vector<int> friends_;
										for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) friends_.push_back(pInfo(peer)->friends[c_].uid);
										if (not pInfo(peer)->invis and not pInfo(peer)->m_h) {
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												t_ += 1 /*+ rand() % 3 + 1*/;
												if (find(friends_.begin(), friends_.end(), pInfo(currentPeer)->uid) != friends_.end()) {
													if (pInfo(currentPeer)->show_friend_notifications_) {
														gamepacket_t p;
														p.Insert("OnConsoleMessage"), p.Insert("`3FRIEND ALERT:`` " + pInfo(peer)->modName + "`` has `2logged on``.");
														packet_(currentPeer, "action|play_sfx\nfile|audio/friend_logon.wav\ndelayMS|0");
														p.CreatePacket(currentPeer);
													}
													on_++;
												}
											}
										}
										{
											gamepacket_t p;
											p.Insert("OnEmoticonDataChanged");
											p.Insert(151749376);
											p.Insert("(wl)|ā|1&(yes)|Ă|1&(no)|ă|1&(love)|Ą|1&(oops)|ą|1&(shy)|Ć|1&(wink)|ć|1&(tongue)|Ĉ|1&(agree)|ĉ|1&(sleep)|Ċ|1&(punch)|ċ|1&(music)|Č|1&(build)|č|1&(megaphone)|Ď|1&(sigh)|ď|1&(mad)|Đ|1&(wow)|đ|1&(dance)|Ē|1&(see-no-evil)|ē|1&(bheart)|Ĕ|1&(heart)|ĕ|1&(grow)|Ė|1&(gems)|ė|1&(kiss)|Ę|1&(gtoken)|ę|1&(lol)|Ě|1&(smile)|Ā|1&(cool)|Ĝ|1&(cry)|ĝ|1&(vend)|Ğ|1&(bunny)|ě|1&(cactus)|ğ|1&(pine)|Ĥ|1&(peace)|ģ|1&(terror)|ġ|1&(troll)|Ġ|1&(evil)|Ģ|1&(fireworks)|Ħ|1&(football)|ĥ|1&(alien)|ħ|1&(party)|Ĩ|1&(pizza)|ĩ|1&(clap)|Ī|1&(song)|ī|1&(ghost)|Ĭ|1&(nuke)|ĭ|1&(halo)|Į|1&(turkey)|į|1&(gift)|İ|1&(cake)|ı|1&(heartarrow)|Ĳ|1&(lucky)|ĳ|1&(shamrock)|Ĵ|1&(grin)|ĵ|1&(ill)|Ķ|1&(eyes)|ķ|1&(weary)|ĸ|1&(moyai)|ļ|1&(plead)|Ľ|0&");
											p.CreatePacket(peer);
										}
										if (pInfo(peer)->surgery_type == -1) pInfo(peer)->surgery_type = rand() % 30;
										SendReceive(peer);
										{
											if (pInfo(peer)->pinata_day != today_day) {
												pInfo(peer)->pinata_prize = false;
												pInfo(peer)->pinata_claimed = false;
											}
											gamepacket_t p;
											p.Insert("OnProgressUISet"), p.Insert(1), p.Insert(0), p.Insert(to_string(pInfo(peer)->pinata_claimed)), p.Insert(1), p.Insert(""), p.Insert(to_string(pInfo(peer)->pinata_prize)), p.CreatePacket(peer);
										}
										if (!load_affinity(pInfo(peer))) {
											pInfo(peer)->affinity_type = NO_AFFINITY;
											pInfo(peer)->affinity_level = 0;
											pInfo(peer)->affinity_with = 0;
										}
										if (pInfo(peer)->affinity_level > 5)
										{
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if ((pInfo(peer)->affinity_with) == (pInfo(currentPeer)->uid)) {
													pInfo(currentPeer)->affinity_level = 0;
													pInfo(currentPeer)->affinity_with = 0;
													pInfo(currentPeer)->affinity_type = NO_AFFINITY;
												}
											}
											try {
												string path = "database/affinity/" + to_string(pInfo(peer)->uid) + "_.json";
												if (!fs::remove(path.c_str())) {
													//printf("Unable to delete file %s\n", pInfo(peer)->affinity_with.c_str());
													break;
												}
												string path__ = "database/affinity/" + to_string(pInfo(peer)->affinity_with) + "_.json";
												if (!fs::remove(path__.c_str())) {
													//printf("Unable to delete file %s\n", pInfo(peer)->affinity_with.c_str());
													break;
												}
											}
											catch (fs::filesystem_error& e) {
												tayo_warn(e.what());
											}
											pInfo(peer)->affinity_type = NO_AFFINITY;
											pInfo(peer)->affinity_level = 0;
											pInfo(peer)->affinity_with = 0;
										}
										if (pInfo(peer)->affinity_with <= 0 || pInfo(peer)->affinity_level <= 0 || pInfo(peer)->affinity_level > 5) pInfo(peer)->affinity_type = NO_AFFINITY;
										if (pInfo(peer)->affinity_type == LOVER) pInfo(peer)->titleDoctor = true;
										else if (pInfo(peer)->affinity_type == BESTIE) pInfo(peer)->titleStartopia = true;
										// ENTER GAME NOTIFICATION
										gamepacket_t p1, p5, p3;
										if (admin_server)
											Algorithm::send_console(peer, "Welcome to `6TayoPs Developer Server``");

										p1.Insert("OnConsoleMessage"), p1.Insert("Welcome back, `w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "````." + (pInfo(peer)->friends.size() == 0 ? "" : (on_ != 0 ? " `w" + to_string(on_) + "`` friend is online." : " No friends are online."))), p1.CreatePacket(peer);
										{
											sort(pInfo(peer)->worlds_owned.begin(), pInfo(peer)->worlds_owned.end());
											pInfo(peer)->worlds_owned.erase(unique(pInfo(peer)->worlds_owned.begin(), pInfo(peer)->worlds_owned.end()), pInfo(peer)->worlds_owned.end());
										}
										int got = 0, c_ = 1;
										modify_inventory(peer, 6336, got);
										if (got == 0) {
											modify_inventory(peer, 6336, c_);
										}
										got = 0;
										c_ = 1;
										modify_inventory(peer, 32, got);
										if (got == 0) {
											modify_inventory(peer, 32, c_);
										}
										got = 0;
										c_ = 1;
										modify_inventory(peer, 18, got);
										if (got == 0) {
											modify_inventory(peer, 18, c_);
										}

										//p5.Insert("OnConsoleMessage"), p5.Insert("`3Weekend Events: You'll get extra `1gnettoken `3if you top up.``"), p5.CreatePacket(peer);
										if (today_day != pInfo(peer)->dd) {
											pInfo(peer)->AlreadyDailyQ = false;
											pInfo(peer)->dd = today_day;
										}
										if (pInfo(peer)->founder) {
											packet_(peer, "action|log\nmsg|You're `c@Founder``! Unlocked all roles and Super Supporter.");
											pInfo(peer)->admin = 1;
											pInfo(peer)->supermod = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->owner = 1;

											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->dev = 1;

											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->umod = 1;
											pInfo(peer)->staff = 1;



											pInfo(peer)->supp = 2;
											pInfo(peer)->unlocked_ab = true;
											pInfo(peer)->unlocked_af = true;
											//pInfo(peer)->unlocked_as = true;
											pInfo(peer)->unlocked_at = true;
											pInfo(peer)->unlocked_fd = true;
											pInfo(peer)->unlocked_fp = true;
											pInfo(peer)->unlocked_fs = true;
											pInfo(peer)->unlocked_ft = true;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->unlocked_hc = true;
											//pInfo(peer)->unlocked_hp = true;
											pInfo(peer)->unlocked_sj = true;
											pInfo(peer)->unlocked_ss = true;
										}
										else	if (pInfo(peer)->owner) {
											packet_(peer, "action|log\nmsg|You're `c@Owner``! Unlocked all roles and Super Supporter.");
											pInfo(peer)->admin = 1;
											pInfo(peer)->supermod = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->dev = 1;

											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->umod = 1;
											pInfo(peer)->staff = 1;



											pInfo(peer)->supp = 2;
											pInfo(peer)->unlocked_ab = true;
											pInfo(peer)->unlocked_af = true;
											//pInfo(peer)->unlocked_as = true;
											pInfo(peer)->unlocked_at = true;
											pInfo(peer)->unlocked_fd = true;
											pInfo(peer)->unlocked_fp = true;
											pInfo(peer)->unlocked_fs = true;
											pInfo(peer)->unlocked_ft = true;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->unlocked_hc = true;
											//pInfo(peer)->unlocked_hp = true;
											pInfo(peer)->unlocked_sj = true;
											pInfo(peer)->unlocked_ss = true;
										}
										else if (pInfo(peer)->staff) {
											packet_(peer, "action|log\nmsg|You're `8@Staff``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->supermod = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->admin = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->supp = 2;
											pInfo(peer)->dev = 1;

										}
										else if (pInfo(peer)->dev) {
											packet_(peer, "action|log\nmsg|You're `6@Developer``! Unlocked all roles and Super Supporter.");
											pInfo(peer)->admin = 1;
											pInfo(peer)->supermod = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->umod = 1;
											pInfo(peer)->supp = 2;
											pInfo(peer)->unlocked_ab = true;
											pInfo(peer)->unlocked_af = true;
											//pInfo(peer)->unlocked_as = true;
											pInfo(peer)->unlocked_at = true;
											pInfo(peer)->unlocked_fd = true;
											pInfo(peer)->unlocked_fp = true;
											pInfo(peer)->unlocked_fs = true;
											pInfo(peer)->unlocked_ft = true;
											pInfo(peer)->unlocked_hc = true;
											//pInfo(peer)->unlocked_hp = true;
											pInfo(peer)->unlocked_sj = true;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->unlocked_ss = true;
										}
										
										else if (pInfo(peer)->admin) {
											packet_(peer, "action|log\nmsg|You're `8@Admin``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->supermod = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->umod) {
											packet_(peer, "action|log\nmsg|You're `6@Ultra-Moderator``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->supermod = 1;

											pInfo(peer)->vvip = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->guardian = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->supermod) {
											packet_(peer, "action|log\nmsg|You're `b@Super-Moderator``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->mod = 1;
											pInfo(peer)->glory = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->guardian = 1;

											pInfo(peer)->mvp = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->guardian) {
											packet_(peer, "action|log\nmsg|You're `#@Guardian``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->glory = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->mod = 1;

											pInfo(peer)->tmod = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->mod) {
											packet_(peer, "action|log\nmsg|You're `#@Moderator``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->glory = 1;
											pInfo(peer)->vvip = 1;
											pInfo(peer)->vip = 1;
											pInfo(peer)->mvp = 1;
											pInfo(peer)->tmod = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->glory) {
											packet_(peer, "action|log\nmsg|You're `cGLORY``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->vvip = 1;
											pInfo(peer)->mvp = 1;

											pInfo(peer)->vip = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->mvp) {
											packet_(peer, "action|log\nmsg|You're `7MVP``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->vip = 1;
											pInfo(peer)->vvip = 1;

											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->vvip) {
											packet_(peer, "action|log\nmsg|You're `9VVIP``! Unlocked all lower roles and Super Supporter.");
											pInfo(peer)->vip = 1;
											pInfo(peer)->supp = 2;
										}
										else if (pInfo(peer)->vip) {
											packet_(peer, "action|log\nmsg|You're `1VIP``! Unlocked VIP Powers and Supporter.");
											pInfo(peer)->supp = pInfo(peer)->supp < 2 ? 1 : 2;
										}
										if (pInfo(peer)->mod) {
											if (get_free_slots(pInfo(peer)) >= 1) {
												if (today_day != pInfo(peer)->mds) {
													pInfo(peer)->mds = today_day;
													vector<int> list2{ 408, 274, 276 };
													int receive = 1, item = list2[rand() % list2.size()], got = 1;
													modify_inventory(peer, item, receive);
													save_player(pInfo(peer), false);
													gamepacket_t p, p2;
													p.Insert("OnConsoleMessage"), p.Insert("Your mod appreciation bonus (feel free keep, trade, or use for prizes) for today is:"), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Given `0" + to_string(got) + " " + items[item].name + "``."), p2.CreatePacket(peer);
												}
											}
										}
										if (pInfo(peer)->lastquest == "zodiac") pInfo(peer)->lastquest = "";
										if (pInfo(peer)->unlocked_hp and not pInfo(peer)->dev) {
											Algorithm::send_console(peer, "You've received 50,000 gems as the compensation of item removal.");
											pInfo(peer)->gems += 50000;
											pInfo(peer)->unlocked_hp = false;
											save_player(pInfo(peer), false);
										}
										if (pInfo(peer)->unlocked_as and not pInfo(peer)->dev) {
											Algorithm::send_console(peer, "You've received 500,000 gems as the compensation of item removal.");
											pInfo(peer)->gems += 500000;
											pInfo(peer)->unlocked_as = false;
											save_player(pInfo(peer), false);
										}
										//pInfo(peer)->unlocked_ab = true;
										if (pInfo(peer)->unlocked_ab || pInfo(peer)->unlocked_af || 
											//pInfo(peer)->unlocked_as || 
											pInfo(peer)->unlocked_at || pInfo(peer)->unlocked_fd
											|| pInfo(peer)->unlocked_fp || pInfo(peer)->unlocked_fs || 
											pInfo(peer)->unlocked_ft || pInfo(peer)->unlocked_hc 
											//|| pInfo(peer)->unlocked_hp
											|| pInfo(peer)->unlocked_sj || pInfo(peer)->unlocked_ss) pInfo(peer)->unlocked_ci = true;

										world_menu(peer);
										//news(peer);
										pInfo(peer)->Cheat_AutoCollect = true; 
									}
								}
								else {
									enet_peer_disconnect_later(peer, 0);
									save_player(pInfo(peer), false);
								}
								break;
							}
						}
						else if (cch.find("buttonClicked|visit_home_world_") != string::npos) {
							if (pInfo(peer)->last_world_enter + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(peer)->last_world_enter = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								string nameworld = explode("\n", explode("buttonClicked|visit_home_world_", cch)[1])[0];
								join_world(peer, nameworld);
								break;
							}
						}
						else if (cch.find("buttonClicked|rename_world") != string::npos) {
							gamepacket_t p;
							p.Insert("OnTextOverlay");
							p.Insert("Sorry but this feature is still under development.");
							p.CreatePacket(peer);
							break;
						}
						if (cch.find("buttonClicked|clear_items") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (paa != worlds.end()) {
								World* world_ = &worlds[paa - worlds.begin()];
								world_->drop.clear();
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`#**`` `$The Ancient Ones `ohave `bcleared `odropped items `oat `w" + pInfo(peer)->world + " `#** `o(`4/rules`o to see the rules!)");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										packet_(currentPeer, "action|play_sfx\nfile|audio/bigboom.wav\ndelayMS|0");
										p.CreatePacket(currentPeer);
									}
									add_modlogs(peer, (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name), "CLEARED DROPPED ITEMS AT: `#" + pInfo(peer)->world, "");
									{
										world_->drop.clear();
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											exit_(currentPeer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("buttonClicked|clear_world") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->admin == 1) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (paa != worlds.end()) {
									World* world_ = &worlds[paa - worlds.begin()];
									if (world_->guild_id != 0) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("You can't clear Guild World");
										p.CreatePacket(peer);
										break;
									}
									else {
										pInfo(peer)->confirm_reset = 0;
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`#**`` `$The Ancient Ones `ohave `bcleared `oworld `w" + pInfo(peer)->world + " `#**`o (`4/rules`o to see the rules!)");
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												packet_(currentPeer, "action|play_sfx\nfile|audio/bigboom.wav\ndelayMS|0");
												p.CreatePacket(currentPeer);
											}
											add_modlogs(peer, (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name), "CLEARED WORLD: `#" + pInfo(peer)->world, "");
											{
												world_->nuked = false;
												world_->n_t = "";
												world_->n_b = "";
												world_->admins.clear();
												world_->drop.clear();
												world_->sbox1.clear();
												world_->bulletin.clear();
												world_->weather = 0;
												world_->open_to_public = false, world_->disable_music_blocks = false, world_->make_music_blocks_invisible = false;
												world_->bannedPlayers.clear();
												//world_->banned_player_uid.clear();
												world_->music_bpm = 100;
												world_->entry_level = 1;
												world_->active_jammers.clear();
												world_->npc.clear();
												world_->machines.clear();
												world_->save_time = 0;
												for (int i_ = 0; i_ < world_->blocks.size(); i_++) {
													if (i_ <= ((world_->width * world_->height) - ((world_->width) * (6 * (world_->width / 100))))) {
														if (world_->blocks[i_].fg != 0 and world_->blocks[i_].fg != 6 and world_->blocks[i_].fg != 8 and items[world_->blocks[i_].fg].blockType != BlockTypes::LOCK) world_->blocks[i_].fg = 0;
														if (world_->blocks[i_].bg != 0) world_->blocks[i_].bg = 0;
													}
												}
											}
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
												exit_(currentPeer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("buttonClicked|nuke_worldconfirm") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->admin == 1) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (paa != worlds.end()) {
									World* world_ = &worlds[paa - worlds.begin()];
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|\nadd_textbox|`4Warning: `oNuking the world without providing enough proofs or for no reason will result in a `4permanent suspension``|\nadd_spacer|small|\nadd_button|nuke_world|`4Punish``|noflags|0|0|\nadd_spacer|small\nend_dialog|custom_view|Cancel||\n");
									p.CreatePacket(peer);
								}
							}
						}
						else if (cch.find("buttonClicked|nuke_world")!= string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->admin == 1) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								string reason = "";
								if (paa != worlds.end()) {
									World* world_ = &worlds[paa - worlds.begin()];
									if (not world_->nuked) {
										struct tm newtime;
										time_t now = time(0);
										localtime_s(&newtime, &now);
										world_->nuked = true;
										world_->n_t = "" + to_string(newtime.tm_mon + 1) + "/" + to_string(newtime.tm_mday) + "/2023 " + to_string(newtime.tm_hour) + ":" + (newtime.tm_min < 10 ? "0" + to_string(newtime.tm_min) + "" : "" + to_string(newtime.tm_min)) + ":" + to_string(newtime.tm_sec) + "";
										world_->n_b = (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``";
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("`o>> `4" + world_->name + " `4was nuked from orbit`o. It's the only way to be sure. Play nice, everybody!");
										p.Insert("OnTextOverlay"), p.Insert("World successfully nuked."), p.CreatePacket(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world and not pInfo(currentPeer)->mod and not pInfo(currentPeer)->admin) exit_(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/bigboom.wav\ndelayMS|0");
											p.CreatePacket(currentPeer);
										}
										add_modlogs(peer, (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name), "NUKED WORLD: `#" + pInfo(peer)->world, "");
									}
									else {
										world_->nuked = false, world_->n_t = "", world_->n_b = "";
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("This world is once again available to everyone!"), p.CreatePacket(peer);
										p.Insert("OnTextOverlay"), p.Insert("This world is once again available to everyone!"), p.CreatePacket(peer);
										add_modlogs(peer, (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name), "UNNUKED WORLD: `#" + pInfo(peer)->world, "");
									}
								}
							}
							break;
						}
						else if (cch.find("buttonClicked|remove_growconfirm") != string::npos) {
							if (pInfo(peer)->mod == 1 || pInfo(peer)->admin == 1) {
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSkip Grow Confirmation|left|32|\nadd_label|small|`oAre you sure you wanna skip all grow from `1" + pInfo(peer)->world + "`o?|\nadd_spacer|small|\nadd_textbox|`oThis will skip grow time for seeds and providers...|\nadd_spacer|small|\nadd_textbox|`4WARNING: `oYour actions are being saved, skipping grow by mistake will get you `4PERMANENTLY`o demoted!``|\nadd_button|remove_grow|`wSkip Grow|noflags|0|0|\nadd_spacer|small\nend_dialog|custom_view|Cancel||\n");
								p.CreatePacket(peer);
								string name_ = pInfo(peer)->world;
								vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (paa != worlds.end()) {
									World* world_ = &worlds[paa - worlds.begin()];
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
									data_.packetType = 19, data_.plantingTree = 500;
									data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
								}
							}
							break;
						}
						else if (cch.find("buttonClicked|remove_grow") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator paa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (paa != worlds.end()) {
								World* world_ = &worlds[paa - worlds.begin()];
								for (int i = 0; i < world_->blocks.size(); i++) {
									if (items[world_->blocks[i].fg].blockType == SEED) world_->blocks[i].planted = 0;
									if (items[world_->blocks[i].fg].blockType == PROVIDER) world_->blocks[i].planted = 0;
								}
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == pInfo(peer)->world) exit_(currentPeer);
								}
							}
							break;
						}
						if (cch.find("action|dialog_return\ndialog_name|dnaproc") != string::npos) {
							int DNAID;
							int remove = 0 - 1;
							int add = 1;
							int x_ = atoi(explode("\n", explode("tilex|", cch)[1])[0].c_str()), y_ = atoi(explode("\n", explode("tiley|", cch)[1])[0].c_str());
							std::stringstream ss(cch);
							std::string to;
							try {
								while (std::getline(ss, to, '\n')) {
									vector<string> infoDat = explode("|", to);
									if (infoDat.at(0) == "choose") {
										DNAID = atoi(infoDat.at(1).c_str());
										if (items[DNAID].name.find("Dino DNA Strand") != string::npos || items[DNAID].name.find("Plant DNA Strand") != string::npos || items[DNAID].name.find("Raptor DNA Strand") != string::npos) {
											if (pInfo(peer)->DNAInput == 0) {
												pInfo(peer)->IDDNA1 = DNAID;
												pInfo(peer)->DNAInput = 1;
												modify_inventory(peer, DNAID, remove);
												save_player(pInfo(peer), false);
												SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
											}
											else if (pInfo(peer)->DNAInput == 1) {
												pInfo(peer)->IDDNA2 = DNAID;
												pInfo(peer)->DNAInput = 2;
												modify_inventory(peer, DNAID, remove);
												save_player(pInfo(peer), false);
												SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
											}
											else if (pInfo(peer)->DNAInput == 2) {
												pInfo(peer)->IDDNA3 = DNAID;
												pInfo(peer)->DNAInput = 3;
												modify_inventory(peer, DNAID, remove);
												save_player(pInfo(peer), false);
												int DnaNumber1 = 0, DnaNumber2 = 0, DnaNumber3 = 0, What;
												ifstream infile("DnaRecipe.txt");
												for (string line; getline(infile, line);) {
													if (line.length() > 3 && line.at(0) != '/' && line.at(1) != '/') {
														auto ex = explode("|", line);
														int id1 = atoi(ex.at(0).c_str());
														int id2 = atoi(ex.at(1).c_str());
														int id3 = atoi(ex.at(2).c_str());
														if (id1 == pInfo(peer)->IDDNA1 && id2 == pInfo(peer)->IDDNA2 && id3 == pInfo(peer)->IDDNA3) {
															DnaNumber1 = atoi(ex.at(0).c_str());
															DnaNumber2 = atoi(ex.at(1).c_str());
															DnaNumber3 = atoi(ex.at(2).c_str());
															What = atoi(ex.at(3).c_str());
															break;
														}
													}
												}
												infile.close();
												if (pInfo(peer)->IDDNA1 == DnaNumber1 && pInfo(peer)->IDDNA2 == DnaNumber2 && pInfo(peer)->IDDNA3 == DnaNumber3 && DnaNumber3 != 0 && DnaNumber2 != 0 && DnaNumber1 != 0 && What != 0) {
													SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, false);
												}
												else {
													if (pInfo(peer)->DNAInput >= 1) {
														SendDNAProcessor(peer, x_, y_, false, true, false, 0, true, true);
													}
													else {
														SendDNAProcessor(peer, x_, y_, false, false, false, 0, true, true);
													}
												}
											}
										}
										else {
											if (pInfo(peer)->DNAInput >= 1) {
												SendDNAProcessor(peer, x_, y_, true, false, false, 0, true, false);
											}
											else {
												SendDNAProcessor(peer, x_, y_, true, false, false, 0, false, false);
											}
										}
									}
								}
							}
							catch (const std::out_of_range& e) {
								std::cout << e.what() << std::endl;
							}
							if (cch.find("buttonClicked|remove0") != string::npos) {
								if (pInfo(peer)->DNAInput == 1) {
									int DNARemoved = pInfo(peer)->IDDNA1;
									modify_inventory(peer, DNARemoved, add);
									save_player(pInfo(peer), false);
									pInfo(peer)->IDDNA1 = 0;
									pInfo(peer)->DNAInput = 0;
									save_player(pInfo(peer), false);
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, false, false);
								}
								if (pInfo(peer)->DNAInput == 2) {
									int DNARemoved = pInfo(peer)->IDDNA1;
									modify_inventory(peer, DNARemoved, add);
									save_player(pInfo(peer), false);
									pInfo(peer)->IDDNA1 = pInfo(peer)->IDDNA2;
									pInfo(peer)->IDDNA2 = 0;
									pInfo(peer)->DNAInput = 1;
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
								}
								if (pInfo(peer)->DNAInput == 3) {
									int DNARemoved = pInfo(peer)->IDDNA1;
									modify_inventory(peer, DNARemoved, add);
									save_player(pInfo(peer), false);
									pInfo(peer)->IDDNA1 = pInfo(peer)->IDDNA2;
									pInfo(peer)->IDDNA2 = pInfo(peer)->IDDNA3;
									pInfo(peer)->IDDNA3 = 0;
									pInfo(peer)->DNAInput = 2;
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
								}
							}
							if (cch.find("buttonClicked|remove1") != string::npos) {
								if (pInfo(peer)->DNAInput == 2) {
									int DNARemoved = pInfo(peer)->IDDNA2;
									modify_inventory(peer, DNARemoved, add);
									save_player(pInfo(peer), false);
									pInfo(peer)->IDDNA2 = 0;
									pInfo(peer)->DNAInput = 1;
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
								}
								if (pInfo(peer)->DNAInput == 3) {
									int DNARemoved = pInfo(peer)->IDDNA2;
									modify_inventory(peer, DNARemoved, add);
									pInfo(peer)->IDDNA2 = pInfo(peer)->IDDNA3;
									pInfo(peer)->IDDNA3 = 0;
									pInfo(peer)->DNAInput = 2;
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
								}
							}
							if (cch.find("buttonClicked|remove2") != string::npos) {
								if (pInfo(peer)->DNAInput == 3) {
									int DNARemoved = pInfo(peer)->IDDNA3;
									modify_inventory(peer, DNARemoved, add);
									pInfo(peer)->IDDNA3 = 0;
									pInfo(peer)->DNAInput = 2;
									SendDNAProcessor(peer, x_, y_, false, false, true, DNARemoved, true, false);
								}
							}
							if (cch.find("buttonClicked|combine") != string::npos) {
								if (pInfo(peer)->DNAInput == 3) {
									int DnaNumber1 = 0, DnaNumber2 = 0, DnaNumber3 = 0, What;
									ifstream infile("DnaRecipe.txt");
									for (string line; getline(infile, line);) {
										if (line.length() > 3 && line.at(0) != '/' && line.at(1) != '/') {
											auto ex = explode("|", line);
											int id1 = atoi(ex.at(0).c_str());
											int id2 = atoi(ex.at(1).c_str());
											int id3 = atoi(ex.at(2).c_str());
											if (id1 == pInfo(peer)->IDDNA1 && id2 == pInfo(peer)->IDDNA2 && id3 == pInfo(peer)->IDDNA3) {
												DnaNumber1 = atoi(ex.at(0).c_str());
												DnaNumber2 = atoi(ex.at(1).c_str());
												DnaNumber3 = atoi(ex.at(2).c_str());
												What = atoi(ex.at(3).c_str());
												break;
											}
										}
									}
									infile.close();
									if (pInfo(peer)->IDDNA1 == DnaNumber1 && pInfo(peer)->IDDNA2 == DnaNumber2 && pInfo(peer)->IDDNA3 == DnaNumber3 && DnaNumber3 != 0 && DnaNumber2 != 0 && DnaNumber1 != 0 && What != 0) {
										int count = items[What].blockType == BlockTypes::FOREGROUND ? 10 : 5;
										modify_inventory(peer, What, count);
										if (items[What].clothType == ClothTypes::FEET) pInfo(peer)->feet = What;
										else if (items[What].clothType == ClothTypes::HAND) pInfo(peer)->hand = What;
										pInfo(peer)->IDDNA1 = 0; pInfo(peer)->IDDNA2 = 0; pInfo(peer)->IDDNA3 = 0; pInfo(peer)->DNAInput = 0;
										if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 15 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress++;
											if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
												pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
										gamepacket_t p, p2;
										p.Insert("OnConsoleMessage"), p.Insert("DNA Processing complete! The DNA combined into a `2" + items[What].name + "``!"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("DNA Processing complete! The DNA combined into a `2" + items[What].name + "``!"), p2.Insert(0), p2.Insert(0), p2.CreatePacket(peer);
										update_clothes(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world) {
												{
													gamepacket_t p;
													p.Insert("OnParticleEffect"); p.Insert(44); p.Insert((float)x_ * 32 + 16, (float)y_ * 32 + 16); p.CreatePacket(currentPeer);
												}
												{
													PlayerMoving data_{};
													data_.packetType = 19, data_.plantingTree = 150, data_.netID = pInfo(peer)->netID;
													data_.punchX = What, data_.punchY = What;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 3;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[]raw;
												}
											}
										}
									}
									else {
										SendDNAProcessor(peer, x_, y_, false, false, false, 0, true, true);
									}
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|news_gacor") != string::npos) {
							string button = "";
							TextScanner parser(cch);
							if (!parser.try_get("buttonClicked", button)) break;

							if (button == "rules_bro") {
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wHelp & Rules``|left|18|\nadd_spacer|small|\nadd_textbox|`w`2World or Accounts`` may be deleted after long periods of inactivity if a world is not visited regularly. `2Item function and prices are subject to change`` at anytime and without warning as we are constantly trying to balance different aspects of the game.``|left|\nadd_spacer|small|\nadd_textbox|`wProtect your worlds and the items by using Doors, Locks, and Blocks wisely, `2never share your login credentials`` with anyone, and be aware of `2fake system messages and phishing websites``.``|left|\nadd_spacer|small|\nadd_textbox|`w`2Inappropriate behavior`` (profanity, racist, sexist or sexual content, abusive behavior, and bullying); any form of scam game such as Drop Game, Dirt Game, etc.; `2buying, selling, or trading TayoPs accounts`` for real money; `2purchase fraud``; and usage of `2third party software``, including, but not limited to, client-side manipulation, speed hacks, clock manipulation, bots, will be sanctioned and may result in a permanent suspension.``|left|\nadd_spacer|small|\nadd_textbox|`wTo report a world, type `2/report`` and exit the world immediately. To report a player, contact us directly. For any issues and questions you may have, please visit our discord server`` `2https://discord.gg/Hz3Kq5xCs8``|left|\nadd_spacer|small|\nadd_textbox|`wAdvertising another `2Private servers or any social medias`` is `4not allowed``.|left|\nadd_spacer|small|\nadd_textbox|`wThere is account creation limit,creating mass account might get you `4banned``.Punishment might applied to you alternative accounts or account that have the same ip address.|left|\nadd_spacer|small|\nadd_textbox|`wAny kind of abusing behaviour is not tolerated such as abusing commands, bugs or glitches will result a `4permanent`` suspension.|left|\nadd_spacer|small|\nadd_textbox|`wPlayers that faking `2System`` messages will be punished.Stolen/Scammed item will never be restored.|left|\nadd_spacer|small|\nadd_textbox|`wBugs and Glitches are bound to happen.if you lost item/rollback it will always happen we will do anything in our power to prevent this sort thing to happen.|left|\nadd_spacer|small|\nadd_button|news|`wI accept these rules!``|noflags|0|0|end_dialog|generic|||");
								p.CreatePacket(peer);
								break;
							}
							if (button == "feature_bang") {
								news(peer, false);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dnaget") != string::npos) {
							TextScanner parser(cch);
							int x_ = 0, y_ = 0;
							int item = 0;
							if (!parser.try_get("tilex", x_)) break;
							if (!parser.try_get("tiley", y_)) break;
							if (!parser.try_get("item", item)) break;
							int remove = -1;
							if (modify_inventory(peer, item, remove) == 0) {
								int Random = rand() % 100, reward = 0, count = 1;
								vector<int> list{ 4082,4084,4086,4088,4090,4092,4120,4122,5488 };
								gamepacket_t p, p2;
								p.Insert("OnTalkBubble"), p2.Insert("OnConsoleMessage"); p.Insert(pInfo(peer)->netID);
								if (Random >= 4 and Random <= 10) {
									reward = list[rand() % list.size()];
									p.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``"), p2.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``");
									modify_inventory(peer, reward, count);
								}
								else if (Random >= 1 and Random <= 3) {
									gamepacket_t a;
									a.Insert("OnConsoleMessage");
									a.Insert("Wow! You discovered the missing link between cave-rayman and the modern Growtopian.");
									reward = 5488;
									p.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``"), p2.Insert("You ground up a " + items[item].name + ", `9and found " + items[reward].name + " inside!``");
									modify_inventory(peer, reward, count);
									a.CreatePacket(peer);
								}
								else {
									p.Insert("You ground up a " + items[item].name + ", `3but any DNA inside was destroyed in the process.``"), p2.Insert("You ground up a " + items[item].name + ", `3but any DNA inside was destroyed in the process.``");
								}
								p.Insert(0), p.Insert(0);
								p2.CreatePacket(peer), p.CreatePacket(peer);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->world == pInfo(peer)->world) {
										if (reward != 0) {
											packet_(currentPeer, "action|play_sfx\nfile|audio/bell.wav\ndelayMS|0");
											PlayerMoving data_{};
											data_.packetType = 19, data_.plantingTree = 150, data_.netID = pInfo(peer)->netID;
											data_.punchX = reward, data_.punchY = reward;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 3;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[]raw;
										}
										else {
											packet_(currentPeer, "action|play_sfx\nfile|audio/ch_start.wav\ndelayMS|0");
										}
									}
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|artifact_upgrade") != string::npos) {
							/*
							5104|Celestial Kaleidoscope
							5106|Harmonic Chimes
							5204|Plasma Globe
							5070|Crystallized Reality
							5072|Crystallized Wealth
							5074|Crystallized Brilliance
							5076|Crystallized Nature|999
							*/
							// Wisdom
							if (cch.find("buttonClicked|upgrade-5126") != string::npos) {
								SendArtifactUpgrade(peer, 5078, 1, 5126, 5104, 1, 5074, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5128") != string::npos) {
								SendArtifactUpgrade(peer, 5126, 1, 5128, 5106, 1, 5074, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5130") != string::npos) {
								SendArtifactUpgrade(peer, 5128, 2, 5130, 5204, 1, 5074, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5132") != string::npos) {
								SendArtifactUpgrade(peer, 5130, 2, 5132, 5104, 2, 5074, 2);
							}
							else if (cch.find("buttonClicked|upgrade-5134") != string::npos) {
								SendArtifactUpgrade(peer, 5132, 3, 5134, 5106, 3, 5074, 2);
							}
							// Tesseract
							else if (cch.find("buttonClicked|upgrade-5144") != string::npos) {
								SendArtifactUpgrade(peer, 5080, 1, 5144, 5204, 1, 5070, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5146") != string::npos) {
								SendArtifactUpgrade(peer, 5144, 1, 5146, 5104, 1, 5070, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5148") != string::npos) {
								SendArtifactUpgrade(peer, 5146, 2, 5148, 5106, 1, 5070, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5150") != string::npos) {
								SendArtifactUpgrade(peer, 5148, 2, 5150, 5204, 2, 5070, 2);
							}
							else if (cch.find("buttonClicked|upgrade-5152") != string::npos) {
								SendArtifactUpgrade(peer, 5150, 2, 5152, 5104, 3, 5070, 2);
							}
							// Seed of Life
							else if (cch.find("buttonClicked|upgrade-5162") != string::npos) {
								SendArtifactUpgrade(peer, 5082, 1, 5162, 5106, 1, 5076, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5164") != string::npos) {
								SendArtifactUpgrade(peer, 5162, 1, 5164, 5204, 1, 5076, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5166") != string::npos) {
								SendArtifactUpgrade(peer, 5164, 2, 5166, 5104, 1, 5076, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5168") != string::npos) {
								SendArtifactUpgrade(peer, 5166, 2, 5168, 5106, 2, 5076, 2);
							}
							else if (cch.find("buttonClicked|upgrade-5170") != string::npos) {
								SendArtifactUpgrade(peer, 5168, 2, 5170, 5204, 3, 5076, 2);
							}
							// Riches
							else if (cch.find("buttonClicked|upgrade-5180") != string::npos) {
								SendArtifactUpgrade(peer, 5084, 1, 5180, 5104, 2, 5072, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5182") != string::npos) {
								SendArtifactUpgrade(peer, 5180, 1, 5182, 5204, 2, 5072, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5184") != string::npos) {
								SendArtifactUpgrade(peer, 5182, 2, 5184, 5106, 2, 5072, 1);
							}
							else if (cch.find("buttonClicked|upgrade-5186") != string::npos) {
								SendArtifactUpgrade(peer, 5184, 2, 5186, 5104, 3, 5072, 2);
							}
							else if (cch.find("buttonClicked|upgrade-5188") != string::npos) {
								SendArtifactUpgrade(peer, 5186, 2, 5188, 5204, 3, 5072, 2);
							}
							// Orb
							else if (cch.find("buttonClicked|upgrade-7168") != string::npos) {
								SendArtifactUpgrade(peer, 7166, 1, 7168, 5106, 2, 7186, 1);
							}
							else if (cch.find("buttonClicked|upgrade-7170") != string::npos) {
								SendArtifactUpgrade(peer, 7168, 1, 7170, 5104, 1, 7186, 1);
							}
							else if (cch.find("buttonClicked|upgrade-7172") != string::npos) {
								SendArtifactUpgrade(peer, 7170, 2, 7172, 5204, 1, 7186, 1);
							}
							else if (cch.find("buttonClicked|upgrade-7174") != string::npos) {
								SendArtifactUpgrade(peer, 7172, 2, 7174, 5106, 2, 7186, 2);
							}
							else if (cch.find("buttonClicked|upgrade-9212") != string::npos) {
								SendArtifactUpgrade(peer, 7174, 2, 9212, 5104, 3, 7186, 2);
							}
							// Upgrading
							if (cch.find("buttonClicked|completecraft-" + to_string(pInfo(peer)->Upgradeto) + "") != string::npos) {
								int SoulStone = 0, Crystalized = 0, Celestial = 0, Riddles = 0, HaveAnces = 0;
								modify_inventory(peer, 5202, SoulStone);
								modify_inventory(peer, pInfo(peer)->IDCrystalized, Crystalized);
								modify_inventory(peer, pInfo(peer)->IDCeles, Celestial);
								modify_inventory(peer, pInfo(peer)->DailyRiddles, Riddles);
								modify_inventory(peer, pInfo(peer)->AncesID, HaveAnces);
								if (SoulStone >= pInfo(peer)->HowmuchSoulStone && Crystalized >= pInfo(peer)->JumlahCrystalized && Celestial >= pInfo(peer)->JumlahCeles && Riddles >= 5 && HaveAnces >= 1) {
									int del_a = -pInfo(peer)->HowmuchSoulStone, del_b = -pInfo(peer)->JumlahCrystalized, del_c = -pInfo(peer)->JumlahCeles, del_d = -5, del_e = -1;
									modify_inventory(peer, 5202, del_a);
									modify_inventory(peer, pInfo(peer)->IDCrystalized, del_b);
									modify_inventory(peer, pInfo(peer)->IDCeles, del_c);
									modify_inventory(peer, pInfo(peer)->DailyRiddles, del_d);
									modify_inventory(peer, pInfo(peer)->AncesID, del_e);
									/*if (pInfo(peer)->Upgradeto == 7174 || pInfo(peer)->Upgradeto == 5186 || pInfo(peer)->Upgradeto == 5150 || pInfo(peer)->Upgradeto == 5168 || pInfo(peer)->Upgradeto == 5132) {
										SendSuccesAchievement(peer, "Ancestral Being", false, "Ancestral Being", 137, 1);
									}*/
									if (pInfo(peer)->ances == pInfo(peer)->AncesID) {
										pInfo(peer)->ances = 0;
										update_clothes(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED || currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world) {
												packet_(currentPeer, "action|play_sfx\nfile|audio/change_clothes.wav\ndelayMS|0");
											}
										}
									}
									int c_ = 1;
									if (modify_inventory(peer, pInfo(peer)->Upgradeto, c_) == 0) {
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("add_label_with_icon|big|`9Ancient Goddess``|left|5086|\nadd_spacer|small|\nadd_textbox|`8You've pleased me, clever one.``|left|\nadd_spacer|small|\nend_dialog|artifact_upgrade|Return|");
										p.CreatePacket(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("add_label_with_icon|big|`9Ancient Goddess``|left|5086|\nadd_spacer|small|\nadd_textbox|`8You didn't have enough inventory.``|left|\nadd_spacer|small|\nend_dialog|artifact_upgrade|Return|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_minokawa_wings") != string::npos) {
							bool Minokawa_1 = atoi(explode("\n", explode("checkbox_minokawa_wings|", cch)[1])[0].c_str()), Minokawa_2 = atoi(explode("\n", explode("checkbox_minokawa_pet|", cch)[1])[0].c_str());
							if (Minokawa_1) pInfo(peer)->MKW = true;
							else pInfo(peer)->MKW = false;
							if (Minokawa_2) pInfo(peer)->MKP = true;
							else pInfo(peer)->MKP = false;
							update_clothes(peer);
							break;
						}
						if (cch.find("action|dialog_return\ndialog_name|billboard_edit\nbillboard_toggle|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int billboard_active = atoi(explode("\n", t_[3])[0].c_str());
							int billboard_price = atoi(explode("\n", t_[4])[0].c_str());
							int peritem = atoi(explode("\n", t_[5])[0].c_str());
							int perlock = atoi(explode("\n", t_[6])[0].c_str());
							bool update_billboard = true;
							if (peritem == perlock or peritem == 0 and perlock == 0 or peritem == 1 and perlock == 1) {
								update_billboard = false;
								gamepacket_t p, p2;
								p.Insert("OnConsoleMessage"), p.Insert("You need to pick one pricing method - 'locks per item' or 'items per lock'"), p.CreatePacket(peer);
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("You need to pick one pricing method - 'locks per item' or 'items per lock'"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							}
							else {
								if (peritem == 1) pInfo(peer)->b_w = 1;
								if (perlock == 1) pInfo(peer)->b_w = 0;
							}
							if (billboard_active == 1)pInfo(peer)->b_a = 1;
							else pInfo(peer)->b_a = 0;
							if (billboard_price < 0 or billboard_price > 99999) {
								update_billboard = false;
								gamepacket_t p, p2;
								p.Insert("OnConsoleMessage"), p.Insert("Price can't be negative. That's beyond science."), p.CreatePacket(peer);
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Price can't be negative. That's beyond science."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							}
							else pInfo(peer)->b_p = billboard_price;
							if (update_billboard && pInfo(peer)->b_p != 0 && pInfo(peer)->b_i != 0) {
								gamepacket_t p(0, pInfo(peer)->netID);
								p.Insert("OnBillboardChange"), p.Insert(pInfo(peer)->netID), p.Insert(pInfo(peer)->b_i), p.Insert(pInfo(peer)->b_a), p.Insert(pInfo(peer)->b_p), p.Insert(pInfo(peer)->b_w);
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
									p.CreatePacket(currentPeer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|billboard_edit\nbillboard_item|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int billboard_item = atoi(explode("\n", t_[3])[0].c_str());
							if (billboard_item > 0 && billboard_item < items.size()) {
								int got = 0;
								modify_inventory(peer, billboard_item, got);
								if (got == 0) break;
								if (items[billboard_item].untradeable == 1 or billboard_item == 1424 or items[billboard_item].blockType == BlockTypes::LOCK or items[billboard_item].blockType == BlockTypes::FISH) {
									gamepacket_t p, p2;
									p.Insert("OnConsoleMessage"), p.Insert("Item can not be untradeable."), p.CreatePacket(peer);
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Item can not be untradeable."), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
								}
								else {
									pInfo(peer)->b_i = billboard_item;
									if (pInfo(peer)->b_p != 0 && pInfo(peer)->b_i != 0) {
										gamepacket_t p(0, pInfo(peer)->netID);
										p.Insert("OnBillboardChange"), p.Insert(pInfo(peer)->netID), p.Insert(pInfo(peer)->b_i), p.Insert(pInfo(peer)->b_a), p.Insert(pInfo(peer)->b_p), p.Insert(pInfo(peer)->b_w);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											p.CreatePacket(currentPeer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|void_ray_settings") != string::npos) {
							if (pInfo(peer)->hand == 8532) {
								pInfo(peer)->void_ray_1hit = atoi(explode("\n", explode("checkbox_void_ray|", cch)[1])[0].c_str()) == 1 ? true : false;
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert(pInfo(peer)->void_ray_1hit ? "You've enabled 1hit mode for Void Dragon Fist" : "You've disabled 1hit mode for Void Dragon Fist");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|sharingan_settings") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|ruler_sword_settings") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|sharingan_settings") != string::npos) {
								if (pInfo(peer)->face == 13554) {
									pInfo(peer)->sharingan_control = atoi(explode("\n", explode("checkbox_s_control|", cch)[1])[0].c_str()) == 1 ? true : false;
									if (pInfo(peer)->sharingan_control && (pInfo(peer)->guild_id != 1 || not pInfo(peer)->dev)) {
										pInfo(peer)->sharingan_control = false;
										Algorithm::send_console(peer, "Only Otsutsukis can use this jutsu. ");
									}
									pInfo(peer)->izanagi = atoi(explode("\n", explode("checkbox_izanagi|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->Cheat_Speed = atoi(explode("\n", explode("checkbox_speed|", cch)[1])[0].c_str()) == 1 ? true : false;
									if (pInfo(peer)->face == 13554 and (pInfo(peer)->dev or pInfo(peer)->guild_id == 1 or pInfo(peer)->face == 13896))
										pInfo(peer)->knockback = atoi(explode("\n", explode("checkbox_izanami|", cch)[1])[0].c_str()) == 1 ? true : false;

									pInfo(peer)->spotlight = pInfo(peer)->izanagi;

									update_clothes(peer);
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("Settings updated.");
									p.CreatePacket(peer);
								}
							}
							else if (cch.find("action|dialog_return\ndialog_name|kamish_word") != string::npos) {
								if ((pInfo(peer)->hand == 9508) and pInfo(peer)->dev)
								{
									pInfo(peer)->kill_mode = atoi(explode("\n", explode("checkbox_killers|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->kamish = atoi(explode("\n", explode("checkbox_far|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->punched = pInfo(peer)->kill_mode ? 13 : 0;
									if (pInfo(peer)->kamish == true) {
										int mish = 9508;
										items[mish].place_far = 20, items[mish].punch_far = 20;
									}
									update_clothes(peer);
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("Settings updated.");
									p.CreatePacket(peer);
								}
								break;
							}
							if (cch.find("action|dialog_return\ndialog_name|ruler_sword_settings") != string::npos) {
								if ((pInfo(peer)->face == 13896) and pInfo(peer)->dev)
								{
									pInfo(peer)->kill_mode = atoi(explode("\n", explode("checkbox_killer|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->genjutsu = atoi(explode("\n", explode("checkbox_genjutsu|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->wide_place = atoi(explode("\n", explode("checkbox_wide_place|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->wide_punch = atoi(explode("\n", explode("checkbox_wide_punch|", cch)[1])[0].c_str()) == 1 ? true : false;
									pInfo(peer)->genjutsu2 = atoi(explode("\n", explode("checkbox_genjutsu2|", cch)[1])[0].c_str()) == 1 ? true : false;
									//if (pInfo(peer)->face == 13554 and pInfo(peer)->hand == 13896) pInfo(peer)->knockback = atoi(explode("\n", explode("checkbox_izanami|", cch)[1])[0].c_str()) == 1 ? true : false;
									//if (pInfo(peer)->izanagi) pInfo(peer)->spotlight = true;
									pInfo(peer)->punched = pInfo(peer)->kill_mode ? 13 : 0;
									if (pInfo(peer)->genjutsu2) {
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->dev >= 1) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world and pInfo(currentPeer)->tankIDName != pInfo(peer)->tankIDName) {
												pInfo(currentPeer)->got_tsukoyomi = true;
												PlayMods new_playmod{};
												new_playmod.id = 3;
												new_playmod.time = time(nullptr) + 86400;
												new_playmod.user = pInfo(currentPeer)->tankIDName;
												pInfo(currentPeer)->playmods.push_back(new_playmod);
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You've caught in Tsukoyomi dream! (`$Tsukoyomi`` mod added, `$permanent time ``left)");
												p.CreatePacket(currentPeer);
												pInfo(currentPeer)->face = 13610;
												update_clothes(currentPeer);
											}
										}
									}
									else {
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or not pInfo(currentPeer)->got_tsukoyomi or pInfo(currentPeer)->dev >= 1) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world and pInfo(currentPeer)->tankIDName != pInfo(peer)->tankIDName) {
												for (int i = 0; i < pInfo(currentPeer)->playmods.size(); i++) 
													if (pInfo(currentPeer)->playmods[i].id == 3)
														pInfo(currentPeer)->playmods[i].time = 0;
												pInfo(currentPeer)->got_tsukoyomi = false;
												
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You've gone from Tsukoyomi dream! (`$Tsukoyomi`` mod removed)");
												p.CreatePacket(currentPeer);
												pInfo(currentPeer)->face = 0;
												update_clothes(currentPeer);
											}
										}
									}
									if (pInfo(peer)->genjutsu) {
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->admin or currentPeer == peer or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											add_curse(currentPeer, 300, "genjutsu", "someone");
										}
									}

									update_clothes(peer);
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("Settings updated.");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						
						else if (cch.find("action|dialog_return\ndialog_name|dialog_rift_cape") != string::npos) {
							if (cch.find("buttonClicked|button_manual") != string::npos) {
								SendDialogRiftCape(peer, true);
								break;
							}
							else if (cch.find("buttonClicked|restore_default") != string::npos) {
								// Rift Cape
								pInfo(peer)->Time_Change = true;
								pInfo(peer)->Cycle_Time = 30;
								// Cape 1
								pInfo(peer)->Cape_R_0 = 147, pInfo(peer)->Cape_G_0 = 56, pInfo(peer)->Cape_B_0 = 143, pInfo(peer)->Collar_R_0 = 147, pInfo(peer)->Collar_G_0 = 56, pInfo(peer)->Collar_B_0 = 143;
								pInfo(peer)->Cape_Collar_0 = true, pInfo(peer)->Closed_Cape_0 = false, pInfo(peer)->Open_On_Move_0 = true, pInfo(peer)->Aura_0 = true, pInfo(peer)->Aura_1st_0 = false, pInfo(peer)->Aura_2nd_0 = false, pInfo(peer)->Aura_3rd_0 = true;
								// Cape 2
								pInfo(peer)->Cape_R_1 = 137, pInfo(peer)->Cape_G_1 = 30, pInfo(peer)->Cape_B_1 = 43, pInfo(peer)->Collar_R_1 = 34, pInfo(peer)->Collar_G_1 = 35, pInfo(peer)->Collar_B_1 = 63;
								pInfo(peer)->Cape_Collar_1 = true, pInfo(peer)->Closed_Cape_1 = true, pInfo(peer)->Open_On_Move_1 = false, pInfo(peer)->Aura_1 = true, pInfo(peer)->Aura_1st_1 = false, pInfo(peer)->Aura_2nd_1 = true, pInfo(peer)->Aura_3rd_1 = false;
								// Total
								pInfo(peer)->Cape_Value = 1952541691, pInfo(peer)->Cape_Collor_0_Value = 2402849791, pInfo(peer)->Cape_Collar_0_Value = 2402849791, pInfo(peer)->Cape_Collor_1_Value = 723421695, pInfo(peer)->Cape_Collar_1_Value = 1059267327;
								// End
								update_clothes(peer);
								break;
							}
							else {
								try {
									pInfo(peer)->Time_Change = atoi(explode("\n", explode("checkbox_time_cycle|", cch)[1])[0].c_str()) == 1 ? true : false;
									if (!is_number(explode("\n", explode("text_input_time_cycle|", cch)[1])[0])) break;
									if (atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str()) < 1) break;
									pInfo(peer)->Cycle_Time = atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str());
									{ // Cape_1
										pInfo(peer)->Cape_Collar_0 = atoi(explode("\n", explode("checkbox_cape_collar0|", cch)[1])[0].c_str()) == 1 ? true : false;
										{
											auto Cape_Color0 = explode(",", explode("\n", explode("text_input_cape_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_cape_color0|", cch)[1])[0].c_str());
											if (Cape_Color0.size() != 3 || t_.size() < 2 || Cape_Color0[0] == "" || Cape_Color0[1] == "" || Cape_Color0[2] == "" || Cape_Color0[0].empty() || Cape_Color0[1].empty() || Cape_Color0[2].empty()) {
												SendDialogRiftCape(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Cape_Color0[0]) || !is_number(Cape_Color0[1]) || !is_number(Cape_Color0[2]) || atoi(Cape_Color0[0].c_str()) > 255 || atoi(Cape_Color0[1].c_str()) > 255 || atoi(Cape_Color0[2].c_str()) > 255 || atoi(Cape_Color0[0].c_str()) < 0 || atoi(Cape_Color0[1].c_str()) < 0 || atoi(Cape_Color0[2].c_str()) < 0) {
												SendDialogRiftCape(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Cape_R_0 = atoi(Cape_Color0.at(0).c_str());
											pInfo(peer)->Cape_G_0 = atoi(Cape_Color0.at(1).c_str());
											pInfo(peer)->Cape_B_0 = atoi(Cape_Color0.at(2).c_str());
											pInfo(peer)->Cape_Collor_0_Value = 255 + (((256 * atoi(Cape_Color0.at(0).c_str())) + atoi(Cape_Color0.at(1).c_str()) * 65536) + atoi(Cape_Color0.at(2).c_str()) * (long long int)16777216);
										}
										{
											auto Cape_Collar_Color0 = explode(",", explode("\n", explode("text_input_collar_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_collar_color0|", cch)[1])[0].c_str());
											if (Cape_Collar_Color0.size() != 3 || t_.size() < 2 || Cape_Collar_Color0[0] == "" || Cape_Collar_Color0[1] == "" || Cape_Collar_Color0[2] == "" || Cape_Collar_Color0[0].empty() || Cape_Collar_Color0[1].empty() || Cape_Collar_Color0[2].empty()) {
												SendDialogRiftCape(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Cape_Collar_Color0[0]) || !is_number(Cape_Collar_Color0[1]) || !is_number(Cape_Collar_Color0[2]) || atoi(Cape_Collar_Color0[0].c_str()) > 255 || atoi(Cape_Collar_Color0[1].c_str()) > 255 || atoi(Cape_Collar_Color0[2].c_str()) > 255 || atoi(Cape_Collar_Color0[0].c_str()) < 0 || atoi(Cape_Collar_Color0[1].c_str()) < 0 || atoi(Cape_Collar_Color0[2].c_str()) < 0) {
												SendDialogRiftCape(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Collar_R_0 = atoi(Cape_Collar_Color0.at(0).c_str());
											pInfo(peer)->Collar_G_0 = atoi(Cape_Collar_Color0.at(1).c_str());
											pInfo(peer)->Collar_B_0 = atoi(Cape_Collar_Color0.at(2).c_str());
											pInfo(peer)->Cape_Collar_0_Value = 255 + (((256 * atoi(Cape_Collar_Color0.at(0).c_str())) + atoi(Cape_Collar_Color0.at(1).c_str()) * 65536) + atoi(Cape_Collar_Color0.at(2).c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Closed_Cape_0 = atoi(explode("\n", explode("checkbox_closed_cape0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Open_On_Move_0 = atoi(explode("\n", explode("checkbox_open_on_move0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_0 = atoi(explode("\n", explode("checkbox_aura0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_1st_0 = atoi(explode("\n", explode("checkbox_aura_1st0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_2nd_0 = atoi(explode("\n", explode("checkbox_aura_2nd0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_3rd_0 = atoi(explode("\n", explode("checkbox_aura_3rd0|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
									{ // Cape_2
										pInfo(peer)->Cape_Collar_1 = atoi(explode("\n", explode("checkbox_cape_collar1|", cch)[1])[0].c_str()) == 1 ? true : false;
										{
											auto Cape_Color1 = explode(",", explode("\n", explode("text_input_cape_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_cape_color1|", cch)[1])[0].c_str());
											if (Cape_Color1.size() != 3 || t_.size() < 2 || Cape_Color1[0] == "" || Cape_Color1[1] == "" || Cape_Color1[2] == "" || Cape_Color1[0].empty() || Cape_Color1[1].empty() || Cape_Color1[2].empty()) {
												SendDialogRiftCape(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Cape_Color1[0]) || !is_number(Cape_Color1[1]) || !is_number(Cape_Color1[2]) || atoi(Cape_Color1[0].c_str()) > 255 || atoi(Cape_Color1[1].c_str()) > 255 || atoi(Cape_Color1[2].c_str()) > 255 || atoi(Cape_Color1[0].c_str()) < 0 || atoi(Cape_Color1[1].c_str()) < 0 || atoi(Cape_Color1[2].c_str()) < 0) {
												SendDialogRiftCape(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Cape_R_1 = atoi(Cape_Color1.at(0).c_str());
											pInfo(peer)->Cape_G_1 = atoi(Cape_Color1.at(1).c_str());
											pInfo(peer)->Cape_B_1 = atoi(Cape_Color1.at(2).c_str());
											pInfo(peer)->Cape_Collor_1_Value = 255 + (((256 * atoi(Cape_Color1.at(0).c_str())) + atoi(Cape_Color1.at(1).c_str()) * 65536) + atoi(Cape_Color1.at(2).c_str()) * (long long int)16777216);
										}
										{
											auto Cape_Collar_Color1 = explode(",", explode("\n", explode("text_input_collar_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_collar_color1|", cch)[1])[0].c_str());
											if (Cape_Collar_Color1.size() != 3 || t_.size() < 2 || Cape_Collar_Color1[0] == "" || Cape_Collar_Color1[1] == "" || Cape_Collar_Color1[2] == "" || Cape_Collar_Color1[0].empty() || Cape_Collar_Color1[1].empty() || Cape_Collar_Color1[2].empty()) {
												SendDialogRiftCape(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Cape_Collar_Color1[0]) || !is_number(Cape_Collar_Color1[1]) || !is_number(Cape_Collar_Color1[2]) || atoi(Cape_Collar_Color1[0].c_str()) > 255 || atoi(Cape_Collar_Color1[1].c_str()) > 255 || atoi(Cape_Collar_Color1[2].c_str()) > 255 || atoi(Cape_Collar_Color1[0].c_str()) < 0 || atoi(Cape_Collar_Color1[1].c_str()) < 0 || atoi(Cape_Collar_Color1[2].c_str()) < 0) {
												SendDialogRiftCape(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Collar_R_1 = atoi(Cape_Collar_Color1.at(0).c_str());
											pInfo(peer)->Collar_G_1 = atoi(Cape_Collar_Color1.at(1).c_str());
											pInfo(peer)->Collar_B_1 = atoi(Cape_Collar_Color1.at(2).c_str());
											pInfo(peer)->Cape_Collar_1_Value = 255 + (((256 * atoi(Cape_Collar_Color1.at(0).c_str())) + atoi(Cape_Collar_Color1.at(1).c_str()) * 65536) + atoi(Cape_Collar_Color1.at(2).c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Closed_Cape_1 = atoi(explode("\n", explode("checkbox_closed_cape1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Open_On_Move_1 = atoi(explode("\n", explode("checkbox_open_on_move1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_1 = atoi(explode("\n", explode("checkbox_aura1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_1st_1 = atoi(explode("\n", explode("checkbox_aura_1st1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_2nd_1 = atoi(explode("\n", explode("checkbox_aura_2nd1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Aura_3rd_1 = atoi(explode("\n", explode("checkbox_aura_3rd1|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
								}
								catch (...) {
									break;
								}
								int TValue = 0;
								if (pInfo(peer)->Time_Change) TValue += 4096;
								if (pInfo(peer)->Cape_Collar_0) TValue += 1;
								if (pInfo(peer)->Cape_Collar_1) TValue += 2;
								if (pInfo(peer)->Closed_Cape_0) TValue += 4;
								if (pInfo(peer)->Closed_Cape_1) TValue += 8;
								if (pInfo(peer)->Open_On_Move_0) TValue += 16;
								if (pInfo(peer)->Open_On_Move_1) TValue += 32;
								if (pInfo(peer)->Aura_0) TValue += 64;
								if (pInfo(peer)->Aura_1) TValue += 128;
								if (pInfo(peer)->Aura_1st_0) TValue += 256;
								if (pInfo(peer)->Aura_1st_1) TValue += 1024;
								if (pInfo(peer)->Aura_2nd_0) TValue += 256 * 2;
								if (pInfo(peer)->Aura_2nd_1) TValue += 1024 * 2;
								if (pInfo(peer)->Aura_3rd_0) TValue += 256 * 3;
								if (pInfo(peer)->Aura_3rd_1) TValue += 1024 * 3;
								pInfo(peer)->Cape_Value = TValue;
								update_clothes(peer);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_cernuous_mask") != string::npos) {
							if (cch.find("buttonClicked|restore_default") != string::npos) {
								pInfo(peer)->Aura_Season = 2;
								pInfo(peer)->Trail_Season = 2;
								update_clothes(peer);
								break;
							}
							else {
								pInfo(peer)->Aura_Season = (atoi(explode("\n", explode("checkbox_none0|", cch)[1])[0].c_str()) == 1 ? 0 : (atoi(explode("\n", explode("checkbox_spring0|", cch)[1])[0].c_str()) == 1 ? 1 : (atoi(explode("\n", explode("checkbox_summer0|", cch)[1])[0].c_str()) == 1 ? 2 : (atoi(explode("\n", explode("checkbox_autumn0|", cch)[1])[0].c_str()) == 1 ? 3 : (atoi(explode("\n", explode("checkbox_winter0|", cch)[1])[0].c_str()) == 1 ? 4 : 0)))));
								pInfo(peer)->Trail_Season = (atoi(explode("\n", explode("checkbox_none1|", cch)[1])[0].c_str()) == 1 ? 0 : (atoi(explode("\n", explode("checkbox_spring1|", cch)[1])[0].c_str()) == 1 ? 1 : (atoi(explode("\n", explode("checkbox_summer1|", cch)[1])[0].c_str()) == 1 ? 2 : (atoi(explode("\n", explode("checkbox_autumn1|", cch)[1])[0].c_str()) == 1 ? 3 : (atoi(explode("\n", explode("checkbox_winter1|", cch)[1])[0].c_str()) == 1 ? 4 : 0)))));
								update_clothes(peer);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|spotify") != string::npos) {
							if (cch.find("\nbuttonClicked|song*") != string::npos) {
								string songid = explode("\n", explode("*", cch)[1])[0].c_str();
								if (!is_number(songid)) break;
								if (stoi(songid) < 0 or stoi(songid) > musics.size()) break;
								playsong(peer, stoi(songid));
							}
							else if (cch.find("\nbuttonClicked|stopsong") != string::npos) {
								stopsong(peer);
							}
							else if (cch.find("\nbuttonClicked|settings") != string::npos) {
								spotifysettings(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|asspotifysettings") != string::npos) {
							spotify(peer);
							if (cch.find("shuffle|") != string::npos) {
								bool shuffle = atoi(explode("\n", explode("shuffle|", cch)[1])[0].c_str());
								pInfo(peer)->shuffle = shuffle;
							}
							if (cch.find("loop_song|") != string::npos) {
								bool shuffle = atoi(explode("\n", explode("loop_song|", cch)[1])[0].c_str());
								pInfo(peer)->loopsong = shuffle;
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|ticket_booth") != string::npos) {
							if (cch.find("buttonClicked|ticket_buy") != string::npos) {
								//cout << "This button is called: " << cch << endl;
								if (pInfo(peer)->lastwrenchb != 1902) break;
								int c_ = 0, c_t = 0, remove = 0;
								modify_inventory(peer, 242, c_);
								if (c_ == 0) break;
								if (modify_inventory(peer, 1898, c_t = 30) == 0) {
									modify_inventory(peer, 242, remove = -1);
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`9You got 30 Golden Tickets.``");
								}
								else {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`4Oops, your inventory can't fit those Golden Tickets.``");
								}
							}
						}
						
						/*
						else if (cch.find("action|dialog_return\ndialog_name|ticket_booth\nticket_amount|") != string::npos) {
							int item = pInfo(peer)->lastchoosenitem;
							if (item <= 0 or pInfo(peer)->lastwrenchb != 1902 or item == NULL or item > items.size()) break;
							int c_ = 0;
							rt_parser parser;
							int amount;
							if (!parser.parse(cch) || parser.get().empty())
								break;
							if (!parser.try_get("ticket_amount", amount))
								break;
							modify_inventory(peer, item, c_);
							if (c_ < amount) break;
							if (items[item].rarity == 999 && item != 242) break;
							if (item == 242) {
								int amt = amount;
								int wls = 0;
								int ticket = 0;
								modify_inventory(peer, 242, wls);
								modify_inventory(peer, 1898, ticket);
								if (wls < amt) break;
								int amount2 = amt * 30;
								if ((ticket + (amount2)) > 200) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "Seems like you've donated too much, please reduce the amount.");
									break;
								}
								if (modify_inventory(peer, 242, amt *= -1) == 0 && modify_inventory(peer, 1898, amount2) == 0) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, format("`9You got {} Golden Tickets.``", amount2));
								}
								else {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "Failed to add Golden Tickets to your inventory.");
								}
							}
							else {
								int amt = amount;
								int wls = 0, ticket = 0;
								wls = c_;
								modify_inventory(peer, 1898, ticket);
								if (wls < amt) break;
								int rarity = items[item].rarity;
								if (rarity + pInfo(peer)->carnival_credit < 100) {
									int remove = wls * -1;
									pInfo(peer)->carnival_credit += rarity;
									Algorithm::send_bubble(peer, pInfo(peer)->netID, format("`9You have a credit of {} Rarity.``", pInfo(peer)->carnival_credit));
									modify_inventory(peer, item, remove);
								}
								else {
									int amount2 = (rarity + pInfo(peer)->carnival_credit) / 100;
									pInfo(peer)->carnival_credit -= (amount2 * 100);
									if ((ticket + (amount2)) > 200) {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "Seems like you'll have to free some spaces.");
										break;
									}
									if (modify_inventory(peer, 242, amt *= -1) == 0 && modify_inventory(peer, 1898, amount2) == 0) {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, format("`9You got {} Golden Tickets.``", amount2));
									}
									else {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "Failed to add Golden Tickets to your inventory.");
									}
								}
							}
						}
						*/
						else if (cch.find("action|dialog_return\ndialog_name|buy_item") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|item_purchase") != string::npos ||
							cch.find("action|dialog_return\ndialog_name|item_purchase_confirm") != string::npos) {
								if (cch.find("action|dialog_return\ndialog_name|item_purchase_confirm") != string::npos) {
									try {
										string data_embed = explode("itemID|", cch)[1];
										replace_str(data_embed, "\n", "");
										int item_id = atoi(data_embed.c_str());
										int count = pInfo(peer)->expected_count;

										if (item_id <= 1 || item_id >= items.size()) break;
										if (count <= 0 || count > 200) {
											packet_(peer, "action|log\nmsg|Please only use 1-200 as the count.");
											break;
										}

										auto it = find_if(reservedItems.begin(), reservedItems.end(), [item_id](const ReservedItem& my_item) {
											return my_item.id == item_id;
											});

										if (it == reservedItems.end()) break;

										int the_price = 0;
										bool per_wl = false;

										for (int i = 0; i < reservedItems.size(); i++) {
											if (reservedItems[i].id == item_id) {
												the_price = reservedItems[i].price;
												per_wl = reservedItems[i].per_wl;
												break;
											}
										}
										if (the_price <= 0) break;
										if (per_wl) {
											if (count < the_price) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You have to buy at least 1 World Lock's worth.");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You have to buy at least 1 World Lock's worth.");
													p.CreatePacket(peer);
												}
											}
											if ((count % the_price) != 0)
												count -= count % the_price;

											int price = count / the_price;
											if (pInfo(peer)->gems < price or pInfo(peer)->gems < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You can't afford that many!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You can't afford that many!");
													p.CreatePacket(peer);
												}
												break;
											}
											int has_alr = get_item_count(pInfo(peer), item_id);
											if (has_alr + count > 200) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int free_slots = get_free_slots(pInfo(peer));
											if (free_slots == 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int a_ = count;
											if (modify_inventory(peer, item_id, a_) == 0) {
												pInfo(peer)->gems = pInfo(peer)->gems - price;
												Algorithm::set_bux(peer);
												Algorithm::send_console(peer, format("`w[`9{} bought {} {} for {} Gems from the /buy.`w]``", pInfo(peer)->tankIDName, count, items[item_id].name, setGems(price)));
											}
										}
										else {
											int price = count * the_price;
											if (pInfo(peer)->gems < price or pInfo(peer)->gems < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You can't afford that many!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You can't afford that many!");
													p.CreatePacket(peer);
												}
												break;
											}
											int has_alr = get_item_count(pInfo(peer), item_id);
											if (has_alr + count > 200) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int free_slots = get_free_slots(pInfo(peer));
											if (free_slots == 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int a_ = count;
											if (modify_inventory(peer, item_id, a_) == 0) {
												pInfo(peer)->gems = pInfo(peer)->gems - price;
												Algorithm::set_bux(peer);
												Algorithm::send_console(peer, format("`w[`9{} bought {} {} for {} Gems from the /buy.`w]``", pInfo(peer)->tankIDName, count, items[item_id].name, setGems(price)));
											}
										}

										pInfo(peer)->expected_count = 0;
										break;
									}
									catch (...) {
										break;
									}
								}
								if (cch.find("action|dialog_return\ndialog_name|item_purchase") != string::npos)
								{
									try {
										string data_embed = explode("itemID|", cch)[1];
										string data_count = explode("input_wl_buy|", cch)[1];
										replace_str(data_count, "\n", "");
										replace_str(data_embed, "\n", "");
										int item_id = atoi(data_embed.c_str());
										int count = atoi(data_count.c_str());
										if (item_id <= 1 || item_id >= items.size()) break;
										if (count <= 0 || count > 200) {
											packet_(peer, "action|log\nmsg|Please only use 1-200 as the count.");
											break;
										}

										auto it = find_if(reservedItems.begin(), reservedItems.end(), [item_id](const ReservedItem& my_item) {
											return my_item.id == item_id;
											});

										if (it == reservedItems.end()) break;

										int the_price = 0;
										bool per_wl = false;

										for (int i = 0; i < reservedItems.size(); i++) {
											if (reservedItems[i].id == item_id) {
												the_price = reservedItems[i].price;
												per_wl = reservedItems[i].per_wl;
												break;
											}
										}
										if (the_price <= 0) break;
										if (per_wl) {
											if (count < the_price) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You have to buy at least 1 World Lock's worth.");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You have to buy at least 1 World Lock's worth.");
													p.CreatePacket(peer);
												}
												break;
											}
											int price = count / the_price;
											if (price < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("Invalid price data, please input a smaller quantity!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("Invalid price data, please input a smaller quantity!");
													p.CreatePacket(peer);
												}
												break;
											}
											if (pInfo(peer)->gems < price or pInfo(peer)->gems < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You can't afford that many!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You can't afford that many!");
													p.CreatePacket(peer);
												}
												break;
											}
											int has_alr = get_item_count(pInfo(peer), item_id);
											if (has_alr + count > 200) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int free_slots = get_free_slots(pInfo(peer));
											if (free_slots == 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
										}
										else {
											int price = count * the_price;
											if (price < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("Invalid price data, please input a smaller quantity!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("Invalid price data, please input a smaller quantity!");
													p.CreatePacket(peer);
												}
												break;
											}
											if (pInfo(peer)->gems < price or pInfo(peer)->gems < 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You can't afford that many!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You can't afford that many!");
													p.CreatePacket(peer);
												}
												break;
											}
											int has_alr = get_item_count(pInfo(peer), item_id);
											if (has_alr + count > 200) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
											int free_slots = get_free_slots(pInfo(peer));
											if (free_slots == 0) {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("You don't have room in your backpack!");
												p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("You don't have room in your backpack!");
													p.CreatePacket(peer);
												}
												break;
											}
										}
										pInfo(peer)->expected_count = count;
										gamepacket_t p;
										p.Insert("OnDialogRequest"), p.Insert(get_buy_dialog(peer, item_id, the_price, per_wl, true));
										p.CreatePacket(peer);
										break;
									}
									catch (...) {
										break;
									}
								}
								if (cch.find("action|dialog_return\ndialog_name|buy_item") != string::npos && cch.find("item_") != string::npos)
								{

									try
									{
										int item_id = atoi(explode("item_", cch)[1].c_str());
										if (item_id <= 1 || item_id >= items.size()) break;

										auto it = find_if(reservedItems.begin(), reservedItems.end(), [item_id](const ReservedItem& my_item) {
											return my_item.id == item_id;
											});

										if (it == reservedItems.end()) break;

										int the_price = 0;
										bool per_wl = false;

										for (int i = 0; i < reservedItems.size(); i++) {
											if (reservedItems[i].id == item_id) {
												the_price = reservedItems[i].price;
												per_wl = reservedItems[i].per_wl;
												break;
											}
										}
										if (the_price <= 0) break;

										gamepacket_t p;
										p.Insert("OnDialogRequest"), p.Insert(get_buy_dialog(peer, item_id, the_price, per_wl));
										p.CreatePacket(peer);
										break;
									}
									catch (const std::exception&)
									{
									}
								}

							}
						else if (cch.find("action|dialog_return\ndialog_name|buy_option") != string::npos) {
							if (!pInfo(peer)->supermod) break;
							rt_parser parser;
							if (!parser.parse(cch) || parser.get().empty())
								break;

							vector<int> list;
							for (const auto& it : parser.get()) {
								if (it.first.find("item_") == std::string::npos || it.second != "1")
									continue;
								int item_id = atoi(it.first.substr(5).c_str());
								if (item_id <= 1 || item_id > items.size() || item_id % 2 != 0) 
									continue;
								list.push_back(item_id);
							}
							OldDialogBuilder db;
							db.set_default_color("o")
								->add_icon_label("Found items: ", 6016, LABEL_BIG)
								->add_spacer();

							for (const auto& it : list) {
								const auto& item = items.at(it);
								bool fail = true;
								if (item.id == 0)
									continue;
								if (pInfo(peer)->supermod <= 0 && !(find(blacklisted_items.begin(), blacklisted_items.end(), item.id) != blacklisted_items.end())) {
									fail = true;
								}
								if (pInfo(peer)->staff) {
									for (int i_ = 0; i_ < items.size(); i_++) {
										uint32_t item_id = items[i_].id;
										if (items[item_id].blockType == SEED) continue;
										if (items[item_id].id == item.id) {
											fail = false;
										}
									}
								}
								else if (pInfo(peer)->dev) {
									for (int i_ = 0; i_ < items.size(); i_++) {
										uint32_t item_id = items[i_].id;
										if (items[item_id].blockType == SEED) continue;
										if (to_lower(items[i_].ori_name).find("legend") != string::npos) continue;
										if (items[item_id].id == item.id) {
											fail = false;
										}
									}
								}
								else {
									for (int i_ = 0; i_ < items.size(); i_++) {
										uint32_t item_id = items[i_].id;
										if (items[item_id].blockType == SEED) continue;
										//if (items[item_id].blockType == SEED) continue;

										//if (items[item_id].blockType != FOREGROUND || items[item_id].blockType != BACKGROUND) continue;
										if (items[item_id].clothType == NONE) continue;
										if (to_lower(items[i_].ori_name).find("legend") != string::npos) continue;
										if (to_lower(items[i_].ori_name).find("lock") != string::npos) continue;
										if (to_lower(items[i_].ori_name).find("Lock") != string::npos) continue;
										if (items[item_id].blockType == LOCK) continue;



										if (items[item_id].id == item.id) {
											fail = false;
										}
									}
								}
								int i = 0;
								modify_inventory(peer, item.id, i);
								int c_ = 200 - i;
								if (not fail) {
									if (modify_inventory(peer, item.id, c_) == 0)
									{
										PlayerMoving data_{};
										data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										data_.packetType = 19, data_.plantingTree = 100;
										data_.punchX = item.id, data_.punchY = pInfo(peer)->netID;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										db.add_label(LABEL_SMALL, format("`6>> Given {} {}.``", c_, item.name));
									}
									else
										db.add_label(LABEL_SMALL, format("`6>> Failed to add {} to inventory.``", item.name));
								}
								else {
									db.add_label(LABEL_SMALL, format("`4{} can't be taken from /find, you can get it in another way.``", item.name));
								}
							}

							db.add_quick_exit()
								->add_spacer()
								->add_input("get_item", "", "", 30)
								->add_custom_packet("add_button_with_icon||END_LIST|noflags|0||\n")
								->end_dialog("findmore", "Find more", "Close");
							
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(db.get());
							p.CreatePacket(peer);
						}
						else if (cch.find("action|dialog_return\ndialog_name|findmore") != string::npos) {
						if (pInfo(peer)->supermod + pInfo(peer)->staff <= 0) break;
						TextScanner parser(cch);
						string name = parser.get("get_item", 1);
						SendCmd(peer, "/find " + name);
                        }
						else if (cch.find("action|dialog_return\ndialog_name|sessionlength_edit") != string::npos) {
							try {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (pInfo(peer)->tankIDName != world_->owner_name) break;
									world_->World_Time = (atoi(explode("\n", explode("checkbox_5|", cch)[1])[0].c_str()) == 1 ? 5 : (atoi(explode("\n", explode("checkbox_10|", cch)[1])[0].c_str()) == 1 ? 10 : (atoi(explode("\n", explode("checkbox_20|", cch)[1])[0].c_str()) == 1 ? 20 : (atoi(explode("\n", explode("checkbox_30|", cch)[1])[0].c_str()) == 1 ? 30 : (atoi(explode("\n", explode("checkbox_40|", cch)[1])[0].c_str()) == 1 ? 40 : (atoi(explode("\n", explode("checkbox_50|", cch)[1])[0].c_str()) == 1 ? 50 : (atoi(explode("\n", explode("checkbox_60|", cch)[1])[0].c_str()) == 1 ? 60 : 0)))))));
									gamepacket_t p;
									p.Insert("OnTalkBubble"); p.Insert(pInfo(peer)->netID);
									p.Insert((world_->World_Time == 0 ? "World Timer limit removed!" : "World Timer limit set to `2" + to_string(world_->World_Time) + " minutes``."));
									p.Insert(0); p.Insert(0); p.CreatePacket(peer);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == world_->name) {
											if (pInfo(currentPeer)->tankIDName != world_->owner_name && world_->World_Time != 0) {
												pInfo(currentPeer)->World_Timed = time(nullptr) + (world_->World_Time * 60);
												pInfo(currentPeer)->WorldTimed = true;
											}
										}
									}
								}
							}
							catch (out_of_range) {
								cout << "Server error invalid (out of range) on " + cch << endl;
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worldcategory") != string::npos) {
							string Cat = "None";
							if (cch.find("buttonClicked|cat10") != string::npos) {
								Cat = "Storage";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|10\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat11") != string::npos) {
								Cat = "Story";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|11\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat12") != string::npos) {
								Cat = "Trade";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|12\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat14") != string::npos) {
								Cat = "Puzzle";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|14\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat15") != string::npos) {
								Cat = "Music";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|15\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat0") != string::npos) {
								Cat = "None";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|0\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat1") != string::npos) {
								Cat = "Adventure";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|1\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat2") != string::npos) {
								Cat = "Art";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|2\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat3") != string::npos) {
								Cat = "Farm";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|3\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat4") != string::npos) {
								Cat = "Game";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|4\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat5") != string::npos) {
								Cat = "Information";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|5\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat6") != string::npos) {
								Cat = "Parkour";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|6\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat7") != string::npos) {
								Cat = "Roleplay";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|7\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat8") != string::npos) {
								Cat = "Shop";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|8\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|cat9") != string::npos) {
								Cat = "Social";
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSet World Category``|left|3802|\nembed_data|chosencat|9\nadd_textbox|Are you sure you want to change your world's category to " + Cat + "?|left|\nadd_smalltext|`4Warning:`` Changing your category will delete all ratings on your world .|left|\nend_dialog|worldcategory|Nevermind|Change Category|");
								p.CreatePacket(peer);
								break;
							}
							else {
								int Category = atoi(explode("\n", explode("chosencat|", cch)[1])[0].c_str());
								if (Category < 0 or Category > 15) break;
								string Cat = "None";
								if (Category == 0) Cat = "None"; if (Category == 1) Cat = "Adventure"; if (Category == 2) Cat = "Art"; if (Category == 3) Cat = "Farm"; if (Category == 4) Cat = "Game"; if (Category == 5) Cat = "Information"; if (Category == 15) Cat = "Music"; if (Category == 6) Cat = "Parkour"; if (Category == 14) Cat = "Puzzle"; if (Category == 7) Cat = "Roleplay"; if (Category == 8) Cat = "Shop"; if (Category == 9) Cat = "Social"; if (Category == 10) Cat = "Storage"; if (Category == 11) Cat = "Story"; if (Category == 12) Cat = "Trade";
								try {
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										if (pInfo(peer)->tankIDName != world_->owner_name) break;
										world_->World_Rating = 0;
										world_->Category = Cat;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("This world has been moved to the '" + Cat + "' category! Everyone, please type `2/rate`` to rate it from 1-5 stars.");
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == world_->name) {
												p.CreatePacket(currentPeer);
											}
										}
									}
								}
								catch (out_of_range) {
									cout << "Server error invalid (out of range) on " + cch << endl;
									break;
								}
								break;
							}
							break;
						}
						// DI CRASH MULU
						/*
						* else if (cch.find("action|dialog_return\ndialog_name|bannerbandolier") != string::npos) {
							if (cch.find("buttonClicked|patternpicker") != string::npos) {
								string dialog = "";
								if (pInfo(peer)->Banner_Flag == 0) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5838|";
								else if (pInfo(peer)->Banner_Flag == 1) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5844|";
								else if (pInfo(peer)->Banner_Flag == 2) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5848|";
								else if (pInfo(peer)->Banner_Flag == 3) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5846|";
								else if (pInfo(peer)->Banner_Flag == 4) dialog += "set_default_color|`o\nadd_label_with_icon|big|`wBanner Bandolier``|left|5842|";
								dialog += "\nadd_spacer|small|\nadd_textbox|Pick a pattern for your banner.|left|\nadd_spacer|small|";
								dialog += "\nadd_label_with_icon_button|big|Harlequin|left|5838|pattern_1|\nadd_spacer|small|";
								dialog += "\nadd_label_with_icon_button|big|Slant|left|5844|pattern_2|\nadd_spacer|small|";
								dialog += "\nadd_label_with_icon_button|big|Stripe|left|5848|pattern_3|\nadd_spacer|small|";
								dialog += "\nadd_label_with_icon_button|big|Panel|left|5846|pattern_4|\nadd_spacer|small|";
								dialog += "\nadd_label_with_icon_button|big|Cross|left|5842|pattern_5|\nadd_spacer|small|";
								dialog += "\nend_dialog|bannerbandolier|Cancel||\nadd_quick_exit|";
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert(dialog), p.CreatePacket(peer);
								break;
							}
							else if (cch.find("buttonClicked|pattern_") != string::npos) {
								int Pattern = atoi(cch.substr(49 + 22, cch.length() - 49 + 22).c_str());
								pInfo(peer)->CBanner_Item = pInfo(peer)->Banner_Item;
								pInfo(peer)->CBanner_Flag = Pattern - 1;
								SendBannerBandolier2(peer);
								break;
							}
							else if (cch.find("buttonClicked|reset") != string::npos) {
								pInfo(peer)->CBanner_Item = 0; pInfo(peer)->CBanner_Flag = 0; pInfo(peer)->Banner_Item = 0; pInfo(peer)->Banner_Flag = 0;
								SendBannerBandolier2(peer);
								break;
							}
							else if (!(cch.find("buttonClicked|patternpicker") != string::npos || cch.find("buttonClicked|pattern_") != string::npos || cch.find("\nbanneritem|") != string::npos)) {
								if (pInfo(peer)->CBanner_Item != 0) pInfo(peer)->Banner_Item = pInfo(peer)->CBanner_Item;
								if (pInfo(peer)->CBanner_Flag != 0) pInfo(peer)->Banner_Flag = pInfo(peer)->CBanner_Flag;
								pInfo(peer)->CBanner_Item = 0; pInfo(peer)->CBanner_Flag = 0;
								update_clothes(peer);
								break;
							}
							else {
								pInfo(peer)->CBanner_Flag = pInfo(peer)->CBanner_Flag;
								pInfo(peer)->CBanner_Item = atoi(explode("\n", explode("banneritem|", cch)[1])[0].c_str());
								SendBannerBandolier2(peer);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|magic_compass") != string::npos) {
							if (cch.find("buttonClicked|clear_item") != string::npos) {
								pInfo(peer)->Magnet_Item = 0;
								update_clothes(peer);
								gamepacket_t p;
								p.Insert("OnMagicCompassTrackingItemIDChanged");
								p.Insert(pInfo(peer)->Magnet_Item);
								p.CreatePacket(peer);
								break;
							}
							else {
								pInfo(peer)->Magnet_Item = atoi(explode("\n", explode("magic_compass_item|", cch)[1])[0].c_str());
								update_clothes(peer);
								gamepacket_t p;
								p.Insert("OnMagicCompassTrackingItemIDChanged");
								p.Insert(atoi(explode("\n", explode("magic_compass_item|", cch)[1])[0].c_str()));
								p.CreatePacket(peer);
								break;
							}
							break;
						}
						
						* else if (cch.find("action|dialog_return\ndialog_name|dialog_rift_wings") != string::npos) {
							if (cch.find("buttonClicked|button_manual") != string::npos) {
								SendDialogRiftWings(peer, true);
								break;
							}
							else if (cch.find("buttonClicked|restore_default") != string::npos) {
								// Rift Wings
								pInfo(peer)->Wing_Time_Change = true;
								pInfo(peer)->Wing_Cycle_Time = 30;
								// Wings 1
								pInfo(peer)->Wing_R_0 = 93, pInfo(peer)->Wing_G_0 = 22, pInfo(peer)->Wing_B_0 = 200, pInfo(peer)->Wing_Metal_R_0 = 220, pInfo(peer)->Wing_Metal_G_0 = 72, pInfo(peer)->Wing_Metal_B_0 = 255;
								pInfo(peer)->Open_Wing_0 = false, pInfo(peer)->Closed_Wing_0 = false, pInfo(peer)->Trail_On_0 = true, pInfo(peer)->Stamp_Particle_0 = true, pInfo(peer)->Trail_1st_0 = true, pInfo(peer)->Trail_2nd_0 = false, pInfo(peer)->Trail_3rd_0 = false, pInfo(peer)->Material_1st_0 = true, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = false;
								// Wings 2
								pInfo(peer)->Wing_R_1 = 137, pInfo(peer)->Wing_G_1 = 30, pInfo(peer)->Wing_B_1 = 43, pInfo(peer)->Wing_Metal_R_1 = 34, pInfo(peer)->Wing_Metal_G_1 = 35, pInfo(peer)->Wing_Metal_B_1 = 65;
								pInfo(peer)->Open_Wing_1 = false, pInfo(peer)->Closed_Wing_1 = false, pInfo(peer)->Trail_On_1 = true, pInfo(peer)->Stamp_Particle_1 = false, pInfo(peer)->Trail_1st_1 = false, pInfo(peer)->Trail_2nd_1 = true, pInfo(peer)->Trail_3rd_1 = false, pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = true, pInfo(peer)->Material_3rd_1 = false;
								// Total
								pInfo(peer)->Wing_Value = 104912, pInfo(peer)->Wing_Color_0_Value = 3356909055, pInfo(peer)->Wing_Metal_0_Value = 4282965247, pInfo(peer)->Wing_Color_1_Value = 723421695, pInfo(peer)->Wing_Metal_1_Value = 1059267327;
								// End
								update_clothes(peer);
								break;
							}
							else {
								try {
									pInfo(peer)->Wing_Time_Change = atoi(explode("\n", explode("checkbox_time_cycle|", cch)[1])[0].c_str()) == 1 ? true : false;
									if (!is_number(explode("\n", explode("text_input_time_cycle|", cch)[1])[0])) break;
									if (atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str()) < 1) break;
									pInfo(peer)->Wing_Cycle_Time = atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str());
									{ // Wing_1
										{
											auto Wings_Color0 = explode(",", explode("\n", explode("text_input_wings_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_wings_color0|", cch)[1])[0].c_str());
											if (Wings_Color0.size() != 3 || t_.size() < 2 || Wings_Color0[0] == "" || Wings_Color0[1] == "" || Wings_Color0[2] == "" || Wings_Color0[0].empty() || Wings_Color0[1].empty() || Wings_Color0[2].empty()) {
												SendDialogRiftWings(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Wings_Color0[0]) || !is_number(Wings_Color0[1]) || !is_number(Wings_Color0[2]) || atoi(Wings_Color0[0].c_str()) > 255 || atoi(Wings_Color0[1].c_str()) > 255 || atoi(Wings_Color0[2].c_str()) > 255 || atoi(Wings_Color0[0].c_str()) < 0 || atoi(Wings_Color0[1].c_str()) < 0 || atoi(Wings_Color0[2].c_str()) < 0) {
												SendDialogRiftWings(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Wing_R_0 = atoi(Wings_Color0.at(0).c_str());
											pInfo(peer)->Wing_G_0 = atoi(Wings_Color0.at(1).c_str());
											pInfo(peer)->Wing_B_0 = atoi(Wings_Color0.at(2).c_str());
											pInfo(peer)->Wing_Color_0_Value = 255 + (((256 * atoi(Wings_Color0.at(0).c_str())) + atoi(Wings_Color0.at(1).c_str()) * 65536) + atoi(Wings_Color0.at(2).c_str()) * (long long int)16777216);
										}
										{
											auto Metal_Color0 = explode(",", explode("\n", explode("text_input_metal_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_metal_color0|", cch)[1])[0].c_str());
											if (Metal_Color0.size() != 3 || t_.size() < 2 || Metal_Color0[0] == "" || Metal_Color0[1] == "" || Metal_Color0[2] == "" || Metal_Color0[0].empty() || Metal_Color0[1].empty() || Metal_Color0[2].empty()) {
												SendDialogRiftWings(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Metal_Color0[0]) || !is_number(Metal_Color0[1]) || !is_number(Metal_Color0[2]) || atoi(Metal_Color0[0].c_str()) > 255 || atoi(Metal_Color0[1].c_str()) > 255 || atoi(Metal_Color0[2].c_str()) > 255 || atoi(Metal_Color0[0].c_str()) < 0 || atoi(Metal_Color0[1].c_str()) < 0 || atoi(Metal_Color0[2].c_str()) < 0) {
												SendDialogRiftWings(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Wing_Metal_R_0 = atoi(Metal_Color0.at(0).c_str());
											pInfo(peer)->Wing_Metal_G_0 = atoi(Metal_Color0.at(1).c_str());
											pInfo(peer)->Wing_Metal_B_0 = atoi(Metal_Color0.at(2).c_str());
											pInfo(peer)->Wing_Metal_0_Value = 255 + (((256 * atoi(Metal_Color0.at(0).c_str())) + atoi(Metal_Color0.at(1).c_str()) * 65536) + atoi(Metal_Color0.at(2).c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Open_Wing_0 = atoi(explode("\n", explode("checkbox_open_wings0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Closed_Wing_0 = atoi(explode("\n", explode("checkbox_closed_wings0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Stamp_Particle_0 = atoi(explode("\n", explode("checkbox_stamp_particle0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_On_0 = atoi(explode("\n", explode("checkbox_trail0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_1st_0 = atoi(explode("\n", explode("checkbox_trail_1st0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_2nd_0 = atoi(explode("\n", explode("checkbox_trail_2nd0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_3rd_0 = atoi(explode("\n", explode("checkbox_trail_3rd0|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
									{ // Wing_2
										{
											auto Wings_Color1 = explode(",", explode("\n", explode("text_input_wings_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_wings_color1|", cch)[1])[0].c_str());
											if (Wings_Color1.size() != 3 || t_.size() < 2 || Wings_Color1[0] == "" || Wings_Color1[1] == "" || Wings_Color1[2] == "" || Wings_Color1[0].empty() || Wings_Color1[1].empty() || Wings_Color1[2].empty()) {
												SendDialogRiftWings(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Wings_Color1[0]) || !is_number(Wings_Color1[1]) || !is_number(Wings_Color1[2]) || atoi(Wings_Color1[0].c_str()) > 255 || atoi(Wings_Color1[1].c_str()) > 255 || atoi(Wings_Color1[2].c_str()) > 255 || atoi(Wings_Color1[0].c_str()) < 0 || atoi(Wings_Color1[1].c_str()) < 0 || atoi(Wings_Color1[2].c_str()) < 0) {
												SendDialogRiftWings(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Wing_R_1 = atoi(Wings_Color1.at(0).c_str());
											pInfo(peer)->Wing_G_1 = atoi(Wings_Color1.at(1).c_str());
											pInfo(peer)->Wing_B_1 = atoi(Wings_Color1.at(2).c_str());
											pInfo(peer)->Wing_Color_1_Value = 255 + (((256 * atoi(Wings_Color1.at(0).c_str())) + atoi(Wings_Color1.at(1).c_str()) * 65536) + atoi(Wings_Color1.at(2).c_str()) * (long long int)16777216);
										}
										{
											auto Metal_Color1 = explode(",", explode("\n", explode("text_input_metal_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_metal_color1|", cch)[1])[0].c_str());
											if (Metal_Color1.size() != 3 || t_.size() < 2 || Metal_Color1[0] == "" || Metal_Color1[1] == "" || Metal_Color1[2] == "" || Metal_Color1[0].empty() || Metal_Color1[1].empty() || Metal_Color1[2].empty()) {
												SendDialogRiftWings(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Metal_Color1[0]) || !is_number(Metal_Color1[1]) || !is_number(Metal_Color1[2]) || atoi(Metal_Color1[0].c_str()) > 255 || atoi(Metal_Color1[1].c_str()) > 255 || atoi(Metal_Color1[2].c_str()) > 255 || atoi(Metal_Color1[0].c_str()) < 0 || atoi(Metal_Color1[1].c_str()) < 0 || atoi(Metal_Color1[2].c_str()) < 0) {
												SendDialogRiftWings(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Wing_Metal_R_1 = atoi(Metal_Color1.at(0).c_str());
											pInfo(peer)->Wing_Metal_G_1 = atoi(Metal_Color1.at(1).c_str());
											pInfo(peer)->Wing_Metal_B_1 = atoi(Metal_Color1.at(2).c_str());
											pInfo(peer)->Wing_Metal_1_Value = 255 + (((256 * atoi(Metal_Color1.at(0).c_str())) + atoi(Metal_Color1.at(1).c_str()) * 65536) + atoi(Metal_Color1.at(2).c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Open_Wing_1 = atoi(explode("\n", explode("checkbox_open_wings1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Closed_Wing_1 = atoi(explode("\n", explode("checkbox_closed_wings1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Stamp_Particle_1 = atoi(explode("\n", explode("checkbox_stamp_particle1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_On_1 = atoi(explode("\n", explode("checkbox_trail1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_1st_1 = atoi(explode("\n", explode("checkbox_trail_1st1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_2nd_1 = atoi(explode("\n", explode("checkbox_trail_2nd1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Trail_3rd_1 = atoi(explode("\n", explode("checkbox_trail_3rd1|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
								}
								catch (...) {
									break;
								}
								int Feathers_0 = atoi(explode("\n", explode("checkbox_material_1st0|", cch)[1])[0].c_str()), Blades_0 = atoi(explode("\n", explode("checkbox_material_2nd0|", cch)[1])[0].c_str()), Scael_0 = atoi(explode("\n", explode("checkbox_material_3rd0|", cch)[1])[0].c_str());
								int Feathers_1 = atoi(explode("\n", explode("checkbox_material_1st1|", cch)[1])[0].c_str()), Blades_1 = atoi(explode("\n", explode("checkbox_material_2nd1|", cch)[1])[0].c_str()), Scael_1 = atoi(explode("\n", explode("checkbox_material_3rd1|", cch)[1])[0].c_str());
								int TValue = 0;
								if (Feathers_0 == 0 && Blades_0 == 0 && Scael_0 == 0) {
									SendDialogRiftWings(peer, false, "You need to select one material for style 1");
									break;
								}
								if (Feathers_1 == 0 && Blades_1 == 0 && Scael_1 == 0) {
									SendDialogRiftWings(peer, false, "You need to select one material for style 2");
									break;
								}
								if (Feathers_0 == 1 && Feathers_1 == 1) {
									TValue += 20480;
									pInfo(peer)->Material_1st_0 = true, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = true, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Feathers_0 == 1 && Blades_1 == 1) {
									TValue += 20480 + 16384;
									pInfo(peer)->Material_1st_0 = true, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = true, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Feathers_0 == 1 && Scael_1 == 1) {
									TValue += 20480 + (16384 * 2);
									pInfo(peer)->Material_1st_0 = true, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = true;
								}
								if (Blades_0 == 1 && Feathers_1 == 1) {
									TValue += 20480 + 4096;
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = true, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = true, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Blades_0 == 1 && Blades_1 == 1) {
									TValue += 20480 + 4096 + 16384;
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = true, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = true, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Blades_0 == 1 && Scael_1 == 1) {
									TValue += 20480 + 4096 + (16384 * 2);
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = true, pInfo(peer)->Material_3rd_0 = false;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = true;
								}
								if (Scael_0 == 1 && Feathers_1 == 1) {
									TValue += 20480 + (4096 * 2);
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = true;
									pInfo(peer)->Material_1st_1 = true, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Scael_0 == 1 && Blades_1 == 1) {
									TValue += 20480 + (4096 * 2) + 16384;
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = true;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = true, pInfo(peer)->Material_3rd_1 = false;
								}
								if (Scael_0 == 1 && Scael_1 == 1) {
									TValue += 20480 + (4096 * 2) + (16384 * 2);
									pInfo(peer)->Material_1st_0 = false, pInfo(peer)->Material_2nd_0 = false, pInfo(peer)->Material_3rd_0 = true;
									pInfo(peer)->Material_1st_1 = false, pInfo(peer)->Material_2nd_1 = false, pInfo(peer)->Material_3rd_1 = true;
								}
								if (pInfo(peer)->Wing_Time_Change) TValue += 65536;
								if (pInfo(peer)->Open_Wing_0) TValue += 1;
								if (pInfo(peer)->Open_Wing_1) TValue += 2;
								if (pInfo(peer)->Closed_Wing_0) TValue += 4;
								if (pInfo(peer)->Closed_Wing_1) TValue += 8;
								if (pInfo(peer)->Trail_On_0) TValue += 16;
								if (pInfo(peer)->Trail_On_1) TValue += 32;
								if (pInfo(peer)->Stamp_Particle_0) TValue += 64;
								if (pInfo(peer)->Stamp_Particle_1) TValue += 128;
								if (pInfo(peer)->Trail_1st_0) TValue += 256;
								if (pInfo(peer)->Trail_1st_1) TValue += 1024;
								if (pInfo(peer)->Trail_2nd_0) TValue += 256 * 2;
								if (pInfo(peer)->Trail_2nd_1) TValue += 1024 * 2;
								if (pInfo(peer)->Trail_3rd_0) TValue += 256 * 3;
								if (pInfo(peer)->Trail_3rd_1) TValue += 1024 * 3;
								pInfo(peer)->Wing_Value = TValue;
								update_clothes(peer);
							}
							break;
						}
						* else if (cch.find("action|dialog_return\ndialog_name|dialog_infinity_crown") != string::npos) {
							if (cch.find("buttonClicked|button_manual") != string::npos) {
								SendDialogInfinityCrown(peer, true);
								break;
							}
							else if (cch.find("buttonClicked|restore_default") != string::npos) {
								// Infinity Crown
								pInfo(peer)->Crown_Time_Change = true;
								pInfo(peer)->Crown_Cycle_Time = 15;
								// Crown 1
								pInfo(peer)->Base_R_0 = 255, pInfo(peer)->Base_G_0 = 255, pInfo(peer)->Base_B_0 = 255;
								pInfo(peer)->Gem_R_0 = 255, pInfo(peer)->Gem_G_0 = 0, pInfo(peer)->Gem_B_0 = 255;
								pInfo(peer)->Crystal_R_0 = 0, pInfo(peer)->Crystal_G_0 = 205, pInfo(peer)->Crystal_B_0 = 249;
								pInfo(peer)->Crown_Floating_Effect_0 = false, pInfo(peer)->Crown_Laser_Beam_0 = true, pInfo(peer)->Crown_Crystals_0 = true, pInfo(peer)->Crown_Rays_0 = true;
								// Crown 2
								pInfo(peer)->Base_R_1 = 255, pInfo(peer)->Base_G_1 = 200, pInfo(peer)->Base_B_1 = 37;
								pInfo(peer)->Gem_R_1 = 255, pInfo(peer)->Gem_G_1 = 0, pInfo(peer)->Gem_B_1 = 64;
								pInfo(peer)->Crystal_R_1 = 26, pInfo(peer)->Crystal_G_1 = 45, pInfo(peer)->Crystal_B_1 = 140;
								pInfo(peer)->Crown_Floating_Effect_1 = false, pInfo(peer)->Crown_Laser_Beam_1 = true, pInfo(peer)->Crown_Crystals_1 = true, pInfo(peer)->Crown_Rays_1 = true;
								// Total
								pInfo(peer)->Crown_Value = 1768716607;
								pInfo(peer)->Crown_Value_0_0 = 4294967295, pInfo(peer)->Crown_Value_0_1 = 4278255615, pInfo(peer)->Crown_Value_0_2 = 4190961919;
								pInfo(peer)->Crown_Value_1_0 = 633929727, pInfo(peer)->Crown_Value_1_1 = 1073807359, pInfo(peer)->Crown_Value_1_2 = 2351766271;
								// End
								update_clothes(peer);
								break;
							}
							else {
								try {
									pInfo(peer)->Crown_Time_Change = atoi(explode("\n", explode("checkbox_time_cycle|", cch)[1])[0].c_str()) == 1 ? true : false;
									if (!is_number(explode("\n", explode("text_input_time_cycle|", cch)[1])[0])) break;
									pInfo(peer)->Crown_Cycle_Time = atoi(explode("\n", explode("text_input_time_cycle|", cch)[1])[0].c_str());
									{ // Crown 1
										pInfo(peer)->Crown_Floating_Effect_0 = atoi(explode("\n", explode("checkbox_floating0|", cch)[1])[0].c_str()) == 1 ? true : false;
										{
											auto Crown_Base_0 = explode(",", explode("\n", explode("text_input_base_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_base_color0|", cch)[1])[0].c_str());
											if (Crown_Base_0.size() != 3 || t_.size() < 2 || Crown_Base_0[0] == "" || Crown_Base_0[1] == "" || Crown_Base_0[2] == "" || Crown_Base_0[0].empty() || Crown_Base_0[1].empty() || Crown_Base_0[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Crown_Base_0[0]) || !is_number(Crown_Base_0[1]) || !is_number(Crown_Base_0[2]) || atoi(Crown_Base_0[0].c_str()) > 255 || atoi(Crown_Base_0[1].c_str()) > 255 || atoi(Crown_Base_0[2].c_str()) > 255 || atoi(Crown_Base_0[0].c_str()) < 0 || atoi(Crown_Base_0[1].c_str()) < 0 || atoi(Crown_Base_0[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Base_R_0 = atoi(Crown_Base_0[0].c_str());
											pInfo(peer)->Base_G_0 = atoi(Crown_Base_0[1].c_str());
											pInfo(peer)->Base_B_0 = atoi(Crown_Base_0[2].c_str());
											pInfo(peer)->Crown_Value_0_0 = (long long int)(255 + (256 * pInfo(peer)->Base_R_0) + pInfo(peer)->Base_G_0 * 65536 + (pInfo(peer)->Base_B_0 * (long long int)16777216));
										}
										{
											pInfo(peer)->Crown_Laser_Beam_0 = atoi(explode("\n", explode("checkbox_laser_beam0|", cch)[1])[0].c_str()) == 1 ? true : false;
											auto Crown_Gem_0 = explode(",", explode("\n", explode("text_input_gem_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_gem_color0|", cch)[1])[0].c_str());
											if (Crown_Gem_0.size() != 3 || t_.size() < 2 || Crown_Gem_0[0] == "" || Crown_Gem_0[1] == "" || Crown_Gem_0[2] == "" || Crown_Gem_0[0].empty() || Crown_Gem_0[1].empty() || Crown_Gem_0[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Crown_Gem_0[0]) || !is_number(Crown_Gem_0[1]) || !is_number(Crown_Gem_0[2]) || atoi(Crown_Gem_0[0].c_str()) > 255 || atoi(Crown_Gem_0[1].c_str()) > 255 || atoi(Crown_Gem_0[2].c_str()) > 255 || atoi(Crown_Gem_0[0].c_str()) < 0 || atoi(Crown_Gem_0[1].c_str()) < 0 || atoi(Crown_Gem_0[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Gem_R_0 = atoi(Crown_Gem_0[0].c_str());
											pInfo(peer)->Gem_G_0 = atoi(Crown_Gem_0[1].c_str());
											pInfo(peer)->Gem_B_0 = atoi(Crown_Gem_0[2].c_str());
											pInfo(peer)->Crown_Value_0_1 = 255 + (256 * atoi(Crown_Gem_0[0].c_str())) + atoi(Crown_Gem_0[1].c_str()) * 65536 + (atoi(Crown_Gem_0[2].c_str()) * (long long int)16777216);
										}
										{
											auto Crown_Crystal_0 = explode(",", explode("\n", explode("text_input_crystal_color0|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_crystal_color0|", cch)[1])[0].c_str());
											if (Crown_Crystal_0.size() != 3 || t_.size() < 2 || Crown_Crystal_0[0] == "" || Crown_Crystal_0[1] == "" || Crown_Crystal_0[2] == "" || Crown_Crystal_0[0].empty() || Crown_Crystal_0[1].empty() || Crown_Crystal_0[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											};
											if (!is_number(Crown_Crystal_0[0]) || !is_number(Crown_Crystal_0[1]) || !is_number(Crown_Crystal_0[2]) || atoi(Crown_Crystal_0[0].c_str()) > 255 || atoi(Crown_Crystal_0[1].c_str()) > 255 || atoi(Crown_Crystal_0[2].c_str()) > 255 || atoi(Crown_Crystal_0[0].c_str()) < 0 || atoi(Crown_Crystal_0[1].c_str()) < 0 || atoi(Crown_Crystal_0[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Crystal_R_0 = atoi(Crown_Crystal_0[0].c_str());
											pInfo(peer)->Crystal_G_0 = atoi(Crown_Crystal_0[1].c_str());
											pInfo(peer)->Crystal_B_0 = atoi(Crown_Crystal_0[2].c_str());
											pInfo(peer)->Crown_Value_0_2 = 255 + (256 * atoi(Crown_Crystal_0[0].c_str())) + atoi(Crown_Crystal_0[1].c_str()) * 65536 + (atoi(Crown_Crystal_0[2].c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Crown_Crystals_0 = atoi(explode("\n", explode("checkbox_crystals0|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Crown_Rays_0 = atoi(explode("\n", explode("checkbox_rays0|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
									{ // Crown 2
										pInfo(peer)->Crown_Floating_Effect_1 = atoi(explode("\n", explode("checkbox_floating1|", cch)[1])[0].c_str()) == 1 ? true : false;
										{
											auto Crown_Base_1 = explode(",", explode("\n", explode("text_input_base_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_base_color1|", cch)[1])[0].c_str());
											if (Crown_Base_1.size() != 3 || t_.size() < 2 || Crown_Base_1[0] == "" || Crown_Base_1[1] == "" || Crown_Base_1[2] == "" || Crown_Base_1[0].empty() || Crown_Base_1[1].empty() || Crown_Base_1[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Crown_Base_1[0]) || !is_number(Crown_Base_1[1]) || !is_number(Crown_Base_1[2]) || atoi(Crown_Base_1[0].c_str()) > 255 || atoi(Crown_Base_1[1].c_str()) > 255 || atoi(Crown_Base_1[2].c_str()) > 255 || atoi(Crown_Base_1[0].c_str()) < 0 || atoi(Crown_Base_1[1].c_str()) < 0 || atoi(Crown_Base_1[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Base_R_1 = atoi(Crown_Base_1[0].c_str());
											pInfo(peer)->Base_G_1 = atoi(Crown_Base_1[1].c_str());
											pInfo(peer)->Base_B_1 = atoi(Crown_Base_1[2].c_str());
											pInfo(peer)->Crown_Value_1_1 = 255 + (256 * atoi(Crown_Base_1[0].c_str())) + atoi(Crown_Base_1[1].c_str()) * 65536 + (atoi(Crown_Base_1[2].c_str()) * (long long int)16777216);
										}
										{
											pInfo(peer)->Crown_Laser_Beam_1 = atoi(explode("\n", explode("checkbox_laser_beam1|", cch)[1])[0].c_str()) == 1 ? true : false;
											auto Crown_Gem_1 = explode(",", explode("\n", explode("text_input_gem_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_gem_color1|", cch)[1])[0].c_str());
											if (Crown_Gem_1.size() != 3 || t_.size() < 2 || Crown_Gem_1[0] == "" || Crown_Gem_1[1] == "" || Crown_Gem_1[2] == "" || Crown_Gem_1[0].empty() || Crown_Gem_1[1].empty() || Crown_Gem_1[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Crown_Gem_1[0]) || !is_number(Crown_Gem_1[1]) || !is_number(Crown_Gem_1[2]) || atoi(Crown_Gem_1[0].c_str()) > 255 || atoi(Crown_Gem_1[1].c_str()) > 255 || atoi(Crown_Gem_1[2].c_str()) > 255 || atoi(Crown_Gem_1[0].c_str()) < 0 || atoi(Crown_Gem_1[1].c_str()) < 0 || atoi(Crown_Gem_1[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Gem_R_1 = atoi(Crown_Gem_1[0].c_str());
											pInfo(peer)->Gem_G_1 = atoi(Crown_Gem_1[1].c_str());
											pInfo(peer)->Gem_B_1 = atoi(Crown_Gem_1[2].c_str());
											pInfo(peer)->Crown_Value_1_1 = 255 + (256 * atoi(Crown_Gem_1[0].c_str())) + atoi(Crown_Gem_1[1].c_str()) * 65536 + (atoi(Crown_Gem_1[2].c_str()) * (long long int)16777216);
										}
										{
											auto Crown_Crystal_1 = explode(",", explode("\n", explode("text_input_crystal_color1|", cch)[1])[0].c_str());
											vector<string> t_ = explode(",", explode("\n", explode("text_input_crystal_color1|", cch)[1])[0].c_str());
											if (Crown_Crystal_1.size() != 3 || t_.size() < 2 || Crown_Crystal_1[0] == "" || Crown_Crystal_1[1] == "" || Crown_Crystal_1[2] == "" || Crown_Crystal_1[0].empty() || Crown_Crystal_1[1].empty() || Crown_Crystal_1[2].empty()) {
												SendDialogInfinityCrown(peer, false, "you need to enter an RGB (Red, Blue, Green) value");
												break;
											}
											if (!is_number(Crown_Crystal_1[0]) || !is_number(Crown_Crystal_1[1]) || !is_number(Crown_Crystal_1[2]) || atoi(Crown_Crystal_1[0].c_str()) > 255 || atoi(Crown_Crystal_1[1].c_str()) > 255 || atoi(Crown_Crystal_1[2].c_str()) > 255 || atoi(Crown_Crystal_1[0].c_str()) < 0 || atoi(Crown_Crystal_1[1].c_str()) < 0 || atoi(Crown_Crystal_1[2].c_str()) < 0) {
												SendDialogInfinityCrown(peer, false, "you need to enter values betwwen 0 and 255");
												break;
											}
											pInfo(peer)->Crystal_R_1 = atoi(Crown_Crystal_1[0].c_str());
											pInfo(peer)->Crystal_G_1 = atoi(Crown_Crystal_1[1].c_str());
											pInfo(peer)->Crystal_B_1 = atoi(Crown_Crystal_1[2].c_str());
											pInfo(peer)->Crown_Value_1_2 = 255 + (256 * atoi(Crown_Crystal_1[0].c_str())) + atoi(Crown_Crystal_1[1].c_str()) * 65536 + (atoi(Crown_Crystal_1[2].c_str()) * (long long int)16777216);
										}
										pInfo(peer)->Crown_Crystals_1 = atoi(explode("\n", explode("checkbox_crystals1|", cch)[1])[0].c_str()) == 1 ? true : false;
										pInfo(peer)->Crown_Rays_1 = atoi(explode("\n", explode("checkbox_rays1|", cch)[1])[0].c_str()) == 1 ? true : false;
									}
								}
								catch (...) {
									break;
								}
								int Total_Value = 1768716288;
								if (pInfo(peer)->Crown_Time_Change) Total_Value += 256;
								if (pInfo(peer)->Crown_Floating_Effect_0) Total_Value += 64;
								if (pInfo(peer)->Crown_Laser_Beam_0) Total_Value += 1;
								if (pInfo(peer)->Crown_Crystals_0) Total_Value += 4;
								if (pInfo(peer)->Crown_Rays_0) Total_Value += 16;
								if (pInfo(peer)->Crown_Floating_Effect_1) Total_Value += 128;
								if (pInfo(peer)->Crown_Laser_Beam_1) Total_Value += 2;
								if (pInfo(peer)->Crown_Crystals_1) Total_Value += 8;
								if (pInfo(peer)->Crown_Rays_1) Total_Value += 32;
								pInfo(peer)->Crown_Value = Total_Value;
								update_clothes(peer);
							}
							break;
						}
						*/
						/*
						*/
						else if (cch.find("action|dialog_return\ndialog_name|xenonite_edit") != string::npos) {
							TextScanner parser(cch);
							int x_ = 0, y_ = 0;
							if (!parser.try_get("tilex", x_) || !parser.try_get("tiley", y_)) break; 
							try {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldBlock* block_ = GetTile(x_, y_, world_);
									if (!block_) break;
									uint16_t t_ = (block_->fg ? block_->fg : block_->bg);
									if (not items.at(t_).xeno) break;
									string owner_name = world_->owner_name, user_name = pInfo(peer)->tankIDName;
									if (owner_name != user_name and not pInfo(peer)->admin and not world_->owner_name.empty()) {
										if (block_->locked) {
											WorldBlock* check_lock = &world_->blocks.at(block_->lock_origin);
											if (check_lock->owner_name != pInfo(peer)->tankIDName) break;
										}
										else {
											break;
										}
									}
									int Xeno_1, Xeno_2, Xeno_3, Xeno_4, Xeno_5, Xeno_6, Xeno_7, Xeno_8, Xeno_9, Xeno_10, Xeno_11, Xeno_12, Xeno_13, Xeno_14, Xeno_15, Xeno_16, Xeno_17, Xeno_18, Xeno_19; 
									if (!parser.try_get("checkbox_force_dbl", Xeno_1) || !parser.try_get("checkbox_block_dbl", Xeno_2)) break;
									if (!parser.try_get("checkbox_force_hig", Xeno_3) || !parser.try_get("checkbox_block_hig", Xeno_4)) break;
									if (!parser.try_get("checkbox_force_asb", Xeno_5) || !parser.try_get("checkbox_block_asb", Xeno_6)) break;
									if (!parser.try_get("checkbox_force_pun", Xeno_7) || !parser.try_get("checkbox_block_pun", Xeno_8)) break;
									if (!parser.try_get("checkbox_force_lng", Xeno_9) || !parser.try_get("checkbox_block_lng", Xeno_10)) break;
									if (!parser.try_get("checkbox_force_spd", Xeno_11) || !parser.try_get("checkbox_block_spd", Xeno_12)) break;
									if (!parser.try_get("checkbox_force_lngb", Xeno_13) || !parser.try_get("checkbox_block_lngb", Xeno_14)) break;
									if (!parser.try_get("checkbox_force_spiw", Xeno_15) || !parser.try_get("checkbox_block_spiw", Xeno_16)) break; 
									if (!parser.try_get("checkbox_force_pdmg", Xeno_17) || !parser.try_get("checkbox_block_pdmg", Xeno_18)) break; 
									if (!parser.try_get("checkbox_force_pwr", Xeno_19)) break; 
									world_->X_1 = Xeno_1 == 1 ? true : false;
									world_->X_2 = Xeno_2 == 1 and !world_->X_1 ? true : false;
									world_->X_3 = Xeno_3 == 1 ? true : false;
									world_->X_4 = Xeno_4 == 1 and !world_->X_3 ? true : false;
									world_->X_5 = Xeno_5 == 1 ? true : false;
									world_->X_6 = Xeno_6 == 1 and !world_->X_5 ? true : false;
									world_->X_7 = Xeno_7 == 1 ? true : false;
									world_->X_8 = Xeno_8 == 1 and !world_->X_7 ? true : false;
									world_->X_9 = Xeno_9 == 1 ? true : false;
									world_->X_10 = Xeno_10 == 1 and !world_->X_9 ? true : false;
									world_->X_11 = Xeno_11 == 1 ? true : false;
									world_->X_12 = Xeno_12 == 1 and !world_->X_11 ? true : false;
									world_->X_13 = Xeno_13 == 1 ? true : false;
									world_->X_14 = Xeno_14 == 1 and !world_->X_13 ? true : false;
									world_->X_15 = Xeno_15 == 1 ? true : false;
									world_->X_16 = Xeno_16 == 1 and !world_->X_15 ? true : false;
									world_->X_17 = Xeno_17 == 1 ? true : false;
									world_->X_18 = Xeno_18 == 1 and !world_->X_17 ? true : false;
									world_->X_19 = Xeno_19 == 1 ? true : false;
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											Send_Xenonite_Update(currentPeer);
										}
									}
								}
							}
							catch (out_of_range) {
								cout << "Server error invalid (out of range) on " + cch << endl;
								break;
							}
							break;
						}
						
						else if (cch.find("action|dialog_return\ndialog_name|audio_rack\n") != string::npos) {
							if (!items[pInfo(peer)->lastwrenchb].audio_rack) break;
							try {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
									if (!items[pInfo(peer)->lastwrenchb].audio_rack) break;
									int new_volume = atoi(explode("\n", explode("audio_volume|", cch)[1])[0].c_str());
									string new_note = explode("audio_notes|", cch)[1];
									replace_str(new_note, "\n", "");
									if (new_volume < 0) {
										Algorithm::send_console(peer, "Negative amount doesn't supported.");
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "Negative amount doesn't supported.");
										break;
									}
									bool Update = true;
									int NoteLength = new_note.size() / 3;
									string Error = "", NoteRewrite = "";
									if (!new_note.empty()) {
										if (new_note.size() > 3) {
											for (int i = 0; i < NoteLength; i++) {
												char Note_1 = new_note[0 + (2 * i) + (i > 0 ? i : 0)];
												char Note_2 = new_note[1 + (2 * i) + (i > 0 ? i : 0)];
												char Note_3 = new_note[2 + (2 * i) + (i > 0 ? i : 0)];
												bool Skip_Second_Note = false;
												if (Note_2 == 'A' or Note_2 == 'B' or Note_2 == 'C' or Note_2 == 'D' or Note_1 == 'E' or Note_1 == 'F' or Note_1 == 'G') {
													Skip_Second_Note = true;
												}
												else if (IsValidNote(Note_2)) {
													Skip_Second_Note = true;
												}
												if (Note_3 == NULL) {
													Error = "`4Your last note is missing its modifier (#, -, or b)!``", Update = false;
													break;
												}
												else if (Note_3 != '#' and Note_3 != '-' and Note_3 != 'b') {
													Error = "`4Valid modifiers are -, #, or b!``", Update = false;
													break;
												}
												else if (Note_2 == NULL) {
													Error = "`4Your last note is missing its note!``", Update = false;
													break;
												}
												else if (!Skip_Second_Note and not IsValidNote(Note_2)) {
													Error = "`4Notes must be from A to G!``", Update = false;
													break;
												}
												/*
												else if (!Skip_Second_Note and Note_2 != 'a' and Note_2 != 'b' and Note_2 != 'c' and Note_2 != 'd' and Note_2 != 'e' and Note_2 != 'f' and Note_2 != 'g') {
													Error = "`4Notes must be from A to G!``", Update = false;
													break;
												}
												*/
												else if (Note_1 != 'P' and Note_1 != 'B' and Note_1 != 'S' and Note_1 != 'F' and Note_1 != 'G' and Note_1 != 'V' and Note_1 != 'L' and Note_1 != 'D' and Note_1 != 'T' and Note_1 != 'E') {
													Error = "`4The only valid instruments are P, B, S, F, G, V, L, D, T and E!``", Update = false;
													break;
												}
												for (int i_ = 0; i_ < 3; i_++) {
													NoteRewrite += new_note[i_ + (2 * i) + (i > 0 ? i : 0)];
												}
												NoteRewrite += " ";
											}
										}
										else {
											string Notes = new_note;
											bool Skip_Second_Note = false;
											if (Notes[1] == 'A' or Notes[1] == 'B' or Notes[1] == 'C' or Notes[1] == 'D' or Notes[0] == 'E' or Notes[0] == 'F' or Notes[0] == 'G') {
												Skip_Second_Note = true;
											}
											else if (Notes[1] == 'a' or Notes[1] == 'b' or Notes[1] == 'c' or Notes[1] == 'd' or Notes[1] == 'e' or Notes[1] == 'f' or Notes[1] == 'g') {
												Skip_Second_Note = true;
											}
											if (Notes[2] == NULL) Error = "`4Your last note is missing its modifier (#, -, or b)!``", Update = false;
											else if (Notes[2] != '#' and Notes[2] != '-' and Notes[2] != 'b') {
												Error = "`4Valid modifiers are -, #, or b!``", Update = false;
											}
											else if (Notes[1] == NULL) Error = "`4Your last note is missing its note!``", Update = false;
											else if (!Skip_Second_Note and Notes[1] != 'A' and Notes[1] != 'B' and Notes[1] != 'C' and Notes[1] != 'D' and Notes[0] != 'E' and Notes[0] != 'F' and Notes[0] != 'G') {
												Error = "`4Notes must be from A to G!``", Update = false;
											}
											else if (!Skip_Second_Note and Notes[1] != 'a' and Notes[1] != 'b' and Notes[1] != 'c' and Notes[1] != 'd' and Notes[1] != 'e' and Notes[1] != 'f' and Notes[1] != 'g') {
												Error = "`4Notes must be from A to G!``", Update = false;
											}
											else if (Notes[0] != 'P' and Notes[0] != 'B' and Notes[0] != 'S' and Notes[0] != 'F' and Notes[0] != 'G' and Notes[0] != 'V' and Notes[0] != 'L' and Notes[0] != 'D' and Notes[0] != 'T' and Notes[0] != 'E') {
												Error = "`4The only valid instruments are P, B, S, F, G, V, L, D, T and E!``", Update = false;
											}
											NoteRewrite = Notes + " ";
										}
									}
									if (!Error.empty()) {
										DialogBuilder db{ "`o" };
										db.add_label_icon(true, pInfo(peer)->lastwrenchb, "`w" + items[pInfo(peer)->lastwrenchb].name + "``")
											.add_spacer(false)
											.add_textbox(Error)
											.add_spacer(false)
											.add_button("audio_instruction", "Instructions")
											.add_spacer(false)
											.add_input(3, "audio_volume", "Volume", to_string(block_->audio_volume))
											.add_input(30, "audio_notes", "Note", block_->audio_notes)
											.end_dialog("audio_rack", "`wCancel``", "`wUpdate``");
										gamepacket_t p;
										p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
									}

									if (Update) {
										block_->audio_volume = new_volume;
										block_->audio_notes = NoteRewrite;

										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
										BYTE* blc = raw + 56;
										form_visual(blc, *block_, *world_, peer, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
										if (block_->locked) upd_lock(*block_, *world_, peer);
									}
								}
							}
							catch (exception& e) {
								cout << "Server error invalid: " << e.what() << endl;
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|punish_view\n") != string::npos) {
						    if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
								Algorithm::send_overlay(peer, "You can't do those actions to him.");
							    break;
							}
							if (pInfo(peer)->staff) {
								if (cch.find("buttonClicked|disconnect") != string::npos) {
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											send_server_logs(pInfo(peer)->tankIDName, "(#" + to_string(pInfo(peer)->uid) + ") disconnected " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											//if (pInfo(currentPeer)->world != "") exit_(currentPeer, true);
											enet_peer_disconnect_later(currentPeer, 2);
											Algorithm::send_overlay(peer, pInfo(currentPeer)->tankIDName + " Disconnected");
										}
									}
								}
								else if (cch.find("action|dialog_return\ndialog_name|clear_this_world") != string::npos) {
									string name_ = pInfo(peer)->world;
									int failed = 0;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p == worlds.end()) break;

									World* world_ = &worlds[p - worlds.begin()];
									if (pInfo(peer)->tankIDName != world_->owner_name) break;
									if (pInfo(peer)->gems <= 200000) break;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * 100)];

									pInfo(peer)->gems -= 200000;

									gamepacket_t newpacket;
									newpacket.Insert("OnSetBux");
									newpacket.Insert(pInfo(peer)->gems), newpacket.Insert(0);
									newpacket.CreatePacket(peer);

									{
										for (int i_ = 0; i_ < world_->blocks.size(); i_++) {
											if (i_ <= 5400) {
												if (world_->blocks[i_].fg != 0 and world_->blocks[i_].fg != 6 and world_->blocks[i_].fg != 8 and items[world_->blocks[i_].fg].blockType != BlockTypes::LOCK) world_->blocks[i_].fg = 0;
												if (world_->blocks[i_].bg != 0) world_->blocks[i_].bg = 0;

												world_->blocks[i_].id = 0;
												world_->blocks[i_].c_ = 0;
												world_->blocks[i_].pr = 0;
											}
										}
										for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
											world_->sbox1[i_].count = 0;
										}
										for (int i_ = 0; i_ < block_->donates.size(); i_++) {
											block_->donates[i_].count = 0;
										}
									}

									world_->drop.clear();

									{
										ENetPacket* packet = Variant{ "OnAddNotification" }.push("interface/large/jump_icon.rttex", "`0" + player::algorithm::get_name(peer) + "`0 has just cleared the world!", "audio/gate_close.wav", 0).pack();
										ENetPacket* consolePacket = Variant{ "OnConsoleMessage" }.push("`0" + player::algorithm::get_name(peer) + "`0 has just cleared the world!").pack();

										player::algorithm::loop_world(world_->name, [&](ENetPeer* currentPeer) {
											enet_peer_send(currentPeer, 0, packet);
											enet_peer_send(currentPeer, 0, consolePacket);
											exit_(currentPeer);
											});
									}
								}
								else if (cch.find("buttonClicked|oan_") != string::npos) {
									if (pInfo(peer)->mod) {
										try {
											string username = "";
											if (to_lower(pInfo(peer)->last_wrenched) == "iDems") break;
											long long int seconds = atoi(cch.substr(63, cch.length() - 63).c_str());
											string reason = cch.substr(72 + to_string(seconds).length(), cch.length() - 72 + to_string(seconds).length()).c_str();
											replace_str(reason, "\n", "");
											writelog(pInfo(peer)->tankIDName + " " + (seconds == 0 ? "unbanned" : "banned") + " (" + reason + ") - " + pInfo(peer)->last_wrenched);
											if (reason.length() <= 0) {
												Algorithm::send_overlay(peer, "Please provide reason");
												break;
											}
											username = pInfo(peer)->last_wrenched;
											vector<string> _bans;
											ifstream ifs("database/players/" + username + "_.json");
											int isBanned = 0, ban_second = 0, ban_time = 0, seconds2 = 0, userid = 0;
											if (ifs.is_open()) {
												json j;
												ifs >> j;
												isBanned = j["b_t"].get<int>();
												userid = j["userID"].get<int>();
												username = j["name"].get<string>();
												_bans = j["bans"].get<vector<string>>();
											}
											if (isBanned != 0 && pInfo(peer)->dev) {
												_bans.push_back("`6ON:`` `#" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` `0(" + pInfo(peer)->requestedName + ") #" + "(unbanned by : " + pInfo(peer)->tankIDName + ") ");
												ifstream in("database/players/" + username + "_.json");
												json infile = json::parse(in);
												infile["b_t"] = 0;
												infile["b_s"] = 0;
												infile["b_r"] = "";
												infile["bans"] = _bans;
												ofstream out("database/players/" + username + "_.json");
												out << setw(4) << infile << endl;
												in.close();
												out.close();
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`o>> Success unbanned `5" + username);
												p.CreatePacket(peer);
												send_server_logs(pInfo(peer)->tankIDName, "(#" + to_string(pInfo(peer)->uid) + ") unbanned " + username + " (#" + to_string(userid) + ")");
												Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") unbanned " + username + " (#" + to_string(userid) + ")");
											}
											else {
												ifstream in("database/players/" + username + "_.json");
												json infile = json::parse(in);
												if (seconds == 729) infile["b_s"] = (6.307e+7 * 1000);
												else if (seconds == 31) infile["b_s"] = (2.678e+6 * 1000);
												else infile["b_s"] = (seconds * 1000);
												infile["b_r"] = reason;
												infile["b_b"] = pInfo(peer)->modName;
												infile["b_t"] = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
												ofstream out("database/players/" + username + "_.json");
												out << setw(4) << infile << endl;
												in.close();
												out.close();
												if (seconds == 729) seconds = 6.307e+7;
												if (seconds == 31) seconds = 2.678e+6;
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`#**`` `$The Ancient Ones`` have `4banned`` " + username + "`` `#**`` (`4/rules`` to see the rules!)");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													p.CreatePacket(currentPeer);
												}
												Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + username + " (#" + to_string(userid) + ")");
												send_server_logs(pInfo(peer)->tankIDName, "(#" + to_string(pInfo(peer)->uid) + ") banned " + username + " reason " + reason);
											}
										}
										catch (exception& e) {
											tayo_warn(e.what());
											break;
										}
									}
									break;
								}
								else if (cch.find("buttonClicked|tape1hour") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											add_mute(currentPeer, 3600, "Profanity", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") muted " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used tape (profanity) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|curse12hour") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											add_curse(currentPeer, 43200, "Scamming", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") cursed " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used curse (sanction for scamming) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|dgh1d") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											add_ban(currentPeer, 86400, "No Reason Provided", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used ban (type: 1 day DGH) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|dgh1w") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											add_ban(currentPeer, 604800, "No Reason Provided", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used ban (type: 1 week DGH) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|dgh2w") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											add_ban(currentPeer, 1210000, "No Reason Provided", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used ban (type: 2 week DGH) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|dgh4w") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											add_ban(currentPeer, 2592000, "Usage of 3rd Party Program", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used ban (type: 4 week DGH) to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|autoban") != string::npos) {
									if (to_lower(pInfo(peer)->last_wrenched) == "iDems") {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("It seems you have lower adminLevel than the user you try to punish.");
										p.CreatePacket(peer);
										break;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
											if (get_role_level(peer) <= get_role_level(currentPeer)) break;
											//if (pInfo(peer)->mod and not pInfo(peer)->admin and pInfo(currentPeer)->dev or pInfo(currentPeer)->supermod or pInfo(currentPeer)->admin) break;
											add_ban(currentPeer, 5184000, "Usage of 3rd Party Program", "System", true);
											Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											send_user_logs(pInfo(peer)->tankIDName, "used Auto-Ban to " + pInfo(currentPeer)->tankIDName);
										}
									}
								}
								else if (cch.find("buttonClicked|ipban") != string::npos) {
									if (pInfo(peer)->supermod == 1 || pInfo(peer)->admin == 1) {
										bool isOnline = false;
										if (to_lower(pInfo(peer)->last_wrenched) == "iDems") break;
										string his_ip = "";
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
												if (get_role_level(peer) <= get_role_level(currentPeer)) break;
												isOnline = true;
												his_ip = pInfo(currentPeer)->ip;
												add_ban(currentPeer, 6.307e+7, "Usage of 3rd Party Program", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
												writelog(pInfo(peer)->tankIDName + " IP Banned (" + pInfo(currentPeer)->ip + ") - " + pInfo(currentPeer)->tankIDName);
												add_ipban(currentPeer);
												add_ridban(currentPeer);
												Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") device banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
											}
										}
										if (not his_ip.empty()) {
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->ip == his_ip) {
													isOnline = true;
													add_ban(currentPeer, 6.307e+7, "Usage of 3rd Party Program", (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "``");
													Algorithm::log_text("PunishLog", "[" + Algorithm::get_time() + "] " + pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") device banned " + pInfo(currentPeer)->tankIDName + " (#" + to_string(pInfo(currentPeer)->uid) + ")");
												}
											}
										}
										if (not isOnline) {
											try {
												string username = to_lower(pInfo(peer)->last_wrenched);
												if (to_lower(pInfo(peer)->last_wrenched) == "iDems") break;
												ifstream ifs("database/players/" + username + "_.json");
												string hisIP = "", hisRID = "";
												if (ifs.is_open()) {
													json j;
													ifs >> j;
													hisIP = j["ip"].get<string>();
													hisRID = j["rid"].get<string>();
												}
												{
													vector<string> ipbans;
													ifstream ifs("database/ipban.json");
													if (ifs.is_open()) {
														json j;
														ifs >> j;
														vector<string> ipbans2 = j["ip"];
														ipbans = ipbans2;
													}
													ipbans.push_back(hisIP);
													ofstream o("database/ipban.json");
													if (!o.is_open()) break;
													json j;
													j["ip"] = ipbans;
													o << j << endl;
												}
												{
													vector<string> bans;
													ifstream ifs("database/ridban.json");
													if (ifs.is_open()) {
														json j;
														ifs >> j;
														vector<string> bans2 = j["rid"];
														bans = bans2;
													}
													bans.push_back(hisRID);
													ofstream o("database/ridban.json");
													if (!o.is_open()) break;
													json j;
													j["rid"] = bans;
													o << j << endl;
												}
												ifstream in("database/players/" + username + "_.json");
												json infile = json::parse(in);
												infile["b_s"] = (6.307e+7 * 1000);
												infile["b_r"] = "Usage of 3rd Party Program";
												infile["b_b"] = pInfo(peer)->modName;
												infile["b_t"] = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
												ofstream out("database/players/" + username + "_.json");
												out << setw(4) << infile << endl;
												in.close();
												out.close();
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`#**`` `$The Ancient Ones`` have `4Banned`` " + username + "`` `#**`` (`4/rules`` to see the rules!)");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													p.CreatePacket(currentPeer);
												}
												writelog(pInfo(peer)->tankIDName + " IP Banned (" + hisIP + ") - " + username);
											}
											catch (exception& e) {
												tayo_warn(e.what());
											}
										}
									}
									break;
								}
								else if (cch.find("buttonClicked|view_inventory") != string::npos) {
									if (pInfo(peer)->mod == 1 || pInfo(peer)->admin == 1) {
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
												string recently_visited = "";
												for (auto it = pInfo(currentPeer)->last_visited_worlds.rbegin(); it != pInfo(currentPeer)->last_visited_worlds.rend(); ++it) {
													string a_ = *it + (next(it) != pInfo(currentPeer)->last_visited_worlds.rend() ? "``, " : "``");
													recently_visited += "`#" + a_;
												}
												string inventory = "";
												int thats5 = 0, thatsadded = 0;
												for (int i_ = 0; i_ < pInfo(currentPeer)->inv.size(); i_++) {
													if (pInfo(currentPeer)->inv[i_].id == 0 || pInfo(currentPeer)->inv[i_].id == 18 || pInfo(currentPeer)->inv[i_].id == 32) continue;
													thats5++;
													thatsadded = 0;
													inventory += "\nadd_button_with_icon|" + (pInfo(peer)->owner == 1 ? to_string(pInfo(currentPeer)->inv[i_].id) : "") + "||staticBlueFrame|" + to_string(pInfo(currentPeer)->inv[i_].id) + "|" + to_string(pInfo(currentPeer)->inv[i_].count) + "|";
													if (thats5 >= 6) {
														thats5 = 0;
														thatsadded = 1;
														inventory += "\nadd_button_with_icon||END_LIST|noflags|0||";
													}
												}
												gamepacket_t p;
												p.Insert("OnDialogRequest");
												p.Insert("set_default_color|`o\nadd_label_with_icon|small|`0Inventory of " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "``'s (" + pInfo(currentPeer)->requestedName + ") - #" + to_string(pInfo(currentPeer)->netID) + "|left|3802|\nadd_spacer|small|\nadd_textbox|Last visited: " + recently_visited + "|\nadd_textbox|Gems: `w" + setGems(pInfo(currentPeer)->gems) + "|\nadd_textbox|Backpack slots: `w" + to_string(pInfo(currentPeer)->inv.size() - 1) + "|" + inventory + "" + (thatsadded == 1 ? "" : "\nadd_button_with_icon||END_LIST|noflags|0||") + "|\nend_dialog|view_inventory|Continue||\nadd_quick_exit|");
												p.CreatePacket(peer);
											}
										}
									}
									break;
								}
								else if (cch.find("buttonClicked|warp_to_") != string::npos) {
									if (pInfo(peer)->last_world_enter + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(peer)->last_world_enter = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										string world_name = cch.substr(67, cch.length() - 67);
										vector<string> test = split_string_by_newline(world_name);
										join_world(peer, test[0]);
									}
									break;
								}
								break;
							}
							break;
						}
						
						else if (cch.find("action|dialog_return\ndialog_name|blast\nname|") != string::npos) {
							if (pInfo(peer)->lastchoosenitem == 830 || pInfo(peer)->lastchoosenitem == 9164 || pInfo(peer)->lastchoosenitem == 9602 || pInfo(peer)->lastchoosenitem == 942 || pInfo(peer)->lastchoosenitem == 1060 || pInfo(peer)->lastchoosenitem == 1136 || pInfo(peer)->lastchoosenitem == 1402 || pInfo(peer)->lastchoosenitem == 9582 || pInfo(peer)->lastchoosenitem == 1532 || pInfo(peer)->lastchoosenitem == 3562 || pInfo(peer)->lastchoosenitem == 4774 || pInfo(peer)->lastchoosenitem == 7380 || pInfo(peer)->lastchoosenitem == 5176 || pInfo(peer)->lastchoosenitem == 7588 || pInfo(peer)->lastchoosenitem == 9488 || pInfo(peer)->lastchoosenitem == 8556) {
								int blast = pInfo(peer)->lastchoosenitem, got = 0;
								modify_inventory(peer, blast, got);
								if (got == 0) break;
								string world = cch.substr(44, cch.length() - 44).c_str();
								replace_str(world, "\n", "");
								transform(world.begin(), world.end(), world.begin(), ::toupper);
								if (find_if(worlds.begin(), worlds.end(), [world](const World& a) { return a.name == world; }) != worlds.end() || not check_blast(world) || _access_s(("database/worlds/" + world + "_.json").c_str(), 0) == 0) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("That world name already exists. You'll have to be more original. Maybe add some numbers after it?"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
								}
								else {
									if (modify_inventory(peer, blast, got = -1) == 0) {
										if (blast == 1532) create_undersea(world, 100, 60);
										else if (blast == 830) modify_inventory(peer, 834, got = -100), create_beach(world, 100, 60);
										else if (blast == 1136) create_mars(world, 100, 60);
									    else if (blast == 5176) create_island(world, 200, 200, 60);
										else create_world_blast(peer, world, blast);
										join_world(peer, world);
										//pInfo(peer)->worlds_owned.push_back(world);
										gamepacket_t p(750), p2(750);
										p.Insert("OnConsoleMessage"), p.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("** `5" + pInfo(peer)->tankIDName + " activates a " + items[blast].name + "! ``**"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|consumedgem\nitemID|5136|\ncount|") != string::npos) {
							string itemCount = cch.substr(64, cch.length() - 64).c_str();
							int intItemCount = atoi(itemCount.c_str());
							if (intItemCount <= 0) break;
							int got = 0;
							int remove = -1 * intItemCount;
							modify_inventory(peer, 5136, got);
							if (got == 0) break;
							if (got < intItemCount) break;
							if (modify_inventory(peer, 5136, remove) == 0) {
								int total = intItemCount * 1000;
								pInfo(peer)->gems += total;
								{
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									}
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|consumedgem\nitemID|9148|\ncount|") != string::npos) {
							string itemCount = cch.substr(64, cch.length() - 64).c_str();
							int intItemCount = atoi(itemCount.c_str());
							if (intItemCount <= 0) break;
							int got = 0;
							int remove = -1 * intItemCount;
							modify_inventory(peer, 9148, got);
							if (got == 0) break;
							if (got < intItemCount) break;
							if (modify_inventory(peer, 9148, remove) == 0) {
								int total = intItemCount * 100000;
								pInfo(peer)->gems += total;
								{
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									}
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|megaphone\nitemID|2480|\nwords|") != string::npos) {
							string text = cch.substr(62, cch.length() - 62).c_str();
							bool cansb = true;
							int c_ = 0;
							modify_inventory(peer, 2480, c_);
							if (c_ == 0) continue;
							if (has_playmod(pInfo(peer), "duct tape")) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`6>> That's sort of hard to do while duct-taped.``");
								p.CreatePacket(peer);
								cansb = false;
								continue;
							}
							if (has_playmod(pInfo(peer), "megaphone!")) {
								int time_ = 0;
								for (PlayMods peer_playmod : pInfo(peer)->playmods) {
									if (peer_playmod.id == 13) {
										time_ = peer_playmod.time - time(nullptr);
										break;
									}
								}
								packet_(peer, "action|log\nmsg|>> (" + Algorithm::to_playmod_time(time_) + "before you can broadcast again)", "");
								break;
							}
							if (cansb) {
								replace_str(text, "\n", "");
								pInfo(peer)->usedmegaphone = 1;
								SendCmd(peer, "/sb " + text, false);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|lockemegaphone\nitemID|11230|\nwords|") != string::npos) {
							string text = cch.substr(62, cch.length() - 62).c_str();
							bool cansb = true;
							int c_ = 0;
							modify_inventory(peer, 11230, c_);
							if (c_ == 0) continue;
							if (has_playmod(pInfo(peer), "duct tape")) {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("`6>> That's sort of hard to do while duct-taped.``");
								p.CreatePacket(peer);
								cansb = false;
								continue;
							}
							if (has_playmod(pInfo(peer), "Locke's Megaphone!")) {
								int time_ = 0;
								for (PlayMods peer_playmod : pInfo(peer)->playmods) {
									if (peer_playmod.id == 13) {
										time_ = peer_playmod.time - time(nullptr);
										break;
									}
								}
								packet_(peer, "action|log\nmsg|>> (" + Algorithm::to_playmod_time(time_) + "before you can broadcast again)", "");
								break;
							}
							if (cansb) {
								replace_str(text, "\n", "");
								pInfo(peer)->usedmegaphone = 1;
								SendCmd(peer, "/lsb " + text, false);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|cancel") != string::npos || cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|clear") != string::npos) 	pInfo(peer)->note = "";
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|notebook_edit\nbuttonClicked|save\n\npersonal_note|") != string::npos) {
							string btn = cch.substr(81, cch.length() - 81).c_str();
							replace_str(btn, "\n", "");
							if (btn.length() > 128) continue;
							pInfo(peer)->note = btn;
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|3898\nbuttonClicked|") != string::npos || cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") {
							string btn = cch.substr(52, cch.length() - 52).c_str();
							if (cch == "action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|53785\n\n" || cch == "action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|53785\n\n") btn = "53785";
							replace_str(btn, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (btn == "12345") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Quest Emporium``|left|3902|\nadd_textbox|HEEEEYYY there Growtopian! I'm Crazy Jim, and my quests are so crazy they're KERRRRAAAAZZY!! And that is clearly very crazy, so please, be cautious around them. What can I do ya for, partner?|left|\nadd_button|chc1_1|Daily Quest|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "53785") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|"
								"\nadd_textbox|It is I, Sales-Man, savior of the wealthy! Let me rescue you from your riches. What would you like to buy today?|left|"
								"\nadd_button|chc4_1|Surgery Items|noflags|0|0|"
								/*\nadd_button|chc3_1|Zombie Defense Items|noflags|0|0|*/
								"\nadd_button|chc2_1|Blue Gem Lock|noflags|0|0|"
								"\nadd_button|ml_lock|Tayo Lock|noflags|0|0|"
								"\nadd_button|chc309_1|Golden Halo Gems|noflags|0|0|"
								"\nadd_button|chc69_1|Purchase Cheat Access|noflags|0|0|\nadd_button|chc0_1|Diamond Lock|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							else if (btn == "chc309_1") {
								int c_ = 0;
								modify_inventory(peer, 5136, c_);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wGolden Halo Gems``|left|9148|\nadd_textbox|Excellent! I'm happy to sell you a Golden Halo Gems in exchange for 100 Digi Gems..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Digi Gems.``|left|" + (c_ >= 100 ? "\nadd_button|chc309_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc309_2_1") {
								int c9148 = 0, c5136 = 0, additem = 0;
								modify_inventory(peer, 5136, c5136);
								if (c5136 < 100) continue;
								modify_inventory(peer, 9148, c9148);
								if (c9148 >= 200) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You don't have room in your backpack!");
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have room in your backpack!");
										p.CreatePacket(peer);
									}
									continue;
								}
								if (c5136 >= 100) {
									if (get_free_slots(pInfo(peer)) >= 2) {
										int cz_ = 1;
										if (modify_inventory(peer, 5136, additem = -100) == 0) {
											modify_inventory(peer, 9148, additem = 1);
											{
												{
													string name_ = pInfo(peer)->world;
													vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
													if (p != worlds.end()) {
														World* world_ = &worlds[p - worlds.begin()];
														PlayerMoving data_{};
														data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
														data_.packetType = 19, data_.plantingTree = 500;
														data_.punchX = 9148, data_.punchY = pInfo(peer)->netID;
														int32_t to_netid = pInfo(peer)->netID;
														BYTE* raw = packPlayerMoving(&data_);
														raw[3] = 5;
														memcpy(raw + 8, &to_netid, 4);
														for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
															if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
															if (pInfo(currentPeer)->world == world_->name) {
																send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
															}
														}
														delete[] raw;
													}
												}
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("[`6You spent 100 Digi Gems to get 1 Golden Halo Gems``]");
												p.CreatePacket(peer);
											}
										}
										int c_ = 0;
										modify_inventory(peer, 5136, c_);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wGolden Halo Gems``|left|9148|\nadd_textbox|Excellent! I'm happy to sell you a Golden Halo Gems in exchange for 100 Digi Gems..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Digi Gems.``|left|" + (c_ >= 100 ? "\nadd_button|chc309_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have enough inventory space!");
										p.CreatePacket(peer);
									}
								}
							}
							else if (btn == "chc1_1") {
								if (!pInfo(peer)->AlreadyDailyQ) {
									int received = 0;
									//modify_inventory(peer, item1, haveitem1);
									//modify_inventory(peer, item2, haveitem2);
									//if (haveitem1 >= item1c && haveitem2 >= item2c) received = 1;
									//if (haveitem1 >= item1c && haveitem2 >= item2c && has_playmod(pInfo(peer), "Delectable") && rand() % 50 + 1 < 25) received = 2;
									if (pInfo(peer)->gems >= gems_c) received = 1;
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|I guess some people call me Crazy Jim because I'm a bit of a hoarder. But I'm very particular about what I want! And today, what I want is this:|left|\nadd_label_with_icon|small|`2" + setGems(gems_c) + " gems.``|left|112|"
										//"|\nadd_smalltext|and|left|\nadd_label_with_icon|small|`2" + to_string(item2c) + " " + items[item2].name + "|left|" + to_string(item2) + 
										"\nadd_spacer|small|\nadd_smalltext|You shove all that through the phone (it works, I've tried it), and I will hand you one of the `2Growtokens`` from my personal collection!  But hurry, this offer is only good until midnight, and only one `2Growtoken`` per person!|left|\nadd_spacer|small|"
										"\nadd_smalltext|`6(You have " + setGems(gems_c) + " gems)``|left|\nadd_spacer|small|" + (received == 1 ? "\nadd_button|turnin|Turn in items|noflags|0|0|" : "") + "\nadd_spacer|small|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								}
								else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc69_1") {
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|Great choice! I'll display all available cheats. So, which cheats would you buy today? Beware of choosing the cheat, once you clicked it you'll automatically get it and will never get any refund.|left|\nadd_button|cheat_1|Cheat Autofarm|noflags|0|0|"
									//"\nadd_button|cheat_2|Cheat Autospam|noflags|0|0|"
									"\nadd_button|cheat_3|Cheat Fast Spin|noflags|0|0|"
								//"\nadd_button|cheat_4|Cheat Hide Players|noflags|0|0|"
									"\nadd_button|cheat_5|Cheat Hide Chats|noflags|0|0|\nadd_button|cheat_6|Cheat Auto Collect Gems|noflags|0|0|\nadd_button|cheat_7|Cheat Fast Drop|noflags|0|0|\nadd_button|cheat_8|Cheat Fast Trash|noflags|0|0|\nadd_button|cheat_9|Cheat Fast Pull|noflags|0|0|\nadd_button|cheat_10|Cheat Antibounce|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else if (btn.starts_with("cheat_")) {
								int num = atoi(explode("_", btn)[1].c_str());
								p.Insert(cheat_offer(peer, num));
							}
							else if (btn.starts_with("purchase_cheat_")) {
								int num = atoi(explode("cheat_", btn)[1].c_str());
								cheat_purchase(peer, num);
								break;
							}
							else if (btn == "turnin") {
								if (!pInfo(peer)->AlreadyDailyQ) {
									int haveitem1 = 0, haveitem2 = 0, received = 0, remove = -1, remove2 = -1, giveitem = 10;
									//modify_inventory(peer, item1, haveitem1);
									//modify_inventory(peer, item2, haveitem2);
									if (rand() % 100 + 1 < 25 and has_playmod(pInfo(peer), "Delectable") and 
										pInfo(peer)->gems >= gems_c) received = 2;
									else if (pInfo(peer)->gems >= gems_c) received = 1;
									int giveToken = received;
									if (received != 0) {
										if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress += received;
											if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
												pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
										if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
											pInfo(peer)->quest_progress += received;
											if (pInfo(peer)->quest_progress >= 28) {
												pInfo(peer)->quest_progress = 28;
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
										pInfo(peer)->AlreadyDailyQ = true;
										pInfo(peer)->gems -= gems_c;
										modify_inventory(peer, 1486, giveToken);
										{
											{
												string name_ = pInfo(peer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = 1486, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
														if (pInfo(currentPeer)->world == world_->name) {
															gamepacket_t p3;
															p3.Insert("OnParticleEffect");
															p3.Insert(198);
															p3.Insert((float)pInfo(peer)->x + 10, (float)pInfo(peer)->y + 16);
															p3.CreatePacket(currentPeer);
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														}
													}
													delete[] raw;
												}
											}
											gamepacket_t p, p4;
											p.Insert("OnConsoleMessage");
											p.Insert("[`6You jammed " + setGems(gems_c) + " gems into the phone, and " + to_string(received) + " `2Growtoken`` popped out!``]");
											p4.Insert("OnTalkBubble");
											p4.Insert(pInfo(peer)->netID);
											p4.Insert("Thanks, pardner! Have " + to_string(received) + " `2Growtoken`w!");
											p4.Insert(0), p4.Insert(0);
											p.CreatePacket(peer), p4.CreatePacket(peer);
										}
									}
								}
								else {
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wCrazy Jim's Daily Quest``|left|3902|\nadd_textbox|You've already completed my Daily Quest for today! Call me back after midnight to hear about my next cravings.|left|\nadd_button|12345|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									p.CreatePacket(peer);
								}
							}
							else if (btn == "chc0_1") {
								int c_ = pInfo(peer)->gems;
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDiamond Lock``|left|1796|\nadd_textbox|Excellent! I'm happy to sell you a Diamond Lock in exchange for 200.000 Gems.|left|\nadd_smalltext|`6You have " + fixint(c_) + " Gems.``|left|" + (c_ >= 200000 ? "\nadd_button|chc0_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc0_2_1") {
								int c_ = pInfo(peer)->gems, c1796 = 0, additem = 0;
								modify_inventory(peer, 1796, c1796);
								if (c_ < 200000) continue;
								if (c1796 >= 200) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You don't have room in your backpack!");
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have room in your backpack!");
										p.CreatePacket(peer);
									}
									continue;
								}
								if (c_ >= 200000) {
									if (get_free_slots(pInfo(peer)) >= 2) {
										int cz_ = 200000;
										if (modify_inventory(peer, 1796, additem = 1) == 0) {
											{
												{
													pInfo(peer)->gems -= cz_;
													string name_ = pInfo(peer)->world;
													vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
													if (p != worlds.end()) {
														gamepacket_t p8;
														p8.Insert("OnSetBux");
														p8.Insert(pInfo(peer)->gems);
														p8.Insert(0);
														p8.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
														if (pInfo(peer)->supp >= 2) {
															p8.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
														}
														p8.CreatePacket(peer);
														World* world_ = &worlds[p - worlds.begin()];
														PlayerMoving data_{};
														data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
														data_.packetType = 19, data_.plantingTree = 500;
														data_.punchX = 1796, data_.punchY = pInfo(peer)->netID;
														int32_t to_netid = pInfo(peer)->netID;
														BYTE* raw = packPlayerMoving(&data_);
														raw[3] = 5;
														memcpy(raw + 8, &to_netid, 4);
														for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
															if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
															if (pInfo(currentPeer)->world == world_->name) {
																send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
															}
														}
														delete[] raw;
													}
												}
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("[`6You spent 200.000 Gems to get 1 Diamond Lock``]");
												p.CreatePacket(peer);
											}
										}
										int c_ = pInfo(peer)->gems;
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDiamond Lock``|left|1796|\nadd_textbox|Excellent! I'm happy to sell you a Diamond Lock in exchange for 200.000 Gems.|left|\nadd_smalltext|`6You have " + fixint(c_) + " Gems.``|left|" + (c_ >= 200000 ? "\nadd_button|chc0_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have enough inventory space!");
										p.CreatePacket(peer);
									}
								}
							}
							else if (btn == "chc2_1") {
								int c_ = 0;
								modify_inventory(peer, 1796, c_);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "ml_lock") {
							int c_ = 0, c_1 = 0;
							modify_inventory(peer, 7188, c_);
							modify_inventory(peer, 8470, c_1);
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTayo Lock``|left|8470|\nadd_textbox|Excellent! I'm happy to sell you a Tayo Lock in exchange for 100 Blue Gem Locks and buy your Tayo Lock for 100 Blue Gem Locks..|left|"
								"\nadd_smalltext|`6You have " + setGems(c_) + " Blue Gem Locks and " + setGems(c_1) + " Tayo Locks.``|left|" + (c_ >= 100 ? "\nadd_button|ml_lock_1|Compress the lock!|noflags|0|0|" : "") + (c_1 >= 1 ? "\nadd_button|ml_lock_2|Break the lock!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							}
							else if (btn == "chc2_2_1") {
								int c7188 = 0, c1796 = 0, additem = 0;
								modify_inventory(peer, 1796, c1796);
								if (c1796 < 100) continue;
								modify_inventory(peer, 7188, c7188);
								if (c7188 >= 200) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You don't have room in your backpack!");
									p.Insert(0), p.Insert(1);
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have room in your backpack!");
										p.CreatePacket(peer);
									}
									continue;
								}
								if (c1796 >= 100) {
									if (get_free_slots(pInfo(peer)) >= 2) {
										int cz_ = 1;
										if (modify_inventory(peer, 1796, additem = -100) == 0) {
											modify_inventory(peer, 7188, additem = 1);
											{
												{
													string name_ = pInfo(peer)->world;
													vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
													if (p != worlds.end()) {
														World* world_ = &worlds[p - worlds.begin()];
														PlayerMoving data_{};
														data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
														data_.packetType = 19, data_.plantingTree = 500;
														data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
														int32_t to_netid = pInfo(peer)->netID;
														BYTE* raw = packPlayerMoving(&data_);
														raw[3] = 5;
														memcpy(raw + 8, &to_netid, 4);
														for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
															if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
															if (pInfo(currentPeer)->world == world_->name) {
																send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
															}
														}
														delete[] raw;
													}
												}
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("[`6You spent 100 Diamond Lock to get 1 Blue Gem Lock``]");
												p.CreatePacket(peer);
											}
										}
										int c_ = 0;
										modify_inventory(peer, 1796, c_);
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBlue Gem Lock``|left|7188|\nadd_textbox|Excellent! I'm happy to sell you a Blue Gem Lock in exchange for 100 Diamond Lock..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Diamond Lock.``|left|" + (c_ >= 100 ? "\nadd_button|chc2_2_1|Thank you!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You don't have enough inventory space!");
										p.CreatePacket(peer);
									}
								}
							}
							else if (btn == "ml_lock_2") {
							int c7188 = 0, c8470 = 0, additem = 0;
							modify_inventory(peer, 7188, c7188);
							modify_inventory(peer, 8470, c8470);
							if (c8470 < 1) continue;
							if (c7188 >= 200) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("You don't have room in your backpack!");
								p.Insert(0), p.Insert(1);
								p.CreatePacket(peer);
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have room in your backpack!");
									p.CreatePacket(peer);
								}
								continue;
							}
							if (c8470 >= 1) {
								if (get_free_slots(pInfo(peer)) >= 2) {
									int cz_ = 1;
									if (modify_inventory(peer, 7188, additem = 100) == 0) {
										modify_inventory(peer, 8470, additem = -1);
										{
											{
												string name_ = pInfo(peer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
														if (pInfo(currentPeer)->world == world_->name) {
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														}
													}
													delete[] raw;
												}
											}
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("[`6You got 100 Blue Gem Locks from breaking 1 Tayo Lock``]");
											p.CreatePacket(peer);
										}
									}
									int c_ = 0;
									modify_inventory(peer, 8470, c_);
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTayo Lock``|left|8470|\nadd_textbox|Excellent! I'm happy to buy your Tayo Lock for 100 Blue Gem Locks each..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Tayo Lock.``|left|" + (c_ >= 1 ? "\nadd_button|ml_lock_2|Break the lock!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough inventory space!");
									p.CreatePacket(peer);
								}
							}
							}
							else if (btn == "ml_lock_1") {
							int c7188 = 0, c8470 = 0, additem = 0;
							modify_inventory(peer, 7188, c7188);
							if (c7188 < 100) continue;
							modify_inventory(peer, 8470, c8470);
							if (c8470 >= 200) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("You don't have room in your backpack!");
								p.Insert(0), p.Insert(1);
								p.CreatePacket(peer);
								{
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have room in your backpack!");
									p.CreatePacket(peer);
								}
								continue;
							}
							if (c7188 >= 100) {
								if (get_free_slots(pInfo(peer)) >= 2) {
									int cz_ = 1;
									if (modify_inventory(peer, 7188, additem = -100) == 0) {
										modify_inventory(peer, 8470, additem = 1);
										{
											{
												string name_ = pInfo(peer)->world;
												vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
												if (p != worlds.end()) {
													World* world_ = &worlds[p - worlds.begin()];
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = 8470, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
														if (pInfo(currentPeer)->world == world_->name) {
															send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														}
													}
													delete[] raw;
												}
											}
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("[`6You spent 100 Blue Gem Locks to get 1 Tayo Lock``]");
											p.CreatePacket(peer);
										}
									}
									int c_ = 0;
									modify_inventory(peer, 7188, c_);
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wTayo Lock``|left|8470|\nadd_textbox|Excellent! I'm happy to sell you a Tayo Lock in exchange for 100 Blue Gem Locks..|left|\nadd_smalltext|`6You have " + to_string(c_) + " Blue Gem Lock.``|left|" + (c_ >= 100 ? "\nadd_button|ml_lock_1|Compress the lock!|noflags|0|0|" : "") + "\nadd_button|53785|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("You don't have enough inventory space!");
									p.CreatePacket(peer);
								}
							}
							}
							else if (btn == "chc3_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4450, zombie_brain);
								modify_inventory(peer, 4452, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Zombie Defense``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you Zombie Defense supplies in exchange for Zombie Brains.|left|\nadd_smalltext|You currently have " + setGems(total) + " Zombie Brains.|left|\nadd_spacer|small|\ntext_scaling_string|5,000ZB|\n" + zombie_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zombie_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else if (btn == "chc4_1") {
								int zombie_brain = 0, pile = 0, total = 0;
								modify_inventory(peer, 4450, zombie_brain);
								modify_inventory(peer, 4300, pile);
								total += zombie_brain + (pile * 100);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man: Surgery``|left|4358|\nadd_textbox|Excellent! I'm happy to sell you rare and precious Surgery prizes in exchange for Caduceus medals.|left|\nadd_smalltext|You currently have " + setGems(total) + " Caducei.|left|\nadd_spacer|small|\ntext_scaling_string|5,000ZB|\n" + surgery_list + "\nadd_button_with_icon||END_LIST|noflags|0||\nadd_button|53785|Back|noflags|0|0|\nend_dialog|zurgery_back|Hang Up||\n");
								p.CreatePacket(peer);
							}
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wDisconnected``|left|774|\nadd_textbox|The number you have tried to reach is disconnected. Please check yourself before you wreck yourself.|left|\nend_dialog|3898|Hang Up||\n");
							if (btn != "turnin") p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|top\nbuttonClicked|warp_to_") != string::npos) {
							string world_name = cch.substr(59, cch.length() - 59);
							replace_str(world_name, "\n", "");
							join_world(peer, world_name);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|gtoken_packet_1") != string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase 5 Growtoken``|left|1486|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `15 TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|`wNo special effects|left|\nadd_spacer|\nadd_button|buy_token_1|`0Purchase for `15 TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|gtoken_packet_2") != string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase 10 Growtoken``|left|1486|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `110 TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|`wNo special effects|left|\nadd_spacer|\nadd_button|buy_token_2|`0Purchase for `110 TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|gtoken_packet_3") != string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase 50 Growtoken``|left|1486|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `150 TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|`wNo special effects|left|\nadd_spacer|\nadd_button|buy_token_3|`0Purchase for `150 TayoPoint``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|gtoken_packet_4") != string::npos) {
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase 1 Mega Growtoken``|left|6802|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1100 TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|`wThis item contain 100 Growtoken|left|\nadd_spacer|\nadd_button|buy_token_4|`0Purchase for `1100 gnettoken``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|fish_center\nbuttonClicked|confirm_craft_") != string::npos) {
						    if (pInfo(peer)->lastwrenchb != 9158) break;
						    auto data = atoi(explode("confirm_craft_", cch)[1].c_str());
							int got = 0, got1 = 0, got2 = 0, c_ = 1;
							if (data <= 0 || data >= items.size() || items[data].craft_price == 0) continue;
							modify_inventory(peer, items[data].craft1.first, got);
							modify_inventory(peer, items[data].craft2.first, got1);
							modify_inventory(peer, items[data].craft3.first, got2);

							if (got < items[data].craft1.second or got1 < items[data].craft2.second or got2 < items[data].craft3.second) {
								Algorithm::send_console(peer, "You don't have enough item to do this.");
								break;
							}

							if (pInfo(peer)->gems < items[data].craft_price) {
								Algorithm::send_console(peer, "You don't have enough gems to do this.");
								break;
							}

							if (modify_inventory(peer, data, c_) == 0) {
								Algorithm::send_console(peer, "Crafted 1 " + items[data].name);
								pInfo(peer)->gems -= items[data].craft_price;
								modify_inventory(peer, items[data].craft1.first, got *= -1);
								modify_inventory(peer, items[data].craft2.first, got1 *= -1);
								modify_inventory(peer, items[data].craft3.first, got2 *= -1);
							}
							else {
								Algorithm::send_console(peer, "Failed to add " + items[data].name + " to your inventory.");
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|fish_center\nbuttonClicked|craft_") != string::npos) {
						auto data = atoi(explode("craft_", cch)[1].c_str());

						DialogBuilder db{ "`o" };
						db.add_label_icon(true, data, format("`wCrafting {}``", items[data].name))
							.add_textbox(format("Recipe: x{} {}, x{} {}, and x{} {}", items[data].craft1.second, items[items[data].craft1.first].name,
								items[data].craft2.second, items[items[data].craft2.first].name, items[data].craft3.second, items[items[data].craft3.first].name))
							.add_textbox("Are you sure you're going to do this process?")
							.add_button(format("confirm_craft_{}", data), format("`1Craft a {}``", items[data].name))
							.end_dialog("fish_center", "`wClose``", "").add_quick_exit();
						gamepacket_t p;
						p.Insert("OnDialogRequest"), p.Insert(db.to_string()), p.CreatePacket(peer);
						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shop_price_") != string::npos) {
						int item = atoi(cch.substr(59, cch.length() - 59).c_str());
						if (item <= 0 || item >= items.size() || items[item].pwl == 0/* || items[item].shop_texture_file.empty() ||
							(items[item].shop_texture_pos.first == -1 and items[item].shop_texture_pos.second == -1)*/) continue;
						string special = "`7There is none yet for that item``";
						// gnettoken shop item special effects description
						//if (item == 9160) special = "`wx3 Gems Drop and Break/Place blocks 3 row at once``";
						if (item == 3130) special = "`wMax Surgery Skill``";
						//if (item == 13026) special = "`w2 Hit on Farmables``";
						//if (item == 5192 || item == 5194) special = "`wDrop x10 Gems``";
						//if (item == 11118) special = "wDrop x2 Gems``";
						if (item == 10384) special = "`wDrop x10 Gems``";
						if (item == 13828) special = "`w20 Far Punch/Place & x10 Gems.``";
						if (item == 8532) special = "`w15 Far Punch/Place & 1 Hit on every blocks.``";
						//if (item == 11318) special = "`wBreakhits 1x on All Farmables``";
						if (item == 5480) special = "`wBreak and Place blocks 3 row at once``";
						//if (item == 8372) special = "`w1 Hit on every blocks``";
						//if (item == 7782) special = "`wDrop 25x Gems";
						//if (item == 9520) special = "Exclusive Items";
						//if (item == 13200) special = "`wx28 XP Multiplier";
						if (item == 11118 || item == 9716) special += ", Breakhits 3x on All Blocks``";
						gamepacket_t p;
						p.Insert("OnDialogRequest");
						p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + (item == 8442 ? "Doctor's Blingin Set" : items[item].name) + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `1" + setGems(items[item].pwl) + " TayoPoint``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Special Effects:``|left|\nadd_smalltext|" + special + "|left|\nadd_spacer|\nadd_button|shop_item_" + to_string(item) + "|`0Purchase for `1" + setGems(items[item].pwl) + " gnettoken``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|shop|Close|noflags|0|0|\nnend_dialog|||OK|");
						p.CreatePacket(peer);

						break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							int wl = 0, dl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + (items[item].gtwl > 200 ? to_string(items[item].gtwl / 100) : to_string(items[item].gtwl)) + " " + (items[item].gtwl > 200 ? "Diamond Lock" : "World Locks") + "``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for " + (items[item].gtwl > 200 ? to_string(items[item].gtwl / 100) : to_string(items[item].gtwl)) + " " + (items[item].gtwl > 200 ? "Diamond Lock" : "World Locks") + " each?|left|\nadd_text_input|howmuch||1|5|\nadd_smalltext|" + (wl + dl != 0 ? "You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "" : "") + "|left|\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|locke|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locm_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].u_gtwl == 0) continue;
							pInfo(peer)->lockeitem = item;
							int wl = 0, dl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`9Buy " + items[item].name + "?``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4" + items[item].description + "``|left|\nadd_smalltext|`1Price: " + setGems(items[item].u_gtwl) + " World Lock``|left|\nadd_spacer|\nadd_textbox|How many " + items[item].name + " do you want to buy, for " + (items[item].u_gtwl > 200 ? to_string(items[item].u_gtwl / 100) : to_string(items[item].u_gtwl)) + " " + (items[item].u_gtwl > 200 ? "Diamond Lock" : "World Locks") + " each?|left|\nadd_text_input|howmuch||1|5|\nadd_smalltext|" + (wl + dl != 0 ? "You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "" : "") + "|left|\nadd_button|lock_item_|`9Purchase``|noflags|0|0||small|\n\nadd_quick_exit|\nadd_button|lockm|No thanks|noflags|0|0|\nnend_dialog|gazette||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|locke") != string::npos) {
							if (pInfo(peer)->world == "LOCKE") {
								int wl = 0, dl = 0;
								modify_inventory(peer, 242, wl);
								modify_inventory(peer, 1796, dl);
								gamepacket_t p;
								p.Insert("OnDialogRequest");
								p.Insert(a + "set_default_color|`o\n\nadd_label_with_icon|big|`9Locke The Traveling Salesman``|left|2398|\nadd_spacer|small|\nadd_smalltext|Ho there, friend! Locke's my name, and locks are my game. I Love 'em all, Diamond, Huge.. even Small! If you can part with some locks, I'll give you something special in return. Whaddya say?|left|\nadd_spacer|small|\nadd_smalltext|" + (wl + dl != 0 ? "`9(Hmm, smells like you care carrying " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", and and " + to_string(dl) + " Diamond Lock" : "") + ")``" : "`9(Hmm, smells like you don't care any world locks)``") + "|left|\nadd_spacer|small|" + shop_list2 + "|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|rayman_carrer") != string::npos) {
							if (cch.find("buttonClicked|regis_ray") != string::npos) {
								int got = 0;
								modify_inventory(peer, 5480, got);
								if (got <= 0 || got == 0) {
									Algorithm::send_overlay(peer, color(peer, "red", "Oops you dont have a rayman"));
									break;
								}
								else {
									modify_inventory(peer, 5480, got = -1);
									pInfo(peer)->udah_daftar = true;
									Algorithm::send_overlay(peer, color(peer, "green", "Succesfully registration!"));
									rayman_upgrade(peer, 2);
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 46, data_.YSpeed = 46, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
								}
								break;
							}
							if (cch.find("buttonClicked|up_rayman") != string::npos) {
								vector<int> normal{ 13868, 13870, 8530, 8532 }, lray{ 13904, 13902, 13900, 13898 }, mray{13912, 13910, 13908, 13906};
								int item;
								int randNum = rand() % 100;
								if (randNum < 70) {
									item = normal[rand() % normal.size()];
								}
								else if (randNum < 90) {
									item = lray[rand() % lray.size()];
								}
								else {
									item = mray[rand() % mray.size()];
								}
								int adadl = 0, adasoul = 0, adaray = 0, adachand = 0;
								modify_inventory(peer, 1796, adadl);
								modify_inventory(peer, 5202, adasoul);
								modify_inventory(peer, 5480, adaray);
								modify_inventory(peer, 340, adachand);
								if (adadl <= 9 && adasoul <= 0 && adaray <= 0 && adachand <= 199) {
									Algorithm::send_overlay(peer, color(peer, "red", "No no is possible you upgrade"));
									break;
								}
								else {
									int dapat = 1;
									pInfo(peer)->udahan = 1;
									modify_inventory(peer, 1796, adadl = -10);
									modify_inventory(peer, 5202, adasoul = -1);
									modify_inventory(peer, 5480, adaray = -1);
									modify_inventory(peer, 340, adachand = -200);
									modify_inventory(peer, item, dapat);
									gamepacket_t p;
									p.Insert("OnTextOverlay");
									p.Insert("`9Quest completed!!");
									p.CreatePacket(peer);
									SendCmd(peer, "/cheer", true);
									equip_clothes(peer, item);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											gamepacket_t p3, p4;
											p3.Insert("OnParticleEffect");
											p3.Insert(73);
											p3.Insert((float)pInfo(peer)->x + 10, (float)pInfo(peer)->y + 16);
											p4.Insert("OnTalkBubble");
											p4.Insert(pInfo(peer)->netID);
											p4.Insert((pInfo(peer)->mod && pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + " `5earned the " + items[item].name + "'!");
											p4.Insert(2), p4.Insert(1);
											p3.CreatePacket(currentPeer), p4.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/achievement.wav\ndelayMS|0");
										}
									}
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|lock_item_\n\nhowmuch|") != string::npos) {
							if (pInfo(peer)->world == "LOCKE") {
								int count = atoi(cch.substr(68, cch.length() - 68).c_str()), count2 = atoi(cch.substr(68, cch.length() - 68).c_str());
								if (count <= 0 || count > 200) continue;
								int item = pInfo(peer)->lockeitem;
								if (item <= 0 || item >= items.size()) continue;
								if (items[item].gtwl == 0 and items[item].u_gtwl == 0) continue;
								int allwl = 0, wl = 0, dl = 0, price = (items[item].gtwl == 0 ? items[item].u_gtwl : items[item].gtwl), priced = 0, bgl = 0;
								price *= count;
								priced = price;
								modify_inventory(peer, 242, wl);
								modify_inventory(peer, 1796, dl);
								modify_inventory(peer, 7188, bgl);
								allwl = wl + (dl * 100);
								int allbgl = bgl * 10000;
								if (allwl >= price || allbgl >= price && price > 20000) {
									int c_ = count;
									if (modify_inventory(peer, item, c_) == 0) {
										if (price <= 20000) {
											if (wl >= price) modify_inventory(peer, 242, price *= -1);
											else {
												modify_inventory(peer, 242, wl *= -1);
												modify_inventory(peer, 1796, dl *= -1);
												int givedl = (allwl - price) / 100;
												int givewl = (allwl - price) - (givedl * 100);
												modify_inventory(peer, 242, givewl);
												modify_inventory(peer, 1796, givedl);
											}
										}
										else {
											int removebgl = (price / 10000) * -1;
											modify_inventory(peer, 7188, removebgl);
										}
										if (item == 1486) {
											if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
												pInfo(peer)->quest_progress += count;
												if (pInfo(peer)->quest_progress >= 28) {
													pInfo(peer)->quest_progress = 28;
													gamepacket_t p;
													p.Insert("OnTalkBubble");
													p.Insert(pInfo(peer)->netID);
													p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
													p.Insert(0), p.Insert(0);
													p.CreatePacket(peer);
												}
											}
										}
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world == pInfo(currentPeer)->world) {
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("`9[" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + " bought " + to_string(count2) + " " + items[item].name + " for " + to_string(priced) + " World Lock]");
												p.CreatePacket(currentPeer);
												packet_(currentPeer, "action|play_sfx\nfile|audio/cash_register.wav\ndelayMS|0");
											}
										}
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`9You don't have enough wls!``");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|shop_item_") != string::npos) {
							int item = atoi(cch.substr(58, cch.length() - 58).c_str());
							if (item <= 0 || item >= items.size() || items[item].pwl == 0 /*|| items[item].shop_texture_file.empty() ||
								(items[item].shop_texture_pos.first == -1 and items[item].shop_texture_pos.second == -1)*/) continue;
							if (pInfo(peer)->gtwl >= items[item].pwl) {
								int c_ = 1;
								if (item == 8442)
								{
									vector<int> item_{8442, 8444, 8446, 8448, 8450};
									if (get_item_count(pInfo(peer), item_[0]) < 200
										and get_item_count(pInfo(peer), item_[1]) < 200
										and get_item_count(pInfo(peer), item_[2]) < 200
										and get_item_count(pInfo(peer), item_[3]) < 200
										and get_item_count(pInfo(peer), item_[4]) < 200 
										and get_free_slots(pInfo(peer)) >= item_.size())
									{
										modify_inventory(peer, item_[0], c_ = 1);
										modify_inventory(peer, item_[1], c_ = 1);
								        modify_inventory(peer, item_[2], c_ = 1);
								        modify_inventory(peer, item_[3], c_ = 1);
										modify_inventory(peer, item_[4], c_ = 1);
										{
											pInfo(peer)->gtwl -= items[item].pwl;
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`o>> You purchased Doctor's Blingin Set!");
											p.CreatePacket(peer);
										}
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
										break;
									}
								}
								else
								{
									if (modify_inventory(peer, item, c_ = 1) == 0) {
										pInfo(peer)->gtwl -= items[item].pwl;
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`o>> You purchased " + items[item].name + "!");
										p.CreatePacket(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1TayoPoint`` to buy this item.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|buy_token_1") != string::npos) {
							if (pInfo(peer)->gtwl >= 5) {
								int c_ = 5;
								if (modify_inventory(peer, 1486, c_) == 0) {
									if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress += c_;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress += c_;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->gtwl -= 5;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased 5 Growtoken!");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1TayoPoint`` to buy this packet.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|buy_token_2") != string::npos) {
							if (pInfo(peer)->gtwl >= 10) {
								int c_ = 10;
								if (modify_inventory(peer, 1486, c_) == 0) {
									if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress += c_;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress += c_;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->gtwl -= 10;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased 10 Growtoken!");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1TayoPoint`` to buy this packet.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|buy_token_3") != string::npos) {
							if (pInfo(peer)->gtwl >= 50) {
								int c_ = 50;
								if (modify_inventory(peer, 1486, c_) == 0) {
									if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress += c_;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress += c_;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->gtwl -= 50;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased 50 Growtoken!");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1TayoPoint`` to buy this packet.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|buy_token_4") != string::npos) {
							if (pInfo(peer)->gtwl >= 100) {
								int c_ = 1;
								if (modify_inventory(peer, 6802, c_) == 0) {
									if (pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress += c_;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress += c_;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->gtwl -= 100;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased 1 Mega Growtoken!");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1TayoPoint`` to buy this packet.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|opop_price_") != string::npos) {
							int item = atoi(cch.substr(59, cch.length() - 59).c_str());
							if (item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\n\nadd_label_with_icon|big|`0Purchase " + items[item].name + "``|left|" + to_string(items[item].id) + "|\nadd_smalltext|`4Make sure to read this information clearly!``|left|\nadd_smalltext|Price: `3" + setGems(items[item].oprc) + "`` `0Credit Points``|left|\nadd_smalltext|Duration: `7[```4~```7]``|left|\nadd_smalltext|Stock: `7[```4~```7]``|left|\n\nadd_textbox|`6Other information:``|left|\nadd_smalltext|" +
								(item == 5138 || item == 5140 || item == 5024 || item == 5026 || item == 5028 || item == 5030 ? format("You'll receive x200 of {}", items[item].name) : items[item].description) + "|left|\nadd_spacer|\nadd_button|opop_item_" + to_string(item) + "|`0Purchase `1" + setGems(items[item].oprc) + " Credit Points``|noflags|0|0||small|\n\nadd_quick_exit|\nnend_dialog|||OK|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|opop_item_") != string::npos) {
							int item = atoi(cch.substr(58, cch.length() - 58).c_str());
							if (item <= 0 || item >= items.size() || items[item].oprc == 0) continue;
							if (pInfo(peer)->opc >= items[item].oprc) {
								int c_ = 1;
								if (item == 5138 || item == 5140 || item == 5024 || item == 5026 || item == 5028 || item == 5030) c_ = 200;
								if (modify_inventory(peer, item, c_) == 0) {
									if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress++;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (item == 1486 && pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress++;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									pInfo(peer)->opc -= items[item].oprc;
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("`o>> You purchased " + items[item].name + "!");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									p.Insert("No inventory space.");
									p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage");
								p.Insert("You don't have enough `1Credit Points`` to purchase this item.");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nitemid|") != string::npos) {
							int item = atoi(cch.substr(57, cch.length() - 57).c_str());
							if (item <= 0 || item >= items.size()) break;
							if (pInfo(peer)->lastwrenchb != 4516 and items[item].untradeable == 1 or item == 1424) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.CreatePacket(peer);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[item].untradeable == 0 or item == 18 || item == 32 || item == 6336 || item == 1424) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.CreatePacket(peer);
							}
							else {
								int got = 0, receive = 0;
								modify_inventory(peer, item, got);
								if (got == 0) break;
								pInfo(peer)->lastchoosenitem = item;
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have " + to_string(got) + " " + items[item].name + ". How many to store?|left|\nadd_text_input|itemcount||" + to_string(got) + "|3|\nadd_spacer|small|\nadd_button|do_add|Store Items|noflags|0|0|\nend_dialog|storageboxxtreme|Cancel||\n"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|itm") != string::npos) {
							int itemnr = atoi(cch.substr(67, cch.length() - 67).c_str()), itemcount = 0;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_access(peer, world_, block_, false, true)) {
									for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
										if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy) {
											itemcount++;
											if (itemnr == itemcount) {
												pInfo(peer)->lastchoosennr = itemnr;
												gamepacket_t p;
												p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[pInfo(peer)->lastwrenchb].name + "``|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nadd_textbox|You have `w" + to_string(world_->sbox1[i_].count) + " " + items[world_->sbox1[i_].id].name + "`` stored.|left|\nadd_textbox|Withdraw how many?|left|\nadd_text_input|itemcount||" + to_string(world_->sbox1[i_].count) + "|3|\nadd_spacer|small|\nadd_button|do_take|Remove Items|noflags|0|0|\nadd_button|cancel|Back|noflags|0|0|\nend_dialog|storageboxxtreme|Exit||\n"), p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nitemid|") != string::npos) {
							int item = atoi(cch.substr(58, cch.length() - 58).c_str()), got = 0;
							modify_inventory(peer, item, got);
							if (got == 0) break;
							if (items[item].untradeable == 1 || item == 1424 || items[item].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4You can't place that in the box, you need it!`7]``"), p.CreatePacket(peer);
							}
							else if (items[item].rarity == 1) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[```4This box only accepts items rarity 2+ or greater`7]``"), p.CreatePacket(peer);
							}
							else if (check_transmuted(peer, item)) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"),
									p.Insert(pInfo(peer)->netID),
									p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
									p.CreatePacket(peer);
								break;
							}
							else {
								pInfo(peer)->lastchoosenitem = item;
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|" + items[item].name + "``|left|" + to_string(item) + "|\nadd_textbox|How many to put in the box as a gift? (Note: You will `4LOSE`` the items you give!)|left|\nadd_text_input|count|Count:|" + to_string(got) + "|5|\nadd_text_input|sign_text|Optional Note:||128|\nadd_spacer|small|\nadd_button|give|`4Give the item(s)``|noflags|0|0|\nadd_spacer|small|\nadd_button|cancel|`wCancel``|noflags|0|0|\nend_dialog|give_item|||\n");
								p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nbuttonClicked|takeall\n") != string::npos) {
							bool took = false, fullinv = false;
							gamepacket_t p3;
							p3.Insert("OnTalkBubble"), p3.Insert(pInfo(peer)->netID);
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (not block_access(peer, world_, block_)) break;
								if (!items[block_->fg].donation) break;
								for (int i_ = 0; i_ < block_->donates.size(); i_++) {
									int receive = block_->donates[i_].count;
									if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
										took = true;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("`7[``" + pInfo(peer)->tankIDName + " receives `5" + to_string(receive) + "`` `w" + items[block_->donates[i_].item].name + "`` from `w" + block_->donates[i_].name + "``, how nice!`7]``");
										block_->donates.erase(block_->donates.begin() + i_);
										i_--;
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											p.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
										}
									}
									else fullinv = true;
								}
								if (block_->donates.size() == 0) {
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							if (fullinv) {
								p3.Insert("I don't have enough room in my backpack to get the item(s) from the box!");
								gamepacket_t p2;
								p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("`2(Couldn't get all of the gifts)``"), p2.CreatePacket(peer);
							}
							else if (took) p3.Insert("`2Box emptied.``");
							p3.CreatePacket(peer);
							packet_(peer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|give_item\nbuttonClicked|give\n\ncount|") != string::npos) {
							if (items[pInfo(peer)->lastwrenchb].blockType != BlockTypes::DONATION) break;
							int count = atoi(cch.substr(69, cch.length() - 69).c_str()), got = 0;
							string text = cch.substr(80 + to_string(count).length(), cch.length() - 80 + to_string(count).length()).c_str();
							if (pInfo(peer)->lastchoosenitem <= 0 or pInfo(peer)->lastchoosenitem == NULL or pInfo(peer)->lastchoosenitem > items.size()) break;
							replace_str(text, "\n", "");
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (text.size() > 128 || got <= 0 || count <= 0 || count > items.size()) break;
							if (count > got || items[pInfo(peer)->lastchoosenitem].untradeable == 1 || pInfo(peer)->lastchoosenitem == 1424 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (count > got) p.Insert("You don't have that to give!");
								else p.Insert("`7[```4You can't place that in the box, you need it!`7]``");
								p.CreatePacket(peer);
							}
							else if (check_transmuted(peer, pInfo(peer)->lastchoosenitem)) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"),
									p.Insert(pInfo(peer)->netID),
									p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
									p.CreatePacket(peer);
								break;
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
									if (!items[block_->fg].donation) break;
									Donate donate_{};
									donate_.item = pInfo(peer)->lastchoosenitem, donate_.count = count, donate_.name = pInfo(peer)->tankIDName, donate_.text = text;
									block_->donates.push_back(donate_);
									{
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
										BYTE* blc = raw + 56;
										block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
										form_visual(blc, *block_, *world_, peer, false, true);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
									}
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``");
											p.Insert(0);
											p.CreatePacket(currentPeer);
											packet_(currentPeer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
										}
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("`7[```5[```w" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "`` places `5" + to_string(count) + "`` `2" + items[pInfo(peer)->lastchoosenitem].name + "`` into the " + items[pInfo(peer)->lastwrenchb].name + "`5]```7]``");
											p.CreatePacket(currentPeer);
										}
									}
									Algorithm::log_text("DonationLog", pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") places " + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].ori_name + " into the " + items[pInfo(peer)->lastwrenchb].ori_name + " at world " + pInfo(peer)->world);
									if (pInfo(peer)->lastchoosenitem == 7188) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously donated " + to_string(count) + " Blue Gem Lock in World: " + pInfo(peer)->world + ".");
									if (pInfo(peer)->lastchoosenitem == 1796 and count > 10) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously donated " + to_string(count) + " Diamond Lock in World: " + pInfo(peer)->world + ".");
									if (pInfo(peer)->lastchoosenitem == 242 and count == 200) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously donated " + to_string(count) + " World Lock in World: " + pInfo(peer)->world + ".");
									modify_inventory(peer, pInfo(peer)->lastchoosenitem, count *= -1);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|cancel") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (world_->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->admin)load_storagebox(peer, world_, block_);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_take\n\nitemcount|") != string::npos) {
							int itemnr = pInfo(peer)->lastchoosennr, countofremoval = atoi(cch.substr(83, cch.length() - 83).c_str()), removed = 0, itemcount = 0;
							removed = countofremoval;
							if (countofremoval <= 0) break;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_access(peer, world_, block_, false, true)) {
									for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
										if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy) {
											itemcount++;
											if (itemnr == itemcount and countofremoval < world_->sbox1[i_].count) {
												if (world_->sbox1[i_].count <= 0) break;
												world_->sbox1[i_].count -= removed;
												if (modify_inventory(peer, world_->sbox1[i_].id, countofremoval) == 0) {
													gamepacket_t p, p2;
													p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = world_->sbox1[i_].id, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[] raw;
													i_ = world_->sbox1.size();
												}
											}
											else if (itemnr == itemcount and world_->sbox1[i_].count == countofremoval) {
												if (world_->sbox1[i_].count <= 0) break;
												if (modify_inventory(peer, world_->sbox1[i_].id, countofremoval) == 0) {
													gamepacket_t p, p2;
													p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													p2.Insert("OnConsoleMessage"), p2.Insert("Removed `w" + to_string(removed) + " " + items[world_->sbox1[i_].id].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
													PlayerMoving data_{};
													data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
													data_.packetType = 19, data_.plantingTree = 500;
													data_.punchX = world_->sbox1[i_].id, data_.punchY = pInfo(peer)->netID;
													int32_t to_netid = pInfo(peer)->netID;
													BYTE* raw = packPlayerMoving(&data_);
													raw[3] = 5;
													memcpy(raw + 8, &to_netid, 4);
													send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													delete[] raw;
													world_->sbox1.erase(world_->sbox1.begin() + i_);
													i_ = world_->sbox1.size();
												}
											}
										}
									}
								}
							}
							break;
						}
						//here code jar
						else if (cch.find("action|dialog_return\ndialog_name|confirm_addpunch") != string::npos) {
							if (cch.find("buttonClicked|confirm_apply") != string::npos) {
								int got = 0;
								modify_inventory(peer, 7188, got);
								if (got < 0 || got == 0) {
									Algorithm::send_overlay(peer, "Oops our bgl is not enough");
									break;
								} else {
									modify_inventory(peer, 7188, got = -1);
									int itemsid = atoi(explode("\n", explode("i_iditem|", cch)[1])[0].c_str());
									int maxgem = atoi(explode("\n", explode("i_punchid|", cch)[1])[0].c_str());
									std::string filename = "./database/config/punch.json";
									json jsonData;
									std::ifstream inputFile(filename);
									if (inputFile.is_open()) {
										inputFile >> jsonData;
										inputFile.close();
									}
									json newData;
									newData["id"] = itemsid;
									newData["punchid"] = maxgem;
									jsonData.push_back(newData);
									std::ofstream outputFile(filename);
									if (outputFile.is_open()) {
										outputFile << jsonData << std::endl;
										outputFile.close();
									}
									//items[itemsid].effect = get_punch_id(itemsid);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										packet_(currentPeer, "action|play_sfx\nfile|audio/levelup.wav\ndelayMS|0");
										Algorithm::send_console(currentPeer, "`2>> `cServer Update :");
										Algorithm::send_console(currentPeer, "");
										Algorithm::send_console(currentPeer, "`2>> `9The Item " + items[itemsid].ori_name + "!!");
										Algorithm::send_console(currentPeer, "`2>> `cHas Been Add Punch Effect!!!!");
									}
								}
							}
							if (cch.find("buttonClicked|edit_puncef") != string::npos) {
								DialogBuilder b;
								b.add_label_icon(true, 5956, "Edit Punch Item")
									.add_spacer(false)
									.add_smalltext("Enter a iditem and id punch to edit item punch effect")
									.add_spacer(false)
									.add_text_input(5, "ditem_x", "Item Id :")
									.add_text_input(4, "idpunch_x", "Id Punch :")
									.add_spacer(false)
									.add_button("confirm_ditem", "Apply")
									.end_dialog("setwdialog", "Nevermind", "");
								b.add_quick_exit();
								Algorithm::send_dialog(peer, b.to_string());
							}
							if (cch.find("buttonClicked|rem_puncef") != string::npos) {
								DialogBuilder b;
								b.add_label_icon(true, 5956, "Remive Punch Item")
									.add_spacer(false)
									.add_smalltext("Enter a iditem and will deleted after reopen exe")
									.add_spacer(false)
									.add_text_input(5, "euyditem", "Item Id :")
									.add_spacer(false)
									.add_button("confirm_rem", "Apply")
									.end_dialog("setwdialog", "Nevermind", "");
								b.add_quick_exit();
								Algorithm::send_dialog(peer, b.to_string());
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|setwdialog") != string::npos) {
							if (cch.find("buttonClicked|confirm_rem") != string::npos) {
								int itemsid = atoi(explode("\n", explode("euyditem|", cch)[1])[0].c_str());
								std::string filename = "./database/config/punch.json";
								json jsonData;
								std::ifstream inputFile(filename);
								if (inputFile.is_open()) {
									inputFile >> jsonData;
									inputFile.close();
								}
								bool found = false;
								for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
									if ((*it)["id"] == itemsid) {
										it = jsonData.erase(it);
										found = true;
										break;
									}
								}
								if (found) {
									std::ofstream outputFile(filename);
									if (outputFile.is_open()) {
										outputFile << std::setw(4) << jsonData << std::endl; // Prettify JSON with indentation
										outputFile.close();
										Algorithm::send_overlay(peer, "`2[`0Succesfully delete Item Punch Effect!`2]");
									}
								}
								else {
									Algorithm::send_overlay(peer, "`4Oops`5, the file / itemid was not found");
								}
							}
							if (cch.find("buttonClicked|confirm_ditem") != string::npos) {
								int retes = atoi(explode("\n", explode("ditem_x|", cch)[1])[0].c_str());
								int idpunch = atoi(explode("\n", explode("idpunch_x|", cch)[1])[0].c_str());
								std::string filenames = "./database/config/punch.json";
								editjson::GrowSoul::edit_punchjson(filenames, retes, idpunch);
								Algorithm::send_overlay(peer, "`c[`0 Succesfully edit punch effect to item `9" + items[retes].ori_name + "`0 !!`c ]");
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|create_worldmain") != string::npos) {
							try {
								if (cch.find("buttonClicked|buyfarm") != string::npos) {
									DialogBuilder o;
									o.add_label_icon(false, 242, "Write Your Name World")
										.add_spacer(false)
										.add_text_input(50, "nama_dunia", "Name World :")
										//.add_text_input(40, "code_type", "Name World to copy :", "")
										.add_spacer(false)
										.add_button("confirm_buyfarm", "Buy World!")
										.end_dialog("create_worldmain", "Close", "");
									o.add_quick_exit(), Algorithm::send_dialog(peer, o.to_string());
								}
								else if (cch.find("buttonClicked|confirm_buyfarm") != string::npos) {
									World world_;
									string world = explode("\n", explode("nama_dunia|", cch)[1])[0].c_str();
									//string codes = explode("\n", explode("code_type|", cch)[1])[0].c_str();
									replace_str(world, "\n", "");
									transform(world.begin(), world.end(), world.begin(), ::toupper);
									if (find_if(worlds.begin(), worlds.end(), [world](const World& a) { return a.name == world; }) != worlds.end() || not check_blast(world) || _access_s(("database/worlds/" + world + "_.json").c_str(), 0) == 0) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("That world name already exists. You'll have to be more original. Maybe add some numbers after it?"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
									}
									else {
										int got;
										vector<int> list{ 100, 180, 150, 80, 70, 190 };
										int kurang = -5;
										modify_inventory(peer, 1796, got);
										if (got < 15) {
											Algorithm::send_overlay(peer, "OOPS YOUR DL IS NOT ENOUGH");
											break;
										}
										if (got == 0) {
											Algorithm::send_overlay(peer, "OOPS YOUR DL IS NOT ENOUGH");
											break;
										}
										int ask = 15;
										World world_{};
										world_ = get_world("GENERATEDOSJDH");
										world_.name = world;
										world_.owner_name = pInfo(peer)->tankIDName;
										world_.d_weather = 1;
										worlds.push_back(world_);
										join_world(peer, world);
										if (modify_inventory(peer, 1796, ask *= -1)) {
											Algorithm::send_bubble(peer, pInfo(peer)->netID, pInfo(peer)->tankIDName + " Has Buy Farm World");
											break;
										}
										//SendCmd(peer, "/copyworld " + codes);
									}
								}
								std::ifstream fileitem("./database/config/createworld.json");
								if (!fileitem.is_open()) {
									std::cerr << "Error opening file." << std::endl;
								}
								json jsonitem;
								fileitem >> jsonitem;
								fileitem.close();
								if (jsonitem.is_array()) {
									for (const auto& obj : jsonitem) {
										if (obj.is_object()) {
											if (obj.contains("worldname") && obj.contains("price")) {
												//string button = obj["button"].get<string>();
												string wname = obj["worldname"].get<string>();
												//string bdialog = obj["btndialog"].get<string>();
												int price = obj["price"].get<int>();
												if (cch.find("buttonClicked|" + wname + "_1") != string::npos) {
													DialogBuilder o;
													o.add_label_icon(false, 242, "Write Your Name World")
														.add_spacer(false)
														.add_text_input(50, "nama_dunia", "Name World :")
														//.add_text_input(40, "code_type", "Name World to copy :", "")
														.add_spacer(false)
														.add_button(wname + "_2", "Buy World!")
														.end_dialog("create_worldmain", "Close", "");
													o.add_quick_exit(), Algorithm::send_dialog(peer, o.to_string());
												}
												else if (cch.find("buttonClicked|" + wname + "_2") != string::npos) {
													//World world_;
													string world = explode("\n", explode("nama_dunia|", cch)[1])[0].c_str();
													//string codes = explode("\n", explode("code_type|", cch)[1])[0].c_str();
													replace_str(world, "\n", "");
													transform(world.begin(), world.end(), world.begin(), ::toupper);
													if (find_if(worlds.begin(), worlds.end(), [world](const World& a) { return a.name == world; }) != worlds.end() || not check_blast(world) || _access_s(("database/worlds/" + world + "_.json").c_str(), 0) == 0) {
														gamepacket_t p;
														p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("That world name already exists. You'll have to be more original. Maybe add some numbers after it?"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
													}
													else {
														int got;
														vector<int> list{ 100, 180, 150, 80, 70, 190 };
														//int kurang = -5;
														modify_inventory(peer, 1796, got);
														if (got < price) {
															Algorithm::send_overlay(peer, "OOPS YOUR DL IS NOT ENOUGH");
															break;
														}
														if (got == 0) {
															Algorithm::send_overlay(peer, "OOPS YOUR DL IS NOT ENOUGH");
															break;
														}
														int ask = 0;
														World world_;
														world_ = get_world(wname);
														world_.name = world;
														if (pInfo(peer)->world.empty()) break;
														world_.owner_name = pInfo(peer)->tankIDName;
														world_.owner_id = pInfo(peer)->uid;
														world_.owner_named = pInfo(peer)->modName;
														pInfo(peer)->worlds_owned.push_back(pInfo(peer)->world);
														string name_ = pInfo(peer)->world;
														auto px = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
														if (px != worlds.end()) {
															World* worl = &(*px);
															update_world(peer, worl, pInfo(peer)->tankIDName, name_);
														}
														worlds.push_back(world_);
														join_world(peer, world);
														int receive = price * -1;
														modify_inventory(peer, 1796, receive);
														Algorithm::send_bubble(peer, pInfo(peer)->netID, pInfo(peer)->tankIDName + " Has Buy World");
													}
												}
											}
										}
									}
								}
							}
							catch (...) {
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|classic_bank") != string::npos) {
							int amount = 0, BGLCount = 0, DLCount = 0, WLCount = 0, remove = 0, add = 0;
							int WLInBank = 0, DLInBank = 0, BGLInBank = 0;
							DLInBank = pInfo(peer)->lockInBank / 100;
							WLInBank = pInfo(peer)->lockInBank - (DLInBank * 100);
							if (DLInBank >= 100) {
								BGLInBank = pInfo(peer)->lockInBank / 10000;
								DLInBank -= 100 * BGLInBank;
							}
							string transfer_to = "";
							bool isOnline = false;
							if (cch.find("buttonClicked|transfer_from_bank") != string::npos) {
								classic_bank_show(peer, true);
								break;
							}
							else if (cch.find("buttonClicked|transfer_lock") != string::npos) {
								transfer_to = explode("\n", explode("transfer_to|", cch)[1])[0];
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) {
									classic_bank_show(peer, true, 3);
									break;
								}
								else if (amount > pInfo(peer)->lockInBank) {
									classic_bank_show(peer, true, 2);
									break;
								}
								else if (amount > 100000) {
									classic_bank_show(peer, true, 1);
									break;
								}
								if (to_lower(transfer_to) == to_lower(pInfo(peer)->tankIDName)) {
									classic_bank_show(peer, true, 4);
									break;
								}
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(transfer_to) == to_lower(pInfo(currentPeer)->tankIDName)) {
										isOnline = true;
										DialogBuilder dialog("`o");
										dialog.add_label_icon(true, 242, "`wTransfer Confirmation``");
										dialog.add_textbox("`9Classic Bank ``Transfer Confirmation. Please check once again before you transfer!")
											.add_textbox("You are currently have:");
										if (BGLInBank != 0) dialog.add_label_icon(false, 7188, to_string(BGLInBank) + " Blue Gem Lock");
										if (DLInBank != 0) dialog.add_label_icon(false, 1796, to_string(DLInBank) + " Diamond Lock");
										if (WLInBank != 0) dialog.add_label_icon(false, 242, to_string(WLInBank) + " World Lock");
										dialog.add_textbox("Inside the `9Classic Bank``")
											.add_spacer(false)
											.add_smalltext(format("Are you sure? you are about to transfer `5{} ``Locks to `w{} (`5#{}`w)``", fixint(amount), pInfo(currentPeer)->tankIDName, pInfo(currentPeer)->uid))
											.embed_data(false, "transfer_amount", to_string(amount))
											.embed_data(false, "transfer_to", pInfo(currentPeer)->tankIDName);
										dialog.add_spacer(false).end_dialog("confirm_transfer_lock_from_bank", "Cancel", "Confirm").add_quick_exit();
										Algorithm::send_dialog(peer, dialog.to_string());
										break;
									}
								}
								if (!isOnline) {
									classic_bank_show(peer, true, 0);
									break;
								}
								break;
							}
							else if (cch.find("buttonClicked|deposit_lock") != string::npos) {
								modify_inventory(peer, 7188, BGLCount), modify_inventory(peer, 1796, DLCount), modify_inventory(peer, 242, WLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0 || amount > 200) {
									classic_bank_show(peer, false, -2);
									break;
								}
								DialogBuilder dialog("`o");
								dialog.add_label_icon(true, 242, "`wClassic Bank``")
									.embed_data(false, "lock_amount", to_string(amount))
									.add_smalltext("`9Classic Bank ``Deposit option. please choose which locks would you like to deposit!")
									.add_textbox("You currently holding:");
								if (BGLCount != 0) dialog.add_label_icon(false, 7188, to_string(BGLCount) + " Blue Gem Lock");
								if (DLCount != 0) dialog.add_label_icon(false, 1796, to_string(DLCount) + " Diamond Lock");
								if (WLCount != 0) dialog.add_label_icon(false, 242, to_string(WLCount) + " World Lock");
								if (BGLCount + DLCount + WLCount == 0) dialog.add_smalltext("You are currently not carrying any Locks");
								dialog.add_textbox("In your inventory")
									.add_spacer(false)
									.add_textbox(format("You are about to deposit {} Locks into the `9Classic Bank ``to continue your deposit please choose locks option below", amount));
								if (BGLCount != 0) dialog.add_small_font_button("deposit_bgl", "Deposit as `eBlue Gem Lock``");
								if (DLCount != 0) dialog.add_small_font_button("deposit_dl", "Deposit as `1Diamond Lock``");
								dialog.add_small_font_button("deposit_wl", "Deposit as `9World Lock``")
									.add_spacer(false)
									.end_dialog("classic_bank", "Cancel", "").add_quick_exit();
								Algorithm::send_dialog(peer, dialog.to_string());
								break;
							}
							else if (cch.find("buttonClicked|deposit_bgl") != string::npos) {
								modify_inventory(peer, 7188, BGLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								if (BGLCount >= amount) {
									remove = amount * -1;
									if (modify_inventory(peer, 7188, remove) == 0) {
										pInfo(peer)->lockInBank += (amount * 10000);
										Algorithm::send_console(peer, "`5[``You deposited `2" + to_string(amount) + " `eBlue Gem Lock ``into the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "`eBlue Gem Lock `wSuccessfully deposited!");
										classic_bank_show(peer);
										break;
									}
								}
								else Algorithm::send_overlay(peer, "You don't have enough `eBlue Gem Lock ``to deposit that amount!"); break;
								break;
							}
							else if (cch.find("buttonClicked|deposit_dl") != string::npos) {
								modify_inventory(peer, 1796, DLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								if (DLCount >= amount) {
									remove = amount * -1;
									if (modify_inventory(peer, 1796, remove) == 0) {
										pInfo(peer)->lockInBank += (amount * 100);
										Algorithm::send_console(peer, "`5[``You deposited `2" + to_string(amount) + " `1Diamond Lock ``into the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "`1Diamond Lock `wSuccessfully deposited!");
										classic_bank_show(peer);
										break;
									}
								}
								else Algorithm::send_overlay(peer, "You don't have enough `1Diamond Lock ``to deposit that amount!"); break;
								break;
							}
							else if (cch.find("buttonClicked|deposit_wl") != string::npos) {
								modify_inventory(peer, 242, WLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								if (WLCount >= amount) {
									remove = amount * -1;
									if (modify_inventory(peer, 242, remove) == 0) {
										pInfo(peer)->lockInBank += amount;
										Algorithm::send_console(peer, "`5[``You deposited `2" + to_string(amount) + " `9World Lock ``into the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "`9World Lock `wSuccessfully deposited!");
										classic_bank_show(peer);
										break;
									}
								}
								else Algorithm::send_overlay(peer, "You don't have enough `9World Lock ``to deposit that amount!"); break;
								break;
							}
							else if (cch.find("buttonClicked|withdraw_lock") != string::npos) {
								modify_inventory(peer, 7188, BGLCount), modify_inventory(peer, 1796, DLCount), modify_inventory(peer, 242, WLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0 || amount > 200) {
									classic_bank_show(peer, false, -2);
									break;
								}
								DialogBuilder dialog("`o");
								dialog.add_label_icon(true, 242, "`wClassic Bank``")
									.embed_data(false, "lock_amount", setGems(amount))
									.add_smalltext("`9Classic Bank ``Withdrawal option. please choose which locks you prefer to withdraw!")
									.add_textbox("You are currently have:");
								if (BGLInBank != 0) dialog.add_label_icon(false, 7188, setGems(BGLInBank) + " Blue Gem Lock");
								if (DLInBank != 0) dialog.add_label_icon(false, 1796, setGems(DLInBank) + " Diamond Lock");
								if (WLInBank != 0) dialog.add_label_icon(false, 242, setGems(WLInBank) + " World Lock");
								dialog.add_textbox("Inside the `9Classic Bank``")
									.add_spacer(false)
									.add_textbox(format("You are about to withdraw {} Locks from the `9Classic Bank ``to continue your withdrawal please choose locks option below", amount));
								if (BGLInBank != 0) dialog.add_small_font_button("withdraw_bgl", "Withdraw as `eBlue Gem Lock``");
								if (DLInBank != 0) dialog.add_small_font_button("withdraw_dl", "Withdraw as `1Diamond Lock``");
								dialog.add_small_font_button("withdraw_wl", "Withdraw as `9World Lock``")
									.add_spacer(false)
									.end_dialog("classic_bank", "Cancel", "").add_quick_exit();
								Algorithm::send_dialog(peer, dialog.to_string());
								break;
							}
							else if (cch.find("buttonClicked|withdraw_bgl") != string::npos) {
								modify_inventory(peer, 7188, BGLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								add = amount;
								if (BGLCount + amount <= 200 && pInfo(peer)->lockInBank >= (amount * 10000)) {
									if (modify_inventory(peer, 7188, add) == 0) {
										pInfo(peer)->lockInBank -= (amount * 10000);
										Algorithm::send_console(peer, "`5[``You withdrawn `2" + setGems(amount) + " ``Blue Gem Lock from the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "Blue Gem Lock withdrawn!");
										PlayerMoving data_{};
										data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										data_.packetType = 19, data_.plantingTree = 100;
										data_.punchX = 7188, data_.punchY = pInfo(peer)->netID;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										break;
									}
									else Algorithm::send_overlay(peer, "No inventory space to pick up the Blue Gem Lock"); break;
								}
								else Algorithm::send_overlay(peer, "You don't have enough locks to withdraw");
								break;
							}
							else if (cch.find("buttonClicked|withdraw_dl") != string::npos) {
								modify_inventory(peer, 1796, DLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								add = amount;
								if (BGLCount + amount <= 200 && pInfo(peer)->lockInBank >= (amount * 100)) {
									if (modify_inventory(peer, 1796, add) == 0) {
										pInfo(peer)->lockInBank -= (amount * 100);
										Algorithm::send_console(peer, "`5[``You withdrawn `2" + setGems(amount) + " ``Diamond Lock from the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "Diamond Lock withdrawn!");
										PlayerMoving data_{};
										data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										data_.packetType = 19, data_.plantingTree = 100;
										data_.punchX = 1796, data_.punchY = pInfo(peer)->netID;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										break;
									}
									else Algorithm::send_overlay(peer, "No inventory space to pick up the Diamond Lock"); break;
								}
								else Algorithm::send_overlay(peer, "You don't have enough locks to withdraw");
								break;
							}
							else if (cch.find("buttonClicked|withdraw_wl") != string::npos) {
								modify_inventory(peer, 242, BGLCount);
								amount = atoi(explode("\n", explode("lock_amount|", cch)[1])[0].c_str());
								if (amount <= 0) break;
								add = amount;
								if (BGLCount + amount <= 200 && pInfo(peer)->lockInBank >= amount) {
									if (modify_inventory(peer, 242, add) == 0) {
										pInfo(peer)->lockInBank -= amount;
										Algorithm::send_console(peer, "`5[``You withdrawn `2" + setGems(amount) + " ``World Lock from the `9Classic Bank`5]");
										Algorithm::send_overlay(peer, "World Lock withdrawn!");
										PlayerMoving data_{};
										data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										data_.packetType = 19, data_.plantingTree = 100;
										data_.punchX = 242, data_.punchY = pInfo(peer)->netID;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										break;
									}
									else Algorithm::send_overlay(peer, "No inventory space to pick up the World Lock"); break;
								}
								else Algorithm::send_overlay(peer, "You don't have enough locks to withdraw");
								break;
							}
							else if (cch.find("buttonClicked|convert_dl") != string::npos) {
								modify_inventory(peer, 1796, DLCount);
								if (DLCount >= 100) {
									remove = -100;
									add = 1;
									if (modify_inventory(peer, 7188, add) == 0 && modify_inventory(peer, 1796, remove) == 0) {
										Algorithm::send_overlay(peer, "Diamond Lock Converted!");
										classic_bank_show(peer);
										break;
									}
									else Algorithm::send_overlay(peer, "Make sure you have empty backpack slots or enough Diamond Lock amount to convert!"); break;
								}
								else if (DLCount == 200) {
									remove = -200;
									add = 2;
									if (modify_inventory(peer, 7188, add) == 0 && modify_inventory(peer, 1796, remove) == 0) {
										Algorithm::send_overlay(peer, "Diamond Lock Converted!");
										classic_bank_show(peer);
										break;
									}
									else Algorithm::send_overlay(peer, "Make sure you have empty backpack slots or enough Diamond Lock amount to convert!"); break;
								}
							}
							break;
							}
						else if (cch.find("action|dialog_return\ndialog_name|set_online_status\nbuttonClicked|set_online_status") != string::npos) {
							pInfo(peer)->p_status = atoi(explode("\n", explode("checkbox_status_online|", cch)[1])[0].c_str()) == 1 ? 0 : atoi(explode("\n", explode("checkbox_status_busy|", cch)[1])[0].c_str()) == 1 ? 1 : atoi(explode("\n", explode("checkbox_status_away|", cch)[1])[0].c_str()) == 1 ? 2 : 0;
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile\nbuttonClicked|save") != string::npos) {
							pInfo(peer)->show_uid = atoi(explode("\n", explode("checkbox_show_uid|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_Achievements = atoi(explode("\n", explode("checkbox_show_achievements|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_OwnedWorlds = atoi(explode("\n", explode("checkbox_show_ownworld_count|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_AccountCreated = atoi(explode("\n", explode("checkbox_show_account_created|", cch)[1])[0].c_str()) == 1 ? true : false;
							if (pInfo(peer)->home_world != "") pInfo(peer)->Show_HomeWorld = atoi(explode("\n", explode("checkbox_show_homeworld|", cch)[1])[0].c_str()) == 1 ? true : false;
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile\nbuttonClicked|discard") != string::npos) {
							send_wrench_self(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile\nbuttonClicked|preview") != string::npos) {
							pInfo(peer)->show_uid = atoi(explode("\n", explode("checkbox_show_uid|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_Achievements = atoi(explode("\n", explode("checkbox_show_achievements|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_OwnedWorlds = atoi(explode("\n", explode("checkbox_show_ownworld_count|", cch)[1])[0].c_str()) == 1 ? true : false;
							pInfo(peer)->Show_AccountCreated = atoi(explode("\n", explode("checkbox_show_account_created|", cch)[1])[0].c_str()) == 1 ? true : false;
							if (pInfo(peer)->home_world != "") pInfo(peer)->Show_HomeWorld = atoi(explode("\n", explode("checkbox_show_homeworld|", cch)[1])[0].c_str()) == 1 ? true : false;
							time_t s__;
							s__ = time(NULL);
							int days_ = int(s__) / (60 * 60 * 24);
							DialogBuilder preview("`o");
							preview.add_label_icon(true, 18, (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "`` `0(```2" + (pInfo(peer)->admin && pInfo(peer)->d_name.empty() ? "???" : to_string(pInfo(peer)->level)) + "```0)``" + (pInfo(peer)->show_uid ? format("(`4{}``)``", pInfo(peer)->uid) : ""))
								.add_spacer(false);
							if (pInfo(peer)->Show_OwnedWorlds) {
								preview.add_spacer(false)
									.add_label(false, "`1Owned Worlds:`` " + to_string(pInfo(peer)->worlds_owned.size()));
							}
							if (pInfo(peer)->Show_AccountCreated) {
								preview.add_spacer(false)
									.add_label(false, "`1Account Age: `` " + to_string(days_ - pInfo(peer)->account_created) + " days");
							}
							if (pInfo(peer)->home_world != "" && pInfo(peer)->Show_HomeWorld) {
								preview.add_spacer(false)
									.add_label(false, "`1Home World:``")
									.add_disabled_button("visit_home_world_" + pInfo(peer)->home_world, "`$Visit " + pInfo(peer)->home_world + "``");
							}
							preview.add_spacer(false)
								.add_disabled_button("trade", "`wTrade``")
								.add_textbox("(No Battle Leash equipped)")
								.add_disabled_button("friend_add", "`wAdd as friend``")
								.add_disabled_button("ignore_player", "`wIgnore Player``")
								.add_disabled_button("report_player", "`wReport Player``")
								.end_dialog("personalize_profile", "", "Back")
								.add_quick_exit();
							enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(preview.to_string()).pack());
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|personalize_profile") != string::npos) {
							SendDialogPersonalizeProfile(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bouncer_dialog\nbuttonClicked|gems_") != string::npos) {
							int wl = 0, dl = 0;
							modify_inventory(peer, 242, wl);
							modify_inventory(peer, 1796, dl);
							DialogBuilder confirm;
							int itemChoosed = atoi(cch.substr(67, cch.length() - 67).c_str());
							switch (itemChoosed) {
							case 50000: {
								confirm.add_label_icon(true, 9490, "`eBuy 50.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 50.000 Gems - Voucher do you want to buy, for 100.000 ė each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 100000: {
								confirm.add_label_icon(true, 9492, "`eBuy 100.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 100.000 Gems - Voucher do you want to buy, for 200.000 ė each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 250000: {
								confirm.add_label_icon(true, 9494, "`eBuy 250.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 250.000 Gems - Voucher do you want to buy, for 500.000 ė each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 500000: {
								confirm.add_label_icon(true, 9496, "`eBuy 500.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 500.000 Gems - Voucher do you want to buy, for 1.000.000 ė each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 1000000: {
								confirm.add_label_icon(true, 9498, "`eBuy 1.000.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 1.000.000 Gems - Voucher do you want to buy, for 2.000.000 ė each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 50: {
								confirm.add_label_icon(true, 9490, "`eBuy 50.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 50.000 Gems - Voucher do you want to buy, for 50 ā each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 100: {
								confirm.add_label_icon(true, 9492, "`eBuy 100.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 100.000 Gems - Voucher do you want to buy, for 100 ā each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 250: {
								confirm.add_label_icon(true, 9494, "`eBuy 250.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 250.000 Gems - Voucher do you want to buy, for 250 ā each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 500: {
								confirm.add_label_icon(true, 9496, "`eBuy 500.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 500.000 Gems - Voucher do you want to buy, for 500 ā each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							case 1000: {
								confirm.add_label_icon(true, 9498, "`eBuy 1.000.000 Gems - Voucher?``")
									.add_smalltext("`2Consume this voucher to earn amount of gems``")
									.add_spacer(false)
									.add_textbox("How many 1.000.000 Gems - Voucher do you want to buy, for 1000 ā each?")
									.add_text_input(5, "how", "", "1")
									.add_smalltext("`9You have " + (wl != 0 ? to_string(wl) + " World Locks" : "") + "" + (dl != 0 ? ", " + to_string(dl) + " Diamond Lock." : ".") + "<CR>You also have " + fixint(pInfo(peer)->gems) + " ė in your pocket.``")
									.add_spacer(false)
									.add_button("bouncer_gemsbuy", "`9Purchase``")
									.add_button("bouncer_backButton", "`wNo Thanks``")
									.embed_data(false, "gemsbuy", to_string(itemChoosed));
								break;
							}
							default: break;
							}
							confirm.add_quick_exit();
							enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(confirm.to_string()).pack());
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|kit_dialog\n") != string::npos) {
							if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
							if (cch.find("buttonClicked|info_kit1") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 1);
								break;
							}
							else if (cch.find("buttonClicked|info_kit2") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 2);
								break;
							}
							else if (cch.find("buttonClicked|info_kit3") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 3);
								break;
							}
							else if (cch.find("buttonClicked|info_kit4") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 4);
								break;
							}
							else if (cch.find("buttonClicked|info_kit5") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 5);
								break;
							}
							else if (cch.find("buttonClicked|info_kit6") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 6);
								break;
							}
							else if (cch.find("buttonClicked|info_kit7") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								SendInfoKitDialog(peer, 7);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|info_kit_dialog\n") != string::npos) {
							if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
							if (cch.find("buttonClicked|claim_kit3") != string::npos) {
								if (not pInfo(peer)->kit3) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 542, addItem = 50) == 0) {
											pInfo(peer)->kit3 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Pot O' Gold Kit");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit2") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit2) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 7914, addItem = 1) == 0) {
											pInfo(peer)->kit2 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Condor Wing Kit");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit1") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit1) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 5178, addItem = 100) == 0) {
											pInfo(peer)->kit1 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Poison Cauldron");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit4") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit4) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 6780, addItem = 1) == 0) {
											pInfo(peer)->kit4 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Dabstep Top Sneakers Kit");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit5") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit5) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 11300, addItem = 1) == 0) {
											pInfo(peer)->kit5 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Neon Party Cape Kit");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit6") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit6) {
									int free = get_free_slots(pInfo(peer)), slot = 1, addItem = 0, adaBrp = 0;
									if (free >= slot) {
										if (modify_inventory(peer, 9190, addItem = 1) == 0) {
											pInfo(peer)->kit6 = true;
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Cloak Ice and Fire Kit");
										}
										else Algorithm::send_bubble(peer, pInfo(peer)->netID, "You don't have enough room in your inventory");
									}
									else Algorithm::send_bubble(peer, pInfo(peer)->netID, "There is no empty slot in your inventory");
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit7") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								if (not pInfo(peer)->kit7) {
									if (!pInfo(peer)->vip) {
										pInfo(peer)->kit7 = true;
										pInfo(peer)->vip = 1;
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Free VIP Kit");
										enet_peer_disconnect_later(peer, 0);
									}
									else {
										int c_ = 100;
										if (modify_inventory(peer, 5136, c_ = 100) == 0) {
											pInfo(peer)->kit7 = true;
											Algorithm::set_bux(peer);
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Claimed Free VIP Kit");
											Algorithm::send_console(peer, "You already have `wVIP ``converted prize into `w100" + items[5136].name + "``");
										}
										else {
											Algorithm::send_bubble(peer, pInfo(peer)->netID, "Failed to claim VIP Kit");
										}
									}
								}
								break;
							}
							else if (cch.find("buttonClicked|claim_kit8") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								Algorithm::send_overlay(peer, "Coming Soon!");
								break;
							}
							else if (cch.find("buttonClicked|claim_kit9") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								Algorithm::send_overlay(peer, "Coming Soon!");
								break;
							}
							else if (cch.find("buttonClicked|claim_kit10") != string::npos) {
								if (not has_playmod(pInfo(peer), "Royal GrowPass")) break;
								Algorithm::send_overlay(peer, "Coming Soon!");
								break;
							}
							else SendCmd(peer, "/kit");
							break;
						}
                        else if (cch.find("action|dialog_return\ndialog_name|start_kit_dialog") != string::npos) {
							if (not pInfo(peer)->startedKit) {
								pInfo(peer)->startedKit = true;
								SendKitDialog(peer);
							}
							break;
						}
						if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|playslot") != string::npos) {
							int data = -1;
							int data2 = 2;
							if (modify_inventory(peer, 1796, data) == 0) {

								int win = rand() % 3;
								if (win == 1) {
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("add_label_with_icon|big|`2YOU WON THIS GAME! YOUR BGL GETTING X2|left|756|\nadd_image_button||interface/large/reme.rttex|bannerlayout|||\n"
										"add_spacer|small|\n"
										"add_textbox|`oWOW! Hi, Netopian You Can Play Slot With This Feature /Troll, In This Game You Should Have `c1DL `oTo Play. If You Luck, Your DL Getting x2 And If You Lose You Will Rip Your DL|left|\n"
										"add_spacer|small|\n"
										"add_button|playslot|`2PLAY AGAIN SLOT GAME!|\n"
										"add_spacer|small|\n"
										"add_button|dawdawd|Okay!|"
									);
									p.CreatePacket(peer);
									modify_inventory(peer, 1796, data2);
									packet_(peer, "action|play_sfx\nfile|audio/slot_win.wav\ndelayMS|0");

								}
								else {
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("add_label_with_icon|big|`4LOL? YOU LOSE ON THIS GAME|left|756|\nadd_image_button||interface/large/reme.rttex|bannerlayout|||\n"
										"add_spacer|small|\n"
										"add_textbox|`oWOW! Hi, Netopian You Can Play Slot With This Feature /Troll, In This Game You Should Have `c1DL `oTo Play. If You Luck, Your DL Getting x2 And If You Lose You Will Rip Your DL|left|\n"
										"add_spacer|small|\n"
										"add_button|playslot|`2PLAY AGAIN SLOT GAME!|\n"
										"add_spacer|small|\n"
										"add_button|dawdawd|Okay!|"
									);
									p.CreatePacket(peer);
									packet_(peer, "action|play_sfx\nfile|audio/slot_lose.wav\ndelayMS|0");
								}

							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|mines\nbuttonClicked|accelts") != string::npos) {
							if (cch.find("minesamount|") != string::npos and cch.find("gems|") != string::npos) {
							}
							else break;
							//Set mine locations
							int gemamo = 0;
							vector<int> mines{};
							if (!is_number(explode("\n", explode("minesamount|", cch)[1])[0])) break;
							int minesll = atoi(explode("\n", explode("minesamount|", cch)[1])[0].c_str());
							gemamo = Algorithm::converttoint(explode("\n", explode("gems|", cch)[1])[0].c_str()); // 100000
							if (minesll < 1 or minesll > 10) {
								Algorithm::send_bubble(peer, pInfo(peer)->netID, "Mines amount is too low or too much");
								break;
							}
							if (gemamo < 100000 or gemamo > 5000000) {
								Algorithm::send_bubble(peer, pInfo(peer)->netID, "Gems amount is too low or too much");
								break;
							}
							while (mines.size() < minesll) {
								int toadd = rand() % 16 + 1;
								if (!includesint(mines, toadd)) mines.push_back(toadd);
							}
							pInfo(peer)->bM = minesll * (gemamo / 9);
							if (pInfo(peer)->gems < gemamo) {
								Algorithm::send_bubble(peer, pInfo(peer)->netID, "You dont have enough gems.");
								break;
							}
							else pInfo(peer)->gems -= gemamo;
							set_bux(peer, pInfo(peer)->gems);
							pInfo(peer)->foundg = 0;
							pInfo(peer)->minesleft = minesll;
							pInfo(peer)->endedit = false;
							pInfo(peer)->mines = mines;
							pInfo(peer)->clickm.clear();
							pInfo(peer)->lostt = false;
							vector<int> clicked = pInfo(peer)->clickm;
							int minesleft = pInfo(peer)->minesleft;
							int foundg = pInfo(peer)->foundg;
							gamepacket_t p;
							int sa = 6888, bm = 6994;
							p.Insert("OnDialogRequest");
							string extr = "";
							if (!clicked.empty()) extr += "\nadd_button|claimnl|Claim and Leave|noflags|0|0|";
							for (int i = 0; i < 4; i++) {
								if (includesint(clicked, i)) extr += "\nadd_button_with_icon|bma_" + to_string(i + 1) + "||staticBlueFrame|6888|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 1) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if (includesint(clicked, i)) extr += "\nadd_button_with_icon|bma_" + to_string(i + 5) + "||staticBlueFrame|6888|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 5) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if (includesint(clicked, i)) extr += "\nadd_button_with_icon|bma_" + to_string(i + 9) + "||staticBlueFrame|6888|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 9) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if (includesint(clicked, i)) extr += "\nadd_button_with_icon|bma_" + to_string(i + 13) + "||staticBlueFrame|6888|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 13) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							p.Insert("\nadd_label_with_icon|big|Tayo Minefield|left|6994|\nadd_spacer|big|\nadd_smalltext|`4Mines Left:" + setGems(minesleft) + ", `2Total Profit: " + setGems(foundg) + " Gems|left|\nadd_spacer|small|" + extr + "\nend_dialog|mines|STOP GAMBLING!|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mines\nbuttonClicked|claimnl") != string::npos) {
							if (pInfo(peer)->lostt) break;
							pInfo(peer)->gems += pInfo(peer)->foundg * pInfo(peer)->bM;
							Algorithm::send_bubble(peer, pInfo(peer)->netID, "Earned `2" + setGems(pInfo(peer)->foundg * pInfo(peer)->bM) + " Gems ė.");
							set_bux(peer, pInfo(peer)->gems);
							pInfo(peer)->minesleft = 0, pInfo(peer)->foundg = 0;
							pInfo(peer)->mines = {};
							pInfo(peer)->clickm = {};
							pInfo(peer)->lostt = false;
							pInfo(peer)->endedit = true;
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mines\nbuttonClicked|bma_") != string::npos) {
							if (pInfo(peer)->endedit) break;
							int cli = atoi(cch.substr(57, cch.length() - 57).c_str());
							if (includesint(pInfo(peer)->clickm, cli)) break;
							if (cli > 16) break;
							//Set mine locations
							pInfo(peer)->clickm.push_back(cli);
							vector<int> mines = pInfo(peer)->mines;
							vector<int> clicked = pInfo(peer)->clickm;
							int minesleft = pInfo(peer)->minesleft;
							int foundg = pInfo(peer)->foundg;
							gamepacket_t p;
							bool lost = false;
							int sa = 6888, bm = 6994;
							p.Insert("OnDialogRequest");
							string extr = "";
							if (includesint(mines, cli)) lost = true;
							if (not lost) pInfo(peer)->foundg++, foundg++;

							for (int i = 0; i < 4; i++) {
								if ((includesint(clicked, i + 1) or lost) and !includesint(mines, i + 1)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 1) + "||staticBlueFrame|6888|";
								else if (includesint(mines, i + 1) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 1) + "||staticYellowFrame|6994|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 1) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if ((includesint(clicked, i + 5) or lost) and !includesint(mines, i + 5)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 5) + "||staticBlueFrame|6888|";
								else if (includesint(mines, i + 5) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 5) + "||staticYellowFrame|6994|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 5) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if ((includesint(clicked, i + 9) or lost) and !includesint(mines, i + 9)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 9) + "||staticBlueFrame|6888|";
								else if (includesint(mines, i + 9) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 9) + "||staticYellowFrame|6994|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 9) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							for (int i = 0; i < 4; i++) {
								if ((includesint(clicked, i + 13) or lost) and !includesint(mines, i + 13)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 13) + "||staticBlueFrame|6888|";
								else if (includesint(mines, i + 13) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 13) + "||staticYellowFrame|6994|";
								else extr += "\nadd_button_with_icon|bma_" + to_string(i + 13) + "||staticBlueFrame|0|";
							}
							extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
							if (!clicked.empty() && not lost) extr += "\nadd_button|claimnl|Claim and Leave|noflags|0|0|";
							if (lost) extr += "\nadd_button|pekKsks|Leave|noflags|0|0|";
							string tx = "Mines Left:" + setGems(minesleft) + ", `2Total Profit: " + setGems(foundg * pInfo(peer)->bM) + " Gems";
							if (lost) tx = "YOU LOST!", pInfo(peer)->lostt = true;
							p.Insert("\nadd_label_with_icon|big|`wTayo Minefield|left|6994|\nadd_spacer|big|\nadd_smalltext|`4" + tx + "|left|\nadd_spacer|small|" + extr + "\nend_dialog|mines|STOP GAMBLING!|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|mines\nbuttonClicked|cum_") != string::npos) {
								if (pInfo(peer)->endedit) break;
								int cli = atoi(cch.substr(57, cch.length() - 57).c_str());
								vector<int> mines = pInfo(peer)->mines;
								vector<int> clicked = pInfo(peer)->clickm;
								int minesleft = pInfo(peer)->minesleft;
								int foundg = pInfo(peer)->foundg;
								gamepacket_t p;
								bool lost = false;
								int sa = 6888, bm = 6994;
								p.Insert("OnDialogRequest");
								string extr = "";
								if (pInfo(peer)->lostt) lost = true;
								for (int i = 0; i < 4; i++) {
									if ((includesint(clicked, i + 1) or lost) and !includesint(mines, i + 1)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 1) + "||staticBlueFrame|6888|";
									else if (includesint(mines, i + 1) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 1) + "||staticYellowFrame|6994|";
									else extr += "\nadd_button_with_icon|bma_" + to_string(i + 1) + "||staticBlueFrame|0|";
								}
								extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
								for (int i = 0; i < 4; i++) {
									if ((includesint(clicked, i + 5) or lost) and !includesint(mines, i + 5)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 5) + "||staticBlueFrame|6888|";
									else if (includesint(mines, i + 5) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 5) + "||staticYellowFrame|6994|";
									else extr += "\nadd_button_with_icon|bma_" + to_string(i + 5) + "||staticBlueFrame|0|";
								}
								extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
								for (int i = 0; i < 4; i++) {
									if ((includesint(clicked, i + 9) or lost) and !includesint(mines, i + 9)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 9) + "||staticBlueFrame|6888|";
									else if (includesint(mines, i + 9) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 9) + "||staticYellowFrame|6994|";
									else extr += "\nadd_button_with_icon|bma_" + to_string(i + 9) + "||staticBlueFrame|0|";
								}
								extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
								for (int i = 0; i < 4; i++) {
									if ((includesint(clicked, i + 13) or lost) and !includesint(mines, i + 13)) extr += "\nadd_button_with_icon|cum_" + to_string(i + 13) + "||staticBlueFrame|6888|";
									else if (includesint(mines, i + 13) and lost) extr += "\nadd_button_with_icon|cum_" + to_string(i + 13) + "||staticYellowFrame|6994|";
									else extr += "\nadd_button_with_icon|bma_" + to_string(i + 13) + "||staticBlueFrame|0|";
								}
								extr += "\nadd_button_with_icon||END_LIST|noflags|0||";
								if (!clicked.empty() && not lost) extr += "\nadd_button|claimnl|Claim and Leave|noflags|0|0|";
								if (lost) extr += "\nadd_button|pekKsks|Leave|noflags|0|0|";
								string tx = "Mines Left:" + setGems(minesleft) + ", `2Total Profit: " + setGems(foundg * pInfo(peer)->bM) + " Gems";
								if (lost) tx = "YOU LOST!";
								p.Insert("\nadd_label_with_icon|big|`wTayo Minefield|left|6994|\nadd_spacer|big|\nadd_smalltext|`4" + tx + "|left|\nadd_spacer|small|" + extr + "\nend_dialog|mines|STOP GAMBLING!|\nadd_quick_exit|");
								p.CreatePacket(peer);
								break;
						}
						if (cch.find("action|dialog_return\ndialog_name|mine_games") != string::npos) {
							try {
								if (cch.find("buttonClicked|mine_") != string::npos) {
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 3 || !pInfo(peer)->isBet || pInfo(peer)->Mine_Pos == -1) break;
									string intenger = explode("\n", t_[3])[0];
									replace_str(intenger, "mine_", "");
									int view = atoi(intenger.c_str());
									bool Winner = false;
									if (view == pInfo(peer)->Mine_Pos) Winner = true;
									bool Found = false;
									string Data_List = "";
									for (int i = 0; i < 5; i++) {
										Data_List += "add_button_with_icon|||" + string(i == pInfo(peer)->Mine_Pos ? "staticYellowFrame" : "staticGreyFrame") + ",no_padding_x|" + string(i == pInfo(peer)->Mine_Pos ? "6880" : "6994") + "||\n";
									}
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBeware of the Mine!``|left|6994|\nadd_textbox|" + string(Winner ? "Congratulations! You won the game." : "Sorry! You lose the game.") + "|left|\nadd_spacer|small|\n" + Data_List + "add_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|mine_games|Okay!||");
									p.CreatePacket(peer);
									pInfo(peer)->isBet = false; pInfo(peer)->Mine_Pos = -1;
									if (Winner) {
										packet_(peer, "action|play_sfx\nfile|audio/love_in.wav\ndelayMS|0");
										int Add = 2;
										if (modify_inventory(peer, 1796, Add) == 0) {}
										else {
											string name_ = pInfo(peer)->world;
											vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
											if (p != worlds.end()) {
												World* world_ = &worlds[p - worlds.begin()];
												WorldDrop drop_block_{};
												drop_block_.id = 1796, drop_block_.count = 2, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
									}
									else packet_(peer, "action|play_sfx\nfile|audio/loser.wav\ndelayMS|0");
									break;
								}
								else {
									int Remove = -1;
									if (modify_inventory(peer, 1796, Remove) == 0) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
										p.Insert("Choose wisely on your choiche! Get the right one.");
										p.Insert(0), p.Insert(1), p.CreatePacket(peer);
										{
										Regenerate:
											bool Found = false;
											string Data_List = "";
											for (int i = 0; i < 5; i++) {
												Data_List += "add_button_with_icon|mine_" + to_string(i) + "||staticGreyFrame,no_padding_x|7188||\n";
												if (rand() % 10 < 3 and !Found) {
													pInfo(peer)->Mine_Pos = i;
													Found = true;
												}
											}
											if (!Found) goto Regenerate;
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wBeware of the Mine!``|left|6994|\nadd_textbox|Choose wisely, there are 4 bombs randomly!|left|\nadd_spacer|small|\n" + Data_List + "add_button_with_icon||END_LIST|noflags|0||\nadd_spacer|small|\nend_dialog|mine_games|Nevermind!||");
											p.CreatePacket(peer);
											pInfo(peer)->isBet = true;
										}
									}
									break;
								}
							}
							catch (...) {
								break;
							}
							break;
						}
						if (cch == "action|dialog_return\ndialog_name|\nbuttonClicked|updatemining\n\n") {
							int got = 0, data = -1;
							modify_inventory(peer, 1796, got);
							if (got < 200) {
								if (modify_inventory(peer, 1796, data) == 0) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`6>> Success Bought 1 Minner!");
									pInfo(peer)->levelMinner += 1;
									save_player(pInfo(peer), false);
								}
							}
							else {
								Algorithm::send_bubble(peer, pInfo(peer)->netID, "`4>> You Should Have 1 Bgl For It!");
							}
						}
						if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|invest") != string::npos) {
							int min = -70;
							if (!pInfo(peer)->isMinner) {
								if (modify_inventory(peer, 1796, min) == 0) {
									Algorithm::send_overlay(peer, "`6>> Success Buy Your Tayo Diamond Lock Minner");
									pInfo(peer)->isMinner = true;
									pInfo(peer)->minnerData = 0.00;
								}
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nbuttonClicked|") != string::npos) {
							int choosen_item = atoi(cch.substr(61, cch.length() - 61).c_str());
							if (choosen_item >= pInfo(peer)->bp.size() || choosen_item > 200 || choosen_item > pInfo(peer)->b_l * 10) break;
							for (int i_ = 0; i_ < pInfo(peer)->bp.size(); i_++) {
								if (choosen_item == i_) {
									if (pInfo(peer)->bp[choosen_item].first <= 0 || pInfo(peer)->bp[choosen_item].first >= items.size()) break;
									int pickedup = pInfo(peer)->bp[choosen_item].second;
									int count = pInfo(peer)->bp[choosen_item].second;
									if (modify_inventory(peer, pInfo(peer)->bp[choosen_item].first, count) == 0) {
										{
											gamepacket_t p, p2;
											p.Insert("OnConsoleMessage"), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p.CreatePacket(peer);
											p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p.Insert("You picked up " + to_string(pickedup) + " " + items[pInfo(peer)->bp[choosen_item].first].name + "."), p2.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.punchX = pInfo(peer)->bp[choosen_item].first, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
										int32_t to_netid = pInfo(peer)->netID;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 5;
										memcpy(raw + 8, &to_netid, 4);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[]raw;
										pInfo(peer)->bp.erase(pInfo(peer)->bp.begin() + i_);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have enough inventory space!"), p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|backpack_menu\nitemid|") != string::npos) {
							if (pInfo(peer)->bp.size() <= pInfo(peer)->b_l * 10) {
								int got = 0, item = atoi(cch.substr(54, cch.length() - 54).c_str());
								modify_inventory(peer, item, got);
								if (got <= 0) break;
								if (item == 18 || item == 32 || item == 5640 || item == 1424 || items[item].blockType == BlockTypes::FISH || item == 5816) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You can't store this item!");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
								else if (check_transmuted(peer, item)) {
									gamepacket_t p;
									p.Insert("OnTalkBubble");
									p.Insert(pInfo(peer)->netID);
									p.Insert("You can't store transmuted item!");
									p.Insert(0), p.Insert(0);
									p.CreatePacket(peer);
								}
								else {
									pInfo(peer)->bp.push_back(make_pair(item, got));
									modify_inventory(peer, item, got *= -1);
									PlayerMoving data_{};
									data_.packetType = 19, data_.punchX = item, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[]raw;
									backpack_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_eq_aura\nbutton_item_selection|") != string::npos) {
							int item = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (item > 0 && item < items.size()) {
								if (items[item].toggleable) {
									pInfo(peer)->eq_a_1 = item;
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wEQ Aura``|left|12634|\nadd_spacer|small|\nadd_textbox|Play music wherever you are with the EQ Aura! Choose a musical block from your inventory to play the song.|left|\nadd_spacer|small|" + (string(pInfo(peer)->eq_a_1 != 0 ? "\nadd_label_with_icon|small|`w" + items[pInfo(peer)->eq_a_1].name + "``|left|" + to_string(pInfo(peer)->eq_a_1) + "|\nadd_spacer|small|" : "")) + "\nadd_item_picker|button_item_selection|`wChange Block Item``|Choose Musical Block Item!|\nadd_button|restore_default|`wRemove Block Item``|noflags|0|0|\nadd_spacer|small|\nadd_spacer|small|\nend_dialog|dialog_eq_aura|Cancel|Update|\nadd_quick_exit|");
									p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wEQ Aura``|left|12634|\nadd_spacer|small|\nadd_spacer|small|\nadd_textbox|`4Invalid item! You can only use musical block items! Please choose something else.``|left|\nadd_spacer|small|\nadd_textbox|Play music wherever you are with the EQ Aura! Choose a musical block from your inventory to play the song.|left|\nadd_spacer|small|" + (string(pInfo(peer)->eq_a != 0 ? "\nadd_label_with_icon|small|`w" + items[pInfo(peer)->eq_a].name + "``|left|" + to_string(pInfo(peer)->eq_a) + "|\nadd_spacer|small|" : "")) + "\nadd_item_picker|button_item_selection|`wChange Block Item``|Choose Musical Block Item!|\nadd_button|restore_default|`wRemove Block Item``|noflags|0|0|\nadd_spacer|small|\nadd_spacer|small|\nend_dialog|dialog_eq_aura|Cancel|Update|\nadd_quick_exit|");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_eq_aura\nbuttonClicked|restore_default") != string::npos) {
							pInfo(peer)->eq_a_1 = 0;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wEQ Aura``|left|12634|\nadd_spacer|small|\nadd_textbox|Play music wherever you are with the EQ Aura! Choose a musical block from your inventory to play the song.|left|\nadd_spacer|small|" + (string(pInfo(peer)->eq_a_1 != 0 ? "\nadd_label_with_icon|small|`w" + items[pInfo(peer)->eq_a_1].name + "``|left|" + to_string(pInfo(peer)->eq_a_1) + "|\nadd_spacer|small|" : "")) + "\nadd_item_picker|button_item_selection|`wChange Block Item``|Choose Musical Block Item!|\nadd_button|restore_default|`wRemove Block Item``|noflags|0|0|\nadd_spacer|small|\nadd_spacer|small|\nend_dialog|dialog_eq_aura|Cancel|Update|\nadd_quick_exit|");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_eq_aura") != string::npos) {
							if (pInfo(peer)->eq_a_1 != 0 && !pInfo(peer)->eq_a_update) pInfo(peer)->eq_a = pInfo(peer)->eq_a_1, pInfo(peer)->eq_a_update = true;
							if (pInfo(peer)->eq_a_1 == 0) pInfo(peer)->eq_a_1 = 0, pInfo(peer)->eq_a = 0;
							update_clothes(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|\nbuttonClicked|trans_") != string::npos) {
							int item = atoi(cch.substr(54, cch.length() - 54).c_str());
							if (item <= 0 || item >= items.size()) break;
							if (item == 256) {
								gamepacket_t p(0, pInfo(peer)->netID);
								p.Insert("OnFlagMay2019"), p.Insert(256);
								pInfo(peer)->flagmay = 256;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
									if (pInfo(peer)->world == pInfo(currentPeer)->world) p.CreatePacket(currentPeer);
								}
							}
							int got = 0;
							modify_inventory(peer, item, got);
							if (got == 0) break;
							if (items[item].flagmay == 256) break;
							gamepacket_t p(0, pInfo(peer)->netID);
							pInfo(peer)->flagmay = items[item].flagmay;
							p.Insert("OnFlagMay2019"), p.Insert(items[item].flagmay);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED) continue;
								if (pInfo(peer)->world == pInfo(currentPeer)->world) p.CreatePacket(currentPeer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|t_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 98, 228, 1746, 1778, 1830, 5078, 1966, 6948, 6946, 4956 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 228 || list[reward - 1] == 1746 || list[reward - 1] == 1778) count = 200;
							if (find(pInfo(peer)->t_p.begin(), pInfo(peer)->t_p.end(), lvl = reward * 5) == pInfo(peer)->t_p.end()) {
								if (pInfo(peer)->t_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->t_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Farmer Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										farmer_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|view_inventory\nbuttonClicked|") != string::npos) {
							if (pInfo(peer)->dev) {
								int item = atoi(cch.substr(62, cch.length() - 62).c_str()), got = 0;
								pInfo(peer)->choosenitem = item;
								if (item <= 0 || item > items.size()) break;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										modify_inventory(currentPeer, pInfo(peer)->choosenitem, got);
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Take`` `w" + items[pInfo(peer)->choosenitem].name + " from`` `#" + pInfo(currentPeer)->tankIDName + "``|left|" + to_string(pInfo(peer)->choosenitem) + "|\nadd_textbox|How many to `4take``? (player has " + to_string(got) + ")|left|\nadd_text_input|count||" + to_string(got) + "|5|\nend_dialog|take_item|Cancel|OK|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|take_item\ncount|") != string::npos) {
							if (pInfo(peer)->staff) {
								int count = atoi(cch.substr(49, cch.length() - 49).c_str()), receive = atoi(cch.substr(49, cch.length() - 49).c_str());
								int remove = count * -1;
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (to_lower(pInfo(currentPeer)->tankIDName) == to_lower(pInfo(peer)->last_wrenched)) {
										if (count <= 0 || count > 200) break;
										if (modify_inventory(peer, pInfo(peer)->choosenitem, count) == 0) {
											int total = 0;
											modify_inventory(currentPeer, pInfo(peer)->choosenitem, total += remove);
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("Collected `w" + to_string(receive) + " " + items[pInfo(peer)->choosenitem].name + "``." + (items[pInfo(peer)->choosenitem].rarity > 363 ? "" : " Rarity: `w" + to_string(items[pInfo(peer)->choosenitem].rarity) + "``") + "");
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|p_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 1008,1044,872,10450,870,5084,876,6950,6952,1674 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 1008) count = 5;
							if (list[reward - 1] == 1044) count = 50;
							if (list[reward - 1] == 872) count = 200;
							if (list[reward - 1] == 10450) count = 3;
							if (find(pInfo(peer)->p_p.begin(), pInfo(peer)->p_p.end(), lvl = reward * 5) == pInfo(peer)->p_p.end()) {
								if (pInfo(peer)->p_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->p_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Provider Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										provider_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|g_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 4654,262,826,828,9712,3146,2266,5072,5070,9716 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 262 || list[reward - 1] == 826 || list[reward - 1] == 828) count = 50;
							if (list[reward - 1] == 3146) count = 10;
							if (find(pInfo(peer)->g_p.begin(), pInfo(peer)->g_p.end(), lvl = reward * 5) == pInfo(peer)->g_p.end()) {
								if (pInfo(peer)->g_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->g_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Geiger Hunting Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										geiger_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|f_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 3010, 3018, 3020, 3044, 5740, 3042, 3098, 3100, 3040, 10262 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 3018) count = 200;
							if (list[reward - 1] == 3020 || list[reward - 1] == 3098) count = 50;
							if (list[reward - 1] == 3044) count = 25;
							if (find(pInfo(peer)->ff_p.begin(), pInfo(peer)->ff_p.end(), lvl = reward * 5) == pInfo(peer)->ff_p.end()) {
								if (pInfo(peer)->ff_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->ff_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Fishing Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										fishing_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("buttonClicked|breakup") != string::npos) {
							if (pInfo(peer)->affinity_type == NO_AFFINITY) break;
							DialogBuilder db{ "`o" };
							db.add_label_icon(true, 2388, "Breaking Up")
								.add_textbox("Are you sure about to end your affinity with " + GetNameByUid(pInfo(peer)->affinity_with) + "? This means you'll lose your affinity levels too.")
								.add_button("confirm_breakup", "`4Break Up``")
								.add_button("", "Close");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(db.to_string());
							p.CreatePacket(peer);
						}
						else if (cch.find("buttonClicked|confirm_breakup") != string::npos) {
							if (pInfo(peer)->affinity_type == NO_AFFINITY) break;
							DialogBuilder db{ "`o" };
							db.add_label_icon(true, 5810, "Broken Up")
								.add_textbox("You have ended your affinity with " + GetNameByUid(pInfo(peer)->affinity_with) + ", that means you lose your affinity levels too.")
								.add_button("", "Close");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							p.Insert(db.to_string());
							p.CreatePacket(peer);

							if (pInfo(peer)->affinity_type == LOVER) pInfo(peer)->titleDoctor = false;
							else if (pInfo(peer)->affinity_type == BESTIE) pInfo(peer)->titleStartopia = false;

							pInfo(peer)->roleIcon = 6;
							pInfo(peer)->roleSkin = 6;

							gamepacket_t pB;
							pB.Insert("OnSetRoleSkinsAndIcons"), pB.Insert(6), pB.Insert(6), pB.Insert(0);
							pB.CreatePacket(peer);

							Algorithm::send_console(peer, "You have ended your affinity with " + GetNameByUid(pInfo(peer)->affinity_with));
							for (ENetPeer* everyPeer = server->peers; everyPeer < &server->peers[server->peerCount]; ++everyPeer) {
								if (everyPeer->state != ENET_PEER_STATE_CONNECTED || everyPeer->data == NULL || pInfo(everyPeer)->isIn == false) continue;
								if ((pInfo(peer)->affinity_with) == (pInfo(everyPeer)->uid)) {
									Algorithm::send_console(everyPeer, "Unfortunately " + GetNameByUid(pInfo(everyPeer)->affinity_with) + " have broken you up! Good luck next time.");
									if (pInfo(everyPeer)->affinity_type == LOVER) pInfo(everyPeer)->titleDoctor = false;
									else if (pInfo(everyPeer)->affinity_type == BESTIE) pInfo(everyPeer)->titleStartopia = false;

									pInfo(everyPeer)->roleIcon = 6;
									pInfo(everyPeer)->roleSkin = 6;
									pInfo(everyPeer)->affinity_with = 0;
									pInfo(everyPeer)->affinity_level = 0;
									pInfo(everyPeer)->affinity_type = NO_AFFINITY;
								}
							}

							try {
								string path = "database/affinity/" + to_string(pInfo(peer)->affinity_with) + "_.json";
								if (!fs::remove(path.c_str())) {
									printf("Unable to delete file %s\n", pInfo(peer)->affinity_with);
									break;
								}
							}
							catch (fs::filesystem_error& e) {
								cout << e.what() << endl;
							}
							pInfo(peer)->affinity_level = 0;
							pInfo(peer)->affinity_type = NO_AFFINITY;
							pInfo(peer)->affinity_with = 0;
							save_player(pInfo(peer), false);

							try {
								string path_ = "database/affinity/" + to_string(pInfo(peer)->uid) + "_.json";
								if (!fs::remove(path_.c_str())) {
									//printf("Unable to delete file %s\n", pInfo(peer)->uid.c_str());
									break;
								}
							}
							catch (fs::filesystem_error& e) {
								cout << e.what() << endl;
							}
						}
						else if (cch.find("buttonClicked|upgrade_aff") != string::npos) {
							if (pInfo(peer)->affinity_level >= 5 || pInfo(peer)->affinity_type == NO_AFFINITY) break;
							int price = (80 + (20 * pInfo(peer)->affinity_level)) * 100; // ad (* 100) karna lgsg konversi ke dl
							int my_wls = get_wls(peer);
							if (price > my_wls) {
								gamepacket_t p;
								p.Insert("OnTextOverlay");
								p.Insert("Sorry, you don't have enough wls to do this action, try to break your bgls to 100dls if you have 1.");
								p.CreatePacket(peer);
								break;
							}
							else {
								int diamond_lock = 0, world_lock = 0, grazinti_wl = 0;
								int is_viso_worldlock = price;
								while (is_viso_worldlock >= 100) {
									is_viso_worldlock -= 100;
									diamond_lock++;
								}
								world_lock = is_viso_worldlock;
								int c_ = 0;
								modify_inventory(peer, 242, c_);
								int turi_world_lock = c_, t_w = c_ * -1;
								c_ = 0;
								modify_inventory(peer, 1796, c_);
								int turi_diamond_lock = c_, t_d = c_ * -1;
								while (diamond_lock > turi_diamond_lock && turi_world_lock >= 100) {
									turi_diamond_lock++, turi_world_lock -= 100;
								} if (diamond_lock > turi_diamond_lock) break;
								while (world_lock > turi_world_lock && turi_diamond_lock >= 1) {
									turi_diamond_lock--, turi_world_lock += 100;
								} if (world_lock > turi_world_lock) break;
								turi_world_lock -= world_lock, turi_diamond_lock -= diamond_lock;
								
								modify_inventory(peer, 242, t_w), modify_inventory(peer, 1796, t_d);
								modify_inventory(peer, 242, turi_world_lock), modify_inventory(peer, 1796, turi_diamond_lock);
								{
									bool isOnline = false;
									pInfo(peer)->affinity_level++;
									string message = "Your affinity level is now " + to_string(pInfo(peer)->affinity_level);
									gamepacket_t p2;
									p2.Insert("OnTextOverlay");
									p2.Insert("Successfully upgraded your affinity level!");
									p2.CreatePacket(peer);
									save_player(pInfo(peer), false);

									Algorithm::send_console(peer, message);
									Algorithm::send_console(peer, aff_lvl_mod_console(pInfo(peer)));

									for (ENetPeer* everyPeer = server->peers; everyPeer < &server->peers[server->peerCount]; ++everyPeer) {
										if (everyPeer->state != ENET_PEER_STATE_CONNECTED || everyPeer->data == NULL || pInfo(everyPeer)->isIn == false) continue;
										if ((pInfo(peer)->affinity_with) == (pInfo(everyPeer)->uid)) {
											isOnline = true;
											pInfo(everyPeer)->affinity_level++;
											Algorithm::send_console(everyPeer, message);
											Algorithm::send_console(everyPeer, aff_lvl_mod_console(pInfo(everyPeer)));
											save_player(pInfo(everyPeer), false);
											enet_peer_disconnect_later(everyPeer, 0);
										}
									}

									{
										ofstream lol("database/affinity/" + to_string(pInfo(peer)->affinity_with) + "_.json");
										json j;
										j["affinity_level"] = pInfo(peer)->affinity_level;
										j["affinity_type"] = pInfo(peer)->affinity_type;
										j["affinity_with"] = pInfo(peer)->affinity_with;
										lol << j << endl;
										lol.close();
									}
									{
										ofstream lol("database/affinity/" + to_string(pInfo(peer)->uid) + "_.json");
										json j;
										j["affinity_level"] = pInfo(peer)->affinity_level;
										j["affinity_type"] = pInfo(peer)->affinity_type;
										j["affinity_with"] = pInfo(peer)->affinity_with;
										lol << j << endl;
										lol.close();
									}
									enet_peer_disconnect_later(peer, 0);
								}
								
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|roadtoglory\nbuttonClicked|claimreward") != string::npos) {
							int count = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (count < 1 || count >10) break;
							if (std::find(pInfo(peer)->glo_p.begin(), pInfo(peer)->glo_p.end(), count) == pInfo(peer)->glo_p.end()) {
								if (pInfo(peer)->level >= count * 10) {
									pInfo(peer)->glo_p.push_back(count);
									packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnSetBux");
									p.Insert(pInfo(peer)->gems += count * 100000);
									p.Insert(0);
									p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
									if (pInfo(peer)->supp >= 2) {
										p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
									}
									p.CreatePacket(peer);
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("Congratulations! You have received your Road to Glory Reward!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									PlayerMoving data_{};
									data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
									BYTE* raw = packPlayerMoving(&data_);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw;
									glory_show(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|clear\n") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							packet_(peer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (items[block_->fg].blockType == BlockTypes::BULLETIN_BOARD || items[block_->fg].blockType == BlockTypes::MAILBOX) {
									for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
										if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
											world_->bulletin.erase(world_->bulletin.begin() + i_);
											i_--;
										}
									}
									{
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2Mailbox emptied.``" : "`2Text cleared.``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
									}
									if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
									PlayerMoving data_{};
									data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
									BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
									BYTE* blc = raw + 56;
									form_visual(blc, *block_, *world_, peer, false);
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
										send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
									}
									delete[] raw, blc;
									if (block_->locked) upd_lock(*block_, *world_, peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|remove_bulletin") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								int letter = 0;
								World* world_ = &worlds[p - worlds.begin()];
								for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
									if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
										letter++;
										if (pInfo(peer)->lastchoosennr == letter) {
											world_->bulletin.erase(world_->bulletin.begin() + i_);
											{
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert("`2Bulletin removed.``");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|edit") != string::npos) {
							int count = atoi(cch.substr(65, cch.length() - 65).c_str()), letter = 0;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								for (int i_ = 0; i_ < world_->bulletin.size(); i_++) {
									if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) {
										letter++;
										if (count == letter) {
											pInfo(peer)->lastchoosennr = count;
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|small|Remove`` \"`w" + world_->bulletin[i_].text + "\"`` from your board?|left|" + to_string(pInfo(peer)->lastwrenchb) + "|\nend_dialog|remove_bulletin|Cancel|OK|");
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nbuttonClicked|send\n\nsign_text|") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							string text = explode("\n", t_[4])[0].c_str();
							replace_str(text, "\n", "");
							if (text.length() <= 2 || text.length() >= 100) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								p.Insert("That's not interesting enough to post.");
								p.Insert(0), p.Insert(0);
								p.CreatePacket(peer);
							}
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									{
										World* world_ = &worlds[p - worlds.begin()];
										if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX || items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::BULLETIN_BOARD) {
											int letter = 0;
											for (int i_ = 0; i_ < world_->bulletin.size(); i_++) if (world_->bulletin[i_].x == pInfo(peer)->lastwrenchx and world_->bulletin[i_].y == pInfo(peer)->lastwrenchy) letter++;
											if (letter == 21) world_->bulletin.erase(world_->bulletin.begin() + 0);
											WorldBulletin bulletin_{};
											bulletin_.x = pInfo(peer)->lastwrenchx, bulletin_.y = pInfo(peer)->lastwrenchy;
											if (pInfo(peer)->name_color == "`4Datemaster." || pInfo(peer)->name_color == "`4@Dr." || pInfo(peer)->name_color == "`6@" || pInfo(peer)->name_color == "`9@" || pInfo(peer)->name_color == "`#@" || pInfo(peer)->name_color == "`0") bulletin_.name = (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + "``";
											else bulletin_.name = "`0" + (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) + "``";
											bulletin_.text = text;
											world_->bulletin.push_back(bulletin_);
											{
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID);
												p.Insert(items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX ? "`2You place your letter in the mailbox.``" : "`2Bulletin posted.``");
												p.Insert(0), p.Insert(0);
												p.CreatePacket(peer);
												packet_(peer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
											}
											if (items[pInfo(peer)->lastwrenchb].blockType == BlockTypes::MAILBOX) {
												WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
												block_->flags = (block_->flags & 0x00400000 ? block_->flags : block_->flags | 0x00400000);
												PlayerMoving data_{};
												data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
												BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_));
												BYTE* blc = raw + 56;
												form_visual(blc, *block_, *world_, peer, false, true);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_), ENET_PACKET_FLAG_RELIABLE);
												}
												delete[] raw, blc;
												if (block_->locked) upd_lock(*block_, *world_, peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|change_password") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_->fg == 8878) {
									if (block_access(peer, world_, block_, false, true)) {
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_smalltext|The ingenious minds at GrowTech bring you the `2Safe Vault`` - a place to store your items safely with its integrated password option!|left|\nadd_smalltext|How the password works:|left|\nadd_smalltext|The Safe Vault requires both a `2password`` and a `2recovery answer`` to be entered to use a password.|left|\nadd_smalltext|Enter your `2password`` and `2recovery answer`` below - keep them safe and `4DO NOT`` share them with anyone you do not trust!|left|\nadd_smalltext|The password and recovery answer can be no longer than 12 characters in length - number and alphabet only please, no special characters are allowed!|left|\nadd_smalltext|If you forget your password, enter your recovery answer to access the Safe Vault - The Safe Vault will `4NOT be password protected now``. You will need to enter a new password.|left|\nadd_smalltext|You can change your password, however you will need to enter the old password before a new one can be used.|left|\nadd_smalltext|`4WARNING``: DO NOT forget your password and recovery answer or you will not be able to access the Safe Vault!|left|\nadd_textbox|`4There is no password currently set on this Safe Vault.``|left|\nadd_textbox|Enter a new password.|left|\nadd_text_input_password|storage_newpassword|||18|\nadd_textbox|Enter a recovery answer.|left|\nadd_text_input|storage_recoveryanswer|||12|\nadd_button|set_password|Update Password|noflags|0|0|\nend_dialog|storageboxpassword|Exit||\nadd_quick_exit|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxpassword") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_->fg == 8878) {
									if (world_->owner_name == pInfo(peer)->tankIDName || pInfo(peer)->admin) {
										vector<string> t_ = explode("|", cch);
										if (t_.size() < 4) break;
										string button = explode("\n", t_[3])[0].c_str();
										if (button == "set_password") {
											string text = explode("\n", t_[4])[0].c_str(), text2 = explode("\n", t_[5])[0].c_str();
											replace_str(text, "\n", "");
											replace_str(text2, "\n", "");
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											if (not check_blast(text) || not check_blast(text2)) p.Insert("`4Your input contains special characters. It should only contain alphanumeric characters!``");
											else if (text == "") p.Insert("`4You did not enter a new password!``");
											else if (text2 == "") p.Insert("`4You did not enter a recovery answer!``");
											else if (text.length() > 12 || text2.length() > 12) p.Insert("`4The password is too long! You can only use a maximum of 12 characters!``");
											else {
												p.Insert("`2Your password has been updated!``");
												block_->door_destination = text;
												block_->door_id = text2;
											}
											p.Insert(0), p.Insert(1);
											p.CreatePacket(peer);
										}
										else if (button == "check_password") {
											string password = cch.substr(99, cch.length() - 100).c_str();
											if (password == block_->door_destination) load_storagebox(peer, world_, block_);
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The password you e did not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
										else if (button == "show_recoveryanswer") {
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSafe Vault``|left|8878|\nadd_textbox|Please enter recovery answer.|left|\nadd_text_input|storage_recovery_answer|||12|\nadd_button|check_recovery|Enter Recovery Answer|noflags|0|0|\nend_dialog|storageboxpassword|Exit||\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										else if (button == "check_recovery") {
											string password = cch.substr(106, cch.length() - 107).c_str();
											if (password == block_->door_id) {
												block_->door_destination = "", block_->door_id = "";
												load_storagebox(peer, world_, block_);
											}
											else {
												gamepacket_t p;
												p.Insert("OnTalkBubble");
												p.Insert(pInfo(peer)->netID), p.Insert("`4The recovery answer you entered does not match!``"), p.Insert(0), p.Insert(1);
												p.CreatePacket(peer);
											}
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|advbegins\nnameEnter|") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_->fg == 4722) {
									if (block_access(peer, world_, block_)) {
										string text = cch.substr(53, cch.length() - 54).c_str();
										if (text.size() > 32) break;
										block_->heart_monitor = text;
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID), p.Insert("Updated adventure!"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(currentPeer)->adventure_begins == false) continue;
											pInfo(currentPeer)->adventure_begins = false;
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|storageboxxtreme\nbuttonClicked|do_add\n\nitemcount|") != string::npos) {
							int count = atoi(cch.substr(82, cch.length() - 82).c_str());
							if (pInfo(peer)->lastchoosenitem <= 0 || pInfo(peer)->lastchoosenitem >= items.size()) break;
							if (pInfo(peer)->lastwrenchb != 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 1 or pInfo(peer)->lastchoosenitem == 1424 or items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Untradeable items there!"), p.CreatePacket(peer);
							}
							else if (pInfo(peer)->lastwrenchb == 4516 and items[pInfo(peer)->lastchoosenitem].untradeable == 0 or pInfo(peer)->lastchoosenitem == 1424 || items[pInfo(peer)->lastchoosenitem].blockType == BlockTypes::FISH || pInfo(peer)->lastchoosenitem == 18 || pInfo(peer)->lastchoosenitem == 32 || pInfo(peer)->lastchoosenitem == 6336) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You can't store Tradeable items there!"), p.CreatePacket(peer);
							}
							else if (check_transmuted(peer, pInfo(peer)->lastchoosenitem)) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"),
									p.Insert(pInfo(peer)->netID),
									p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
									p.CreatePacket(peer);
								break;
							}
							else {
								int got = 0, receive = 0;
								modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
								if (count <= 0 || count > got) {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You don't have that many!"), p.CreatePacket(peer);
								}
								else {
									receive = count * -1;
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										WorldBlock block_ = world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
										if (items[pInfo(peer)->lastchoosenitem].untradeable == 1 && block_.fg != 4516) break;
										if (items[block_.fg].blockType != BlockTypes::STORAGE) break;
										gamepacket_t p1, p2;
										p1.Insert("OnTalkBubble"), p1.Insert(pInfo(peer)->netID), p1.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in " + items[pInfo(peer)->lastwrenchb].name + "."), p1.CreatePacket(peer);
										p2.Insert("OnConsoleMessage"), p2.Insert("Stored `w" + to_string(count) + " " + items[pInfo(peer)->lastchoosenitem].name + "`` in the " + items[pInfo(peer)->lastwrenchb].name + "."), p2.CreatePacket(peer);
										modify_inventory(peer, pInfo(peer)->lastchoosenitem, receive);
										bool dublicated = true;
										for (int i_ = 0; i_ < world_->sbox1.size(); i_++) {
											if (dublicated) {
												if (world_->sbox1[i_].x == pInfo(peer)->lastwrenchx and world_->sbox1[i_].y == pInfo(peer)->lastwrenchy and world_->sbox1[i_].id == pInfo(peer)->lastchoosenitem and world_->sbox1[i_].count + count <= 200) {
													world_->sbox1[i_].count += count;
													dublicated = false;
												}
											}
										}
										if (dublicated) {
											WorldSBOX1 sbox1_{};
											sbox1_.x = pInfo(peer)->lastwrenchx, sbox1_.y = pInfo(peer)->lastwrenchy;
											sbox1_.id = pInfo(peer)->lastchoosenitem, sbox1_.count = count;
											world_->sbox1.push_back(sbox1_);
										}
										PlayerMoving data_{};
										data_.packetType = 19, data_.netID = -1, data_.plantingTree = 0;
										data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16;
										data_.XSpeed = pInfo(peer)->x + 16, data_.YSpeed = pInfo(peer)->y + 16;
										data_.punchX = pInfo(peer)->lastchoosenitem;
										BYTE* raw = packPlayerMoving(&data_);
										raw[3] = 6;
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
										load_storagebox(peer, world_, &block_);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|account_security\nchange|") != string::npos) {
							string change = cch.substr(57, cch.length() - 57).c_str();
							replace_str(change, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "email") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else if (change == "password") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password|OK|Continue|\n");
							if (change == "email" or change == "password") p.CreatePacket(peer);
							break;
						}
						// ADD PIN SECURITY
						if (cch.find("action|dialog_return\ndialog_name|change_email\nchange|") != string::npos) {
							string change = cch.substr(53, cch.length() - 53).c_str();
							replace_str(change, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5email address``|left|\nadd_text_input|change|||50|\nend_dialog|change_email|OK|Continue|\n");
							else {
								pInfo(peer)->email = change;
								save_player(pInfo(peer), false);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|Having an up-to-date email address attached to your account is a great step toward improved account security.|left|\nadd_smalltext|Your new Email: `5" + pInfo(peer)->email + "``|left|\nadd_spacer|small|\nend_dialog|changedemail|OK||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_password\nchange|") != string::npos) {
							string change = cch.substr(56, cch.length() - 56).c_str();
							replace_str(change, "\n", "");
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (change == "") p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_spacer|small|\nadd_smalltext|Type your new `5password``|left|\nadd_text_input|change|||18|\nend_dialog|change_password|OK|Continue|\n");
							else {
								{
									gamepacket_t p;
									p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->tankIDName), p.Insert(pInfo(peer)->tankIDPass = change);
									p.CreatePacket(peer);
								}
								save_player(pInfo(peer), false);
								p.Insert("set_default_color|`o\nadd_label_with_icon|big|`0Account Security``|left|1424|\nadd_spacer|small|\nadd_textbox|`6Information``|left|\nadd_smalltext|A hacker may attempt to access your account more than once over a period of time.|left|\nadd_smalltext|Changing your password `2often reduces the risk that they will have frequent access``.|left|\nadd_smalltext|Your new password: `5" + pInfo(peer)->tankIDPass + "``|left|\nadd_spacer|small|\nend_dialog|changedpassword|OK||\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|world_swap\nname_box|") != string::npos) {
							string world = cch.substr(53, cch.length() - 53).c_str(), currentworld = pInfo(peer)->world;
							int got = 0;
							replace_str(world, "\n", "");
							transform(world.begin(), world.end(), world.begin(), ::toupper);
							if (not check_blast(world) || currentworld == world) {
								gamepacket_t p;
								p.Insert("OnDialogRequest"), p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSwap World Names``|left|2580|\nadd_textbox|`4World swap failed - you don't own both worlds!``|left|\nadd_smalltext|This will swap the name of the world you are standing in with another world `4permanently``.  You must own both worlds, with a World Lock in place.<CR>Your `wChange of Address`` will be consumed if you press `5Swap 'Em``.|left|\nadd_textbox|Enter the other world's name:|left|\nadd_text_input|name_box|||32|\nadd_spacer|small|\nend_dialog|world_swap|Cancel|Swap 'Em!|"), p.CreatePacket(peer);
							}
							else create_address_world(peer, world, currentworld);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_guild_name\nbuttonClicked|confirm") != string::npos) {
							string newName = explode("\n", explode("new_guild_name|", cch)[1])[0];
							bool bad_name = false, already_taken = false;
							int adaBrp = 0;
							string check_name = to_lower(newName);
							for (int i = 0; i < swear_words.size(); i++) {
								if (check_name.find(swear_words[i]) != string::npos) {
									bad_name = true;
									break;
								}
							} for (Guild check_guild_name : guilds) {
								if (to_lower(check_guild_name.guild_name) == check_name) {
									already_taken = true;
									break;
								}
							}
							if (already_taken or bad_name or newName.size() < 3 or newName.size() > 15 or special_char(newName)) goto lad;
							if (modify_inventory(peer, 7190, adaBrp = -1) == 0) {
								if (pInfo(peer)->guild_id != 0) {
									uint32_t guild_id = pInfo(peer)->guild_id;
									uint32_t my_rank = 0;
									vector<Guild>::iterator p = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
									if (p != guilds.end()) {
										Guild* guild_information = &guilds[p - guilds.begin()];
										for (GuildMember member_search : guild_information->guild_members) {
											if (member_search.member_name == pInfo(peer)->tankIDName) {
												my_rank = member_search.role_id;
												break;
											}
										}
										if (my_rank == 3) {
											Guild* set_guild = &guilds[p - guilds.begin()];
											set_guild->guild_name = newName;
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert(get_guild_info(peer));
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						lad: {
							uint32_t my_rank = 0;
							string guildName = "";
							if (pInfo(peer)->guild_id != 0) {
								uint32_t guild_id = pInfo(peer)->guild_id;
								vector<Guild>::iterator p = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
								if (p != guilds.end()) {
									Guild* guild_information = &guilds[p - guilds.begin()];
									for (GuildMember member_search : guild_information->guild_members) {
										guildName = guild_information->guild_name;
										if (member_search.member_name == pInfo(peer)->tankIDName) {
											my_rank = member_search.role_id;
											break;
										}
									}
								}
								if (my_rank == 3) {
									DialogBuilder guild("`o");
									guild.add_label_icon(true, 5814, "`wChange Guild Name``")
										.add_textbox("Current Guild Name:" + guildName)
										.add_smalltext("`4You can't use that name for your guild name or the name is already taken``")
										.add_text_input(15, "new_guild_name", "Guild Name:", "")
										.add_button("confirm", "Confirm")
										.end_dialog("change_guild_name", "Close", "");
									enet_peer_send(peer, 0, Variant{ "OnDialogRequest" }.push(guild.to_string()).pack());
								}
								else {
									gamepacket_t p;
									p.Insert("OnTextOverlay"), p.Insert("Only Guild Leader can Change Guild Name!"), p.CreatePacket(peer);
									break;
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnTextOverlay"), p.Insert("You must have Guild to use Guild Name Changer!"), p.CreatePacket(peer);
								break;
							}
							break;
							}
						}
						else if (cch.find("action|dialog_return\ndialog_name|change_mid\nmtps_id|") != string::npos) {
							if (has_playmod(pInfo(peer), "Recently Name-Changed")) {
								Algorithm::send_console(peer, "You're on cooldown");
								break;
							}
							TextScanner parser(cch);
							string newName = parser.get("mtps_id", 1);
							//cout << (newName.empty() ? "The name is empty" : newName) << endl;
							if (newName.empty()) break;
							int got = 0;
							replaceAll(newName, "\n", "");
							string check_name = to_lower(newName);
							string exist = "database/players/" + newName + "_.json";
							for (int i = 0; i < swear_words.size(); i++) {
								if (check_name.find(swear_words[i]) != string::npos) {
									SendBirth_Dialog(peer, "`4Oops! That is not a great choice for a name... kids play on here, man!``");
									break;
								}
							}
							if (newName.size() < 3 or newName.size() > 18) {
								SendBirth_Dialog(peer, "`4Ooops! Your `wTayoPsID `4must be between `o3 `4and `o18 `4characters long.``");
								break;
							}
							else if (special_char(newName)) {
								SendBirth_Dialog(peer, "`4Oops! You can only use letters and numbers in your TayoPsID.``");
								break;
							}
							else if (_access_s(exist.c_str(), 0) == 0) {
								SendBirth_Dialog(peer, "`4Oops! The name `w" + newName + " `4is unavailable. Please choose a different name.``");
								break;
							}
							pInfo(peer)->ChoosingName = newName;
							SendBirth_Dialog(peer, "Are you absolutely sure you want to use up 1 `2Birth Certificate ``to permanently change your name to \"`9" + newName + "``\"?", true);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|confirm_hid") != string::npos) {
							if (pInfo(peer)->ChoosingName.empty() || pInfo(peer)->lastchoosenitem != 1280) break;
							if (has_playmod(pInfo(peer), "Recently Name-Changed")) break;
							int remove = -1, adaBrp = 0;
							modify_inventory(peer, 1280, adaBrp);
							if (adaBrp == 0) break;
							if (modify_inventory(peer, 1280, remove) == 0) {
								try {
									string oldName = "database/players/" + pInfo(peer)->tankIDName + "_.json";
									string newName = "database/players/" + pInfo(peer)->ChoosingName + "_.json";
									rename(oldName.c_str(), newName.c_str());
									World world_ = get_world(pInfo(peer)->world);
									for (int i = 0; i < pInfo(peer)->worlds_owned.size(); i++)
										if (get_world(pInfo(peer)->worlds_owned[i]).owner_name != pInfo(peer)->tankIDName)
											pInfo(peer)->worlds_owned.erase(pInfo(peer)->worlds_owned.begin() + i);
									// GUILDS DATA CHANGE IF EXIST
									if (pInfo(peer)->guild_id != 0) {
										uint32_t guild_id = pInfo(peer)->guild_id;
										vector<Guild>::iterator p = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
										if (p != guilds.end()) {
											Guild* guild_information = &guilds[p - guilds.begin()];
											for (int i_ = 0; i_ < guild_information->guild_members.size(); i_++) {
												GuildMember* edit_member = &guild_information->guild_members[i_];
												if (edit_member->member_name == pInfo(peer)->tankIDName) {
													edit_member->member_name = pInfo(peer)->ChoosingName;
												}
											}
										}
									}
									// PLAYERS DATA CHANGE
									pInfo(peer)->old_name = pInfo(peer)->tankIDName;
									pInfo(peer)->tankIDName = pInfo(peer)->ChoosingName;
									pInfo(peer)->name_color = ((pInfo(peer)->tmod == 1 and not pInfo(peer)->d_name.empty()) ? (pInfo(peer)->dev == 1 ? "`6@" : pInfo(peer)->admin == 1 ? "`8@" : pInfo(peer)->supermod == 1 ? "`b@" : (pInfo(peer)->tmod == 1) ? "`#@" :
										pInfo(peer)->glory == 1 ? "`c[GLORY] " : pInfo(peer)->vvip == 1 ? "`9[VVIP] " : pInfo(peer)->vip == 1 ? "`1[VIP] " : "`0") : (pInfo(peer)->tankIDName == world_.owner_name) ? "`2" : (guild_access(peer, world_.guild_id) or find(world_.admins.begin(), world_.admins.end(), pInfo(peer)->uid) != world_.admins.end()) ? "`^" :
										pInfo(peer)->is_legend and not(pInfo(peer)->tankIDName == world_.owner_name or (guild_access(peer, world_.guild_id) or find(world_.admins.begin(), world_.admins.end(), pInfo(peer)->uid) != world_.admins.end())) ? "`9" : "`0");
									pInfo(peer)->modName = pInfo(peer)->name_color + pInfo(peer)->tankIDName;
									pInfo(peer)->d_name = "";
									if (not pInfo(peer)->Alt_Accounts.empty()) {
										for (int i = 0; i < pInfo(peer)->Alt_Accounts.size(); i++) {
											if (pInfo(peer)->tankIDName == pInfo(peer)->Alt_Accounts[i]) {
												pInfo(peer)->Alt_Accounts.erase(pInfo(peer)->Alt_Accounts.begin() + i);
												break;
											}
										}
									}
									PlayMods new_playmod{};
									new_playmod.id = 124;
									new_playmod.time = time(nullptr) + 2592000;
									pInfo(peer)->playmods.push_back(new_playmod);
									// WORLDS OWNED DATA CHANGE
									for (int x = 0; x < pInfo(peer)->worlds_owned.size(); x++) {
										string name_ = pInfo(peer)->worlds_owned[x];
										vector<World>::iterator px = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (px != worlds.end()) {
											World* world_ = &worlds[px - worlds.begin()];
											update_world(peer, world_, pInfo(peer)->tankIDName, pInfo(peer)->worlds_owned[x]);
										}
									}
									gamepacket_t p2(0, pInfo(peer)->netID);
									p2.Insert("OnNameChanged");
									p2.Insert((not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->name_color + pInfo(peer)->ChoosingName) + (pInfo(peer)->is_datemaster ? " `4Datemaster" : "") + (pInfo(peer)->is_legend ? " of Legend" : "") + "``");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											p2.CreatePacket(currentPeer);
										}
									}

									pInfo(peer)->ChoosingName = "";
									gamepacket_t p;
									p.Insert("SetHasGrowID"), p.Insert(1), p.Insert(pInfo(peer)->tankIDName), p.Insert(pInfo(peer)->tankIDPass), p.CreatePacket(peer);

									//update_monitor(peer);
									//write_uid_data(pInfo(peer));
									save_player(pInfo(peer), false);
									//Algorithm::send_console(peer, "Disabled!");
									InsertUIDData(pInfo(peer)); 
								}
								catch (exception& e) {
									tayo_warn(e.what());
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|surgery\nbuttonClicked|tool") != string::npos) {
							if (pInfo(peer)->surgery_started) {
								int count = atoi(cch.substr(59, cch.length() - 59).c_str());
								if (count == 999) end_surgery(peer);
								else load_surgery(peer, count);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dynamo_extractor\nbuttonClicked|extractOnceObj_") != string::npos) {
							if (pInfo(peer)->lastwrenchb != 6142) break;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								int uid = atoi(cch.substr(79, cch.length() - 79).c_str());
								for (int i_ = 0; i_ < world_->drop.size(); i_++) {
									if (world_->drop[i_].id != 0 && world_->drop[i_].x > 0 && world_->drop[i_].y > 0 && world_->drop[i_].uid == uid) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
										int c_ = world_->drop[i_].count;
										if (modify_inventory(peer, world_->drop[i_].id, c_) == 0) {
											p.Insert("You have extracted " + to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name + ".");
											int32_t to_netid = pInfo(peer)->netID;
											PlayerMoving data_{}, data2_{};
											data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = 0, data_.plantingTree = world_->drop[i_].uid;
											data2_.x = world_->drop[i_].x, data2_.y = world_->drop[i_].y, data2_.packetType = 19, data2_.plantingTree = 500, data2_.punchX = world_->drop[i_].id, data2_.punchY = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											BYTE* raw2 = packPlayerMoving(&data2_);
											raw2[3] = 5;
											memcpy(raw2 + 8, &to_netid, 4);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[]raw, raw2;
											world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
										}
										else p.Insert("No inventory space.");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|compactor\ncount|") != string::npos) {
							int count = atoi(cch.substr(49, cch.length() - 49).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (got < count) break;
							if (items[item].r_1 == 2037 || items[item].r_2 == 2037 || items[item].r_1 == 2035 || items[item].r_2 == 2035 || items[item].r_1 + items[item].r_2 == 0 || items[item].blockType != BlockTypes::CLOTHING || items[item].untradeable || item == 1424 || items[item].rarity > 200) break;
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									string received = "";
									vector<pair<int, int>> receivingitems;
									vector<int> list = { items[item].r_1,  items[item].r_2,  items[item].r_1 - 1 ,  items[item].r_2 - 1 }, random_compactor_rare = { 3178, 2936, 5010, 2644, 2454, 2456, 2458, 2460, 6790, 9004, 11060 };
									for (int i = 0; i < count; i++) {
										if (rand() % items[item].newdropchance < 20) {
											bool dublicate = false;
											int given_item = list[rand() % list.size()];
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else if (rand() % 50 < 1) {
											bool dublicate = false;
											int given_item = 0;
											if (rand() % 100 < 1) given_item = random_compactor_rare[rand() % random_compactor_rare.size()];
											else given_item = 2462;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else {
											bool dublicate = false;
											int given_item = 112, given_count = (rand() % items[item].max_gems) / 2 + 1;
											if (rand() % 3 < 1) given_item = 856, given_count = 1;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += given_count;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, given_count));
										}
									}
									int remove = count * -1;
									modify_inventory(peer, item, remove);
									for (int i = 0; i < receivingitems.size(); i++) {
										if (receivingitems.size() == 1) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
										else {
											if (receivingitems.size() - 1 == i)received += "and " + to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
											else if (i != receivingitems.size()) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``") + ", ";
										}
										int given_count = receivingitems[i].second;
										if (receivingitems[i].first != 112) {
											if (modify_inventory(peer, receivingitems[i].first, given_count) == 0) {
											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = receivingitems[i].first, drop_block_.count = given_count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (pInfo(peer)->lastwrenchx * 32) + rand() % 17, drop_block_.y = (pInfo(peer)->lastwrenchy * 32) + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										else {
											gamepacket_t p;
											p.Insert("OnSetBux"), p.Insert(pInfo(peer)->gems += given_count), p.Insert(0), p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
									}
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``"), p.Insert(0);
									p2.Insert("OnConsoleMessage"), p2.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``");
									for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
										if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != name_) continue;
										p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|lock_compactor\ncount|") != string::npos) {
							int count = atoi(cch.substr(54, cch.length() - 54).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (got < count) break;
							vector<int> list{ 1796, 242, 7188 };
							// todo here
							if (find(list.begin(), list.end(), item) != list.end() == false) break;
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									string received = "";
									vector<pair<int, int>> receivingitems;
									vector<int> rare_prize = { 5140 };
									for (int i = 0; i < count; i++) {
										if (rand() % 100 + 1 < 50) {
											bool dublicate = false;
											int given_item = rare_prize[rand() % rare_prize.size()];
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else {
											bool dublicate = false;
											int given_item = 112, given_count = (rand() % 25000) / 2 + 1;
											if (rand() % 3 < 1) given_item = 856, given_count = 1;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += given_count;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, given_count));
										}
									}
									int remove = count * -1;
									modify_inventory(peer, item, remove);
									for (int i = 0; i < receivingitems.size(); i++) {
										if (receivingitems.size() == 1) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
										else {
											if (receivingitems.size() - 1 == i)received += "and " + to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
											else if (i != receivingitems.size()) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``") + ", ";
										}
										int given_count = receivingitems[i].second;
										if (receivingitems[i].first != 112) {
											if (modify_inventory(peer, receivingitems[i].first, given_count) == 0) {
											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = receivingitems[i].first, drop_block_.count = given_count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (pInfo(peer)->lastwrenchx * 32) + rand() % 17, drop_block_.y = (pInfo(peer)->lastwrenchy * 32) + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										else {
											gamepacket_t p;
											p.Insert("OnSetBux"), p.Insert(pInfo(peer)->gems += given_count), p.Insert(0), p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
									}
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``"), p.Insert(0);
									p2.Insert("OnConsoleMessage"), p2.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``");
									for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
										if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != name_) continue;
										p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|iotm_compactor\ncount|") != string::npos) {
							int count = atoi(cch.substr(54, cch.length() - 54).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (got < count) break;
							vector<int> list{ 4534, 4628, 5020, 5206, 5322, 5712, 6294, 6338, 6758, 6892, 7196, 7384, 7568, 7570, 7572, 7574, 7676, 7678, 7680, 7682, 7836, 7838, 7840, 7842, 8006, 8010, 8288, 8290, 8292, 8294, 8304, 8432, 8434, 8436, 8438, 8576, 8578, 8580, 8816, 8818, 8820, 8822, 8898, 8900, 8902, 8904, 9008, 9010, 9012, 9014, 9116, 9118, 9120, 9122, 9136, 9138, 9236, 9348, 9408, 9462, 9464, 9606, 9648, 9760, 10044, 10128, 10166, 10246, 10426, 10618, 10666, 10718, 10810, 11006, 11116, 11232, 11306, 11440, 11506, 11704, 11750, 11882, 11906, 12246, 12344, 12432, 12642, 12834, 12872, 13022, 4988, 5480, 4590, 4746, 5088, 7192, 8582, 5138, 5154, 8012, 8008, 10496, 10914, 11308, 11442, 11508, 11552, 11662, 11706, 11752, 11884, 11908, 12248, 12346, 12434, 12644, 12836, 12874, 12958, 13024 };
							// todo here
							if (find(list.begin(), list.end(), item) != list.end() == false) break;
							else {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									string received = "";
									vector<pair<int, int>> receivingitems;
									vector<int> rare_prize = { 5140 };
									for (int i = 0; i < count; i++) {
										if (rand() % 2 < 1) {
											bool dublicate = false;
											int given_item = rare_prize[rand() % rare_prize.size()];
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += 1;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, 1));
										}
										else {
											bool dublicate = false;
											int given_item = 112, given_count = (rand() % 25000) / 2 + 1;
											if (rand() % 3 < 1) given_item = 856, given_count = 1;
											for (int i = 0; i < receivingitems.size(); i++) {
												if (receivingitems[i].first == given_item) {
													dublicate = true;
													receivingitems[i].second += given_count;
												}
											}
											if (dublicate == false) receivingitems.push_back(make_pair(given_item, given_count));
										}
									}
									int remove = count * -1;
									modify_inventory(peer, item, remove);
									for (int i = 0; i < receivingitems.size(); i++) {
										if (receivingitems.size() == 1) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
										else {
											if (receivingitems.size() - 1 == i)received += "and " + to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``");
											else if (i != receivingitems.size()) received += to_string(receivingitems[i].second) + " " + (items[item].r_1 == receivingitems[i].first || items[item].r_2 == receivingitems[i].first || items[item].r_2 - 1 == receivingitems[i].first || items[item].r_1 - 1 == receivingitems[i].first ? "`2" + items[receivingitems[i].first].name + "``" : (receivingitems[i].first == 112) ? items[receivingitems[i].first].name : "`1" + items[receivingitems[i].first].name + "``") + ", ";
										}
										int given_count = receivingitems[i].second;
										if (receivingitems[i].first != 112) {
											if (modify_inventory(peer, receivingitems[i].first, given_count) == 0) {
											}
											else {
												WorldDrop drop_block_{};
												drop_block_.id = receivingitems[i].first, drop_block_.count = given_count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = (pInfo(peer)->lastwrenchx * 32) + rand() % 17, drop_block_.y = (pInfo(peer)->lastwrenchy * 32) + rand() % 17;
												dropas_(world_, drop_block_);
											}
										}
										else {
											gamepacket_t p;
											p.Insert("OnSetBux"), p.Insert(pInfo(peer)->gems += given_count), p.Insert(0), p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
									}
									gamepacket_t p, p2;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``"), p.Insert(0);
									p2.Insert("OnConsoleMessage"), p2.Insert("`7[``From crushing " + to_string(count) + " " + items[item].name + ", " + pInfo(peer)->tankIDName + " extracted " + received + ".`7]``");
									for (ENetPeer* currentPeer_event = server->peers; currentPeer_event < &server->peers[server->peerCount]; ++currentPeer_event) {
										if (currentPeer_event->state != ENET_PEER_STATE_CONNECTED or currentPeer_event->data == NULL or pInfo(currentPeer_event)->world != name_) continue;
										p.CreatePacket(currentPeer_event), p2.CreatePacket(currentPeer_event);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nisStatsWorldBlockUsableByPublic") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							bool world_public = atoi(explode("\n", t_[3])[0].c_str()), floating_public = atoi(explode("\n", t_[4])[0].c_str()), changed = false;
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (pInfo(peer)->tankIDName == world_->owner_name) {
									for (int i_ = 0; i_ < world_->gscan.size(); i_++) {
										if (world_->gscan[i_].x == pInfo(peer)->lastwrenchx and world_->gscan[i_].y == pInfo(peer)->lastwrenchy) {
											changed = true;
											world_->gscan[i_].world_public = world_public;
											world_->gscan[i_].floating_public = floating_public;
										}
									}
								}
								if (changed == false) {
									WorldGrowscan gscan_{};
									gscan_.x = pInfo(peer)->lastwrenchx, gscan_.y = pInfo(peer)->lastwrenchy;
									gscan_.world_public = world_public, gscan_.floating_public = floating_public;
									world_->gscan.push_back(gscan_);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|back_to_gscan\n") != string::npos || cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|back_to_gscan\n") != string::npos) {
							edit_tile(peer, pInfo(peer)->lastwrenchx, pInfo(peer)->lastwrenchy, 32);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|floatingItems\n") != string::npos) {
							send_growscan_floating(peer, "start", "1");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|search_") != string::npos) {
							try {
								string type = cch.substr(65, 1);
								string search = cch.substr(79, cch.length() - 79);
								if (type.empty()) break;
								if (search.empty()) break;
								replace_str(search, "\n", "");
								replace_str(type, "\n", "");
								//if (type.empty()) break;
								//if (search.empty()) break;
								send_growscan_floating(peer, search, type);
							}
							catch (out_of_range) {
								tayo_warn("Server error invalid (out of range) on " + cch);
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|worldBlocks\n") != string::npos || cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|worldBlocks\n") != string::npos) {
							if (cch.find("action|dialog_return\ndialog_name|statsblock\nbuttonClicked|worldBlocks\n") != string::npos) send_growscan_worldblocks(peer, "start", "1");
							if (cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|worldBlocks\n") != string::npos) send_growscan_worldblocks(peer, "start", "1");
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|statsblockworld\nbuttonClicked|search_") != string::npos) {
							try {
								if (cch.length() < 84) break;
								string type = cch.substr(70, 1); 
								if (type.empty()) break; 
								string search = cch.substr(84, cch.length() - 84);
								if (search.empty()) break;
								replace_str(search, "\n", "");
								replace_str(type, "\n", "");
								send_growscan_worldblocks(peer, search, type);
							}
							catch (exception) { break; }
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_scarf_of_seasons\nbuttonClicked") != string::npos) {
							if (pInfo(peer)->necklace == 11818) pInfo(peer)->i_11818_1 = 0, pInfo(peer)->i_11818_2 = 0, update_clothes(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|title_edit\nbuttonClicked|") != string::npos) {
							try {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								int total = 4;
								if (pInfo(peer)->drtitle) {
									pInfo(peer)->drt = atoi(explode("\n", t_.at(total++)).at(0).c_str());
									string thetag = (pInfo(peer)->mod || pInfo(peer)->admin ? "@" : "");
									pInfo(peer)->d_name = (pInfo(peer)->drt ? "`4" + thetag : pInfo(peer)->name_color) + (pInfo(peer)->drt ? "Dr." : "") + pInfo(peer)->tankIDName;
									if (!pInfo(peer)->drt) pInfo(peer)->d_name = "";
									{
										gamepacket_t p2(0, pInfo(peer)->netID);
										p2.Insert("OnNameChanged"), p2.Insert((not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)));
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											p2.CreatePacket(currentPeer);
										}
									}
								}
								if (pInfo(peer)->level >= 125) pInfo(peer)->lvl125 = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								if (pInfo(peer)->legend) {
									pInfo(peer)->is_legend = atoi(explode("\n", t_.at(total++)).at(0).c_str());
									string modtag = (pInfo(peer)->mod || pInfo(peer)->admin ? "@" : "");
									{
										gamepacket_t p2(0, pInfo(peer)->netID);
										p2.Insert("OnNameChanged"), p2.Insert((not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + (pInfo(peer)->is_legend ? " of Legend``" : ""));
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
											p2.CreatePacket(currentPeer);
										}
									}
								}
								if (pInfo(peer)->gp) pInfo(peer)->donor = atoi(explode("\n", t_.at(total++)).at(0).c_str()), pInfo(peer)->master = atoi(explode("\n", t_.at(total++)).at(0).c_str());
								update_clothes(peer);
							}
							catch (out_of_range& e) {
								tayo_warn(e.what());
								break;
							}
							break;
							}

						else if (cch.find("action|dialog_return\ndialog_name|dialog_scarf_of_seasons\ncheckbox") != string::npos) {
							try {
								if (pInfo(peer)->necklace == 11818) {
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 4) break;
									for (int i = 3; i <= 10; i++) {
										if (i <= 6 && atoi(explode("\n", t_.at(i)).at(0).c_str()) == 1) pInfo(peer)->i_11818_1 = i - 3;
										else if (atoi(explode("\n", t_.at(i)).at(0).c_str()) == 1) pInfo(peer)->i_11818_2 = i - 7;
									}
									update_clothes(peer);
								}
							}
							catch (out_of_range) {
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|bulletin_edit\nsign_text|\ncheckbox_locked|") != string::npos) {
							try {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								bool public_can_add = atoi(explode("\n", t_.at(4)).at(0).c_str()), hide_names = atoi(explode("\n", t_.at(5)).at(0).c_str());
								bool changed = false;
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (pInfo(peer)->tankIDName == world_->owner_name) {
										for (int i_ = 0; i_ < world_->bulletins.size(); i_++) {
											if (world_->bulletins.at(i_).x == pInfo(peer)->lastwrenchx and world_->bulletins.at(i_).y == pInfo(peer)->lastwrenchy) {
												changed = true;
												world_->bulletins.at(i_).public_can_add = public_can_add;
												world_->bulletins.at(i_).hide_names = hide_names;
											}
										}
									}
									if (changed == false) {
										WorldBulletinSettings set_{};
										set_.x = pInfo(peer)->lastwrenchx, set_.y = pInfo(peer)->lastwrenchy, set_.public_can_add = public_can_add, set_.hide_names = hide_names;
										world_->bulletins.push_back(set_);
									}
								}
							}
							catch (out_of_range) {
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|camera_edit\ncheckbox_showpick|") != string::npos) {
							try {
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								bool show_item_taking = atoi(explode("\n", t_.at(3)).at(0).c_str()), show_item_dropping = atoi(explode("\n", t_.at(4)).at(0).c_str()), show_people_entering = atoi(explode("\n", t_.at(5)).at(0).c_str()), show_people_exiting = atoi(explode("\n", t_.at(6)).at(0).c_str()), dont_show_owner = atoi(explode("\n", t_.at(7)).at(0).c_str()), dont_show_admins = atoi(explode("\n", t_.at(8)).at(0).c_str()), dont_show_noaccess = atoi(explode("\n", t_.at(9)).at(0).c_str()), changed = false;
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									for (int i_ = 0; i_ < world_->cctv_settings.size(); i_++) {
										if (world_->cctv_settings.at(i_).x == pInfo(peer)->lastwrenchx and world_->cctv_settings.at(i_).y == pInfo(peer)->lastwrenchy) {
											changed = true;
											world_->cctv_settings.at(i_).show_item_taking = show_item_taking;
											world_->cctv_settings.at(i_).show_item_dropping = show_item_dropping;
											world_->cctv_settings.at(i_).show_people_entering = show_people_entering;
											world_->cctv_settings.at(i_).show_people_exiting = show_people_exiting;
											world_->cctv_settings.at(i_).dont_show_owner = dont_show_owner;
											world_->cctv_settings.at(i_).dont_show_admins = dont_show_admins;
											world_->cctv_settings.at(i_).dont_show_noaccess = dont_show_noaccess;
										}
									}
									if (changed == false) {
										WorldCCTVSettings cctvs_{};
										cctvs_.x = pInfo(peer)->lastwrenchx, cctvs_.y = pInfo(peer)->lastwrenchy;
										cctvs_.show_item_taking = show_item_taking, cctvs_.show_item_dropping = show_item_dropping, cctvs_.show_people_entering = show_people_entering, cctvs_.show_people_exiting = show_people_exiting, cctvs_.dont_show_owner = dont_show_owner, cctvs_.dont_show_admins = dont_show_admins, cctvs_.dont_show_noaccess = dont_show_noaccess;
										world_->cctv_settings.push_back(cctvs_);
									}
								}
							}
							catch (out_of_range) {
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|camera_edit\nbuttonClicked|clear") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								for (int i_ = 0; i_ < world_->cctv.size(); i_++)if (world_->cctv[i_].x == pInfo(peer)->lastwrenchx and world_->cctv[i_].y == pInfo(peer)->lastwrenchy) {
									if (i_ != 0) {
										world_->cctv.erase(world_->cctv.begin() + i_);
										i_--;
									}
								}
							}
							{
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("`2Camera log cleared.``"), p.Insert(0), p.Insert(0), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|b_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 6896, 5212, 6948, 1068, 1966, 1836, 5080, 10754, 1874, 6946 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 1068) count = 10;
							if (find(pInfo(peer)->bb_p.begin(), pInfo(peer)->bb_p.end(), lvl = reward * 5) == pInfo(peer)->bb_p.end()) {
								if (pInfo(peer)->bb_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->bb_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Builder Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										builder_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|autoclave\nbuttonClicked|tool") != string::npos) {
							int itemtool = atoi(cch.substr(61, cch.length() - 61).c_str());
							if (itemtool == 1258 || itemtool == 1260 || itemtool == 1262 || itemtool == 1264 || itemtool == 1266 || itemtool == 1268 || itemtool == 1270 || itemtool == 4308 || itemtool == 4310 || itemtool == 4312 || itemtool == 4314 || itemtool == 4316 || itemtool == 4318) {
								int got = 0;
								modify_inventory(peer, itemtool, got);
								if (got >= 20) {
									pInfo(peer)->lastchoosenitem = itemtool;
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9Autoclave``|left|4322|\nadd_spacer|small|\nadd_textbox|Are you sure you want to destroy 20 " + items[itemtool].ori_name + " in exchange for one of each of the other 12 surgical tools?|left|\nadd_button|verify|Yes!|noflags|0|0|\nend_dialog|autoclave|Cancel||");
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|autoclave\nbuttonClicked|verify") != string::npos) {
							int removeitem = pInfo(peer)->lastchoosenitem, inventory_space = 12, slots = get_free_slots(pInfo(peer)), got = 0;
							modify_inventory(peer, removeitem, got);
							if (got >= 20) {
								vector<int> noobitems{ 1262, 1266, 1264, 4314, 4312, 4318, 4308, 1268, 1258, 1270, 4310, 4316 };
								bool toobig = false;
								for (int i_ = 0, remove = 0; i_ < pInfo(peer)->inv.size(); i_++) for (int i = 0; i < noobitems.size(); i++) {
									if (pInfo(peer)->inv[i_].id == noobitems[i]) {
										if (pInfo(peer)->inv[i_].count == 200) toobig = true;
										else inventory_space -= 1;
									}
								}
								gamepacket_t p;
								p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
								if (toobig == false && slots >= inventory_space) {
									modify_inventory(peer, removeitem, got = -20);
									for (int i = 0; i < noobitems.size(); i++) {
										if (noobitems[i] == removeitem) continue;
										modify_inventory(peer, noobitems[i], got = 1);
									}
									gamepacket_t p2;
									p.Insert("[`3I swapped 20 " + items[removeitem].ori_name + " for 1 of every other instrument!``]");
									p2.Insert("OnTalkBubble"), p2.Insert("[`3I swapped 20 " + items[removeitem].name + " for 1 of every other instrument!``]"), p2.CreatePacket(peer);
								}
								else p.Insert("No inventory space!");
								p.Insert(0), p.Insert(1), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|extractor\nbuttonClicked|extractOnceObj_") != string::npos) {
							int got = 0;
							modify_inventory(peer, 6140, got);
							if (got >= 1) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									int uid = atoi(cch.substr(72, cch.length() - 72).c_str());
									if (world_->owner_name != pInfo(peer)->tankIDName and not pInfo(peer)->admin and (!guild_access(peer, world_->guild_id) and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end())) break;
									for (int i_ = 0; i_ < world_->drop.size(); i_++) {
										if (world_->drop[i_].id != 0 && world_->drop[i_].x > 0 && world_->drop[i_].y > 0 && world_->drop[i_].uid == uid) {
											gamepacket_t p;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
											int c_ = world_->drop[i_].count;
											if (modify_inventory(peer, world_->drop[i_].id, c_) == 0) {
												modify_inventory(peer, 6140, got = -1);
												p.Insert("You have extracted " + to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name + ".");
												int32_t to_netid = pInfo(peer)->netID;
												PlayerMoving data_{}, data2_{};
												data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = 0, data_.plantingTree = world_->drop[i_].uid;
												data2_.x = world_->drop[i_].x, data2_.y = world_->drop[i_].y, data2_.packetType = 19, data2_.plantingTree = 500, data2_.punchX = world_->drop[i_].id, data2_.punchY = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												BYTE* raw2 = packPlayerMoving(&data2_);
												raw2[3] = 5;
												memcpy(raw2 + 8, &to_netid, 4);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
													send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[]raw, raw2;
												world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
											}
											else p.Insert("No inventory space.");
											p.CreatePacket(peer);
										}
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zombie_back\nbuttonClicked|zomb_price_") != string::npos) {
							int item = atoi(cch.substr(70, cch.length() - 70).c_str());
							if (item <= 0 || item >= items.size() || items[item].zombieprice == 0) continue;
							pInfo(peer)->lockeitem = item;
							int zombie_brain = 0, pile = 0, total = 0;
							modify_inventory(peer, 4450, zombie_brain);
							modify_inventory(peer, 4452, pile);
							total += zombie_brain + (pile * 100);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (total >= items[item].zombieprice) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].zombieprice) + " Zombie Brains. Are you sure you want to buy it? You have " + setGems(total) + " Zombie Brains.|left|\nadd_button|zomb_item_|Yes, please|noflags|0|0|\nadd_button|back|No, thanks|noflags|0|0|\nend_dialog|zombie_purchase|Hang Up||\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].zombieprice) + " Zombie Brains. You only have " + setGems(total) + " Zombie Brains so you can't afford it. Sorry!|left|\nadd_button|chc3_1|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zurgery_back\nbuttonClicked|zurg_price_") != string::npos) {
							int item = atoi(cch.substr(71, cch.length() - 71).c_str());
							if (item <= 0 || item >= items.size() || items[item].surgeryprice == 0) continue;
							pInfo(peer)->lockeitem = item;
							int zombie_brain = 0, pile = 0, total = 0;
							modify_inventory(peer, 4298, zombie_brain);
							modify_inventory(peer, 4300, pile);
							total += zombie_brain + (pile * 100);
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							if (total >= items[item].surgeryprice) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].surgeryprice) + " Caduceus. Are you sure you want to buy it? You have " + setGems(total) + " Caduceus.|left|\nadd_button|zurg_item_|Yes, please|noflags|0|0|\nadd_button|back|No, thanks|noflags|0|0|\nend_dialog|zurgery_purchase|Hang Up||\n");
							else p.Insert("set_default_color|`o\nadd_label_with_icon|big|`wSales-Man``|left|4358|\nadd_textbox|" + items[item].name + " costs " + setGems(items[item].surgeryprice) + " Caduceus. You only have " + setGems(total) + " Caduceus so you can't afford it. Sorry!|left|\nadd_button|chc4_1|Back|noflags|0|0|\nend_dialog|3898|Hang Up||\n");
							p.CreatePacket(peer);
							break;
						}
						else if (cch == "action|dialog_return\ndialog_name|findmore\nget_item|") {
							if (pInfo(peer)->admin + pInfo(peer)->dev <= 0) break;
							string item_name = cch.substr(51, cch.length() - 51);
							SendCmd(peer, "/find " + item_name);
						}
						else if (cch.find("action|dialog_return\ndialog_name|giantpotogold\namt|") != string::npos) {
							int count = atoi(cch.substr(51, cch.length() - 51).c_str()), got = 0;
							modify_inventory(peer, pInfo(peer)->lastchoosenitem, got);
							if (got <= 0 || count <= 0 || count > items.size()) break;
							int item = pInfo(peer)->lastchoosenitem;
							if (items[item].untradeable == 1 || item == 1424 || items[item].rarity >= 363 || items[item].rarity == 0 || items[item].rarity < 1 || count > got) {
								gamepacket_t p;
								p.Insert("OnTalkBubble");
								p.Insert(pInfo(peer)->netID);
								if (count > got) p.Insert("You don't have that to give!");
								else p.Insert("I'm sorry, we can't accept items without rarity!");
								p.CreatePacket(peer);
							}
							else {
								pInfo(peer)->b_ra += count * items[item].rarity;
								modify_inventory(peer, pInfo(peer)->lastchoosenitem, count *= -1);
								if (pInfo(peer)->b_ra >= 20000) pInfo(peer)->b_lvl = 2;
								int chance = 29;
								if (pInfo(peer)->b_ra > 25000) chance += 7;
								if (pInfo(peer)->b_ra > 40000) chance += 25;
								if (rand() % 100 < chance && pInfo(peer)->b_ra >= 20000) {
									int give_count = 1, given_count = 1;
									vector<int> list{ 7978,5734, 7986,5724,7980,7990,5730,5726,5728,7988,7992 };
									if (pInfo(peer)->b_ra >= 40000 && rand() % 100 < 15) list = { 7978,5734, 7986,5724,7980,7990,5730,5726,5728,7988,7992, 7996,5718,5720,9418,5732,5722,8000,5740,8002,9414,11728,11730 };
									int given_item = list[rand() % list.size()];
									if (given_item == 7978 || given_item == 5734 || given_item == 7986 || given_item == 5724 || given_item == 7992 || given_item == 7980 || given_item == 7990) give_count = 5, given_count = 5;
									if (given_item == 5730 || given_item == 5726 || given_item == 5728 || given_item == 7988 || given_item == 7980 || given_item == 7990) give_count = 10, given_count = 10;
									if (modify_inventory(peer, given_item, given_count) == 0) {
										gamepacket_t p, p2;
										p.Insert("OnConsoleMessage"), p.Insert(a + "Thanks for your generosity! The pot overflows with `6" + (pInfo(peer)->b_ra < 40000 ? "20" : "40") + ",000 rarity``! Your `6Level 2 prize`` is a fabulous `2" + items[given_item].name + "!``"), p.CreatePacket(peer);
										p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert(a + "Thanks for your generosity! The pot overflows with `6" + (pInfo(peer)->b_ra < 40000 ? "20" : "40") + ",000 rarity``! Your `6Level 2 prize`` is a fabulous `2" + items[given_item].name + "!``"), p2.CreatePacket(peer);
										pInfo(peer)->b_lvl = 1, pInfo(peer)->b_ra = 0;
									}
									else {
										gamepacket_t p;
										p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
									}
								}
								else {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("Thank you for your generosity!"), p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|donation_box_edit\nbuttonClicked|clear_selected\n") != string::npos) {
							try {
								bool took = false, fullinv = false;
								gamepacket_t p3;
								p3.Insert("OnTalkBubble"), p3.Insert(pInfo(peer)->netID);
								string name_ = pInfo(peer)->world;
								vector<string> t_ = explode("|", cch);
								if (t_.size() < 4) break;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									if (world_->owner_name != pInfo(peer)->tankIDName and not pInfo(peer)->admin and not world_->owner_name.empty() and (!guild_access(peer, world_->guild_id) and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end())) break;
									WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
									if (!items[block_->fg].donation) break;
									for (int i_ = 0, remove_ = 0; i_ < block_->donates.size(); i_++, remove_++) {
										if (atoi(explode("\n", t_.at(4 + remove_)).at(0).c_str())) {
											int receive = block_->donates[i_].count;
											if (modify_inventory(peer, block_->donates[i_].item, block_->donates[i_].count) == 0) {
												took = true;
												gamepacket_t p;
												p.Insert("OnConsoleMessage"), p.Insert("`7[``" + pInfo(peer)->tankIDName + " receives `5" + to_string(receive) + "`` `w" + items[block_->donates[i_].item].name + "`` from `w" + block_->donates[i_].name + "``, how nice!`7]``");
												block_->donates.erase(block_->donates.begin() + i_), i_--;
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													p.CreatePacket(currentPeer);
													packet_(currentPeer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
												}
											}
											else fullinv = true;
										}
									}
									if (block_->donates.size() == 0) {
										if (block_->flags & 0x00400000) block_->flags ^= 0x00400000;
										WorldBlock block_ = world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
										PlayerMoving data_{};
										data_.packetType = 5, data_.punchX = pInfo(peer)->lastwrenchx, data_.punchY = pInfo(peer)->lastwrenchy, data_.characterState = 0x8;
										BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, &block_));
										BYTE* blc = raw + 56;
										form_visual(blc, block_, *world_, peer, false);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 112 + alloc_(world_, &block_), ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw, blc;
									}
								}
								if (fullinv) {
									p3.Insert("I don't have enough room in my backpack to get the item(s) from the box!");
									gamepacket_t p2;
									p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("`2(Couldn't get all of the gifts)``"), p2.CreatePacket(peer);
								}
								else if (took) p3.Insert("`2Box emptied.``");
								packet_(peer, "action|play_sfx\nfile|audio/page_turn.wav\ndelayMS|0");
								p3.CreatePacket(peer);
							}
							catch (out_of_range) {
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|password_reply\npassword|") != string::npos) {
							string password = cch.substr(57, cch.length() - 57).c_str();
							string name_ = pInfo(peer)->world;
							vector<World>::iterator pa = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (pa != worlds.end()) {
								World* world_ = &worlds[pa - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								if (block_->fg == 762 && block_->door_id != "") {
									gamepacket_t p;
									p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID);
									replace_str(password, "\n", "");
									transform(password.begin(), password.end(), password.begin(), ::toupper);
									if (block_->door_id != password) p.Insert("`4Wrong password!``");
									else {
										p.Insert(a + "`2The door opens!" + (block_->door_destination == "" ? " But nothing is behind it." : "") + "``");
										if (block_->door_destination != "") {
											gamepacket_t p3(0, pInfo(peer)->netID);
											p3.Insert("OnPlayPositioned"), p3.Insert("audio/door_open.wav"), p3.CreatePacket(peer);
											string door_target = block_->door_destination, door_id = "";
											World target_world = worlds[pa - worlds.begin()];
											int spawn_x = 0, spawn_y = 0;
											if (door_target.find(":") != string::npos) {
												vector<string> detales = explode(":", door_target);
												door_target = detales[0], door_id = detales[1];
											}
											//int ySize = (int)target_world.blocks.size() / world_->width, xSize = (int)target_world.blocks.size() / ySize;
											if (not door_id.empty()) {
												for (int i_ = 0; i_ < target_world.blocks.size(); i_++) {
													WorldBlock block_data = target_world.blocks[i_];
													if (block_data.fg == 762) continue;
													if (block_data.fg == 1684 or block_data.fg == 4482 or block_data.fg == 1912 or items[block_data.fg].blockType == BlockTypes::DOOR or items[block_data.fg].blockType == BlockTypes::PORTAL) {
														if (block_data.door_id == door_id) {
															spawn_x = i_ % target_world.width, spawn_y = i_ / target_world.width;
															break;
														}
													}
												}
											}
											join_world(peer, target_world.name, spawn_x, spawn_y, 250, false, true);
										}
									}
									p.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|2646\nID|") != string::npos) {
							int netID = atoi(cch.substr(41, cch.length() - 41).c_str());
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								string new_spotlight = "";
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[pInfo(peer)->lastwrenchx + (pInfo(peer)->lastwrenchy * world_->width)];
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world) continue;
									if (block_->spotlight == pInfo(currentPeer)->tankIDName || pInfo(currentPeer)->netID == netID) {
										if (pInfo(currentPeer)->netID == netID) {
											new_spotlight = pInfo(currentPeer)->tankIDName, pInfo(currentPeer)->spotlight = true;
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("All eyes are on you! (`$In the Spotlight`` mod added)"), p.CreatePacket(currentPeer);
										}
										else {
											gamepacket_t p;
											p.Insert("OnConsoleMessage"), p.Insert("Back to anonymity. (`$In the Spotlight`` mod removed)"), p.CreatePacket(currentPeer);
											pInfo(currentPeer)->spotlight = false;
										}
										if (new_spotlight != "") for (int i_ = 0; i_ < world_->blocks.size(); i_++) if (world_->blocks[i_].spotlight == new_spotlight) world_->blocks[i_].spotlight = "";
										gamepacket_t p;
										p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You shine the light on " + (new_spotlight == pInfo(peer)->tankIDName ? "yourself" : new_spotlight) + "!"), p.CreatePacket(peer);
										update_clothes(currentPeer);
									}
								}
								block_->spotlight = new_spotlight;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|grinder\ncount|") != string::npos) {
							int count = atoi(cch.substr(47, cch.length() - 47).c_str()), item = pInfo(peer)->lastchoosenitem, got = 0;
							modify_inventory(peer, item, got);
							if (items[item].grindable_count == 0 || got == 0 || count <= 0 || count * items[item].grindable_count > got) break;
							int remove = (count * items[item].grindable_count) * -1;
							modify_inventory(peer, item, remove);
							gamepacket_t p, p2;
							p.Insert("OnConsoleMessage"), p.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p.CreatePacket(peer);
							p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("Ground up " + to_string(count * items[item].grindable_count) + " " + items[item].name + " into " + to_string(count) + " " + items[items[item].grindable_prize].name + "!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
							{
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = items[item].grindable_prize, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									int c_ = count;
									if (modify_inventory(peer, items[item].grindable_prize, c_) != 0) {
										WorldDrop drop_block_{};
										drop_block_.id = items[item].grindable_prize, drop_block_.count = count, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x + rand() % 17, drop_block_.y = pInfo(peer)->y + rand() % 17;
										dropas_(world_, drop_block_);
									}
									{
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 221, data_.YSpeed = 221, data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.XSpeed = item;
										BYTE* raw = packPlayerMoving(&data_);
										send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										delete[] raw;
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worlds_list\nbuttonClicked|s_claimreward") != string::npos) {
							int reward = atoi(cch.substr(72, cch.length() - 72).c_str()), lvl = 0, count = 1;
							vector<int> list{ 6900, 6982, 6212, 3172, 9068, 6912, 10836, 3130, 8284 };
							if (reward <= 0 || reward > list.size()) break;
							if (list[reward - 1] == 10836) count = 100;
							if (list[reward - 1] == 6212) count = 50;
							if (list[reward - 1] == 3172 || list[reward - 1] == 6912) count = 25;
							if (find(pInfo(peer)->surg_p.begin(), pInfo(peer)->surg_p.end(), lvl = reward * 5) == pInfo(peer)->surg_p.end()) {
								if (pInfo(peer)->s_lvl >= lvl) {
									if (modify_inventory(peer, list[reward - 1], count) == 0) {
										pInfo(peer)->surg_p.push_back(lvl);
										packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										{
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("Congratulations! You have received your Surgeon Reward!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
										PlayerMoving data_{};
										data_.packetType = 17, data_.netID = 198, data_.YSpeed = 198, data_.x = pInfo(peer)->x + 16, data_.y = pInfo(peer)->y + 16;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										{
											PlayerMoving data_{};
											data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16, data_.packetType = 19, data_.plantingTree = 100, data_.punchX = list[reward - 1], data_.punchY = pInfo(peer)->netID;
											int32_t to_netid = pInfo(peer)->netID;
											BYTE* raw = packPlayerMoving(&data_);
											raw[3] = 5;
											memcpy(raw + 8, &to_netid, 4);
											send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											delete[] raw;
										}
										surgery_reward_show(peer);
									}
									else {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("You have full inventory space!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|dialog_cheat_menu") != string::npos)
						{
							TextScanner parser{ cch };
							try {
								World* world = GetCurrentWorld(pInfo(peer)->world);
								if (!world) break;
								bool cheat_1, cheat_2, cheat_3, cheat_4, cheat_5, cheat_6, cheat_7, cheat_8;
								//bool cheats[9] = { false, false, false, false, false, false, false, false };
								if (!parser.try_get("checkbox_cheat_antibounce", cheat_1)) break;
								if (!parser.try_get("checkbox_cheat_autofarm", cheat_2)) break;
								if (!parser.try_get("checkbox_cheat_hidechat", cheat_3)) break;
								if (!parser.try_get("checkbox_cheat_autocollect", cheat_4)) break;
								if (!parser.try_get("checkbox_cheat_fastpull", cheat_5)) break;
								if (!parser.try_get("checkbox_cheat_fastdrop", cheat_6)) break;
								if (!parser.try_get("checkbox_cheat_fasttrash", cheat_7)) break;
								if (!parser.try_get("checkbox_cheat_fastspin", cheat_8)) break;
				

								pInfo(peer)->Cheat_AB = cheat_1;
								pInfo(peer)->Cheat_AF = cheat_2;
								pInfo(peer)->Cheat_HideChat = cheat_3;
								//pInfo(peer)->Cheat_AutoCollect = cheat_4;
								pInfo(peer)->Cheat_FastPull = cheat_5;
								pInfo(peer)->Cheat_FastDrop = cheat_6;
								pInfo(peer)->Cheat_FastTrash = cheat_7;
								pInfo(peer)->Cheat_FastSpin = cheat_8;

								if (pInfo(peer)->Cheat_AB && world->no_antibounce) {
									pInfo(peer)->Cheat_AB = false;
									Algorithm::send_console(peer, "The owner disallowed antibounce here.");
								}
								if (not pInfo(peer)->Cheat_AF and pInfo(peer)->Cheat_AF_PunchX != 0 and pInfo(peer)->Cheat_AF_PunchY != 0 and pInfo(peer)->Cheat_AF_PlantingTree != 0) {
									//Algorithm::RemovePeerAFDuration(peer);
									pInfo(peer)->Cheat_AF_PunchX = 0;
									pInfo(peer)->Cheat_AF_PunchY = 0;
									pInfo(peer)->Cheat_AF_PlantingTree = 0;
									pInfo(peer)->Cheat_AF_isRunning = false;
									pInfo(peer)->isFacingLeft = false;
								}

								string button = "";
								if (!parser.try_get("buttonClicked", button)) break;
								//if (!pInfo(peer)->unlocked_ci) break;
								if (button == "restore_default") {
									Algorithm::RemovePeerAFDuration(peer);
									pInfo(peer)->Cheat_AB = false;
									pInfo(peer)->Cheat_AF = false;
									pInfo(peer)->Cheat_Speed = false;
									pInfo(peer)->Cheat_Jump = false;
									pInfo(peer)->Cheat_HideOther = false;
									pInfo(peer)->Cheat_HideChat = false;
									pInfo(peer)->Cheat_AF_PunchX = 0;
									pInfo(peer)->Cheat_AF_PunchY = 0;
									pInfo(peer)->Cheat_AF_PlantingTree = 0;
									pInfo(peer)->Cheat_AF_isRunning = false;
									pInfo(peer)->isFacingLeft = false;
									pInfo(peer)->Cheat_FastPull = false;
									pInfo(peer)->Cheat_FastDrop = false;
									pInfo(peer)->Cheat_FastTrash = false;
									pInfo(peer)->Cheat_AutoCollect = false;
									pInfo(peer)->Cheat_FastSpin = false;
									pInfo(peer)->Cheat_Spam = false;
									pInfo(peer)->Cheat_Spam_Delay = 3;
									pInfo(peer)->Cheat_Spam_Text = "`cTayoPs `wBest Server";
									update_clothes(peer);
									break;
								}
								if (button == "info" || button == "level" || button == "quests" ||
									button == "skills" || button == "badges" || button == "cheats" || button == "mod") {
									send_wrench_self(peer, button);
									break;
								}
							}
							catch (exception& e) {
								tayo_warn(format("Error {} on {}", e.what(), cch));
								break;
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zombie_purchase\nbuttonClicked|zomb_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].zombieprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].zombieprice;
							modify_inventory(peer, 4450, wl);
							modify_inventory(peer, 4452, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (wl >= price) modify_inventory(peer, 4450, price *= -1);
									else {
										modify_inventory(peer, 4450, wl *= -1);
										modify_inventory(peer, 4452, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4450, givewl);
										modify_inventory(peer, 4452, givedl);
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].zombieprice) + " Zombie Brains."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Zombie Brains!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|zurgery_purchase\nbuttonClicked|zurg_item_") != string::npos) {
							int item = pInfo(peer)->lockeitem;
							if (item <= 0 || item >= items.size() || items[item].surgeryprice == 0) continue;
							int allwl = 0, wl = 0, dl = 0, price = items[item].surgeryprice;
							modify_inventory(peer, 4298, wl);
							modify_inventory(peer, 4300, dl);
							allwl = wl + (dl * 100);
							if (allwl >= price) {
								int c_ = 1;
								if (modify_inventory(peer, item, c_) == 0) {
									if (item == 1486 && pInfo(peer)->C_QuestActive && pInfo(peer)->C_QuestKind == 11 && pInfo(peer)->C_QuestProgress < pInfo(peer)->C_ProgressNeeded) {
										pInfo(peer)->C_QuestProgress++;
										if (pInfo(peer)->C_QuestProgress >= pInfo(peer)->C_ProgressNeeded) {
											pInfo(peer)->C_QuestProgress = pInfo(peer)->C_ProgressNeeded;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Ring Quest task complete! Go tell the Ringmaster!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (item == 1486 && pInfo(peer)->quest_active && pInfo(peer)->quest_step == 6 && pInfo(peer)->quest_progress < 28) {
										pInfo(peer)->quest_progress++;
										if (pInfo(peer)->quest_progress >= 28) {
											pInfo(peer)->quest_progress = 28;
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9Legendary Quest step complete! I'm off to see a Wizard!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
										}
									}
									if (wl >= price) modify_inventory(peer, 4298, price *= -1);
									else {
										modify_inventory(peer, 4298, wl *= -1);
										modify_inventory(peer, 4300, dl *= -1);
										int givedl = (allwl - price) / 100;
										int givewl = (allwl - price) - (givedl * 100);
										modify_inventory(peer, 4298, givewl);
										modify_inventory(peer, 4300, givedl);
									}
									PlayerMoving data_{};
									data_.x = pInfo(peer)->lastwrenchx * 32 + 16, data_.y = pInfo(peer)->lastwrenchy * 32 + 16, data_.packetType = 19, data_.plantingTree = 500, data_.punchX = item, data_.punchY = pInfo(peer)->netID;
									int32_t to_netid = pInfo(peer)->netID;
									BYTE* raw = packPlayerMoving(&data_);
									raw[3] = 5;
									memcpy(raw + 8, &to_netid, 4);
									send_raw(peer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
									delete[] raw;
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("`3You bought " + items[item].name + " for " + setGems(items[item].surgeryprice) + " Caduceus."), p.CreatePacket(peer);
								}
								else {
									gamepacket_t p;
									p.Insert("OnConsoleMessage"), p.Insert("No inventory space."), p.CreatePacket(peer);
								}
							}
							else {
								gamepacket_t p;
								p.Insert("OnConsoleMessage"), p.Insert("`9You don't have enough Caduceus!``"), p.CreatePacket(peer);
							}
							break;
						}
						else if (cch.find("action|dialog_return\ndialog_name|worldreport\nreport_reason|") != string::npos) {
							if (pInfo(peer)->tankIDName == "") break;
							Algorithm::send_bubble(peer, pInfo(peer)->netID, "Thank you for your report. Now leave this world so you don't get punished along with the scammers!");
							break;
						}
						else if (cch.find("action|dialog_return") != string::npos) {
							call_dialog(peer, cch);
							break;
						}
						else if (cch.find("action|drop") != string::npos) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (pInfo(peer)->Cheat_FastDrop) {
								if (p != worlds.end()) {
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 4) break;
									int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
									if (id_ <= 0 or id_ >= items.size()) break;
									int dropCount = 0;
									World* world_ = &worlds[p - worlds.begin()];
									modify_inventory(peer, id_, c_);
									dropCount = 0 - c_;
									if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 4758) != world_->active_jammers.end()) {
										if (world_->owner_name != pInfo(peer)->tankIDName and not pInfo(peer)->admin and !guild_access(peer, world_->guild_id) and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end()) {
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("The Mini-Mod says no dropping items in this world!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
											break;
										}
									}
									if ((items[id_].untradeable and not pInfo(peer)->dev) or id_ == 18 or id_ == 32 or id_ == 1424 or id_ == 5816 or id_ == 13592 or id_ == 9500 or id_ == 8530 or id_ == 8532 or id_ == 9150 or id_ == 9636) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("You can't drop that.");
										p.CreatePacket(peer);
										break;
									}
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										WorldDrop drop_{};
										drop_.id = id_;
										drop_.count = c_;
										drop_.uid = uint16_t(world_->drop.size()) + 1;
										int a_ = rand() % 12;
										drop_.x = (pInfo(peer)->state == 16 ? pInfo(peer)->x - (a_ + 20) : (pInfo(peer)->x + 20) + a_);
										drop_.y = pInfo(peer)->y + rand() % 16;
										if (drop_.x / 32 < 0 or drop_.x / 32 >= world_->width or drop_.y / 32 < 0 or drop_.y / 32 >= world_->height) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert("You can't drop that here, face somewhere with open space.");
											p.CreatePacket(peer);
											break;
										}
										BlockTypes type_ = FOREGROUND;
										WorldBlock* block_ = &world_->blocks[(pInfo(peer)->state == 16 ? drop_.x / 32 : round((double)drop_.x / 32)) + (drop_.y / 32 * world_->width)];
										if (items[block_->fg].collisionType == 1 || block_->fg == 6 || items[block_->fg].entrance || items[block_->fg].toggleable and is_false_state(world_->blocks[(pInfo(peer)->state == 16 ? drop_.x / 32 : round((double)drop_.x / 32)) + (drop_.y / 32 * world_->width)], 0x00400000)) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert(items[block_->fg].blockType == BlockTypes::MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
											p.CreatePacket(peer);
											break;
										}
										int count_ = 0;
										bool dublicated = false;
										for (int i_ = 0; i_ < world_->drop.size(); i_++) {
											if (abs(world_->drop[i_].y - drop_.y) <= 16 and abs(world_->drop[i_].x - drop_.x) <= 16) {
												count_ += 1;
											}
											if (world_->drop[i_].id == id_) if (world_->drop[i_].count + drop_.count < 200) dublicated = true;
										}
										if (!dublicated) {
											if (count_ > 20) {
												gamepacket_t p;
												p.Insert("OnTextOverlay");
												p.Insert("You can't drop that here, find an emptier spot!");
												p.CreatePacket(peer);
												break;
											}
										}
										int dropped = c_; bool Growganoth = false;
										if (modify_inventory(peer, id_, dropCount) == 0) {
											if (id_ == 7188) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously dropped " + to_string(drop_.count) + " Blue Gem Lock in World: " + pInfo(peer)->world + ".");
											if (id_ == 1796 and drop_.count > 10) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously dropped " + to_string(drop_.count) + " Diamond Lock in World: " + pInfo(peer)->world + ".");
											if (id_ == 242 and drop_.count == 200) send_user_logs(pInfo(peer)->tankIDName, "Suspiciously dropped " + to_string(drop_.count) + " World Lock in World: " + pInfo(peer)->world + ".");
											Algorithm::log_text("DropLog", pInfo(peer)->tankIDName + " (#" + to_string(pInfo(peer)->uid) + ") dropped " + to_string(drop_.count) + " " + items[id_].ori_name + " in world " + pInfo(peer)->world);
											/*
											
											*/
											if (world_->name == "GROWGANOTH" && Halloween) {
												if (drop_.x / 32 >= 47 and drop_.x / 32 <= 52 and drop_.y / 32 == 15) {
													vector<int> Maw_Prize{};
													bool Guaranteed_Drops = false, Consumable = false;
													int Reward = 0, Give = 1, Give_Back = abs(dropped);
													if (items[id_].consume_prize != 0 && items[id_].consume_needed != 0) Growganoth = true, Consumable = true;
													if (items[id_].rarity != 0 && items[id_].rarity < 367) {
														if (items[id_].rarity >= 1) Maw_Prize.push_back(1208);
														if (items[id_].rarity >= 5) Maw_Prize.insert(Maw_Prize.end(), { 1992,5256,11348 });
														if (items[id_].rarity >= 10) Maw_Prize.insert(Maw_Prize.end(), { 1222,11342,1198,12786 });
														if (items[id_].rarity >= 15) Maw_Prize.insert(Maw_Prize.end(), { 1982,1200 });
														if (items[id_].rarity >= 20) Maw_Prize.insert(Maw_Prize.end(), { 4160,1220,10198 });
														if (items[id_].rarity >= 25) Maw_Prize.insert(Maw_Prize.end(), { 5240,1202 });
														if (items[id_].rarity >= 30) Maw_Prize.insert(Maw_Prize.end(), { 1238,5238,4162,10316,10196,10196 });
														if (items[id_].rarity >= 35) Maw_Prize.insert(Maw_Prize.end(), { 9044,1168 });
														if (items[id_].rarity >= 40) Maw_Prize.insert(Maw_Prize.end(), { 3116,4164,1172,12788 });
														if (items[id_].rarity >= 45) Maw_Prize.push_back(1230);
														if (items[id_].rarity >= 50) Maw_Prize.insert(Maw_Prize.end(), { 1192,11340,1194,10202 });
														if (items[id_].rarity >= 55) Maw_Prize.insert(Maw_Prize.end(), { 1226,7108 });
														if (items[id_].rarity >= 60) Maw_Prize.insert(Maw_Prize.end(), { 1196,12784 });
														if (items[id_].rarity >= 65) Maw_Prize.insert(Maw_Prize.end(), { 1236,9046 });
														if (items[id_].rarity >= 70) Maw_Prize.insert(Maw_Prize.end(), { 1980,1972,7110,1182,1994,1184,1186,1188,11330 });
														if (items[id_].rarity >= 73) Maw_Prize.insert(Maw_Prize.end(), { 10204,3102,4154,5232,5224,5222 });
														if (items[id_].rarity >= 75) Maw_Prize.push_back(1988);
														if (items[id_].rarity >= 80) Maw_Prize.insert(Maw_Prize.end(), { 4158,7128,1170,12790 });
														if (items[id_].rarity >= 85) Maw_Prize.push_back(1984);
														if (items[id_].rarity >= 90) Maw_Prize.insert(Maw_Prize.end(), { 1216,1218,5248,7114,10322,7112 });
														if (items[id_].rarity >= 97) Maw_Prize.push_back(3106);
														if (items[id_].rarity >= 100) Maw_Prize.push_back(7130);
														if (items[id_].rarity >= 101) Maw_Prize.push_back(3110);
														Reward = Maw_Prize[rand() % Maw_Prize.size()];
														Growganoth = true;
													}
													if (id_ == 10328) {
														gamepacket_t p;
														p.Insert("OnProgressUIUpdateValue"); p.Insert(pInfo(peer)->Darking_Sacrifice); p.Insert(0); p.CreatePacket(peer);
														Maw_Prize = { 12772, 12416, 12396, 12418, 12420, 12780, 10236  ,  4152  , 3114  , 1250  , 11314  , 3108  , 1956  , 11332  , 11336  , 11334  , 9042  ,9048  , 3126  , 10186  , 1990  , 1968  , 4166  , 5208  , 9056  , 7122  , 4172  , 4156  , 1234  , 5250  , 5254  , 5252  , 4138  , 4146  , 4140  , 4142  , 4148  , 4144  , 2000  , 1960  , 5210  , 1948  , 1214  , 7100  , 1966  , 1962  , 4188  , 4186  , 4190  , 10192  , 4136  , 10212  , 5236  , 5216  , 5218  , 5220  , 5214  , 4248  , 4182  , 4246  , 3098  , 5244  , 5256  , 10232  , 3118  , 10194  , 4192  , 1178  , 11316  , 1232  , 3124  , 11312  , 9040  , 10190  , 9034  , 3122  , 9050  , 9054  , 9052  , 7126  , 10200  , 10234  , 7124  , 11320  , 1210, 9018 };
														if (rand() % 1000 < 1 or pInfo(peer)->owner and pInfo(peer)->hand == 5176) Maw_Prize = { 5176, 1970, 12414, 9114, 11322, 7104, 10206, 11318, 10184, 3100, 7102 };
														Reward = Maw_Prize[rand() % Maw_Prize.size()];
														Growganoth = true;
														int Give_Again = Give_Back - 1;
														if (abs(dropped) > 1) modify_inventory(peer, id_, Give_Again);
														dropped = 1;
													}
													if (id_ == 1950) Growganoth = true, Guaranteed_Drops = true, Reward = 4152;
													if (id_ == 2722) Growganoth = true, Guaranteed_Drops = true, Reward = 3114;
													if (id_ == 910) Growganoth = true, Guaranteed_Drops = true, Reward = 1250;
													if (id_ == 11046) Growganoth = true, Guaranteed_Drops = true, Reward = 11314;
													if (id_ == 94 or id_ == 2636 or id_ == 604 or id_ == 1956) Growganoth = true, Guaranteed_Drops = true, Reward = 3108;
													if (id_ == 276 or id_ == 274) Growganoth = true, Guaranteed_Drops = true, Reward = 1956;
													if (id_ == 6300) Growganoth = true, Guaranteed_Drops = true, Reward = 7102;
													if (id_ == 10730) Growganoth = true, Guaranteed_Drops = true, Reward = 11332;
													if (id_ == 10734) Growganoth = true, Guaranteed_Drops = true, Reward = 11350;
													if (id_ == 10732) Growganoth = true, Guaranteed_Drops = true, Reward = 11334;
													if (id_ == 6176) Growganoth = true, Guaranteed_Drops = true, Reward = 9042;
													if (id_ == 3040) Growganoth = true, Guaranteed_Drops = true, Reward = 3100;
													if (id_ == 6144) Growganoth = true, Guaranteed_Drops = true, Reward = 7104;
													if (id_ == 7998) Growganoth = true, Guaranteed_Drops = true, Reward = 9048;
													if (id_ == 1162) Growganoth = true, Guaranteed_Drops = true, Reward = 3126;
													if (id_ == 9322) Growganoth = true, Guaranteed_Drops = true, Reward = 10184;
													if (id_ == 7696) Growganoth = true, Guaranteed_Drops = true, Reward = 10186;
													if (id_ == 1474) Growganoth = true, Guaranteed_Drops = true, Reward = 1990;
													if (id_ == 1506) Growganoth = true, Guaranteed_Drops = true, Reward = 1968;
													if (id_ == 2386) Growganoth = true, Guaranteed_Drops = true, Reward = 4166;
													if (id_ == 9364) Growganoth = true, Guaranteed_Drops = true, Reward = 10206;
													if (id_ == 10576) Growganoth = true, Guaranteed_Drops = true, Reward = 11322;
													if (id_ == 4960) Growganoth = true, Guaranteed_Drops = true, Reward = 5208;
													if (id_ == 6196) Growganoth = true, Guaranteed_Drops = true, Reward = 9056;
													if (id_ == 4326) Growganoth = true, Guaranteed_Drops = true, Reward = 7122;
													if (id_ == 2860 or id_ == 2268) Growganoth = true, Guaranteed_Drops = true, Reward = 4172;
													if (id_ == 1114) Growganoth = true, Guaranteed_Drops = true, Reward = 4156;
													if (id_ == 362) Growganoth = true, Guaranteed_Drops = true, Reward = 1234;
													if (id_ == 11574) Growganoth = true, Guaranteed_Drops = true, Reward = 12772;
													if (id_ == 4334) Growganoth = true, Guaranteed_Drops = true, Reward = 5250;
													if (id_ == 4338) Growganoth = true, Guaranteed_Drops = true, Reward = 5252;
													if (id_ == 1408) Growganoth = true, Guaranteed_Drops = true, Reward = 5254;
													if (id_ == 3288) Growganoth = true, Guaranteed_Drops = true, Reward = 4138;
													if (id_ == 3290) Growganoth = true, Guaranteed_Drops = true, Reward = 4140;
													if (id_ == 3292) Growganoth = true, Guaranteed_Drops = true, Reward = 4142;
													if (id_ == 3294) Growganoth = true, Guaranteed_Drops = true, Reward = 4144;
													if (id_ == 3296) Growganoth = true, Guaranteed_Drops = true, Reward = 4146;
													if (id_ == 3298) Growganoth = true, Guaranteed_Drops = true, Reward = 4148;
													if (id_ == 900 or id_ == 7752 or id_ == 7754 or id_ == 7136 or id_ == 1576 or id_ == 1378 or id_ == 7758 or id_ == 7760 or id_ == 7748) Growganoth = true, Guaranteed_Drops = true, Reward = 2000;
													if (id_ == 1746) Growganoth = true, Guaranteed_Drops = true, Reward = 1960;
													if (id_ == 5018) Growganoth = true, Guaranteed_Drops = true, Reward = 5210;
													if (id_ == 1252) Growganoth = true, Guaranteed_Drops = true, Reward = 1948;
													if (id_ == 1190) Growganoth = true, Guaranteed_Drops = true, Reward = 1214;
													if (id_ == 2868) Growganoth = true, Guaranteed_Drops = true, Reward = 7100;
													if (id_ == 11638) Growganoth = true, Guaranteed_Drops = true, Reward = 12780;
													if (id_ == 11534) Growganoth = true, Guaranteed_Drops = true, Reward = 12416;
													if (id_ == 1830) Growganoth = true, Guaranteed_Drops = true, Reward = 1966;
													if (items[id_].name.find("Chemical") != string::npos and id_ != 1962) Growganoth = true, Guaranteed_Drops = true, Reward = 1962;
													if (id_ == 3556) {
														Maw_Prize.insert(Maw_Prize.end(), { 4186, 4188 });
														Growganoth = true, Guaranteed_Drops = true, Reward = Maw_Prize[rand() % Maw_Prize.size()];
													}
													if (id_ == 762) Growganoth = true, Guaranteed_Drops = true, Reward = 4190;
													if (id_ == 3818) Growganoth = true, Guaranteed_Drops = true, Reward = 10192;
													if (id_ == 366) Growganoth = true, Guaranteed_Drops = true, Reward = 4136;
													if (id_ == 9262) Growganoth = true, Guaranteed_Drops = true, Reward = 10212;
													if (id_ == 5236) Growganoth = true, Guaranteed_Drops = true, Reward = 1294;
													if (id_ == 1246) Growganoth = true, Guaranteed_Drops = true, Reward = 5214;
													if (id_ == 1242) Growganoth = true, Guaranteed_Drops = true, Reward = 5216;
													if (id_ == 1244) Growganoth = true, Guaranteed_Drops = true, Reward = 5218;
													if (id_ == 1248) Growganoth = true, Guaranteed_Drops = true, Reward = 5220;
													if (id_ == 7026 or id_ == 7028 or id_ == 7030) Growganoth = true, Guaranteed_Drops = true, Reward = 12396;
													if (id_ == 3016 or id_ == 3018 or id_ == 5526 or id_ == 5528) Growganoth = true, Guaranteed_Drops = true, Reward = 4248;
													if (id_ == 3012 or id_ == 3014 or id_ == 2914) Growganoth = true, Guaranteed_Drops = true, Reward = 4246;
													if (id_ == 242) Growganoth = true, Guaranteed_Drops = true, Reward = 1212;
													if (id_ == 2972) Growganoth = true, Guaranteed_Drops = true, Reward = 4182;
													if (id_ == 1460) Growganoth = true, Guaranteed_Drops = true, Reward = 1970;
													if (id_ == 2392) Growganoth = true, Guaranteed_Drops = true, Reward = 9114;
													if (id_ == 3218 or id_ == 3020) Growganoth = true, Guaranteed_Drops = true, Reward = 3098;
													if (id_ == 3792) Growganoth = true, Guaranteed_Drops = true, Reward = 5244;
													if (id_ == 10634) Growganoth = true, Guaranteed_Drops = true, Reward = 11318;
													if (id_ == 1198) Growganoth = true, Guaranteed_Drops = true, Reward = 5256;
													if (id_ == 8468) Growganoth = true, Guaranteed_Drops = true, Reward = 10232;
													if (id_ == 2984) Growganoth = true, Guaranteed_Drops = true, Reward = 3118;
													if (id_ == 4360) Growganoth = true, Guaranteed_Drops = true, Reward = 10194;
													if (id_ == 10148) Growganoth = true, Guaranteed_Drops = true, Reward = 12418;
													if (items[id_].name.find("Sheet Music: ") != string::npos and id_ != 4192 or id_ == 412) Growganoth = true, Guaranteed_Drops = true, Reward = 4192;
													if (id_ == 5012 or id_ == 1018 or id_ == 592) Growganoth = true, Guaranteed_Drops = true, Reward = 1178;
													if (id_ == 10406) Growganoth = true, Guaranteed_Drops = true, Reward = 11316;
													if (id_ == 10652) Growganoth = true, Guaranteed_Drops = true, Reward = 12420;
													if (id_ == 882) Growganoth = true, Guaranteed_Drops = true, Reward = 1232;
													if (id_ == 1934) Growganoth = true, Guaranteed_Drops = true, Reward = 3124;
													if (id_ == 10626) Growganoth = true, Guaranteed_Drops = true, Reward = 11312;
													if (id_ == 6160) Growganoth = true, Guaranteed_Drops = true, Reward = 9040;
													if (id_ == 3794) Growganoth = true, Guaranteed_Drops = true, Reward = 10190;
													if (id_ == 8018) Growganoth = true, Guaranteed_Drops = true, Reward = 9034;
													if (id_ == 2390) Growganoth = true, Guaranteed_Drops = true, Reward = 3122;
													if (id_ == 9050) Growganoth = true, Guaranteed_Drops = true, Reward = 5246;
													if (id_ == 9052) Growganoth = true, Guaranteed_Drops = true, Reward = 5246;
													if (id_ == 9054) Growganoth = true, Guaranteed_Drops = true, Reward = 5246;
													if (id_ == 6798) Growganoth = true, Guaranteed_Drops = true, Reward = 7126;
													if (id_ == 9722 or id_ == 9724) Growganoth = true, Guaranteed_Drops = true, Reward = 10200;
													if (id_ == 9388) Growganoth = true, Guaranteed_Drops = true, Reward = 10234;
													if (id_ == 4732) Growganoth = true, Guaranteed_Drops = true, Reward = 7124;
													if (id_ == 10804) Growganoth = true, Guaranteed_Drops = true, Reward = 11320;
													if (id_ == 11666) Growganoth = true, Guaranteed_Drops = true, Reward = 12414;
													if (items[id_].name.find("Weather Machine - ") != string::npos and id_ != 932 or id_ == 8836 or id_ == 1210) Growganoth = true, Guaranteed_Drops = true, Reward = 1210;
													if (id_ != 0 and Reward == 0) Growganoth = true;
													if (Growganoth) {
														gamepacket_t a, b;
														a.Insert("OnTalkBubble"), a.Insert(pInfo(peer)->netID);
														b.Insert("OnConsoleMessage");
														if (Reward != 0) {
															gamepacket_t p, p2;
															if (Consumable) a.Insert("`4Growganoth turns your " + items[id_].ori_name + " into an " + items[items[id_].consume_prize].ori_name + " and hatches it with evil!``"), b.Insert("`4Growganoth turns your " + items[id_].ori_name + " into an " + items[items[id_].consume_prize].ori_name + " and hatches it with evil!``"), b.CreatePacket(peer);
															else a.Insert("`4Growganoth is pleased by your offering and grants a reward! Also, he eats you.``"), b.Insert("`4Growganoth is pleased by your offering and grants a reward! Also, he eats you.``"), b.CreatePacket(peer);
															p.Insert("OnConsoleMessage"), p.Insert("" + (string(abs(dropped) != 1 ? to_string(abs(dropped)) + " " : "")) + "`2" + items[id_].ori_name + "`` was devoured by Growganoth!"), p.CreatePacket(peer);
															p2.Insert("OnConsoleMessage");
															if (Guaranteed_Drops) {
																if (modify_inventory(peer, Reward, Give_Back) == 0) {
																	p2.Insert((string(abs(dropped) != 1 ? to_string(abs(dropped)) : "A")) + " `2" + items[Reward].ori_name + "`` was bestowed upon you!");
																	p2.CreatePacket(peer);
																}
																else {
																	modify_inventory(peer, id_, Give_Back);
																	p2.Insert("Nothing was upon to you! you didn't have enough inventory!");
																	p2.CreatePacket(peer);
																}
															}
															else {
																if (modify_inventory(peer, Reward, Give) == 0) {
																	p2.Insert("A `2" + items[Reward].ori_name + "`` was bestowed upon you!");
																	p2.CreatePacket(peer);
																}
																else {
																	if (id_ == 10328) Give_Back = 1;
																	modify_inventory(peer, id_, Give_Back);
																	p2.Insert("Nothing was upon to you! you didn't have enough inventory!");
																	p2.CreatePacket(peer);
																}
															}
															if (Guaranteed_Drops) {
																gamepacket_t a, b;
																a.Insert("OnTalkBubble"), a.Insert(pInfo(peer)->netID);
																b.Insert("OnConsoleMessage"); a.Insert("`4Growganoth rejects your puny offering and eats you instead!``"), b.Insert("`4Growganoth rejects your puny offering and eats you instead!``"), b.CreatePacket(peer);
																a.Insert(0), a.Insert(0), a.CreatePacket(peer);
															}
															if (Reward == 9488 || Reward == 1970 || Reward == 12414 || Reward == 9114 || Reward == 11322 || Reward == 7104 || Reward == 10206 || Reward == 11318 || Reward == 10184 || Reward == 3100 || Reward == 7102) {
																if (id_ == 10328) {
																	gamepacket_t p;
																	p.Insert("OnConsoleMessage"), p.Insert("`4Growganoth`` honors " + (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name) + "`` with a `5Rare " + items[Reward].name + "`` for their Dark King's Offering!");
																	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																		if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																		p.CreatePacket(currentPeer);
																	}
																	lastsbworld = pInfo(peer)->world;
																}
															}
														}
														else {
															a.Insert("`4Growganoth rejects your puny offering and eats you instead!``"), b.Insert("`4Growganoth rejects your puny offering and eats you instead!``"), b.CreatePacket(peer);
															modify_inventory(peer, id_, Give_Back);
														}
														a.Insert(0), a.Insert(0), a.CreatePacket(peer);
														SendRespawn(peer, true, 0, 1);
													}
												}
												else {
													int Give_Back = abs(dropped);
													gamepacket_t p;
													p.Insert("OnTextOverlay");
													p.Insert("Not close enough to the maw!");
													p.CreatePacket(peer);
													modify_inventory(peer, id_, Give_Back);
													break;
												}
											}

											add_cctv(peer, "dropped", to_string(abs(dropped)) + " " + items[id_].name);
											if (!Growganoth) dropas_(world_, drop_, pInfo(peer)->netID);
										}
									}
								}
							}
							else {
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									vector<string> t_ = explode("|", cch);
									if (t_.size() < 4) break;
									int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
									if (id_ <= 0 or id_ >= items.size()) break;
									if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 4758) != world_->active_jammers.end()) {
										if (world_->owner_name != (not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : pInfo(peer)->tankIDName) and not pInfo(peer)->admin and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end()) {
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("The Mini-Mod says no dropping items in this world!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);
											break;
										}
									}
									if (check_transmuted(peer, id_)) {
										gamepacket_t p;
										p.Insert("OnTalkBubble"),
											p.Insert(pInfo(peer)->netID),
											p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
											p.CreatePacket(peer);
										break;
									}
									if ((items[id_].untradeable and not pInfo(peer)->dev) or id_ == 32 or id_ == 18 or id_ == 1424) {
										gamepacket_t p;
										p.Insert("OnTextOverlay");
										p.Insert("You can't drop that.");
										p.CreatePacket(peer);
										break;
									}
									string name_ = pInfo(peer)->world;
									vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
									if (p != worlds.end()) {
										World* world_ = &worlds[p - worlds.begin()];
										int a_ = rand() % 12;
										int x = (pInfo(peer)->state == 16 ? pInfo(peer)->x - (a_ + 20) : (pInfo(peer)->x + 20) + a_);
										int y = pInfo(peer)->y + rand() % 16;
										//BlockTypes type_ = FOREGROUND;
										int where_ = (pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * world_->width);
										if (where_ < 0 || x < 0 || y < 0 || where_ >= ((world_->width * world_->height) - ((world_->width) * (6 * (world_->width / 100))))) continue;
										WorldBlock* block_ = &world_->blocks[where_];
										if (items[block_->fg].collisionType == 1 || block_->fg == 6 || items[block_->fg].entrance || items[block_->fg].toggleable and is_false_state(world_->blocks[(pInfo(peer)->state == 16 ? x / 32 : round((double)x / 32)) + (y / 32 * world_->width)], 0x00400000)) {
											gamepacket_t p;
											p.Insert("OnTextOverlay");
											p.Insert(items[block_->fg].blockType == BlockTypes::MAIN_DOOR ? "You can't drop items on the white door." : "You can't drop that here, face somewhere with open space.");
											p.CreatePacket(peer);
											break;
										}
										int count_ = 0;
										bool dublicated = false;
										for (int i_ = 0; i_ < world_->drop.size(); i_++) {
											if (abs(world_->drop[i_].y - y) <= 16 and abs(world_->drop[i_].x - x) <= 16) {
												count_ += 1;
											}
											if (world_->drop[i_].id == id_) if (world_->drop[i_].count < 200) dublicated = true;
										}
										if (!dublicated) {
											if (count_ > 20) {
												gamepacket_t p;
												p.Insert("OnTextOverlay");
												p.Insert("You can't drop that here, find an emptier spot!");
												p.CreatePacket(peer);
												break;
											}
										}
									}
									modify_inventory(peer, id_, c_);
									{
										gamepacket_t p;
										p.Insert("OnDialogRequest");
										p.Insert("set_default_color|`o\nadd_label_with_icon|big|`w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to drop?|left|\nadd_text_input|count||" + to_string(c_) + "|5|\nembed_data|itemID|" + to_string(id_) + "" + (world_->owner_name != pInfo(peer)->tankIDName and not pInfo(peer)->admin and (!guild_access(peer, world_->guild_id) and find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) == world_->admins.end()) ? "\nadd_textbox|If you are trying to trade an item with another player, use your wrench on them instead to use our Trade System! `4Dropping items is not safe!``|left|" : "") + "\nend_dialog|drop_item|Cancel|OK|");
										p.CreatePacket(peer);
									}
								}
							}
							break;
						}
						else if (cch.find("action|setRoleIcon") != string::npos || cch.find("action|setRoleSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							uint32_t role_t = atoi(id_.c_str());
							/*
							* 0 = farmer
							* 1 = builder
							* 2 = doctor
							* 3 = fisherman
							* 4 = chef
							* 5 = startopia
							* 6 = default
							*/
							if (cch.find("action|setRoleIcon") != string::npos) {
								if (role_t == 6) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 2 and pInfo(peer)->titleDoctor) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 3 and pInfo(peer)->titleFisher) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 4 and pInfo(peer)->titleChef) pInfo(peer)->roleIcon = role_t;
								else if (role_t == 5 and pInfo(peer)->titleStartopia) pInfo(peer)->roleIcon = role_t;
							}
							else {
								if (role_t == 6) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 0 and pInfo(peer)->t_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 1 and pInfo(peer)->bb_lvl >= 50) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 2 and pInfo(peer)->unlockRoleSkin) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 3 and pInfo(peer)->unlockRoleSkin) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 4 and pInfo(peer)->unlockRoleSkin) pInfo(peer)->roleSkin = role_t;
								else if (role_t == 5 and pInfo(peer)->unlockRoleSkin) pInfo(peer)->roleSkin = role_t;
							}
							gamepacket_t p(0, pInfo(peer)->netID);
							p.Insert("OnSetRoleSkinsAndIcons"), p.Insert(pInfo(peer)->roleSkin), p.Insert(pInfo(peer)->roleIcon), p.Insert(0);
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != pInfo(peer)->world) continue;
								p.CreatePacket(currentPeer);
							}
							break;
						}
						else if (cch.find("action|setSkin") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string id_ = explode("\n", t_[2])[0];
							if (not isdigit(id_[0])) break;
							char* endptr = NULL;
							unsigned int skin_ = strtoll(id_.c_str(), &endptr, 10);
							if (skin_ == 3531226367 and pInfo(peer)->supp == 2 or skin_ == 4023103999 and pInfo(peer)->supp == 2 or skin_ == 1345519520 and pInfo(peer)->supp == 2 or skin_ == 194314239 and pInfo(peer)->supp == 2) pInfo(peer)->skin = skin_;
							else if (skin_ == 3578898848 and pInfo(peer)->gp or skin_ == 3317842336 and pInfo(peer)->gp) pInfo(peer)->skin = skin_;
							else if (skin_ != 1348237567 and skin_ != 1685231359 and skin_ != 2022356223 and skin_ != 2190853119 and skin_ != 2527912447 and skin_ != 2864971775 and skin_ != 3033464831 and skin_ != 3370516479) {
								if (pInfo(peer)->supp >= 1) {
									if (skin_ != 2749215231 and skin_ != 3317842431 and skin_ != 726390783 and skin_ != 713703935 and skin_ != 3578898943 and skin_ != 4042322175) break;
									else pInfo(peer)->skin = skin_;
								}
								else break;
							}
							else pInfo(peer)->skin = skin_;
							update_clothes(peer);
							break;
						}
						else if (cch.find("action|trash") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int id_ = atoi(explode("\n", t_[3])[0].c_str()), c_ = 0;
							if (id_ <= 0 or id_ >= items.size()) break;
							gamepacket_t p;
							if (id_ == 18 || id_ == 32) {
								packet_(peer, "action|play_sfx\nfile|audio/cant_place_tile.wav\ndelayMS|0");
								p.Insert("OnTextOverlay"), p.Insert("You'd be sorry if you lost that!"), p.CreatePacket(peer);
								break;
							}
							if (check_transmuted(peer, id_)) {
								gamepacket_t p;
								p.Insert("OnTalkBubble"),
									p.Insert(pInfo(peer)->netID),
									p.Insert("`4This item has been transmuted. You need to keep at least one item in your inventory!"),
									p.CreatePacket(peer);
								break;
							}
							modify_inventory(peer, id_, c_); // gauna itemo kieki
							if (pInfo(peer)->Cheat_FastTrash) {
								int adaBrp = c_, hapusBrp = 0;
								hapusBrp = 0 - c_;
								if (items[id_].untradeable) {
									int has_ = 0;
									modify_inventory(peer, id_, has_);
									if (has_ < c_) break;
									gamepacket_t p;
									p.Insert("OnDialogRequest");
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Recycle`` " + to_string(adaBrp) + " `w" + items[id_].name + "``|left|" + to_string(id_) + "|\nembed_data|itemID|" + to_string(id_) + "\nembed_data|count|" + to_string(adaBrp) + "\nadd_textbox|You are recycling an `9UNTRADEABLE`` item. Are you absolutely sure you want to do this? There is no way to get the item back if you select yes.|left|\nend_dialog|trash_item2|NO!|Yes, I am sure|");
									p.CreatePacket(peer);
									break;
								}
								if (modify_inventory(peer, id_, hapusBrp) == 0) {
									packet_(peer, "action|play_sfx\nfile|audio/trash.wav\ndelayMS|0");
									gamepacket_t p;
									p.Insert("OnConsoleMessage");
									if (pInfo(peer)->supp != 0) {
										int item = id_, maxgems = 0, receivegems = 0;
										int count = adaBrp;
										if (id_ % 2 != 0) item -= 1;
										maxgems = items[item].max_gems2;
										if (items[item].max_gems2 != 0) if (maxgems != 0) for (int i = 0; i < count; i++) receivegems += rand() % maxgems;
										if (items[item].max_gems3 != 0) receivegems = count * items[item].max_gems3;
										if (receivegems != 0) {
											pInfo(peer)->gems += receivegems;
											gamepacket_t p;
											p.Insert("OnSetBux");
											p.Insert(pInfo(peer)->gems);
											p.Insert(0);
											p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
										p.Insert((items[id_].blockType == BlockTypes::FISH ? (to_string(abs(c_))) + "lb." : to_string(adaBrp)) + " `w" + items[id_].ori_name + "`` recycled, `0" + setGems(receivegems) + "`` gems earned.");
									}
									else p.Insert((items[id_].blockType == BlockTypes::FISH ? (to_string(abs(c_))) + "lb." : to_string(adaBrp)) + " `w" + items[id_].ori_name + "`` trashed.");
									p.CreatePacket(peer);
									break;
								}
								break;
							}
							else {
								p.Insert("OnDialogRequest");
								if (pInfo(peer)->supp == 0) p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Trash`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|How many to `4destroy``? (you have " + to_string(c_) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
								else {
									int item = id_, maxgems = 0, maximum_gems = 0;
									if (id_ % 2 != 0) item -= 1;
									maxgems = items[item].max_gems2;
									if (items[item].max_gems3 != 0) maximum_gems = items[item].max_gems3;
									string recycle_text = "0" + (maxgems == 0 ? "" : "-" + to_string(maxgems)) + "";
									if (maximum_gems != 0) recycle_text = to_string(maximum_gems);
									p.Insert("set_default_color|`o\nadd_label_with_icon|big|`4Recycle`` `w" + items[id_].ori_name + "``|left|" + to_string(id_) + "|\nadd_textbox|You will get " + recycle_text + " gems per item.|\nadd_textbox|How many to `4destroy``? (you have " + to_string(c_) + ")|left|\nadd_text_input|count||0|5|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|trash_item|Cancel|OK|");
								}
								p.CreatePacket(peer);
								break;
							}
							break;
						}
						else if (cch.find("action|info") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							gamepacket_t p;
							p.Insert("OnDialogRequest");
							int id_ = atoi(explode("\n", t_[3])[0].c_str());
                            if (id_ >= items.size()) break;
							if (id_ % 2 != 0) p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].ori_name + "``|left|" + to_string(id_) + "|" + to_string(items[id_ - 1].chi) + "|\nadd_spacer|small|\nadd_textbox|Plant this seed to grow a `0" + items[id_ - 1].ori_name + " Tree.``|left|\nadd_spacer|small|\nadd_textbox|Rarity: `0" + to_string(items[id_].rarity) + "``|left|\nadd_spacer|small|\nend_dialog|continue||OK|\n");
							else {
								string extra_ = "\nadd_textbox|";
								if (id_ == 18) {
									extra_ += "You've punched `w" + to_string(pInfo(peer)->punch_count) + "`` times.";
								} if (items[id_].blockType == BlockTypes::LOCK) {
									extra_ += "A lock makes it so only you (and designated friends) can edit an area.";
								} if (items[id_].r_1 == 0 or items[id_].r_2 == 0) {
									extra_ += "<CR>This item can't be spliced.";
								}
								else {
									extra_ += "Rarity: `w" + to_string(items[id_].rarity) + "``<CR><CR>To grow, plant a `w" + items[id_ + 1].name + "``.   (Or splice a `w" + items[items[id_].r_1].name + "`` with a `w" + items[items[id_].r_2].name + "``)<CR>";
								} if (items[id_].properties & Property_Dropless or items[id_].rarity == 999) {
									extra_ += "<CR>`1This item never drops any seeds.``";
								} if (items[id_].properties & Property_Untradable) {
									extra_ += "<CR>`1This item cannot be dropped or traded.``";
								} if (items[id_].properties & Property_AutoPickup) {
									extra_ += "<CR>`1This item can't be destroyed - smashing it will return it to your backpack if you have room!``";
								} if (items[id_].properties & Property_Wrenchable) {
									extra_ += "<CR>`1This item has special properties you can adjust with the Wrench.``";
								} if (items[id_].properties & Property_MultiFacing) {
									extra_ += "<CR>`1This item can be placed in two directions, depending on the direction you're facing.``";
								} if (items[id_].properties & Property_NoSelf) {
									extra_ += "<CR>`1This item has no use... by itself.``";
								}
								extra_ += "|left|";
								if (extra_ == "\nadd_textbox||left|") extra_ = "";
								else extra_ = replace_str(extra_, "add_textbox|<CR>", "add_textbox|");

								string extra_ore = "";
								if (id_ == 9386) extra_ore = rare_text;
								if (id_ == 9384) extra_ore = rare2_text;
								if (id_ == 9148) extra_ore = rainbow_text;
								p.Insert("set_default_color|`o\nadd_label_with_ele_icon|big|`wAbout " + items[id_].name + "``|left|" + to_string(id_) + "|" + to_string(items[id_].chi) + "|\nadd_spacer|small|\nadd_textbox|" + items[id_].description + "|left|" + (extra_ore != "" ? "\nadd_spacer|small|\nadd_textbox|This item also drops:|left|" + extra_ore : "") + "" + (id_ == 8552 ? "\nadd_spacer|small|\nadd_textbox|Angelic Healings: " + to_string(pInfo(peer)->surgery_done) + "|left|" : "") + "\nadd_spacer|small|" + extra_ + "\nadd_spacer|small|\nembed_data|itemID|" + to_string(id_) + "\nend_dialog|continue||OK|\n");
							}
							p.CreatePacket(peer);
							break;
						}
						else if (cch.find("action|wrench") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 4) break;
							int netID = atoi(explode("\n", t_[3])[0].c_str());
							if (pInfo(peer)->netID == netID) {
								send_wrench_self(peer);
							}
							else {
								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (pInfo(currentPeer)->netID == netID and pInfo(currentPeer)->world == pInfo(peer)->world) {
										if (pInfo(peer)->Cheat_FastPull) {
											if (pInfo(peer)->mod or pInfo(peer)->admin) SendCmd(peer, "/summon " + pInfo(currentPeer)->tankIDName);
											else SendCmd(peer, "/pull " + pInfo(currentPeer)->tankIDName);
											break;
										}
										bool already_friends = false, trade_blocked = false, muted = false;
										for (int c_ = 0; c_ < pInfo(peer)->friends.size(); c_++) {
											if (pInfo(peer)->friends[c_].uid == pInfo(currentPeer)->uid) {
												already_friends = true;
												if (pInfo(peer)->friends[c_].block_trade)
													trade_blocked = true;
												if (pInfo(peer)->friends[c_].mute)
													muted = true;
												break;
											}
										}
										pInfo(peer)->last_wrenched = pInfo(currentPeer)->tankIDName;
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											string msg2 = "";
											for (int i = 0; i < to_string(pInfo(currentPeer)->level).length(); i++) msg2 += "?";
											string inv_guild = "", inv_aff = "";
											string extra = "";
											int with = 0;
											int name = pInfo(currentPeer)->uid;
											AffinityType my_type = NO_AFFINITY;
											int my_aff_lvl = 0;
											load_affinity(name, my_type, my_aff_lvl, with);
											pInfo(currentPeer)->affinity_type = my_type;
											pInfo(currentPeer)->affinity_level = my_aff_lvl;
											pInfo(currentPeer)->affinity_with = with;
											if (pInfo(currentPeer)->affinity_level <= 0 || pInfo(currentPeer)->affinity_with <= 0) pInfo(peer)->affinity_type = NO_AFFINITY;
											if (pInfo(currentPeer)->affinity_type != NO_AFFINITY and pInfo(currentPeer)->affinity_with != 0) {
												extra += "\nadd_textbox|Affinity: " + GetNameByUid(pInfo(currentPeer)->affinity_with) + "'s " + affinity_to_string(pInfo(currentPeer)->affinity_type) + "|left|";
												extra += "\nadd_textbox|Affinity level: " + to_string(pInfo(currentPeer)->affinity_level) + "|left|\nadd_spacer|small|";
											}
											if (pInfo(peer)->affinity_type == NO_AFFINITY and pInfo(currentPeer)->affinity_type == NO_AFFINITY) {
												inv_aff += "\nadd_button|aff_lover|`2Request for lover affinity``|noflags|0|0|";
												inv_aff += "\nadd_button|aff_bestie|`2Request for bestie affinity``|noflags|0|0|";
											}
											if (pInfo(currentPeer)->guild_id != 0) {
												uint32_t guild_id = pInfo(currentPeer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (member_search.member_name == pInfo(currentPeer)->tankIDName) {
															if (guild_information->guild_mascot[1] == 0 and guild_information->guild_mascot[0] == 0) {
																extra += "\nadd_label_with_icon|small|`9Guild: `2" + guild_information->guild_name + "``|left|5814|\nadd_textbox|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															else {
																extra += "\nadd_dual_layer_icon_label|small|`9Guild: `2" + guild_information->guild_name + "``|left|" + to_string(guild_information->guild_mascot[1]) + "|" + to_string(guild_information->guild_mascot[0]) + "|1.0|1|\nadd_smalltext|`9Rank: `2" + (member_search.role_id == 0 ? "Member" : (member_search.role_id == 1 ? "Elder" : (member_search.role_id == 2 ? "Co-Leader" : "Leader"))) + "``|left|\nadd_spacer|small|";
															}
															break;
														}
													}
												}
											} if (pInfo(peer)->guild_id != 0 and pInfo(currentPeer)->guild_id == 0) {
												uint32_t guild_id = pInfo(peer)->guild_id;
												vector<Guild>::iterator find_guild = find_if(guilds.begin(), guilds.end(), [guild_id](const Guild& a) { return a.guild_id == guild_id; });
												if (find_guild != guilds.end()) {
													Guild* guild_information = &guilds[find_guild - guilds.begin()];
													for (GuildMember member_search : guild_information->guild_members) {
														if (member_search.member_name == pInfo(peer)->tankIDName) {
															if (member_search.role_id >= 1) {
																inv_guild = "\nadd_button|invitetoguild|`2Invite to Guild``|noflags|0|0|";
															}
															break;
														}
													}
												}
											}
											time_t s__;
											s__ = time(NULL);
											int days_ = int(s__) / (60 * 60 * 24);
											double hours_ = (double)((s__ - pInfo(currentPeer)->playtime) + pInfo(currentPeer)->seconds) / 3600;
											string num_text = to_string(hours_), status = "";
											string rounded = num_text.substr(0, num_text.find(".") + 3);
											string surgery = "\nadd_spacer|small|\nadd_button|start_surg|`$Perform Surgery``|noflags|0|0|\nadd_smalltext|Surgeon Skill: " + to_string(pInfo(peer)->surgery_skill) + "|left|";
											string PVP = "\nadd_spacer|small|\n" + string(pInfo(peer)->PVP_Ongoing ? "add_smalltext|`4You're currently on battle!``|left|" : (pInfo(currentPeer)->PVP_Ongoing ? "add_smalltext|`4This player currently on battle!``|left|" : (pInfo(peer)->PVP_netID == pInfo(currentPeer)->netID ? "add_smalltext|`4You're already request a battle with him!``|left|\nadd_button|cancel_pvp|`4Cancel request!``|noflags|0|0|" : "add_button|battle_pvp|`4Ask for PVP``|noflags|0|0|")));
											for (int i_ = 0; i_ < pInfo(currentPeer)->playmods.size(); i_++) if (pInfo(currentPeer)->playmods[i_].id == 89) surgery = "\nadd_spacer|small|\nadd_textbox|Recovering from surgery...|left|";
											if (pInfo(currentPeer)->hospital_bed == false) surgery = "";
											gamepacket_t p;
											p.Insert("OnDialogRequest");
											p.Insert("embed_data|netID|" + to_string(pInfo(peer)->netID) + "\nset_default_color|`o\nadd_label_with_icon|big|" + (pInfo(currentPeer)->mod && pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->name_color : "`0") + "" + (pInfo(currentPeer)->d_name.empty() ? pInfo(currentPeer)->tankIDName : pInfo(currentPeer)->d_name) + "`` `0(```2" + (pInfo(currentPeer)->admin && pInfo(currentPeer)->d_name.empty() ? msg2 : to_string(pInfo(currentPeer)->level)) + "```0)``" + (pInfo(peer)->show_uid ? "(`4" + to_string(pInfo(currentPeer)->uid) + "``)" : "") + "|left|18|" + (pInfo(currentPeer)->trader ? "\nadd_smalltext|This user is verified global trader|" : "") + PVP + surgery + "\nembed_data|netID|" + to_string(netID) + "\nadd_spacer|small|" + extra + (pInfo(currentPeer)->Show_OwnedWorlds ? "\nadd_label|small|`1Account Playtime:`` " + rounded + " hours|left" : "") + (pInfo(currentPeer)->Show_AccountCreated ? "\nadd_spacer|small|\nadd_label|small|`1Account Age:`` " + to_string(days_ - pInfo(currentPeer)->account_created) + " days|left" : "") + (pInfo(currentPeer)->Show_HomeWorld ? "\nadd_spacer|small|\nadd_label|small|`1Home World:``|left\nadd_button|visit_home_world_" + pInfo(currentPeer)->home_world + "|`$Visit " + pInfo(currentPeer)->home_world + "``|noflags|0|0|" : "") + "\nadd_spacer|small|" + (trade_blocked ? "\nadd_button||`4Trade Blocked``|disabled|||" : "\nadd_button|trade|`wTrade``|noflags|0|0|") + "\nadd_textbox|(No Battle Leash equipped)|left|\nadd_textbox|Your opponent needs a valid license to battle!|left|" + (world_->owner_name == pInfo(peer)->tankIDName or (guild_access(peer, world_->guild_id) or find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) != world_->admins.end()) or pInfo(peer)->admin + pInfo(peer)->mod > 0 ? "\nadd_button|kick|`4Kick``|noflags|0|0|\nadd_button|pull|`5Pull``|noflags|0|0|\nadd_button|worldban|`4World Ban``|noflags|0|0|" : "") + (pInfo(peer)->mod == 1 ? "\nadd_button|punish_view|`5Punish/View``|noflags|0|0|\nadd_button|thaw_player|`cThaw" : already_friends ? "\nadd_button|freeze_player|`cFreeze|noflags|0|0|" : "") + inv_guild + inv_aff + (!already_friends ? "\nadd_button|friend_add|`wAdd as friend``|noflags|0|0|" : "") + (muted ? "\nadd_button|unmute_player|`wUnmute``|noflags|0|0|" : (already_friends ? "\nadd_button|mute_player|`wMute``|noflags|0|0|" : "")) + ""/*"\nadd_button|ignore_player|`wIgnore Player``|noflags|0|0|\nadd_button|report_player|`wReport Player``|noflags|0|0|"*/"\nadd_spacer|small|\nend_dialog|popup||Continue|\nadd_quick_exit|");
											p.CreatePacket(peer);
										}
										break;
									}
								}
							}
							break;
						}
						else if (cch.find("action|friends") != string::npos) {
							send_social(peer);
							break;
						}
						else if (cch.find("action|buy") != string::npos) {
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string item = explode("\n", t_[2])[0];
							int price = 0, free = get_free_slots(pInfo(peer)), slot = 1, getcount = 0, get_counted = 0, random_pack = 0, token = 0;
							gamepacket_t p2;
							p2.Insert("OnStorePurchaseResult");
							if (item == "main") shop_tab(peer, "tab1");
							else if (item == "locks") shop_tab(peer, "tab2");
							else if (item == "itempack") shop_tab(peer, "tab3");
							else if (item == "bigitems") shop_tab(peer, "tab4");
							else if (item == "weather") shop_tab(peer, "tab5");
							else if (item == "token") shop_tab(peer, "tab6");
							else if (item == "demonic_blade") {
								price = 1000000;
								if (price > pInfo(peer)->gems) {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("You can't afford `0Demonic Blade``!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short.");
									p2.CreatePacket(peer);
								}
								else {
									vector<int> item_{ 5172, 5174 };
									int rand_item = item_[rand() % item_.size()], c_ = 1;
									if (modify_inventory(peer, rand_item, c_) == 0) {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You've purchased `0Demonic Blade`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.");
											p.CreatePacket(peer);
										}
										p2.Insert("You've purchased `0Demonic Blade`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.\n\n`5Received: ```0" + items[rand_item].name + "``\n");
										pInfo(peer)->gems -= price;
										{
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											gamepacket_t p;
											p.Insert("OnSetBux");
											p.Insert(pInfo(peer)->gems);
											p.Insert(0);
											p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											//if (item == "arm_guy") grow4good(peer, false, "purchase_waving", 0);
											string item_name = items[rand_item].name;
											gamepacket_t p_;
											p_.Insert("OnConsoleMessage"), p_.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n" + item_name), p_.CreatePacket(peer);
											p2.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n\n`5Received: ``" + item_name + "\n"), p2.CreatePacket(peer);
											break;
										}
									}
									else {
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You don't have enough space in your inventory to buy that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										p2.CreatePacket(peer);
										break;
									}
								}
							}
							else if (item == "beach") {
								price = 20000;
								if (price > pInfo(peer)->gems) {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("You can't afford `0Beach Blast + 100 Fireworks Pack``!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short.");
									p2.CreatePacket(peer);
								}
								else {
									//vector<int> item_{ 830, 834 };
									int c_ = 1, c_1 = 100;
									if (get_item_count(pInfo(peer), 830) < 200 && get_item_count(pInfo(peer), 834) < 200) {
										modify_inventory(peer, 830, c_);
										modify_inventory(peer, 834, c_1);
										{
											{
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You've purchased `0Beach Blast + 100 Fireworks Pack`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.");
												p.CreatePacket(peer);
											}
											p2.Insert("You've purchased `0Beach Blast + 100 Fireworks Pack`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.\n\n`5Received: ```0Beach Blast + 100 Fireworks Pack``\n");
											pInfo(peer)->gems -= price;
											{
												packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
												gamepacket_t p;
												p.Insert("OnSetBux");
												p.Insert(pInfo(peer)->gems);
												p.Insert(0);
												p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
												if (pInfo(peer)->supp >= 2) {
													p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
												}
												p.CreatePacket(peer);
												packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
												//if (item == "arm_guy") grow4good(peer, false, "purchase_waving", 0);
												string item_name = "Beach Blast + 100 Fireworks Pack";
												gamepacket_t p_;
												p_.Insert("OnConsoleMessage"), p_.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n" + item_name), p_.CreatePacket(peer);
												p2.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n\n`5Received: ``" + item_name + "\n"), p2.CreatePacket(peer);
												break;
											}
										}
									}
									else {
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You don't have enough space in your inventory to buy that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										p2.CreatePacket(peer);
										break;
									}
								}
							}
							else if (item == "upgrade_backpack") {
								price = 100 * ((((pInfo(peer)->inv.size() - 17) / 10) * ((pInfo(peer)->inv.size() - 17) / 10)) + 1);
								if (price > pInfo(peer)->gems) {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("You can't afford `0Upgrade Backpack`` (`w10 Slots``)!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short.");
								}
								else {
									if (pInfo(peer)->inv.size() < INV_MAX) {
										{
											gamepacket_t p;
											p.Insert("OnConsoleMessage");
											p.Insert("You've purchased `0Upgrade Backpack`` (`010 Slots``) for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.");
											p.CreatePacket(peer);
										}
										p2.Insert("You've purchased `0Upgrade Backpack (10 Slots)`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems - price) + "`` Gems left.\n\n`5Received: ```0Backpack Upgrade``\n");
										pInfo(peer)->gems -= price;
										{
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											gamepacket_t p;
											p.Insert("OnSetBux");
											p.Insert(pInfo(peer)->gems);
											p.Insert(0);
											p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											p.CreatePacket(peer);
										}
										for (int i_ = 0; i_ < 10; i_++) { // default inv dydis
											Items itm_{};
											itm_.id = 0, itm_.count = 0;
											pInfo(peer)->inv.push_back(itm_);
										}
										send_inventory(peer);
										update_clothes(peer);
										shop_tab(peer, "tab2");
									}
								}
								p2.CreatePacket(peer);
							}
							else {
								vector<int> list;
								int mtoken = 0;
								vector<vector<int>> itemai;
								string item_name = "";
								
								ifstream ifs("database/shop/-" + item + ".json");
								if (ifs.is_open()) {
									json j;
									ifs >> j;
									price = j["g"].get<int>();
									item_name = j["p"].get<string>();
									if (j.find("itemai") != j.end()) { // mano sistema
										if (pInfo(peer)->gems < price) {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert("You can't afford `o" + item_name + "``!  You're `$" + setGems(price - pInfo(peer)->gems) + "`` Gems short."), p2.CreatePacket(peer);
											break;
										}
										itemai = j["itemai"].get<vector<vector<int>>>();
										int reik_slots = itemai.size();
										int turi_slots = get_free_slots(pInfo(peer));
										for (vector<int> item_info : itemai) {
											int turi_dabar = 0;
											modify_inventory(peer, item_info[0], turi_dabar);
											if (turi_dabar != 0) reik_slots--;
											if (turi_dabar + item_info[1] > 200) goto fail;
										}
										if (turi_slots < reik_slots) goto fail;
										{
											//if (item == "g4good_Gem_Charity") grow4good(peer, false, "donate_gems", 0);
											//if (item != "arm_guy" and item != "g4good_Gem_Charity") grow4good(peer, false, "gems", price);
											pInfo(peer)->gems -= price;
											gamepacket_t gem_upd;
											gem_upd.Insert("OnSetBux"), gem_upd.Insert(pInfo(peer)->gems), gem_upd.Insert(0), gem_upd.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
											if (pInfo(peer)->supp >= 2) {
												gem_upd.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
											}
											gem_upd.CreatePacket(peer);
											vector<string> received_items{}, received_items2{};
											for (vector<int> item_info : itemai) {
												uint32_t item_id = item_info[0];
												int item_count = item_info[1];
												modify_inventory(peer, item_id, item_count);
												received_items.push_back("Got " + to_string(item_info[1]) + " `#" + items[item_id].ori_name + "``."), received_items2.push_back(to_string(item_info[1]) + " " + items[item_id].ori_name);
											}
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
											//if (item == "arm_guy") grow4good(peer, false, "purchase_waving", 0);
											gamepacket_t p_;
											p_.Insert("OnConsoleMessage"), p_.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n" + join(received_items, "\n")), p_.CreatePacket(peer);
											p2.Insert("You've purchased `o" + item_name + "`` for `$" + setGems(price) + "`` Gems.\nYou have `$" + setGems(pInfo(peer)->gems) + "`` Gems left." + "\n\n`5Received: ``" + join(received_items2, ", ") + "\n"), p2.CreatePacket(peer);
											break;
										}
									fail:
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You don't have enough space in your inventory to buy that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										p2.CreatePacket(peer);
										break;
									}
									list = j["i"].get<vector<int>>();
									getcount = j["h"].get<int>();
									get_counted = j["h"].get<int>();
									slot = j["c"].get<int>();
									token = j["t"].get<int>();
									random_pack = j["random"].get<int>();
									if (j.find("mt") != j.end()) mtoken = j["mt"].get<int>();
									int totaltoken = 0, tokencount = 0, mega_token = 0, inventoryfull = 0;
									modify_inventory(peer, 1486, tokencount);
									modify_inventory(peer, 6802, mega_token);
									totaltoken = tokencount + (mega_token * 100);
									vector<pair<int, int>> receivingitems;
									if (token == 0 and mtoken == 0 ? price > pInfo(peer)->gems : mtoken != 0 ? mtoken > pInfo(peer)->gtwl : token > totaltoken) {
										packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
										p2.Insert("You can't afford `o" + item_name + "``!"
											"  You're `$" + (token == 0 and mtoken == 0 ? "" + setGems(price - pInfo(peer)->gems) + "`` Gems short." : mtoken != 0 ? "" + setGems(mtoken - pInfo(peer)->gtwl) + "`` TayoPoint short." : "" + setGems(token - totaltoken) + "`` TayoPoint short."));
									}
									else {
										if (free >= slot) {
											string received = "", received2 = "";
											if (item == "basic_splice") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											if (item == "race_packa") {
												slot++;
												receivingitems.push_back(make_pair(11, 10));
											}
											for (int i = 0; i < slot; i++) receivingitems.push_back(make_pair((random_pack == 1 ? list[rand() % list.size()] : list[i]), getcount));
											for (int i = 0; i < slot; i++) {
												int itemcount = 0;
												modify_inventory(peer, receivingitems[i].first, itemcount);
												if (itemcount + getcount > 200) inventoryfull = 1;
											}
											if (inventoryfull == 0) {
												int i = 0;
												for (i = 0; i < slot; i++) {
													received += (i != 0 ? ", " : "") + items[receivingitems[i].first].name;
													received2 += "Got " + to_string(receivingitems[i].second) + " `#" + items[receivingitems[i].first].name + "``." + (i == (slot - 1) ? "" : "\n") + "";
													modify_inventory(peer, receivingitems[i].first, receivingitems[i].second);
												}
											}
											else {
												packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
												p2.Insert("You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
												p2.CreatePacket(peer);
												break;
											}
											{
												gamepacket_t p;
												p.Insert("OnConsoleMessage");
												p.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 and mtoken == 0 ? "" + setGems(price) + "`` Gems." : mtoken != 0 ? "" + setGems(mtoken) + "`` TayoPoint." : "" + setGems(token) + "`` TayoPoint.") + "\nYou have `$" + (token == 0 and mtoken == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : mtoken != 0 ? "" + setGems(pInfo(peer)->gtwl - mtoken) + "`` TayoPoint left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n" + received2);
												p.CreatePacket(peer);
											}
											p2.Insert("You've purchased `o" + received + "`` for `$" + (token == 0 and mtoken == 0 ? "" + setGems(price) + "`` Gems." : mtoken != 0 ? "" + setGems(mtoken) + "`` TayoPoint." : "" + setGems(token) + "`` TayoPoint.") + "\nYou have `$" + (token == 0 and mtoken == 0 ? "" + setGems(pInfo(peer)->gems - price) + "`` Gems left." : mtoken != 0 ? "" + setGems(pInfo(peer)->gtwl - mtoken) + "`` TayoPoint left." : "" + setGems(totaltoken - token) + "`` Growtokens left.") + "\n\n`5Received: ``" + (get_counted <= 1 ? "" : "`0" + to_string(get_counted)) + "`` " + received + "\n"), p2.CreatePacket(peer);
											if (token == 0 and mtoken == 0) {
												pInfo(peer)->gems -= price;
												gamepacket_t p;
												p.Insert("OnSetBux");
												p.Insert(pInfo(peer)->gems);
												p.Insert(0);
												p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
												if (pInfo(peer)->supp >= 2) {
													p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
												}
												p.CreatePacket(peer);
											}
											else if (mtoken != 0) {
												if (pInfo(peer)->gtwl >= mtoken) pInfo(peer)->gtwl -= mtoken;
												save_player(pInfo(peer), false);
											}
											else {
												if (tokencount >= token) modify_inventory(peer, 1486, token *= -1);
												else {
													modify_inventory(peer, 1486, tokencount *= -1);
													modify_inventory(peer, 6802, mega_token *= -1);
													int givemegatoken = (totaltoken - token) / 100;
													int givetoken = (totaltoken - token) - (givemegatoken * 100);
													modify_inventory(peer, 1486, givetoken);
													modify_inventory(peer, 6802, givemegatoken);
												}
											}
											packet_(peer, "action|play_sfx\nfile|audio/piano_nice.wav\ndelayMS|0");
										}
										else {
											packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
											p2.Insert(slot > 1 ? "You'll need " + to_string(slot) + " slots free to buy that! You have " + to_string(free) + " slots." : "You don't have enough space in your inventory that. You may be carrying to many of one of the items you are trying to purchase or you don't have enough free spaces to fit them all in your backpack!");
										}
									}
								}
								else {
									packet_(peer, "action|play_sfx\nfile|audio/bleep_fail.wav\ndelayMS|0");
									p2.Insert("This item was not found. Server error.");
									p2.CreatePacket(peer);
								}
							}
							break;
						}
						else if (cch.find("action|respawn") != string::npos) SendRespawn(peer, false, 0, (cch.find("action|respawn_spike") != string::npos) ? false : true);
					}
					else if (cch.find("action|dialog_return") != string::npos) {
						call_dialog(peer, cch);
						break;
					}
					break;
				}
				case 3:
				{
					string str;
					if (pInfo(peer)->isIn == false) break;
					if (pInfo(peer)->trading_with != -1) {
						cancel_trade(peer, false);
						break;
					}
					if (Algorithm::get_str_from_packet(event.packet, str))
					{
						/*
						if (parse(str) || scr.get().size() <= 0) {
							string log = format("`4Warning: `w{}`` `9has been banned for sending invalid packets.````", pData::Algorithm::get_display_name(peer));
							send_logs(log);
							Algorithm::send_console(peer, "Your client sent invalid packets to the server.");
							add_ban(peer, 2.592e+6, "Usage of third party programs", "System");
						}
						*/
						if (pInfo(peer)->isIn) {
							if (pInfo(peer)->lpps + 750 < (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count()) {
								pInfo(peer)->pps = 0;
								pInfo(peer)->lpps = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
							}
							else {
								pInfo(peer)->pps++;
								if (pInfo(peer)->pps >= 25) {
									string log = format("`4Warning: `w{}`` `9tried to spam packets. peer is now disconnected!````", pData::Algorithm::get_display_name(peer));
									send_logs(log);
									enet_peer_disconnect_later(event.peer, 0U);
									save_player(pInfo(peer), false);
									break;
								}
							}
						}
					}
					string cch = text_(event.packet);
					if (cch.find("action|validate_world") != string::npos && pInfo(peer)->world != "") {
						if (pInfo(peer)->lpps + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->pps = 0;
							pInfo(peer)->lpps = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
						}
						else {
							pInfo(peer)->pps++;
							if (pInfo(peer)->pps >= 15) {
								OnCatchWrongAction(peer, 3, cch);
								break;
							}
						}
					}
					if (pInfo(peer)->lpps2 + 100 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
						pInfo(peer)->pps2 = 0;
						pInfo(peer)->lpps2 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
					}
					else {
						pInfo(peer)->pps2++;
						if (pInfo(peer)->pps2 >= 15) {
							send_server_logs(pInfo(peer)->tankIDName, "Over Packet Limit in " + pInfo(peer)->world + " (FLAGGED AS: OVER_PACKET_LIMIT Code(3))");
							tayo_warn("Over packet 3 limit from " + pInfo(peer)->tankIDName + " in world " + pInfo(peer)->world + " packet was " + cch);
							packet_(peer, "action|log\nmsg|`7Your client sending too many packets. attempt to reconnect");
							if (not pInfo(peer)->tankIDName.empty()) {
								save_player(pInfo(peer), false);
							}
							if (pInfo(peer)->trading_with != -1) {
								cancel_trade(peer, false);
							}
							if (pInfo(peer)->world != "") exit_(peer, true);
							enet_peer_disconnect_later(peer, 0);
							save_player(pInfo(peer), false);
							break;
						}
					}
					if (cch == "action|quit") {
						if (not pInfo(peer)->tankIDName.empty()) {
							save_player(pInfo(peer), false);
						}
						if (pInfo(peer)->trading_with != -1) {
							cancel_trade(peer, false);
						}
						enet_peer_disconnect_later(peer, 0);
						save_player(pInfo(peer), false);
						delete peer->data;
						peer->data = NULL;
					}
					else if (cch == "action|quit_to_exit") {
						exit_(peer);
					}
					else if (cch.find("action|join_request") != string::npos) {
						if (pInfo(peer)->last_world_enter + 500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->last_world_enter = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							vector<string> t_ = explode("|", cch);
							if (t_.size() < 3) break;
							string world_name = explode("\n", t_[2])[0];
							transform(world_name.begin(), world_name.end(), world_name.begin(), ::toupper);
							join_world(peer, world_name);
						}
					}
					else if (cch == "action|gohomeworld\n" && pInfo(peer)->world == "") {
						packet_(peer, "action|log\nmsg|Disabled temporarily.");
						gamepacket_t p;
						p.Insert("OnFailedToEnterWorld"), p.CreatePacket(peer);
						break;
						/*
						if (pInfo(peer)->home_world == "") {
							gamepacket_t p, p2;
							p.Insert("OnDialogRequest");
							p.Insert("set_default_color|`o\nadd_label_with_icon|big|`9No Home World Set ``|left|1432|\nadd_spacer|small|\nadd_textbox|Use /sethome to assign the current world as your home world.|left|\nadd_spacer|small|\nend_dialog||OK||");
							p.CreatePacket(peer);
							p2.Insert("OnFailedToEnterWorld"), p2.CreatePacket(peer);
						}
						else {
							packet_(peer, "action|log\nmsg|Magically warping to home world `5" + pInfo(peer)->home_world + "``...");
							string world_name = pInfo(peer)->home_world;
							join_world(peer, world_name);
						}
						*/
					}
					break;
				}
				case 4:
				{
					if (event.packet->dataLength < 56) {
						string log = format("`4Warning: `w{}`` `9has been banned for client-manipulating packets.````", pData::Algorithm::get_display_name(peer));
						send_logs(log);
						add_ban(peer, 2.592e+6, "Usage of third party programs", "System");
						break;
					}
					if (pInfo(peer)->isIn) {
						if (pInfo(peer)->lpps + 750 < (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->pps = 0;
							pInfo(peer)->lpps = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())).count();
						}
						else {
							pInfo(peer)->pps++;
							if (pInfo(peer)->pps >= 250) {
								//string log = format("`4Warning: `w{}`` `9is cheating. Please check him out.````", pData::Algorithm::get_display_name(peer));
								//send_logs(log);
								break;
							}
						}
					}
					BYTE* data_ = get_struct(event.packet);
					if (data_ == nullptr) break;
					PlayerMoving* p_ = unpackPlayerMoving(data_);
					//add_packet_logs("database/tankpacket.txt", " [" + pInfo(peer)->ip + "] (" + pInfo(peer)->name_color + pInfo(peer)->tankIDName + " | World: " + pInfo(peer)->world == "" ? "EXIT" : pInfo(peer)->world + ") " + "[PACKET TYPE]: " + to_string(p_->packetType) + " [NetId]: " + to_string(p_->netID) + " [characterState]: " + to_string(p_->characterState) + " [PlantingTree]: " + to_string(p_->plantingTree) + " [X]: " + to_string(p_->x) + " [Y]: " + to_string(p_->y) + "[PunchX]:" + to_string(p_->punchX) + "[PunchY] : " + to_string(p_->punchY) + "");
					switch (p_->packetType) {
					case 0: /* player state or player movement */
					{
						loop_cheat();
						move_(peer, p_);
						if (pInfo(peer)->x == -1 and pInfo(peer)->y == -1) {
							if (!pInfo(peer)->isStateUpdated) {
								update_clothes(peer);
								pInfo(peer)->isStateUpdated = true;
							}
						}
						CL_Vec2f position = { static_cast<float>(pInfo(peer)->x), static_cast<float>(pInfo(peer)->y) };

						pInfo(peer)->lastx = pInfo(peer)->x;
						pInfo(peer)->lasty = pInfo(peer)->y;
						pInfo(peer)->x = (int)p_->x;
						pInfo(peer)->y = (int)p_->y;
						pInfo(peer)->characterState = p_->characterState;
						pInfo(peer)->state = p_->characterState & 0x10;

						CL_Vec2i current_pos = { static_cast<int>((pInfo(peer)->x + 10) / 32), static_cast<int>((pInfo(peer)->y + 15) / 32) };
						CL_Vec2i future_pos = { static_cast<int>((p_->x + 10) / 32), static_cast<int>((p_->y + 15) / 32) };

						CL_Vec2i top_left = { static_cast<int>(p_->x / 32), static_cast<int>(p_->y / 32) };
						CL_Vec2i top_right = { static_cast<int>((p_->x + 19) / 32), static_cast<int>(p_->y / 32) };
						CL_Vec2i bottom_left = { static_cast<int>(p_->x / 32), static_cast<int>((p_->y + 29) / 32) };
						CL_Vec2i bottom_right = { static_cast<int>((p_->x + 19) / 32), static_cast<int>((p_->y + 29) / 32) };

						//CL_Vec2f position = { static_cast<float>(pInfo(peer)->x), static_cast<float>(pInfo(peer)->y) };

						//bool invalid_movement = false;

						if (pInfo(peer)->hand == 3066) {
							auto world = GetCurrentWorld(pInfo(peer)->world);
							if ((p_->punchX > 0 && p_->punchX < world->width) && (p_->punchY > 0 && p_->punchY < world->height) && p_->plantingTree == 0) edit_tile(peer, p_->punchX, p_->punchY, 18);
						}
						if ((int)p_->characterState == 268435472 || (int)p_->characterState == 268435488 || (int)p_->characterState == 268435504 || (int)p_->characterState == 268435616 || (int)p_->characterState == 268435632 || (int)p_->characterState == 268435456 || (int)p_->characterState == 224 || (int)p_->characterState == 112 || (int)p_->characterState == 80 || (int)p_->characterState == 96 || (int)p_->characterState == 224 || (int)p_->characterState == 65584 || (int)p_->characterState == 65712 || (int)p_->characterState == 65696 || (int)p_->characterState == 65536 || (int)p_->characterState == 65552 || (int)p_->characterState == 65568 || (int)p_->characterState == 65680 || (int)p_->characterState == 192 || (int)p_->characterState == 65664 || (int)p_->characterState == 65600 || (int)p_->characterState == 67860 || (int)p_->characterState == 64) {
							if (pInfo(peer)->lava_time + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
								pInfo(peer)->lavaeffect = 0;
								pInfo(peer)->lava_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							}
							else {
								if (pInfo(peer)->lavaeffect >= (pInfo(peer)->feet == 250 ? 7 : 3) || pInfo(peer)->lavaeffect >= (pInfo(peer)->necklace == 5426 ? 7 : 3)) {
									pInfo(peer)->lavaeffect = 0;
									respawn_player(peer, 0, true);
								}
								else pInfo(peer)->lavaeffect++;
							}
						}
						if (pInfo(peer)->fishing_used != 0) {
							if (pInfo(peer)->f_xy != pInfo(peer)->x + pInfo(peer)->y) pInfo(peer)->move_warning++;
							if (pInfo(peer)->move_warning > 1) stop_fishing(peer, true, "Sit still if you wanna fish!");
							if (p_->punchX > 0 && p_->punchY > 0) {
								pInfo(peer)->punch_warning++;
								if (pInfo(peer)->punch_warning >= 2) stop_fishing(peer, false, "");
							}
						}
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							if (pInfo(peer)->x != -1 and pInfo(peer)->y != -1) {
								//try {
								int x_ = (pInfo(peer)->state == 16 ? (int)p_->x / 32 : round((double)p_->x / 32));
								int y_ = (int)p_->y / 32;
								if (x_ < 0 or x_ >= world_->width or y_ < 0 or y_ >= world_->height) continue;
								WorldBlock* block_ = &world_->blocks[x_ + (y_ * world_->width)];
								if (block_->fg == 1256) pInfo(peer)->hospital_bed = true;
								else pInfo(peer)->hospital_bed = false;
								if (IsObstacle(world_, pInfo(peer), top_left) && IsObstacle(world_, pInfo(peer), top_right) && IsObstacle(world_, pInfo(peer), bottom_left) && IsObstacle(world_, pInfo(peer), bottom_right)) {
									pInfo(peer)->x = position.x, pInfo(peer)->y = position.y;
									Algorithm::set_pos(peer, position, pInfo(peer)->netID);
									break;
								}
								else if (IsObstacle(world_, pInfo(peer), top_left) || IsObstacle(world_, pInfo(peer), top_right) || IsObstacle(world_, pInfo(peer), bottom_left) || IsObstacle(world_, pInfo(peer), bottom_right)) {
									pInfo(peer)->x = position.x, pInfo(peer)->y = position.y;
									Algorithm::set_pos(peer, position, pInfo(peer)->netID);
									break;
								}
								else {
									int x_diff = std::abs(current_pos.x - future_pos.x);
									int y_diff = std::abs(current_pos.y - future_pos.y);
									if (x_diff >= 10 || y_diff >= 10) {
										pInfo(peer)->x = position.x, pInfo(peer)->y = position.y;
										Algorithm::set_pos(peer, position, pInfo(peer)->netID);
										break;
									}
									WorldBlock* current = GetTile(current_pos, world_);
									WorldBlock* future = GetTile(future_pos, world_);
									if (current && future) {
										if (!OnFindPath(pInfo(peer), world_, current_pos, future_pos)) {
											//puts("Arturi noclipa called!");
											pInfo(peer)->x = position.x, pInfo(peer)->y = position.y;
											Algorithm::set_pos(peer, position, pInfo(peer)->netID);
											pInfo(peer)->flagged++;
											pInfo(peer)->isFlagged = true;
											if (pInfo(peer)->flagged >= 3) {
												respawn_player(peer, 0, true);
											}
											if (pInfo(peer)->flagged >= 6) {
												enet_peer_disconnect_later(peer, 0);
											}
											break;
										}
									}
									else {
										pInfo(peer)->x = position.x, pInfo(peer)->y = position.y;
										Algorithm::set_pos(peer, position, pInfo(peer)->netID);
										pInfo(peer)->flagged++;
										pInfo(peer)->isFlagged = true;
										if (pInfo(peer)->flagged >= 3) {
											respawn_player(peer, 0, true);
										}
										if (pInfo(peer)->flagged >= 6) {
											enet_peer_disconnect_later(peer, 0);
										}
										break;
									}
									/*
									if (pInfo(peer)->c_x * 32 != (int)p_->x and pInfo(peer)->c_y * 32 != (int)p_->y and not pInfo(peer)->ghost) {
										bool NoClip = patchNoClip(world_, pInfo(peer)->x, pInfo(peer)->y, block_, peer);
										if (NoClip)
										{
											if (items[block_->fg].actionType != 31 or items[block_->fg].actionType != 32 or items[block_->fg].actionType != 45) {
												gamepacket_t p(0, pInfo(peer)->netID);
												p.Insert("OnSetPos");
												p.Insert(pInfo(peer)->x, pInfo(peer)->y);
												p.CreatePacket(peer);
												pInfo(peer)->flagged++;
												pInfo(peer)->isFlagged = true;
												if (pInfo(peer)->flagged >= 3) {
													respawn_player(peer, 0, true);
												}
												if (pInfo(peer)->flagged >= 6) {
													enet_peer_disconnect_later(peer, 0);
												}
												break;
											}
										}
									}
									*/
								}
								
								if (block_->fg == 428 and not world_->name.empty() and not pInfo(peer)->OnRace) {
									gamepacket_t p(0, pInfo(peer)->netID), p3(0, pInfo(peer)->netID);
									p3.Insert("OnPlayPositioned");
									p3.Insert("audio/race_start.wav");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											p3.CreatePacket(currentPeer);
										}
									}
									p.Insert("OnRaceStart"), p.CreatePacket(peer);
									pInfo(peer)->OnRace = true;
									pInfo(peer)->Race_Time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								}
								if (block_->fg == 430 and pInfo(peer)->OnRace and not world_->name.empty()) {
									int time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count() - pInfo(peer)->Race_Time;
									gamepacket_t p(0, pInfo(peer)->netID), p2, p3(0, pInfo(peer)->netID);
									p2.Insert("OnConsoleMessage");
									p2.Insert((not pInfo(peer)->d_name.empty() ? pInfo(peer)->d_name : (pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color : "`0") + "" + (pInfo(peer)->d_name.empty() ? pInfo(peer)->tankIDName : pInfo(peer)->d_name)) + (pInfo(peer)->is_datemaster ? " `4Datemaster" : "") + (pInfo(peer)->is_legend ? " of Legend" : "") + " `0finished in `$" + Algorithm::detailMSTime(time) + "`o!``");
									p3.Insert("OnPlayPositioned");
									p3.Insert("audio/race_end.wav");
									for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
										if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
										if (pInfo(currentPeer)->world == pInfo(peer)->world) {
											p2.CreatePacket(currentPeer), p3.CreatePacket(currentPeer);
										}
									}
									p.Insert("OnRaceEnd"), p.Insert(time), p.CreatePacket(peer);
									pInfo(peer)->OnRace = false;
									pInfo(peer)->Race_Time = 0;
								}
								if (block_->fg == 1792 and not world_->name.empty()) {
									string world_name = "LEGENDARYMOUNTAIN";
									if (pInfo(peer)->world == world_name) {
										int adaBrp = 0;
										modify_inventory(peer, 1794, adaBrp);
										if (adaBrp != 0) {
											break;
										}
										else {
											int c = 1;
											modify_inventory(peer, 1794, c);
											gamepacket_t p;
											p.Insert("OnTalkBubble");
											p.Insert(pInfo(peer)->netID);
											p.Insert("`9You have claimed a Legendary Orb!");
											p.Insert(0), p.Insert(0);
											p.CreatePacket(peer);

											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == pInfo(peer)->world) {
													gamepacket_t p3;
													p3.Insert("OnParticleEffect");
													p3.Insert(46);
													p3.Insert((float)pInfo(peer)->x + 10, (float)pInfo(peer)->y + 16);
													p3.CreatePacket(currentPeer);
												}
											}
											break;
										}
									}
								}
								if (block_->fg == 3212 and not world_->name.empty()) {
									string world_name = "GROWCH";
									if (pInfo(peer)->world == world_name) {
										int adaBrp = 0;
										modify_inventory(peer, 3210, adaBrp);
										if (adaBrp != 0) {
											break;
										}
										else {
											int c = 1;
											modify_inventory(peer, 3210, c);
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
												if (pInfo(currentPeer)->world == pInfo(peer)->world) {
													gamepacket_t p3;
													p3.Insert("OnParticleEffect");
													p3.Insert(46);
													p3.Insert((float)pInfo(peer)->x + 10, (float)pInfo(peer)->y + 16);
													p3.CreatePacket(currentPeer);
												}
											}
											break;
										}
									}
								}
							}
							if (pInfo(peer)->hand == 2286) {
								if (rand() % 100 < 6) {
									pInfo(peer)->geiger_++;
									if (pInfo(peer)->geiger_ >= 100) {
										int c_ = -1;
										modify_inventory(peer, 2286, c_);
										int c_2 = 1;
										modify_inventory(peer, 2204, c_2);
										pInfo(peer)->hand = 2204;
										pInfo(peer)->geiger_ = 0;
										gamepacket_t p;
										p.Insert("OnConsoleMessage");
										p.Insert("You are detecting radiation... (`$Geiger Counting`` mod added)");
										p.CreatePacket(peer);
										packet_(peer, "action|play_sfx\nfile|audio/dialog_confirm.wav\ndelayMS|0");
										update_clothes(peer);
									}
								}
							}
							if (pInfo(peer)->gems > 0 && pInfo(peer)->back == 240) {
								if (pInfo(peer)->x != (int)p_->x) {
									if (pInfo(peer)->i240 + 750 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
										pInfo(peer)->i240 = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
										pInfo(peer)->gems -= 1;
										WorldDrop item_{};
										item_.id = 112, item_.count = 1, item_.x = (int)p_->x + rand() % 17, item_.y = (int)p_->y + rand() % 17, item_.uid = uint16_t(world_->drop.size()) + 1;
										dropas_(world_, item_);
										gamepacket_t p;
										p.Insert("OnSetBux");
										p.Insert(pInfo(peer)->gems);
										p.Insert(0);
										p.Insert((pInfo(peer)->supp >= 1) ? 1 : 0);
										if (pInfo(peer)->supp >= 2) {
											p.Insert((float)Algorithm::GetServerTime(), (float)1, (float)0);
										}
										p.CreatePacket(peer);
									}
								}
							}
						}
						break;
					}
					
					case 3: /* tempat player wrench atau place tile atau hit tile */
					{
						if (p_->punchX < 0 || p_->punchY < 0) break;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							if (p_->punchX > world_->width || p_->punchY > world_->height) break;
						}
						if (p_->plantingTree <= 0 || p_->plantingTree >= items.size()) break;
						//if ((pInfo(peer)->dev + pInfo(peer)->superdev == 0) && (abs(p_->x / 32 - p_->punchX) > (pInfo(peer)->hand == 11094 ? 6 : 4) || abs(p_->y / 32 - p_->punchY) > (pInfo(peer)->hand == 11094 ? 6 : 4))) break;
						if (pInfo(peer)->Cheat_AF and has_playmod(pInfo(peer), "Cheat Active: Auto Farm") and pInfo(peer)->Cheat_AF_isRunning) {
							gamepacket_t p;
							p.Insert("OnTalkBubble");
							p.Insert(pInfo(peer)->netID);
							p.Insert("You can't do that while auto-farming");
							p.Insert(0), p.Insert(0);
							p.CreatePacket(peer);
							break;
						}
						if (p_->plantingTree != 18 and p_->plantingTree != 32 and items[p_->plantingTree].blockType != BlockTypes::CLOTHING) {
							if (pInfo(peer)->Cheat_AF_PlantingTree == 0 and pInfo(peer)->Cheat_AF) {
								if (p_->plantingTree == 5640) pInfo(peer)->isRemote = true;
								pInfo(peer)->Cheat_AF_PunchX = p_->punchX;
								pInfo(peer)->Cheat_AF_PunchY = p_->punchY;
								pInfo(peer)->Cheat_AF_PlantingTree = p_->plantingTree;
								pInfo(peer)->Cheat_AF_isRunning = true;
								if (p_->punchY == pInfo(peer)->y / 32) {
									if (pInfo(peer)->state == 16) pInfo(peer)->isFacingLeft = true;
								}
								/*
								gamepacket_t p(0, pInfo(peer)->netID);
								p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
								*/
							}
							int c_ = 0;
							modify_inventory(peer, p_->plantingTree, c_);
							if (c_ == 0) break;
						}
						if (p_->plantingTree == 18) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								{
									ENetPeer* punched_on = get_clicked_on(world_, p_->punchX, p_->punchY, peer);
									if (punched_on != NULL and punched_on != peer) {
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(currentPeer)->world == pInfo(peer)->world) {
												if (punched_on == currentPeer and currentPeer != peer and pInfo(currentPeer)->PVP_Ongoing and pInfo(peer)->PVP_Enemy == pInfo(currentPeer)->tankIDName and pInfo(currentPeer)->respawn_time + 2500 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
													pInfo(currentPeer)->PVP_Hit--;
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("`2BATTLE PVP``: " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "`` lost a live!");
													for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
														if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(currentPeer2)->world != pInfo(peer)->world) continue;
														p.CreatePacket(currentPeer2);
													}
													if (pInfo(currentPeer)->PVP_Hit <= 0) {
														pInfo(currentPeer)->PVP_Hit = 10; pInfo(peer)->PVP_Point++;
														SendRespawn(currentPeer, true, 0, 1);
														gamepacket_t p;
														p.Insert("OnConsoleMessage");
														p.Insert("`2BATTLE PVP``: " + pInfo(peer)->name_color + pInfo(peer)->tankIDName + "`` kills " + pInfo(currentPeer)->name_color + pInfo(currentPeer)->tankIDName + "`` and score a point!");
														for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
															if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(currentPeer2)->world != pInfo(peer)->world) continue;
															p.CreatePacket(currentPeer2);
															packet_(currentPeer2, "action|play_sfx\nfile|audio/getpoint.wav\ndelayMS|0");
														}
													}
												}
											}
										}
									}
								}
								if (pInfo(peer)->punch_time + 100 > (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) break;
								pInfo(peer)->punch_time = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
								if (has_playmod(pInfo(peer), "Infected!")) pInfo(peer)->last_infected = p_->punchX + (p_->punchY * 100);
								pInfo(peer)->last_punched = p_->punchX + (p_->punchY * 100);
							}
						}
						if (pInfo(peer)->trading_with != -1 and p_->packetType != 0 and p_->packetType != 18) {
							cancel_trade(peer, false, true);
							break;
						}
						// EDITING FAR BLOCK BREAK
						int punch_ = items[pInfo(peer)->hand].punch_far;
						if (p_->plantingTree == 18 and (punch_ != 1 or pInfo(peer)->wide_punch)) {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								WorldBlock* block_ = &world_->blocks[p_->punchX + (p_->punchY * world_->width)];
								if (block_->fg == 0 and block_->bg == 0) break;

								if (p_->punchY == pInfo(peer)->y / 32) {
									if (pInfo(peer)->wide_punch) punch_ = world_->width;
									if (pInfo(peer)->state == 16) {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX - i, p_->punchY, p_->plantingTree);
									}
									else {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX + i, p_->punchY, p_->plantingTree);
									}
								}
								else if (p_->punchX == pInfo(peer)->x / 32) {
									if (pInfo(peer)->wide_punch) punch_ = world_->height;
									if (p_->punchY > pInfo(peer)->y / 32) {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, p_->punchX, i == 0 ? p_->punchY : p_->punchY + i, p_->plantingTree);
									}
									else if (p_->punchY < pInfo(peer)->y / 32) {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, p_->punchX, i == 0 ? p_->punchY : p_->punchY - i, p_->plantingTree);
									}
									else {
										edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
									}
								}
								else if (p_->punchY < pInfo(peer)->y / 32) {
									if (pInfo(peer)->wide_punch) punch_ = world_->height;
									if (pInfo(peer)->state == 16) {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX - i, i == 0 ? p_->punchY : p_->punchY - i, p_->plantingTree);
									}
									else {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX + i, i == 0 ? p_->punchY : p_->punchY + i, p_->plantingTree);
									}
								}
								else if (p_->punchY > pInfo(peer)->y / 32) {
									if (pInfo(peer)->wide_punch) punch_ = world_->height;
									if (pInfo(peer)->state == 16) {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX - i, i == 0 ? p_->punchY : p_->punchY + i, p_->plantingTree);
									}
									else {
										for (int i = 0; i < punch_; i++)
											edit_tile(peer, i == 0 ? p_->punchX : p_->punchX + i, i == 0 ? p_->punchY : p_->punchY + i, p_->plantingTree);
									}
								}
							}
						}
						else {
							bool empty = false;
							int p_f = items[pInfo(peer)->hand].place_far;
							if (p_->plantingTree == 5640) {
								string name_ = pInfo(peer)->world;
								vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
								if (p != worlds.end()) {
									World* world_ = &worlds[p - worlds.begin()];
									for (int i_ = 0; i_ < world_->machines.size(); i_++) {
										WorldMachines machine = world_->machines[i_];
										if (machine.x == pInfo(peer)->magnetron_x and machine.y == pInfo(peer)->magnetron_y and machine.id == 5638) {
											if (machine.enabled) {
												WorldBlock* itemas = &world_->blocks[machine.x + (machine.y * world_->width)];
												if (itemas->magnetron and itemas->id == pInfo(peer)->magnetron_id) {
													if (itemas->pr > 0) {
														if (p_f > itemas->pr) p_f = itemas->pr;
														p_->plantingTree = itemas->id;
														if (p_f != 1 and not pInfo(peer)->wide_place) {
															if (pInfo(peer)->wide_place) p_f = world_->width;
															if (p_->punchY == pInfo(peer)->y / 32) {
																if (pInfo(peer)->state == 16) {
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, i == 0 ? p_->punchX : p_->punchX - i, p_->punchY, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else {
																	if (pInfo(peer)->wide_place) p_f = world_->width;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, i == 0 ? p_->punchX : p_->punchX + i, p_->punchY, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
															}
															else if (p_->punchX == pInfo(peer)->x / 32) {
																if (p_->punchY > pInfo(peer)->y / 32) {
																	if (pInfo(peer)->wide_place) p_f = world_->height;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX, i == 0 ? p_->punchY : p_->punchY + i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else if (p_->punchY < pInfo(peer)->y / 32) {
																	if (pInfo(peer)->wide_place) p_f = world_->height;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX, i == 0 ? p_->punchY : p_->punchY - i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else {
																	if (edit_tile(peer, p_->punchX, p_->punchY, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
															}
															else if (p_->punchY < pInfo(peer)->y / 32) {
																if (pInfo(peer)->wide_place) p_f = world_->width;
																if (pInfo(peer)->state == 16) {
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX - i, p_->punchY - i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else {
																	if (pInfo(peer)->wide_place) p_f = world_->width;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX + i, p_->punchY - i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
															}
															else if (p_->punchY < pInfo(peer)->y / 32) {
																if (pInfo(peer)->wide_place) p_f = world_->width;
																if (pInfo(peer)->state == 16) {
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX - i, p_->punchY - i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else {
																	if (pInfo(peer)->wide_place) p_f = world_->width;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX + i, p_->punchY - i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
															}
															else if (p_->punchY > pInfo(peer)->y / 32) {
																if (pInfo(peer)->wide_place) p_f = world_->width;
																if (pInfo(peer)->state == 16) {
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX - i, p_->punchY + i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
																else {
																	if (pInfo(peer)->wide_place) p_f = world_->width;
																	for (int i = 0; i < p_f; i++)
																		if (edit_tile(peer, p_->punchX + i, p_->punchY + i, itemas->id, true)) itemas->pr--;
																	if (itemas->pr <= 0) {
																		PlayerMoving data_{};
																		data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																		BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																		BYTE* blc = raw + 56;
																		form_visual(blc, *itemas, *world_, NULL, false);
																		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																			if (pInfo(currentPeer)->world == world_->name) {
																				send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																			}
																		}
																		delete[] raw, blc;
																	}
																	break;
																}
															}
														}
														else if (edit_tile(peer, p_->punchX, p_->punchY, itemas->id, true))
														{
															itemas->pr--;
															if (itemas->pr <= 0) {
																PlayerMoving data_{};
																data_.packetType = 5, data_.punchX = machine.x, data_.punchY = machine.y, data_.characterState = 0x8;
																BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, itemas));
																BYTE* blc = raw + 56;
																form_visual(blc, *itemas, *world_, NULL, false);
																for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																	if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																	if (pInfo(currentPeer)->world == world_->name) {
																		send_raw(currentPeer, 4, raw, 112 + alloc_(world_, itemas), ENET_PACKET_FLAG_RELIABLE);
																	}
																}
																delete[] raw, blc;
															}
														}
														break;
													}
													else {
														empty = true;
														gamepacket_t p;
														p.Insert("OnTalkBubble");
														p.Insert(pInfo(peer)->netID);
														p.Insert("The `2" + items[machine.id].name + "`` is empty!");
														p.Insert(0), p.Insert(0);
														p.CreatePacket(peer);
													}
												}
											}
											break;
										}
									}
									if (p_->plantingTree == 5640 and not empty) {
										gamepacket_t p;
										p.Insert("OnTalkBubble");
										p.Insert(pInfo(peer)->netID);
										p.Insert("There is no active `2" + items[5638].name + "``!");
										p.Insert(0), p.Insert(0);
										p.CreatePacket(peer);
									}
									break;
								}
							}
							else
							{
								int adaBrp = 0;
								modify_inventory(peer, p_->plantingTree, adaBrp);
								/*
								if (p_->plantingTree == 32) {
									edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
									break;
								}
								*/
								int remove = 0;
								int width = 0, height = 0;
								bool removed = true;
								if (adaBrp >= p_f) {
									if ((p_f > 1 or pInfo(peer)->wide_place) and (p_->plantingTree != 32 and items[p_->plantingTree].blockType != BlockTypes::CLOTHING and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].blockType != CONSUMABLE)) {
										string name_ = pInfo(peer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											width = world_->width;
											height = world_->height;
											if (world_->blocks[p_->punchX + (p_->punchY * world_->width)].fg % 2 != 0 and (p_->plantingTree % 2 != 0)) removed = false;
										}
										if (p_->punchY == pInfo(peer)->y / 32) {
											if (pInfo(peer)->wide_place) p_f = width;
											if (pInfo(peer)->state == 16) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX - i, p_->punchY, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX + i, p_->punchY, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
										}
										else if (p_->punchX == pInfo(peer)->x / 32) {
											if (pInfo(peer)->wide_place) p_f = height;
											if (p_->punchY > pInfo(peer)->y / 32) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX, p_->punchY + i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else if (p_->punchY < pInfo(peer)->y / 32) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX, p_->punchY - i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else {
												if (edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree, true))
													//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
													modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
										}
										else if (p_->punchY < pInfo(peer)->y / 32) {
											if (pInfo(peer)->wide_place) p_f = width;
											if (pInfo(peer)->state == 16) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX - i, p_->punchY - i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX + i, p_->punchY - i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].collisionType != 1 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
										}
										else if (p_->punchY < pInfo(peer)->y / 32) {
											if (pInfo(peer)->wide_place) p_f = width;
											if (pInfo(peer)->state == 16) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX - i, p_->punchY - i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX + i, p_->punchY - i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
										}
										else if (p_->punchY > pInfo(peer)->y / 32) {
											if (pInfo(peer)->wide_place) p_f = width;
											if (pInfo(peer)->state == 16) {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX - i, p_->punchY + i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
											else {
												for (int i = 0; i < p_f; i++)
													if (edit_tile(peer, p_->punchX + i, p_->punchY + i, p_->plantingTree, true))
														//if (p_->plantingTree != 32 and p_->plantingTree != 18 and p_->plantingTree != 6336 and items[p_->plantingTree].blockType != CONSUMABLE)
														modify_inventory(peer, p_->plantingTree, remove = -1, removed), remove = 0;
											}
										}
									}
									else if ((p_f == 1) || (p_f > 1 and (p_->plantingTree == 32 or p_->plantingTree % 2 != 0 or items[p_->plantingTree].blockType != BlockTypes::CLOTHING or p_->plantingTree == 18 or p_->plantingTree == 6336 or items[p_->plantingTree].blockType == CONSUMABLE)))
										edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
								}
								else if ((adaBrp < p_f and adaBrp != 0))
									edit_tile(peer, p_->punchX, p_->punchY, p_->plantingTree);
}
						}
						break;
					}
					case 7: /* tile activate request */
					{
						try {
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								if (p_->punchX < 0 or p_->punchX > world_->width or p_->punchY < 0 or p_->punchY >= world_->height) break;
								//try {
								WorldBlock* block_ = &world_->blocks[p_->punchX + (p_->punchY * world_->width)];
								bool impossible = patchNoClip(world_, pInfo(peer)->x, pInfo(peer)->y, block_, peer);
								if (impossible) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`w(Too far away)``");
									break;
								}
								//pInfo(peer)->ignore_cheats2 = true;
								
								/*
								* CL_Vec2i current_pos = { pInfo(peer)->x / 32, pInfo(peer)->y / 32 };
								CL_Vec2i future_pos = { static_cast<int>(GetTilePos(block_->fg, world_).x), static_cast<int>(GetTilePos(block_->fg, world_).y) };
								WorldBlock* current = GetTile(current_pos, world_);
								WorldBlock* tile = GetTile(future_pos, world_);
								if (!current || !tile) break;
								auto base = items[tile->fg ? tile->fg : tile->bg];
								if (std::abs(current_pos.x - future_pos.x) >= 10 || std::abs(current_pos.y - future_pos.y) >= 10) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`w(Too far away)``");
									break;
								}
								if (!OnFindPath(pInfo(peer), world_, current_pos, future_pos) && current != tile) {
									Algorithm::send_bubble(peer, pInfo(peer)->netID, "`w(Too far away)``");
									break;
								}
								*/
								
								if (items[items[block_->fg ? block_->fg : block_->bg].id].blockType == BlockTypes::CHECKPOINT) {
									pInfo(peer)->c_x = p_->punchX, pInfo(peer)->c_y = p_->punchY;
									gamepacket_t p(0, pInfo(peer)->netID);
									p.Insert("SetRespawnPos");
									p.Insert(pInfo(peer)->c_x + (pInfo(peer)->c_y * world_->width));
									p.CreatePacket(peer);
								}
								else if (items[block_->fg ? block_->fg : block_->bg].id == 6) exit_(peer);
								else if (block_->fg == 4722 && pInfo(peer)->adventure_begins == false) {
									pInfo(peer)->adventure_begins = true;
									gamepacket_t p(0);
									p.Insert("OnAddNotification"), p.Insert("interface/large/adventure.rttex"), p.Insert(block_->heart_monitor), p.Insert("audio/gong.wav"), p.Insert(0), p.CreatePacket(peer);
								}
								else if (items[block_->fg].blockType == BlockTypes::DOOR or items[block_->fg].blockType == BlockTypes::PORTAL) {
									if (world_->name == "CARNIVAL" and carnival_open) {
										if (block_->door_destination.find(":GAME") != string::npos) {
											int count = -1;
											if (modify_inventory(peer, 1898, count) == 0) {
												carnival_start(peer, block_->door_destination);
											}
											else {
												gamepacket_t p2;
												p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("This game costs 1 `8Golden Ticket``!"), p2.Insert(0), p2.Insert(1), p2.CreatePacket(peer);
												gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
												p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
												p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
												p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
												break;
											}
										}
									}
									string door_target = block_->door_destination, door_id = "";
									World target_world = worlds[p - worlds.begin()];
									bool locked = (block_->open ? false : (target_world.owner_name == pInfo(peer)->tankIDName or pInfo(peer)->admin or target_world.open_to_public or target_world.owner_name.empty() or (guild_access(peer, target_world.guild_id) or find(target_world.admins.begin(), target_world.admins.end(), pInfo(peer)->uid) != target_world.admins.end()) ? false : true));
									int spawn_x = 0, spawn_y = 0;
									if (not locked and block_->fg != 762) {
										if (door_target.find(":") != string::npos) {
											vector<string> detales = explode(":", door_target);
											door_target = detales[0], door_id = detales[1];
										} if (not door_target.empty() and door_target != world_->name) {
											if (not check_name(door_target)) {
												gamepacket_t p(250, pInfo(peer)->netID);
												p.Insert("OnSetFreezeState");
												p.Insert(1);
												p.CreatePacket(peer);
												{
													gamepacket_t p(250);
													p.Insert("OnConsoleMessage");
													p.Insert(door_target);
													p.CreatePacket(peer);
												}
												{
													gamepacket_t p(250);
													p.Insert("OnZoomCamera");
													p.Insert((float)10000.000000);
													p.Insert(1000);
													p.CreatePacket(peer);
												}
												{
													gamepacket_t p(250, pInfo(peer)->netID);
													p.Insert("OnSetFreezeState");
													p.Insert(0);
													p.CreatePacket(peer);
												}
												break;
											}
											target_world = get_world(door_target);
										}
										//int ySize = (int)target_world.blocks.size() / world_->width, xSize = (int)target_world.blocks.size() / ySize, square = (int)target_world.blocks.size();
										if (not door_id.empty()) {
											for (int i_ = 0; i_ < target_world.blocks.size(); i_++) {
												WorldBlock block_data = target_world.blocks[i_];
												if (block_data.fg == 1684 or block_data.fg == 4482 or block_data.fg == 1912 or items[block_data.fg].blockType == BlockTypes::DOOR or items[block_data.fg].blockType == BlockTypes::PORTAL) {
													if (block_data.door_id == door_id) {
														spawn_x = i_ % target_world.width, spawn_y = i_ / target_world.width;
														break;
													}
												}
											}
										}
									}
									if (block_->fg == 762) {
										pInfo(peer)->lastwrenchx = p_->punchX, pInfo(peer)->lastwrenchy = p_->punchY;
										gamepacket_t p2;
										if (block_->door_id == "") p2.Insert("OnTalkBubble"), p2.Insert(pInfo(peer)->netID), p2.Insert("No password has been set yet!"), p2.Insert(0), p2.Insert(1);
										else p2.Insert("OnDialogRequest"), p2.Insert("set_default_color|`o\nadd_label_with_icon|big|`wPassword Door``|left|762|\nadd_textbox|The door requires a password.|left|\nadd_text_input|password|Password||24|\nend_dialog|password_reply|Cancel|OK|");
										p2.CreatePacket(peer);
										gamepacket_t p(250, pInfo(peer)->netID), p3(250), p4(250, pInfo(peer)->netID);
										p.Insert("OnSetFreezeState"), p.Insert(1), p.CreatePacket(peer);
										p3.Insert("OnZoomCamera"), p3.Insert((float)10000.000000), p3.Insert(1000), p3.CreatePacket(peer);
										p4.Insert("OnSetFreezeState"), p4.Insert(0), p4.CreatePacket(peer);
									}
									if (block_->fg != 762) join_world(peer, target_world.name, spawn_x, spawn_y, 250, locked, true);
								}
								else {
									switch (block_->fg) {
									case 3270: case 3496:
									{
										Position2D steam_connector = track_steam(world_, block_, p_->punchX, p_->punchY);
										if (steam_connector.x != -1 and steam_connector.y != -1) {
											WorldBlock* block_s = &world_->blocks[steam_connector.x + (steam_connector.y * world_->width)];
											switch (block_s->fg) {
											case 3286: //steam door
											{
												block_s->flags = (block_s->flags & 0x00400000 ? block_s->flags ^ 0x00400000 : block_s->flags | 0x00400000);
												PlayerMoving data_{};
												data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
												BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
												BYTE* blc = raw + 56;
												form_visual(blc, *block_s, *world_, peer, false);
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(currentPeer)->world == world_->name) {
														send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
													}
												}
												delete[] raw, blc;
												break;
											}
											case 3724: // spirit storage unit
											{
												uint32_t scenario = 20;
												{
													// check for ghost jars
													for (int i = 0; i < world_->drop.size(); i++) {
														WorldDrop* check_drop = &world_->drop[i];
														Position2D dropped_at{ check_drop->x / 32, check_drop->y / 32 };
														if (dropped_at.x == steam_connector.x and dropped_at.y == steam_connector.y) {
															if (check_drop->id == 3722) {
																uint32_t explo_chance = check_drop->count;
																// remove drop
																{
																	PlayerMoving data_{};
																	data_.packetType = 14, data_.netID = -2, data_.plantingTree = check_drop->uid;
																	BYTE* raw = packPlayerMoving(&data_);
																	int32_t item = -1;
																	memcpy(raw + 8, &item, 4);
																	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																		if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																		if (pInfo(currentPeer)->world == name_) {
																			send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																		}
																	}
																	world_->drop[i].id = 0, world_->drop[i].x = -1, world_->drop[i].y = -1;
																	delete[] raw;
																}
																block_s->c_ += explo_chance;
																// explode or not
																{
																	if (block_s->c_ * 5 >= 105) {
																		float explosion_chance = (float)((block_s->c_ * 5) - 100) * 0.5;
																		if (explosion_chance > rand() % 100) {
																			//bam bam
																			block_s->fg = 3726;
																			// drop the prize
																			{
																				vector<int> all_p{ 3734, 3732, 3748, 3712, 3706, 3708, 3718, 11136, 7728, 10056, 9148, 3730, 7728,   3788, 3750, 3738, 6060, 3738, 6840, 3736, 3750 };
																				uint32_t prize = 0;
																				if (block_s->c_ * 5 <= 115) prize = 3734;
																				else if (block_s->c_ * 5 <= 130) prize = 3732;
																				else if (block_s->c_ * 5 <= 140) prize = 3748;
																				else if (block_s->c_ * 5 <= 170) {
																					vector<int> p_drops = {
																						3712, 3706, 3708, 3718, 11136
																					};
																					prize = p_drops[rand() % p_drops.size()];
																				}
																				else if (block_s->c_ * 5 <= 180) prize = 9148;
																				else if (block_s->c_ * 5 <= 190)  prize = 7782;
																				else if (block_s->c_ * 5 <= 205)  prize = 10056;
																				else if (block_s->c_ * 5 <= 220)  prize = 9148;
																				else if (block_s->c_ * 5 == 225)  prize = 3788;
																				else if (block_s->c_ * 5 <= 240)  prize = 3750;
																				else if (block_s->c_ * 5 == 245)  prize = 3738;
																				else if (block_s->c_ * 5 <= 255)  prize = 6060;
																				else if (block_s->c_ * 5 <= 265 or explo_chance * 5 >= 265) {
																					if (explo_chance * 5 >= 265) prize = all_p[rand() % all_p.size()];
																					else prize = 3738;
																				}
																				else {
																					vector<int> p_drops = {
																						6840
																					};
																					if (block_s->c_ * 5 >= 270) p_drops.push_back(3736);
																					if (block_s->c_ * 5 >= 295) p_drops.push_back(3750);
																					if (block_s->c_ * 5 >= 300) p_drops.push_back(6064);
																					prize = p_drops[rand() % p_drops.size()];
																				} if (prize != 0) {
																					WorldDrop drop_block_{};
																					drop_block_.x = steam_connector.x * 32 + rand() % 17;
																					drop_block_.y = steam_connector.y * 32 + rand() % 17;
																					drop_block_.id = prize, drop_block_.count = 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1;
																					dropas_(world_, drop_block_);
																					{
																						PlayerMoving data_{};
																						data_.packetType = 0x11, data_.x = steam_connector.x * 32 + 16, data_.y = steam_connector.y * 32 + 16;
																						data_.YSpeed = 97, data_.XSpeed = 3724;
																						BYTE* raw = packPlayerMoving(&data_);
																						PlayerMoving data_2{};
																						data_2.packetType = 0x11, data_2.x = steam_connector.x * 32 + 16, data_2.y = steam_connector.y * 32 + 16;
																						data_2.YSpeed = 108;
																						BYTE* raw2 = packPlayerMoving(&data_2);
																						gamepacket_t p;
																						p.Insert("OnConsoleMessage");
																						p.Insert("`#[A `9Spirit Storage Unit`` exploded, bringing forth an `9" + items[prize].name + "`` from The Other Side!]``");
																						for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																							if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																							if (pInfo(currentPeer)->world == world_->name) {
																								p.CreatePacket(currentPeer);
																								send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
																								send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
																							}
																						}
																						delete[] raw, raw2;
																					}
																					scenario = 22;
																				}
																			}
																			block_s->c_ = 0;
																		}
																	}
																}
																// update visuals
																{
																	PlayerMoving data_{};
																	data_.packetType = 5, data_.punchX = steam_connector.x, data_.punchY = steam_connector.y, data_.characterState = 0x8;
																	BYTE* raw = packPlayerMoving(&data_, 112 + alloc_(world_, block_s));
																	BYTE* blc = raw + 56;
																	form_visual(blc, *block_s, *world_, peer, false);
																	for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
																		if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
																		if (pInfo(currentPeer)->world == world_->name) {
																			send_raw(currentPeer, 4, raw, 112 + alloc_(world_, block_s), ENET_PACKET_FLAG_RELIABLE);
																		}
																	}
																	delete[] raw, blc;
																}
																break;
															}
														}
													}
												}
												PlayerMoving data_{};
												data_.packetType = 32; // steam update paketas
												data_.punchX = steam_connector.x;
												data_.punchY = steam_connector.y;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = scenario;
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
													if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
													send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												}
												delete[] raw;
												break;
											}
											default:
												break;
											}
										}
										PlayerMoving data_{};
										data_.packetType = 32; // steam update paketas
										data_.punchX = p_->punchX;
										data_.punchY = p_->punchY;
										BYTE* raw = packPlayerMoving(&data_);
										for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
											if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
											if (pInfo(peer)->world != pInfo(currentPeer)->world) continue;
											send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
										}
										delete[] raw;
										break;
									}
									default:
										break;
									}
								}
							}
						}
						catch (const out_of_range& e) {
							tayo_warn(e.what());
						}
						break;
					}
					case 10: /*Kai zaidejas paspaudzia du kartus ant inventory itemo*/
					{
						if (pInfo(peer)->trading_with != -1) {
							cancel_trade(peer, false);
							break;
						}
						if (p_->plantingTree <= 0 or p_->plantingTree >= items.size()) break;
						int c_ = 0;
						modify_inventory(peer, p_->plantingTree, c_);
						if (c_ == 0) break;
						if (items[p_->plantingTree].blockType != BlockTypes::CLOTHING) {
							int free = get_free_slots(pInfo(peer)), slot = 1;
							int c242 = 242, c1796 = 1796, c6802 = 6802, c1486 = 1486, countofused = 0, getdl = 1, getwl = 100, removewl = -100, removedl = -1, countwl = 0, c4450 = 4450, c4452 = 4452;
							int c4298 = 4298, c4300 = 4300;
							int c7188 = 7188;
							modify_inventory(peer, p_->plantingTree, countofused);
							if (free >= slot) {
								if (p_->plantingTree == 242 || p_->plantingTree == 1796) {
									modify_inventory(peer, p_->plantingTree == 242 ? c1796 : c242, countwl);
									if (p_->plantingTree == 242 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 242 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 242 ? c242 : c1796, p_->plantingTree == 242 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 242 ? c1796 : c242, p_->plantingTree == 242 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 242 ? "You compressed 100 `2World Lock`` into a `2Diamond Lock``!" : "You shattered a `2Diamond Lock`` into 100 `2World Lock``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 242 ? "You compressed 100 `2World Lock`` into a `2Diamond Lock``!" : "You shattered a `2Diamond Lock`` into 100 `2World Lock``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 7188) {
									modify_inventory(peer, c1796, countwl);
									if (countwl <= 100) {
										if (countofused >= 1) {
											modify_inventory(peer, c7188, removedl);
											modify_inventory(peer, c1796, getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert("You shattered a `2Blue Gem Lock`` into 100 `2Diamond Lock``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert("You shattered a `2Blue Gem Lock`` into 100 `2Diamond Lock``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 1486 || p_->plantingTree == 6802) {
									modify_inventory(peer, p_->plantingTree == 1486 ? c6802 : c1486, countwl);
									if (p_->plantingTree == 1486 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 1486 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 1486 ? c1486 : c6802, p_->plantingTree == 1486 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 1486 ? c6802 : c1486, p_->plantingTree == 1486 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 1486 ? "You compressed 100 `2Growtoken`` into a `2Mega Growtoken``!" : "You shattered a `2Mega Growtoken`` into 100 `2Growtoken``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 1486 ? "You compressed 100 `2Growtoken`` into a `2Mega Growtoken``!" : "You shattered a `2Mega Growtoken`` into 100 `2Growtoken``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 4450 || p_->plantingTree == 4452) {
									modify_inventory(peer, p_->plantingTree == 4450 ? c4452 : c4450, countwl);
									if (p_->plantingTree == 4450 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 4450 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 4450 ? c4450 : c4452, p_->plantingTree == 4450 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 4450 ? c4452 : c4450, p_->plantingTree == 4450 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 4450 ? "You compressed 100 `2Zombie Brain`` into a `2Pile of Zombie Brains``!" : "You shattered a `2Pile of Zombie Brains`` into 100 `2Zombie Brain``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 4450 ? "You compressed 100 `2Zombie Brain`` into a `2Pile of Zombie Brains``!" : "You shattered a `2Pile of Zombie Brains`` into 100 `2Zombie Brain``!"), p2.CreatePacket(peer);
										}
									}
								}
								else if (p_->plantingTree == 4298 || p_->plantingTree == 4300) {
									modify_inventory(peer, p_->plantingTree == 4298 ? c4300 : c4298, countwl);
									if (p_->plantingTree == 4298 ? countwl <= 199 : countwl <= 100) {
										if (p_->plantingTree == 4298 ? countofused >= 100 : countofused >= 1) {
											modify_inventory(peer, p_->plantingTree == 4298 ? c4298 : c4300, p_->plantingTree == 4298 ? removewl : removedl);
											modify_inventory(peer, p_->plantingTree == 4298 ? c4300 : c4298, p_->plantingTree == 4298 ? getdl : getwl);
											gamepacket_t p, p2;
											p.Insert("OnTalkBubble"), p.Insert(pInfo(peer)->netID), p.Insert(p_->plantingTree == 4298 ? "You compressed 100 `2Caduceus`` into a `2Golden Caduceus``!" : "You shattered a `2Golden Caduceus`` into 100 `2Caduceus``!"), p.Insert(0), p.Insert(1), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert(p_->plantingTree == 4298 ? "You compressed 100 `2Caduceus`` into a `2Golden Caduceus``!" : "You shattered a `2Golden Caduceus`` into 100 `2Caduceus``!"), p2.CreatePacket(peer);
										}
									}
								}
							}
							/*compress ir t.t*/
							break;
						}
						/*equip*/
						equip_clothes(peer, p_->plantingTree);
						break;
					}
					case 11: /*Taking dropped items*/
					{
						if (p_->x < 0 || p_->y < 0) break;
						int currentX = pInfo(peer)->x / 32;
						int currentY = pInfo(peer)->y / 32;
						int targetX = p_->x / 32;
						int targetY = p_->y / 32;
						if (abs(targetX - currentX) >= 2 || abs(targetY - currentY) >= 2)
						{
							if (currentX != 0 && currentY != 0)
							{
								break;
							}
						}
						
						bool displaybox = true;
						string name_ = pInfo(peer)->world;
						vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
						if (p != worlds.end()) {
							World* world_ = &worlds[p - worlds.begin()];
							for (int i_ = 0; i_ < world_->drop.size(); i_++) {
								if (world_->drop[i_].id == 0 or world_->drop[i_].x / 32 < 0 or world_->drop[i_].x / 32 > world_->width or world_->drop[i_].y / 32 < 0 or world_->drop[i_].y / 32 > (world_->height - 1)) continue;
								if (world_->drop[i_].uid == p_->plantingTree) {
									WorldBlock* block_ = &world_->blocks[world_->drop[i_].x / 32 + (world_->drop[i_].y / 32 * world_->width)];
									if (block_->fg == 1422 || block_->fg == 2488) displaybox = block_access(peer, world_, block_);
									if (abs((int)p_->x / 32 - world_->drop[i_].x / 32) > 1 || abs((int)p_->x - world_->drop[i_].x) >= 32 or abs((int)p_->y - world_->drop[i_].y) >= 32) displaybox = false;
									bool noclip = false;
									/*
									float x = (world_->drop[i_].x) / 32;
									float y = (world_->drop[i_].y) / 32;
									if (x - static_cast<int>(x) >= 0.75f)
										x = static_cast<float>(std::round(x));
									if (y - static_cast<int>(y) >= 0.75f)
										y = static_cast<float>(std::round(y));

									CL_Vec2i player_pos = { (pInfo(peer)->x + 10) / 32, (pInfo(peer)->y + 15) / 32 };
									CL_Vec2i current_pos = { player_pos.x, player_pos.y };
									CL_Vec2i future_pos = { static_cast<int>(x), static_cast<int>(y) };
									int x_diff = std::abs(current_pos.x - future_pos.x);
									int y_diff = std::abs(current_pos.y - future_pos.y);

									if (x_diff >= 25 || y_diff >= 25) {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "`w(Too far away)``");
										break;
									}
									*/
									if (pInfo(peer)->dev || pInfo(peer)->admin || world_->owner_name == pInfo(peer)->tankIDName || find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) != world_->admins.end()) {
										if (pInfo(peer)->dev || world_->owner_name == pInfo(peer)->tankIDName || find(world_->admins.begin(), world_->admins.end(), pInfo(peer)->uid) != world_->admins.end()) displaybox = true;
									}
									else {
										noclip = NotCollectable(world_, block_, pInfo(peer));
										//noclip = OnFindPath(pInfo(peer), world_, current_pos, future_pos);
									}
									if (noclip) {
										Algorithm::send_bubble(peer, pInfo(peer)->netID, "`w(Too far away)``");
										break;
									}
									if (displaybox && noclip == false) {
										int c_ = world_->drop[i_].count;
										if (world_->drop[i_].id == world_->special_event_item && world_->special_event && world_->drop[i_].special) {
											world_->special_event_item_taken++;
											if (items[world_->special_event_item].event_total == world_->special_event_item_taken) {
												gamepacket_t p, p3;
												p.Insert("OnAddNotification"), p.Insert("interface/large/special_event.rttex"), p.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess! " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` found it!``" : "All items found!``") + ""), p.Insert("audio/cumbia_horns.wav"), p.Insert(0);
												p3.Insert("OnConsoleMessage"), p3.Insert("`2" + items[world_->special_event_item].event_name + ":`` `oSuccess!`` " + (items[world_->special_event_item].event_total == 1 ? "`2" + pInfo(peer)->tankIDName + "`` `ofound it!``" : "All items found!``") + "");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													if (items[world_->special_event_item].event_total != 1) {
														gamepacket_t p2;
														p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->special_event_item].name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``"), p2.CreatePacket(currentPeer);
													}
													p.CreatePacket(currentPeer);
													p3.CreatePacket(currentPeer);
												}
												world_->last_special_event = 0, world_->special_event_item = 0, world_->special_event_item_taken = 0, world_->special_event = false;
											}
											else {
												gamepacket_t p2;
												p2.Insert("OnConsoleMessage"), p2.Insert("`2" + items[world_->special_event_item].event_name + ":`` `0" + pInfo(peer)->tankIDName + "`` found a " + items[world_->special_event_item].name + "! (" + to_string(world_->special_event_item_taken) + "/" + to_string(items[world_->special_event_item].event_total) + ")``");
												for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
													if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
													p2.CreatePacket(currentPeer);
												}
											}
										}
										if (modify_inventory(peer, world_->drop[i_].id, c_, false, true) == 0 or world_->drop[i_].id == 112) {
											PlayerMoving data_{};
											data_.effect_flags_check = 1, data_.packetType = 14, data_.netID = pInfo(peer)->netID, data_.plantingTree = world_->drop[i_].uid;
											BYTE* raw = packPlayerMoving(&data_);
											if (world_->drop[i_].id == 112) {
												pInfo(peer)->gems += c_;
												if (pInfo(peer)->lastquest == "Ruler Sword" && pInfo(peer)->quest_step == 3 && pInfo(peer)->quest_active) {
													pInfo(peer)->quest_progress += c_;
													if (pInfo(peer)->quest_progress >= 2000000000 && pInfo(peer)->blmNotip) pInfo(peer)->blmNotip = false, Algorithm::send_console_message(peer, "`9Legendary Wizz Quest: Your Ruler Sword (3/5) earn 2B gems complete!");
												}
											}
											else {
												add_cctv(peer, "took", to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name);
												gamepacket_t p;
												p.Insert("OnConsoleMessage"), p.Insert("Collected `w" + to_string(world_->drop[i_].count) + "" + (items[world_->drop[i_].id].blockType == BlockTypes::FISH ? "lb." : "") + " " + items[world_->drop[i_].id].ori_name + "``." + (items[world_->drop[i_].id].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop[i_].id].rarity) + "``") + ""), p.CreatePacket(peer);
											}
											for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
												if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
												send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
											}
											delete[]raw;
											world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
											break;
										}
										else {
											if ((c_ < 200 and world_->drop[i_].count >(200 - c_))) {
												if ((items[world_->drop[i_].id].blockType == FISH)) {
													int got = 0;
													modify_inventory(peer, world_->drop[i_].id, got);
													if (got > 0) break;
												}
												int b_ = 200 - c_;
												world_->drop[i_].count -= b_;
												if (modify_inventory(peer, world_->drop[i_].id, b_, false) == 0) {
													add_cctv(peer, "took", to_string(world_->drop[i_].count) + " " + items[world_->drop[i_].id].name);
													WorldDrop drop_{};
													drop_.id = world_->drop[i_].id, drop_.count = world_->drop[i_].count, drop_.uid = uint16_t(world_->drop.size()) + 1, drop_.x = world_->drop[i_].x, drop_.y = world_->drop[i_].y;
													world_->drop.push_back(drop_);
													gamepacket_t p;
													p.Insert("OnConsoleMessage");
													p.Insert("Collected `w" + to_string(200 - c_) + " " + items[world_->drop[i_].id].ori_name + "``." + (items[world_->drop[i_].id].rarity > 363 ? "" : " Rarity: `w" + to_string(items[world_->drop[i_].id].rarity) + "``") + "");
													PlayerMoving data_{};
													data_.packetType = 14, data_.netID = -1, data_.plantingTree = world_->drop[i_].id, data_.x = world_->drop[i_].x, data_.y = world_->drop[i_].y;
													int32_t item = -1;
													float val = world_->drop[i_].count;
													BYTE* raw = packPlayerMoving(&data_);
													data_.plantingTree = world_->drop[i_].id;
													memcpy(raw + 8, &item, 4);
													memcpy(raw + 16, &val, 4);
													val = 0;
													data_.netID = pInfo(peer)->netID;
													data_.plantingTree = world_->drop[i_].uid;
													data_.x = 0, data_.y = 0;
													BYTE* raw2 = packPlayerMoving(&data_);
													BYTE val2 = 0;
													memcpy(raw2 + 8, &item, 4);
													memcpy(raw2 + 16, &val, 4);
													memcpy(raw2 + 1, &val2, 1);
													world_->drop[i_].id = 0, world_->drop[i_].x = -1, world_->drop[i_].y = -1;
													for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
														if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(currentPeer)->world != name_) continue;
														send_raw(currentPeer, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
														if (pInfo(currentPeer)->netID == pInfo(peer)->netID)
															p.CreatePacket(currentPeer);
														send_raw(currentPeer, 4, raw2, 56, ENET_PACKET_FLAG_RELIABLE);
													}
													delete[]raw, raw2;
													break;
												}
											}
										}
									}
								}
							}
						}
						break;
					}
					case 18: { //PACKET_SET_ICON_STATE
						//move_(peer, p_);
						break;
					}
					case 23: /*TO DO KILL PLAYER QUEST*/
					{
						if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								bool can_cancel = true;
								if (find(world_->active_jammers.begin(), world_->active_jammers.end(), 1276) != world_->active_jammers.end()) can_cancel = false;
								if (can_cancel) {
									if (pInfo(peer)->trading_with != -1 and not pInfo(peer)->knockback and p_->packetType != 0 and p_->packetType != 18) {
										cancel_trade(peer, false, true);
										break;
									}
								}
							}
						}
						{
							string name_ = pInfo(peer)->world;
							vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
							if (p != worlds.end()) {
								World* world_ = &worlds[p - worlds.begin()];
								gamepacket_t p3;

								for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
									if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
									if (peer != currentPeer) {
										if (pInfo(peer)->world == pInfo(currentPeer)->world) {
											if (pInfo(currentPeer)->lastwrenchx == (pInfo(peer)->x / 32) and pInfo(currentPeer)->kill_mode and pInfo(currentPeer)->lastwrenchy == (pInfo(peer)->y / 32)) {
												p3.Insert("OnParticleEffect"), p3.Insert(108), p3.Insert((float)pInfo(peer)->x + 16, (float)pInfo(peer)->y + 16);
												SendRespawn(peer, false, true, true);
												pInfo(currentPeer)->kill_++;
												bool unlocked_ = false;
												if (pInfo(currentPeer)->kill_ == 20) world_->d_weather = 9, unlocked_ = true;
												for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
													if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(peer)->world != pInfo(currentPeer2)->world) continue;
													if (unlocked_) {
														Algorithm::send_console(currentPeer2, format("{}`` has unlocked their God Mode!", pInfo(currentPeer)->modName));
														gamepacket_t p;
														p.Insert("OnSetCurrentWeather");
														p.Insert(world_->d_weather);
														p.CreatePacket(currentPeer2);
													}
													p3.CreatePacket(currentPeer2);
												}
											}
										}
									}
								}
							}
						}
						/*
						if (pInfo(peer)->last_inf + 5000 < (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count()) {
							pInfo(peer)->last_inf = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
							bool inf = false;
							for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
								if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL or pInfo(peer)->world != pInfo(currentPeer)->world or pInfo(peer)->netID == pInfo(currentPeer)->netID) continue;
								if (abs(pInfo(currentPeer)->last_infected - p_->plantingTree) <= 3) {
									if (has_playmod(pInfo(currentPeer), "Infected!") && not has_playmod(pInfo(peer), "Infected!") && inf == false) {
										if (has_playmod(pInfo(peer), "Antidote!")) {
											for (ENetPeer* currentPeer2 = server->peers; currentPeer2 < &server->peers[server->peerCount]; ++currentPeer2) {
												if (currentPeer2->state != ENET_PEER_STATE_CONNECTED or currentPeer2->data == NULL or pInfo(peer)->world != pInfo(currentPeer2)->world) continue;
												PlayerMoving data_{};
												data_.packetType = 19, data_.punchX = 782, data_.x = pInfo(peer)->x + 10, data_.y = pInfo(peer)->y + 16;
												int32_t to_netid = pInfo(peer)->netID;
												BYTE* raw = packPlayerMoving(&data_);
												raw[3] = 5;
												memcpy(raw + 8, &to_netid, 4);
												send_raw(currentPeer2, 4, raw, 56, ENET_PACKET_FLAG_RELIABLE);
												delete[]raw;
											}
										}
										else {
											pInfo(currentPeer)->last_infected = 0;
											inf = true;
											gamepacket_t p, p2;
											p.Insert("OnAddNotification"), p.Insert("interface/large/infected.rttex"), p.Insert("`4You were infected by " + pInfo(currentPeer)->tankIDName + "!"), p.CreatePacket(peer);
											p2.Insert("OnConsoleMessage"), p2.Insert("You've been infected by the g-Virus. Punch others to infect them, too! Braiiiins... (`$Infected!`` mod added, `$1 mins`` left)"), p2.CreatePacket(peer);
											PlayMods give_playmod{};
											give_playmod.id = 28, give_playmod.time = time(nullptr) + 60;
											pInfo(peer)->playmods.push_back(give_playmod);
											update_clothes(peer);
										}
									}
									if (has_playmod(pInfo(peer), "Infected!") && not has_playmod(pInfo(currentPeer), "Infected!") && inf == false) {
										inf = true;
										SendRespawn(peer, 0, true);
										for (int i_ = 0; i_ < pInfo(peer)->playmods.size(); i_++) {
											if (pInfo(peer)->playmods[i_].id == 28) {
												pInfo(peer)->playmods[i_].time = 0;
												break;
											}
										}
										string name_ = pInfo(currentPeer)->world;
										vector<World>::iterator p = find_if(worlds.begin(), worlds.end(), [name_](const World& a) { return a.name == name_; });
										if (p != worlds.end()) {
											World* world_ = &worlds[p - worlds.begin()];
											WorldDrop drop_block_{};
											drop_block_.id = rand() % 100 < 50 ? 4450 : 4490, drop_block_.count = pInfo(currentPeer)->hand == 9500 ? 2 : 1, drop_block_.uid = uint16_t(world_->drop.size()) + 1, drop_block_.x = pInfo(peer)->x, drop_block_.y = pInfo(peer)->y;
											dropas_(world_, drop_block_);
										}
									}
								}
							}
						}*/
						break;
					}
					default:
					{
						break;
					}
					}
					break;
				}
				default:
					break;
				}
				//enet_event_destroy(event);
				break;
			}
			default:
				break;
			}
		}
	}
	}
	catch (exception& e) {
		tayo_warn(e.what());
	}
	return 0;
}