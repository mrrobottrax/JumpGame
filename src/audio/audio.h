#pragma once

namespace Audio
{
	void init();
	void shutdown();

	struct AudioFile
	{
		char *pData;
		size_t length;
	};
	AudioFile convert_audio(AudioFile &input);
}