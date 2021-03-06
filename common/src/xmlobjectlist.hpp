/*
 * =====================================================================================
 *
 *       Filename: xmlobjectlist.hpp
 *        Created: 06/17/2015 06:24:14
 *  Last Modified: 04/03/2016 17:34:22
 *
 *    Description: analyze specifically formatted XML
 *                      <ROOT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                          <OBJECT></OBJECT>
 *                      </ROOT>
 *                 start with root node <ROOT> and format as flat object list,
 *                 for hierarchical text desc use XMLRoot
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include <tinyxml2.h>
#include <vector>

class XMLObjectList
{
    protected:
        tinyxml2::XMLDocument m_XMLDoc;
        const tinyxml2::XMLElement *m_Root;
        const tinyxml2::XMLElement *m_CurrentObject;

    public:
        XMLObjectList()
            : m_XMLDoc()
            , m_Root(nullptr)
            , m_CurrentObject(nullptr)
        {}

       ~XMLObjectList() = default;

    public:
        const tinyxml2::XMLElement *Root()
        {
            return m_XMLDoc.RootElement();
        }

    public:
        void Reset();
        bool Validate();
        const tinyxml2::XMLElement *Fetch();

    public:
        void Add(const std::vector<std::pair<std::string, std::string>> &, const char *);

    public:
        std::string Print()
        {
            if(Validate()){
                tinyxml2::XMLPrinter stPrinter;
                m_XMLDoc.Print(&stPrinter);
                return std::string(stPrinter.CStr());
            }
            return std::string("<Root></Root>");
        }

    public:
        bool Load(const char *szFileName, bool bValidate = true)
        {
            bool bRes = false;
            if(szFileName && m_XMLDoc.LoadFile(szFileName) == tinyxml2::XML_NO_ERROR){
                // load ok
                if(bValidate){
                    bRes = Validate();
                }else{
                    bRes = true;
                }
            }
            Reset();
            return bRes;
        }

        bool Parse(const char *szXMLContent, bool bValidate = true)
        {
            bool bRes = false;
            if(szXMLContent && m_XMLDoc.Parse(szXMLContent) == tinyxml2::XML_NO_ERROR){
                // load ok
                if(bValidate){
                    bRes = Validate();
                }else{
                    bRes = true;
                }
            }
            Reset();
            return bRes;
        }

    private:
        bool ValidObjectNode(const tinyxml2::XMLElement *);
};
