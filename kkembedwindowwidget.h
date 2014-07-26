#pragma once;

class KKEmbedWindowWidgetPrivate;

class KKEmbedWindowWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KKEmbedWindowWidget)
public:
	explicit KKEmbedWindowWidget(QGraphicsItem *parent=NULL);
	~KKEmbedWindowWidget();
	KWindow *realWindow();
	KWidget *rootWidget();

protected slots:
	void on_scenePosition(const QPointF& pt);

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void construct();
	void visibleEvent(bool bvis);
	void resetGeometry();
private:
	virtual KWidget *layoutWidget();
};