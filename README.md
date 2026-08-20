# Discord API for AMX Mod X

A powerful library that brings the functionality of the **Discord API** directly to **AMX Mod X** plugins. Powered by [D++ (DPP)](https://github.com/brainboxdotcc/DPP), this module exposes convenient natives and forwards so developers can seamlessly bridge their Counter-Strike 1.6 servers with Discord.

## 📋 Requirements

* **AMX Mod X:** `v1.9.0.5294` or newer (includes the JSON module).
* **Metamod-R:** `v1.3.0.149` or newer.

> **Compatibility Note:** Other Metamod versions may work, but full stability is only guaranteed with the specified Metamod-R release. Furthermore, this project is built using **C++20**. If you are running your server in a Docker container (e.g., via Pterodactyl Panel), ensure your operating system is up-to-date. We recommend using the `ghcr.io/parkervcp/steamcmd:debian` image.

---

## 🚀 Getting Started

### 1. Bot Configuration
Before installing the module, you must properly configure your bot in the [Discord Developer Portal](https://discord.com/developers/applications):
* **Copy the Token:** Generate and save your bot token.
* **Enable Intents:** Turn on **Presence Intent**, **Server Members Intent**, and **Message Content Intent** to ensure message events are captured properly.
* **Set Permissions:** Invite the bot to your guild with **Administrator** permissions to prevent `Insufficient permissions` API errors.
* **Adjust Role Hierarchy:** If your bot will manage member roles, its assigned role must be positioned strictly *above* any roles it needs to assign or revoke.

### 2. Installation
* Download the latest stable release.
* Move the `discordapi_amxx_i386.so` file into `/cstrike/addons/amxmodx/modules/`.
* Open `/cstrike/addons/amxmodx/configs/modules.ini` and add `discordapi` on a new line.
* Restart your server and type `amxx modules` in the console to verify successful loading.

---

## ⚡ Features & API Reference

### 🤖 Bot Lifecycle & Management
* **Create & Start Bots:** Initialize and start Discord bot instances using tokens.
* **Lifecycle Management:** Safely stop or completely remove bot instances from memory.
* **Status Checks:** Verify bot existence (`BotExists`) or connection status (`IsBotReady`).
* **Configuration:** Adjust custom console prefixes, log levels (`NONE`, `DEFAULT`, `VERBOSE`), and toggle event printing.
* **Interaction Replies:** Respond directly to slash command interactions using `SendReply`.

### 💬 Messaging & Channels
* **Send Messages:** Post text messages directly to specific Discord channels.
* **Event Listeners:** Intercept incoming channel messages with full JSON event payloads.
* **Channel Operations:** Construct, edit, inspect, or delete guild channels asynchronously.

### ⚡ Guild Slash Commands
* **Command Registration:** Easily register simple or complex slash commands with string, integer, and float options.
* **Command Management:** Query existing commands or remove them dynamically.
* **Interaction Forwards:** Handle executed commands, reading parameters, caller IDs, and channel contexts.

### 👥 Guild Members & Roles
* **Member Queries:** Search local guild caches by user ID or username.
* **Data Inspection:** Read nicknames, join timestamps, timeout statuses, and boost dates.
* **API Fetching:** Asynchronously fetch members outside the local cache directly from Discord.
* **Role Management:** Verify, assign, or revoke roles from guild members dynamically.

---

## 💻 AMX Mod X Scripting Guidelines

All natives and forwards are documented in the [include folder](https://github.com/lexzor/amxmodx-discord-api/tree/main/amxmodx/scripting/include). Reference the [scripting folder](https://github.com/lexzor/amxmodx-discord-api/tree/main/amxmodx/scripting) for plugin examples.

* **Unique Identifiers:** Almost all natives/forwards require a bot `identifier`. This allows you to run multiple bots or split logic across multiple plugins. Ensure identifiers remain consistent; they are not strictly validated by the module.
* **Asynchronous Requests:** Actions interacting directly with the Discord API (HTTPS/WebSocket) cannot return responses in a single server frame. Background tasks are handled by the DPP library, and runtime errors will print directly to the server console.
* **Handling Interactions:** If a forward is marked as an *Interaction* (e.g., `OnChannelMessageCreated`), you can use the `SendReply` native to have the bot reply directly to the triggering user.
* **JSON Event Data:** Events push data via forwards in JSON format. To prevent AMX Mod X stack errors, some payload data is minimized.
* **Debugging:** Change the bot's `LOG_LEVEL` option to `VERBOSE` to inspect raw HTTPS/WebSocket traffic. Alternatively, enable `PRINT_EVENT_DATA` to log incoming JSON payloads to the console without overwhelming it with full network traffic.

---

## 🛠️ Contribution & Building from Source

This module statically links libraries (ZLib, OpenSSL, cURL, DPP) so it does not rely on the host OS at runtime. To ensure broad compatibility across Half-Life 1 game servers, the project is compiled inside a **Debian 12 Docker container**. 

*The following steps utilize Windows PowerShell scripts, but they can be easily adapted to Bash.*

### Build Instructions
* **Setup Docker:** Navigate to `docker-images\debian12` and run `build_container.ps1` to prepare the isolated compilation environment.
* **Compile Dependencies:** Run `build_libs.ps1` inside the same directory. This compiles the static libraries and copies them to `vendor\bin` (only needs to be done once).
* **Compile the Module:** Run `build_project.ps1` to compile the final AMXX module.