# Toy-12306

## 项目简介

Ticket System 是SJTU CS1951（数据结构）课程的期末大作业，旨在实现一个类似[12306](https://www.12306.cn/index/)的火车票管理系统。
~但该系统具有明显缺陷：如不支持并发、不支持三线及以上换乘、学生票、车厢选座、各种舱型选座~（划掉😢）

## 功能特性

- 用户注册、登录、退出登录、查询信息、修改信息
- 列车添加、删除、发布
- 列车信息、车票查询
- 换乘信息查询
- 车票预订与取消
- 订单查询、退票

## 数据结构
主要采用带[LRU Cache](https://en.wikipedia.org/wiki/LRU)和空间回收的BPT、用AVL树实现的map、用左偏堆实现的priority_queue、用动态数组实现的vector以及普通文件流进行数据存储，具体实现分别在BPT.h、map.h、priority_queue.h、vector.h、MemoryRiver.h中。

## 项目框架
```
├── include
│   ├── BPT.h
│   ├── exceptions.h
│   ├── HashFunction.h
│   ├── list.h
│   ├── LRUCache.h
│   ├── map.h
│   ├── MemoryRiver.h
│   ├── priority_queue.h
│   ├── String.h
│   ├── ticket.h
│   ├── time.h
│   ├── tokenscanner.h
│   ├── tokenSlicer.h
│   ├── train.h
│   ├── user.h
│   ├── utility.h
│   ├── vector.h
├── src
│   ├── ticket.cpp
│   ├── time.cpp
│   ├── tokenscanner.cpp
│   ├── tokenSlicer.cpp
│   ├── train.cpp
│   ├── user.cpp
├── main.cpp
├── README.md
├── CMakeLists.txt
├── run-test
```

## Special Notice
Jane 在写完了tokenscanner类后悲剧地发现《所有输入格式均合法》，但不忍心删除它，于是请gitnore它😭