/*
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2013 by Sean Sube
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

/**
 * @addtogroup voodoo_macros
 * @{
 */

/**
 * @defgroup voodoo_macros_debug Debug Macros
 * These will enable a significant amount of additional code and will cause major performance loss. However, full
 * memory management and program flow will be logged.
 * @{
 */
#if defined(VOODOO_DEBUG)
#   define VOODOO_DEBUG_BREAK   DebugBreak()
#   define VOODOO_CHECK_IMPL    if (!m_Impl) { Throw(VSTR("Extended Debug"), VSTR("Object has no implementation instance."), nullptr); }
#   define SAFE_INCREMENT(x)    InterlockedIncrement(&x)
#   define SAFE_DECREMENT(x)    InterlockedDecrement(&x)
#else
#   define VOODOO_DEBUG_BREAK
#   define VOODOO_CHECK_IMPL
#   define SAFE_INCREMENT(x)    ++x
#   define SAFE_DECREMENT(x)    --x
#endif

/**
 * @defgroup voodoo_macros_debug_log Extended Logging
 * @{
 */
#if defined(VOODOO_DEBUG) && defined(VOODOO_DEBUG_EXTLOG)
#   define VOODOO_DEBUG_FUNCLOG(logger) \
    { \
        if (logger)\
        {\
            logger->LogMessage(VSLog_Debug | VSLog_Critical | VSLog_System, VSTR("Extended Debug"), \
                StringFormat("Entered function %1% in %2% (line %3%).") << __FUNCTION__ << __FILE__ << __LINE__); \
        }\
    }
#else
#   define VOODOO_DEBUG_FUNCLOG(logger)
#endif
/**
 * @}
 * @defgroup voodoo_macros_debug_mem Memory Management
 * @{
 */
#if defined(VOODOO_DEBUG) && defined(VOODOO_DEBUG_MEMORY)
#   define _CRTDBG_MAP_ALLOC
#   include <stdlib.h>
#   include <crtdbg.h>
#   include <set>
#   define DeclareDebugCache() std::set<VOODOO_DEBUG_TYPE *> DebugCache_##VOODOO_DEBUG_TYPE
#   define AddThisToDebugCache() DebugCache_##VOODOO_DEBUG_TYPE.insert(this)
#   define RemoveThisFromDebugCache() DebugCache_##VOODOO_DEBUG_TYPE.erase(this)
#   define vnew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#   define vdelete delete
#else
#   define DeclareDebugCache()
#   define AddThisToDebugCache()
#   define RemoveThisFromDebugCache()
#   define vnew new
#   define vdelete delete
#endif
/**
 * @}
 * @}
 * @}
 */
