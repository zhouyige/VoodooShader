
#include "Gem_Adapter.hpp"
#include "Gem_Converter.hpp"

#include "../../Version.hpp"

IDirect3DSurface9 * scratchSurface = NULL;
IDirect3DTexture9 * scratchTexture = NULL;

namespace VoodooShader
{
	namespace DirectX89
	{
		LPDIRECT3DVERTEXBUFFER9 FSQuadVerts = NULL;

		Adapter::Adapter(Core * core, IDirect3DDevice9 * device)
			: mCore(core), mDevice(device)
		{
			assert(device);

			if ( !core )
			{
				core = VoodooShader::Core::Create("Voodoo_Gem.log");
			}

			core->GetLog()->Log("Voodoo Gem: Starting adapter...\n");

			try
			{
				core->SetAdapter(reinterpret_cast<VoodooShader::Adapter*>(this));
				core->GetLog()->Log("Voodoo Gem: Core adapter set to this.\n");
			} catch ( VoodooShader::Exception & exc ) {
				core->GetLog()->Format("Voodoo Gem: Error setting adapter on core: %s.\n")
					.With(exc.Message()).Done();
				//exit(1);
			}

			core->GetLog()->Log("Voodoo Gem: Assembly ID: "VOODOO_META_STRING_VERSION_FULL(GEM)"\n");

			HRESULT hr = cgD3D9SetDevice(device);
			if ( !SUCCEEDED(hr) )
			{
				Throw("Voodoo Gem: Could not set Cg device.", core);
			} else {
				core->GetLog()->Log("Voodoo Gem: Set Cg device.\n");
			}

			//#ifdef _DEBUG
			cgD3D9EnableDebugTracing(CG_TRUE);
			//#endif

			cgD3D9SetManageTextureParameters(core->GetCGContext(), CG_TRUE);

			cgD3D9RegisterStates(core->GetCGContext());

			HRESULT errors = cgD3D9GetLastError();
			if ( !SUCCEEDED(errors) )
			{
				core->GetLog()->Log("Voodoo Gem: Errors setting Cg states.\n");
			} else {
				core->GetLog()->Log("Voodoo Gem: Cg states set successfully.\n");
			}

			// Setup profiles
			CGprofile bestFrag = cgD3D9GetLatestPixelProfile();
			CGprofile bestVert = cgD3D9GetLatestVertexProfile();

			core->GetLog()->Format("Voodoo Gem: The following profiles have been detected as the latest available:\n\tVertex: %s\n\tFragment: %s\n")
				.With(cgGetProfileString(bestVert)).With(cgGetProfileString(bestFrag)).Done();

			// Get params
			D3DVIEWPORT9 viewport;
			device->GetViewport(&viewport);

			float fx = ( (float)viewport.Width  / 2 ) + 0.5f	;/// 2;
			float fy = ( (float)viewport.Height / 2 ) + 0.5f	;/// 2;

			mCore->GetLog()->Format("Voodoo Gem: Prepping for %d by %d target.\n")
				.With(fx).With(fy).Done();

			hr = this->mDevice->CreateVertexBuffer(6 * sizeof(FSVert), 0, D3DFVF_CUSTOMVERTEX,
				D3DPOOL_DEFAULT, &FSQuadVerts, NULL);

			if ( FAILED(hr) )
			{
				mCore->GetLog()->Log("Voodoo Gem: Failed to create vertex buffer.\n");
			}

			FSVert g_Vertices[4];
			memset(g_Vertices, 0, sizeof(FSVert) * 4);

			g_Vertices[0].x = -0.5f; g_Vertices[0].y = -0.5f; g_Vertices[0].z = 0.5f;
			g_Vertices[1].x =    fx; g_Vertices[1].y = -0.5f; g_Vertices[1].z = 0.5f;
			g_Vertices[2].x = -0.5f; g_Vertices[2].y =    fy; g_Vertices[2].z = 0.5f;
			g_Vertices[3].x =    fx; g_Vertices[3].y =    fy; g_Vertices[3].z = 0.5f;

			g_Vertices[0].rhw = g_Vertices[1].rhw = g_Vertices[2].rhw = g_Vertices[3].rhw = 1.0f;

			g_Vertices[0].tu = 0.0f; g_Vertices[0].tv = 0.0f;
			g_Vertices[1].tu = 1.0f; g_Vertices[1].tv = 0.0f;
			g_Vertices[2].tu = 0.0f; g_Vertices[2].tv = 1.0f;
			g_Vertices[3].tu = 1.0f; g_Vertices[3].tv = 1.0f;

			void * pVertices;
			FSQuadVerts->Lock(0, sizeof(FSVert) * 4, &pVertices, 0);

			memcpy(pVertices, g_Vertices, sizeof(FSVert) * 4);

			FSQuadVerts->Unlock();
		}

