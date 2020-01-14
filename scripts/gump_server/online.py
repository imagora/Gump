# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import flask
import requests


def online_scan_index_members(logger, params):
    def request_online_page(request_params):
        url = flask.current_app.config['ONLINE_SCAN_INDEX_MEMBERS_URL']
        online_data = requests.get(url, request_params)
        logger.info('[online]scan index members: {}'.format(online_data.url))
        if online_data.status_code == 200:
            logger.warn('[online]scan index members parse data failed, code: {}'.format(online_data.status_code))
            return online_data.json()
        return None

    members = []
    try:
        cursor = 0
        while True:
            params['cursor'] = cursor
            data = request_online_page(params)
            if data is None or data['success'] is not True or 'data' not in data:
                logger.warn('[online]scan index members parse data failed')
                break

            data = data['data']
            if 'members' not in data:
                logger.warn('[online]scan index members parse data failed, no members')
                break
            members.extend(data['members'])

            if 'cursor' not in data or data['cursor'] == 0:
                break

            cursor = data['cursor']
    except Exception as e:
        logger.error('[online]scan index members failed: {}'.format(repr(e)))

    logger.info('[online]scan index members size: {}'.format(len(members)))
    return members


def online_list_streams_by_key(logger, params):
    members = []
    try:
        url = flask.current_app.config['ONLINE_LIST_STREAMS_BY_KEY_URL']
        online_data = requests.get(url, params)
        logger.info('[online]list stream by key: {}'.format(online_data.url))
        if online_data.status_code != 200:
            logger.warn('[online]list stream by key failed, code: {}'.format(online_data.status_code))
            return members

        data = online_data.json()
        if data is None or data['success'] is not True or 'data' not in data:
            logger.warn('[online]list stream by key parse data failed')
            return members

        data = data['data']
        if 'streams' in data:
            members = data['streams']
    except Exception as e:
        logger.error('[online]list stream by key failed: {}'.format(repr(e)))

    logger.info('[online]list stream by key size: {}'.format(len(members)))
    return members
