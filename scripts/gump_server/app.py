# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"

import os
import flask
import logging

import db
import auth
import config


app = flask.Flask(__name__)
app.logger.setLevel(logging.INFO)
app.config.from_object('setting.BaseConfig')
app.register_blueprint(auth.bp)
app.register_blueprint(config.bp)

try:
    os.makedirs(app.instance_path)
except OSError:
    pass

db.init_app(app)


def create_app(test_config=None):
    return app


@app.before_request
def auth_request():
    app.logger.info('recv %s request', flask.request.remote_addr)
    if flask.request.remote_addr in app.config['OAUTH_PROXY']:
        return

    app.logger.warn('%s has no privilege to access this service',
                    flask.request.remote_addr)
    flask.abort(401)


if __name__ == '__main__':
    app.run()
