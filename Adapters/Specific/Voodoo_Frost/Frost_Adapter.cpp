#include "Frost_Adapter.hpp"
#include "VoodooGL.hpp"

#define VOODOO_OGL_HOOK_PARAMS(x) #x, &x, &v##x

namespace VoodooShader
{
    namespace Frost
    {
        Adapter::Adapter(_In_ Core * core)
            : mCore(core), mDC(NULL), mGLRC(NULL)
        {
            // Get the handles to the needed hook modules
            HMODULE procmodule = GetModuleHandle(NULL);
            if ( procmodule )
            {
                char procpath[MAX_PATH];
                GetModuleFileName(procmodule, procpath, MAX_PATH);
                
                mCore->Log(LL_Info, VOODOO_FROST_NAME, "Frost loaded into process \"%s\".", procpath);

                char * pos = strrchr(procpath, '\\');
                if ( pos != NULL )
                {
                    if ( strcmp(pos+1, "nwmain.exe") != 0 )
                    {
                        mCore->Log(LL_Warning, VOODOO_FROST_NAME, "The process does not appear to be the engine. Frost will not run.");
                        return;
                    }
                }
            } else {
                mCore->Log(LL_Error, VOODOO_FROST_NAME, "Unable to find target module. Frost will not run.");

                return;
            }

            VoodooCore = mCore;
            VoodooFrost = this;

            mCore->Log(LL_Debug, VOODOO_FROST_NAME, "Beginning OpenGL hook procedure.");

            IHookManager * hooker = mCore->GetHookManager();
            bool success = true;

            // System-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glGetString));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glViewport));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglCreateContext));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglDeleteContext));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglGetProcAddress));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglMakeCurrent));

            // Shader-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glClear));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(wglSwapLayerBuffers));

            // Material-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBindTexture));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDeleteTextures));

            // Shader/material shared
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glBegin));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glDrawElements));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnd));

            // Fog-related
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glEnable));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogf));
            success &= hooker->CreateHook(VOODOO_OGL_HOOK_PARAMS(glFogfv));

            // Check the results and handle
            if ( success )
            {
                mCore->Log(LL_Info, VOODOO_FROST_NAME, "OpenGL hooked successfully.");
            } else {
                mCore->Log(LL_Error, VOODOO_FROST_NAME, "OpenGL hook procedure failed.");
            }

            // Init Cg
            this->mCgContext = cgCreateContext();

            if ( !cgIsContext(this->mCgContext) )
            {
                throw std::exception("Unable to create Cg context.");
            }

            cgSetContextBehavior(mCgContext, CG_BEHAVIOR_LATEST);
            cgSetLockingPolicy(CG_NO_LOCKS_POLICY);
            cgSetErrorHandler(&(Core::CgErrorHandler), this);

            cgSetAutoCompile(mCgContext, CG_COMPILE_IMMEDIATE);
            cgSetParameterSettingMode(mCgContext, CG_IMMEDIATE_PARAMETER_SETTING);

            cgGLRegisterStates(context);
            cgGLSetManageTextureParameters(context, CG_TRUE);

            mCore->SetCgContext(context);
        }

        Adapter::~Adapter()
        {
            if ( cgIsContext(mCgContext) )
            {
                cgDestroyContext(mCgContext);
            }
        }

        // IObject
        const char * Adapter::GetObjectClass()
        {
            return "Frost_Adapter";
        }

        // IAdapter
        bool Adapter::LoadPass( _In_ Pass * pass )
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        bool Adapter::UnloadPass( _In_ Pass * pass )
        {
            UNREFERENCED_PARAMETER(pass);

            return true;
        }

        void Adapter::BindPass( _In_ PassRef pass )
        {
            CGpass cgpass = pass->GetCgPass();
            cgSetPassState(cgpass);

            mLastPass = cgpass;
        }

        void Adapter::UnbindPass()
        {
            cgResetPassState(mLastPass);
        }

        void Adapter::DrawQuad(Vertex * vertexData)
        {
            if ( vertexData )
            {
                glBegin(GL_QUADS);
                glTexCoord2f(vertexData[0].tu, vertexData[0].tv);
                glVertex3f(vertexData[0].x, vertexData[0].y, vertexData[0].z);
                glTexCoord2f(vertexData[1].tu, vertexData[1].tv);
                glVertex3f(vertexData[1].x, vertexData[1].y, vertexData[1].z);
                glTexCoord2f(vertexData[2].tu, vertexData[0].tv);
                glVertex3f(vertexData[2].x, vertexData[2].y, vertexData[2].z);
                glTexCoord2f(vertexData[3].tu, vertexData[3].tv);
                glVertex3f(vertexData[3].x, vertexData[3].y, vertexData[3].z);
                glEnd();
            } else {
                glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2d(0.0f, 250.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2d(0.0f, 0.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2d(250.0f, 250.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2d(250.0f, 0.0f);
                glEnd();
            }
        }

        void Adapter::ApplyParameter( _In_ ParameterRef param )
        {
            ParameterType pt = param->GetType();
            ParameterCategory pc = Converter::ToParameterCategory(pt);
            CGparameter cgparam = param->GetCgParameter();

            if ( pc == PC_Float )
            {
                float * values = param->GetFloat();
                switch ( pt )
                {
                case PT_Float1:
                    cgGLSetParameter1fv(cgparam, values);
                    break;
                case PT_Float2:
                    cgGLSetParameter2fv(cgparam, values);
                    break;
                case PT_Float3:
                    cgGLSetParameter3fv(cgparam, values);
                    break;
                case PT_Float4:
                    cgGLSetParameter4fv(cgparam, values);
                    break;
                }
            } else if ( pc == PC_Sampler ) {
                GLuint textureID = (GLuint)param->GetTexture()->GetData();
                cgGLSetupSampler(cgparam, textureID);
            } else {
                mCore->Log(LL_Warning, VOODOO_FROST_NAME, "Unable to apply parameter %s.", param->GetName().c_str());
            }
        }

        void Adapter::DrawShader( _In_ ShaderRef shader )
        {
            if ( shader.get() == NULL )
            {
                mCore->Log(LL_Error, VOODOO_FROST_NAME, "Unable to draw null shader.");
                return;
            }

            TechniqueRef tech = shader->GetDefaultTechnique();

            if ( tech.get() == NULL )
            {
                mCore->Log(LL_Error, VOODOO_FROST_NAME, "No default technique given for shader %s.", shader->GetName().c_str());
                return;
            }

            size_t passes = tech->GetPassCount();
            for ( size_t curpass = 0; curpass < passes; ++curpass )
            {
                PassRef pass = tech->GetPass(curpass);

                this->BindPass(pass);
                this->DrawQuad(NULL);
                this->UnbindPass();

                TextureRef target = pass->GetTarget();
                if ( target.get() )
                {
                    GLuint passtarget = (GLuint)target->GetData();
                    glBindTexture(GL_TEXTURE_2D, passtarget);
                    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
                }
            }

            TextureRef target = tech->GetTarget();
            if ( target.get() )
            {
                GLuint techtarget = (GLuint)tech->GetData();
                glBindTexture(GL_TEXTURE_2D, techtarget);
                glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 250, 250, 0);
            }
        }

        TextureRef Adapter::CreateTexture( _In_ String name, _In_ TextureDesc desc )
        {
            GLuint texture;
            GLint internalFormat = GL_RGBA8; //Frost::Converter::ToGLFormat(desc.Format);

            glGenTextures(1, &texture);
            GLenum error =  glGetError();
            while ( error != GL_NO_ERROR )
            {
                mCore->Log(LL_Warning_API, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error =  glGetError();
            }

            if ( glIsTexture(texture) )
            {
                mCore->Log(LL_Debug, VOODOO_FROST_NAME, "OpenGL texture %u created successfully.", texture);
            } else {
                mCore->Log(LL_Debug, VOODOO_FROST_NAME, "OpenGL create failed, returned texture %u.", texture);
            }

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, desc.Width, desc.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            error =  glGetError();
            while ( error != GL_NO_ERROR )
            {
                mCore->Log(LL_Warning_API, VOODOO_FROST_NAME, "OpenGL returned error %u: %s", error, glGetString(error));
                error =  glGetError();
            }

            return mCore->AddTexture(name, (void*)texture);
        }

        bool Adapter::ConnectTexture( _In_ ParameterRef param, _In_ TextureRef texture )
        {
            cgGLSetupSampler(param->GetCgParameter(), (GLuint)texture->GetData());
            return true;
        }

        void Adapter::HandleError( _In_ CGcontext context, _In_ CGerror error, _In_ void * core )
        {
            if ( core )
            {
                Core::CgErrorHandler(context, error, core);
            }
        }

        // Frost

        void Adapter::SetDC(_In_opt_ HDC hdc)
        {
            mDC = hdc;
        }

        void Adapter::SetGLRC(_In_opt_ HGLRC hglrc)
        {
            if ( hglrc != NULL )
            {
                TextureDesc desc;
                desc.Width = desc.Height = 256;
                desc.Depth = 1;
                desc.Mipmaps = false;
                desc.Format = TF_RGBA8;

                mTexLastPass = this->CreateTexture(":thisframe", desc);
                mTexLastPass = this->CreateTexture(":lastpass", desc);
                mTexLastShader = this->CreateTexture(":lastshader", desc);
                mCore->SetTexture(TT_ShaderTarget, mTexLastShader);
                mCore->SetTexture(TT_PassTarget, mTexLastPass);

                // Load shader
                TestShader = mCore->CreateShader("test.cgfx", NULL);
                TestShader->Link();
            } else {
                mTexLastPass = NULL;
                mTexLastShader = NULL;

                TestShader = NULL;
            }

            mGLRC = hglrc;
        }
    }
}