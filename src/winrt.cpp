#include <Windows.h>
#include <avatar/platforms/winrt.hpp>
#include <stdlib.h>
#include <window_procedure.hpp>

struct winrt_specifics {
		HWND handle;
};

int winrt_allocate(winrt_platform *winrt) {
	struct winrt_specifics *specifics = (struct winrt_specifics *) malloc(sizeof(struct winrt_specifics));
	if (NULL == specifics) {
		return 1;
	}
	winrt->specifics  = specifics;
	winrt->last_event = WINRT_NONE;
	return 0;
}

void winrt_destroy(winrt_platform *winrt) {
	if (NULL == winrt) {
		return;
	}

	if (NULL != winrt->specifics) {
		DestroyWindow(winrt->specifics->handle);
		free(winrt->specifics);
		winrt->specifics = NULL;
	}
}

int winrt_initalize(winrt_platform &winrt, const wchar_t *app_name, unsigned int width, unsigned int height) {
	HRESULT   result          = S_OK;
	HINSTANCE current_process = GetModuleHandle(NULL);
	if (0 != register_window_class(current_process)) {
		return 1;
	}
	winrt.specifics->handle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		WINDOW_CLASS_NAME,
		app_name,
		WS_OVERLAPPEDWINDOW, // Don't show the window at creation
		CW_USEDEFAULT,       // Default X position
		CW_USEDEFAULT,       // Default Y position
		width,               // Window width
		height,              // Window height
		NULL,                // No parent
		NULL,                // No default menu
		current_process,     // Attach window to calling process
		NULL                 // No additional params
	);
	if (NULL == winrt.specifics->handle) {
		fprintf(stderr, "Unable to create the Window : (code : %d)\n", GetLastError());
		return 2;
	}
	return 0;
}

void winrt_show(winrt_platform &winrt) {
	ShowWindow(winrt.specifics->handle, SW_SHOW);
}

// FIXME: It seems that the close event isn't detected until I move the window a little. Strange.
void winrt_wait_next_event(winrt_platform &winrt) {
	MSG msg = {};
	if (!GetMessage(&msg, winrt.specifics->handle, 0, 0)) {
		winrt.last_event = WINRT_QUIT;
		return;
	}
	TranslateMessage(&msg);
	DispatchMessageW(&msg);
	winrt.last_event = WINRT_NONE;
}

extern HWND winrt_get_window_handle(winrt_platform &winrt) {
	return winrt.specifics->handle;
}