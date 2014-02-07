#include "StdAfx.h"
#include "FileFinder.h"

#include "../Utils/Common.h"

#include <Poco/Path.h>
#include <Poco/StringTokenizer.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/// check if the file extension specified by 'it' matches with one from exts
/// @param  itr    extension string
/// @param  exts   list to extensions to look for matches
/// @return        true if exts is empty or if match found
bool isExtensionMatches(const fs::directory_iterator& itr, const std::set<string_t>& exts)
{
    string_t e = itr->path().extension().string();
    if (!e.empty())
        e = e.substr(1);
    return (exts.empty() || exts.find( e ) != exts.end());
}


FileFinder::FileFinder(
    bool recursiveScan,
    const string_t& rootPath,
    const string_t& extensions /*= ""*/,
    const string_t& ignoredFiles /*= ""*/ )
    : rootPath_(rootPath)
    , recursiveScan_(recursiveScan)
    , numDirs_(0)
{
    /// have normalized string
    Poco::Path tmp(rootPath);
    rootPath_ = tmp.toString();

    Poco::StringTokenizer tokenizer(extensions, ";",
                                    Poco::StringTokenizer::TOK_IGNORE_EMPTY |
                                    Poco::StringTokenizer::TOK_TRIM);

    std::set<string_t> exts(tokenizer.begin(), tokenizer.end());

    Poco::StringTokenizer ignoreToks(
        ignoredFiles, ";",
        Poco::StringTokenizer::TOK_IGNORE_EMPTY |
        Poco::StringTokenizer::TOK_TRIM);

    for (auto it = ignoreToks.begin(); it != ignoreToks.end(); ++it) {
        /// the reason we are inserting paths through Poco::Path is
        /// that it will let us have paths similar path in each platform
        /// in windows with '\', in linux '/'
        ignore_.insert(Poco::Path(*it).toString());
    }

    performScan(rootPath_, exts);
}

FileFinder::~FileFinder()
{

}

const tStringSet& FileFinder::getFiles() const
{
    return files_;
}

const tStringSet& FileFinder::getDirs() const
{
    return dirs_;
}

void FileFinder::performScan( const boost::filesystem::path& dir_path, const std::set<string_t>& exts )
{
    if ( !fs::exists( dir_path ) )
        return;

    fs::directory_iterator end_itr; // default construction yields past-the-end
    for ( fs::directory_iterator itr( dir_path );
            itr != end_itr;
            ++itr ) {

        string_t::size_type pos = rootPath_.size();

        string_t fullPath   = toUtf8(itr->path().wstring());
        string_t relPath    = fullPath.substr(pos);

        if ( fs::is_directory(itr->status()) ) {
            ++numDirs_;
            dirs_.insert(relPath);

            if (recursiveScan_)
                performScan( itr->path(), exts );
        }
        else if ( fs::is_regular_file(itr->status()) ) { // see below
            if ( isExtensionMatches(itr, exts) ) {

                /// if this is not a file in the ignore list
                if ( ignore_.find(relPath) == ignore_.end() )
                    files_.insert(relPath);
            }
        }
    }
}

const string_t& FileFinder::getRootPath() const
{
    return rootPath_;
}

size_t FileFinder::numDirectories() const
{
    return numDirs_;
}

size_t FileFinder::numFiles() const
{
    return files_.size();
}

