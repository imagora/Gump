# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import json
import flask
import base64

import auth
import search


bp = flask.Blueprint('gump', __name__, url_prefix='/gump')


@bp.before_request
def auth_request():
    logger = flask.current_app.logger
    logger.info('recv %s request', flask.request.remote_addr)
    if flask.request.remote_addr in flask.current_app.config['OAUTH_PROXY']:
        return

    logger.warn('%s has no privilege to access this service',
                flask.request.remote_addr)
    flask.abort(401)


@bp.route('/ping', methods=('GET',))
def on_ping():
    logger = flask.current_app.logger
    user = auth.user_info(flask.session, flask.request, logger)
    if user is None:
        flask.abort(401)

    logger.info('{} ping'.format(user))
    return json.dumps({'status': 'success'})


@bp.route('/logout', methods=('GET',))
def on_logout():
    logger = flask.current_app.logger
    user = auth.user_info(flask.session, flask.request, logger)
    if user is None:
        flask.abort(401)

    logger.info('{} logout'.format(user))
    flask.session.clear()
    return json.dumps({'status': 'success'})


@bp.route('/stream', methods=('GET',))
def on_stream():
    logger = flask.current_app.logger
    user = auth.user_info(flask.session, flask.request, logger)
    if user is None:
        flask.abort(401)

    logger.info('{} request {}'.format(user, flask.request.url))

    if 'url' not in flask.request.args:
        flask.abort(400)

    url = base64.b64decode(flask.request.args['url']).decode()
    stream_info = search.search_by_url(logger, url)
    if stream_info is None:
        return json.dumps({'status': 'failed'})

    if 'vid' in flask.request.args:
        stream_info['vendor'] = flask.request.args['vid']
    if 'cname' in flask.request.args:
        stream_info['cname'] = base64.b64decode(flask.request.args['cname']).decode()
    if 'host_uid' in flask.request.args:
        stream_info['uid'] = flask.request.args['host_uid']
    if 'call_id' in flask.request.args:
        stream_info['call_id'] = flask.request.args['call_id']

    message = {'status': 'success', 'stream': stream_info}
    return json.dumps(message)


@bp.route('/search', methods=('GET',))
def on_search():
    logger = flask.current_app.logger
    user = auth.user_info(flask.session, flask.request, logger)
    if user is None:
        flask.abort(401)

    logger.info('{} request {}'.format(user, flask.request.url))

    stream_info = None
    message = {'status': 'failed'}
    if 'vendor' not in flask.request.args:
        if 'url' in flask.request.args:
            url = base64.b64decode(flask.request.args['url']).decode()
            stream_info = search.search_by_url(logger, url)
        if 'id' in flask.request.args:
            key = base64.b64decode(flask.request.args['id']).decode()
            if key:
                stream_info = search.search_by_key(flask.session, logger, key)
        if 'ip' in flask.request.args:
            ip = flask.request.args['ip']
            if ip:
                stream_info = search.search_by_detail(flask.session, logger, ip, '', '')
    else:
        ip = ''
        vendor = ''
        cname = ''
        if 'ip' in flask.request.args:
            ip = flask.request.args['ip']
        if 'vendor' in flask.request.args:
            vendor = flask.request.args['vendor']
        if 'cname' in flask.request.args:
            cname = base64.b64decode(flask.request.args['cname']).decode()
        stream_info = search.search_by_detail(flask.session, logger, ip, vendor, cname)

    if stream_info is not None:
        message['status'] = 'success'
        message['stream'] = stream_info
    return json.dumps(message)

