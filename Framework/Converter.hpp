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

#ifndef VOODOO_CONVERTER_HPP
#define VOODOO_CONVERTER_HPP

#include "Meta.hpp"

namespace VoodooShader
{
	/**
	 * Conversion-helper class for Voodoo core types. This class provides 
	 * methods to convert various enums to other types or retrieve readable 
	 * names.
	 */
	class Converter
	{
	public:
		/**
		 * Convert a Voodoo parameter type to the corresponding Cg runtime type.
		 * This function should be used to convert between these two types, as 
		 * it allows for future types to be added or updated as necessary 
		 * without changing any other code.
		 * 
		 * @param pt The internal parameter type.
		 * @return The Cg runtime's corresponding type, if one is known.
		 */
		static inline CGtype ToCGType(ParameterType pt)
		{
			switch ( pt )
			{
			case PT_Sampler1D:
				return CG_SAMPLER1D;
			case PT_Sampler2D:
				return CG_SAMPLER2D;
			case PT_Sampler3D:
				return CG_SAMPLER3D;
			case PT_Float1:
				return CG_FLOAT;
			case PT_Float2:
				return CG_FLOAT2;
			case PT_Float3:
				return CG_FLOAT3;
			case PT_Float4:
				return CG_FLOAT4;
			case PT_Matrix:
				return CG_FLOAT4x4;
			case PT_Unknown:
			default:
				return CG_UNKNOWN_TYPE;
			}
		}

		/**
		* Convert a Cg runtime type to the corresponding Voodoo parameter type.
		* This function should be used to convert between these two types, as it
		* allows for future types to be added or updated as necessary without 
		* changing any other code.
		* 
		* @param pt The Cg runtime's type.
		* @return The internal parameter corresponding type, if one is known.
		*/
		static inline ParameterType ToParameterType(CGtype pt)
		{
			switch ( pt )
			{
			case CG_SAMPLER1D:
				return PT_Sampler1D;
			case CG_SAMPLER2D:
				return PT_Sampler2D;
			case CG_SAMPLER3D:
				return PT_Sampler3D;
			case CG_FLOAT:
				return PT_Float1;
			case CG_FLOAT2:
				return PT_Float2;
			case CG_FLOAT3:
				return PT_Float3;
			case CG_FLOAT4:
				return PT_Float4;
			case CG_FLOAT4x4:
				return PT_Matrix;
			case CG_UNKNOWN_TYPE:
			default:
				return PT_Unknown;
			}
		}

		static inline ParameterCategory ToParameterCategory(ParameterType type)
		{
			switch ( type )
			{
			case PT_Float1:
			case PT_Float2:
			case PT_Float3:
			case PT_Float4:
				return PC_Float;
			case PT_Sampler1D:
			case PT_Sampler2D:
			case PT_Sampler3D:
				return PC_Sampler;
			case PT_Matrix:
				return PC_Matrix;
			case PT_Unknown:
			default:
				return PC_Unknown;
			}
		}

		/**
		 * Helper function to convert enum values into readable strings used in 
		 * logging; this overload handles texture formats.
		 *
		 * @param tf The texture format to find the name of.
		 * @return A C-string with the name (usually identical to the 
		 *		identifier).
		 */
		static inline const char * ToString(TextureFormat tf)
		{
			switch ( tf )
			{
			case TF_RGB5:
				return "TF_RGB5";
			case TF_RGB5A1:
				return "TF_RGB5A1";
			case TF_RGB8:
				return "TF_RGB8";
			case TF_RGBA8:
				return "TF_RGBA8";
			case TF_RGB10A2:
				return "TF_RGB10A2";
			case TF_RGBA16F:
				return "TF_RGBA16f";
			case TF_RGBA32F:
				return "TF_RGBA32f";
			case TF_D16:
				return "TF_D16";
			case TF_D32:
				return "TF_D32";
			case TF_Unknown:
			default:
				return "TF_Unknown";
			}
		}

		/**
		* Helper function to convert enum values into readable strings used in 
		* logging; this overload handles parameter types.
		*
		* @param pt The parameter type to find the name of.
		* @return A C-string with the name (usually identical to the 
		*		identifier).
		*/
		static inline const char * ToString(ParameterType pt)
		{
			switch ( pt )
			{
			case PT_Sampler1D:
				return "PT_Sampler1D";
			case PT_Sampler2D:
				return "PT_Sampler2D";
			case PT_Sampler3D:
				return "PT_Sampler3D";
			case PT_Float1:
				return "PT_Float1";
			case PT_Float2:
				return "PT_Float2";
			case PT_Float3:
				return "PT_Float3";
			case PT_Float4:
				return "PT_Float4";
			case PT_Unknown:
			default:
				return "PT_Unknown";
			}
		}

		static inline const char * ToString(ParameterCategory cat)
		{
			switch ( cat )
			{
			case PC_Float:
				return "PC_Float";
			case PC_Sampler:
				return "PC_Sampler";
			case PC_Unknown:
			default:
				return "PC_Unknown";
			}
		}
	};
}

#endif /*VOODOO_CONVERTER_HPP*/
