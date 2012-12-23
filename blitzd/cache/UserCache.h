#ifndef _USERCACHE_H_
#define _USERCACHE_H_

#include "utils/SharedPtr.h"
#include "utils/Memory.h"
#include "utils/Stream.h"
#include "core/BNetDefs.h"
#include "CacheFunc.h"

namespace Core
{
	class Client;
}

namespace Cache
{
	class UserCacheItem:
		public Utils::SharedClass<UserCacheItem>
	{
	public:
		struct Friend_t
		{
			bool mutual;
			UserCacheItem::Pointer user;
		};
	public:
		UserCacheItem(const char *);

		inline bool IsExist() { return _id > 0; }
		inline bool IsDirty() { return _dirty; }
		inline void SetDirty(bool d) { _dirty = d; }
		inline bool IsFriendDirty() { return _friend_dirty; }
		inline void SetFriendDirty(bool d) { _friend_dirty = d; }
		inline std::string GetUsername() { return _username; }

		inline uint GetUserId() { return _id; }
		inline void SetUserId(uint id) { _id = id; }

		inline const byte * GetSalt() { return _salt; }
		void SetSalt(const byte* s);
		inline const byte * GetVerifier() { return _verifier; }
		void SetVerifier(const byte * s);
		inline const byte * GetPasswd() { return _passwd; }
		void SetPasswd(const byte * p);

		inline std::string GetMail() { return _mail; }
		void SetMail(std::string);

		inline std::string GetData(std::string k) { return _data_map[k]; }
		void SetData(std::string, std::string);
		void SetDataMap(const byte *, uint size);
		void BuildDataMap(Utils::Stream& st);

		inline uint GetFlag() { return _flag & 0xFFFFFFF; }
		inline uint GetFullFlag() { return _flag; }
		void SetFlag(uint f);
		inline void AddFlag(uint f) { SetFlag(_flag | f); }
		inline void DelFlag(uint f) { SetFlag(_flag & ~f); }

		int AddFriend(UserCacheItem::Pointer user, int *mutIdx = NULL);
		bool MoveFriend(uint idx1, uint idx2);
		bool RemoveFriend(uint idx);
		bool RemoveFriend(UserCacheItem * user);
		bool RemoveFriend(std::string name);
		int FindFriend(UserCacheItem * user);
		int FindFriend(std::string name);
		uint GetFriendCount();
		Friend_t* GetFriend(uint index);
		void FriendSendInfo(std::string text, bool mutual = true);
		void FriendWhisper(Core::Client* from, std::string text, bool mutual = true);
	private:
		bool _dirty;
		uint _id;
		std::string _username;
		std::string _mail;
		std::string _description;
		std::string _location;
		byte _salt[32];
		byte _verifier[32];
		byte _passwd[20];
		uint _flag;
		bool _friend_dirty;
		std::vector<Friend_t> _friend_list;

		typedef std::map<std::string, std::string, CompareInCaseLess> data_map_t;
		data_map_t _data_map;
	};

	class UserCache
	{
	public:
		UserCache();
		inline void SetNextUId(uint n) {_next_uid = n;}
		UserCacheItem::Pointer operator[] (std::string);
		UserCacheItem::Pointer operator[] (uint);

		UserCacheItem::Pointer Add(const char *, uint = 0);
		UserCacheItem::Pointer Take(uint);
		UserCacheItem::Pointer Take(const char *);
		void Remove(uint);
		void Remove(const char *);
		inline size_t Count() {return _id_map.size();}

		void LoadFromDB();
		void SaveToDB();

		inline uint GetCount() {return _count;}

	private:
		UserCacheItem::Pointer LoadUser(const char*, uint = 0);

	private:
		uint _next_uid;
		uint _count;

		typedef std::unordered_map<std::string, UserCacheItem::Pointer, LowerCaseHash, CompareInCaseEqual> InCaseStringMap_t;
		typedef std::unordered_map<uint, UserCacheItem::Pointer> UIntMap_t;
		InCaseStringMap_t _name_map;
		UIntMap_t _id_map;
	};
	extern UserCache userCache;
}

#endif // _USERCACHE_H_
