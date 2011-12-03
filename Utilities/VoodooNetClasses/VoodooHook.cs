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
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;
using Microsoft.Win32;

namespace VoodooNetClasses
{
    [Serializable]
    public class VoodooHook : INotifyPropertyChanged, ISerializable, IVoodooRegistryObject
    {
        public event PropertyChangedEventHandler PropertyChanged;

        Boolean m_Active;
        String m_Name, m_Target, m_Config;

        public VoodooHook()
        {

        }

        public VoodooHook(bool Active, String Name, String Target, String Config)
        {
            m_Active = Active;
            m_Name   = Name;
            m_Target = Target;
            m_Config = Config;
        }

        protected VoodooHook(SerializationInfo info, StreamingContext context)
        {
            m_Active = info.GetBoolean("Active");
            m_Name   = info.GetString("Name");
            m_Target = info.GetString("Target");
            m_Config = info.GetString("Config");
        }

        [SecurityPermissionAttribute(SecurityAction.Demand, SerializationFormatter = true)]
        public virtual void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue("Active", m_Active);
            info.AddValue("Name",   m_Name);
            info.AddValue("Target", m_Target);
            info.AddValue("Config", m_Config);
        }

        public void FromRegistryKey(RegistryKey key)
        {
            try
            {
                m_Active = Convert.ToBoolean(key.GetValue("Active") as String);
            }
            catch (System.Exception)
            {
                m_Active = false;
            }
            try
            {
                m_Name = key.GetValue("Name") as String;
                if (m_Name == null)
                {
                    m_Name = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Name = String.Empty;
            }
            try
            {
                m_Target = key.GetValue("Target") as String;
                if (m_Target == null)
                {
                    m_Target = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Target = String.Empty;            	
            }
            try
            {
                m_Config = key.GetValue("Config") as String;
                if (m_Config == null)
                {
                    m_Config = String.Empty;
                }
            }
            catch (System.Exception)
            {
                m_Config = String.Empty;            	
            }
        }

        public void ToRegistryKey(RegistryKey parent)
        {
            String keyName = Guid.NewGuid().ToString("N");

            RegistryKey key = parent.OpenSubKey(keyName);
            if (key != null)
            {
                key.Close();
                parent.DeleteSubKeyTree(keyName);
            }

            key = parent.CreateSubKey(keyName, RegistryKeyPermissionCheck.ReadWriteSubTree);

            key.SetValue("Active", m_Active.ToString().ToLower());
            key.SetValue("Name",   m_Name);
            key.SetValue("Target", m_Target);
            key.SetValue("Config", m_Config);

            key.Close();
        }

        public bool Active
        {
            get { return m_Active; }
            set { m_Active = value; this.NotifyPropertyChanged("Active"); }
        }

        public String Name
        {
            get { return m_Name; }
            set { m_Name = value; this.NotifyPropertyChanged("Name"); }
        }

        public String Target
        {
            get { return m_Target; }
            set { m_Target = value; this.NotifyPropertyChanged("Target"); }
        }

        public String Config
        {
            get { return m_Config; }
            set { m_Config = value; this.NotifyPropertyChanged("Config"); }
        }

        private void NotifyPropertyChanged(String name)
        {
            if (PropertyChanged != null) { PropertyChanged(this, new PropertyChangedEventArgs(name)); }
        }
    }
}