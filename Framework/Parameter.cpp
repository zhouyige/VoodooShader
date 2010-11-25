
#include "Parameter.hpp"

#include "Exception.hpp"
#include "Core.hpp"
#include "Converter.hpp"

namespace VoodooShader
{
	Parameter::Parameter(Core * parent, String name, ParameterType type)
		: mType(type)
	{ 
		mParam = cgCreateParameter(parent->GetCGContext(), Converter::ToCGType(type));

		switch ( this->mType )
		{
		case PT_Sampler1D:
		case PT_Sampler2D:
		case PT_Sampler3D:
			this->mValueTexture = NULL;
			break;
		case PT_Float4:
			this->mValueFloat[3] = 0.0f;
		case PT_Float3:
			this->mValueFloat[2] = 0.0f;
		case PT_Float2:
			this->mValueFloat[1] = 0.0f;
		case PT_Float1:
			this->mValueFloat[0] = 0.0f;
			break;
		case PT_Unknown:
		default:
			//! @todo Fix this, shouldn't Throw with a NULL Core
			Throw("Invalid parameter type.", NULL);
			break;
		}
	}

	CGparameter Parameter::Param(void)
	{
		return this->mParam;
	}

	ParameterType Parameter::Type(void)
	{
		return this->mType;
	}

	void Parameter::Set(TextureRef newTex)
	{
		this->mValueTexture = newTex;
	}

	void Parameter::Set(float newX)
	{
		this->mValueFloat[0] = newX;
	}

	void Parameter::Set(float newX, float newY)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
	}

	void Parameter::Set(float newX, float newY, float newZ)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
		this->mValueFloat[2] = newZ;
	}

	void Parameter::Set(float newX, float newY, float newZ, float newW)
	{
		this->mValueFloat[0] = newX;
		this->mValueFloat[1] = newY;
		this->mValueFloat[2] = newZ;
		this->mValueFloat[3] = newW;
	}

	void Parameter::Get(TextureRef & param)
	{
		param = this->mValueTexture;
	}

	void Parameter::Get(float & paramX)
	{
		paramX = this->mValueFloat[0];
	}

	void Parameter::Get(float & paramX, float & paramY)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
	}

	void Parameter::Get(float & paramX, float & paramY, float & paramZ)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
		paramZ = this->mValueFloat[2];
	}

	void Parameter::Get(float & paramX, float & paramY, float & paramZ, float & paramW)
	{
		paramX = this->mValueFloat[0];
		paramY = this->mValueFloat[1];
		paramZ = this->mValueFloat[2];
		paramW = this->mValueFloat[3];
	}
}