``` c++
		layout->setContentsMargins(0, margin, 0, margin);//left, top, right, bottom
		layout->setSpacing(0);
		layout->addWidget(new QWidget);
		layout->addSpacerItem(new QSpacerItem(0,margin,QSizePolicy::Expanding,QSizePolicy::Fixed));
		layout->addWidget(this);
		layout->activate();
```