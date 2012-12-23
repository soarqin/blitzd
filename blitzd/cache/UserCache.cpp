#include "Config.h"
#include "UserCache.h"
#include "core/Cfg.h"
#include "core/Message.h"
#include "core/Client.h"
#include "storage/StorageSqlite.h"

namespace Cache
{
	UserCache userCache;

	UserCacheItem::UserCacheItem( const char * username ):
		_dirty(false), _id(0), _username(username), _flag(0), _friend_dirty(false)
	{
		memset(_salt, 0, 32);
		memset(_verifier, 0, 32);
	}

	void UserCacheItem::SetSalt( const byte* s )
	{
		if(memcmp(_salt, s, 32) == 0)
			return;
		_dirty = true;
		memcpy(_salt, s, 32);
	}

	void UserCacheItem::SetVerifier( const byte * s )
	{
		if(memcmp(_verifier, s, 32) == 0)
			return;
		_dirty = true;
		memcpy(_verifier, s, 32);
	}

	void UserCacheItem::SetPasswd( const byte * p )
	{
		if(memcmp(_passwd, p, 20) == 0)
			return;
		_dirty = true;
		memcpy(_passwd, p, 20);
	}

	void UserCacheItem::SetMail( std::string m )
	{
		if(_mail == m)
			return;
		_mail = m;
		_dirty = true;
	}

	void UserCacheItem::SetFlag( uint f )
	{
		if(_flag == f)
			return;
		_flag = f;
		_dirty = true;
	}

	void UserCacheItem::SetData( std::string k, std::string v )
	{
		std::string& modstr = _data_map[k];
		if(modstr == v)
			return;
		modstr = v;
		_dirty = true;
	}

	void UserCacheItem::SetDataMap( const byte * data, uint size )
	{
		const char * d = (const char *)data;
		uint p = 0;
		while(p < size)
		{
			uint l = (uint)strnlen(d + p, size - p);
			uint e = p + l;
			for(uint i = p; i < e; i ++)
			{
				if(d[i] == '=')
				{
					_data_map[std::string(d + p, d + i)] = std::string(d + i + 1, d + e);
					break;
				}
			}
			p = e + 1;
		}
	}

	void UserCacheItem::BuildDataMap( Utils::Stream& st )
	{
		data_map_t::iterator it;
		for(it = _data_map.begin(); it != _data_map.end(); ++ it)
		{
			if(it->second.empty())
				continue;
			std::string single = it->first + '=' + it->second;
			st << single;
		}
	}

	void UserCacheItem::FriendSendInfo(std::string text, bool mutual)
	{
		std::set<Core::Client *> toset;
		for(uint i = 0; i < _friend_list.size(); i ++)
		{
			Friend_t& fr = _friend_list[i];
			Core::Client * c;
			if((!mutual || fr.mutual) && fr.user.get() != NULL && (c = Core::clientPool[fr.user->GetUsername()]) != NULL)
			{
				toset.insert(c);
			}
		}
		if(!toset.empty())
			Core::Message::Send(toset, NULL, Core::Message::Info, text, false);
	}

	void UserCacheItem::FriendWhisper(Core::Client* from, std::string text, bool mutual)
	{
		std::set<Core::Client *> toset;
		for(uint i = 0; i < _friend_list.size(); i ++)
		{
			Friend_t& fr = _friend_list[i];
			Core::Client * c;
			if((!mutual || fr.mutual) && fr.user.get() != NULL && (c = Core::clientPool[fr.user->GetUsername()]) != NULL)
			{
				toset.insert(c);
			}
		}
		if(!toset.empty())
			Core::Message::Send(toset, from, Core::Message::Whisper, text, false);
	}

	int UserCacheItem::AddFriend( UserCacheItem::Pointer user, int *mutIdx /*= NULL*/ )
	{
		if(user.get() == NULL || user.get() == this)
		{
			return -1;
		}
		Friend_t fr;
		int idx1 = FindFriend(user.get());
		if(idx1 >= 0)
			return idx1;
		if(_friend_list.size() > 20)
			return -1;
		int idx2 = user->FindFriend(this);
		if(idx2 >= 0)
		{
			if(mutIdx != NULL)
			{
				*mutIdx = idx2;
			}
			user->GetFriend(idx2)->mutual = true;
		}
		else
		{
			if(mutIdx != NULL)
			{
				*mutIdx = -1;
			}
		}
		fr.mutual = (idx2 >= 0);
		fr.user = user;
		_friend_list.push_back(fr);
		_friend_dirty = true;
		return _friend_list.size() - 1;
	}

	bool UserCacheItem::MoveFriend( uint idx1, uint idx2 )
	{
		if(idx1 < 0 || idx1 >= _friend_list.size())
			return false;
		if(idx2 < 0 || idx2 >= _friend_list.size())
			return false;
		if(idx1 == idx2)
			return true;
		Friend_t& fr = _friend_list[idx1];
		if(idx1 < idx2)
		{
			for(uint i = idx1; i < idx2; i ++)
			{
				_friend_list[i] = _friend_list[i + 1];
			}
		}
		else
		{
			for(uint i = idx1; i > idx2; i --)
			{
				_friend_list[i] = _friend_list[i - 1];
			}
		}
		_friend_list[idx2] = fr;
		_friend_dirty = true;
		return true;
	}

