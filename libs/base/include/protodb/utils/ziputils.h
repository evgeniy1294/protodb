#pragma once

#include <protodb/export/base_cfg.hpp>

#include <string>
#include <filesystem>

class QString;

bool PROTODB_BASE_EXPORT zipDirectory(const QString& dir, const QString& zip);
bool PROTODB_BASE_EXPORT zipDirectory(const std::string& dir, const std::string& zip);
bool PROTODB_BASE_EXPORT zipDirectory(const std::filesystem::path& dir, const std::filesystem::path& zip);

bool PROTODB_BASE_EXPORT unzipToDirectory(const QString& zip, const QString& dir);
bool PROTODB_BASE_EXPORT unzipToDirectory(const std::string& zip, const std::string& dir);
bool PROTODB_BASE_EXPORT unzipToDirectory(const std::filesystem::path& zip, const std::filesystem::path& dir);

