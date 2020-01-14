# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import copy
import rules


KEY_STREAM = 0
KEY_V1_V2 = 1
KEY_V3 = 2


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


def generate_stream_info(logger, key, members=None, index=0):
    key_split = key.split(':')
    key_type = int(key_split[0])

    stream_info = copy.deepcopy(STREAM_INFO)
    stream_info['id'] = key

    if members and len(members) > index:
        member = members[index]
        stream_info['vendor'] = str(member['vid'])
        stream_info['cname'] = member['cname']
        stream_info['uid'] = str(member['uid'])
        stream_info['url'] = member['url']
        play_url = rules.convert_to_play_stream(logger, stream_info['url'])
        if play_url is not None:
            stream_info['play_url'] = play_url

        if key_type == KEY_V1_V2:
            stream_info['ip'] = member['node_ip']
        else:
            stream_info['ip'] = member['ip']

        if len(members) > 1:
            prev_member = members[index - 1]
            next_member = members[index + 1]
            stream_info['prev'] = str(key_type) + ':' + str(prev_member['vid']) + ';' + \
                prev_member['cname'] + ';' + str(prev_member['uid'])
            stream_info['next'] = str(key_type) + ':' + str(next_member['vid']) + ';' + \
                next_member['cname'] + ';' + str(next_member['uid'])
        else:
            stream_info['prev'] = ''
            stream_info['next'] = ''

    return stream_info

