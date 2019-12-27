# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import json
import http.client


def user_info(request, logger):
    remote = request.remote_addr
    try:
        if 'Accesstoken' not in request.headers:
            logger.info('[auth] {} not found access token from request'.format(remote))
            return None

        token = request.headers['Accesstoken']
        conn = http.client.HTTPSConnection('oauth.agoralab.co')
        conn.request('GET', '/api/userInfo',
                     headers={'authorization': 'Bearer ' + token})
        user_info_res = conn.getresponse()
        if user_info_res.code != 200:
            logger.warn('[auth] {} get user info failed, code: {}'.format(remote, user_info_res.code))
            return None

        user = json.loads(user_info_res.read().decode())
        return user['email']
    except Exception as e:
        logger.info('[auth] {} auth error: {}'.format(remote, repr(e)))
        return None

