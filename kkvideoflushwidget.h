#pragma once;

class KKVideoFlushWidget : public KWidget
{
	Q_OBJECT
public:
	explicit KKVideoFlushWidget(QGraphicsItem *parent);
	~KKVideoFlushWidget();

	void updateImage(const QImage& img );
	void setDrawEnable(bool on);
	void setBlockVideo(bool on);
	QImage makeCapture();
signals:
	void contextMenu();
	void enter();
	void leave();
	void clicked();

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
private:
	QImage m_image;
	bool m_bDrawEnable;
	bool m_bBlockVideo;
};