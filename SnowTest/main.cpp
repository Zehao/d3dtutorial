#include <Windows.h>
#include "d3dutility.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "SnowMan.h"
#include "House.h"
#include "Tree.h"
#include "SnowFlake.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib,"winmm.lib")


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool initWindowClass(HINSTANCE hInstance, int nCmdShow);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	initWindowClass(hInstance, nCmdShow);

	initD3d();

	initLight(g_light);

	//�����
	Camera *camera = new Camera(&D3DXVECTOR3(160,0,0));
	camera->transUp(-90);

	//��պ�
	SkyBox *skybox = new SkyBox(g_device);
	skybox->initTexture("res/cloudsky");
	skybox->initVertexs();

	//����
	Terrain *terrain = new Terrain(g_device);
	terrain->initTexture("res/terrain/terrain.bmp");
	terrain->initHeightMap("res/heightmap/heightmap.raw", 257,SKYBOX_SIZE/256);
	terrain->generateVertex();

	//ѩ��
	SnowMan *snowMan = new SnowMan(g_device, terrain, "res/snowman/snowman.x");
	snowMan->initMesh();
	snowMan->setLight(&g_light);

	//����
	House *house = new House(g_device, terrain, "res/house/house.x");
	house->initMesh();
	house->setLight(&g_light);

	//��
	Tree *tree = new Tree(g_device, terrain, "res/tree/firtree4.x");
	tree->initMesh();
	tree->setLight(&g_light);
	vector< vector<int> > randomPoints;
	randomPoints = randPos(30);

	//ѩ��
	SnowFlakeManager *flakeManager = new SnowFlakeManager(g_device, terrain, "res/snowflake/snowflake.dds", 2000);
	flakeManager->initSnowFlakes();

	//ͶӰ����
	Projection(60, 1, 1024);

	//main loop
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

		static float previousTime = (float)timeGetTime(); //��ʼ��ʱ��

		float currentTime = (float)timeGetTime();
		float deltaTime = (currentTime - previousTime)*0.001f;
		previousTime = currentTime;

		/*
		��ȡ��������
		*/
		bool isMousing;
		if (KEY_DOWN('W')){
			camera->moveFB(CAM_MOVE_SPEED*deltaTime);
		}
		if (KEY_DOWN('A')){
			camera->moveLR(-CAM_MOVE_SPEED*deltaTime);
		}
		if (KEY_DOWN('S')){
			camera->moveFB(-CAM_MOVE_SPEED*deltaTime);
		}
		if (KEY_DOWN('D')){
			camera->moveLR(CAM_MOVE_SPEED*deltaTime);
		}
		if (KEY_DOWN(VK_LBUTTON)){
			isMousing = true;
		}
		else{
			isMousing = false;
		}

		//��ȡ�����
		static POINT previousMousePoint;

		POINT currentMousePoint;
		GetCursorPos(&currentMousePoint);
		ScreenToClient(g_hwnd, &currentMousePoint);
		//��������ƶ���ͷ
		if (isMousing == true){
			int dx = currentMousePoint.x - previousMousePoint.x;
			int dy = currentMousePoint.y - previousMousePoint.y;
			if (dx != 0){
				camera->transUp((float)dx / CAM_ROTATE_SPEED);
			}
			if (dy != 0){
				camera->transRight((float)dy / CAM_ROTATE_SPEED);
			}
		}
		previousMousePoint = currentMousePoint;

		//ÿ֡����ѩ��
		flakeManager->updateSnowFlakes(deltaTime);

		//Camera�ĸ߶���Terrain��ǰ���yֵ���ơ�
		D3DXVECTOR3 camPos;
		camera->getPosition(&camPos);
		camPos.y = terrain->getHeight(camPos.x, camPos.z) + 20.0f;
		camera->setPosition(&camPos);

		//���ݵ�ǰcamera����viewport
		D3DXMATRIX viewMat;
		camera->getViewportMatrix(&viewMat);
		g_device->SetTransform(D3DTS_VIEW, &viewMat);

		g_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		g_device->BeginScene();

		skybox->draw(camera);

		terrain->draw();

		//�򿪵ƹ�
		turnOnLight();

		house->draw(0,0);

		//�������ͬ�����ѩ�� ( x , z, rotateAngle ) �ڷ�����Χ

		//��
		snowMan->draw(5,65,0);
		snowMan->draw(50, 65, 0);
		snowMan->draw(-50, 65, 0);
		//��
		snowMan->draw(0, -90, 180);
		snowMan->draw(-50, -90, 180);
		snowMan->draw(50, -90, 180);
		//ǰ
		snowMan->draw(50, -15, 90);
		snowMan->draw(50, 30, 90);
		snowMan->draw(50, -40, 90);
		//��
		snowMan->draw(-120, -10, 270);
		snowMan->draw(-100, 40, 270);
		snowMan->draw(-100, -40, 270);

		//�������20�ò�ͬλ�úʹ�С����
		for (int i = 0; i < randomPoints.size(); i++){
			tree->draw(randomPoints[i][0], randomPoints[i][1], randomPoints[i][2]);
		}
		
		//�رյƹ�
		turnOffLight();
		//ѩ��
		flakeManager->draw();

		g_device->EndScene();
		g_device->Present(NULL, NULL, NULL, NULL);

		
	}

	//do clean job
	

	return msg.wParam;
}





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