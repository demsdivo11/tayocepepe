#pragma once
#include <chrono>
#include <sstream>
#include <filesystem>
#include <regex>
#include <random>
#include "enet/enet.h"
#include "GameUpdatePacket.h"
//#include "stlastar.h"




class DialogReturnParser {
	vector<pair<string, string>> contents;
public:
	DialogReturnParser(string const& str) {
		for (string const& line : explode("\n", str)) {
			vector<string> lineContents = explode("|", line);
			if (lineContents.size() < 2) continue;

			contents.emplace_back(lineContents[0], lineContents[1]);
		}
	}
	pair<string, string> get_value_starts_with(string const& key) {
		for (auto const& [contentKey, value] : contents) {
			if (contentKey.starts_with(key)) return { contentKey, value };
		}
		return {};
	}
	string get_value(string const& key) {
		for (auto const& [contentKey, value] : contents) {
			if (contentKey == key) return value;
		}
		return "";
	}
};
std::size_t callback(
	const char* in,
	std::size_t size,
	std::size_t num,
	std::string* out)
{
	const std::size_t totalBytes(size * num);
	out->append(in, totalBytes);
	return totalBytes;
}
class AStarPathFinderBase {
	AStarPathFinderBase() = default;
	explicit AStarPathFinderBase(int width, int height) {
		_M_Size = { width, height };
		_M_Map.reserve(width * height);
		_M_Map = {
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,   // 00
			1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1,   // 01
			1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 02
			1,9,9,1,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 03
			1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 04
			1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 05
			1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 06
			1,9,9,9,9,9,9,9,9,1,1,1,9,9,9,9,9,9,9,1,   // 07
			1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 08
			1,9,1,9,9,9,9,9,9,9,1,1,9,9,9,9,9,9,9,1,   // 09
			1,9,1,1,1,1,9,1,1,9,1,1,1,1,1,1,1,1,1,1,   // 10
			1,9,9,9,9,9,1,9,1,9,1,9,9,9,9,9,1,1,1,1,   // 11
			1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 12
			1,9,1,9,1,9,9,9,1,9,1,9,1,9,1,9,9,9,1,1,   // 13
			1,9,1,1,1,1,9,9,1,9,1,9,1,1,1,1,9,9,1,1,   // 14
			1,9,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,1,   // 15
			1,9,9,9,9,1,1,1,1,1,1,9,9,9,9,1,1,1,1,1,   // 16
			1,1,9,9,9,9,9,9,9,1,1,1,9,9,9,1,9,9,9,9,   // 17
			1,9,1,1,1,1,1,1,1,1,1,9,1,1,1,1,1,1,1,1,   // 18
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1   // 19
		};
	}
	~AStarPathFinderBase() = default;
	auto GetMap(int x, int y) -> int {
		if (x >= _M_Size.x || x < 0 || y >= _M_Size.y || y < 0)
			return { 9 };

		return _M_Map.at(x + (y * _M_Size.y));
	}
	auto GetMap(CL_Vec2i pos) -> int {
		return this->GetMap(pos.x, pos.y);
	}
private:
	CL_Vec2i _M_Size{ 100, 60 };

