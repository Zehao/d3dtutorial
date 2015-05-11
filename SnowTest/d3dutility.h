#ifndef __D3DUTILITY_H__
#define __D3DUTILITY_H__
/*
	dx3d��һЩ���ú��������ݽṹ�ķ�װ

*/
#include <d3d9.h>
#include <d3dx9.h>
#include "winutility.h"

//global
IDirect3D9* g_d3d = 0;
IDirect3DDevice9* g_device = 0;

D3DXVECTOR3 CAMERA_Y(0.0f, 1.0f, 0.0f);



void Projection(int angleDegree , float nearZ , float farZ ){
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj,
		D3DXToRadian(angleDegree),
		(float)g_width / (float)g_height,
		nearZ,
		farZ);
	g_device->SetTransform(D3DTS_PROJECTION, &proj);
}


void Viewport(const D3DXVECTOR3 &position, const D3DXVECTOR3 &target, const D3DXVECTOR3 &upside = CAMERA_Y){

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(
		&V,
		&position,
		&target,
		&upside);
	g_device->SetTransform(D3DTS_VIEW, &V);
}


bool initD3d(){
	g_d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!g_d3d){
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


	params.MultiSampleType = D3DMULTISAMPLE_NONE; //ȫ��ʱ�����
	params.MultiSampleQuality = 0;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;//������������Ч��DISCARD

	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D24S8; //���24��ģ��8

	//ȫ��ʱ
	params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	params.Flags = 0;

	D3DCAPS9 caps;
	g_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	//Ӳ���Ƿ�֧�ֶ��㴦��
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//�����豸
	HRESULT hr = g_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hwnd, vp, &params, &g_device);
	if (FAILED(hr)){
		MessageBox(0, "CreateDevice  FAILED", 0, 0);
		return false;
	}
	g_d3d->Release();
	return true;
}

void renderFrame(){

}

#endif