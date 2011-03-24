#include "Shader.hpp"

#include "Adapter.hpp"
#include "Converter.hpp"
#include "Core.hpp"
#include "Exception.hpp"

namespace VoodooShader
{
    Shader::Shader(Core * parent, String filename, const char ** args)
        : mCore(parent), mName(filename), mDefaultTechnique()
    {
        this->mEffect = cgCreateEffectFromFile
        (
            mCore->GetCgContext(), 
            mName.c_str(), 
            args
        );

        if ( !cgIsEffect(this->mEffect) )
        {
            //Throw(VOODOO_CORE_NAME, "Failed to create shader.", mCore);
            return;
        }

        cgSetEffectName(this->mEffect, this->mName.c_str());
    }
        
    Shader::Shader(Core * parent, CGeffect effect)
        : mCore(parent), mEffect(effect)
    {
        this->mName = cgGetEffectName(effect);
    }

    TechniqueRef Shader::GetDefaultTechnique()
    {
        return this->mDefaultTechnique;
    }

    size_t Shader::GetParameterCount()
    {
        return mParameters.size();
    }

    ParameterRef Shader::GetParameter(size_t index)
    {
        if ( index < mParameters.size() )
        {
            ParameterMap::iterator param = mParameters.begin();

            //! @todo Hack. Fix this (get item from std::map based on position).
            size_t pos = 0;
            while ( pos < index )
            {
                ++param;
                ++pos;
            }

            return param->second;
        } else {
            return ParameterRef();
        }
    }

    ParameterRef Shader::GetParameter(String name)
    {
        ParameterMap::iterator param = mParameters.find(name);
        if ( param != mParameters.end() )
        {
            return param->second;
        } else {
            return ParameterRef();
        }
    }

    String Shader::GetName()
    {
        return mName;
    }

    Core * Shader::GetCore()
    {
        return mCore;
    }

    void Shader::Link()
    {
        // Link parameters first
        CGparameter cParam = cgGetFirstEffectParameter(this->mEffect);

        while ( cgIsParameter(cParam) )
        {
            const char * name = cgGetParameterName(cParam);
            ParameterRef param(new Parameter(this, cParam));

            this->mParameters[name] = param;

            this->LinkParameter(param);

            cParam = cgGetNextParameter(cParam);
        }

        this->SetupTechniques();
    }

    void Shader::LinkParameter(ParameterRef param)
    {
        // Cache basic data for future use 
        ParameterType type = param->GetType();
        CGparameter cgparam = param->GetCgParameter();

        // Check if it has a global link annotation
        CGannotation globalAnnotation = cgGetNamedParameterAnnotation(
                cgparam, "parameter");

        if ( cgIsAnnotation(globalAnnotation) && cgGetAnnotationType(globalAnnotation) == CG_STRING )
        {
            const char * globalName = cgGetStringAnnotationValue(globalAnnotation);
            if ( globalName != NULL && strlen(globalName) > 0 )
            {
                ParameterRef globalParam = mCore->GetParameter(globalName, type);
                if ( globalParam.get() )
                {
                    globalParam->Attach(param);
                } else {
                    mCore->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME, 
                        "Unable to find global param %s for parameter %s.",
                        globalName, param->GetName().c_str()
                    );
                }
            } else {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME, 
                    "Unable to read global annotation for parameter %s.",
                    param->GetName().c_str()
                );
            }

