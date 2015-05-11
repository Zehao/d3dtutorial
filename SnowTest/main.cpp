#include "d3dutility.h"
#include <Windows.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib,"winmm.lib")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	initWindowClass(hInstance, nCmdShow);

	//initialize Direct3D
	initD3d();

	//init light
	//initLight();

	g_device->SetRenderState(D3DRS_LIGHTING, TRUE); //灯光
	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    //法线标准化

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		renderFrame();
	}

	//do clean job
	

	return msg.wParam;
}

