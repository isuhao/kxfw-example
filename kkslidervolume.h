#pragma once;


class KKSliderVolume : public KFrameItem
{
	Q_OBJECT
	Q_PROPERTY(QSize popupSize READ popupSize WRITE setPopupSize)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(QObject* pframe READ popupFrameObject)
	Q_PROPERTY(QObject* status READ statusObject)
	Q_PROPERTY(QObject* slider READ sliderObject)
	Q_PROPERTY(QObject* sframe READ sliderFrameObject)

public:
	explicit KKSliderVolume(QGraphicsItem *parent = NULL);
	~KKSliderVolume();

	QSize popupSize() const;
	void setPopupSize(const QSize& s);

	QString imagePath() const;
	void setImagePath(const QString& s);

	QObject *popupFrameObject() const;
	QObject *statusObject() const;
	QObject *sliderObject() const;
	QObject *sliderFrameObject() const;

	void setVolume(float val, bool mute);
	bool mute();

signals:
	void valueChanged(qreal val);
	void muteChanged(bool mute);

protected:
	void timerEvent(QTimerEvent * event);
	void resizeEvent(QGraphicsSceneResizeEvent * event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

private slots:
	void on_mute_clicked();

private:
	void updateLayout();

private:
	QPointer<KBubbleTip> m_pWindow;
	QPointer<KFrameItem> m_sliderPannel;
	QPointer<KFrameItem> m_mute;
	QPointer<KSlider> m_slider;
	QPointer<KSpacer> m_sliderFrame;
	QSize m_popupSize;
	int m_timerid;
};