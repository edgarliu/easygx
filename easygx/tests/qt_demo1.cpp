/***************************************************************************
 *   Copyright (C) 2007 by lcf   *
 *   lcf@localhost   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTextCodec>
#include <QPushButton>
#include <QVBoxLayout>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	#ifdef ENABLE_QT4
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	#endif
	QWidget* pWidget = new QWidget;
	QLabel label(pWidget);
	label.setText(QObject::tr("同一个世界，同一个梦想！"));
	QPushButton* btn = new QPushButton(QObject::tr("关闭"), pWidget);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(&label);
	layout->addWidget(btn);
	pWidget->setLayout(layout);
	QObject::connect(btn, SIGNAL(clicked()), pWidget, SLOT(close()));
	pWidget->show();
	return app.exec();
}
