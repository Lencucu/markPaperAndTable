#ifndef MAINBOARD_HPP
#define MAINBOARD_HPP


#include <QWidget>
#include <QVBoxLayout>
#include <QSizePolicy>




// position
// size
// style
// base layout








class MainBoard:public QWidget{
	QVBoxLayout layout{this};
	QWidget* w1_p=nullptr;
	QWidget* w2_p=nullptr;
public:
	MainBoard(QWidget* w1_p=new QWidget,QWidget* w2_p=new QWidget):w1_p(w1_p),w2_p(w2_p)
	{	// @@@ here can't be sure w1_p or w2_p is valid or not
		auto& w1=*w1_p;
		auto& w2=*w2_p;

		w1.setMinimumSize(200,50);
		w1.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		w2.setMinimumSize(200,100);
		w2.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		layout.setSpacing(layout.contentsMargins().left());
		layout.addWidget(&w1);
		layout.addWidget(&w2);
		// layout->setContentsMargins(0, margin, 0, margin);//left, top, right, bottom
		// layout->setSpacing(0);
		// layout->addWidget(new QWidget);
		// layout->addSpacerItem(new QSpacerItem(0,margin,QSizePolicy::Expanding,QSizePolicy::Fixed));
		// layout->addWidget(this);
		// layout->activate();
		setObjectName("main");
		w1.setObjectName("sub1");
		w2.setObjectName("sub2");

		setStyleSheet(R"(
			#main{background-color: #D0E8D8;}
			#sub1{background-color: #D3F1E1;}
			#sub2{background-color: #D3F1E1;}
		)");
	}
};








#endif // MAINBOARD_HPP