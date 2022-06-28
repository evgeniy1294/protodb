#include <protodb/utils/ziputils.h>


#include <zip.h>

#include <QString>

#include <iostream>

namespace fs = std::filesystem;

static void zip_walk_directory(const std::filesystem::path &dir, zip_t* zipper) {

    for (const fs::directory_entry& entry: fs::directory_iterator(dir)) {
        if (is_directory(entry)) {
            if (zip_dir_add(zipper, entry.path().c_str(), ZIP_FL_ENC_UTF_8) < 0) {
                std::cerr << "Failed to add directory to zip: " + std::string(zip_strerror(zipper));
                continue;
            }

            zip_walk_directory(entry.path(), zipper);
        }
        else {
            zip_source_t *source = zip_source_file(zipper, entry.path().c_str(), 0, 0);
            if (source == nullptr) {
                std::cerr << "Failed to add file to zip: " + std::string(zip_strerror(zipper));
                continue;
            }

            if (zip_file_add(zipper, entry.path().c_str(), source, ZIP_FL_ENC_UTF_8) < 0) {
                zip_source_free(source);
                std::cerr << "Failed to add file to zip: " + std::string(zip_strerror(zipper));
            }
        }
    }

    return;
}

bool zipDirectory(const QString &dir, const QString &zip)
{
    return zipDirectory(fs::path(dir.toStdString()), fs::path(zip.toStdString()));
}

bool zipDirectory(const std::string &dir, const std::filesystem::path &zip)
{
    return zipDirectory(fs::path(dir), fs::path(zip));
}


bool zipDirectory(const fs::path &dir, const fs::path &zip)
{
    if ( !std::filesystem::exists(dir) ) {
        std::cerr << "Failed zip directory: directory is not exists";
        return false;
    }

    if ( !std::filesystem::is_directory(dir) ) {
        std::cerr << "Failed zip directory: is not directory";
        return false;
    }

    int errorp; zip_t *zipper = zip_open(zip.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr) {
        zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
        std::cerr << "Failed to open output file " + zip.string() + ": " + zip_error_strerror(&ziperror);

        return false;
    }

    zip_walk_directory(dir, zipper);
    zip_close(zipper);

    return true;
}

/*
bool unzipDirectory(const QString &dir, const QString &zip)
{
    return unzipDirectory(std::filesystem::path(dir.toStdString()), std::filesystem::path(zip.toStdString()));
}

bool unzipDirectory(const std::filesystem::path &dir, const std::filesystem::path &zip)
{
    return unzipDirectory(dir.string(), zip.string());
}
*/

bool unzipDirectory(const std::string &dir, const std::string &zip)
{
    return false;
}
