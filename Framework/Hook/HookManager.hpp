/**
 * This file is part of the Voodoo Shader Framework.
 *
 * Copyright (c) 2010-2011 by Sean Sube
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

#include <easyhook.h>

#define VOODOO_IMPORT
#define VOODOO_NO_CG
#define VOODOO_NO_PUGIXML
#include "VoodooFramework.hpp"

namespace VoodooShader
{
    namespace EasyHook
    {
        /**
         * Classes and functions related to the EasyHook-based implementation of IHookManager.
         *
         * @addtogroup voodoo_module_ehhook Voodoo/EHHook @{
         */
        typedef std::map<String, TRACED_HOOK_HANDLE> HookMap;

        const Version * API_ModuleVersion(void);
        const uint32_t API_ClassCount(void);
        const char * API_ClassInfo(_In_ uint32_t number);
        IObject * API_ClassCreate(_In_ uint32_t number, _In_ ICore * pCore);

        /* VSEHHookManager: e6f312b5-05af-11e1-9e05-005056c00008 */
        DEFINE_UUID_IMPL(VSEHHookManager, 0xB5, 0x12, 0xF3, 0xE6, 0xAF, 0x05, 0xE1, 0x11, 0x9E, 0x05, 0x00, 0x50, 0x56, 0xC0, 0x00, 0x08);

        /**
         * Handles function-level hooks, redirecting function calls into new locations.
         */
        class VSEHHookManager :
            public IHookManager
        {
        public:
            /* Creates a new VSEHHookManager bound to the current process. */
            VSEHHookManager(_In_ ICore * pCore);

            /* Removes all hooks and cleans up the VSEHHookManager. */
            virtual ~VSEHHookManager(void);

            virtual uint32_t AddRef(void) const throw();
            virtual uint32_t Release(void) const throw();
            virtual bool CheckedCast(_In_ Uuid & clsid, _Deref_out_opt_ const void ** ppOut) const throw();
            virtual String ToString(void) const throw();
            virtual ICore * GetCore(void) const throw();

            /**
             * Install a hook at the specified function.
             *
             * @param name The name for the hook.
             * @param pSrc The point to install the hook at.
             * @param pDest The function to redirect execution into.
             * @return The success of the hook installation.
             *
             * @note The name is often the name of the function in pSrc (&func) for simplicities sake.
             *
             * @warning The calling convention of pSrc and pDest must be identical, or bad things might happen. This is only
             *      a bother with member functions, but can be worked around relatively easily.
             */
            bool Add(_In_ const String & name, _In_ void * pSrc, _In_ void * pDest);

            /**
             * Removes a single hook.
             *
             * @param name The name of the hook to remove.
             * @return The success of the removal operation.
             *
             * @warning <em>Do not</em>, under any circumstances, remove a hook while execution is passing through the
             *      trampoline function. This will likely crash the process.
             */
            bool Remove(_In_ const String & name);

            /**
             * Removes all hooks created with this VSEHHookManager.
             */
            void RemoveAll(void);

        private:
            mutable uint32_t m_Refs;
            ICore * m_Core;

            HookMap m_Hooks;

            unsigned long m_ThreadCount;
            unsigned long * m_ThreadIDs;
        };
        /**
         * @}
         */
    };
}
