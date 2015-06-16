#include "Camera.h"

Camera::Camera(){
	this->pos = D3DXVECTOR3(0.0, 0.0, 0.0);
	this->right = D3DXVECTOR3(1.0, 0.0, 0.0);
	this->up = D3DXVECTOR3(0.0, 1.0, 0.0);
	this->look = D3DXVECTOR3(0.0, 0.0, 1.0);
}

Camera::Camera(D3DXVECTOR3* pos){
	this->pos = *pos;
	this->right = D3DXVECTOR3(1.0, 0.0, 0.0);
	this->up = D3DXVECTOR3(0.0, 1.0, 0.0);
	this->look = D3DXVECTOR3(0.0, 0.0, 1.0);
}

Camera::~Camera(){

}

void Camera::getRight(D3DXVECTOR3* right){
	*right = this->right;
}

void Camera::getUp(D3DXVECTOR3* up){
	*up = this->up;
}

void Camera::getLook(D3DXVECTOR3* look){
	*look = this->look;
}

void Camera::getPosition(D3DXVECTOR3* pos){
	*pos = this->pos;
}

void Camera::setPosition(D3DXVECTOR3* pos){
	this->pos = *pos;
}


/*
	rotate.
	ע��D3DXVec3TransformNormal��D3DXVec3TransformCoord���𣬷ֱ���������͵㣬
	������ò��û��Ӱ�졣
*/
void Camera::transLook(float angleDegree){
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &look, D3DXToRadian(angleDegree)); //�����������ת
	D3DXVec3TransformNormal(&right, &right, &t);
	D3DXVec3TransformNormal(&up, &up, &t);

}

void Camera::transUp(float angleDegree){
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &up, D3DXToRadian(angleDegree));
	D3DXVec3TransformNormal(&right, &right, &t);
	D3DXVec3TransformNormal(&look, &look, &t);

}

void Camera::transRight(float angleDegree){
	D3DXMATRIX t;
	D3DXMatrixRotationAxis(&t, &right, D3DXToRadian(angleDegree));
	D3DXVec3TransformNormal(&up, &up, &t);
	D3DXVec3TransformNormal(&look, &look, &t);

}

//move
void Camera::moveFB(float dis){
	this->pos += (this->look)*dis;
}

void Camera::moveLR(float dis){
	this->pos += (this->right)*dis;
}

void Camera::moveUD(float dis){
	this->pos += (this->up) * dis ;
}

/*
	���� viewport ����
	��α任��up��look��right�ܾ���Ӱ�첻��ֱ����Ҫ�����趨����֤�����淶����
*/
void Camera::getViewportMatrix(D3DXMATRIX *mat){

	//����Ҫע����˳��
	D3DXVec3Normalize(&look, &look); 
	D3DXVec3Cross(&up, &look, &right);
	D3DXVec3Normalize(&up, &up);
	D3DXVec3Cross(&right, &up, &look);
	D3DXVec3Normalize(&look, &look);

	float v_30 = -D3DXVec3Dot(&right, &pos);
	float v_31 = -D3DXVec3Dot(&up, &pos);
	float v_32 = -D3DXVec3Dot(&look, &pos);

	(*mat)(0, 0) = right.x; (*mat)(0, 1) = up.x; (*mat)(0, 2) = look.x; (*mat)(0, 3) = 0.0f;
	(*mat)(1, 0) = right.y; (*mat)(1, 1) = up.y; (*mat)(1, 2) = look.y; (*mat)(1, 3) = 0.0f;
	(*mat)(2, 0) = right.z; (*mat)(2, 1) = up.z; (*mat)(2, 2) = look.z; (*mat)(2, 3) = 0.0f;
	(*mat)(3, 0) = v_30;    (*mat)(3, 1) = v_31; (*mat)(3, 2) = v_32;   (*mat)(3, 3) = 1.0f;

}