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

	//ϵͳ����
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

	//��ȡ��Ļ���
	int   cx = GetSystemMetrics(SM_CXSCREEN);
	int   cy = GetSystemMetrics(SM_CYSCREEN);

	//���ô�������Ļ����
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

	g_device->SetRenderState(D3DRS_LIGHTING, FALSE); //�ƹ�
	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    //���߱�׼��

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

	//g_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

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
		Camera�ĸ߶���Terrain��ǰ���yֵ���ơ�
		*/
		D3DXVECTOR3 camPos;
		camera->getPosition(&camPos);
		camPos.y = terrain->getHeight(camPos.x, camPos.z) + 5.0f;
		camera->setPosition(&camPos);


		g_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		Projection(45, 0, 1000);

		D3DXVECTOR3 pos(0, 512,-512);
		D3DXVECTOR3 target(0, 0,0);
		//Viewport(pos, target);

		D3DXMATRIX viewMat;
		camera->getViewportMatrix(&viewMat);
		g_device->SetTransform(D3DTS_VIEW, &viewMat);
		g_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_device->BeginScene();
		
		//skybox->draw(camera);
		
		terrain->draw();

		g_device->EndScene();
		g_device->Present(NULL, NULL, NULL, NULL);
		
	}

	//do clean job
	

	return msg.wParam;
}



void computeMouse(){

}

void procCamera(Camera *camera){

	static float previousTime = (float)timeGetTime(); //��ʼ��ʱ��

	static POINT previousMousePoint;

	g_currentTime = (float)timeGetTime();
	g_deltaTime = (g_currentTime - previousTime)*0.001f;
	previousTime = g_currentTime;

	/*
	��ȡ��������
	*/
	bool isMousing;
	if (KEY_DOWN('W')){
		camera->moveFB(CAM_MOVESPEED*g_deltaTime);
	}
	if (KEY_DOWN('A')){
		camera->moveLR(-CAM_MOVESPEED*g_deltaTime);
	}
	if (KEY_DOWN('S')){
		camera->moveFB(-CAM_MOVESPEED*g_deltaTime);
	}
	if (KEY_DOWN('D')){
		camera->moveLR(CAM_MOVESPEED*g_deltaTime);
	}
	if (KEY_DOWN(VK_LBUTTON)){
		isMousing = true;
	}
	else{
		isMousing = false;
	}

	POINT mousePoint;
	GetCursorPos(&mousePoint);
	ScreenToClient(g_hwnd, &mousePoint);

	if (isMousing == true){

	}

}