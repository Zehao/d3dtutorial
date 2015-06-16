#include "d3dutility.h"
#include <Windows.h>
#include "Config.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib,"winmm.lib")




LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
						   PostQuitMessage(0);
						   return 0;
		} break;
	}

	//系统处理
	return DefWindowProc(hWnd, message, wParam, lParam);
}


bool initWindowClass(HINSTANCE hInstance, int nCmdShow){

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	//获取屏幕宽高
	int   cx = GetSystemMetrics(SM_CXSCREEN);
	int   cy = GetSystemMetrics(SM_CYSCREEN);

	//设置窗口在屏幕中央
	g_hwnd = CreateWindowEx(NULL, "WindowClass", "SnowTest",
		WS_OVERLAPPEDWINDOW, (cx - g_width) / 2, (cy - g_height) / 2, g_width, g_height,
		NULL, NULL, hInstance, NULL);

	ShowWindow(g_hwnd, nCmdShow);
	UpdateWindow(g_hwnd);
	return true;
}



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

	Terrain *terrain = new Terrain(g_device);
	terrain->initTexture("res/terrainTex.jpg");
	terrain->initHeightMap("res/hm.raw", 513);
	terrain->generateVertex();

	

	static float previousTime = (float)timeGetTime(); //初始化时间
	float currentTime;												  //当前时间
	float deltaTime;                                                  //时间差

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;


		/**
		Camera的高度受Terrain当前点的y值控制。
		*/
		D3DXVECTOR3 camPos;
		camera->getPosition(&camPos);
		camPos.y = terrain->getHeight(camPos.x, camPos.z) + 20.0f;
		camera->setPosition(&camPos);

		currentTime = (float)timeGetTime();
		deltaTime = (currentTime - previousTime)*0.001f;
		previousTime = currentTime;

		if (KEY_DOWN('W')){
			camera->moveFB(CAM_MOVESPEED*deltaTime);
		}
		if (KEY_DOWN('A')){
			camera->moveLR(-CAM_MOVESPEED*deltaTime);
		}
		if (KEY_DOWN('S')){
			camera->moveFB(-CAM_MOVESPEED*deltaTime);
		}
		if (KEY_DOWN('D')){
			camera->moveLR(CAM_MOVESPEED*deltaTime);
		}
		if (KEY_DOWN(VK_LBUTTON)){
			
		}else{

		}

		g_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		Projection(60, 0, 1000);

		D3DXVECTOR3 pos(0, 512, 700);
		D3DXVECTOR3 target(0, 0,-512);
		Viewport(pos, target);

		D3DXMATRIX viewMat;
		//camera->getViewportMatrix(&viewMat);
		//g_device->SetTransform(D3DTS_VIEW, &viewMat);

		g_device->BeginScene();

		skybox->draw(camera);
		terrain->draw();

		g_device->EndScene();
		g_device->Present(NULL, NULL, NULL, NULL);
		
	}

	//do clean job
	

	return msg.wParam;
}

