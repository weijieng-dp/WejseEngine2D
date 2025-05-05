#include "ScriptingAPI.h"
#include "regex"
#include "filesystem"
#include <fstream>
#include <sstream>
#include "Windows.h"




void generateCodeForClass(std::vector<std::pair<std::string, std::vector<std::string>>> var)
{
	std::ofstream os("../ComponentLoader/ScriptLoader.cpp");

	std::string stringtocopy;

	if (!os.is_open())
	{
		std::cerr << "error cannot open component Loader file" << std::endl;
	}
	stringtocopy = R"(
#include "pch.h"
#include "ScriptLoader.h"
#include <rttr/registration>
#include <iostream>

)";

	os << stringtocopy;

	for (std::pair<std::string, std::vector<std::string>> value : var)
		os << "#include \"" << value.first << ".h\"\n";


	stringtocopy = R"(

void ScriptInitialise() {
    Registry& registry = Registry::instance();
    componentRegistry& ComponentRegistry = componentRegistry::instance();

        )";




	os << stringtocopy;



	for (std::pair<std::string, std::vector<std::string>> value : var)
	{
		os << "ComponentRegistry.registerComponent<" << value.first << ">(\n";
		os << " rttr::type::get<" << value.first << ">().get_name().to_string(),\n";
		os << "        [&registry](EntityRegistry::Entity entity) {\n";
		os << "            registry.addComponent<" << value.first << ">(entity, {});\n";
		os << "});\n";
	}
	os << "}\n";



	stringtocopy = R"(void ScriptDestroy()
{
    componentRegistry& ComponentRegistry = componentRegistry::instance();
)";
	os << stringtocopy;
	for (std::pair<std::string, std::vector<std::string>> value : var)
	{
		os << "ComponentRegistry.unregisterComponent(rttr::type::get<"<< value.first << ">().get_name().to_string());\n";

	}
	
os << " }\n";


	stringtocopy = R"(RTTR_PLUGIN_REGISTRATION
{

)";

	os << stringtocopy;
	for (std::pair<std::string, std::vector<std::string>> value : var)
	{
		os << "rttr::registration::class_<" << value.first << ">(\"" << value.first << "\")\n";
		os << "		.constructor<>()\n";
		os << "		.method(\"OnStart\", &" << value.first << "::OnStart)\n";
		os << "		.method(\"OnUpdate\", &" << value.first << "::OnUpdate)\n";
		for (std::string property : value.second)
		{
			os << "     .property(\"" << property << "\", &" << value.first << "::" << property << ")\n";
		}
		os << ";\n";
	}
	os << "}";
}

void ScanDirectoryForClasses(const char* directoryPath)
{
	std::regex classRegex(R"(class\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*:\s*public\s+Script)");
	std::regex propertyREGEX(R"(UPROPERTY)");


	std::vector<std::pair<std::string, std::vector<std::string>>> ClassesAndProperties;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".h")
		{
			std::ifstream file(entry.path());
			std::string line;

			if (!file.is_open()) {
				std::cerr << "Failed to open: " << entry.path() << std::endl;
				continue;
			}
			std::cout << "Opened: " << entry.path() << std::endl;
			std::vector<std::string>Properties;
			std::string className;

			while (std::getline(file, line))
			{
				std::smatch propmatch;

				if (std::regex_search(line, propmatch, propertyREGEX) && !className.empty())
				{
					std::cout << "found UPROPERTY" << std::endl;
					std::string nextLine;
					if (std::getline(file, nextLine))
					{
						// Optional: strip leading/trailing whitespace
						std::istringstream iss(nextLine);
						std::string name;
						//skips first word
						iss >> name;

						iss >> name;
						std::cout << "found type:" << name << std::endl;
						name.erase(std::remove(name.begin(), name.end(), ';'), name.end());
						Properties.push_back(name);

					}
				}
				std::smatch match;
				if (std::regex_search(line, match, classRegex))
				{
					// Extract class name from regex match className = match[1];
					std::cout << "Found class: " << className << " in " << entry.path() << "\n";
					className = match[1];

					// Generate code for the class (this part can use RTTR or your own logic)
				}


			}
			if (!className.empty())
			{
				ClassesAndProperties.emplace_back(std::make_pair(className, Properties));
				className.clear();
			}
		}
	}
	if (ClassesAndProperties.empty())
		return;
	generateCodeForClass(ClassesAndProperties);

}


/***************************************************************************/
/*!
\brief
Watches the given directory for file changes and invokes a callback
when a file is modified.

\param directory
  The path to the directory to monitor.

\param callback
  Function to call when a file is modified.
*/
/***************************************************************************/
void WatchDirectory(const std::wstring& directory, FileChangeCallback callback)
{
	HANDLE hDir = CreateFileW(
		directory.c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);

	if (hDir == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open directory for watching.\n";
		return;
	}

	OVERLAPPED overlapped = { 0 };
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!overlapped.hEvent) {
		std::cerr << "Failed to create event.\n";
		CloseHandle(hDir);
		return;
	}

	char buffer[4096];
	DWORD bytesReturned;

	while (true) {
		BOOL success = ReadDirectoryChangesW(
			hDir,
			buffer,
			sizeof(buffer),
			TRUE,
			FILE_NOTIFY_CHANGE_LAST_WRITE,
			&bytesReturned,
			&overlapped,
			NULL
		);

		if (!success) {
			std::cerr << "ReadDirectoryChangesW failed.\n";
			break;
		}

		DWORD waitStatus = WaitForSingleObject(overlapped.hEvent, INFINITE);
		if (waitStatus == WAIT_OBJECT_0) {
			FILE_NOTIFY_INFORMATION* pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
			do {
				if (pNotify->Action == FILE_ACTION_MODIFIED) {
					std::wstring filename(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));
					callback(filename);  // Invoke the user-defined callback
				}
				if (pNotify->NextEntryOffset == 0)
					break;

				pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
					reinterpret_cast<BYTE*>(pNotify) + pNotify->NextEntryOffset);

			} while (true);
		}
	}

	CloseHandle(overlapped.hEvent);
	CloseHandle(hDir);
}

/***************************************************************************/
/*!
\brief
Starts the watcher in a separate thread.

\param directory
  The path to the directory to monitor.

\param callback
  Function to call when a file is modified.
*/
/***************************************************************************/
void StartDirectoryWatcher(const std::wstring& directory, FileChangeCallback callback)
{
	std::thread([directory, callback]() {
		WatchDirectory(directory, callback);
		}).detach();
}

bool copyFile(const std::string& source, const std::string& destination) {
	try {
		fs::copy(source, destination, fs::copy_options::overwrite_existing);
		std::cout << "Successfully copied " << source << " to " << destination << std::endl;
		return true;
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Error copying file: " << e.what() << std::endl;
		return false;
	}
}