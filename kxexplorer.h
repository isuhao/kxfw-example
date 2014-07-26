#pragma once;

class KTextBubbleTip;

class KxExplorer : public QObject
{
	Q_OBJECT
public:
	explicit KxExplorer(QObject *parent = NULL);
	~KxExplorer();
public:
	static KxExplorer *instance();

public:
	Q_INVOKABLE void execute();

private slots:
	void on_btnFace_clicked();
	void on_facePage_visibleChanged();
	void initFaceTable(KTableWidget *table, QSizeF cellSize, const QString& gifpath, const QString& gifkey);
	void on_facePage_item_clicked();
	void on_colorWord_clicked();
	void on_captionArea_mouseEvent( QEvent *e );

private:
	bool eventFilter(QObject *w, QEvent *e);

private:
	QPointer<KPopupWindow> m_pMainWindow;
	QPointer<KTextBubbleTip> m_tipRegister;
	QPointer<KWidget> m_pRoom;
	QPointer<KWidget> m_pRoomInput;
	QPointer<KWidget> m_pFacePage;
	QPointer<KWidget> m_pFaceFrame;
	QPointer<QWidget> m_pViewport;

	QPointer<KPushButton> m_pBtnColorWord;

	QPointer<KTableWidget> m_pFace[4];
};