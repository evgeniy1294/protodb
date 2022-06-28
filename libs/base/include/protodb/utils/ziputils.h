#pragma once

#include <string>
#include <filesystem>

class QString;

bool zipDirectory(const QString& dir, const QString& zip);
bool zipDirectory(const std::string& dir, const std::string& zip);
bool zipDirectory(const std::filesystem::path& dir, const std::filesystem::path& zip);

/*
bool unzipDirectory(const QString& dir, const QString& zip);
bool unzipDirectory(const std::string& dir, const std::string& zip);*/
bool unzipDirectory(const std::filesystem::path& dir, const std::filesystem::path& zip);

