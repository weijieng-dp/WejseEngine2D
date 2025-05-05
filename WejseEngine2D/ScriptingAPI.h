#pragma once
#include <rttr/registration.h>

#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <filesystem>

#define UPROPERTY
using FileChangeCallback = std::function<void(const std::wstring&)>;

void ScanDirectoryForClasses(const char*);


struct Script
{
    virtual void OnStart() {}
    virtual void OnUpdate() {}

    RTTR_ENABLE()
};

void StartDirectoryWatcher(const std::wstring& directory, FileChangeCallback callback);


namespace fs = std::filesystem;

bool copyFile(const std::string& source, const std::string& destination);