	vector<int> _M_Map;
};
class AStarPathFinderNode {
public:
	AStarPathFinderNode() = default;
	explicit AStarPathFinderNode(CL_Vec2i pos) : _M_Pos(pos) {}
	explicit AStarPathFinderNode(int x, int y) {
		_M_Pos = { x, y };
	}
	auto GetPos() const -> CL_Vec2i {
		return _M_Pos;
	}
	auto GetDistance(AStarPathFinderNode& node) -> float {
		return abs(_M_Pos.x - node.GetPos().x) + abs(_M_Pos.y - node.GetPos().y);
	}
	auto SamePosition(AStarPathFinderNode& node) -> bool {
		return _M_Pos == node.GetPos();
	}
private:
	CL_Vec2i _M_Pos{ -1, -1 };
};
namespace Algorithm {
	int GetServerTime() {
		std::time_t now = std::time(nullptr);
		std::tm* startOfDay = std::localtime(&now);
		startOfDay->tm_hour = 0;
		startOfDay->tm_min = 0;
		startOfDay->tm_sec = 0;
		std::time_t startOfDayTime = std::mktime(startOfDay);
		std::time_t duration = now - startOfDayTime;
		return duration;
	}
	bool get_str_from_packet(ENetPacket* packet, std::string& str)
	{
		packet->data[packet->dataLength - 1] = 0;
		str = reinterpret_cast<char*>(packet->data + 4);

		return !(packet->dataLength < 5 || packet->dataLength > 512);
	}
	char* enet_get_text_ptr(ENetPacket* packet)
	{
		packet->data[packet->dataLength - 1] = 0;
		return (char*)(packet->data + 4);
	}
	template<typename T>
	T rand(T const& min, T const& max) {
		mt19937 generator{ random_device{}() };
		return uniform_int_distribution<T>{ min, max }(generator);
	}
	template<typename T>
	vector<T> skip_array(vector<T> const& v, int const& start, int const& distance) {
		vector<T> result;
		for (int i = (start - 1) * distance; i < (start - 1) * distance + distance; i++) {
			if (i > v.size() - 1) break;
			result.push_back(v[i]);
		}
		return result;
	}
	template<typename T = std::chrono::seconds>
	int64_t get_epoch_time() {
		return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	int64_t get_epoch_seconds() {
		return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	int64_t get_epoch_ms() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	string get_time() {
		time_t time = std::time(nullptr);
		tm localTime = *std::localtime(&time);

		ostringstream os;
		os << put_time(&localTime, "%Y-%m-%d %H:%M:%S");

		return os.str();
	}
	string decode_special_char(string const& str) {
		string result;
		for (char const& c : str) {
			switch (c) {
			case '\n': result += "\\n";
				break;
			case '\t': result += "\\t";
				break;
			case '\\': result += "\\\\";
				break;
			default: result += c;
			}
		}
		return result;
	}
	string join(vector<string> const& v, string const& delimeter) {
		string str;
		for (auto it = v.begin(); it != v.end(); it++) {
			str.append(*it).append((it + 1 != v.end()) ? delimeter : "");
		}
		return str;
	}
	string r_dialog(const string& r_, const string& a_ = "", const string& b_ = "", const string& c_ = "", const string& d_ = "") {
		return "text_scaling_string|Dirttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt|\nset_default_color|`o\nadd_label_with_icon|big|`wGet a TayoPsID``|left|206|\nadd_spacer|small|\nadd_textbox|" + (r_.empty() ? "By choosing a `wTayoPsID``, you can use a name and password to logon from any device.Your `wname`` will be shown to other players!" : r_) + "|left|\nadd_spacer|small|\nadd_text_input|logon|Name|" + a_ + "|18|\nadd_textbox|Your `wpassword`` must contain `w8 to 18 characters, 1 letter, 1 number`` and `w1 special character: @#!$^&*.,``|left|\nadd_text_input_password|password|Password|" + b_ + "|18|\nadd_text_input_password|password_verify|Password Verify|" + c_ + "|18|\nadd_textbox|Your `wemail`` will only be used for account verification and support. If you enter a fake email, you can't verify your account, recover or change your password.|left|\nadd_text_input|email|Email|" + d_ + "|64|\nadd_textbox|We will never ask you for your password or email, never share it with anyone!|left|\nend_dialog|growid_apply|Cancel|Get My TayoPsID!|\n";
	}
	string to_playmod_time(int seconds) {
		int day = seconds / (24 * 3600);
		seconds = seconds % (24 * 3600);
		int hour = seconds / 3600;
		seconds %= 3600;
		int minute = seconds / 60;
		seconds %= 60;
		int second = seconds;
		if (day == 0 and hour == 0 and minute == 0 and second == 0) return "Removing now ";
		return (day > 0 ? to_string(day) + " days" : "") + (hour > 0 ? (day > 0 ? ", " : "") + to_string(hour) + " hours" : "") + (minute > 0 ? (hour > 0 ? ", " : "") + to_string(minute) + " mins" : "") + (second > 0 ? (minute > 0 ? ", " : "") + to_string(second) + " secs " : " ");
	}
	string detailSecond(int seconds) {
		int day = seconds / (24 * 3600);
		seconds = seconds % (24 * 3600);
		int hour = seconds / 3600;
		seconds %= 3600;
		int minute = seconds / 60;
		seconds %= 60;
		int second = seconds;

		return (day > 0 ? to_string(day) + " Days" : "") + (hour > 0 ? (day > 0 ? ", " : "") + to_string(hour) + " Hours" : "") + (minute > 0 ? (hour > 0 ? ", " : "") + to_string(minute) + " Minutes" : "") + (second > 0 ? (minute > 0 ? ", " : "") + to_string(second) + " Seconds " : " ");
	}
	string detailMSTime(int milliseconds) {
		long hr = milliseconds / 3600000;
		milliseconds = milliseconds - 3600000 * hr;
		long min = milliseconds / 60000;
		milliseconds = milliseconds - 60000 * min;
		long sec = milliseconds / 1000;
		milliseconds = milliseconds - 1000 * sec;
		return (hr > 0 ? to_string(hr) + ":" : "") + to_string(min) + ":" + (sec >= 10 ? to_string(sec) + "." : "0" + to_string(sec) + ".") + to_string(milliseconds / 10);
	}
	string second_to_time(std::int64_t n) {
		int second = n % 60;
		n /= 60;

		int minute = n % 60;
		n /= 60;

		int hour = n % 24;
		n /= 24;

		int day = n;

		string str;
		if (day != 0) str.append(to_string(day) + " days ");

		if (hour != 0) str.append(to_string(hour) + " hours ");

		if (minute != 0) str.append(to_string(minute) + " minutes ");

		if (second != 0) str.append(to_string(second) + " seconds ");

		return str;
	}
	int rgb_to_int(int const& r, int const& g, int const& b, int const& a = 255) {
		return (b << 24) | (g << 16) | (r << 8) | a;
	}
	bool is_number(string const& str) {
		return regex_match(str, regex("^[0-9]+$"));
	}
	bool contains_regex(string const& str, string const& regex) {
		std::regex r{ regex };
		return regex_match(str, r);
	}
	void modify_json(string const& file, std::function<void(json&)> fn) {
		try {
			ifstream istream(file);
			json j;
			istream >> j;

			fn(j);

			ofstream ostream(file);
			ostream << j;
		}
		catch (nlohmann::detail::parse_error&) { return; }
	}
	void filter_color(string& str) {
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '`') str.erase(i, 2);
		}
	}
	void log_text(string const& file, string const& content) {
		ofstream ostream("database/server_logs/" + file + ".txt", ios::app);
		ostream << decode_special_char(content) << endl;
	}
	void RemovePeerAFDuration(ENetPeer* peer) {
		if (pInfo(peer)->Cheat_AF_Since != 0) {
			int DurationUsed = get_epoch_seconds() - pInfo(peer)->Cheat_AF_Since;
			int result = pInfo(peer)->Cheat_AF_Time - DurationUsed;
			pInfo(peer)->Cheat_AF_Time = result;
			pInfo(peer)->Cheat_AF_Since = 0;
		}
	}

