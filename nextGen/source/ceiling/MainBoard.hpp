#ifndef MAINBOARD_HPP
#define MAINBOARD_HPP


#include <QWidget>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLineEdit>
#include <functional>




// position
// size
// style
// base layout








class MainBoard:public QWidget{
	QVBoxLayout layout{this};
	QLineEdit lineEdit;
	QWidget* widget=nullptr;
public:
	const QObject* top(){return &lineEdit;}
	MainBoard(QWidget* attach=nullptr,QWidget* parent=nullptr):widget(attach),QWidget(parent)
	{	// @@@ here can't be sure w1_p or widget is valid or not
		if(!widget) widget=new QWidget;
		// QObject::connect(w1_p,,widget,);

		lineEdit.setMinimumSize(300,24);
		lineEdit.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		widget->setMinimumSize(300,90);
		widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		layout.setSpacing(layout.contentsMargins().left());
		layout.addWidget(&lineEdit);
		layout.addWidget(widget);
		// layout->setContentsMargins(0, margin, 0, margin);//left, top, right, bottom
		// layout->setSpacing(0);
		// layout->addWidget(new QWidget);
		// layout->addSpacerItem(new QSpacerItem(0,margin,QSizePolicy::Expanding,QSizePolicy::Fixed));
		// layout->addWidget(this);
		// layout->activate();
		setObjectName("main");
		lineEdit.setObjectName("sub1");
		widget->setObjectName("sub2");

		setStyleSheet(R"(
			#main{background-color: #D0E8D8;}
			#sub1{background-color: #D3F1E1;}
			#sub2{background-color: #D3F1E1;}
		)");
	}
};








#endif // MAINBOARD_HPP