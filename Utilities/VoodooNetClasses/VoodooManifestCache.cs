﻿/*
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
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace VoodooNetClasses
{
    public class VoodooManifestCache
    {
        public String Path { get; set; }

        public VoodooManifestCache(String iPath)
        {
            Path = iPath;

            if (!Directory.Exists(Path))
            {
                Directory.CreateDirectory(Path);
            }
        }

        public void Sync(Remote remote)
        {            
            try
            {
                WebClient client = new WebClient();
                String remotefile = Path + "\\remote_" + VoodooHash.Hash(remote.Uri) + ".xml";

                client.DownloadFile(remote.Uri, remotefile);

                RemoteManifest remotemanifest = (RemoteManifest)VoodooXml.ValidateObject(remotefile, typeof(RemoteManifest));

                foreach (Package package in remotemanifest.Packages)
                {
                    try
                    {
                        String packagefile = Path + "\\package_" + package.PackId.ToString() + ".xml";

                        client.DownloadFile(package.ManifestUri, packagefile);

                        PackageManifest packagemanifest = (PackageManifest)VoodooXml.ValidateObject(packagefile, typeof(PackageManifest));
                    }
                    catch (System.Exception ex)
                    {
                        return;                    	
                    }
                }
            }
            catch (Exception exc)
            {
                return;
            }
        }

        public void Sync(IEnumerable<Remote> remotes)
        {
            foreach (Remote remote in remotes)
            {
                Sync(remote);
            }
        }
    }
}
