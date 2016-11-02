#include "EditorPCH.h"
#include "PrivateInclude.h"


QRenderPanel::QRenderPanel( QWidget* Parent /*= NULL */ )
: QWidget( Parent )
, mRenderWindow( null )
, mInputDriver( null )
{
	setAttribute(Qt::WA_PaintOnScreen, true );

	// 鼠标移动时接收MOUSE_MOVE消息
	setMouseTracking( true );

	setFocusPolicy(Qt::WheelFocus);
	setAttribute(Qt::WA_NoBackground);
}







QRenderPanel::~QRenderPanel()
{

}


void QRenderPanel::Initialize( RTTIClass* CameraControllerType )
{
	mRenderWindow = GEngine->CreateGameWindow( (handle)winId(), false );

	mInputDriver = dynamic_cast<MessageInputDriver*>( mRenderWindow->GetInputDriver() );
	mInputDriver->SetReceiveOnActive( false );

	mWorld = GEngine->CreateWorld();

	mCameraActor = RTTI_CAST(CameraActor, 
		mWorld->SpawnActor( CameraControllerType, 
		StringHelper::FormatString( L"@EditorCamera_WindowType%p", this ), 
		null,  
		mInputDriver ) );

	mRenderWindow->AddSceneView( mWorld, mCameraActor->GetCamera() );
}

PresentWindow* QRenderPanel::GetRenderWindow( )
{
	return mRenderWindow;
}

void QRenderPanel::resizeEvent( QResizeEvent* evt )
{
	if ( mRenderWindow )
		mRenderWindow->Resize( width(), height());
}

//void QPresentPanel::OnTick()
//{
//	update( );
//}
//
//void QPresentPanel::paintEvent( QPaintEvent* Event )
//{
//	// 避免闪屏
//	if ( updatesEnabled() )
//	{
//		if ( GEngine )
//			GEngine->Tick();
//	}
//}



void QRenderPanel::mouseMoveEvent( QMouseEvent *evt )
{
	Point MP( evt->x(), evt->y() );

	mInputDriver->NotifyMousePos( MP );
}

void QRenderPanel::mousePressEvent( QMouseEvent *evt )
{
	__super::mousePressEvent( evt );

	NotifyMouseEvent( evt, MKS_Down );
}

void QRenderPanel::NotifyMouseEvent( QMouseEvent *evt, eMessageKeyState MKS )
{
	Point MP( evt->x(), evt->y() );

	mInputDriver->NotifyMousePos( MP );

	eMouseButton Button;
	switch ( evt->button() )
	{
	case Qt::LeftButton:		
		Button = MB_Left;
		break;
	case Qt::RightButton:
		Button = MB_Right;
		break;
	case Qt::MidButton:
		Button = MB_Middle;
		break;
	default:
		return;
	}

	mInputDriver->NotifyMouseState( Button, MKS );
}




void QRenderPanel::wheelEvent( QWheelEvent *evt )
{
	mInputDriver->NotifyMouseScroll( evt->delta( ));
}

bool QRenderPanel::event( QEvent * evt )
{
	if ( evt->type() == QEvent::MouseButtonRelease )
	{
		QMouseEvent* me = (QMouseEvent*)evt;

		NotifyMouseEvent( me, MKS_Up );	
	}


	return __super::event( evt );
}

void QRenderPanel::focusOutEvent( QFocusEvent * evt )
{
	__super::focusOutEvent( evt );

	/// 解决按下按键时窗口失去窗口,不按这个键依然有操纵的问题
	mInputDriver->Cleanup( true );
}