	void OnSuperMain(ENetPeer* peer, int const& itemHash) {
		gamepacket_t packet;
		packet.Insert("OnSuperMainStartAcceptLogonHrdxs47254722215a");
		packet.Insert(itemHash);
		packet.Insert("www.growtopia1.com");
		packet.Insert("cache/");
		packet.Insert("launcher.powerkuy.growlauncer cc.cz.madkite.freedom org.aqua.gg idv.aqua.bulldog com.cih.gamecih2 com.cih.gamecih com.cih.game_cih cn.maocai.gamekiller com.gmd.speedtime org.dax.attack com.x0.strai.frep com.x0.strai.free org.cheatengine.cegui org.sbtools.gamehack com.skgames.traffikrider org.sbtoods.gamehaca com.skype.ralder org.cheatengine.cegui.xx.multi1458919170111 com.prohiro.macro me.autotouch.autotouch com.cygery.repetitouch.free com.cygery.repetitouch.pro com.proziro.zacro com.slash.gamebuster");
		string pay_status = (pInfo(peer)->supp >= 1 ? "1" : "0");
		packet.Insert("proto=206|choosemusic=audio/ogg/tayo.ogg|active_holiday=9|wing_week_day=0|ubi_week_day=0|server_tick=153732843|clash_active=0|dropeerlavacheck_faster=1|isPayingUser=" + pay_status + "|usingStoreNavigation=1|enableInventoryTab=1|bigBackpack=1|");
		//PLAYER_TRIBUTE hash
		packet.CreatePacket(peer);
	}
	class DialogReturnParser {
		std::vector<std::pair<std::string, std::string>> contents;
	public:
		DialogReturnParser(std::string const& str) {
			for (std::string const& line : explode("\n", str)) {
				std::vector<std::string> lineContents = explode("|", line);
				if (lineContents.size() < 2) continue;

				contents.emplace_back(lineContents[0], lineContents[1]);
			}
		}
		std::pair<std::string, std::string> get_value_starts_with(std::string const& key) {
			for (auto const& [contentKey, value] : contents) {
				if (contentKey.starts_with(key)) return { contentKey, value };
			}
			return {};
		}
		std::string get_value(std::string const& key) {
			for (auto const& [contentKey, value] : contents) {
				if (contentKey == key) return value;
			}
			return "";
		}
	};
	void set_peer_pos(ENetPeer* peer, int const& posX, int const& posY, int const& netID) {
		gamepacket_t packet(0, netID);
		packet.Insert("OnSetPos");
		packet.Insert(posX, posY);
		packet.CreatePacket(peer);
	}
	void set_pos(ENetPeer* peer, const CL_Vec2f& pos, int netID) {
		gamepacket_t packet(0, netID);
		packet.Insert("OnSetPos");
		packet.Insert(pos.x, pos.y);
		packet.CreatePacket(peer);
	}
	void send_dialog(ENetPeer* peer, string const& dialog, int const& delay = 0) {
		gamepacket_t packet(delay);
		packet.Insert("OnDialogRequest");
		packet.Insert(dialog);
		packet.CreatePacket(peer);
	}
	void send_console(ENetPeer* peer, string const& message) {
		gamepacket_t packet;
		packet.Insert("OnConsoleMessage");
		packet.Insert(message);
		packet.CreatePacket(peer);
	}
	void send_overlay(ENetPeer* peer, string const& message) {
		gamepacket_t packet;
		packet.Insert("OnTextOverlay");
		packet.Insert(message);
		packet.CreatePacket(peer);
	}
	void send_bubble(ENetPeer* peer, int const& netID, string const& message) {
		gamepacket_t packet;
		packet.Insert("OnTalkBubble");
		packet.Insert(netID);
		packet.Insert(message);
		packet.Insert(0), packet.Insert(0);
		packet.CreatePacket(peer);
	}
	void send_console_message(ENetPeer* peer, std::string const& message) {
		gamepacket_t p;
		p.Insert("OnConsoleMessage");
		p.Insert(message);
		p.CreatePacket(peer);
	}
	void add_notif(ENetPeer* peer, string const& text) {
		gamepacket_t packet;
		packet.Insert("OnAddNotification");
		packet.Insert("interface/atomic_button.rttex");
		packet.Insert(text);
		packet.Insert("audio/hub_open.wav");
		packet.Insert(0);
	}
	void avatar_remove(ENetPeer* peer, int const& netID) {
		gamepacket_t packet;
		packet.Insert("OnRemove");
		packet.Insert("netID|" + to_string(netID) + "\n");
		packet.CreatePacket(peer);
	}
	void send_countdown(ENetPeer* peer, int const& netID, int const& seconds) {
		// SOON LUPA NGENTOT
	}
	void send_audio_pos(ENetPeer* peer, int const& posNetID, string const& audioFileName, int const& delay = 0) {
		gamepacket_t packet(delay, posNetID);
		packet.Insert("OnPlayPositioned");
		packet.Insert("audio/" + audioFileName + ".wav");
		packet.CreatePacket(peer);
	}
	int converttoint(string number) {
		float gemamo = 0;
		regex pattern("[0-9]+\\.?[0-9]*[mkMKbB]?");
		if (regex_match(number, pattern)) {
			int tomulti = 0;
			string rn = number; // 100000
			if (rn.back() == 'm' or rn.back() == 'M') tomulti = 1000000;
			else if (rn.back() == 'k' or rn.back() == 'K') tomulti = 1000;
			else if (rn.back() == 'b' or rn.back() == 'B') tomulti = 100000000;
			else gemamo = stof(number);
			if (tomulti != 0) {
				string ff = rn.substr(0, rn.size() - 1);
				gemamo = stof(ff) * tomulti;
			}
		}
		return round(gemamo);
	}
	void set_bux(ENetPeer* peer) {
		if (pInfo(peer)->gems < 0) {
			ENetPacket* send_notif = Variant{ "OnAddNotification" }.push("interface/cash_icon_overlay.rttex", "You have reached max gems, please use it now!", "audio/hub_open.wav", 0).pack(); enet_peer_send(peer, 0, send_notif);
			pInfo(peer)->gems = 2147483647;
		}
		Variant variant = "OnSetBux";
		variant.push(pInfo(peer)->gems, 0, static_cast<int>(pInfo(peer)->supp >= 1));
		if (pInfo(peer)->supp >= 2) variant.push((float)GetServerTime(), 1.f, 0.f);
		enet_peer_send(peer, 0, variant.pack());
	}
	inline constexpr uint32_t quick_hash(const std::string_view& data) {
		uint32_t hash = 5381;
		for (const auto& c : data)
			hash = ((hash << 5) + hash) + c;
		return hash;
	}
	void send_modlogs(string const& who, int const& uid, string const& ngapain, string const& kesiapa, int const& uidsiapa) {
		gamepacket_t packet;
		packet.Insert("OnConsoleMessage");
		packet.Insert(format("CT:[FC]_>> `5>> MOD-LOG: `o{} (ID: {}) {} {} (ID: {})", who, uid, ngapain, kesiapa, uidsiapa));
		for (ENetPeer* currentPeer = server->peers; currentPeer < &server->peers[server->peerCount]; ++currentPeer) {
			if (currentPeer->state != ENET_PEER_STATE_CONNECTED or currentPeer->data == NULL) continue;
			if (pInfo(currentPeer)->mod + pInfo(currentPeer)->supermod + pInfo(currentPeer)->admin + pInfo(currentPeer)->dev == 0) continue;
			packet.CreatePacket(currentPeer);
		}
	}
}

