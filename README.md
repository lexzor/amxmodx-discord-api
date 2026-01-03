# Discord API into Pawn AMX Mod X

A library for AMX Mod X that provides some of the Discord API functionality to AMXX plugins.

It uses <a href="https://github.com/brainboxdotcc/DPP">D++ (DPP)</a> as the underlying C++ library to interface with <b>Discord API</b>, exposing convenient natives that plugin developers can call directly from their <b>AMX Mod X</b> scripts.

> [!IMPORTANT]
> This module is still in <b>BETA</b>, so crashes may occur due to the module itself or <b>AMX Mod X</b> plugins you create.

## Usage
Before installing the module, you have to know it may not work on outdated operating system because the project uses <b>C++20</b>. If you use <a href="https://github.com/pterodactyl/panel">Pterodactyl Panel</a> to run your server in a <b>Docker container</b>, you may want to update the docker image to `ghcr.io/parkervcp/steamcmd:debian`.

#### Installation
1. Download last stable release.
2. Copy `discordapi_amxx_i386.so` to `/cstrike/addons/amxmodx/modules`.
3. Enable module in `/cstrike/addons/amxmodx/configs/modules.ini` by typing a new line `discordapi`.
4. Restart server and type in server console `amxx modules` to check if it was loaded properly.

#### Discord Bot Configuration
Discord bots use <b>intents</b> to specify which events they receive. This module uses the `Message Content Intent` by default for message events. You must enable this intent in the <b>Discord Developer Portal</b>.

It is also recommended to <b>enable all other intents</b> from the portal to ensure the bot works correctly.

## AMX Mod X Scripting
All natives and forwards can be found in [include folder](https://github.com/lexzor/amxmodx-discord-api/tree/main/amxmodx/scripting/include).
Plugin examples can be found in [scripting folder](https://github.com/lexzor/amxmodx-discord-api/tree/main/amxmodx/scripting).

<details>
  <summary>1. General Informations</summary>
  Almost all natives and forwards include a parameter called <b>identifier</b>, which must be unique. This is necessary because the module needs to know which bot the changes should apply to or which bot is emitting an event. It can also be used to split bot functionality across multiple plugins.

   The identifier is not validated by the module, so using duplicate identifiers can result in changes being applied to the wrong bot or events being captured incorrectly.
</details>

<details>
  <summary>2. Discord API Requests</summary>
  Some natives send requests over HTTPS or WebSocket protocols to the Discord API, and the response cannot be returned in the same server frame. All asynchronous logic is mostly handled by the <b>DPP library</b> and the module itself, but if an error occurs, it will be printed in the console.
  For example, the native <b>StartBot</b> will return an error if the bot does not exist. However, if the bot fails to connect to the Discord API, an error will be printed later in the console when a response is received.
</details>

<details>
  <summary>3. Debugging</summary>
  You can set the log level to <b>VERBOSE</b> in the bot options (which can be changed at any time during the bot’s lifecycle). Note that the <b>VERBOSE</b> log level can be overwhelming, as it displays almost all HTTPS or WebSocket data received from the Discord API, but it may be useful when manipulating bot options (for example, global or guild slash commands) to catch errors.
  To debug events without <b>VERBOSE</b> log level, you can use the <b>PRINTS_EVENT_DATA</b> option in the Options array with <b>SetBotOptions</b> (see <b>discordapi.inc</b>) to output the full <b>JSON</b> data received from the Discord API. Note that the printed values do not exactly represent the data passed to AMX Mod X forwards due to plugin limitations.
  If you consider there is not enough data to develop your plugin, you can anytime open an issue or a PR.
</details>

## Contribution
Before diving into the build steps, it’s important to understand that the project is compiled inside a Docker container running a specific Unix-like distribution. This approach was adopted because Half-Life 1 game servers are hosted on a wide variety of operating systems. Currently, testing has only been performed on <a href="https://hostsrc.io/">Hostsrc.io</a> Counter-Strike 1.6 game servers running in a <b>Debian 12 Docker container</b>.

The project uses <a href="https://github.com/brainboxdotcc/DPP">D++ (DPP)</a> as the underlying C++ library, which requires at least <b>C++17</b>. This project itself is compiled with <b>C++20</b>. D++ relies on additional libraries such as <b>ZLib</b>, <b>OpenSSL</b>, and <b>cURL</b>, which are compiled and statically linked into the project to avoid issues with missing APIs on different operating systems.

> [!IMPORTANT]
> All build steps are intended for Windows users, as they use PowerShell scripts; however, they can be easily converted to Bash scripts.

#### Build steps

<details>
  <summary>1. Docker Setup</summary>
  First, you need to <b>build the Docker container</b> that will be used to compile both project and libraries. Currently only <b>Debian12</b> is supported.
  
    1. Open Windows Powershell Terminal.
    2. Navigate to amxmodx-discord-api\docker-images\debian12.
    3. Run build_container.ps1.
</details>

After this step, the Docker container should be installed on your computer.

<details>
  <summary>2. Build Libraries</summary>
  As mentioned earlier, the project statically links libraries so that the <b>AMXX module</b> does not depend on OS-installed libraries at runtime. This step only needs to be done <b>once per Docker container</b>.
  
    1. Open Windows Powershell Terminal.
    2. Navigate to amxmodx-discord-api\docker-images\debian12.
    3. Run build_libs.ps1.
</details>

After completion, all required libraries will be copied to the `amxmodx-discord-api\vendor\bin` directory.

<details>  
  <summary>3. Build Project</summary>
  Finally, build the project itself:
  
    1. Open Windows Powershell Terminal.
    2. Navigate to amxmodx-discord-api\docker-images\debian12.
    3. Run build_project.ps1.
</details>

