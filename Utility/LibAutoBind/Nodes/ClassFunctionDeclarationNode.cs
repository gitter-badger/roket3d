﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LibAutoBind.Nodes
{
    class ClassFunctionDeclarationNode : Node
    {
        private List<string> m_Keywords;
        private string m_Name;
        private List<string> m_Arguments = new List<string>();

        internal ClassFunctionDeclarationNode(List<string> keywords, string name)
        {
            this.m_Keywords = keywords;
            name = name.Trim('{').Trim();

            // We need to split up the name into the actual function
            // name and the arguments.
            string[] s = name.Trim(')').Split(new char[] {'('}, 2);
            this.m_Name = s[0].Trim();
            foreach (string a in s[1].Split(','))
            {
                this.m_Arguments.Add(a.Trim());
            }

            // Construct a representation for the Content property.
            string keys = "";
            foreach (string k in keywords)
                keys += k + " ";
            keys = keys.Trim();
            this.p_Content = keys + " " + name;
        }

        internal List<string> AllKeywords
        {
            get { return this.m_Keywords; }
        }

        internal List<string> CPPKeywords
        {
            get
            {
                List<string> ret = new List<string>();
                foreach (string k in this.m_Keywords)
                {
                    if (!Keywords.LuaDeclKeywords.Contains(k) && !Keywords.LuaVisibilityKeywords.Contains(k))
                    {
                        ret.Add(k);
                    }
                    else if (k == "bound")
                        ret.Add("public");
                }
                return ret;
            }
        }

        internal string Name
        {
            get { return this.m_Name; }
        }

        internal List<string> Arguments
        {
            get { return this.m_Arguments; }
        }
    }
}