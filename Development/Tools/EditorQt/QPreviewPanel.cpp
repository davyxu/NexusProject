#include "EditorPCH.h"
#include "PrivateInclude.h"


QPreviewPanel::QPreviewPanel( QWidget* Parent /*= NULL */ )
: QRenderPanel( Parent )
{
	setWindowTitle(QString("Preview"));

}

QPreviewPanel::~QPreviewPanel()
{

}

