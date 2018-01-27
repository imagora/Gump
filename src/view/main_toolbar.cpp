#include "main_toolbar.h"
#include "commons/definations.h"
#include <log4cplus/log4cplus.h>
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
  QToolButton *play_btn = new QToolButton(tool_bar);
  play_btn->setIcon(QIcon(":/play.png"));
  QToolButton *pause_btn = new QToolButton(tool_bar);
  pause_btn->setIcon(QIcon(":/pause.png"));
  QToolButton *stop_btn = new QToolButton(tool_bar);
  stop_btn->setIcon(QIcon(":/stop.png"));

  tool_bar_layout->addWidget(search_label);
  tool_bar_layout->addWidget(search_edit_);
  tool_bar_layout->addWidget(search_btn);
  tool_bar_layout->addWidget(play_btn);
  tool_bar_layout->addWidget(pause_btn);
  tool_bar_layout->addWidget(stop_btn);

  tool_bar->setLayout(tool_bar_layout);
  addWidget(tool_bar);

  connect(search_edit_, SIGNAL(returnPressed()), this, SLOT(OnSearch()));
  connect(search_btn, SIGNAL(released()), this, SLOT(OnSearch()));
  connect(play_btn, SIGNAL(released()), this, SLOT(OnPlay()));
  connect(pause_btn, SIGNAL(released()), this, SLOT(OnPause()));
  connect(stop_btn, SIGNAL(released()), this, SLOT(OnStop()));
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
      LOG4CPLUS_WARN_FMT(LOGGER_NAME, "Cannot parse search string: %s",
                         str.toStdString().c_str());
    }
  }

  LOG4CPLUS_INFO_FMT(LOGGER_NAME, "Search vid: [%s] cname: [%s] stream: [%s]",
                     vid.c_str(), cname.c_str(), stream.c_str());
  emit SearchItem(vid, cname, stream);
}

void MainToolBar::OnPlay()
{
  emit PlayStream();
}

void MainToolBar::OnPause()
{
  emit PauseStream();
}

void MainToolBar::OnStop()
{
  emit StopStream();
}

