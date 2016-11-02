#include "EditorPCH.h"
#include "PrivateInclude.h"


int main(int argc, char *argv[])
{
	NEKO_CORE_INIT
	NEKO_ENGINE_INIT

	QApplication a(argc, argv);

	QDir::setCurrent( QApplication::applicationDirPath() );

	QEditorFrame frame;

	frame.Initialize();
	
	int ret = a.exec();

	frame.Finalize( );

	return ret;
}
