#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <vector>
#include <algorithm>
#include <memory>
#include <mutex>

#include "Window.h"
#include "WinEventHandler.h"
#include "DialogWindow.h"
#include "ModelessDialogWindow.h"
#include "WinFrame.h"
#include "WinKernel.h"
#include "WinClipboard.h"
#include "WinMemory.h"
#include "WinFile.h"

// Controls
#include "Ctrl.h"
#include "EditCtrl.h"
#include "ButtonCtrl.h"
#include "ComboboxCtrl.h"
#include "ListboxCtrl.h"
#include "ProgressBarCtrl.h"
#include "AnimationCtrl.h"
#include "SliderCtrl.h"
#include "DeviceContext.h"
#include "WinAccelerator.h"
#include "UpDownCtrl.h"