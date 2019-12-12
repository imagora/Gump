# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import re
import json
import flask

bp = flask.Blueprint('auth', __name__, url_prefix='/auth')


@bp.route('/login', methods=('GET', 'POST'))
def login():
    message = {'status': 'success'}
    username = ''

    try:
        if flask.request.method == 'GET':
            username = flask.request.args['username']
        if flask.request.method == 'POST':
            username = flask.request.json['username']
    except Exception as e:
        flask.current_app.logger.info('{} auth error: {}'.format(
            flask.request.remote_addr, repr(e)))
        message['status'] = 'auth error, please input username'
        return json.dumps(message)

    if not username:
        message['status'] = 'user name empty'
        return json.dumps(message)

    user_regex = flask.current_app.config['USER_REGEX']
    if user_regex != '':
        user_pattern = re.compile(user_regex)
        if not user_pattern.fullmatch(username):
            message['status'] = 'user name not match pattern'
            return json.dumps(message)

    message['id'] = flask.current_app.config['OAUTH_ID']
    message['key'] = flask.current_app.config['OAUTH_KEY']
    return json.dumps(message)

