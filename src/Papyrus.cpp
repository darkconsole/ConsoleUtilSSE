#include "Papyrus.h"

#include "skse64/GameReferences.h"

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace ConsoleUtil
{
	enum
	{
		kVersion = 4
	};


	void ExecuteCommand(RE::StaticFunctionTag*, RE::BSFixedString a_command)
	{
		auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
		if (!scriptFactory) {
			return;
		}

		auto script = scriptFactory->Create();
		if (!script) {
			return;
		}

		auto selectedRef = RE::Console::GetSelectedRef();
		script->SetCommand(a_command);
		script->Invoke(selectedRef.get());
		delete script;
	}


	RE::TESObjectREFR* GetSelectedReference(RE::StaticFunctionTag*)
	{
		auto selectedRef = RE::Console::GetSelectedRef();
		return selectedRef.get();
	}


	void SetSelectedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_reference)
	{
		using Message = RE::UIMessage::Message;

		if (a_reference) {
			auto factory = RE::MessageDataFactoryManager::GetSingleton();
			auto uiStrHolder = RE::UIStringHolder::GetSingleton();
			if (!factory || !uiStrHolder) {
				return;
			}

			auto creator = factory->GetCreator<RE::ConsoleData>(uiStrHolder->consoleData);
			if (!creator) {
				return;
			}

			auto consoleData = creator->Create();
			if (consoleData) {
				consoleData->unk28 = 1;
				consoleData->handle = a_reference->CreateRefHandle();
			} else {
				return;
			}

			auto uiManager = RE::UIManager::GetSingleton();
			if (uiManager) {
				uiManager->AddMessage(uiStrHolder->console, Message::kRefresh, consoleData);
			}
		} else {
			auto mm = RE::MenuManager::GetSingleton();
			auto uiStrHolder = RE::UIStringHolder::GetSingleton();
			if (!mm || !uiStrHolder) {
				return;
			}

			auto console = mm->GetMenu<RE::Console>(uiStrHolder->console);
			if (console) {
				console->SetSelectedRef(*g_invalidRefHandle);
			}
		}
	}


	RE::BSFixedString ReadMessage(RE::StaticFunctionTag*)
	{
		auto consoleManager = RE::ConsoleManager::GetSingleton();
		return consoleManager ? consoleManager->lastMessage : "";
	}


	void PrintMessage(RE::StaticFunctionTag*, RE::BSFixedString a_message)
	{
		auto consoleManager = RE::ConsoleManager::GetSingleton();
		if (consoleManager) {
			consoleManager->Print(a_message.c_str());
		}
	}


	SInt32 GetVersion(RE::StaticFunctionTag*)
	{
		return kVersion;
	}


	bool RegisterFuncs(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("ExecuteCommand", "ConsoleUtil", ExecuteCommand);
		a_vm->RegisterFunction("GetSelectedReference", "ConsoleUtil", GetSelectedReference);
		a_vm->RegisterFunction("SetSelectedReference", "ConsoleUtil", SetSelectedReference);
		a_vm->RegisterFunction("ReadMessage", "ConsoleUtil", ReadMessage);
		a_vm->RegisterFunction("PrintMessage", "ConsoleUtil", PrintMessage);
		a_vm->RegisterFunction("GetVersion", "ConsoleUtil", GetVersion);
		return true;
	}
}
