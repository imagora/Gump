# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import json
import flask
import base64

import auth
import stream


bp = flask.Blueprint('config', __name__, url_prefix='/gump')


@bp.before_request
def auth_request():
    logger = flask.current_app.logger
    logger.info('recv %s request', flask.request.remote_addr)
    if flask.request.remote_addr in flask.current_app.config['OAUTH_PROXY']:
        return

    logger.warn('%s has no privilege to access this service',
                flask.request.remote_addr)
    flask.abort(401)


@bp.route('/stream', methods=('GET',))
def get_stream_url():
    logger = flask.current_app.logger
    user = auth.user_info(flask.request, logger)
    if user is None:
        flask.abort(401)

    logger.info('{} request {}'.format(user, flask.request.url))

    if 'url' not in flask.request.args:
        flask.abort(400)
    url = base64.b64decode(flask.request.args['url']).decode()

    vid = 0
    if 'vid' in flask.request.args:
        vid = int(flask.request.args['vid'])
    cname = ''
    if 'cname' in flask.request.args:
        cname = flask.request.args['cname']
    host_uid = 0
    if 'host_uid' in flask.request.args:
        host_uid = int(flask.request.args['host_uid'])
    call_id = ''
    if 'call_id' in flask.request.args:
        call_id = flask.request.args['call_id']

    play_url = stream.convert_to_play_stream(logger, url)
    if play_url is None:
        return json.dumps({'status': 'failed'})

    logger.info('{} play {}'.format(user, play_url))
    message = {'status': 'success', 'stream': play_url}
    return json.dumps(message)



