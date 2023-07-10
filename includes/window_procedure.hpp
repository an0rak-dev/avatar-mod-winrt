#ifndef AVATAR_MOD_WINRT_WINDOW_PROCEDURE
#define AVATAR_MOD_WINRT_WINDOW_PROCEDURE

#include <Windows.h>
#include <stdio.h>

#define WINDOW_CLASS_NAME L"AvatarWindowClass"

LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM wide_params, LPARAM long_params);

int register_window_class(HINSTANCE current_process);

#endif
