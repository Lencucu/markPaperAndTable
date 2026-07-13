#include "Paper.hpp"

void lc_setTextDocumentMargin(QTextDocument* doc, qreal value)
{	QTextFrame* frame = doc->rootFrame();
	QTextFrameFormat fmt=frame->frameFormat();
	fmt.setLeftMargin(value*1.5);
	fmt.setTopMargin(value);
	fmt.setRightMargin(value*1.07);
	fmt.setBottomMargin(value);
	frame->setFrameFormat(fmt);
}

void lc_setDocument(QTextDocument* target, QTextDocument* from)
{	target->setMarkdown(from->toMarkdown());

	QTextFrameFormat from_fmt=from->rootFrame()->frameFormat();
	QTextFrame* frame = target->rootFrame();
	QTextFrameFormat fmt=frame->frameFormat();
	fmt.setLeftMargin(from_fmt.leftMargin());
	fmt.setTopMargin(from_fmt.topMargin());
	fmt.setRightMargin(from_fmt.rightMargin());
	fmt.setBottomMargin(from_fmt.bottomMargin());
	frame->setFrameFormat(fmt);

	target->setBaseUrl(from->baseUrl());
}