            return;
        }

        // If it's not linked to a global, it doesn't need linked unless it is
        // a sampler.
        if (  Converter::ToParameterCategory(type) == PC_Sampler )
        {
            this->LinkSampler(param);
        }
    }

    void Shader::LinkSampler(ParameterRef param)
    {
        CGparameter cgparam = param->GetCgParameter();

        // Link to a texture
        CGannotation textureAnnotation = cgGetNamedParameterAnnotation(
            cgparam, "texture");

        if ( cgIsAnnotation(textureAnnotation) && cgGetAnnotationType(textureAnnotation) == CG_STRING )
        {
            const char * textureName = cgGetStringAnnotationValue(textureAnnotation);
            if ( textureName != NULL && strlen(textureName) > 0 )
            {
                TextureRef texture = mCore->GetTexture(textureName);
                if ( texture.get() )
                {
                    mCore->GetAdapter()->ConnectTexture(param, texture);
                } else {
                    mCore->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Could not find texture %s for parameter %s, attempting to create",
                        textureName, param->GetName().c_str()
                    );

                    this->CreateParameterTexture(param);
                }
            } else {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Could not retrieve texture name for parameter %s.",
                    param->GetName().c_str()
                );
            }
        } else {
            mCore->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "Could not retrieve texture annotation for parameter %s.",
                param->GetName().c_str()
            );
        }
    }

    void Shader::CreateParameterTexture( _In_ ParameterRef param )
    {
        CGparameter parameter = param->GetCgParameter();

        if ( !cgIsParameter(parameter) )
        {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Could not create parameter texture for unknown parameter."
            );

            return;
        }

        CGannotation atexName   = cgGetNamedParameterAnnotation(parameter, "texture");
        CGannotation atexSize   = cgGetNamedParameterAnnotation(parameter, "size");
        CGannotation atexFormat = cgGetNamedParameterAnnotation(parameter, "format");

        if ( !(cgIsAnnotation(atexName) && cgIsAnnotation(atexSize) && cgIsAnnotation(atexFormat) ) )
        {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Could not create texture for parameter %s, required fields were missing.",
                param->GetName().c_str()
            );

            return;
        }

        String texName;

        if ( cgGetAnnotationType(atexName) != CG_STRING )
        {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture name annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        } else {
            texName = cgGetStringAnnotationValue(atexName);
        }

        CGtype texSizeType = cgGetAnnotationType(atexSize);
        TextureDesc texDesc;

        if ( texSizeType ==  CG_INT1 )
        {
            mCore->Log
             (
                LL_Debug,
                VOODOO_CORE_NAME,
                "1-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDesc.Depth = 1;
        } else if ( texSizeType == CG_INT2 ) {
            mCore->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "2-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDims[1];
            texDesc.Depth = 1;
        } else if ( texSizeType == CG_INT3 ) {
            mCore->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "3-dimensional texture size found."
            );

            int outCount;
            const int * texDims = cgGetIntAnnotationValues(atexSize, &outCount);
            texDesc.Width = texDims[0];
            texDesc.Height = texDims[1];
            texDesc.Depth = texDims[2];
        } else if ( texSizeType == CG_INT4 ) {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "4-dimensional texture size found. Creating quantum texture... Oh wait, not really (invalid type on %s).",
                param->GetName().c_str()
            );        
        } else {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture size annotation type for parameter %s.",
                param->GetName().c_str()
            );
        }

        texDesc.Mipmaps = false;

        if ( cgGetAnnotationType(atexFormat) != CG_STRING )
        {
            mCore->Log
            (
                LL_Error,
                VOODOO_CORE_NAME,
                "Invalid texture format annotation type in %s.",
                param->GetName().c_str()
            );

            return;
        } else {
            const char * formatString = cgGetStringAnnotationValue(atexFormat);
            texDesc.Format = Converter::ToTextureFormat(formatString);
        }

        IAdapter * adapter = mCore->GetAdapter();
        TextureRef texture = adapter->CreateTexture(texName, texDesc);
        param->Set(texture);
    }


    void Shader::SetupTechniques()
    {
        CGtechnique cTech = cgGetFirstTechnique(mEffect);
        while ( cgIsTechnique(cTech) )
        {
            CGbool valid = cgValidateTechnique(cTech);

            if ( valid == CG_TRUE )
            {
                mCore->Log
                (
                    LL_Debug,
                    VOODOO_CORE_NAME,
                    "Validated technique %s.",
                    cgGetTechniqueName(cTech)
                );

                // Insert the technique into the map
                TechniqueRef tech(new Technique(this, cTech));

                const char * techName = cgGetTechniqueName(cTech);
                mTechniques[techName] = tech;

                // The first valid technique is the default one
                if ( !mDefaultTechnique.get() )
                {
                    this->mDefaultTechnique = tech;
                }

                tech->Link();
            } else {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Technique failed to validate: %s.",
                    cgGetTechniqueName(cTech)
                );
            }

            cTech = cgGetNextTechnique(cTech);
        }
    }

    void Shader::SetDefaultTechnique(String name)
    {
        TechniqueRef tech = this->GetTechnique(name);
        if ( tech.get() )
        {
            this->mDefaultTechnique = tech;
        } else {
            Throw(VOODOO_CORE_NAME, "Technique not found in shader.", mCore);
        }
    }

    TechniqueRef Shader::GetTechnique(String name)
    {
        TechniqueMap::iterator tech = mTechniques.find(name);
        if ( tech != mTechniques.end() )
        {
            return tech->second;
        } else {
            return TechniqueRef();
        }
    }

    Technique::Technique(Shader * parent, CGtechnique cgTech)
        : mParent(parent), mTechnique(cgTech)
    {
        this->mCore = this->mParent->GetCore();

        const char * techName = cgGetTechniqueName(this->mTechnique);
        if ( techName )
        {
            this->mName = techName;
        } else {
            char nameBuffer[16];
            _itoa_s((int)(&this->mTechnique), nameBuffer, 16, 16);

            this->mName = "tech_";
            this->mName += nameBuffer;
        }
    }

    Core * Technique::GetCore()
    {
        return mCore;
    }

    String Technique::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
        return name;
    };

    PassRef Technique::GetPass(size_t index)
    {
        if ( index < this->mPasses.size() )
        {
            return this->mPasses[index];
        } else {
            Throw(VOODOO_CORE_NAME, "Voodoo Core: Invalid pass index (> pass count).", mCore);
        }
    }

    TextureRef Technique::GetTarget()
    {
        return mTarget;
    }

    size_t Technique::GetPassCount()
    {
        return this->mPasses.size();
    }

    void Technique::Link()
    {
        this->mTarget = TextureRef();

        // Process the technique's target annotation
        CGannotation targetAnnotation = cgGetNamedTechniqueAnnotation(this->mTechnique, "target");

        if ( cgIsAnnotation(targetAnnotation) )
        {
            if ( cgGetAnnotationType(targetAnnotation) == CG_STRING )
            {
                const char * targetName = cgGetStringAnnotationValue(targetAnnotation);

                this->mTarget = mCore->GetTexture(targetName);

                if ( !this->mTarget.get() )
                {
                    mCore->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Pass %s cannot find target %s.",
                        this->GetName().c_str(), targetName
                    );

                    this->mTarget = mCore->GetTexture(TT_ShaderTarget);
                }
            } else {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                );

                this->mTarget = mCore->GetTexture(TT_ShaderTarget);
            }
        } else {
            mCore->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.",
                this->GetName().c_str()
            );

            this->mTarget = mCore->GetTexture(TT_ShaderTarget);
        }


        this->mPasses.clear();

        CGpass cPass = cgGetFirstPass(mTechnique);

        while ( cgIsPass(cPass) )
        {
            // Insert the pass into the vector
            PassRef pass(new Pass(this, cPass));

            mPasses.push_back(pass);

            pass->Link();

            cPass = cgGetNextPass(cPass);
        }
    }

    Pass::Pass(Technique * parent, CGpass cgPass)
        : mParent(parent), mPass(cgPass)
    {
        this->mCore = this->mParent->GetCore();

        const char * passName = cgGetPassName(this->mPass);
        if ( passName )
        {
            this->mName = passName;
        } else {
            char nameBuffer[16];
            _itoa_s((int)(&this->mPass), nameBuffer, 16, 16);

            this->mName = "pass_";
            this->mName += nameBuffer;
        }
    }

    String Pass::GetName()
    {
        String name = mParent->GetName();
        name += "::";
        name += mName;
        return name;
    };

    Core * Pass::GetCore()
    {
        return mCore;
    }

    TextureRef Pass::GetTarget()
    {
        return mTarget;
    }

    CGprogram Pass::GetProgram(ProgramStage stage)
    {
        switch ( stage )
        {
        case PS_Vertex:
            return mVertexProgram;
        case PS_Fragment:
            return mFragmentProgram;
        case PS_Geometry:
            return mGeometryProgram;
        case PS_Domain:
        case PS_Hull:
        case PS_Unknown:
        default:
            return NULL;
        }
    }

    void Pass::Link()
    {
        this->mVertexProgram   = cgGetPassProgram(this->mPass, CG_VERTEX_DOMAIN  );
        this->mFragmentProgram = cgGetPassProgram(this->mPass, CG_FRAGMENT_DOMAIN);
        this->mGeometryProgram = cgGetPassProgram(this->mPass, CG_GEOMETRY_DOMAIN);

        this->mTarget = TextureRef();
        CGannotation targetAnnotation = cgGetNamedPassAnnotation(this->mPass, "target");
        if ( cgIsAnnotation(targetAnnotation) )
        {
            if ( cgGetAnnotationType(targetAnnotation) == CG_STRING )
            {
                const char * targetName = cgGetStringAnnotationValue(targetAnnotation);

                this->mTarget = mCore->GetTexture(targetName);

                if ( !this->mTarget.get() )
                {
                    mCore->Log
                    (
                        LL_Warning,
                        VOODOO_CORE_NAME,
                        "Pass %s cannot find target %s.", 
                        this->GetName().c_str(), targetName
                    );

                    this->mTarget = mCore->GetTexture(TT_PassTarget);
                }
            } else {
                mCore->Log
                (
                    LL_Warning,
                    VOODOO_CORE_NAME,
                    "Pass %s has annotation \"target\" of invalid type.",
                    this->GetName().c_str()
                );

                this->mTarget = mCore->GetTexture(TT_PassTarget);
            }
        } else {
            mCore->Log
            (
                LL_Debug,
                VOODOO_CORE_NAME,
                "Pass %s has no target annotation.", 
                this->GetName().c_str()
            );

            this->mTarget = mCore->GetTexture(TT_PassTarget);
        }

        // Load the programs
        IAdapter * adapter = mCore->GetAdapter();

        if ( !adapter )
        {
            mCore->Log
            (
                LL_Warning,
                VOODOO_CORE_NAME,
                "No adapter found, pass %s must be explicitly loaded later.", 
                this->GetName().c_str()
            );
        } else {
            if ( !adapter->LoadPass(this) )
            {
                mCore->Log(LL_Error, VOODOO_CORE_NAME, "Failed to load pass %s.", this->GetName().c_str());
            } else {
                mCore->Log(LL_Info, VOODOO_CORE_NAME, "Successfully loaded pass %s.", this->GetName().c_str());
            }
        }
    }
}
