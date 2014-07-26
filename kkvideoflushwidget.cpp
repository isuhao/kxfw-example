#include "stdafx.h"
#include "kkvideoflushwidget.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KKVideoFlushWidget)

KKVideoFlushWidget::KKVideoFlushWidget( QGraphicsItem *parent )
: KWidget(parent)
, m_bDrawEnable(false)
, m_bBlockVideo(false)
{
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
}

KKVideoFlushWidget::~KKVideoFlushWidget()
{

}

void KKVideoFlushWidget::updateImage( const QImage& img )
{
	if(m_bBlockVideo)
		return;
	m_image = img;
	update();
}

void KKVideoFlushWidget::setDrawEnable( bool on )
{
	if(m_bDrawEnable == on)
		return;
	m_bDrawEnable = on;
	if(!on)
	{
		m_image = QImage();
		update();
	}
}

void KKVideoFlushWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	if(m_bDrawEnable)
	{
		QPainter::RenderHints rhs = painter->renderHints();
		QRectF drawRt = rect();
		painter->drawImage(drawRt, m_image, drawRt);
		painter->setRenderHints(rhs);
	}
}

void KKVideoFlushWidget::contextMenuEvent( QGraphicsSceneContextMenuEvent *event )
{
	emit contextMenu();
}

void KKVideoFlushWidget::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{
	emit enter();
}

void KKVideoFlushWidget::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{
	emit leave();
}

QImage KKVideoFlushWidget::makeCapture()
{
	return m_image;
}

void KKVideoFlushWidget::setBlockVideo( bool on )
{
	m_bBlockVideo = on;
	if(on)
	{
		m_image = QImage();
		update();
	}
}

void KKVideoFlushWidget::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	emit clicked();
}
