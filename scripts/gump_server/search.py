# -*- coding: utf-8 -*-
__author__ = "winking324@gmail.com"
__copyright__ = "Copyright (c) 2019 winking324"


import uuid
import rules
import online
import stream_info


def search_v1_v2(logger, vid, cname):
    params = {
        'vid': vid
    }
    if cname != '':
        params['cname'] = cname

    return online.online_list_streams_by_key(logger, params)


def search_v3(logger, ip, vendor):
    params = {
        'app': 'mix_streaming',
        'indexKeyObject': '',
        'indexMemberObject': 'worker',
        'indexType': 'sortedSet',
        'indexValue': 'create',
        'indexKey': '',
        'cursor': 0
    }
    if ip != '':
        logger.info('[search]v3 by ip {}'.format(ip))
        params['indexKeyObject'] = 'ip'
        params['indexKey'] = ip
    else:
        logger.info('[search]v3 by vendor {}'.format(vendor))
        params['indexKeyObject'] = 'vendor'
        params['indexKey'] = vendor

    return online.online_scan_index_members(logger, params)


def search_by_url(logger, url):
    if url == '':
        return None

    play_url = rules.convert_to_play_stream(logger, url)
    if play_url is None:
        return None

    stream_key = str(stream_info.KEY_STREAM) + ':' + ''.join(str(uuid.uuid1()).split('-'))
    stream = stream_info.generate_stream_info(logger, stream_key)
    stream['url'] = url
    stream['play_url'] = play_url
    return stream


def search_by_detail(session, logger, ip, vendor, cname):
    if vendor == '' and ip == '':
        logger.info('[search]vendor and ip both empty')
        return None

    key_type = stream_info.KEY_V1_V2
    try:
        vid = int(vendor)
        session['members'] = search_v1_v2(logger, vid, cname)
    except:
        key_type = stream_info.KEY_V3
        session['members'] = search_v3(logger, ip, vendor)

    members = session.get('members')
    if len(members) == 0:
        return None

    member = members[0]
    key = str(key_type) + ':' + str(member['vid']) + ';' + \
        member['cname'] + ';' + str(member['uid'])
    return stream_info.generate_stream_info(logger, key, members)


def search_by_key(session, logger, key):
    key_split = key.split(':')
    key_type = int(key_split[0])

    if key_type == stream_info.KEY_STREAM:
        logger.info('[search]key type 0 not supported, ignore')
        return None

    members = session.get('members')
    if members is None or len(members) == 0:
        logger.info('[search]has no members in session')
        return None

    logger.info('[search]key type: {}'.format(key_type))
    info_split = (':'.join(key_split[1:])).split(';')
    if len(info_split) != 3:
        logger.info('[search]split key {} failed'.format(key))
        return None

    vendor = info_split[0]
    cname = info_split[1]
    uid = info_split[2]
    for i in range(len(members)):
        member = members[i]
        if member['cname'] != cname:
            continue

        if str(member['uid']) != uid:
            continue

        if str(member['vid']) != vendor:
            continue
        return stream_info.generate_stream_info(logger, key, members, i)




