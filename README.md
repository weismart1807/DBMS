# homework

A collection of six progressively-built C projects that implement a lightweight, Redis-like NoSQL database engine on Linux.  
Each assignment adds new data structures, commands, and performance tooling.

---

## 📚 Assignment 1 — Simple NoSQL Engine

Develop a minimal key-value store in **C**:

- Efficiently handles large volumes of key-value pairs  
- Supports full **CRUD** (Create, Read, Update, Delete)  
- Uses pointers & dynamic memory for data management  

---

### 🔧 Assignment 2 — Library + Benchmark

1. **Refactor** Assignment 1 into a reusable library / API.  
2. **Benchmark** program (C)  
   - **Bulk Insert**: generate ≥ 100 k unique key-value pairs, insert into *your* DB **and** open-source **Redis**; record latency & memory.  
   - **Latency Test**: average time for Create and random Read of ≥ 100 k records.  
   - **Memory Footprint**: compare total RAM used by both engines.  

---

## 📑 Assignment 3 — List Commands

Extend the engine to support Redis-style **List** operations:  
`LPUSH • LPOP • RPUSH • RPOP • LLEN • LRANGE`

---

## 🌲 Assignment 4 — Tree Support

- Manage multiple trees; each node stores a key and pointer to its value.  
- Custom commands to **insert / search / delete** nodes in any tree.

---

## 📈 Assignment 5 — Sorted Set

Add Redis-like **Sorted Set** commands:  
`ZADD, ZCARD, ZCOUNT, ZINTERSTORE, ZUNIONSTORE, ZRANGE,  
ZRANGEBYSCORE, ZRANK, ZREM, ZREMRANGE, ZREMRANGEBYSCORE`

---

## 🗄️ Assignment 6 — Hash + Events

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

