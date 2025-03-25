#include "pch.h"
#include "audio.h"
#include "exceptions/exceptions.h"
#include "console/console.h"

/*

Todo: https://learn.microsoft.com/en-us/windows/win32/coreaudio/mmdevice-api
https://learn.microsoft.com/en-us/windows/win32/coreaudio/rendering-a-stream

*/

#define REFTIMES_PER_SEC  10000000

static CComPtr<IMMDevice> pDefaultDevice;
static HANDLE hAudioThread;
static volatile bool shouldEndAudioThread = false;

DWORD WINAPI AudioThreadProc(
	_In_ LPVOID lpParameter
)
{
	while (!shouldEndAudioThread)
	{
		Sleep(10);
	}

	return 0;
}

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

	UINT32 bufferSize;
	ThrowIfFailed(pClient->GetBufferSize(&bufferSize));

	CComPtr<IAudioRenderClient> pRenderClient;
	ThrowIfFailed(pClient->GetService(__uuidof(IAudioRenderClient), (void **)&pRenderClient));

	BYTE *pData;
	ThrowIfFailed(pRenderClient->GetBuffer(bufferSize, &pData));

	ThrowIfFailed(pRenderClient->ReleaseBuffer(bufferSize, 0));

	CoTaskMemFree(pFormat);

	hAudioThread = CreateThread(NULL, 0, AudioThreadProc, NULL, 0, NULL);
	ThrowIfNull(hAudioThread);
}

void MAGE_EndAudio()
{
	shouldEndAudioThread = true;
	WaitForSingleObject(hAudioThread, INFINITE);
	CloseHandle(hAudioThread);
}
