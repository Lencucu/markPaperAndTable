/*
卡片排版策略：
	所有卡片的执政通过set方式记录，从set中按序排列
卡片交互策略：
	1>鼠标在ScrollArea上滚动时滚动卡片列表
	·1·鼠标静止状态悬浮一定时间如果鼠标下方落在非空白区域则对应子控件获得焦点；也可以直接通过左键/中键点击获取焦点
	当在焦点控件区域外部滚动鼠标时效果同1>
	焦点控件在一定时间内没有鼠标进入/主动转移焦点会触发失焦动画并失焦
效果：
	·1·鼠标悬浮时有一种蓄力的感觉，临近焦点派发时效果突然强烈，当获取焦点后对应子控件获取聚焦效果，之前的焦点控件在此之前失焦
*/


#include <QScrollArea>
#include <QEvent>
#include <QHBoxLayout>
#include <QTimer>
#include <QWheelEvent>// can pack with eventFilter
#include <QPropertyAnimation>// can pack with eventFilter
#include <QEasingCurve>// can pack with eventFilter
#include <QAbstractAnimation>// can pack with eventFilter
#include <QScrollBar>// can pack with eventFilter
#include <QFile>// can pack with addContents
#include <QTextStream>// can pack with addContents
#include <ceiling/ceiling/ceiling/Card.hpp>// can pack with addContents
#include <QResizeEvent>// can pack with resizeEvent
#include <QCoreApplication>// can pack with constructor
#include <QMouseEvent>// can pack with eventFilter

template<class T_content, bool demo=false>
class ScrollLand:public QScrollArea{// ScrollArea->viewport->container
	QWidget contentContainer;
	QHBoxLayout layout{&contentContainer};
	qreal contentRatio=1.3333;

	T_content* focuMark=nullptr;
	bool focuPointOnSub=false;
	// 要设置一个主动跳出的逻辑

public:
	ScrollLand(QWidget* parent=nullptr):QScrollArea(parent)
	{	installEventFilter(this);
		viewport()->installEventFilter(this);
		contentContainer.installEventFilter(this);

		setWidget(&contentContainer);
		setWidgetResizable(true);// 不知道怎么回事，好像不加这一条或者说是false的情况时视口不会追踪widget，即没内容，而且只会在widget resize时才会更新视口一样，所以默认还是加上，不知道不加和加到底有什么区别
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 关闭滚动条
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		layout.setAlignment(Qt::AlignLeft);
		layout.setContentsMargins(0,0,0,0);
		layout.setSpacing(10);

		if(demo)
		{	addContent(QCoreApplication::applicationDirPath()+"/notes/design.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/story.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/timetest.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/installEventFilter.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/design.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/story.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/timetest.md");
			addContent(QCoreApplication::applicationDirPath()+"/notes/installEventFilter.md");
		}

		resize(400,150);
	}

	void addContent(const QString& filename)
	{	QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

		Card* card = new Card{qreal(1/2.0),this};
		card->installEventFilter(this);
		card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		layout.addWidget(card);

		QTextStream in(&file);
		card->setText(in.readAll());
		file.close();
	}


protected:
    void resizeEvent(QResizeEvent *e) override
	{	contentContainer.setFixedWidth(qRound(e->size().height()*layout.count()*contentRatio));
		QScrollArea::resizeEvent(e);
	}
	bool eventFilter(QObject* obj, QEvent* ev) override
	{	if(ev->type()==QEvent::Wheel)
		{	if(!focuPointOnSub)// 当滚动且焦点没在子控件上时
			{	// 获取滚轮滚动的角度
				int delta = static_cast<QWheelEvent*>(ev)->angleDelta().y()*2/3; // 默认纵向滚动值
				// 用纵向滚轮值去滚动水平滚动条
				QPropertyAnimation* anim = new QPropertyAnimation(horizontalScrollBar(), "value");

				anim->setDuration(300);
				anim->setStartValue(horizontalScrollBar()->value());
				anim->setEndValue(horizontalScrollBar()->value() - delta);   // 目标滚动位置
				anim->setEasingCurve(QEasingCurve::OutCubic);
				anim->start(QAbstractAnimation::DeleteWhenStopped);
				return true;
			}
		}
		// 无焦点只有中键press后获取focu，其他都需要release才行
		// 退焦的话左右键都只需要press，中键的话由于有导航逻辑需要release
		if(ev->type()==QEvent::MouseButtonRelease && obj!=this)// 注册过的子控件获得焦点
		{	if(focuMark=qobject_cast<T_content*>(obj))
			{	if(static_cast<QMouseEvent*>(ev)->button()!=Qt::MiddleButton && focuPointOnSub)
				{	focuMark=nullptr;
					focuPointOnSub=false;
					return true;
				}
				focuPointOnSub=true;
				return true;
			}
		}
		if(ev->type()==QEvent::Leave && obj==focuMark && focuPointOnSub)// 注册过的子控件丢失焦点
		{	focuMark=nullptr;
			focuPointOnSub=false;
			return true;
		}
		return QScrollArea::eventFilter(obj,ev);
	}
};