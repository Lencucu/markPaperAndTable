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
	// textEdit->setHtml("<h1>Hello</h1><p>This is a test.</p>");
	textEdit->setMarkdown(R"(Qt 有明确的顺序：

**后安装的先执行（LIFO，后进先出）。**

例如：

```cpp
label->installEventFilter(card);
label->installEventFilter(scrollLand);
```

收到事件时调用顺序是：

```text
ScrollLand::eventFilter()
↓
Card::eventFilter()
↓
Label::event()
```

如果 `ScrollLand::eventFilter()` 返回 `true`：

```text
ScrollLand::eventFilter()
↓
结束
```

后面的 `Card` 和 `Label` 都不会收到。

---

如果再安装一个：

```cpp
label->installEventFilter(A);
label->installEventFilter(B);
label->installEventFilter(C);
```

顺序就是：

```text
C
↓
B
↓
A
↓
Label
```

所以可以把它理解成一个**栈**，每次 `installEventFilter()` 都压栈，事件发生时从栈顶开始调用。
)");

	auto& scene=*(new QGraphicsScene);
	auto& view=*(new QGraphicsView{&scene});
	QGraphicsProxyWidget *proxy = scene.addWidget(textEdit);

	view.show();
	// 缩放整个视图（光学变焦！）
	// view.setTransform(QTransform().scale(1.5, 1.5));  // 放大 1.5 倍
	static qreal factor=1;
	QObject::connect(timer,&QTimer::timeout,timer,[&]
		{	factor*=1-0.001;
			view.setTransform(QTransform().scale(factor, factor));  // 缩小到 0.8 倍
		}
	);
	timer->start();
}