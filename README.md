# Unreal Engine 5 – Third Person Action Prototype  
*(Udemy Tutorial Project Showcase)*

![Cover](Docs/Cover.png)

This repository contains the **C++ source code and configuration** from my work in the  
📚 *Unreal Engine 5 C++ The Ultimate Game Developer Course (Udemy)*.

I use GitHub to **showcase code and design work**, while the **full project including assets**  
(animations, materials, large files) is hosted privately on **Azure DevOps**, where I also experimented  
with large file handling and DevOps workflows.

---

## 🎮 Features Implemented
In this project I extended the base tutorial and built a prototype featuring:

- ✅ **Custom Third Person Character** – animations, movement, attacks  
- 🔄 **Weapon System** – pick up & carry weapons, switch between armed/unarmed states  
- 🪓 **Destructible Objects** – props that react to attacks  
- 🎁 **Loot Pickups** – items that can be collected during play  
- 🤖 **AI Enemies** – full animation set, hit reactions, combat logic  
- 🧭 **AI Pathing** – navigation mesh integration, enemies track & follow the player

---

## 🖼️ Media

### Gameplay Examples

<!-- Variante A: sauberes 2-Spalten-Grid per Markdown-Tabelle -->
| Weapon Pickup | Loot Pickup |
|---|---|
| ![Weapon Pickup](Docs/WeaponPickup.gif) | ![Loot Pickup](Docs/LootPickup.gif) |

| Enemy Hit Reaction | Enemy Follow / Lose Interest |
|---|---|
| ![Enemy Hit Reaction](Docs/EnemyAttackOnDamage.gif) | ![Enemy Follow / Lose Interest](Docs/EnemyFollowLoseInterest.gif) |

| Destructible Objects |
|---|
| ![Destructible Objects](Docs/DestructibleObjects.gif) |


### Editor Screenshots

| AI Components | Breakable Objects |
|---|---|
| ![AI Components](Docs/AIComponents.png) | ![Breakable Objects](Docs/BreakableObjects.png) |

| Weapon BP – Object Destruction | Weapon Pickups Blueprint |
|---|---|
| ![Weapon BP – Object Destruction](Docs/WeaponBP_ObjectDestruction.png) | ![Weapon Pickups](Docs/WeaponPickups.png) |

| Two-Handed Weapon Setup |
|---|
| ![Two-Handed Weapon](Docs/TwoHandedWeapon.png) |

*(GIFs and screenshots captured from the Azure DevOps project build.)*

---

## ⚙️ Tech Stack
- **Unreal Engine 5 (UE5)**  
- **C++ Gameplay Programming**  
- Blueprints (for rapid prototyping / animation state machines)  
- Animation Blueprints, AI Controllers, Behavior Trees  
- Azure DevOps (full repo, large assets, pipelines)

---

## 📂 Repository Structure
This repo contains only the **code and config** relevant for learning & review:

```text
Source/            → C++ gameplay classes
Config/            → UE project settings
Udemy.uproject
README.md
Docs/              → screenshots, gifs, notes
