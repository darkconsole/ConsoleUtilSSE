#include "skse64_common/skse_version.h"  // RUNTIME_VERSION

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "Events.h"  // MenuOpenCloseEventHandler
#include "Papyrus.h"  // RegisterFuncs
#include "version.h"  // VERSION_VERSTRING, VERSION_MAJOR

#include "RE/Skyrim.h"
#include "SKSE/API.h"


namespace
{
	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			auto mm = RE::MenuManager::GetSingleton();
			mm->GetMenuOpenCloseEventSource()->AddEventSink(Events::MenuOpenCloseEventHandler::GetSingleton());
			_MESSAGE("[MESSAGE] Registered menu open close event sink");
			break;
		}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\ConsoleUtilSSE.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("ConsoleUtilSSE v%s", CUTL_VERSION_VERSTRING);

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "ConsoleUtilSSE";
		a_info->version = CUTL_VERSION_MAJOR;

		if (a_skse->IsEditor()) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}
		
		switch (a_skse->RuntimeVersion()) {
		case RUNTIME_VERSION_1_5_73:
		case RUNTIME_VERSION_1_5_80:
			break;
		default:
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", a_skse->RuntimeVersion());
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("[MESSAGE] ConsoleUtilSSE loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		auto messaging = SKSE::GetMessagingInterface();
		if (!messaging->RegisterListener("SKSE", MessageHandler)) {
			_FATALERROR("[FATAL ERROR] Failed to register messaging listener!\n");
			return false;
		}

		auto papyrus = SKSE::GetPapyrusInterface();
		if (!papyrus->Register(ConsoleUtil::RegisterFuncs)) {
			_FATALERROR("[FATAL ERROR] Failed to register papyrus callback!\n");
			return false;
		}

		return true;
	}
};
