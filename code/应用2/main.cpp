#include "Calculate_Zc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Calculate_Zc w;
	w.show();
	//a.quit();
	return a.exec();
}
