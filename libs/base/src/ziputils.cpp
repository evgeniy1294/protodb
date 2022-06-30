#include <protodb/utils/ziputils.h>


#include <zip.h>

#include <QString>

#include <cstring>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

static bool is_zip_directory(struct zip_stat& zs) {
    auto length = std::strlen(zs.name);
    return (length > 0) && zs.name[length-1] == '/';
}

static bool zip_walk_directory(zip_t* zipper, const std::filesystem::path &dir, const std::filesystem::path& relative) {

    for (const fs::directory_entry& entry: fs::directory_iterator(dir)) {
        if (is_directory(entry)) {
            if (zip_dir_add(zipper, fs::relative(entry.path(), relative).c_str(), ZIP_FL_ENC_UTF_8) < 0) {
                std::cerr << "Failed to add directory to zip: " + std::string(zip_strerror(zipper));
                return false;
            }

            if (!zip_walk_directory(zipper, entry.path(), relative)) {
                return false;
            }
        }
        else {
            zip_source_t *source = zip_source_file(zipper, entry.path().c_str(), 0, 0);
            if (source == nullptr) {
                std::cerr << "Failed to add file to zip: " + std::string(zip_strerror(zipper));
                return false;
            }

            if (zip_file_add(zipper, fs::relative(entry.path(), relative).c_str(), source, ZIP_FL_ENC_UTF_8 | ZIP_FL_OVERWRITE) < 0) {
                zip_source_free(source);
                std::cerr << "Failed to add file to zip: " + std::string(zip_strerror(zipper));
                return false;
            }
        }
    }

    return true;
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
        std::cerr << "Failed zip directory: directory is not exists" << std::endl;
        return false;
    }

    if ( !std::filesystem::is_directory(dir) ) {
        std::cerr << "Failed zip directory: is not directory" << std::endl;
        return false;
    }

    int errorp; zip_t *zipper = zip_open(zip.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr) {
        zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
        std::cerr << "Failed to open output file " + zip.string() + ": " + zip_error_strerror(&ziperror) << std::endl;

        return false;
    }

    bool ret = zip_walk_directory(zipper, dir, dir);
    zip_close(zipper);

    if (!ret) {
        fs::remove(zip);
    }

    return ret;
}

bool unzipToDirectory(const QString &zip, const QString &dir)
{
    return unzipToDirectory(fs::path(zip.toStdString()), fs::path(dir.toStdString()));
}

bool unzipToDirectory(const std::string &zip, const std::string &dir)
{
    return unzipToDirectory(fs::path(zip), fs::path(dir));
}

bool unzipToDirectory(const std::filesystem::path& zip, const std::filesystem::path& dir)
{
    if ( !std::filesystem::exists(zip) ) {
        std::cerr << "Failed zip directory: directory is not exists" << std::endl;
        return false;
    }

    int errorp; zip_t *za = zip_open(zip.c_str(), 0, &errorp);
    if (za == nullptr) {
        zip_error_t ziperror;
            zip_error_init_with_code(&ziperror, errorp);
        std::cerr << "Can't open zip archive " + zip.string() + ": " + zip_error_strerror(&ziperror) << std::endl;

        return false;
    }

    zip_file* zf;
    struct zip_stat zs;
    char buf[100]; bool ok = true;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if ( zip_stat_index(za, i,0, &zs) == 0 ) {

            if (is_zip_directory(zs)) {
                fs::create_directories(dir/zs.name);
            }
            else {
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    std::cerr << "Can't unzip file: " << zs.name;
                    ok = false; break;
                }

                fs::path file_path = dir/zs.name;
                fs::create_directories( file_path.parent_path() );

                std::ofstream ofs(file_path);
                if (!ofs.is_open()) {
                    std::cerr << "File not created: " << file_path << std::endl;
                    ok = false; break;
                }
                std::size_t sum = 0;
                while (sum != zs.size) {
                    size_t len = zip_fread(zf, buf, sizeof(buf));
                    if (len < 0) {
                        std::cerr << "Can't read zip-file: " << zs.name;
                        ok = false; break;
                    }
                    ofs.write(buf, len);
                    sum += len;
                }
                ofs.close();
                zip_fclose(zf);
            }
        }
    }

    zip_close(za);

    if (!ok) {
        fs::remove_all(dir);
    }

    return ok;
}


