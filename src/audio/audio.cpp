#include "pch.h"
#include "audio.h"
#include "exceptions/exceptions.h"
#include "console/console.h"

/*

Todo: https://learn.microsoft.com/en-us/windows/win32/coreaudio/mmdevice-api

*/

void MAGE_InitAudio()
{
	IMMDeviceEnumerator *pEnumerator{};

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	ThrowIfFailed(CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void **)&pEnumerator));

	IMMDeviceCollection *pCollection;
	ThrowIfFailed(pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection));

	UINT deviceCount;
	ThrowIfFailed(pCollection->GetCount(&deviceCount));
	for (UINT i = 0; i < deviceCount; ++i)
	{
		IMMDevice *pDevice;
		ThrowIfFailed(pCollection->Item(i, &pDevice));

		IMMEndpoint *pEndpoint;
		ThrowIfFailed(pDevice->QueryInterface(&pEndpoint));

		LPWSTR id;
		ThrowIfFailed(pDevice->GetId(&id));

		IPropertyStore *pStore;
		ThrowIfFailed(pDevice->OpenPropertyStore(STGM_READ, &pStore));

		PROPVARIANT friendlyName;
		PropVariantInit(&friendlyName);

		ThrowIfFailed(pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName));

		if (friendlyName.vt != VT_EMPTY)
		{
			// Print endpoint friendly name and endpoint ID.
			Log("Endpoint %d: \"%S\" (%S)",
				   i, friendlyName.pwszVal, id);
		}

		CoTaskMemFree(id);
		id = NULL;
		ThrowIfFailed(PropVariantClear(&friendlyName));
		pStore->Release();
		pEndpoint->Release();
		pDevice->Release();
	}

	pCollection->Release();
	pEnumerator->Release();
}
