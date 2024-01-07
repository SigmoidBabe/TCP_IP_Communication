#include <iostream>
#include <dshow.h>

int main() {
    CoInitialize(NULL);

    ICreateDevEnum* deviceEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnum));

    if (SUCCEEDED(hr)) {
        IEnumMoniker* enumMoniker = NULL;
        hr = deviceEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumMoniker, 0);

        if (SUCCEEDED(hr)) {
            IMoniker* moniker = NULL;
            ULONG fetched;
            while (enumMoniker->Next(1, &moniker, &fetched) == S_OK) {
                IPropertyBag* propertyBag = NULL;
                hr = moniker->BindToStorage(0, 0, IID_PPV_ARGS(&propertyBag));

                if (SUCCEEDED(hr)) {
                    VARIANT var;
                    VariantInit(&var);
                    hr = propertyBag->Read(L"DevicePath", &var, 0);

                    if (SUCCEEDED(hr)) {
                        std::wcout << L"Device Path: " << var.bstrVal << std::endl;
                        VariantClear(&var);
                    }

                    propertyBag->Release();
                }

                moniker->Release();
            }

            enumMoniker->Release();
        }

        deviceEnum->Release();
    }

    CoUninitialize();
    return 0;
}

