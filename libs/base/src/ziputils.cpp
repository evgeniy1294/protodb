#include <protodb/utils/ziputils.h>


#include <zip.h>

#include <QString>

#include <cstring>

#include <iostream>

namespace fs = std::filesystem;

static bool is_zip_directory(struct zip_stat& zs) {
    auto length = std::strlen(zs.name);
    return (length > 0) && zs.name[length-1] == '/';
}

static void zip_walk_directory(zip_t* zipper, const std::filesystem::path &dir, const std::filesystem::path& relative) {

    for (const fs::directory_entry& entry: fs::directory_iterator(dir)) {
        if (is_directory(entry)) {
            if (zip_dir_add(zipper, fs::relative(entry.path(), relative).c_str(), ZIP_FL_ENC_UTF_8) < 0) {
                std::cerr << "Failed to add directory to zip: " + std::string(zip_strerror(zipper));
                continue;
            }

            zip_walk_directory(zipper, entry.path(), relative);
        }
        else {
            zip_source_t *source = zip_source_file(zipper, entry.path().c_str(), 0, 0);
            if (source == nullptr) {
                std::cerr << "Failed to add file to zip: " + std::string(zip_strerror(zipper));
                continue;
            }

            if (zip_file_add(zipper, fs::relative(entry.path(), relative).c_str(), source, ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE) < 0) {
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

    zip_walk_directory(zipper, dir, dir);
    zip_close(zipper);

    return true;
}

bool unzipDirectory(const QString &zip, const QString &dir)
{
    return unzipDirectory(fs::path(zip.toStdString()), fs::path(dir.toStdString()));
}

bool unzipDirectory(const std::string &zip, const std::string &dir)
{
    return unzipDirectory(fs::path(zip), fs::path(dir));
}

bool unzipDirectory(const std::filesystem::path& zip, const std::filesystem::path& dir)
{
    if ( !std::filesystem::exists(zip) ) {
        std::cerr << "Failed zip directory: directory is not exists";
        return false;
    }

    int errorp; zip_t *za = zip_open(zip.c_str(), 0, &errorp);
    if (za == nullptr) {
        zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
        std::cerr << "Can't open zip archive " + zip.string() + ": " + zip_error_strerror(&ziperror);

        return false;
    }

    zip_file* zf;
    struct zip_stat zs;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if ( zip_stat_index(za, i,0, &zs) == 0 ) {
            std::cout << zs.name << std::endl;

            if (is_zip_directory(zs)) {

            }
            else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    std::cerr << "Can't unzip file: " << zs.name;
                }

                // Cоздать пустой файл по требуемому пути и открыть его
                // Создать промежуточный буфер
                // Считать данные из архивированного файла в буфер
                // Записать данные в файл
                // закрыть файл
                // закрыть архивный файл
            }
        }
    }

    return true;
}


