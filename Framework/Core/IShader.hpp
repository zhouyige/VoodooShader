/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2012 by Sean Sube
 *
 * The Voodoo Shader Framework is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 US
 *
 * Support and more information may be found at
 *   http://www.voodooshader.com
 * or by contacting the lead developer at
 *   peachykeen@voodooshader.com
 */
#pragma once

#include "VoodooFramework.hpp"

namespace VoodooShader
{
    /**
     * @addtogroup voodoo_interfaces
     * @{
     */
    /**
     * @class IShader
     *
     * Complete shader class, managing techniques, passes and metadata.
     *
     * Shaders contain significant linking and processing data, as well as the Voodoo-specific linking stage. IShader file
     * loading and compilation are handled internally, with the shader being compiled from a VSFX file.
     *
     * @note A shader may contain multiple @ref ITechnique techniques. IShader techniques must be separately validated,
     *     only techniques valid on the current hardware will be usable in the shader.
     *
     * @warning For any technique in a shader to be valid, the entire shader must compile without errors. This
     *     <em>does not</em> mean that all techniques will be valid. Care must be taken when using the latest profile
     *     keyword, as this may cause unsupported techniques to generate compile errors, preventing all techniques
     *     from being used.
     *
     * @restag  None.
     * @iid     e6f31294-05af-11e1-9e05-005056c00008
     */
    VOODOO_INTERFACE(IShader, IResource, ({0x94, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08}))
    {
    public:
        /**
         * @name IObject Methods
         * @{
         */
        VOODOO_METHOD_(uint32_t, AddRef)() CONST PURE;
        VOODOO_METHOD_(uint32_t, Release)() CONST PURE;
        VOODOO_METHOD(QueryInterface)(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) CONST PURE;
        VOODOO_METHOD_(String, ToString)() CONST PURE;
        VOODOO_METHOD_(ICore *, GetCore)() CONST PURE;
        /**
         * @}
         * @name IResource Methods
         * @{
         */
        VOODOO_METHOD_(String, GetName)() CONST PURE;
        VOODOO_METHOD(GetTag)(_In_ Variant * pValue) CONST PURE;
        VOODOO_METHOD(SetTag)(_In_ const Variant & value) PURE;
        /**
         * @}
         * @name Source Code Methods
         * @{
         */
        /**
         * Retrieve a preprocessor definition from the shader header. If the symbol is not defined, an empty string is
         * returned.
         * 
         * @param name      The name of the define.
         */
        VOODOO_METHOD_(String, GetDefine)(const String & name) CONST PURE;
        /**
         * Set the value of a preprocessor definition.
         * 
         * @param name      The name of the define.
         * @param value     The value of the define.
         */
        VOODOO_METHOD(SetDefine)(const String & name, const String & value) PURE;
        /**
         * Retrieve the source code for the shader. Preprocessor defines and includes are left in their original form, but
         * variables are parsed out.
         */
        VOODOO_METHOD_(String, GetSource)() CONST PURE;
        /**
         * @}
         * @name Technique Methods
         * @{
         */
        /**
         * Get the number of validated techniques this shader contains.
         */
        VOODOO_METHOD_(uint32_t, GetTechniqueCount)() CONST PURE;
        /**
         * Retrieve a technique from the shader by name. Most cases should use the default technique, but some specific
         * applications may want a particular technique.
         *
         * @param index The index of the technique to get.
         */
        VOODOO_METHOD_(ITechnique *, GetTechnique)(const uint32_t index) CONST PURE;
        /**
         * Retrieves the default technique from this shader. All drawing should be done with the default technique: it is
         * guaranteed to be valid for the current shader profiles and API. The default technique is the first technique
         * to validate.
         *
         * @note To influence the priority of techniques, simply place the most specific or least compatible first (often
         *     high-quality ones) and count down with the most compatible and simplest technique last. The first valid
         *     technique found becomes the default.
         */
        VOODOO_METHOD_(ITechnique *, GetDefaultTechnique)() CONST PURE;
        /**
         * Set a technique from this shader to be used as the default technique. Some adapter functions simply retrieve the
         * default technique to draw with.
         *
         * @param pTechnique The technique within this shader to use.
         *
         * @note This will validate that the technique belongs to this shader.
         */
        VOODOO_METHOD(SetDefaultTechnique)(_In_ ITechnique * const pTechnique) PURE;
        /**
         * @}
         */
    };
    /**
     * @}
     */
}
