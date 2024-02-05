#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Transform.h"
#include "Direct3D.h"
#include "Texture.h"
#include "Camera.h"

using namespace DirectX;

#define SAFE_DELETE_ARRAY(p) if(p != nullptr){ delete p; p = nullptr;}

//四角形ポリゴン（三角形を２枚）を描画するクラス
class Sprite
{
private:
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matNormal;		//ワールド行列
		XMMATRIX	uvTrans;	// テクスチャ座標変換行列
		XMFLOAT4	color;		// テクスチャとの合成色
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;			//位置
		XMVECTOR uv;				//UV
	};

	//--------------Initilaizeから呼ばれる関数
	virtual void InitVertexData();		//頂点情報準備
	HRESULT CreateVertexBuffer();		//頂点バッファ作成
	virtual void InitIndexData();		//インデックス情報を準備
	HRESULT CreateIndexBuffer();		//インデックスバッファ作成
	HRESULT CreateConstantBuffer();		//コンスタントバッファ作成
	HRESULT LoadTexture();				//テクスチャをロード

	HRESULT LoadTexture(std::string fileName);

	//--------------Draw関数から呼ばれる関数
	void PassDataToCB(Transform transformmatNormal);		//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();								//各バッファをパイプラインにセット

protected:
	uint64_t vertexNum_;				//頂点数
	std::vector<VERTEX> vertices_;		//頂点情報
	ID3D11Buffer* pVertexBuffer_;		//頂点バッファ

	uint64_t indexNum_;					//インデックス数
	std::vector<int> index_;			//インデックス情報
	ID3D11Buffer* pIndexBuffer_;		//インデックスバッファ

	ID3D11Buffer* pConstantBuffer_;		//コンスタントバッファ

	Texture* pTexture_;					//テクスチャ

public:
	Sprite();
	~Sprite();

	//各情報初期化、戻り値：失敗/成功
	HRESULT Initialize();				

	//引数：transform	トランスフォームクラスオブジェクト
	void Draw(Transform& transform);	
	void Draw(Transform& transform, RECT rect, float alpha);

	//画像サイズの取得
	//戻値：画像サイズ
	XMFLOAT2 GetTextureSize() { return pTexture_->GetTextureSize(); }

	//ロード
	//引数：fileName	画像ファイル名
	//戻値：成功/失敗
	HRESULT Load(std::string fileName);

	//解放
	void Release();						
};

