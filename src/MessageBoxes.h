#pragma once

namespace MessageBoxes 
{
	class MessageBoxResultCallback : public RE::IMessageBoxCallback
	{
	public:
		MessageBoxResultCallback(std::function<void(unsigned int)> callback) : _callback(std::move(callback)) {}

		void Run(RE::IMessageBoxCallback::Message message);

		

	private:
		std::function<void(unsigned int)> _callback;
	};

	void ShowMessageBox(const std::string& bodyText, const std::vector<std::string>& buttonTextValues,
		std::function<void(unsigned int)> callback);
}
