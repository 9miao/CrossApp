
#ifndef __CC_FILEUTILS_WIN32_H__
#define __CC_FILEUTILS_WIN32_H__

#include "platform/CAFileUtils.h"
#include "platform/CCPlatformMacros.h"
#include "ccTypes.h"
#include "ccTypeInfo.h"
#include <string>
#include <vector>

NS_CC_BEGIN

/**
* @addtogroup platform
* @{
*/

//! @brief  Helper class to handle file operations
class CC_DLL FileUtilsWin32 : public FileUtils
{
	friend class FileUtils;
	FileUtilsWin32();
public:
	/* override functions */
	bool init();
	virtual std::string getWritablePath();
	virtual bool isAbsolutePath(const std::string& strPath) const override;
	virtual std::string getSuitableFOpen(const std::string& filenameUtf8) const override;
	virtual long getFileSize(const std::string &filepath);
protected:

	virtual bool isFileExistInternal(const std::string& strFilePath) const override;

	/**
	*  Renames a file under the given directory.
	*
	*  @param path     The parent directory path of the file, it must be an absolute path.
	*  @param oldname  The current name of the file.
	*  @param name     The new name of the file.
	*  @return True if the file have been renamed successfully, false if not.
	*/
	virtual bool renameFile(const std::string &path, const std::string &oldname, const std::string &name) override;

	/**
	*  Renames a file under the given directory.
	*
	*  @param oldfullpath  The current path + name of the file.
	*  @param newfullpath  The new path + name of the file.
	*  @return True if the file have been renamed successfully, false if not.
	*/
	virtual bool renameFile(const std::string &oldfullpath, const std::string &newfullpath) override;

	/**
	*  Checks whether a directory exists without considering search paths and resolution orders.
	*  @param dirPath The directory (with absolute path) to look up for
	*  @return Returns true if the directory found at the given absolute path, otherwise returns false
	*/
	virtual bool isDirectoryExistInternal(const std::string& dirPath) const override;

	/**
	*  Removes a file.
	*
	*  @param filepath The full path of the file, it must be an absolute path.
	*  @return True if the file have been removed successfully, false if not.
	*/
	virtual bool removeFile(const std::string &filepath) override;

	/**
	*  Creates a directory.
	*
	*  @param dirPath The path of the directory, it must be an absolute path.
	*  @return True if the directory have been created successfully, false if not.
	*/
	virtual bool createDirectory(const std::string& dirPath) override;

	/**
	*  Removes a directory.
	*
	*  @param dirPath  The full path of the directory, it must be an absolute path.
	*  @return True if the directory have been removed successfully, false if not.
	*/
	virtual bool removeDirectory(const std::string& dirPath) override;

	/**
	*  Gets resource file data
	*
	*  @param[in]  filename    The resource file name which contains the path.
	*  @param[in]  mode        The read mode of the file.
	*  @param[out] size        If the file read operation succeeds, it will be the data size, otherwise 0.
	*  @return Upon success, a pointer to the data is returned, otherwise NULL.
	*  @warning Recall: you are responsible for calling delete[] on any Non-NULL pointer returned.
	*/
	virtual unsigned char* getFileData(const std::string& filename, const char* mode, unsigned long *size);

	/**
	*  Gets full path for filename, resolution directory and search path.
	*
	*  @param filename The file name.
	*  @param resolutionDirectory The resolution directory.
	*  @param searchPath The search path.
	*  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
	*/
	virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const override;

	/**
	*  Gets full path for the directory and the filename.
	*
	*  @note Only iOS and Mac need to override this method since they are using
	*        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
	*        Other platforms will use the default implementation of this method.
	*  @param directory The directory contains the file we are looking for.
	*  @param filename  The name of the file.
	*  @return The full path of the file, if the file can't be found, it will return an empty string.
	*/
	virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const override;
};

// end of platform group
/// @}

NS_CC_END


#endif    // __CC_FILEUTILS_WIN32_H__