		bool Adapter::LoadPass(Pass * pass)
		{
			assert(pass);

			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			HRESULT hr = S_OK;

			if ( cgIsProgram(vertProg) )
			{
				hr = cgD3D9LoadProgram(vertProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo Gem: Error loading vertex program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return false;
				}
			}

			if ( cgIsProgram(fragProg) )
			{
				hr = cgD3D9LoadProgram(fragProg, CG_TRUE, 0);
				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo Gem: Error loading fragment program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return false;
				}
			}

			this->mCore->GetLog()->Format("Voodoo Gem: Successfully loaded programs from '%s'.\n")
				.With(pass->Name()).Done();
			return true;
		}

		void Adapter::BindPass(PassRef pass)
		{
			// Both should be loaded and valid (if they exist and prepare was called)
			CGprogram vertProg = pass->VertexProgram();
			CGprogram fragProg = pass->FragmentProgram();

			if ( cgIsProgram(vertProg) )
			{
				HRESULT hr = cgD3D9BindProgram(pass->VertexProgram());

				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo Gem: Error binding vertex program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();
					return;
				} else {
					mBoundVP = vertProg;
				}
			} else {
				mDevice->SetVertexShader(NULL);
			}

			if ( cgIsProgram(fragProg) )
			{
				HRESULT hr = cgD3D9BindProgram(pass->FragmentProgram());

				if ( !SUCCEEDED(hr) )
				{
					this->mCore->GetLog()->Format("Voodoo Gem: Error binding fragment program from '%s': %s.\n")
						.With(pass->Name()).With(cgD3D9TranslateHRESULT(hr)).Done();

					if ( cgIsProgram(vertProg) )
					{
						cgD3D9UnbindProgram(pass->VertexProgram());
						mBoundVP = NULL;
					}
					return;
				} else {
					mBoundFP = fragProg;
				}
			} else {
				mDevice->SetPixelShader(NULL);
			}
		}

		void Adapter::UnbindPass()
		{
			if ( cgIsProgram(mBoundVP) )
			{
				cgD3D9UnbindProgram(mBoundVP);
			}

			if ( cgIsProgram(mBoundFP) )
			{
				cgD3D9UnbindProgram(mBoundFP);
			}

			mDevice->SetVertexShader(NULL);
			mDevice->SetPixelShader(NULL);
		}

		void Adapter::DrawShader(ShaderRef shader)
		{
			// Set up textures and set scratch surface as render target
			IDirect3DSurface9 * rt = NULL;

			HRESULT hr = mDevice->GetRenderTarget(0, &rt);
			if ( FAILED(hr) )
			{
				mCore->GetLog()->Format("Voodoo Gem: Failed to retrieve render target for shader %s.\n").With(shader->Name()).Done();
			}
			
			hr = mDevice->SetRenderTarget(0, scratchSurface);
			if ( FAILED(hr) )
			{
				mCore->GetLog()->Format("Voodoo Gem: Failed to bind render target for shader %s.\n").With(shader->Name()).Done();
				return;
			}

			// Get technique
			TechniqueRef tech = shader->GetDefaultTechnique();

			size_t passCount = tech->GetPassCount();

			for ( size_t curPass = 0; curPass < passCount; ++curPass )
			{
				PassRef pass = tech->GetPass(curPass);

				this->BindPass(pass);
				
				this->DrawQuad(NULL);

				this->UnbindPass();

				/*
				TextureRef passTarget = pass->GetTarget();
				IDirect3DTexture9 * passTargetD3D = (IDirect3DTexture9 *)passTarget->GetTexture();
				IDirect3DSurface9 * passSurface = NULL;

				HRESULT hr = passTargetD3D->GetSurfaceLevel(0, &passSurface);
				if ( FAILED(hr) || !passSurface )
				{	
					mCore->GetLog()->Format("Voodoo Gem: Failed to get target surface for pass %s (targeting texture %s).\n")
						.With(pass->Name()).With(passTarget->Name()).Done();

					hr = mDevice->StretchRect(scratchSurface, NULL, passSurface, NULL, D3DTEXF_NONE);
					if ( FAILED(hr) )
					{
						mCore->GetLog()->Format("Voodoo Gem: Failed to copy results to target for pass %s.\n")
							.With(pass->Name()).Done();
					}
				} 
				*/
				
				hr = mDevice->StretchRect(scratchSurface, NULL, backbufferSurf, NULL, D3DTEXF_NONE);

				if ( FAILED(hr) )
				{
					mCore->GetLog()->Format("Voodoo Gem: Failed to copy results to target for pass %s (result %d).\n")
						.With(pass->Name()).With(hr).Done();
				}
			}

			/*
			TextureRef techTarget = tech->GetTarget();
			IDirect3DTexture9 * techTargetD3D = (IDirect3DTexture9 *)techTarget->GetTexture();
			IDirect3DSurface9 * techSurface = NULL;

			HRESULT hr = techTargetD3D->GetSurfaceLevel(0, &techSurface);
			if ( FAILED(hr) || !techSurface )
			{
				mCore->GetLog()->Format("Voodoo Gem: Failed to get target surface for technique %s (targeting texture %s).\n")
					.With(tech->Name()).With(techTarget->Name()).Done();
			} else {
				hr = mDevice->StretchRect(scratchSurface, NULL, techSurface, NULL, D3DTEXF_NONE);
				if ( FAILED(hr) )
				{
					mCore->GetLog()->Format("Voodoo Gem: Failed to copy results to target for technique %s.\n")
						.With(tech->Name()).Done();
				}
			}
			*/

			mDevice->SetRenderTarget(0, rt);
		}

		void Adapter::DrawQuad(Vertex * vertexData)
		{
			if ( !vertexData )
			{
				IDirect3DVertexBuffer9 * sourceBuffer;
				UINT sourceOffset, sourceStride;
				DWORD sourceFVF, zEnabled, aEnabled, cullMode;

				this->mDevice->GetStreamSource(0, &sourceBuffer, &sourceOffset, &sourceStride);
				this->mDevice->GetFVF(&sourceFVF);
				this->mDevice->GetRenderState(D3DRS_ZENABLE, &zEnabled);
				this->mDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &aEnabled);
				this->mDevice->GetRenderState(D3DRS_CULLMODE, &cullMode);

				this->mDevice->SetStreamSource(0, FSQuadVerts, 0, sizeof(FSVert));
				this->mDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
				this->mDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				this->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				this->mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					this->mDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2); 
					this->mDevice->EndScene();
				} else {
					mCore->GetLog()->Log("Voodoo Gem: Failed to draw quad.\n");
				}

				this->mDevice->SetStreamSource(0, sourceBuffer, sourceOffset, sourceStride);
				this->mDevice->SetFVF(sourceFVF);
				this->mDevice->SetRenderState(D3DRS_ZENABLE, zEnabled);
				this->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, aEnabled);
				this->mDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
			} else {
				// Draw a quad from user vertexes
				HRESULT hr = this->mDevice->BeginScene();
				if ( SUCCEEDED(hr) )
				{
					hr = this->mDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexData, sizeof(FSVert));
					if ( !SUCCEEDED(hr) )
					{
						this->mCore->GetLog()->Log("Voodoo Gem: Error drawing user quad.");
					}
					this->mDevice->EndScene();
				}
			}
		}

		void Adapter::ApplyParameter(ParameterRef param)
		{
			switch ( Converter::ToParameterCategory(param->GetType()) )
			{
			case PC_Float:
				cgD3D9SetUniform(param->GetParameter(), param->GetFloat());
				break;
			case PC_Sampler:
				cgD3D9SetTextureParameter(param->GetParameter(), (IDirect3DTexture9 *)param->GetTexture()->GetTexture());
				break;
			case PC_Unknown:
			default:
				this->mCore->GetLog()->Format("Voodoo Gem: Unable to bind parameter %s of unknown type.")
					.With(param->Name()).Done();
			}
		}

		bool Adapter::ConnectTexture(ParameterRef param, TextureRef texture)
		{
			if ( Converter::ToParameterCategory(param->GetType()) == PC_Sampler )
			{
				param->Set(texture);

				IDirect3DTexture9 * texObj = (IDirect3DTexture9 *)texture->GetTexture();
				CGparameter texParam = param->GetParameter();
				cgD3D9SetTextureParameter(texParam, texObj);
				mCore->GetLog()->Format("Voodoo Gem: Bound texture %s to parameter %s.\n")
					.With(texture->Name()).With(param->Name()).Done();
				return true;
			} else {
				Throw("Voodoo Gem: Invalid binding attempt, parameter is not a sampler.\n", this->mCore);
				return false;
			}
		}

		TextureRef Adapter::CreateTexture(std::string name, size_t width, size_t height, size_t depth, 
			bool mipmaps, TextureFormat format)
		{
			IDirect3DTexture9 * tex = NULL;
			D3DFORMAT fmt = Gem_Converter::ToD3DFormat(format);

			HRESULT hr = mDevice->CreateTexture(width, height, depth, 
				D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex, NULL);
			if ( SUCCEEDED(hr) )
			{
				TextureRef texRef = mCore->CreateTexture(name, reinterpret_cast<void*>(tex));
				return texRef;
			} else {
				const char * error = cgD3D9TranslateHRESULT(hr);
				mCore->GetLog()->Format("Voodoo Gem: Error creating texture %s: %s\n")
					.With(name).With(error).Done();
				return TextureRef();
			}
		}

		void Adapter::HandleError(CGcontext context, CGerror error, void * core)
		{
			Core * actualCore = reinterpret_cast<Core*>(core);
			actualCore->GetLog()->Format("Voodoo Gem: Cg error: %s\n")
				.With(cgD3D9TranslateCGerror(error)).Done();
		}
	}
}