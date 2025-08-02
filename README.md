# DBMS

This repository contains six C projects I developed for a **Data Structures course**.  
The goal was to build a lightweight, Redis-like NoSQL database engine on Linux from scratch.  
Each assignment gradually adds features like new data structures, command sets, and performance benchmarks.

---

## homework1 — Simple NoSQL Engine

Develop a minimal key-value store in **C**:

- Efficiently handles large volumes of key-value pairs  
- Supports full **CRUD** (Create, Read, Update, Delete)  
- Uses pointers & dynamic memory for data management  

---

### homework2 — Library + Benchmark

1. **Refactor** Assignment 1 into a reusable library / API.  
2. **Benchmark** program (C)  
   - **Bulk Insert**: generate ≥ 100 k unique key-value pairs, insert into *your* DB **and** open-source **Redis**; record latency & memory.  
   - **Latency Test**: average time for Create and random Read of ≥ 100 k records.  
   - **Memory Footprint**: compare total RAM used by both engines.  

---

## homework3 — List Commands

Extend the engine to support Redis-style **List** operations:  
`LPUSH • LPOP • RPUSH • RPOP • LLEN • LRANGE`

---

## homework4 — Tree Support

- Manage multiple trees; each node stores a key and pointer to its value.  
- Custom commands to **insert / search / delete** nodes in any tree.

---

## homework5 — Sorted Set

Add Redis-like **Sorted Set** commands:  
`ZADD, ZCARD, ZCOUNT, ZINTERSTORE, ZUNIONSTORE, ZRANGE,  
ZRANGEBYSCORE, ZRANK, ZREM, ZREMRANGE, ZREMRANGEBYSCORE`

---

## homework6 — Hash + Events

Implement Redis-style **Hash** with `HSET HGET HDEL EXPIRE` plus:

1. **Chaining** for collision resolution  
2. **Dynamic Resizing** based on load factor  
3. **Event-Driven Architecture** (e.g., via `libev`) — callbacks handle commands & auto-expire keys  

---

### 🚀 How to Build & Run

```bash
# clone and enter the repository
git clone https://github.com/weismart1807/DBMS.git
cd DBMS

