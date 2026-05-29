# 🛡️🧙 MyMMO

<p align="center">
  <img src="idle-mmorpg-data/idle-mmorpg-configuration/logo/logo.png" alt="iMMO Logo" height="300"/>
</p>

# 💡 Motivation
I got tired of playing MMORPGs that are abandoned by developers, filled with aggressive monetization, and lacking meaningful updates or player interaction.  
MyMMO was born from the desire to build something better: a idle-friendly game with constant progression, active features, and no paywalls — focused on fun, accessibility, and long-term evolution.

# 👥 People
This project was envisioned, created, and is maintained by:  
- **[Luiz Antonio Nicolau Anghinoni](https://github.com/luizantoniona)**

# 🎯 Project Goals
- ⚔️ Create a accessible idle MMORPG experience  
- 🌐 Build a scalable multiplayer backend in **C++**  
- 💬 Offer real-time interaction via **WebSocket** and web interface  

# 📦 Project Organization:

## 🛡️ [`Server`](https://github.com/luizantoniona/idle-mmorpg/tree/main/server)
- **TODO**
  - Central game logic and state management.
  - Manages user sessions, world state, entities, and events.
  - Communicates with the client via API and WebSocket.
  - Loads static game data (items, maps, skills) at runtime from JSON files.

### ⚙️ **Database**
- **For now, you need to run the schema manually.**
- **Once all tables are defined and only minor changes are needed, we will create the migration steps.**

## ⚔️ [`Client`](https://github.com/luizantoniona/idle-mmorpg/tree/main/client)
- **TODO**

## 🧩 [`Data`](https://github.com/luizantoniona/idle-mmorpg/tree/main/data)
- **TODO**

---

# 🚀 Running
These instructions will help you set up the project locally for development and testing.

## 📁 Clone the Repository
```bash
git clone https://github.com/luizantoniona/idle-mmorpg.git
cd idle-mmorpg
```