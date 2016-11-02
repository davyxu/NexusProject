#pragma once

class TriangleVB : public VertexStream
{
public:
	struct Vertex2D 
	{
		Vector3 mPosition;
		float mRHW;
		dword mColor;
		Vertex2D( const Vector3& p, dword c )
			: mPosition( p )
			, mColor( c )
		{
			mRHW = 1.0f;
		}
	};


	TriangleVB( );

	virtual dword GetStride();

	virtual VertexFormat* GetVertexFormat( );

	virtual dword GetSize();

	virtual void GetData(void* data);

	void Render( IGenericRender* render );

private:
	friend class Demo_Primitive2D;
	VertexFormat mVertexFormat;

	typedef std::vector<Vertex2D> VertexBuffer;
	VertexBuffer mVB;
};



class QuadIB : public IndexStream
{
public:
	virtual dword GetSize();	

	virtual void GetData(void* data);
};

class QuadVB : public VertexStream
{
public:
	struct Vertex2D 
	{
		Vector3 mPosition;
		float mRHW;
		dword mColor;
		Vector2 mUV;
		Vertex2D( const Vector3& p, dword c, const Vector2& uv )
			: mPosition( p )
			, mColor( c )
			, mUV( uv )
		{
			mRHW = 1.0f;
		}
	};


	struct QuadEffect : RenderEffect 
	{
		ShaderParameter		mParameterColor;
		ShaderParameter		mShaderTexture1;
		ShaderParameter		mShaderTexture2;

		QuadEffect( )
			: RenderEffect( L"material\\demo_primitive2d.effect" )
		{
			Bind( &mParameterColor, "vecBlend"  );
			Bind( &mShaderTexture1, "texInput1"  );
			Bind( &mShaderTexture2, "texInput2"  );
		}
	};

	QuadVB( );

	virtual VertexFormat* GetVertexFormat( );

	virtual dword GetStride();

	virtual dword GetSize();

	virtual void GetData(void* data);

	void Render( IGenericRender* render );

private:
	friend class Demo_Primitive2D;
	QuadEffect mEffect;
	Texture2D mTexture1;
	Texture2D mTexture2;

	typedef std::vector<Vertex2D> VertexBuffer;
	VertexBuffer mVB;
	QuadIB mIB;

	VertexFormat mVertexFormat;


};


class Demo_Primitive2D : public SceneListener
{
	DECLARE_RTTIOBJECT(Demo_Primitive2D);

public:
	virtual void OnEnter( );

	virtual void OnLeave( );

	virtual void OnRender( RenderContext* rc  );
private:
	TriangleVB mTriangleVB;
	QuadVB mQuadVB;
};