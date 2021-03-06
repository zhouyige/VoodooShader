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

#include "VoodooFramework.hpp"
#include "String.hpp"

namespace VoodooShader
{
    /**
     * @ingroup voodoo_utility
     * Voodoo internal regex class, providing constant and compiler-safe regex handling between modules. This internally
     * uses <a href="http://www.boost.org/doc/libs/1_49_0/libs/regex/doc/html/index.html">Boost.Regex</a> in extended-syntax
     * mode for both expressions and match formatting.
     *
     * @related String
     */
    class VOODOO_API Regex
    {
        class RegexImpl;

    public:
        /**
         * @name Regex Constructors
         * @{
         */
        /**
         * Creates an empty regular expression.
         */
        Regex();
        /**
         * Creates a regular expression from the given string, using extended syntax.
         *
         * @param expr The string to use.
         */
        Regex(_In_ CONST String & expr);
        /**
         * @}
         */
        ~Regex();
        /**
         * @name Regex Expression
         * @{
         */
        /**
         * Sets the internal expression and recompiles.
         *
         * @param expr The string to use.
         */
        void SetExpr(_In_ CONST String & expr);
        /**
         * Retrieve the current expression.
         *
         * @return The expression string.
         */
        String GetExpr() CONST;
        /**
         * @}
         * @name Regex Operations
         * @{
         */
        /**
         * Attempt to match the given string against the stored expression.
         *
         * @param string The string to search.
         * @return A RegexMatch for all match groups.
         */
        RegexMatch Match(_In_ CONST String & string) CONST;
        /**
         * Test if the given string is matched by the expression.
         *
         * @param find The string to search.
         * @return True if the expression matches the string in full.
         */
        bool Find(_In_ CONST String & find) CONST;
        /**
         * Perform a find and replace on all segments which this regex matches. Matches are replaced using the same rules as
         * RegexMatch::Replace().
         *
         * @param find The string to search.
         * @param replace The replace format string.
         * @return A copy of find with all matches replaced.
         */
        String Replace(_In_ CONST String & find, _In_ CONST String & replace) CONST;
        /**
         * @}
         */

    private:
        RegexImpl * m_Impl;
    };

    /**
     * @ingroup voodoo_utility
     *
     * Voodoo internal regex match class, encapsulating matches found by @ref Regex. This class provides methods to read
     * each match group, or use them as arguments to a formatted string.
     */
    class VOODOO_API RegexMatch
    {
        class RegexMatchImpl;
        friend class Regex;

    public:
        RegexMatch(_In_ CONST RegexMatch & other);

        ~RegexMatch();

        /**
         * Gets the number of submatches in this match.
         *
         * @return Total number of matches.
         */
        int32_t GetCount() CONST;

        /**
         * Check if any matches were found.
         *
         * @return True if any matches are contained.
         */
        bool IsEmpty() CONST;

        /**
         * Retrieve a particular match within this group.
         *
         * @return The specified submatch.
         */
        String GetMatch(_In_ CONST int32_t index) CONST;

        /**
         * Formats the found submatches into an output string. This uses the <code>$0</code> syntax to perform formatting,
         * all other text is copied.
         *
         * @param fmt The format string to use.
         * @return A formatted copy using the submatches.
         */
        String Format(_In_ CONST String & fmt) CONST;

    private:
        RegexMatch();

        RegexMatchImpl * m_Impl;
    };
}
