#ifndef CARD_HPP
#define CARD_HPP


#include <QDebug>

#include <QSize>
#include <QRect>
#include <QColor>
#include <QPointF>
#include <QGraphicsScene>

#include <QWidget>
#include <QTextBrowser>
#include <QTextDocument>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include "ceiling/Paper.hpp"

#include <QEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>

#include <QPainter>
#include <QHBoxLayout>
// #include <tools/physical2pixel/physical2pixel.hpp>
#include <functional>







class Card : public QWidget{
	Q_OBJECT
	QRect rect_bg;
	QGraphicsScene paper_view_scene{this};

private:
	qreal shrinkFactor;// also align to textBrowser, this will effect Card show content range
	QPointF contentAnchor;// pre中心对应的点 【!default set to top left!】
	bool middleButtonClicked=false;

private:
	QHBoxLayout layout{this};
	QWidget mask{this};
	Paper paper{SET_AUTO_HIDE_ENABLE};
	QGraphicsView paperView{&paper_view_scene,this};
	QGraphicsProxyWidget* paperProxy=nullptr;

	void paperAboutResize()
	{	paperView.setTransform(QTransform{}.scale(shrinkFactor,shrinkFactor));
		paperView.resize(paper.realSize()*shrinkFactor);
	}

	void paperAboutMoveAlign()
	{	// 同步到paper
		QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
		paperView.move(
			qRound(center_point.x()-paperView.size().toSizeF().width()*contentAnchor.x()),
			qRound(center_point.y()-paperView.size().toSizeF().height()*contentAnchor.y())
		);
	}

public:
	Card(qreal shrink_factor=1/1.2,QWidget* parent=nullptr):shrinkFactor(shrink_factor),QWidget(parent/*,Qt::FramelessWindowHint*/)
	{   // 1.UI set
		installEventFilter(this);
		mask.installEventFilter(this);
		mask.raise();// 置顶
		layout.setContentsMargins(0,0,0,0);// 填满
		setLayout(&layout);
		layout.addWidget(&mask);
		mask.setStyleSheet("background-color:rgba(0,0,0,0%);");
		paperView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		paperView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		// 2.paper install
		paperProxy=paper_view_scene.addWidget(&paper);//////////////////////
		paper.undetach();// 这里主要作用在自动隐藏按钮 // scene.addWidget不会调用setParent所以只能手动触发
		paper.installEventFilter(this);
		paper.signFlatDragBtFunc([this](){paperProxy->setWidget(&paper);});
		paperView.installEventFilter(this);
		paperAboutResize();// apply shrink factor

		// 3.init by resize after install
		resize(400,400);// size默认无效值(-1,-1)
	};
	~Card(){};

	void setText(const QString& line)
	{	paper.setText(line);
	}

	void installEventFilter(QObject* filterObj)
	{	QObject::installEventFilter(filterObj);
		mask.installEventFilter(filterObj);
	}

