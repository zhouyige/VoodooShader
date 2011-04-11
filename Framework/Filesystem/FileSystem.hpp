/**************************************************************************************************\
 * This file is part of the Voodoo Shader Framework, a comprehensive shader support library.
 * Copyright (c) 2010-2011 by Sean Sube
 *
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation; either version 2 of the 
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; 
 * if  not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301 US
 *
 * Support and more information may be found at http://www.voodooshader.com, or by contacting the
 * developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#define VOODOO_IMPORT
#include "Voodoo_Core.hpp"

namespace VoodooShader
{
    /**
     * Primary Voodoo filesystem implementation. This provides a thin wrapper for the Windows file
     * access API and DevIL, matching them to the Voodoo interfaces.
     * 
     * @note This module exports 1 class, named @p WFileSystem (@ref VoodooWFS::FileSystem) and 
     *    provides implementations of @ref IFileSystem, @ref IFile and @ref IImage. The following
     *    global variables are added when the filesystem object is created:
     *    @li <code>\$(mygames)</code> Current user's My Games folder.
     *    @li <code>\$(allgames)</code> Shared (all user's) My Games folder.
     *    @li <code>\$(systemroot)</code> System path (e.g., <code>C:\\Windows\\System32</code>)
     * 
     * @addtogroup VoodooWFileSystem Voodoo/WFileSystem
     * @{
     */
    namespace VoodooWFS
    {
        typedef std::list<String> StringList;

        Version API_ModuleVersion();
        int API_ClassCount();
        const char * API_ClassInfo(_In_ int number);
        IObject * API_ClassCreate( _In_ int number, _In_ Core * core );

        /**
         * Provides a unified file management system for loading shaders and other resources. This 
         * file system implementation is a thin wrapper for the Windows API, adding only a few 
         * nonstandard functions (directory searching and path variables).
         */
        class FileSystem
            : public IFileSystem
        {
        public:
            FileSystem
            (
                _In_ Core * core
            );

            ~FileSystem();

            const char * GetObjectClass();

            /**
             * Add a directory to the search path. Directories are pushed to the front of the list, 
             * which is searched in order (last added has highest priority).
             * 
             * @note This function uses Parser::ParseString() for all paths. Variables are 
             *    evaluated when the path is added.
             */
            void AddDirectory
            (
                _In_ String dir
            );

            void RemoveDirectory
            (
                _In_ String dir
            );

            /**
             * Resolves a relative filename (usually just filename and extension,
             * but may include directories) into an absolute filename by searching
             * the list of resource directories registered with this manager.
             *
             * @param name The filename to search for.
             * @return If the file is found, a reference to an unopened file object
             *      is returned. If no file is found, an empty reference is returned.
             *      
             * @note This functions uses Parser::ParseString() on the path. It is evaluated once
             *    when the function enters, then appended to each stored path until a valid file is
             *    found.
             */
            IFileRef GetFile
            (
                _In_ String name
            );

            IImageRef GetImage
            (
                _In_ String name
            );

        private:
            StringList mDirectories;
            Core * mCore;
        };

        class File
            : public IFile
        {
        public:
            /**
             * Creates a file object from a path (usually an absolute path). This should usually 
             * not be called directly, FileManager::GetFile(String) will automatically resolve and 
             * return paths to simplify things.
             */
            File
            (
                _In_ Core * core, 
                _In_ String name
            );

            ~File();

            const char * GetObjectClass();

            /**
             * Retrieves the path this File was created with. This may be relative or absolute.
             * 
             * @return Internal path.
             */
            String GetPath();

            /**
             * Opens the file for read-write access. 
             * 
             * @param mode The mode to open the file in.
             * @return Whether the file was successfully opened.
             *
             * @note If this file handle was returned by a FileSystem and the absolute path is 
             *    still valid, this function should always succeed. In single-threaded or 
             *    fullscreen scenarios with few user tasks, this is usually the case.
             * @warning If this file handle uses a relative path, then it is subject to changes in 
             *    working directory by the any module in the process. FileSystem::GetFile() uses an 
             *    absolute path in the constructor and is not susceptible to this.
             */
            bool Open
            (
                _In_ FileOpenMode mode
            );

            /**
             * Closes the file, preventing further access.
             * 
             * @return Success of the operation
             */
            bool Close();

            /**
             * Reads a chunk of data from the file. The file must have been opened for reading 
             * previously, or this call will fail.
             * 
             * @param count The number of bytes to read, -1 for all.
             * @param buffer The buffer to be read into (may be null, see notes).
             * @return The number of bytes read.
             * 
             * @note If @arg buffer is null, the number of bytes that would have been read is 
             *    returned but the file position is unchanged. If @arg count is -1, this returns 
             *    the filesize remaining.
             */
            int Read(_In_ int count, _In_opt_count_(count) void * buffer);

            /**
             * Writes a chunk of data to the file. The file must have been opened for writing.
             * 
             * @param count The number of bytes to write.
             * @param buffer The data to write.
             * @return Success of the write operation.
             * 
             * @note If @p buffer is null, @p count zeros are written into the file. This is 
             *    useful for padding binary formats.
             */
            bool Write(_In_ int count, _In_opt_count_(count) void * buffer);

        private:
            HANDLE mHandle;
            String mName;
            Core * mCore;
        };

        /**
         * Provides image loading, using the DevIL library. This class provides internal loading 
         * and conversion, and can manage 1-3 dimensional images (regular textures and volumes).
         * 
         * @todo Provide image saving.
         * @todo Provide layer, cubemap and animation handling.
         */
        class Image
            : public IImage
        {
        public:
            static IImageRef Load(Core * core, String name);

            Image(Core * core, String name, unsigned int image);

            ~Image();

            const char * GetObjectClass();

            /**
             * Retrieves texture format data from the image. The runtime can use this to set up a 
             * texture or decide if it can handle the texture format.
             * 
             * @return Texture information.
             */
            TextureDesc GetImageDesc();
            
            /**
             * Retrieves a portion of the texture data from the image.
             * 
             * @param desc The region and format to be returned.
             * @param buffer The memory for the return data to be placed in. Must already be 
             *    allocated, or null.
             * @return The number of bytes retrieved (or, if @p buffer is null, the number that 
             *    would be retrieved).
             * @throws Exception on invalid texture format.
             *      
             * @warning Due to limitations in this library (or DevIL, not sure which), the only 
             *    texture formats this function can convert into are @ref TF_RGB8, @ref TF_RGBA8, 
             *    @ref TF_RGBA16F and @ref TF_RGBA32F. Others are not supported and will cause this 
             *    function to throw.
             * @note This can convert data between most formats, so the format given in @p desc 
             *    will be the returned format. This makes calculating the buffer size relatively 
             *    easy.
             * @warning If this function converts formats or copies a large region, it will be 
             *    slow. Avoid calling often.
             */
            size_t CopyImageData(_In_ TextureRegion desc, _In_opt_ void * buffer);

            /**
             * Retrieves a pointer to the image data.
             * 
             * @return Pointer to the image data.
             * 
             * @warning The pointer provided <em>must not</em> be deleted. To free the data, call 
             *    @ref Image::FreeImageData(). Only one image can be bound/accessed through this
             *    method at a time, making it thread-unsafe. Using @ref Image::CopyImageData is
             *    recommended for simple image access.
             */
            void * GetImageData();

            void FreeImageData();

        private:
            Core * mCore;
            ParserRef mParser;
            String mName;
            unsigned int mImage;
            TextureDesc mDesc;
        };
    }
}
/**
 * @}
 */
