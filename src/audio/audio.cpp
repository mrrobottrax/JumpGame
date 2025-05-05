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
	// constants

	// REFERENCE_TIME is a win32 thing = 100 nanoseconds
	constexpr size_t REFTIMES_PER_SEC = 10000000;
	constexpr size_t REFTIMES_PER_MILLISEC = 10000;

	// statics
	static WAVEFORMATEX *pFormat;
	static UINT32 bufferSize;
	static DWORD sampleRate;
	static REFERENCE_TIME hnsBufferDuration;
	static WORD containerSize;
	static WORD channels;

	static CComPtr<IMMDevice> pDefaultDevice;
	static CComPtr<IAudioClient> pClient;
	static CComPtr<IAudioRenderClient> pRenderClient;
	static HANDLE hAudioThread;
	static volatile bool shouldEndAudioThread = false;

	enum class EAudioFormat
	{
		UNKNOWN,
		FLOAT_32,
		INT_16,
		INT_8,
		UNSUPPORTED,
	};
	static EAudioFormat currentFormat = EAudioFormat::UNKNOWN;

	// static methods
	static DWORD WINAPI audio_thread_proc(_In_ LPVOID lpParameter);
	static EAudioFormat get_format(WAVEFORMATEX *pWaveFormatEx);

	void init()
	{
		CComPtr<IMMDeviceEnumerator> pEnumerator{};

		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		throw_if_failed(CoCreateInstance(
			CLSID_MMDeviceEnumerator, NULL,
			CLSCTX_ALL, IID_IMMDeviceEnumerator,
			(void **)&pEnumerator));

		// find endpoint
		pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDefaultDevice);

		// print name
		{
			CComPtr<IPropertyStore> pStore;
			throw_if_failed(pDefaultDevice->OpenPropertyStore(STGM_READ, &pStore));

			PROPVARIANT friendlyName;
			PropVariantInit(&friendlyName);

			throw_if_failed(pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName));

			if (friendlyName.vt != VT_EMPTY)
			{
				log("Default Endpoint: \"%S\"", friendlyName.pwszVal);
			}

			throw_if_failed(PropVariantClear(&friendlyName));
		}

		// create client
		throw_if_failed(pDefaultDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void **)&pClient));

		// get format
		throw_if_failed(pClient->GetMixFormat(&pFormat));
		currentFormat = get_format(pFormat);

		sampleRate = pFormat->nSamplesPerSec;
		channels = pFormat->nChannels;
		containerSize = pFormat->wBitsPerSample;

		log("Sample rate: %u", sampleRate);
		log("Channels: %u", channels);

		// init client
		REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC; // 1 second
		throw_if_failed(pClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pFormat, NULL));

		throw_if_failed(pClient->GetBufferSize(&bufferSize));
		log("Buffer size: %u", bufferSize);

		hnsBufferDuration = (REFERENCE_TIME)((double)REFTIMES_PER_SEC * bufferSize / sampleRate);
		log("Buffer duration: %lu", hnsBufferDuration);

		// create render client
		throw_if_failed(pClient->GetService(__uuidof(IAudioRenderClient), (void **)&pRenderClient));

		// fill buffer with 0
		BYTE *pData;
		throw_if_failed(pRenderClient->GetBuffer(bufferSize, &pData));

		memset(pData, 0, static_cast<size_t>(bufferSize) * (containerSize / 8) * channels);

		throw_if_failed(pRenderClient->ReleaseBuffer(bufferSize, 0));
		throw_if_failed(pClient->Start());

		hAudioThread = CreateThread(NULL, 0, audio_thread_proc, NULL, 0, NULL);
		throw_if_null(hAudioThread);
	}

	void shutdown()
	{
		CoTaskMemFree(pFormat);
		pClient->Stop();

		shouldEndAudioThread = true;
		WaitForSingleObject(hAudioThread, INFINITE);
		CloseHandle(hAudioThread);
	}

	AudioFile convert_audio(AudioFile &input)
	{
		// resample and convert to float/pcm
		return AudioFile();
	}

	static DWORD WINAPI audio_thread_proc(
		_In_ LPVOID lpParameter
	)
	{
		UINT64 counter = 0;

		while (!shouldEndAudioThread)
		{
			// sleep for half of buffer
			Sleep((DWORD)(hnsBufferDuration / REFTIMES_PER_MILLISEC / 2));

			UINT32 padding;
			throw_if_failed(pClient->GetCurrentPadding(&padding));

			UINT32 available = bufferSize - padding;

			BYTE *pData;
			throw_if_failed(pRenderClient->GetBuffer(available, &pData));

			

			throw_if_failed(pRenderClient->ReleaseBuffer(available, 0));

			counter += available;
		}

		return 0;
	}

	EAudioFormat get_format(WAVEFORMATEX *pWaveFormatEx)
	{
		enum
		{
			UNKNOWN = 0,
			FLOAT,
			PCM
		} roughFormat = UNKNOWN;
		int bits = 0;

		if (pWaveFormatEx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
		{
			WAVEFORMATEXTENSIBLE &waveFormat = *((WAVEFORMATEXTENSIBLE *)pWaveFormatEx);

			if (waveFormat.SubFormat == KSDATAFORMAT_SUBTYPE_PCM)
			{
				roughFormat = PCM;
				log("Format: PCM");
			}
			else if (waveFormat.SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)
			{
				roughFormat = FLOAT;
				log("Format: Float");
			}
			else
			{
				throw runtime_error(format("Unknown audio format {0}:{1}:{2}:{3}.{4}.{5}.{6}.{7}.{8}.{9}.{10}. Email me at andrewhoult77@gmail.com so I can add support.", waveFormat.SubFormat.Data1, waveFormat.SubFormat.Data2, waveFormat.SubFormat.Data3, waveFormat.SubFormat.Data4[0], waveFormat.SubFormat.Data4[1], waveFormat.SubFormat.Data4[2], waveFormat.SubFormat.Data4[3], waveFormat.SubFormat.Data4[4], waveFormat.SubFormat.Data4[5], waveFormat.SubFormat.Data4[6], waveFormat.SubFormat.Data4[7]));
			}

			bits = waveFormat.Samples.wValidBitsPerSample;
			log("Bits: %i", waveFormat.Samples.wValidBitsPerSample);
		}
		else if (pWaveFormatEx->wFormatTag == WAVE_FORMAT_PCM)
		{
			roughFormat = PCM;
			bits = pWaveFormatEx->wBitsPerSample;

			log("Format: PCM");
			log("Bits: %i", pWaveFormatEx->wBitsPerSample);
		}
		else
		{
			throw runtime_error(format("Uknown audio format {0}. Email me at andrewhoult77@gmail.com so I can add support.", pWaveFormatEx->wFormatTag));
		}

		if (roughFormat == FLOAT)
		{
			if (bits == 32)
			{
				return EAudioFormat::FLOAT_32;
			}
			else
			{
				throw runtime_error(format("Unsupported float bit depth of {0}. Email me at andrewhoult77@gmail.com so I can fix this.", bits));
			}
		}
		else if (roughFormat == PCM)
		{
			if (bits == 8)
			{
				return EAudioFormat::INT_8;
			}
			else if (bits == 16)
			{
				return EAudioFormat::INT_16;
			}
			else
			{
				throw runtime_error(format("Unsupported PCM bit depth of {0}. Email me at andrewhoult77@gmail.com so I can fix this.", bits));
			}
		}

		throw runtime_error("Unknown audio format error");
	}
}