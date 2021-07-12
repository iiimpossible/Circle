#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <qpushbutton.h>
#include <QHBoxLayout>
#include <qwidget.h>

#include "FileSystemWidget.h"
FileSystemWidget::FileSystemWidget(QWidget* widget):
	QWidget(widget)
{
	QFileSystemModel* model = new QFileSystemModel;
	model->setRootPath(QDir::currentPath());

	QTreeView* treeView = new QTreeView(this);
	treeView->setModel(model);
	treeView->setRootIndex(model->index(QDir::currentPath()));

	QPushButton *mkdirButton = new QPushButton(tr("Make Directory..."), this);
	QPushButton *rmButton = new QPushButton(tr("Remove"), this);
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(mkdirButton);
	buttonLayout->addWidget(rmButton);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(treeView);
	layout->addLayout(buttonLayout);

	setLayout(layout);
	setWindowTitle("File System Model");

	connect(mkdirButton, SIGNAL(clicked()),
		this, SLOT(mkdir()));
	connect(rmButton, SIGNAL(clicked()),
		this, SLOT(rm()));
}