//
// Created by JaneZ on 2025/5/5.
//

#ifndef USER_H
#define USER_H
#include "../Data Structure/String.h"
#include "../Data Structure/BPT.h"
#include "../Data Structure/map.h"

using ull = unsigned long long int;

struct UserInfo {
    JaneZ::String<22> username;
    JaneZ::String<32> password;
    JaneZ::String<22> name;
    JaneZ::String<32> mailAddr;
    int privilege;

    UserInfo():privilege(10){}

    UserInfo(const char* u,const char *p,const char* n,const char *m,int pr):username(u),password(p),name(n),mailAddr(m),privilege(pr) {}

    UserInfo(const UserInfo& other) = default;

    bool operator==(const UserInfo &other) const {
        return username == other.username;
    }

    bool operator<(const UserInfo &other) const {
        return username < other.username;
    }

    bool operator<=(const UserInfo &other) const {
        return username <= other.username;
    }

    bool operator>(const UserInfo &other) const {
        return username > other.username;
    }

    bool operator>=(const UserInfo &other) const {
        return username >= other.username;
    }
};

struct UB {
    UserInfo Info;
    bool canBeOperated;
};

class UserSystem {
private:
    BPT<ull,UserInfo> UserBase;
    sjtu::map<ull,int> LoginStack;

public:
    bool add_user(JaneZ::String<22> &currentUsername,JaneZ::String<22> &username,JaneZ::String<32> &password,JaneZ::String<22> &name,JaneZ::String<32> &mailAddr,int privilege);

    bool login(JaneZ::String<22> &username,JaneZ::String<32> &password);

    bool logout(JaneZ::String<22> &username);

    UB query_profile(JaneZ::String<22> &currentUsername,JaneZ::String<22> &username);

    UB modify_profile(JaneZ::String<22> &currentUsername,JaneZ::String<22> &username,JaneZ::String<32> &password,JaneZ::String<22> &name,JaneZ::String<32> &mailAddr,int privilege);
};
#endif //USER_H