	bool UserCacheItem::RemoveFriend( uint idx )
	{
		if(idx < 0 || idx >= _friend_list.size())
			return false;
		if(_friend_list.size() > idx + 1)
		{
			for(size_t i = _friend_list.size() - 1; i > idx; i --)
			{
				_friend_list[i - 1] = _friend_list[i];
			}
		}
		_friend_list.resize(_friend_list.size() - 1);
		_friend_dirty = true;
		return true;
	}

	bool UserCacheItem::RemoveFriend( UserCacheItem * user )
	{
		int index = FindFriend(user);
		if(index < 0)
			return false;
		return RemoveFriend(index);
	}

	bool UserCacheItem::RemoveFriend( std::string name )
	{
		int index = FindFriend(name);
		if(index < 0)
			return false;
		return RemoveFriend(index);
	}

	int UserCacheItem::FindFriend( UserCacheItem * user )
	{
		for(size_t i = 0; i < _friend_list.size(); i ++)
		{
			if(_friend_list[i].user == user)
				return i;
		}
		return -1;
	}

	int UserCacheItem::FindFriend( std::string name )
	{
		for(size_t i = 0; i < _friend_list.size(); i ++)
		{
			if(_STR.strcmpi(_friend_list[i].user->GetUsername().c_str(), name.c_str()) == 0)
				return i;
		}
		return -1;
	}

	uint UserCacheItem::GetFriendCount()
	{
		return _friend_list.size();
	}

	UserCacheItem::Friend_t* UserCacheItem::GetFriend( uint index )
	{
		if(index < 0 || index >= _friend_list.size())
			return NULL;
		return &_friend_list[index];
	}

	UserCache::UserCache(): _next_uid(1), _count(0)
	{
		_name_map.rehash(Core::cfg.GetHashtableSize());
		_id_map.rehash(Core::cfg.GetHashtableSize());
	}

	UserCacheItem::Pointer UserCache::Add( const char * name, uint uid )
	{
		UserCacheItem * item_ = new UserCacheItem(name);
		if(item_ == NULL)
			return NULL;
		UserCacheItem::Pointer item(item_);
		if(uid == 0)
		{
			uid = _next_uid;
			++ _next_uid;
		}
		else
		{
			if(uid >= _next_uid)
				_next_uid = uid + 1;
		}

		item->SetUserId(uid);
		if(uid > 0)
		{
			_count ++;
			_id_map[uid] = item;
		}
		_name_map[name] = item;
		return item;
	}

	UserCacheItem::Pointer UserCache::Take( uint id )
	{
		if(id == 0)
			return UserCacheItem::Pointer();
		UIntMap_t::iterator it = _id_map.find(id);
		if(it == _id_map.end())
			return UserCacheItem::Pointer();
		_count --;
		UserCacheItem::Pointer ptr = it->second;
		_name_map.erase(ptr->GetUsername());
		_id_map.erase(it);

		return ptr;
	}

	UserCacheItem::Pointer UserCache::Take( const char * name )
	{
		InCaseStringMap_t::iterator it = _name_map.find(name);
		if(it == _name_map.end())
			return UserCacheItem::Pointer();
		UserCacheItem::Pointer ptr = it->second;
		if(ptr->GetUserId() > 0)
		{
			_count --;
			_id_map.erase(ptr->GetUserId());
		}
		_name_map.erase(it);
		return ptr;
	}

	void UserCache::Remove( uint id )
	{
		if(id == 0)
			return;
		UIntMap_t::iterator it = _id_map.find(id);
		if(it == _id_map.end())
			return;
		_count --;
		_name_map.erase(it->second->GetUsername());
		_id_map.erase(it);
	}

	void UserCache::Remove( const char * name )
	{
		InCaseStringMap_t::iterator it = _name_map.find(name);
		if(it == _name_map.end())
			return;
		if(it->second->GetUserId() > 0)
		{
			_count --;
			_id_map.erase(it->second->GetUserId());
		}
		_name_map.erase(it);
	}

	UserCacheItem::Pointer UserCache::operator[]( std::string name )
	{
		InCaseStringMap_t::iterator it = _name_map.find(name);
		if(it == _name_map.end())
			return LoadUser(name.c_str());
		if(!it->second->IsExist())
			return UserCacheItem::Pointer();
		return it->second;
	}

	UserCacheItem::Pointer UserCache::operator[]( uint id )
	{
		UIntMap_t::iterator it = _id_map.find(id);
		if(it == _id_map.end())
			return LoadUser(NULL, id);
		if(!it->second->IsExist())
			return UserCacheItem::Pointer();
		return it->second;
	}

