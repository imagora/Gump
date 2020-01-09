# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import copy


STREAM_INFO = {
    'id': '',
    'vendor': '',
    'cname': '',
    'uid': '',
    'ip': '',
    'url': '',
    'play_url': '',
    'call_id': '',
    'prev': '',
    'next': '',
}


def generate_stream_info(key):
    stream_info = copy.deepcopy(STREAM_INFO)
    stream_info['id'] = key
    return stream_info
