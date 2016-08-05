#include <QSysInfo>

#include "helpdialog.h"
#include "ui_about.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    ui->helpTextBrowser->append("<b><center>Rana 1.7 (revision 17:0.8.2:JIT)</b> </center>");
    QString system = QSysInfo::kernelType();

	if(QT_VERSION_MINOR > 4 && QT_VERSION_MAJOR >= 5)
	{
		system.append(" : ");
		system.append(QSysInfo::buildCpuArchitecture());
	}

    ui->helpTextBrowser->append(system);
    ui->helpTextBrowser->append(" ");

    QString depend ="<p></p>Based on <a href=\"http://en.cppreference.com/w/cpp\"><span style=\" text-decoration: underline; color:#0000ff;\">C++ 11</span></a>, <a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#0000ff;\">QT 5</span></a> and <a href=\"http://luajit.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">luaJIT 2.0.4</span></a>.</p>";
    QString source = "Source: <a href=\"https://github.com/sojoe02/RANA\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/sojoe02/RANA</span></a>";
    QString releases = "<div align=\"left\">Releases:<a href=\"https://github.com/sojoe02/RANA/releases\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/sojoe02/RANA/releases</span></a></div>";
    QString documentation = "<div align=\"lef\">Documentation <a href=\"https://github.com/sojoe02/RANA/wiki\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/sojoe02/RANA/wiki</span></a></div>";
    QString copyright = "Copyright 2013-2016 Søren V. Jørgensen (sojoe02@gmail.com). All rights reservet.";
    QString license = "License: <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">GPLv3</a>";

    ui->helpTextBrowser->append(depend);
    ui->helpTextBrowser->append(" ");
    ui->helpTextBrowser->setAlignment(Qt::AlignLeft);
    ui->helpTextBrowser->append(source);
    ui->helpTextBrowser->append(releases);
    ui->helpTextBrowser->append(documentation);
    ui->helpTextBrowser->append(" ");
    ui->helpTextBrowser->append(copyright);
    ui->helpTextBrowser->append(license);

}
HelpDialog::~HelpDialog()
{
    delete ui;
}
