#include "d3dutility.h"
#include <Windows.h>
#include "Camera.h"
#include "SkyBox.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib,"winmm.lib")


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	initWindowClass(hInstance, nCmdShow);

	//initialize Direct3D
	initD3d();

	//init light
	//initLight();

	g_device->SetRenderState(D3DRS_LIGHTING, FALSE); //灯光
	g_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    //法线标准化

	// enter the main loop:

	MSG msg;
	Camera *camera = new Camera();
	SkyBox *skybox = new SkyBox(g_device);
	skybox->initTexture("res/snowblind");
	skybox->initVertexs();
	
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;

		g_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		Projection(60, 0, 1000);
		D3DXVECTOR3 pos(0, 20, 0);
		D3DXVECTOR3 target(0, 0,512);
		Viewport(pos, target);
		g_device->BeginScene();
		skybox->draw(camera);
		g_device->EndScene();
		g_device->Present(NULL, NULL, NULL, NULL);
		//renderFrame();
	}

	//do clean job
	

	return msg.wParam;
}

