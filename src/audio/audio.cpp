#include "pch.h"
#include "audio.h"
#include "exceptions/exceptions.h"
#include "console/console.h"

using namespace Console;

/*

Todo: https://learn.microsoft.com/en-us/windows/win32/coreaudio/mmdevice-api
https://learn.microsoft.com/en-us/windows/win32/coreaudio/rendering-a-stream

*/

namespace Audio
{
// REFERENCE_TIME is a win32 thing = 100 nanoseconds
#define REFTIMES_PER_SEC 10000000
#define REFTIMES_PER_MILLISEC 10000

	static WAVEFORMATEX *pFormat;
	static UINT32 bufferSize;
	static DWORD sampleRate;
	static REFERENCE_TIME hnsBufferDuration;
	static WORD bitDepth;
	static WORD channels;

	static CComPtr<IMMDevice> pDefaultDevice;
	static CComPtr<IAudioClient> pClient;
	static CComPtr<IAudioRenderClient> pRenderClient;
	static HANDLE hAudioThread;
	static volatile bool shouldEndAudioThread = false;

	DWORD WINAPI AudioThreadProc(
		_In_ LPVOID lpParameter
	)
	{
		UINT64 counter = 0;

		while (!shouldEndAudioThread)
		{
			// sleep for half of buffer
			Sleep((DWORD)(hnsBufferDuration / REFTIMES_PER_MILLISEC / 2));

			UINT32 padding;
			ThrowIfFailed(pClient->GetCurrentPadding(&padding));

			UINT32 available = bufferSize - padding;

			BYTE *pData;
			ThrowIfFailed(pRenderClient->GetBuffer(available, &pData));

			if (bitDepth == 32)
			{
				float *pFloat = reinterpret_cast<float *>(pData);
				for (UINT32 i = 0; i < available; ++i)
				{
					double tSeconds = (double)(counter + i) / sampleRate;
					double val = sin(tSeconds * 440.0 * 2.0 * 3.14159265);
					for (UINT32 ch = 0; ch < channels; ++ch)
					{
						pFloat[i * channels + ch] = (float)val;
					}
				}
			}

			// i've actually just made it throw when it's not the float format. lazy. so these don't do anything now.

			else if (bitDepth == 16)
			{
				// 16-bit signed PCM
				int16_t *pInt16 = reinterpret_cast<int16_t *>(pData);
				for (UINT32 i = 0; i < available; ++i)
				{
					double tSeconds = (double)(counter + i) / sampleRate;
					double val = sin(tSeconds * 440.0 * 2.0 * 3.14159265);
					int16_t sample = static_cast<int16_t>(val * 32767.0);
					for (UINT32 ch = 0; ch < channels; ++ch)
					{
						pInt16[i * channels + ch] = sample;
					}
				}
			}
			else if (bitDepth == 8)
			{
				// 8-bit unsigned PCM (unlikely, but handle it)
				for (UINT32 i = 0; i < available; ++i)
				{
					double tSeconds = (double)(counter + i) / sampleRate;
					double val = sin(tSeconds * 440.0 * 2.0 * 3.14159265) / 2.0 + 0.5;
					BYTE sample = static_cast<BYTE>(val * 255.0);
					for (UINT32 ch = 0; ch < channels; ++ch)
					{
						pData[i * channels + ch] = sample;
					}
				}
			}

			ThrowIfFailed(pRenderClient->ReleaseBuffer(available, 0));

			counter += available;
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

		// find endpoint
		pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultDevice);

		// print name
		{
			CComPtr<IPropertyStore> pStore;
			ThrowIfFailed(pDefaultDevice->OpenPropertyStore(STGM_READ, &pStore));

			PROPVARIANT friendlyName;
			PropVariantInit(&friendlyName);

			ThrowIfFailed(pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName));

			if (friendlyName.vt != VT_EMPTY)
			{
				Log("Default Endpoint: \"%S\"", friendlyName.pwszVal);
			}

			ThrowIfFailed(PropVariantClear(&friendlyName));
		}

		// create client
		ThrowIfFailed(pDefaultDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **)&pClient));

		// get format
		ThrowIfFailed(pClient->GetMixFormat(&pFormat));

		if (pFormat->wFormatTag != WAVE_FORMAT_EXTENSIBLE)
		{
			throw std::runtime_error("Unsupported audio format (type 1). Email me at andrewhoult77@gmail.com so I know to add support!");
		}

		sampleRate = pFormat->nSamplesPerSec;
		channels = pFormat->nChannels;
		bitDepth = pFormat->wBitsPerSample;

		Log("Sample rate: %u", sampleRate);
		Log("Channels: %u", channels);
		Log("Bit depth: %u", bitDepth);

		if (pFormat->wBitsPerSample != 32)
		{
			throw std::runtime_error("Unsupported audio format (type 2). Email me at andrewhoult77@gmail.com so I know to add support!");
		}

		// init client
		REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC; // 1 second
		ThrowIfFailed(pClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pFormat, NULL));

		ThrowIfFailed(pClient->GetBufferSize(&bufferSize));

		Log("Buffer size: %u", bufferSize);

		hnsBufferDuration = (REFERENCE_TIME)((double)REFTIMES_PER_SEC * bufferSize / sampleRate);

		Log("Buffer duration: %lu", hnsBufferDuration);

		// create render client
		ThrowIfFailed(pClient->GetService(__uuidof(IAudioRenderClient), (void **)&pRenderClient));

		// fill buffer with 0
		BYTE *pData;
		ThrowIfFailed(pRenderClient->GetBuffer(bufferSize, &pData));

		memset(pData, 0, static_cast<size_t>(bufferSize) * (bitDepth / 8) * channels);

		ThrowIfFailed(pRenderClient->ReleaseBuffer(bufferSize, 0));
		ThrowIfFailed(pClient->Start());

		hAudioThread = CreateThread(NULL, 0, AudioThreadProc, NULL, 0, NULL);
		ThrowIfNull(hAudioThread);
	}

	void MAGE_EndAudio()
	{
		CoTaskMemFree(pFormat);
		pClient->Stop();

		shouldEndAudioThread = true;
		WaitForSingleObject(hAudioThread, INFINITE);
		CloseHandle(hAudioThread);
	}

	void Init()
	{

	}

	void Shutdown()
	{

	}
}