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
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; 
* if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
* Boston, MA  02110-1301 US
*
* Support and more information may be found at http://www.voodooshader.com, or by contacting the
* developer at peachykeen@voodooshader.com
\**************************************************************************************************/

#ifndef VOODOO_META_HPP
#define VOODOO_META_HPP

#ifndef VOODOO_IMPORT
#    define VOODOO_API __declspec(dllexport)
#else
#    define VOODOO_API __declspec(dllimport)
#endif

// Hide the DLL-interface warning
#pragma warning(disable:4251)

#include "Includes.hpp"
#include "Reference.hpp"

#define VOODOO_CORE_COPYRIGHT "\n\
    Voodoo Shader Framework, Copyright � 2010 by Sean Sube\n\
    The Voodoo Shader Framework comes with ABSOLUTELY NO WARRANTY.\n\
    This is free software and you are welcome to redistribute it under certain conditions.\n\
    Please see the included license file for more details.\n\n"

namespace VoodooShader
{
    class Core;
    class Adapter;
    class Logger;

    class FullscreenManager;
    class MaterialManager;

    class Shader;
    class Texture;
    
    class Technique;
    class Pass;
    class Parameter;

    class FileSystem;
    class File;
    class Image;

    class Exception;

    class Converter;

    struct Version
    {
        int Major;
        int Minor;
        long Patch;
        long Rev;
    };

    struct TextureDesc
    {
        size_t Width;
        size_t Height;
        size_t Depth;
        bool Mipmaps;
        TextureFormat Format;
    };

    const size_t VersionSize = sizeof(Version);

#define Throw(module, msg, core) throw Exception(module, msg, core, __FILE__, __FUNCTION__, __LINE__);

    typedef std::string String;

    // Reference-counted pointer types
    typedef Reference<Shader>                    ShaderRef;
    typedef Reference<Texture>                    TextureRef;
    typedef Reference<Technique>                TechniqueRef;
    typedef Reference<Pass>                        PassRef;
    typedef Reference<Parameter>                ParameterRef;
    typedef Reference<File>                        FileRef;
    typedef Reference<Image>                    ImageRef;

    // Shader collections
    typedef std::map<String, ShaderRef>            ShaderMap;
    typedef std::list<ShaderRef>                ShaderList;
    typedef std::vector<ShaderRef>                ShaderVector;

    // Technique collections
    typedef std::map<String, TechniqueRef>        TechniqueMap;
    typedef std::list<TechniqueRef>                TechniqueList;
    typedef std::vector<TechniqueRef>            TechniqueVector;

    // Pass collections
    typedef std::map<String, PassRef>            PassMap;
    typedef std::list<PassRef>                    PassList;
    typedef std::vector<PassRef>                PassVector;

    // Parameter collections
    typedef std::map<String, ParameterRef>        ParameterMap;
    typedef std::list<ParameterRef>                ParameterList;
    typedef std::vector<ParameterRef>            ParameterVector;

    // Texture collections
    typedef std::map<String, TextureRef>        TextureMap;
    typedef std::list<TextureRef>                TextureList;
    typedef std::vector<TextureRef>                TextureVector;

    // Miscellaneous collections
    typedef std::map<TextureRef, ShaderRef>        MaterialMap;
    typedef std::map<String, CGeffect>            CGEffectMap;
    typedef std::vector<String>                    StringVector;
    typedef std::list<String>                    StringList;

    /**
     * Texture formats for use by @ref VoodooShader::Texture "Textures". These 
     * may not be  implemented by the underlying graphics API exactly as they 
     * are indicated  here, but the available components and sizes are 
     * guaranteed to be equal to or greater than the indicated values. Further 
     * information on texture formats and depth may be found on the 
     * @ref Textures "texture formats page".
     */
    enum TextureFormat
    {
        TF_Unknown = 0,        /*!< Unknown texture format */
        // Backbuffer formats
        TF_RGB5,            /*!< 5 bit RGB (1 bit X in DX, may be R5G6B5 in OGL) */
        TF_RGB5A1,            /*!< 5 bit RGB, 1 bit alpha */
        TF_RGB8,            /*!< 8 bit RGB (8 bit X in DX) */
        TF_RGBA8,            /*!< 8 bit RGBA */
        TF_RGB10A2,            /*!< 10 bit RGB, 2 bit A */
        // Float texture formats
        TF_RGBA16F,            /*!< Half-precision RGBA */
        TF_RGBA32F,            /*!< Full-precision RGBA (float/single) */
        // Depth-buffer formats
        TF_D16,                /*!< Half-precision depth (Z-buffer, see @ref depthbuffers "the depth buffers notes" for more info) */
        TF_D32,                /*!< Full-precision depth (Z-buffer, see @ref depthbuffers "the depth buffers notes" for more info) */
        // Max
        TF_Count            /*!< Enumerator values count. */
    };

