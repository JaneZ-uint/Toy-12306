//
// Created by JaneZ on 2025/5/5.
//

#ifndef USER_H
#define USER_H
#include "../Data Structure/String.h"
#include "../Data Structure/BPT.h"
#include "../Data Structure/map.h"
#include "../Tool/HashFunction.h"
#include "../Tool/MemoryRiver.h"

using ull = unsigned long long int;

struct UserInfo {
    JaneZ::String<22> username;
    JaneZ::String<32> password;
    JaneZ::String<22> name;
    JaneZ::String<32> mailAddr;
    int privilege = -1;
    //ull HashUsername = JaneZ::Hash<22>::HashFunction(username);

    UserInfo(): privilege(-1) {
    }

    UserInfo(const char *u, const char *p, const char *n, const char *m, int pr): username(u), password(p), name(n),
        mailAddr(m), privilege(pr) {
    }

    UserInfo(const UserInfo &other) = default;

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
    friend class TicketSystem;

private:
    BPT<ull, int> UserBase;
    MemoryRiver<UserInfo, 1> UserFile;
    sjtu::map<ull, bool> LoginStack;
    int total = 0;

public:
    UserSystem(): UserBase("UserBaseIndex", "UserBaseLeaf") {
        UserFile.initialise("user_info.txt");
        UserFile.get_info(total, 1);
    }

    ~UserSystem() {
        UserFile.write_info(total, 1);
    }

    bool add_user(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username, JaneZ::String<32> &password,
                  JaneZ::String<22> &name, JaneZ::String<32> &mailAddr, int privilege);

    bool login(JaneZ::String<22> &username, JaneZ::String<32> &password);

    bool logout(JaneZ::String<22> &username);

    UB query_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username);

    UB modify_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username, JaneZ::String<32> &password,
                      JaneZ::String<22> &name, JaneZ::String<32> &mailAddr, int privilege);

    void clean() {
        // 删除用户数据的索引和叶子文件
        std::remove("UserBaseIndex");
        std::remove("UserBaseLeaf");

        LoginStack.clear();
    }
};
#endif //USER_H
