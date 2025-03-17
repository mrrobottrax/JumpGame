#include "pch.h"
#include "audio.h"
#include "exceptions/exceptions.h"
#include "console/console.h"

/*

Todo: https://learn.microsoft.com/en-us/windows/win32/coreaudio/mmdevice-api

*/

#define REFTIMES_PER_SEC  10000000

static CComPtr<IMMDevice> pDefaultDevice;

void MAGE_InitAudio()
{
	CComPtr<IMMDeviceEnumerator> pEnumerator{};

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	ThrowIfFailed(CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void **)&pEnumerator));

	pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultDevice);
	{
		CComPtr<IPropertyStore> pStore;
		ThrowIfFailed(pDefaultDevice->OpenPropertyStore(STGM_READ, &pStore));

		PROPVARIANT friendlyName;
		PropVariantInit(&friendlyName);

		ThrowIfFailed(pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName));

		if (friendlyName.vt != VT_EMPTY)
		{
			// Print endpoint friendly name and endpoint ID.
			Log("Default Endpoint: \"%S\"", friendlyName.pwszVal);
		}

		ThrowIfFailed(PropVariantClear(&friendlyName));
	}

	CComPtr<IAudioClient> pClient;
	ThrowIfFailed(pDefaultDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **)&pClient));

	WAVEFORMATEX *pFormat;
	ThrowIfFailed(pClient->GetMixFormat(&pFormat));

	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC; // 1 second
	ThrowIfFailed(pClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pFormat, NULL));
}

void MAGE_EndAudio()
{}
