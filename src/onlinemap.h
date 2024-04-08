#pragma once

#include <QtWidgets/QDialog>
#include "ui_onlinemap.h"

class OnlineMap : public QDialog
{
	Q_OBJECT

public:
	OnlineMap(QWidget* parent = nullptr);
	~OnlineMap();

private:
	void settings();
	void loadData();
	void clearData();
	void loadHtmlFinined();
	void dataConvert();

	void showBase(int is_visible);
	void show5711(int is_visible);
	void showOther(int is_visible);

	void saveConfig();
	void readConfig();

private:
	Ui::OnlineMapClass ui;

	QString _map_file;
	std::vector<QString> _render_functions;
	std::vector<QString> _clear_functions;
	std::vector<QString> _visible_functions;
	std::vector<QString> _data_files;
	double _lon_precise = 1e-8;
	double _lat_precise = 1e-8;
};
