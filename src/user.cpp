//
// Created by JaneZ on 2025/5/5.
//
using ull = unsigned long long int;
#include "../include/user.h"

bool UserSystem::add_user(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username, JaneZ::String<32> &password,
                          JaneZ::String<22> &name, JaneZ::String<32> &mailAddr, int privilege) {
    if (UserBase.empty()) {
        ull HashUsername = JaneZ::Hash<22>::HashFunction(username);
        //LoginStack.insert({HashUsername,true});不代表已经登录
        UserInfo firstUser;
        firstUser.username = username;
        firstUser.password = password;
        firstUser.name = name;
        firstUser.mailAddr = mailAddr;
        firstUser.privilege = 10;
        UserBase.insert(HashUsername, total);
        UserFile.write(firstUser, total);
        ++total;
        return true;
    } else {
        ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
        if (LoginStack.find(HashCur) == LoginStack.end()) {
            return false;
        }
        sjtu::vector<int> result = UserBase.find(HashCur);
        UserInfo current;
        UserFile.read(current, result[0]);
        if (current.privilege <= privilege) {
            return false;
        }
        ull HashNow = JaneZ::Hash<22>::HashFunction(username);
        sjtu::vector<int> tmp = UserBase.find(HashNow);
        if (!tmp.empty()) {
            return false;
        }
        UserInfo info;
        info.username = username;
        info.password = password;
        info.name = name;
        info.mailAddr = mailAddr;
        info.privilege = privilege;
        UserBase.insert(HashNow, total);
        UserFile.write(info, total);
        ++total;
        return true;
    }
}

bool UserSystem::login(JaneZ::String<22> &username, JaneZ::String<32> &password) {
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    if (LoginStack.find(HashNow) == LoginStack.end()) {
        sjtu::vector<int> result = UserBase.find(HashNow);
        if (result.empty()) {
            return false;
        }
        UserInfo current;
        UserFile.read(current, result[0]);
        if (current.password == password) {
            LoginStack.insert({HashNow, true});
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool UserSystem::logout(JaneZ::String<22> &username) {
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    if (LoginStack.find(HashNow) == LoginStack.end()) {
        return false;
    }
    /*sjtu::vector<int> result = UserBase.find(HashNow);
    if(result.empty()) {
        return false;
    }*/
    LoginStack.erase(LoginStack.find(HashNow));
    return true;
}

UB UserSystem::query_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username) {
    ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
    UB Query;
    if (LoginStack.find(HashCur) == LoginStack.end()) {
        Query.canBeOperated = false;
        return Query;
    }
    sjtu::vector<int> resultCur = UserBase.find(HashCur);
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    sjtu::vector<int> result = UserBase.find(HashNow);
    if (result.empty()) {
        Query.canBeOperated = false;
        return Query;
    }
    UserInfo curInfo;
    UserFile.read(curInfo, resultCur[0]);
    UserInfo info;
    UserFile.read(info, result[0]);
    if (curInfo.privilege < info.privilege) {
        Query.canBeOperated = false;
        return Query;
    } else if (curInfo.privilege == info.privilege && curInfo.username != info.username) {
        Query.canBeOperated = false;
        return Query;
    } //-c 的权限大于 -u 的权限，或是 -c 和 -u 相同」!!!(不是权限相同，是同一个号的意思
    Query.Info.username = username;
    Query.Info.password = info.password;
    Query.Info.name = info.name;
    Query.Info.mailAddr = info.mailAddr;
    Query.Info.privilege = info.privilege;
    Query.canBeOperated = true;
    return Query;
}

UB UserSystem::modify_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username,
                              JaneZ::String<32> &password, JaneZ::String<22> &name, JaneZ::String<32> &mailAddr,
                              int privilege) {
    ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
    UB Query;
    if (LoginStack.find(HashCur) == LoginStack.end()) {
        Query.canBeOperated = false;
        return Query;
    }
    sjtu::vector<int> resultCur = UserBase.find(HashCur);
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    sjtu::vector<int> result = UserBase.find(HashNow);
    if (result.empty()) {
        Query.canBeOperated = false;
        return Query;
    }
    UserInfo curInfo;
    UserFile.read(curInfo, resultCur[0]);
    UserInfo info;
    UserFile.read(info, result[0]);
    if (curInfo.privilege < info.privilege) {
        Query.canBeOperated = false;
        return Query;
    } else if (curInfo.privilege == info.privilege && curInfo.username != info.username) {
        Query.canBeOperated = false;
        return Query;
    } //-c 的权限大于 -u 的权限，或是 -c 和 -u 相同」!!!(不是权限相同，是同一个号的意思
    if (privilege >= curInfo.privilege) {
        Query.canBeOperated = false;
        return Query;
    } // -g 需低于 -c 的权限
    Query.Info.username = username;
    if (password != "\0") {
        Query.Info.password = password;
        info.password = password;
    } else {
        Query.Info.password = info.password;
    }
    if (name != "\0") {
        Query.Info.name = name;
        info.name = name;
    } else {
        Query.Info.name = info.name;
    }
    if (mailAddr != "\0") {
        Query.Info.mailAddr = mailAddr;
        info.mailAddr = mailAddr;
    } else {
        Query.Info.mailAddr = info.mailAddr;
    }
    if (privilege != -1) {
        Query.Info.privilege = privilege;
        info.privilege = privilege;
    } else {
        Query.Info.privilege = info.privilege;
    }
    Query.canBeOperated = true;
    /*UserBase.erase(HashNow,result[0]);
    UserBase.insert(HashNow,Query.Info);*/
    UserFile.write(info, result[0]); //写回原文件中
    return Query;
}
