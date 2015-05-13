#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <d3d9.h>
#include <d3dx9.h>

/*
	摄像机类，包括了位置和3个方向（轴），可以向不同的方向旋转和移动。

	类中最主要的是获取viewport变换矩阵的getViewportMatrix方法，
	参考 Frank Luna 的 Introduction to 3D Game Programming with DirectX 9.0

*/

class Camera{

private:
	D3DXVECTOR3 pos; //位置

	//方向
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 look;

public:

	//getters and setters
	void getRight(D3DXVECTOR3* right);

	void getUp(D3DXVECTOR3* up);

	void getLook(D3DXVECTOR3* look);

	void getPosition(D3DXVECTOR3* pos);

	void setPosition(D3DXVECTOR3* pos);

	//rotate，angleDegree为角度
	void transLook(float angleDegree);

	void transUp(float angleDegree);

	void transRight(float angleDegree);

	//move
	void moveFB(float dis); // forward & backward

	void moveLR(float dis); // left & right

	void moveUD(float dis); // up & down

	//view matrix
	void getViewportMatrix(D3DXMATRIX *mat);
public:

	Camera();

	Camera(D3DXVECTOR3* pos);

	virtual ~Camera();
};

#endif