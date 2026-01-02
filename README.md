# Discord API into Pawn AMX Mod X

A library for AMX Mod X that provides Discord API functionality to AMXX plugins.

It uses <a href="https://github.com/brainboxdotcc/DPP">D++ (DPP)</a> as the underlying C++ library to interface with Discord, exposing convenient natives that plugin developers can call directly from their AMXX scripts.

This allows you to integrate Discord features directly within your AMXX plugins without needing to implement the Discord protocol from scratch or use another virtual machine to host the Discord bot.

<div style="border: 1px solid #f1c40f; padding: 12px; border-radius: 6px; background: #fff8d5; font-size: 12px;">
  <h3>⚠️ Important Notice</h3>
  <p>
    This module it's still in BETA version and crashes may occur due to module or AMXX plugins.
  </p>
</div>

## Installation
Before installing the module, you have to know it may not work on outdated operating system because the project uses <b>C++20</b>. If you use <a href="https://github.com/pterodactyl/panel">Pterodactyl Panel</a> to run your server in a <b>Docker container</b>, you may want to update the docker image to `ghcr.io/parkervcp/steamcmd:debian`.

1. Download last stable release.
2. Copy `discordapi_amxx_i386.so` to `/cstrike/addons/amxmodx/modules`.
3. Enable module in `/cstrike/addons/amxmodx/configs/modules.ini` by typing a new line `discordapi`.
4. Restart server and type in server console `amxx modules` to check if it was loaded properly.

## Discord Bot Configuration
Discord bots use <b>intents</b> to specify which events they receive. This module uses the `Message Content Intent` by default for message events. You must enable this intent in the <b>Discord Developer Portal</b>.

It is also recommended to <b>enable all other intents</b> from the portal to ensure the bot works correctly.

<details>
<summary>Contribution</summary>
Before diving into the build steps, it’s important to understand that the project is compiled inside a Docker container running a specific Unix-like distribution. This approach was adopted because Half-Life 1 game servers are hosted on a wide variety of operating systems. Currently, testing has only been performed on <a href="https://hostsrc.io/">Hostsrc.io</a> Counter-Strike 1.6 game servers running in a <b>Debian 12 Docker container</b>.

The project uses <a href="https://github.com/brainboxdotcc/DPP">D++ (DPP)</a> as the underlying C++ library, which requires at least <b>C++17</b>. This project itself is compiled with <b>C++20</b>. D++ relies on additional libraries such as <b>ZLib</b>, <b>OpenSSL</b>, and <b>cURL</b>, which are compiled and statically linked into the project to avoid issues with missing APIs on different operating systems.

<div style="border: 1px solid #f1c40f; padding: 12px; border-radius: 6px; background: #fff8d5; font-size: 12px;">
  <h3>⚠️ Important Notice</h3>
  <p>
    All building steps are intended for Windows users.
  </p>
</div>

<details>
  <summary>1. Docker Setup</summary>
  First, you need to <b>build the Docker container</b> that will be used to compile both project and libraries. Currently only <b>Debian12</b> is supported.
  
    1. Open Windows Powershell Terminal.
    2. Navigate to `amxmodx-discord-api\docker-images\debian12`.
    3. Run `build_container.ps1`.
</details>

After this step, the Docker container should be installed on your computer.

<details>
  <summary>2. Build Libraries</summary>
  As mentioned earlier, the project statically links libraries so that the <b>AMXX module</b> does not depend on OS-installed libraries at runtime. This step only needs to be done <b>once per Docker container</b>.
  
    1. Open Windows Powershell Terminal.
    2. Navigate to `amxmodx-discord-api\docker-images\debian12`.
    3. Run `build_libs.ps1`.
</details>

After completion, all required libraries will be copied to the `amxmodx-discord-api\vendor\bin` directory.

<details>  
  <summary>Build Project</summary>
  Finally, build the project itself:
  
    1. Open Windows Powershell Terminal.
    2. Navigate to `amxmodx-discord-api\docker-images\debian12`.
    3. Run `build_project.ps1`.
</details>

</details>
