#include "main_toolbar.h"
#include <iostream>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QSize>


using namespace gump;


MainToolBar::MainToolBar(QWidget *parent/* = nullptr*/)
  : QToolBar(parent)
{
  setMovable(false);

  QWidget *tool_bar = new QWidget(this);
  tool_bar->setToolTip("vid:xxx cname:xxx stream:xxx");
  QHBoxLayout *tool_bar_layout = new QHBoxLayout(tool_bar);

  QLabel *search_label = new QLabel("Search:", tool_bar);
  search_edit_ = new QLineEdit(tool_bar);
  QToolButton *search_btn = new QToolButton(tool_bar);
  search_btn->setIcon(QIcon(":/search.png"));

  tool_bar_layout->addWidget(search_label);
  tool_bar_layout->addWidget(search_edit_);
  tool_bar_layout->addWidget(search_btn);

  tool_bar->setLayout(tool_bar_layout);
  addWidget(tool_bar);

  connect(search_edit_, SIGNAL(returnPressed()), this, SLOT(OnSearch()));
  connect(search_btn, SIGNAL(released()), this, SLOT(OnSearch()));
}

MainToolBar::~MainToolBar()
{

}

void MainToolBar::OnSearch()
{
  QStringList search_strs = search_edit_->text().split(" ");
  if (search_strs.empty()) return;

  std::string vid;
  std::string cname;
  std::string stream;
  foreach (const QString &str, search_strs) {
    if (str.startsWith("vid:", Qt::CaseInsensitive)) {
      vid = str.section(":", 1).toStdString();
    } else if (str.startsWith("cname:", Qt::CaseInsensitive)) {
      cname = str.section(":", 1).toStdString();
    } else if (str.startsWith("stream:", Qt::CaseInsensitive)) {
      stream = str.section(":", 1).toStdString();
    } else {
      std::cout << "cannot parse search string: " << str.toStdString() << std::endl;
    }
  }

  std::cout << "Search vid:" << vid << " cname:"
            << cname << " stream:" << stream << std::endl;
  emit SearchItem(vid, cname, stream);
}
