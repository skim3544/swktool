#pragma once

#import <msxml6.dll> raw_interfaces_only



class XMLInterface {

    // Helper function to create a DOM instance. 
    HRESULT CreateAndInitDOM(IXMLDOMDocument** ppDoc)
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

