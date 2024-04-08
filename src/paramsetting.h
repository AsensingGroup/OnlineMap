#pragma once

#include <QDialog>
#include "ui_paramsetting.h"

class ParamSetting : public QDialog
{
	Q_OBJECT

public:
	ParamSetting(QWidget* parent = nullptr);
	~ParamSetting();

	/// <summary>
	/// 初始化界面
	/// </summary>
	/// <param name="files"></param>
	/// <param name="lon_precise"></param>
	/// <param name="lat_precise"></param>
	void initDialog(const std::vector<QString>& files, double lon_precise, double lat_precise);

	std::vector<QString> dataFiles()const {
		return _data_files;
	}
	double lonPrecise() const {
		return _lon_precise;
	}

	double latPrecise() const {
		return _lat_precise;
	}

private:
	void selectBase();
	void select5711();
	void selectMerge();

	void saveData();

private:
	Ui::ParamSettingClass ui;
	std::vector<QString> _data_files;
	double _lon_precise = 1e-8;
	double _lat_precise = 1e-8;
};
