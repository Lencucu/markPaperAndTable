#ifndef MAINBOARD_HPP
#define MAINBOARD_HPP


#include <QWidget>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLineEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <functional>




// position
// size
// style
// base layout


// // 当lineEdit内容过长即压缩，临近三格时显示报警条，最后一击变红且不增加内容，再输入炸条
// QFont font = lineEdit.font();
// // 设置拉伸因子，默认是 100
// // 小于 100 表示压缩（压扁），大于 100 表示拉伸
// font.setStretch(60);  // 60% 的宽度，产生明显的压扁效果
// lineEdit.setFont(font);








class MainBoard:public QWidget{
	QVBoxLayout layout{this};
	QLineEdit lineEdit;
	QWidget* widget=nullptr;


public:
	const QObject* top(){return &lineEdit;}
	MainBoard(QWidget* attach=nullptr,QWidget* parent=nullptr,Qt::WindowFlags f=Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint):widget(attach),QWidget(parent,f)
	{	if(!widget) widget=new QWidget;

		lineEdit.setMinimumSize(300,24);
		lineEdit.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		widget->setMinimumSize(300,90);
		widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		layout.setSpacing(layout.contentsMargins().left());
		layout.addWidget(&lineEdit);
		layout.addWidget(widget);
		setObjectName("main");
		lineEdit.setObjectName("sub1");
		widget->setObjectName("sub2");

		setStyleSheet(R"(
			#main{background-color: #D0E8D8;}
			#sub1{background-color: #D3F1E1;}
			#sub2{background-color: #000000;}
		)");
	}


protected:
	void paintEvent(QPaintEvent *event)
	{	QPainter painter(this);

		// 1. 设置画笔（决定边框样式）
		int border_size=2;
		QPen pen(Qt::gray);          // 边框颜色
		pen.setWidth(border_size*2);            // 边框宽度
		painter.setPen(pen);

		// 2. 设置画刷为不填充（关键步骤）
		painter.setBrush(Qt::NoBrush);  // 或者 painter.setBrush(Qt::transparent);

		// 3. 绘制矩形
		painter.drawRect(0, 0, width(), height());
	}
};








#endif // MAINBOARD_HPP