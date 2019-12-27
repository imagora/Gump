# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import re
import json
import flask


bp = flask.Blueprint('login', __name__, url_prefix='/auth')


@bp.route('/login', methods=('GET', 'POST'))
def login():
    logger = flask.current_app.logger
    remote = flask.request.remote_addr
    username = ''
    try:
        if flask.request.method == 'GET':
            username = flask.request.args['username']
        if flask.request.method == 'POST':
            username = flask.request.json['username']
    except Exception as e:
        logger.info('[login] {} auth error: {}'.format(remote, repr(e)))
        flask.abort(401)

    if not username:
        logger.info('[login] {} user name empty'.format(remote))
        flask.abort(400)

    user_regex = flask.current_app.config['USER_REGEX']
    if user_regex != '':
        user_pattern = re.compile(user_regex)
        if not user_pattern.fullmatch(username):
            logger.info('[login] {} user {} login failed, name error'.format(remote, username))
            flask.abort(400)

    logger.info('[login] {} user {} login success'.format(remote, username))
    message = {'id': flask.current_app.config['OAUTH_ID'], 'key': flask.current_app.config['OAUTH_KEY']}
    return json.dumps(message)

