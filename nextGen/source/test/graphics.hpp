#include <QGraphicsScene>// obj
#include <QGraphicsView>// widget
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QTimer>



// view fetch data from scene, scene form data from widget or item
void graphics()
{	QTimer* timer=new QTimer;
	timer->setInterval(30);

	QTextEdit *textEdit = new QTextEdit;
	textEdit->setHtml("<h1>Hello</h1><p>This is a test.</p>");

	auto& scene=*(new QGraphicsScene);
	auto& view=*(new QGraphicsView{&scene});
	QGraphicsProxyWidget *proxy = scene.addWidget(textEdit);

	view.show();
	// 缩放整个视图（光学变焦！）
	// view.setTransform(QTransform().scale(1.5, 1.5));  // 放大 1.5 倍
	static qreal factor=1;
	QObject::connect(timer,&QTimer::timeout,timer,[&]
		{	factor*=1.001;
			view.setTransform(QTransform().scale(factor, factor));  // 缩小到 0.8 倍
		}
	);
	timer->start();
}