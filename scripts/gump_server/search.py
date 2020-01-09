# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import uuid
import rules
import stream_info


def search_v1_v2(vid, cname, uid):
    BASE_URL = 'http://online.agoralab.co:12630/v1/online/stream/listStreamsByKey?vid={}&cname={}'
    query_url = BASE_URL.format(vid, cname)
    return None


def search_v3(vendor_key, cname, uid, url):
    BASE_URL = 'http://online.agoralab.co:12630/v1/online/uap/scanIndexMembers'
    return None


def search_by_url(logger, url):
    if url == '':
        return None

    play_url = rules.convert_to_play_stream(logger, url)
    if play_url is None:
        return None

    stream_key = '0:' + ''.join(str(uuid.uuid1()).split('-'))
    stream = stream_info.generate_stream_info(stream_key)
    stream['url'] = url
    stream['play_url'] = play_url
    return stream


def search_by_key(logger, key):
    key_split = key.split(':')
    key_type = int(key_split[0])
    if key_type == 0:
        logger.info('search by key type 0 not supported, ignore')
        return None
    elif key_type == 1:
        logger.info('search by key ver 1 and ver 2 stream')
        info_split = ':'.join(key_split[1:]).split(';')
        if len(info_split) != 3:
            logger.info('split key {} failed'.format(key))
            return None
        return search_v1_v2(info_split[0], info_split[1], info_split[2])
    elif key_type == 2:
        logger.info('search by key ver 3 stream')
    elif key_type == 3:
        logger.info('search by key type server ip')
    else:
        logger.info('search by key type {} error'.format(key_split[0]))
    return None
