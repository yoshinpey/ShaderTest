#include "GifSprite.h"
#include "Engine\\Camera.h"


//コンストラクタ
GifSprite::GifSprite(string _fileName) :
	vertexNum_(0), pVertexBuffer_(nullptr),
	indexNum(0), pIndexBuffer_(nullptr),
	pConstantBuffer_(nullptr),
	pTexture_(nullptr), spriteFileName_(_fileName),
	fCounter_(MAX_F_COUNT), imageCount_(0), crrImageNum_(0)
{
}


//デストラクタ
GifSprite::~GifSprite()
{
	Release();
}

//初期化
HRESULT GifSprite::Initialize()
{
	//頂点情報
	InitVertexData();					//データを用意して
	if (FAILED(CreateVertexBuffer()))	//頂点バッファ作成
	{
		return E_FAIL;
	}

	//インデックス情報
	InitIndexData();					//データを用意して
	if (FAILED(CreateIndexBuffer()))	//インデックスバッファ作成
	{
		return E_FAIL;
	}

	//コンスタントバッファ作成
	if (FAILED(CreateConstantBuffer()))
	{
		return E_FAIL;
	}

	//テクスチャのロード
	if (FAILED(LoadTexture(spriteFileName_)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//描画
void GifSprite::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_2D);


	transform.Calclation();//トランスフォームを計算

	//コンスタントバッファに情報を渡す
	PassDataToCB(transform.GetWorldMatrix());

	//頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
	SetBufferToPipeline();

	//描画
	Direct3D::pContext_->DrawIndexed(indexNum, 0, 0);
}

//解放
void GifSprite::Release()
{

	SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}


///////////////ここからはprivate関数///////////////

//頂点情報の準備
void GifSprite::InitVertexData()
{
	// 頂点情報
	vertices_ =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },// 四角形の頂点（左上）
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（右上）
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),  XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },// 四角形の頂点（右下）
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（左下）		
	};

	//頂点の数
	vertexNum_ = vertices_.size();	//全データのサイズ　÷　1頂点分のサイズ　＝　頂点数

	//メンバ変数へコピー
	//vertices_ = new VERTEX[vertexNum_];				//必要なサイズの配列にして
	//memcpy(vertices_, vertices, sizeof(vertices));	//コピー
}

//頂点バッファを作成
HRESULT GifSprite::CreateVertexBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//インデックス情報を準備
void GifSprite::InitIndexData()
{
	index_ = { 0,2,3, 0,1,2 };

	//インデックス数
	indexNum = index_.size();

	//メンバ変数へコピー
	//index_ = new int[indexNum];
	//memcpy(index_, index, sizeof(index));
}

//インデックスバッファを作成
HRESULT GifSprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//コンスタントバッファ作成
HRESULT GifSprite::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

//テクスチャをロード
HRESULT GifSprite::LoadTexture(string _fileName)
{
	pTexture_ = new GifTexture;

	HRESULT hr;
	hr = pTexture_->Load(_fileName);
	if (FAILED(hr))
	{
		MessageBox(NULL, "テクスチャの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	imageCount_ = pTexture_->GetTextureNum();
	return S_OK;
}

//コンスタントバッファに各種情報を渡す
void GifSprite::PassDataToCB(XMMATRIX worldMatrix)
{
	if (GetCounter()) {
		crrImageNum_ = (++crrImageNum_) % imageCount_;
		ResetCounter();
	}
	else
		UpdateCounter();
	CONSTANT_BUFFER cb;
	cb.matW = XMMatrixTranspose(worldMatrix);

	Direct3D::pContext_->UpdateSubresource(pConstantBuffer_, 0, NULL, &cb, 0, 0);

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(crrImageNum_, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(crrImageNum_, 1, &pSRV);
}

//各バッファをパイプラインにセット
void GifSprite::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
}

void GifSprite::UpdateCounter()
{
	fCounter_--;
}

bool GifSprite::GetCounter()
{
	return(fCounter_ < 0);
}

void GifSprite::ResetCounter()
{
	fCounter_ = MAX_F_COUNT;
}