namespace player::algorithm {
	std::string get_name(ENetPeer* peer) {
		if (peer == nullptr || peer->data == nullptr) return "";

		try {
			return pInfo(peer)->d_name.empty() ? pInfo(peer)->name_color + pInfo(peer)->tankIDName : pInfo(peer)->d_name;
		}
		catch (...) { return ""; }
	}
}

constexpr uint32_t operator "" _qh(const char* str, std::size_t len) {
	return Algorithm::quick_hash(std::string_view{ str, len });
}

namespace editjson {
	namespace nxr {

		void add_untrade_newjson(ENetPeer* peer, const json& newItem, json& jsonData) {
			auto itemId = newItem["ItemId"];
			auto existingItem = std::find_if(jsonData.begin(), jsonData.end(),
				[itemId](const json& item) { return item["ItemId"] == itemId; });

			if (existingItem == jsonData.end()) {
				jsonData.push_back(newItem);
				std::ofstream fileOutput("./database/config/untradeableitem.json");
				fileOutput << std::setw(4) << jsonData << std::endl;
				fileOutput.close();
				Algorithm::send_overlay(peer, "`2[ `0Successfully edit Untradeable Item! `2]");
			}
			else {
				Algorithm::send_overlay(peer, "The floater " + to_string(itemId) + " already exists.");
				Algorithm::send_console(peer, "`2>> `cItemId with Name " + items[itemId].ori_name + " already exists.");
			}
		}
	}
}