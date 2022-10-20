#pragma once

#ifdef __linux__
#define IMV_RESOURCE_DIR "/home/caffeine/.local/share/ikulab-motion-viewer"
#endif

#ifndef IMV_RESOURCE_DIR
#define IMV_RESOURCE_DIR "."
#endif