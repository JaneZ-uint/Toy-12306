//
// Created by JaneZ on 2025/5/5.
//
using ull = unsigned long long int;
#include "user.h"

bool UserSystem::add_user(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username, JaneZ::String<32> &password, JaneZ::String<22> &name, JaneZ::String<32> &mailAddr, int privilege) {
    if(UserBase.empty()) {
        ull HashUsername = JaneZ::Hash<22>::HashFunction(username);
        LoginStack.insert({HashUsername,true});
        UserInfo firstUser;
        firstUser.username = username;
        firstUser.password = password;
        firstUser.name = name;
        firstUser.mailAddr = mailAddr;
        firstUser.privilege = 10;
        UserBase.insert(HashUsername,firstUser);
        return true;
    }else {
        ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
        if(LoginStack.find(HashCur) == LoginStack.end()) {
            return false;
        }
        sjtu::vector<UserInfo> result = UserBase.find(HashCur);
        if(result[0].privilege <= privilege) {
            return false;
        }
        ull HashNow = JaneZ::Hash<22>::HashFunction(username);
        sjtu::vector<UserInfo> tmp = UserBase.find(HashNow);
        if(!tmp.empty()) {
            return false;
        }
        UserInfo info;
        info.username = username;
        info.password = password;
        info.name = name;
        info.mailAddr = mailAddr;
        info.privilege = privilege;
        UserBase.insert(HashNow,info);
        return true;
    }
}

bool UserSystem::login(JaneZ::String<22> &username, JaneZ::String<32> &password) {
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    if(LoginStack.find(HashNow) == LoginStack.end()) {
        sjtu::vector<UserInfo> result = UserBase.find(HashNow);
        if(result.empty()) {
            return false;
        }
        if(result[0].password == password) {
            LoginStack.insert({HashNow,true});
            return true;
        }else {
            return false;
        }
    }else {
        return false;
    }
}

bool UserSystem::logout(JaneZ::String<22> &username) {
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    if(LoginStack.find(HashNow) == LoginStack.end() || LoginStack.find(HashNow)->second == false) {
        return false;
    }
    sjtu::vector<UserInfo> result = UserBase.find(HashNow);
    if(result.empty()) {
        return false;
    }
    LoginStack.erase(LoginStack.find(HashNow));
    return true;
}

UB UserSystem::query_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username) {
    ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
    UB Query;
    if(LoginStack.find(HashCur) == LoginStack.end()) {
        Query.canBeOperated = false;
        return Query;
    }
    sjtu::vector<UserInfo> resultCur = UserBase.find(HashCur);
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    sjtu::vector<UserInfo> result = UserBase.find(HashNow);
    if(result.empty()) {
        Query.canBeOperated = false;
        return Query;
    }
    if(resultCur[0].privilege < result[0].privilege) {
        Query.canBeOperated = false;
        return Query;
    }
    Query.Info.username = username;
    Query.Info.password = result[0].password;
    Query.Info.name = result[0].name;
    Query.Info.mailAddr = result[0].mailAddr;
    Query.Info.privilege = result[0].privilege;
    Query.canBeOperated = true;
    return Query;
}

UB UserSystem::modify_profile(JaneZ::String<22> &currentUsername, JaneZ::String<22> &username, JaneZ::String<32> &password, JaneZ::String<22> &name, JaneZ::String<32> &mailAddr, int privilege) {
    ull HashCur = JaneZ::Hash<22>::HashFunction(currentUsername);
    UB Query;
    if(LoginStack.find(HashCur) == LoginStack.end()) {
        Query.canBeOperated = false;
        return Query;
    }
    sjtu::vector<UserInfo> resultCur = UserBase.find(HashCur);
    ull HashNow = JaneZ::Hash<22>::HashFunction(username);
    sjtu::vector<UserInfo> result = UserBase.find(HashNow);
    if(result.empty()) {
        Query.canBeOperated = false;
        return Query;
    }
    if(resultCur[0].privilege < result[0].privilege) {
        Query.canBeOperated = false;
        return Query;
    }
    Query.Info.username = username;
    if(password != "\0") {
        Query.Info.password = password;
    }else {
        Query.Info.password = result[0].password;
    }
    if(name != "\0") {
        Query.Info.name = name;
    }else {
        Query.Info.name = result[0].name;
    }
    if(mailAddr != "\0") {
        Query.Info.mailAddr = mailAddr;
    }else {
        Query.Info.mailAddr = result[0].mailAddr;
    }
    if(privilege != -1) {
        Query.Info.privilege = privilege;
    }else {
        Query.Info.privilege = result[0].privilege;
    }
    Query.canBeOperated = true;
    UserBase.erase(HashNow,result[0]);
    UserBase.insert(HashNow,Query.Info);
    return Query;
}
