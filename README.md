# (AMXMODX) Discord API

A library for AMX Mod X that provides Discord API functionality to AMXX plugins.

It uses D++ (DPP) as the underlying C++ library to interface with Discord, exposing convenient natives that plugin developers can call directly from their AMXX scripts.

This allows you to integrate Discord features directly within your AMXX plugins without needing to implement the Discord protocol from scratch or use another virtual machine to host the Discord bot.

<div style="border: 1px solid #f1c40f; padding: 12px; border-radius: 6px; background: #fff8d5;">
  <h3>⚠️ Important Notice</h3>
  <p>
    This module it's still in BETA version and crashes may occur due to module or AMXX plugins errors.
  </p>
</div>

# Installation
Before installing the module, you have to know it may not work on outdated systems (ubuntu18 as an example) because the project uses C++20. If you use pterodactyl to run your server in a docker container update the docker image to `ghcr.io/parkervcp/steamcmd:debian`.


1. Download last stable release.
2. Copy `discordapi_amxx_i386.so` to `/cstrike/addons/amxmodx/modules`.
3. Enable module in `/cstrike/addons/amxmodx/configs/modules.ini` by typing `discordapi`.
4. Restart server and type in server console `amxx modules` to check if it was loaded properly
