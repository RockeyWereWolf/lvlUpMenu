#include "MessageBoxes.h"

namespace MessageBoxes
{
	void MessageBoxResultCallback::Run(RE::IMessageBoxCallback::Message message)
	{
		_callback(static_cast<unsigned int>(message));
	}
	

	void MessageBoxes::ShowMessageBox(const std::string& bodyText, const std::vector<std::string>& buttonTextValues,
		std::function<void(unsigned int)> callback)
	{
		auto* factoryManager = RE::MessageDataFactoryManager::GetSingleton();
		auto* uiStringHolder = RE::InterfaceStrings::GetSingleton();
		auto* factory = factoryManager->GetCreator<RE::MessageBoxData>(uiStringHolder->messageBoxData);
		auto* messagebox = factory->Create();

		auto messageCallback = RE::make_smart<MessageBoxResultCallback>(callback);
		messagebox->callback = messageCallback;
		messagebox->bodyText = bodyText;

		for (const auto& text : buttonTextValues) {
			messagebox->buttonText.push_back(text.c_str());
		}

		messagebox->QueueMessage();
	}
}
