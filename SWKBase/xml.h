#pragma once

#include <stdio.h>
#include <tchar.h>
#include <wtypes.h>

#import <msxml6.dll> 

//
namespace swktool
{    
    class XMLDOMInterface {
        MSXML2::IXMLDOMDocumentPtr XmlDoc_;

    public:
        XMLDOMInterface() : XmlDoc_(nullptr)
        {
            HRESULT hr1= CoInitialize(NULL);

            HRESULT hr = CreateAndInitDOM(&XmlDoc_);
        }

        bool Load(const std::string& sXmlFile) {
            return XmlDoc_->load(sXmlFile.c_str());
        }

    protected:
        // Helper function to create a DOM instance. 
        HRESULT CreateAndInitDOM(MSXML2::IXMLDOMDocument** ppDoc)
        {
            HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
            if (SUCCEEDED(hr))
            {
                // these methods should not fail so don't inspect result
                (*ppDoc)->put_async(VARIANT_FALSE);
                (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
                (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
            }
            return hr;
        }

    };

}