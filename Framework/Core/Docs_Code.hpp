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
  
/**
 * @page adapterspec Adapter Specifications
 * 
 * The Voodoo Core is API and program independent and leaves most interfacing to the
 * loaded adapter. To work properly with the core, each Adapter must provide a standard
 * entry point and implement a class derived from the Adapter interface defined in
 * the core.
 * 
 * @section adapterep Entry Point
 * <p>
 *  Each adapter module must export a pair of functions, used to create and destroy the Adapter. The
 *  functions must have the signatures:
 * </p>
 * <p>
 *  <code>
 *      VoodooShader::Adapter * __stdcall CreateAdapter( VoodooShader::Core * core);<br />
 *      void __stdcall DestroyAdapter( VoodooShader::Adapter * adapter);
 *  </code>
 * </p>
 * <p>
 *  The exports must not be mangled or have any symbols prepended or appended. It is recommended that
 *  these functions be simple wrappers for <code>new</code> and <code>delete</code>, with any
 *  additional initialization performed within the Adapter constructor and destructor if possible.
 * </p>
 * <p>
 *  The chain of execution for the Voodoo loading processes begins with the dynamic loader, which
 *  intercepts a single hook function in the target process. The loader then finds and loads the
 *  Voodoo core, passing the core's binary path and some additional data through. The Core starts
 *  itself and parses the VoodooConfig.xml file located in the target's startup directory. This
 *  configuration file specifies the Adapter module to be used, as well as any extended options
 *  needed. The Core locates and loads the adapter, following the given settings, and calls the
 *  <code>CreateAdapter</code> function, passing <code>this</code> as the parameter. The Adapter
 *  may access the Core's creation parameter by calling Core::GetCreationParameters(). 
 * </p>
 * <p>
 *  The <code>DestroyAdapter</code> is later called when the process is shutting down, although
 *  the Adapter may destroy itself and the Core at an earlier point, if it so chooses. If the
 *  Core and/or Adapter are still alive when the loader receives a process detach notification,
 *  the core's destroy function is called. If an Adapter is still bound to the Core, the 
 *  destructor then calls <code>DestroyAdapter</code>, passing the bound adapter as the parameter.
 *  The Adapter is responsible for any necessary cleanup at this point. After the adapter is
 *  destroyed, the hook manager is destroyed, and finally the logger is dumped to disk and
 *  destroyed.
 * </p>
 * 
 * @section adapteri Interface
 * <p>
 *  To allow the Voodoo core to work with adapters, each adapter module must implement
 *  a class derived from the Adapter interface and register the adapter object by calling
 *  Core::SetAdapter() when the adapter object is created. 
 * </p>
 * <p>
 *  The adapter object may be created at any time during execution. Delays occur often
 *  in DirectX adapters, which must wait for the process to create a device before creating
 *  the Adapter object. To provide this delay, the adapter module's LoadAdapter function
 *  should install any necessary hooks to intercept later function calls. 
 * </p>
 * <p>
 *  The adapter object must implement all functions given in the Adapter interface and they
 *  must perform as stated in the interface documentation. Different behavior may cause
 *  serious bugs.
 * </p>
 * 
 * @page codestyle Code Style
 * 
 * @section intro Intro
 * <p>
 *  The Voodoo Shader Framework is designed to work across many games, created across a wide 
 *  variety of compilers, graphics libraries, code styles and time periods. To maximize
 *  stability and speed, both critical to Voodoo's purpose, all Voodoo projects must meet 
 *  the following code rules. These are designed to eradicate bugs within Voodoo itself
 *  and provide the most pleasant user and developer experience possible, especially 
 *  considering the high degree of complexity within Voodoo.
 * </p>
 * <p>
 *  Third-party code that is used in Voodoo, or adapters that are not part of the
 *  official project, do not need to use these rules, but they can rely on Voodoo
 *  following them precisely.
 * </p>
 * <p>
 *  Any bugs, potential bugs, performance bottlenecks, possible points of improvement or
 *  generally questionable code encountered in Voodoo should be reported as soon as possible.
 *  Please file an issue with as much information as possible in the official Voodoo bug
 *  tracker.
 * </p>
 * 
 * @section general General Rules
 * <ul>
 * <li>All projects must be configured for Visual Studio 2010 (vcxproj files, all dependency
 *      paths set in the project settings, etc).</li>
 * <li>#pragma preprocessor directives and other VS-specific features should be avoided.</li>
 * <ul>
 *  <li>Voodoo is designed to compile on Windows within Visual Studio. Compatibility with other
 *       compilers is not guaranteed or tested for, compatibility with other systems is highly
 *      unlikely. Voodoo relies on some Windows-specific features and APIs, as well as targeting
 *      mainly Windows-specific software.</li>
 *  <li>#pragma to disable DLL interface warnings (warning 4251) is excepted.</li>
 * </ul>
 * <li>All projects must have 3 standard build configurations and each config must follow the 
 *      appropriate rules.</li>
 * <li>All projects must build in all configurations before any release.</li>
 * <ul>
 *  <li>This is especially important for new projects. Building under Debug_Analysis can find
 *      many code issues before they run.</li>
 *  <li>For some adapters, building under Debug_Analysis may not always be possible. If at all
 *      possible, it should be done; if not, exceptions may be made only as <em>necessary</em>.
 *      </li>
 * </ul>
 * <li>Project behavior must remain consistent between build configurations.</li>
 * <ul>
 *  <li>#ifdef directives dependent on the build mode should be avoided wherever possible. Log
 *      messages should use LL_Debug instead (this has no notable performance hit). This improves
 *      debugging.</li>
 * </ul>
 * <li>C++ TR1 and C++0x features may be used <em>as necessary</em> or where they notably improve
 *      code quality, readability or functionality.</li>
 * <li>All functions must use SAL annotations (<code>_In_, _Check_return_, etc</code>) wherever
 *      appropriate.</li>
 * <ul>
 *   <li>All functions and methods must have annotations on all parameters.</li>
 * </ul>
 * <li>All classes, methods, fields, functions, structs, enums, typedefs or other applicable code
 *      constructs must be documented with their purpose and any vital notes.</li>
 *  <ul>
 *   <li>All documentation must be doxygen compatible and use the same style as the Core.</li>
 *   <li>Any function that throws must be marked with a <code>@ throws</code> note.</li>
 *   <li>Any function with specific requirements must have them noted in SAL annotations (as
 *       well as possible) and with <code>@ note</code> or <code>@ warning</code> documentation.</li>
 *   <li>All function parameters, global variables, enum members, class members, etc must be
 *       documented. <em>(Note: The project does not fully conform to this requirement yet)</em></li>
 *  </ul>
 * <li>Headers should use the HPP extension, code files CPP.</li>
 * <li>Headers should include as few files as possible.</li>
 * <ul>
 *   <li>If the header defines a class which has member of or using the type Reference<T>, the
 *       appropriate header for the class T must be included in the header (otherwise some
 *       rather confusing compiler errors will show up).</li>
 * </ul>
 * </ul>
 * 
 * @section events Events
 * 
 * @subsection commit Commit
 * <ul>
 *  <li>Version information must be updated to reflect the new revision.</li>
 *  <li>Every major commit or on the first commit of a week, a SourceMonitor entry must be made with
 *      the current revision number as the name (e.g., rev111).</li>
 * </ul>
 * 
 * @subsection push Push
 * <ul>
 *   <li>Doxygen documentation must be generated and uploaded along with the push for all modified
 *          projects.</li>
 * </ul>
 * 
 * @subsection release Release
 * <ul>
 *   <li>Full build and static analysis must be run and code must pass.</li>
 *   <ul>
 *     <li>The Debug_Analysis build must succeed with no warnings or errors.</li>
 *     <li>CppCheck analysis must report no warnings or errors.</li>
 *     <li>Yasca analysis must find no warnings or errors.
 *     <ul>
 *       <li>Unused functions are excepted but should be avoided.</li>
 *        <li>Unsafe/banned functions should be avoided wherever possible.</li>
 *     </ul>
 *   </ul>
 *  <li>A SourceMonitor entry must be made with the release version (and name, if present).</li>
 *  <li>Version information must be updated.</li>
 *  <li>A release commit must be made and pushed. The commit message must include the release 
 *      version and name.</li>
 * </ul>
 * 
 * 
 * @section configs Build Configs
 * 
 * @subsection all All
 * <ul>
 *  <li>Compiler</li>
 *  <ul>
 *    <li>Use of MFC must be set to "Use Standard Windows Libraries".</li>
 *    <li>Use of ATL must be set to "Not using ATL".</li>
 *    <li>Character set must be set to "Not Set" at this time.</li>
 *    <li>Common language runtime support must be set to "No Common
 *          Language Runtime support".</li>
 *  </ul>
 * </ul>
 *    
 * @subsection debug_analysis Debug_Analysis
 * <ul>
 *  <li>Compiler</li>
 *  <ul>
 *    <li>Debug mode must be turned on and _DEBUG must be defined.</li>
 *    <li>Debug information must be set to /Zi (program database).</li>
 *    <li>Warning level must be /W4.</li>
 *    <li>Warnings as errors (/WX) must be enabled.</li>
 *    <li>All optimizations must be disabled.</li>
 *    <li>Whole program optimizations must be set to "No whole program optimizations".</li>
 *    <li>Minimal rebuild must be off (/Gm-).</li>
 *    <li>Runtime checks must be both (/RTC1).</li>
 *    <li>Runtime library must be multithreaded debug DLL (/MDd).</li>
 *  </ul>
 *  <li>Linker</li>
 *  <ul>
 *    <li>Generate debug info must be on (/DEBUG).</li>
 *  </ul>
 *  <li>Analysis</li>
 *  <ul>
 *    <li>Code analysis must be enabled.</li>
 *    <li>Code analysis on build must be enabled.</li>
 *    <li>"Microsoft All Rules" ruleset must be used.</li>
 *  </ul>
 * </ul>
 *  
 * @subsection debug Debug
 * <ul>
 *  <li>Compiler</li>
 *  <ul>
 *    <li>Debug mode must be turned on and _DEBUG must be defined.</li>
 *    <li>Debug information must be set to /Zi (program database).</li>
 *    <li>Warning level must be /W4.</li>
 *    <li>Warnings as errors (/WX) must not be enabled.</li>
 *    <li>All optimizations must be disabled.</li>
 *    <li>Whole program optimizations must be set to "No whole program optimizations".</li>
 *    <li>Minimal rebuild must be off (/Gm-).</li>
 *    <li>Runtime checks must be both (/RTC1).</li>
 *    <li>Runtime library must be multithreaded debug DLL (/MDd).</li>
 *  </ul>
 *  <li>Linker</li>
 *  <ul>
 *    <li>Generate debug info must be on (/DEBUG).</li>
 *  </ul>
 *  <li>Analysis</li>
 *  <ul>
 *    <li>Code analysis must not be enabled.</li>
 *  </ul>
 * </ul>
 *  
 * @subsection release Release
 * <ul>
 *  <li>Compiler</li>
 *  <ul>
 *    <li>Debug mode must be turned off and _DEBUG must not be defined.</li>
 *    <li>Debug information must be set to /Zi (program database).</li>
 *    <li>Warning level must be /W4.</li>
 *    <li>Warnings as errors (/WX) must not be enabled.</li>
 *    <li>Whole program optimizations must be set to "Link-time code generation".</li>
 *    <li>Optimizations should be set to "Maximize speed" (/O2).</li>
 *    <li>Inline function expansion should be set to "Any suitable" (/Ob2).</li>
 *    <li>Enable Intrinsic Functions must be set to "Yes" (/Oi).</li>
 *    <li>Favor Size Or Speed must be set to "Favor fast code" (/Ot).</li>
 *    <li>Runtime Library must be set to multithreaded DLL (/MD).</li>
 *  </ul>
 *  <li>Linker</li>
 *  <ul>
 *    <li>Generate debug info must be off.</li>
 *  </ul>
 *  <li>Analysis</li>
 *  <ul>
 *    <li>Code analysis must not be enabled.</li>
 *  </ul>
 * </ul>
 */