// Copyright (c) 2014-2019 winking324
//

#include "view/login_widget.h"

#include <QPushButton>
#include <QGridLayout>


namespace gump {


LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
  auto *title_label = new QLabel("Login", this);
  title_label->setStyleSheet("font-size:39px;color:gray;");
  title_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

  auto *username_label = new QLabel("Username", this);
  username_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  username_label->setStyleSheet("color:gray;margin-top:30px;");

  username_input_ = new LineEdit(QIcon(":/username.png"), QSize(32, 32), this);
  username_input_->setPlaceholderText("Type your username");

  QPushButton *login_btn = new QPushButton("LOGIN", this);
  login_btn->setStyleSheet("QPushButton{border:2px groove gray;"
                           "border-radius:16px;"
                           "margin-top:30px;height:40px;color:gray;} "
                           "QPushButton:hover {color:white;} "
                           "QPushButton:pressed{color:white;background:gray;}");

  error_label_ = new QLabel(this);
  error_label_->setStyleSheet("color:red;margin-top:40px;");

  auto *layout = new QGridLayout(this);
  layout->setAlignment(Qt::AlignVCenter);

  layout->addWidget(title_label, 0, 1, 1, 1);
  layout->addWidget(username_label, 1, 0, 1, 1);
  layout->addWidget(username_input_, 2, 0, 1, 3);
  layout->addWidget(login_btn, 3, 0, 1, 3);
  layout->addWidget(error_label_, 4, 0, 1, 3);

  setLayout(layout);

  connect(login_btn, SIGNAL(released()), this, SLOT(OnLogin()));
}

void LoginWidget::SetUsername(const QString &username) {
  username_input_->setText(username);
}

void LoginWidget::SetErrorMessage(const QString &msg) {
  error_label_->setText(msg);
}

void LoginWidget::OnLogin() {
  emit Login(username_input_->text());
}


}  // namespace gump
