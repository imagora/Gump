# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import json
import flask
import http.client

import db


bp = flask.Blueprint('config', __name__, url_prefix='/gump')


@bp.route('/update', methods=('GET',))
def update():
    message = {'status': 'success'}

    try:
        if 'Accesstoken' not in flask.request.headers:
            message['status'] = 'Accesstoken not set in header'
            return json.dumps(message)
        token = flask.request.headers['Accesstoken']
        conn = http.client.HTTPSConnection('oauth.agoralab.co')
        conn.request('GET', '/api/userInfo',
                     headers={'authorization': 'Bearer ' + token})
        user_info_res = conn.getresponse()
        if user_info_res.code != 200:
            message['status'] = 'get user info failed, code: ' + \
                                str(user_info_res.code)
            return json.dumps(message)

        user_info = json.loads(user_info_res.read().decode())
        flask.current_app.logger.info(
            'user {} req update'.format(user_info['email']))
    except Exception as e:
        flask.current_app.logger.info('auth error: {}'.format(repr(e)))
        message['status'] = 'auth error, please input username'
        return json.dumps(message)

    database = db.get_db()
    rules = {}
    for key, regex, url in database.execute('SELECT * FROM rules'):
        rules[key] = {'re': regex, 'url': url}

    online_url = flask.current_app.config['ONLINE_URL']
    tracer_url = flask.current_app.config['TRACER_URL']
    message['rules'] = rules
    message['online_url'] = online_url
    message['tracer_url'] = tracer_url
    return json.dumps(message)
