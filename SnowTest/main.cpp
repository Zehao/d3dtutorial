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

	//摄像机
	Camera *camera = new Camera(&D3DXVECTOR3(160,0,0));
	camera->transUp(-90);

	//天空盒
	SkyBox *skybox = new SkyBox(g_device);
	skybox->initTexture("res/cloudsky");
	skybox->initVertexs();

	//地形
	Terrain *terrain = new Terrain(g_device);
	terrain->initTexture("res/terrain/terrain.bmp");
	terrain->initHeightMap("res/heightmap/heightmap.raw", 257,SKYBOX_SIZE/256);
	terrain->generateVertex();

	//雪人
	SnowMan *snowMan = new SnowMan(g_device, terrain, "res/snowman/snowman.x");
	snowMan->initMesh();
	snowMan->setLight(&g_light);

	//房子
	House *house = new House(g_device, terrain, "res/house/house.x");
	house->initMesh();
	house->setLight(&g_light);

	//树
	Tree *tree = new Tree(g_device, terrain, "res/tree/firtree4.x");
	tree->initMesh();
	tree->setLight(&g_light);
	vector< vector<int> > randomPoints;
	randomPoints = randPos(30);

	//雪花
	SnowFlakeManager *flakeManager = new SnowFlakeManager(g_device, terrain, "res/snowflake/snowflake.dds", 2000);
	flakeManager->initSnowFlakes();

	//投影参数
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

		static float previousTime = (float)timeGetTime(); //初始化时间

		float currentTime = (float)timeGetTime();
		float deltaTime = (currentTime - previousTime)*0.001f;
		previousTime = currentTime;

		/*
		获取按键输入
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

		//获取点击点
		static POINT previousMousePoint;

		POINT currentMousePoint;
		GetCursorPos(&currentMousePoint);
		ScreenToClient(g_hwnd, &currentMousePoint);
		//根据鼠标移动镜头
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

		//每帧更新雪花
		flakeManager->updateSnowFlakes(deltaTime);

		//Camera的高度受Terrain当前点的y值控制。
		D3DXVECTOR3 camPos;
		camera->getPosition(&camPos);
		camPos.y = terrain->getHeight(camPos.x, camPos.z) + 20.0f;
		camera->setPosition(&camPos);

		//根据当前camera设置viewport
		D3DXMATRIX viewMat;
		camera->getViewportMatrix(&viewMat);
		g_device->SetTransform(D3DTS_VIEW, &viewMat);

		g_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		g_device->BeginScene();

		skybox->draw(camera);

		terrain->draw();

		//打开灯光
		turnOnLight();

		house->draw(0,0);

		//画多个不同朝向的雪人 ( x , z, rotateAngle ) 在房子周围

		//左
		snowMan->draw(5,65,0);
		snowMan->draw(50, 65, 0);
		snowMan->draw(-50, 65, 0);
		//右
		snowMan->draw(0, -90, 180);
		snowMan->draw(-50, -90, 180);
		snowMan->draw(50, -90, 180);
		//前
		snowMan->draw(50, -15, 90);
		snowMan->draw(50, 30, 90);
		snowMan->draw(50, -40, 90);
		//后
		snowMan->draw(-120, -10, 270);
		snowMan->draw(-100, 40, 270);
		snowMan->draw(-100, -40, 270);

		//随机生成20棵不同位置和大小的树
		for (int i = 0; i < randomPoints.size(); i++){
			tree->draw(randomPoints[i][0], randomPoints[i][1], randomPoints[i][2]);
		}
		
		//关闭灯光
		turnOffLight();
		//雪花
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