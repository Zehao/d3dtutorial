#ifndef __D3DUTILITY_H__
#define __D3DUTILITY_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
using namespace std;

//全局变量
IDirect3D9* g_d3d = 0;
IDirect3DDevice9* g_device = 0;
D3DLIGHT9 g_light;  //全局灯光
HWND g_hwnd = 0;

//窗口大小
const int g_width = 1024;
const int g_height = 768;

//摄像机移动速度
const int CAM_MOVE_SPEED = 30;
const int CAM_ROTATE_SPEED = 20;

#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)

#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)

//投影
void Projection(int angleDegree , float nearZ , float farZ ){
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
		D3DXToRadian(angleDegree),
		(float)g_width / (float)g_height,
		nearZ,
		farZ);
	g_device->SetTransform(D3DTS_PROJECTION, &proj);
}

//初始化D3D
bool initD3d(){
	g_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (FAILED(g_d3d)){
		MessageBox(0, "Direct3DCreate9  FAILED", 0, 0);
		return false;
	}

	D3DPRESENT_PARAMETERS params;
	memset(&params, 0, sizeof(params));

	params.Windowed = TRUE;
	params.hDeviceWindow = g_hwnd;

	//for backbuffer
	//params.BackBufferFormat = dm.Format;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	params.BackBufferWidth = g_width;
	params.BackBufferHeight = g_height;
	params.BackBufferCount = 1;


	params.MultiSampleType = D3DMULTISAMPLE_NONE; //全屏时抗锯齿
	params.MultiSampleQuality = 0;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换链中最有效的DISCARD

	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D24S8; //深度24，模板8

	//全屏时
	params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	params.Flags = 0;

	D3DCAPS9 caps;
	g_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//硬件是否支持顶点处理
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//创建设备
	HRESULT hr = g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, vp, &params, &g_device);
	if (FAILED(hr)){
		MessageBox(0, "CreateDevice  FAILED", 0, 0);
		return false;
	}
	g_d3d->Release();

	return true;
}


void initLight(D3DLIGHT9 &light){

	//灯光
	//D3DLIGHT9 light;
	memset(&light, 0, sizeof(light));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Direction = D3DXVECTOR3(-1, -1, 1);//根据天空盒太阳位置得出
	//light.Direction = D3DXVECTOR3(1, 1, 1);
	D3DXCOLOR white = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	light.Ambient = white*0.9f;
	light.Diffuse = white*0.4f;
	light.Specular = white*0.5f;
	//dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	g_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	
}


void turnOnLight(){
	g_device->SetRenderState(D3DRS_LIGHTING, TRUE);

	g_device->SetLight(0, &g_light);
	g_device->LightEnable(0, true);
	g_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_device->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	//dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void turnOffLight(){
	g_device->SetRenderState(D3DRS_LIGHTING, FALSE); 
}

/*
围绕房屋所在的范围，随机生成树的位置和大小
scale范围为[3,8]
*/
vector<vector<int> > randPos(int cnt){
	vector<vector<int> > v;
	int range = 150;
	srand((int)timeGetTime());

	for (int i = 0; i < cnt; i++){
		int x, y, scale;
		vector<int> vi;
		while (true){
			x = rand() % 450;
			y = rand() % 450;
			if (x*x + y*y >= range * range){  //房屋所在的圆范围外
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

#endif