#pragma once

#include <QWebEngineView>

class OnLineMapWidget : public QWebEngineView
{
	Q_OBJECT

public:

	enum Type {
		Default = 0,
		GaoDe = 1,
		Bing = 2
	};

	explicit OnLineMapWidget(QWidget* parent = nullptr);
	~OnLineMapWidget();

	/// <summary>
	/// ���ü��ص���ҳ�ļ�
	/// </summary>
	/// <param name="url_file"></param>
	/// <returns></returns>
	bool setUrlFile(const QString& url_file);

	/// <summary>
	/// INS�켣ˢ��
	/// </summary>
	/// <param name="lon">����</param>
	/// <param name="lat">γ��</param>
	/// <param name="time">ʱ��</param>
	/// <param name="height">�߶�</param>
	/// <param name="roll">�����</param>
	/// <param name="pitch">������</param>
	/// <param name="yaw">�����</param>
	/// <param name="v">�ٶ�</param>
	/// <param name="follow_ins">�Ƿ�ins����</param>
	void renderTraceIns(double lon, double lat, uint64_t time, double height
		, double roll, double pitch, double yaw, double v, bool follow_ins);

	/// <summary>
	/// GPS�켣ˢ��
	/// </summary>
	/// <param name="lon">����</param>
	/// <param name="lat">γ��</param>
	/// <param name="time">ʱ��</param>
	/// <param name="height">�߶�</param>
	/// <param name="roll">�����</param>
	/// <param name="pitch">������</param>
	/// <param name="yaw">�����</param>
	/// <param name="v">�ٶ�</param>
	void renderTraceGps(double lon, double lat, uint64_t time, double height
		, double roll, double pitch, double yaw, double v);

	void renderTraceGps(double lon, double lat);

	/// <summary>
	/// ���GPS��·��
	/// </summary>
	void clearGpsPath();

	/// <summary>
	/// ���INS��·��
	/// </summary>
	void clearInsPath();

	/// <summary>
	/// ���
	/// </summary>
	/// <param name="type">1:��ʼ���i��2:�رղ��</param>
	void measureDistance(int type);

	/// <summary>
	/// ����INS����
	/// </summary>
	/// <param name="is_visible"></param>
	void setInsVisible(bool is_visible);

	/// <summary>
	/// ����GPS����
	/// </summary>
	/// <param name="is_visible"></param>
	void setGpsVisible(bool is_visible);

	/// <summary>
	/// �ص�ָ��λ��
	/// </summary>
	/// <param name="lon"></param>
	/// <param name="lat"></param>
	void locate(double lon, double lat);

	/// <summary>
	/// ���õ�ͼ������ʽ
	/// </summary>
	/// <param name="name"></param>
	void setMapMode(QString name);

	/// <summary>
	/// ���õ�ͼ����
	/// </summary>
	/// <param name="lang"></param>
	//void setMapLang(QString lang);

	/// <summary>
	/// ִ��javascript�ű�
	/// </summary>
	/// <param name="script"></param>
	void runJavaScript(const QString& script);
};