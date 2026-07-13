#include <ceiling/MainBoard.hpp>
#include <ceiling/ceiling/ScrollLand.hpp>
#include <ceiling/ceiling/ceiling/Card.hpp>

#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QHotkey>

#include <test/graphics.hpp>




void displayLogicWrapper(QApplication& a)
{	QSystemTrayIcon *tray = new QSystemTrayIcon(&a);// 托盘图标
		tray->setIcon(QIcon(QCoreApplication::applicationDirPath()+"/logo.png"));   // 换成你的图标
		tray->setToolTip("mpt");

		QMenu* menu=new QMenu;// 托盘菜单
			QAction *quitAct = menu->addAction("退出");
			QObject::connect(quitAct, &QAction::triggered, &a, &QApplication::quit);
		tray->setContextMenu(menu);


	MainBoard* mainboard=new MainBoard{new ScrollLand<Card,true>};
		QHotkey* hotkey=new QHotkey{QKeySequence("Ctrl+Alt+."), true, &a}; //The hotkey will be automatically registered//    qDebug() << "Is registered:" << hotkey.isRegistered();
		QObject::connect(hotkey, &QHotkey::activated, qApp, [&](){
			if (mainboard->isVisible()) {
				if(mainboard->isActiveWindow())
					mainboard->hide();
				else
					mainboard->activateWindow();
			} else {
				// mainboard->effect->setOpacity(1);
				mainboard->show();
				mainboard->activateWindow();
			}
		});
	QObject::connect(tray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason){
		if (reason == QSystemTrayIcon::Trigger) { 
			// 普通点击（单击）处理
			// qDebug() << "托盘图标被点击";
			if (mainboard->isVisible()) {
				if(!mainboard->isActiveWindow())
					mainboard->activateWindow();
			} else {
				// mainboard->effect->setOpacity(1);
				mainboard->show();
				mainboard->activateWindow();
			}
		}
		// 如果需要，也可以处理双击：
		// else if (reason == QSystemTrayIcon::DoubleClick) { ... }
	});
	mainboard->show();
	tray->show();
}




#include <windows.h>


// #include <test/graphics.hpp>
// #include <test/MyWidget.hpp>
int main(int argc, char *argv[])
{	HANDLE hMutex = CreateMutexA(NULL, TRUE, "Global\\MyUniqueAppName");
	if (GetLastError() == ERROR_ALREADY_EXISTS) return 0; // 已有实例

	QApplication a(argc, argv);
	displayLogicWrapper(a);
	// graphics();
	auto c=a.exec();

	ReleaseMutex(hMutex);
	CloseHandle(hMutex);
	return c;
}