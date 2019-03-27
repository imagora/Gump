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
  auto *password_label = new QLabel("Password", this);
  username_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  password_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
  username_label->setStyleSheet("color:gray;margin-top:30px;");
  password_label->setStyleSheet("color:gray;margin-top:20px;");

  username_input_ = new LineEdit(QIcon(":/username.png"), QSize(32, 32), this);
  username_input_->setPlaceholderText("Type your username");

  password_input_ = new LineEdit(QIcon(":/password.png"), QSize(32, 32), this);
  password_input_->setPlaceholderText("Type your password");
  password_input_->setEchoMode(QLineEdit::PasswordEchoOnEdit);

  QPushButton *login_btn = new QPushButton("LOGIN", this);
  login_btn->setStyleSheet("QPushButton{border:2px groove gray;"
                           "border-radius:16px;"
                           "margin-top:30px;height:40px;color:gray;} "
                           "QPushButton:hover {color:white;} "
                           "QPushButton:pressed{color:white;background:gray;}");

  error_label_ = new QLabel("Error: timeout", this);
  error_label_->setStyleSheet("color:red;margin-top:40px;");

  auto *layout = new QGridLayout(this);
  layout->setAlignment(Qt::AlignVCenter);

  layout->addWidget(title_label, 0, 1, 1, 1);
  layout->addWidget(username_label, 1, 0, 1, 1);
  layout->addWidget(username_input_, 2, 0, 1, 3);
  layout->addWidget(password_label, 3, 0, 1, 1);
  layout->addWidget(password_input_, 4, 0, 1, 3);
  layout->addWidget(login_btn, 5, 0, 1, 3);
  layout->addWidget(error_label_, 6, 0, 1, 3);

  setLayout(layout);

  connect(login_btn, SIGNAL(released()), this, SLOT(OnLogin()));
}

void LoginWidget::SetErrorMessage(const QString &msg) {
  error_label_->setText(msg);
}

void LoginWidget::OnLogin() {
  emit Login(username_input_->text(), password_input_->text());
}


}  // namespace gump
