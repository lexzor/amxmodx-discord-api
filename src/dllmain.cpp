#include "precompiled.h"

#include "dllmain.h"
#include "amxx/amx_natives.h"
#include "amxx/amx_forwards.h"
#include "mpsc/events_queue.h"
#include "console_commands/concmds.h"
#include "console_variables/cvars.h"
#include "amxx/pending_amx_object_store_impl.h"

void OnAmxxAttach()
{
	RegisterNatives();
}

void OnAmxxDetach() {}

void OnPluginsLoaded()
{
	RegisterForwards();

	g_EventsQueue->SetProcessingLock(false);

	// Send OnBotReady event for bots which are started from a previous map
	g_DiscordBotsManager->ForEach([](DiscordBot& bot) {
		if (bot.IsStarted())
			ExecuteForward(ON_BOT_READY, bot.GetIdentifier().c_str());
		}
	);
}

void OnPluginsUnloading()
{
	g_EventsQueue->SetProcessingLock(true);
}

void OnMetaAttach(PLUG_LOADTIME current_phase)
{
	SetupOpenSSLCertPaths();
	InitializeDiscordBotsManager();
	InitializeEventsQueue();
	RegisterConsoleCommands();
	RegisterConsoleVariables();

	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Success: Version %s compiled (%s)", MODULE_VERSION, MODULE_DATE);
}

void OnMetaDetach(PLUG_LOADTIME iCurrentPhase, PL_UNLOAD_REASON iReason)
{
	ConsumeQueueEvents();
	DeleteUnfinishedPendingAmxObjectStore();
	DeinitializeDiscordBotsManager();

	gpMetaUtilFuncs->pfnLogConsole(PLID, "[DiscordAPI] Module detached");
}

inline void SetupOpenSSLCertPaths()
{
    if (getenv("SSL_CERT_FILE") == nullptr) {
        static constexpr std::array<const char*, 5> certFiles = {
            "/etc/ssl/certs/ca-certificates.crt",                  // Debian / Ubuntu / Arch
            "/etc/pki/tls/certs/ca-bundle.crt",                    // CentOS / RHEL / Fedora
            "/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem",   // CentOS 7+ / RHEL 7+
            "/etc/ssl/ca-bundle.pem",                              // OpenSUSE
            "/var/lib/ca-certificates/ca-bundle.pem"
        };

        for (const char* path : certFiles) {
            if (std::filesystem::exists(path)) {
                setenv("SSL_CERT_FILE", path, 1);
                break;
            }
        }
    }

    if (getenv("SSL_CERT_DIR") == nullptr) {
        static constexpr std::array<const char*, 2> certDirs = {
            "/etc/ssl/certs",
            "/etc/pki/tls/certs"
        };

        for (const char* path : certDirs) {
            if (std::filesystem::exists(path)) {
                setenv("SSL_CERT_DIR", path, 1);
                break;
            }
        }
    }
}