    /**
     * Parameter types for use by @ref VoodooShader::Parameter "Parameters." 
     * These are generally handled within the Cg runtime and rarely change in
     * hardware, the common ones (float4 and such) are identical across the 
     * board.
     */
    enum ParameterType
    {
        PT_Unknown = 0,        /*!< Unknown parameter type */
        // Float-vectors
        PT_Float1,            /*!< Single-component float vector */
        PT_Float2,            /*!< Two-component float vector */
        PT_Float3,            /*!< Three-component float vector */
        PT_Float4,            /*!< Four-component float vector */
        // Samplers
        PT_Sampler1D,        /*!< One-dimensional sampler (for a 1D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler2D,        /*!< Two-dimensional sampler (for a 2D texture, see @ref texturetypes "texture types" for more info) */
        PT_Sampler3D,        /*!< Three-dimensional sampler (for a 3D/volume texture, see @ref texturetypes "texture types" for more info) */
        // Matrices
        PT_Matrix,            /*!< Generic float4x4 matrix type */
        // Max
        PT_Count            /*!< Enumerator values count */
    };

    enum ParameterCategory
    {
        PC_Unknown = 0,        /*!< Unknown parameter category */
        // Valid categories
        PC_Float,            /*!< Float vector parameter (may have 1 to 4 components) */
        PC_Sampler,            /*!< Sampler parameter (may sample 1D to 3D textures) */
        PC_Matrix,            /*!< Matrix parameter, 4x4 components */
        // Max
        PC_Count            /*!< Enumerator values count */
    };

    enum ProgramStage
    {
        PS_Unknown = 0,        /*!< Unknown program stage */
        // Basic stages
        PS_Vertex,            /*!< Vertex program stage (usually supported, see @ref programstages "program stages" for more info) */
        PS_Fragment,        /*!< Fragment program stage (usually supported, see @ref programstages "program stages" for more info) */
        // Geometry stages
        PS_Geometry,        /*!< Geometry program stage (sometimes supported, see @ref programstages "program stages" for more info) */
        // Tessellation stages
        PS_Domain,            /*!< Domain program stage (not always supported, see @ref programstages "program stages" for more info) */
        PS_Hull,            /*!< Hull program stage (not always supported, see @ref programstages "program stages" for more info) */
        // Max
        PS_Count            /*!< Enumerator values count */
    };

    enum TextureType
    {
        TT_Unknown = 0,        /*!< Unknown texture type */
        // Generic types
        TT_Generic,            /*!< Generic texture type, no special function */
        // Special types
        TT_ShaderTarget,    /*!< Shader target texture */
        TT_PassTarget,        /*!< Pass target texture */
        // Max
        TT_Count            /*!< Enumerator values count */
    };

    enum FileOpenMode
    {
        FM_Unknown = 0,
        // Valid modes
        FM_Read,
        FM_Write,
        // Max
        FM_Count
    };

    /**
     * Log message levels. These are spread out so additional levels can be
     * added in the future without affecting behavior.
     */
    enum LogLevel
    {
        LL_Unknown  = 0x00,
        // Working values
        LL_Debug    = 0x10, // Verbose debug log messages
        LL_Info     = 0x20, // Informational log messages
        LL_Warning  = 0x30, // Warning log messages
        LL_Error    = 0x40, // General error log messages
        LL_Fatal    = 0x50, // Fatal error log messages
        // Max value
        LL_Max
    };
}

#endif /*VOODOO_META_HPP*/
