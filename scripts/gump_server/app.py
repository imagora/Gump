# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"

import os
import flask
import logging

import db
import login
import gump


app = flask.Flask(__name__)
app.logger.setLevel(logging.INFO)
app.config.from_object('setting.BaseConfig')
app.register_blueprint(login.bp)
app.register_blueprint(gump.bp)

try:
    os.makedirs(app.instance_path)
except OSError:
    pass

db.init_app(app)


def create_app(test_config=None):
    return app


if __name__ == '__main__':
    app.run()
