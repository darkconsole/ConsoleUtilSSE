#include "Papyrus.h"

#include "skse64/GameReferences.h"

#include "RE/Skyrim.h"
#include "REL/Relocation.h"


namespace Papyrus
{
	void ConsoleUtil::ExecuteCommand(RE::StaticFunctionTag*, RE::BSFixedString a_command)
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


	RE::TESObjectREFR* ConsoleUtil::GetSelectedReference(RE::StaticFunctionTag*)
	{
		auto selectedRef = RE::Console::GetSelectedRef();
		return selectedRef.get();
	}


	void ConsoleUtil::SetSelectedReference(RE::StaticFunctionTag*, RE::TESObjectREFR* a_reference)
	{
		using Message = RE::UI_MESSAGE_TYPE;

		if (a_reference) {
			auto factory = RE::MessageDataFactoryManager::GetSingleton();
			auto intfcStr = RE::InterfaceStrings::GetSingleton();
			if (!factory || !intfcStr) {
				return;
			}

			auto creator = factory->GetCreator<RE::ConsoleData>(intfcStr->consoleData);
			if (!creator) {
				return;
			}

			auto consoleData = creator->Create();
			if (consoleData) {
				consoleData->type = static_cast<RE::ConsoleData::DataType>(1);
				consoleData->pickRef = a_reference->CreateRefHandle();
			} else {
				return;
			}

			auto msgQ = RE::UIMessageQueue::GetSingleton();
			if (msgQ) {
				msgQ->AddMessage(intfcStr->console, Message::kReshow, consoleData);
			}
		} else {
			auto ui = RE::UI::GetSingleton();
			if (!ui) {
				return;
			}

			auto console = ui->GetMenu<RE::Console>();
			if (console) {
				RE::ObjectRefHandle nullHandle;
				console->SetSelectedRef(nullHandle);
			}
		}
	}


	RE::BSFixedString ConsoleUtil::ReadMessage(RE::StaticFunctionTag*)
	{
		auto consoleManager = RE::ConsoleLog::GetSingleton();
		return consoleManager ? consoleManager->lastMessage : "";
	}


	void ConsoleUtil::PrintMessage(RE::StaticFunctionTag*, RE::BSFixedString a_message)
	{
		auto consoleManager = RE::ConsoleLog::GetSingleton();
		if (consoleManager) {
			consoleManager->Print(a_message.c_str());
		}
	}


	SInt32 ConsoleUtil::GetVersion(RE::StaticFunctionTag*)
	{
		return kVersion;
	}


	bool ConsoleUtil::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("ExecuteCommand", CLASS_NAME, ExecuteCommand);
		a_vm->RegisterFunction("GetSelectedReference", CLASS_NAME, GetSelectedReference);
		a_vm->RegisterFunction("SetSelectedReference", CLASS_NAME, SetSelectedReference);
		a_vm->RegisterFunction("ReadMessage", CLASS_NAME, ReadMessage);
		a_vm->RegisterFunction("PrintMessage", CLASS_NAME, PrintMessage);
		a_vm->RegisterFunction("GetVersion", CLASS_NAME, GetVersion);

		_MESSAGE("Registered funcs for class %s", CLASS_NAME);

		return true;
	}


	bool Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		ConsoleUtil::Register(a_vm);
		return true;
	}
}
