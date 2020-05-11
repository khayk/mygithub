#ifndef FILE_FINDER_H__
#define FILE_FINDER_H__

#include <filesystem>

/// Discover files in the specified path
class FileFinder {
public:

    /// Constructor
    /// @param recursiveScan  whether to scan sub folders also
    /// @param rootPath       full path to a directory
    /// @param extensions     semicolon separated list of extensions (example, exe;dll;cpp),
    ///                       if it is empty all files will be discovered
    /// @param ignoredFiles   semicolon separated list of files to be ignored
    ///                       while scanning the root path. files should be specified
    ///                       related to the root path
    ///                       [example 'folder/file.ext' will be searched in
    ///                       'rootPath/folder/file.ext']
    FileFinder(
        bool recursiveScan,
        const string_t& rootPath,
        const string_t& extensions = "",
        const string_t& ignoredFiles = "" );

    ~FileFinder();

    /// returns the relative path of discovered files
    const tStringSet& getFiles() const;

    /// returns the relative path of discovered directories
    const tStringSet& getDirs() const;

    /// root path ends with a slash
    const string_t&   getRootPath() const;

    /// returns number of directories discovered in a root path
    size_t            numDirectories() const;

    /// returns number of files discovered in a root path
    size_t            numFiles() const;

private:

    void performScan(const std::filesystem::path& path, const std::set<string_t>& exts);

private:
    tStringSet ignore_;
    tStringSet files_;
    tStringSet dirs_;
    string_t   rootPath_;
    size_t     numDirs_;
    bool       recursiveScan_;
};

#endif // FILE_FINDER_H__

