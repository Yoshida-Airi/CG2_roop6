#pragma once
#include"Matrix4x4.h"
#include"Vector3.h"
#include<assert.h>
#include<cmath>

struct Transform
{
	Vector3 scale;		//サイズ
	Vector3 rotate;		//回転
	Vector3 translate;	//位置
};

/*=====================================*/
/* 　　　　   　　　行列　　　 　　　　 　　 */
/*=====================================*/

//1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

//2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

//3.行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

//4.逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

//5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

//6.単位行列の作成
Matrix4x4 MakeIdentity4x4();


/*=====================================*/
/* 　　　　　　拡大縮小と平行移動　　 　　　 */
/*=====================================*/

//1.平行移動行列
Matrix4x4 MakeTranselateMatrix(const Vector3& transelate);

//2.拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//3.座標変換
Vector3 Transformation(const Vector3& vector, const Matrix4x4& matrix);


/*=====================================*/
/* 　　　　   　　　回転　　　 　　　　 　　 */
/*=====================================*/

//1.x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

//2.y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

//3.z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);


/*=====================================*/
/* 　　　 　　　　アフィン変換　　　　　　 　 */
/*=====================================*/

//3次元アフィン変換行列
Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);