	UserCacheItem::Pointer UserCache::LoadUser( const char* name, uint id )
	{
		std::vector<std::vector<Storage::Storage::DataDef> > result;
		const char * keys[] = {"userid", "username", "salt", "verifier", "passwd", "mail", "flag", "data", NULL};
		char whereexpr[128];
		if(name != NULL)
		{
			sprintf(whereexpr, "username=\"%s\"", name);
		}
		else
		{
			sprintf(whereexpr, "userid=%u", id);
		}
		Storage::storage->GetMatch("users", keys, whereexpr, result);
		size_t c = result.size();
		if(c > 1)
		{
			Storage::storage->RemoveMatch("users", whereexpr);
		}
		else if(c == 1)
		{
			std::vector<Storage::Storage::DataDef>& row = result[0];
			LOG_DEBUG(("Loaded %s", row[1].sVal));
			uint user_id = (uint)row[0].lVal;
			UserCacheItem::Pointer ptr = Add(row[1].sVal, user_id);
			if(row[2].bSize >= 32)
				ptr->SetSalt(row[2].bVal);
			if(row[3].bSize >= 32)
				ptr->SetVerifier(row[3].bVal);
			if(row[4].bSize >= 20)
				ptr->SetPasswd(row[4].bVal);
			ptr->SetMail(row[5].sVal);
			ptr->SetFlag((uint)row[6].lVal);
			ptr->SetDataMap(row[7].bVal, row[7].bSize);
			ptr->SetDirty(false);

			const char * fkeys[] = {"friendlist", NULL};
			result.clear();
			sprintf(whereexpr, "userid=%u", user_id);
			Storage::storage->GetMatch("friends", fkeys, whereexpr, result);
			c = result.size();
			if(c >= 1)
			{
				std::vector<Storage::Storage::DataDef>& row = result[0];
				const uint * data = (const uint *)row[0].bVal;
				c = row[0].bSize / sizeof(uint);
				for(uint i = 0; i < c; ++ i)
					ptr->AddFriend((*this)[data[i]], NULL);
			}
			return ptr;
		}
		UserCacheItem * item_ = new UserCacheItem(name);
		if(item_ == NULL)
			return NULL;
		UserCacheItem::Pointer item(item_);
		_name_map[name] = item;
		return UserCacheItem::Pointer();
	}

	void UserCache::LoadFromDB()
	{
		std::vector<std::vector<Storage::Storage::DataDef> > result;
		const char * keys[] = {"MAX(userid)", NULL};
		Storage::storage->GetAll("users", keys, result);
		size_t c = result.size();
		if(c >= 1)
		{
			std::vector<Storage::Storage::DataDef>& row = result[0];
			LOG_DEBUG(("Maximum user id is: %u", (uint)row[0].lVal));
			_next_uid = (uint)row[0].lVal + 1;
		}
		const char * keys2[] = {"COUNT(userid)", NULL};
		Storage::storage->GetAll("users", keys2, result);
		c = result.size();
		if(c >= 1)
		{
			std::vector<Storage::Storage::DataDef>& row = result[0];
			LOG_DEBUG(("User count is: %u", (uint)row[0].lVal));
			_count = (uint)row[0].lVal;
		}
	}

	void UserCache::SaveToDB()
	{
		UIntMap_t::iterator it;
		Storage::storage->Begin();
		for(it = _id_map.begin(); it != _id_map.end(); ++ it)
		{
			if(it->second.get() == NULL)
				continue;
			UserCacheItem * item = it->second.get();
			if(item->IsDirty())
			{
				item->SetDirty(false);
				Storage::Storage::DataDef df[8];
				df[0].SetInt(item->GetUserId());
				df[1].SetString(item->GetUsername().c_str());
				df[2].SetBinary(item->GetSalt(), 32);
				df[3].SetBinary(item->GetVerifier(), 32);
				df[4].SetBinary(item->GetPasswd(), 20);
				df[5].SetString(item->GetMail().c_str());
				df[6].SetInt(item->GetFullFlag());
				Utils::Stream buf;
				item->BuildDataMap(buf);
				df[7].SetBinary(buf, buf.size());
				const char * keys[] = {"userid", "username", "salt", "verifier", "passwd", "mail", "flag", "data", NULL};
				Storage::storage->Set("users", keys, df);
			}
			if(item->IsFriendDirty())
			{
				item->SetFriendDirty(false);
				Storage::Storage::DataDef df[2];
				df[0].SetInt(item->GetUserId());
				std::vector<uint> fridlist;
				for(uint i = 0; i < item->GetFriendCount(); ++ i)
				{
					UserCacheItem::Friend_t * fr = item->GetFriend(i);
					if(fr == NULL || fr->user.get() == NULL)
						continue;
					fridlist.push_back(fr->user->GetUserId());
				}
				if(fridlist.empty())
					df[1].SetBinary(NULL, 0);
				else
					df[1].SetBinary((const byte *)&fridlist[0], sizeof(uint) * fridlist.size());
				const char * keys[] = {"userid", "friendlist", NULL};
				Storage::storage->Set("friends", keys, df);
			}
		}
		Storage::storage->Commit();
	}
}
