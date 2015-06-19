#include "d3dutility.h"
#include <Windows.h>
#include "Config.h"
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


void render(Camera *camera, Terrain *terrain, SkyBox *skybox);
vector<vector<int> > randPos(int cnt);

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
	//g_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    //���߱�׼��

	// enter the main loop:

	MSG msg;
	
	Camera *camera = new Camera(&D3DXVECTOR3(160,0,0));
	camera->transUp(-90);

	SkyBox *skybox = new SkyBox(g_device);
	skybox->initTexture("res/cloudsky");
	skybox->initVertexs();

	Terrain *terrain = new Terrain(g_device);
	terrain->initTexture("res/terrain.bmp");
	terrain->initHeightMap("res/heightmapdata2.raw", 257,SKYBOX_SIZE/256);
	terrain->generateVertex();

	SnowMan *snowMan = new SnowMan(g_device, terrain, "res/Snowman2.x");
	snowMan->initMesh();

	House *house = new House(g_device, terrain, "res/house/house.x");
	house->initMesh();

	Tree *tree = new Tree(g_device, terrain, "res/tree/firtree4.x");
	tree->initMesh();
	vector< vector<int> > randomPoints;
	randomPoints = randPos(30);


	SnowFlakeManager *manager = new SnowFlakeManager(g_device, terrain, "res/snowflake/snowflake.jpg", 800);
	manager->initSnowFlakes();

	Projection(60, 1, 1024);

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

		static POINT previousMousePoint;

		POINT currentMousePoint;
		GetCursorPos(&currentMousePoint);
		ScreenToClient(g_hwnd, &currentMousePoint);

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

		manager->updateSnowFlakes(deltaTime);

		//Camera�ĸ߶���Terrain��ǰ���yֵ���ơ�
		D3DXVECTOR3 camPos;
		camera->getPosition(&camPos);
		camPos.y = terrain->getHeight(camPos.x, camPos.z) + 20.0f;
		camera->setPosition(&camPos);

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
		snowMan->draw(0,60,0);
		snowMan->draw(50, 60, 0);
		snowMan->draw(-50, 60, 0);
		//��
		snowMan->draw(0, -80, 180);
		snowMan->draw(-50, -80, 180);
		snowMan->draw(50, -80, 180);
		//ǰ
		snowMan->draw(90, -10, 90);
		snowMan->draw(50, 40, 90);
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

		manager->draw();

		g_device->EndScene();
		g_device->Present(NULL, NULL, NULL, NULL);

		
	}

	//do clean job
	

	return msg.wParam;
}


/*
	�����������λ�úʹ�С

	scale��ΧΪ[3,8]
*/
vector<vector<int> > randPos(int cnt){
	vector<vector<int> > v;
	srand((int)timeGetTime());

	for (int i = 0; i < cnt; i++){
		int x, y, scale;
		vector<int> vi;
		while (true){
			x = rand() % 450;
			y = rand() % 450;
			if (x*x + y*y > 120 * 120){  //�������ڵ�Բ��Χ��
				break;
			}
		}
		int sign1 = rand() % 2 == 0 ? 1 : -1;
		int sign2 = rand() % 2 == 0 ? 1 : -1;
		
		scale = rand() % 5 + 3;
		vi.push_back(x*sign1);
		vi.push_back(y*sign2);
		vi.push_back(scale);
		v.push_back(vi);
	}
	return v;
}