	template<bool selfcall=false>
	void fresh_content()// 将内容对齐到左上角
	{	if(!selfcall) middleButtonClicked=false;
		contentAnchor.setX(mask.size().toSizeF().width()/2/paperView.size().width());
		contentAnchor.setY(mask.size().toSizeF().height()/2/paperView.size().height());
	}


protected:
	bool hasHeightForWidth() const override
	{	return true;
	}
	int heightForWidth(int w) const override
	{	return int(w*0.8);
	}
	void resizeEvent(QResizeEvent *e) override// won't change paper size
	{	paperAboutMoveAlign();
	};
	bool eventFilter(QObject *obj, QEvent *ev) override
	{	if(ev->type()==QEvent::Resize)// draw paper
		{	if(!middleButtonClicked) fresh_content<true>();
			else if(obj==&paperView && !paper.detached())// paper作为子控件缩小
			{	auto&& new_size_f=static_cast<QResizeEvent*>(ev)->size().toSizeF();
				auto&& old_size_f=static_cast<QResizeEvent*>(ev)->oldSize().toSizeF();

				QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
				// pos=center-size*anchor
				QPointF pos{
					center_point.x()-old_size_f.width()*contentAnchor.x(),
					center_point.y()-old_size_f.height()*contentAnchor.y()
				};
				auto ratio=new_size_f.width()/
					old_size_f.width();
				// (newpos-center)/(pos-center)=ratio
				auto newpos=((pos-center_point)*ratio+center_point).toPoint();
				paperView.move(newpos);
			}
		}
		if(obj==&mask && ev->type()==QEvent::Paint)// draw mask
		{	QPainter qp{&mask};

			// 绘制参数预算
			bool tinyPaper_width_base_on_mask=false;// 画布缩放是否基于mask的宽
			qreal paper_wh_ratio=paper.realSize().toSizeF().width()/paper.realSize().toSizeF().height();
			if(paper_wh_ratio>(qreal(mask.width())/mask.height())) tinyPaper_width_base_on_mask=true;// paper宽高比大于mask宽高比
			QColor color_bg{120,120,120,30};
			QColor color_fill{180,200,255,50};

			// 绘制
			if(tinyPaper_width_base_on_mask)
			{	// 纸张缩略图
				QSizeF tinypaper_wh{
					qreal(mask.width()),
					qreal(mask.width())/paper_wh_ratio
				};
				rect_bg.setRect(
					0,
					qRound((mask.height()-tinypaper_wh.height())/2),
					qRound(tinypaper_wh.width()),
					qRound(tinypaper_wh.height())
				);
				qp.fillRect(rect_bg,color_bg);
				// 预览框轮廓示意
				// size: pre/tinypaper=mask/paper=>pre=mask*(tinypaper/paper)
				QSizeF pre_wh{
					mask.width()*tinypaper_wh.height()/paperView.height(),
					mask.height()*tinypaper_wh.height()/paperView.height()
				};
				QRect rect{
					qRound(contentAnchor.x()*tinypaper_wh.width()-pre_wh.width()/2),
					qRound(contentAnchor.y()*tinypaper_wh.height()-pre_wh.height()/2+(mask.height()-tinypaper_wh.height())/2),
					qRound(pre_wh.width()),
					qRound(pre_wh.height())
				};
				qp.fillRect(rect,color_fill);
			}
			else
			{	// 纸张缩略图
				QSizeF tinypaper_wh{
					qreal(mask.height())*paper_wh_ratio,
					qreal(mask.height())
				};
				rect_bg.setRect(
					qRound((mask.width()-tinypaper_wh.width())/2),
					0,
					qRound(tinypaper_wh.width()),
					qRound(tinypaper_wh.height())
				);
				qp.fillRect(rect_bg,color_bg);
				// 预览框轮廓示意
				// size: pre/tinypaper=mask/paper=>pre=mask*(tinypaper/paper)
				QSizeF pre_wh{
					mask.width()*tinypaper_wh.width()/paperView.width(),
					mask.height()*tinypaper_wh.width()/paperView.width()
				};
				QRect rect{
					qRound(contentAnchor.x()*tinypaper_wh.width()-pre_wh.width()/2+(mask.width()-tinypaper_wh.width())/2),
					qRound(contentAnchor.y()*tinypaper_wh.height()-pre_wh.height()/2),
					qRound(pre_wh.width()),
					qRound(pre_wh.height())
				};
				qp.fillRect(rect,color_fill);
			}
			return true;
		}
		if(obj==&mask && !paper.detached() && (ev->type()==QEvent::MouseButtonPress || ev->type()==QEvent::MouseMove))// show-pos control
		{	if(static_cast<QMouseEvent*>(ev)->buttons()&Qt::MiddleButton)
			{	if(!middleButtonClicked) middleButtonClicked=true;
				contentAnchor.rx()=(static_cast<QMouseEvent*>(ev)->position().x()-rect_bg.x())/rect_bg.width();
				contentAnchor.ry()=(static_cast<QMouseEvent*>(ev)->position().y()-rect_bg.y())/rect_bg.height();
				// 同步到paper
				paperAboutMoveAlign();
				mask.update();
				return true;
			}
		}
		if(obj==&mask && !paper.detached() && ev->type()==QEvent::Wheel)// zoom paper
		{	if (!static_cast<QWheelEvent*>(ev)->pixelDelta().isNull())
				shrinkFactor*=(qreal(static_cast<QWheelEvent*>(ev)->pixelDelta().y())/1000+1);
			else
				shrinkFactor*=(qreal(static_cast<QWheelEvent*>(ev)->angleDelta().y())/1000+1);

			paperAboutResize();

			mask.update();
			return true;
		}
		if(ev->type()==QEvent::MouseButtonDblClick && static_cast<QMouseEvent*>(ev)->button()==Qt::LeftButton)
		{	if(obj==&mask)
			{	paperProxy->setWidget(nullptr);         // 解除代理和控件的绑定
				paper.detach();
				return true;
			}
		}
		return QWidget::eventFilter(obj,ev);
	}
};








#endif // CARD_HPP